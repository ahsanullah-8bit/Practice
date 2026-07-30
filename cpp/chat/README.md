This is just a basic Chat application made using Asio, as a practice project. *Use it at your own risk. Because there's no security or anything at all.*

# Building
## Prerequisites
You need `CMake >= 3.8` and C++ compiler with C++23 support (or you can change the required standard, but at least C++20 is required for Asio's coroutines). Asio itself will be added as a submodule.

## Build and run
Clone the repo using
```bash
git clone --recursive https://github.com/ahsanullah-8bit/Chat.git
```
and do
```bash
cd Chat
mkdir build
cd build

cmake -S .. -B .
```

and then whatever your Generator name is, mine is `make`
```bash
make
```
and you should've your binaries.
