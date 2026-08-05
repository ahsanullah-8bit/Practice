import os
import sys
import json
import platform
import shutil
import subprocess
from argparse import ArgumentParser

VCPKG_VERSION = "2026.06.01"
REGISTRY_FILE = "registry.json"

def load_registry():
    try:
        with open(REGISTRY_FILE, "r", encoding="utf-8") as file:
            data = json.load(file)
            return data
    except json.JSONDecodeError as error:
        print(f"Error: '{REGISTRY_FILE}' contains invalid JSON formatting.")
        print(f"Details: {error}")
        return {}
    
def sorted_projects(projects: list):
    return sorted(
        projects, 
        key=lambda x: (x.get("language", "").lower(), x.get("name", "").lower())
    )
    
def elide(text, max_width, add_ellipsis=True):
    text = str(text)
    if len(text) <= max_width:
        return text

    if add_ellipsis and max_width > 3:
        return text[:max_width - 3] + "..."
    return text[:max_width]

def resolve_targets(user_input, sorted_projects):
    cleaned_input = user_input.strip().lower()

    if cleaned_input == "all":
        return sorted_projects

    selected_projects = []
    tokens = [t.strip() for t in cleaned_input.split(",") if t.strip()]

    for token in tokens:
        # an id (e.g., "2")
        if token.isdigit():
            idx = int(token) - 1
            if 0 <= idx < len(sorted_projects):
                if sorted_projects[idx] not in selected_projects:
                    selected_projects.append(sorted_projects[idx])
            else:
                print(f"Warning: Index '{idx}' is out of range (0 to {len(sorted_projects) - 1}). Skipping.")
        
        # string name (e.g., "ocr_diff_bin")
        else:
            found = False
            for project in sorted_projects:
                if project.get("name", "").lower() == token:
                    if project not in selected_projects:
                        selected_projects.append(project)
                    found = True
                    break
            if not found:
                print(f"Warning: Target '{token}' not recognized by name or index. Skipping.")

    return selected_projects

def resolve_dependencies(targets: list, dependencies: list):
    requested_names = {
        name.lower()
        for target in targets
        for name in target.get("dependencies", [])
    }

    final_deps = [
        dep for dep in dependencies
        if dep.get("name", "").lower() in requested_names
    ]

    found_names = { dep.get("name", "").lower() for dep in final_deps }
    for name in requested_names:
        if name not in found_names:
            print(f"Warning: Dependency '{name}' not recognized by name. Build may fail.")
    
    return final_deps

def write_vcpkg_manifest(manifest: dict, name: str = "vcpkg.json"):
    try:
        with open(name, "w", encoding="utf-8") as file:
            json.dump(manifest, file, indent=4, sort_keys=False)
    except json.JSONDecodeError as error:
        print(f"Error: '{REGISTRY_FILE}' contains invalid JSON formatting.")
        print(f"Details: {error}")
        return {}

def bootstrap_vcpkg() -> str:
    subprocess.run([
        "git", "clone", 
        "--branch", VCPKG_VERSION, 
        "--depth", "1", 
        "https://github.com/microsoft/vcpkg.git"
    ], check=True)

    if sys.platform == "win32":
        bootstrap_cmd = ["bootstrap-vcpkg.bat"]
    else:
        bootstrap_cmd = ["./bootstrap-vcpkg.sh"]

    print("Bootstrapping vcpkg...")
    subprocess.run(
        bootstrap_cmd, 
        cwd="vcpkg",
        check=True
    )
    return "vcpkg"

def list_targets():
    registry = load_registry()
    projects = sorted_projects(registry.get("projects", []))
    dependencies = registry.get("dependencies", [])

    TOTAL_SEC_CHARS = 105

    # 1. Print projects Section
    print("\n" + "=" * TOTAL_SEC_CHARS)
    print(f" {'REGISTERED PROJECTS':^100}")
    print("=" * TOTAL_SEC_CHARS)
    print(f" {'ID':<4} | {'NAME':<20} | {'LANG':<6} | {'PATH':<30} | {'REQUIRES'}")
    print("-" * TOTAL_SEC_CHARS)
    
    i = 1
    for project in projects:
        name = elide(project.get("name", "N/A"), 20)
        lang = elide(project.get("language", "N/A").upper(), 6, add_ellipsis=False)
        path = elide(project.get("path", "N/A"), 30)
        
        deps = project.get("dependencies", [])
        deps_str = ", ".join(deps) if deps else "-"
        deps_str = elide(deps_str, 30)
        
        print(f" {i:<4} | {name:<20} | {lang:<6} | {path:<30} | {deps_str}")
        i += 1

