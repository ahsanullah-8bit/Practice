/*
    To compile this file, you need to have the ODB compiler installed. You can download the archives
    from https://codesynthesis.com/download/odb/2.5.0 for your platform.

    Was compiled using:
    odb -d sqlite -q -s -o <dst_dir> --std c++17 --schema-format embedded
            -I<full_path_to_libodb>/include "models.h"

*/

#pragma once

#include <string>
#include <odb/core.hxx>

#pragma db model version(1, 2)

#pragma db object
class User {
public:
    #pragma db id auto
    unsigned long id;
    std::string name;

    User() {};
    User(std::string name) : name(std::move(name)) {}
};

#pragma db object
class Task {
public:
    #pragma db id auto
    unsigned long id;
    std::string description;

    #pragma db added(2)
    int priority;

    Task() {};
    Task(std::string description, int priority) : description(std::move(description)), priority(priority) {}
};