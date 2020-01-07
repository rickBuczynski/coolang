[![Build Status](https://travis-ci.com/rickBuczynski/coolang.svg?branch=master)](https://travis-ci.com/rickBuczynski/coolang)

Coolang is a COOL (Classroom Object Oriented Language) compiler with LLVM backend.

## Building Coolang

### Prerequisites

#### Linux

- CMake 3.10 (or more recent)
- a C++17 compiler
- LLVM 8 and Clang 8
    - see https://apt.llvm.org/
    - install the packages: `clang-8`, `llvm-8`, `llvm-8-dev`
- also install the `libedit-dev` package since llvm fails to link if it's not installed

#### Windows

- CMake 3.10 (or more recent)
- Visual studio 2017 (or more recent)
- LLVM 8 and Clang 8
     - Options for how to get Clang 8
          - Download the installer from here and run http://releases.llvm.org/download.html
               - Confusingly, the installer is called LLVM-8.0.1-... but it doesn't install the LLVM development libs, we'll do that after
          - Or if you use the chocolatey package manager:
               - ```choco install llvm --version=8.0.1```
               - this also calls itself LLVM but doesn't include development libs
     - Options for how to get LLVM 8 development libs
          - Download a pre-built binary from here: https://github.com/vovkos/llvm-package-windows/releases/tag/llvm-8.0.0
               - note these are built for Visual Studio 15 and 14 (aka 2017 and 2015) so they might not work for Visual Studio 2019 (aka 16)
               - TODO test if these work on 2019
               - Choose one to download based on your Visual Studio version and compile options (x86, and msvcrt-dbg are default)
               - TODO test if -dbg supports both debug and release builds
               - Extract to somewhere CMake will look for packages ("C:\Program Files (x86)" or "C:\Program Files" by default)
          - Or install LLVM using vcpkg
               - Note LLVM is huge and vcpkg will take ~1hour to build and install it even on a fast machine
               - If vcpkg has updated their LLVM version at head to 9 or later you may need to reset your vcpkg client to a previous version that still uses llvm 8, see https://github.com/microsoft/vcpkg/commits/master/ports/llvm
               - later when running CMake you'll need to set your `-DCMAKE_TOOLCHAIN_FILE` as usual for vcpkg
          - Or build from source and install
               - Same warning as with using vcpkg this will probably take even longer than 1hour

### Build and run tests

```
mkdir build
cd build
cmake ..
cmake --build .
ctest --verbose
```

This is not an official Google product. 