def build_target(targets_ids: str, build_type: str, clear: bool = False, clean: bool = False):
    registry = load_registry()
    projects = sorted_projects(registry.get("projects", []))

    targets = resolve_targets(targets_ids, projects)
    if not targets:
        print("No valid targets selected to build.")
        sys.exit(1)

    print(f"\nSelected Targets: {", ".join([target.get("name", "") for target in targets])}")

    dependencies = resolve_dependencies(targets, registry.get("dependencies", []))

    # generate the vcpkg.json
    manifest = { "dependencies": dependencies }
    write_vcpkg_manifest(manifest, "vcpkg.json")
    
    # ensure tools
    vcpkg_root = os.environ.get("VCPKG_ROOT", "vcpkg")
    if not os.path.exists(vcpkg_root):
        vcpkg_root = bootstrap_vcpkg()
    toolchain_path = os.path.join(vcpkg_root, "scripts", "buildsystems", "vcpkg.cmake")

    build_dir = f"build/{build_type}"
    # clear
    if clear:
        os.remove(os.path.join(build_dir, "CMakeCache.txt"))
        shutil.rmtree(os.path.join(build_dir, "CMakeFiles"))

    # clean
    if clean:
        shutil.rmtree(os.path.join(build_dir))

    # run the main CMakeLists.txt with vcpkg
    configure_cmds = ["cmake", "-S", ".",
                  "-B", build_dir, 
                  "-G", "Ninja",
                  f"-DCMAKE_BUILD_TYPE:STRING={build_type}",
                  f"-DCMAKE_TOOLCHAIN_FILE={toolchain_path}",
                  "-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE",
                  "-DVCPKG_OVERLAY_PORTS=custom-ports",
                  "-DVCPKG_MANIFEST_MODE=ON",
                ]
    print(f"\n\nConfiguring: {" ".join(configure_cmds)}\n\n")
    subprocess.run(configure_cmds)

    # build
    build_cmds = ["cmake", "--build", build_dir]
    print(f"\n\nBuilding: {" ".join(build_cmds)}\n\n")
    subprocess.run(build_cmds, check=True)

def run_target(target_id: str, build_type: str):
    registry = load_registry()
    projects = sorted_projects(registry.get("projects", []))

    target_id = target_id.strip().lower()
    target_id = [t.strip() for t in target_id.split(",") if t.strip()][0]
    if not target_id.isdigit():
        print(f"Error: Target '{target_id}' not recognized by name or index. Skip running.")
        sys.exit(1)

    target = projects[int(target_id)]
    if not target:
        print("No valid targets selected to run.")
        sys.exit(1)

    build_dir = f"build/{build_type}"
    executable_dir = f"{build_dir}/{target.get("path", "")}"
    subprocess.run(
        [target.get("name", "")],
        cwd=executable_dir,
        check=True
    )


if __name__ == "__main__":
    parser = ArgumentParser("labs", description="Testing/Practice projects")

    action_group = parser.add_mutually_exclusive_group(required=True)

    action_group.add_argument("-l", "--list", action="store_true", help="Lists all possible targets/projects")
    action_group.add_argument("-b", "--build", metavar="TARGET", help="Build a specific target by name")
    # action_group.add_argument("-r", "--run", metavar="TARGET", help="Run a specific target by name, after build")
    parser.add_argument("-r", "--clear", action="store_true", help="Clear CMake Cache")
    parser.add_argument("-n", "--clean", action="store_true", help="Clean the build")
    parser.add_argument("-c", "--config", choices=["Debug", "Release", "RelWithDebInfo"], default="Debug", help="Choose the build profile (Default: Debug)")

    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(1)

    args = parser.parse_args()

    if args.list:
        list_targets()
    elif args.build and args.config:
        build_target(args.build,
                     args.config,
                     clear= True if args.clear else False,
                     clean= True if args.clean else False
                     )
    # elif args.run:
        # build_target(args.run, args.config)