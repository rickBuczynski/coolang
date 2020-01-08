# Coolang

Coolang is a compiler for the [COOL language](https://en.wikipedia.org/wiki/Cool_(programming_language)) with an LLVM backend.

[![Build Status](https://travis-ci.com/rickBuczynski/coolang.svg?branch=master)](https://travis-ci.com/rickBuczynski/coolang)

## Building Coolang

### Prerequisites

#### Linux

- CMake 3.10 (or more recent)
- a C++17 compiler
- LLVM 8 and Clang 8
    - see https://apt.llvm.org/
    - you can use the auto-install script they provide if you want, just specify version 8. But you really only need: `clang-8`, `llvm-8`, and `llvm-8-dev`
- also install the `libedit-dev` package since llvm fails to link if it's not installed

#### Windows

- CMake 3.10 (or more recent)
- Visual studio 2017 (or more recent)
- LLVM 8 and Clang 8
     - Options for how to get Clang 8
          - Download the installer from here and run http://releases.llvm.org/download.html
               - Confusingly, the installer is called LLVM-8.0.1-... but it doesn't install the LLVM development libs, we'll do that later
          - Or if you use the chocolatey package manager:
               - ```choco install llvm --version=8.0.1```
               - this also calls itself LLVM but doesn't include development libs
     - Options for how to get LLVM 8 development libs
          - Download a pre-built binary from here: https://github.com/vovkos/llvm-package-windows/releases/tag/llvm-8.0.0
               - note these are built for Visual Studio 15 and 14 (aka 2017 and 2015) so they might not work for Visual Studio 2019 (aka 16)
               - TODO test if these work on 2019
               - Choose one to download based on your Visual Studio version and compile options (x86, and msvcrt-dbg are default)
               - Extract to somewhere CMake will look for packages ("C:\Program Files (x86)" or "C:\Program Files" by default)
          - Or install LLVM using vcpkg
               - Note LLVM is huge and vcpkg will take ~1hour to build and install it even on a fast machine
               - If vcpkg has updated their LLVM version at head to 9 or later you may need to reset your vcpkg client to a previous version that still uses llvm 8, see https://github.com/microsoft/vcpkg/commits/master/ports/llvm
               - later when running CMake you'll need to set your `-DCMAKE_TOOLCHAIN_FILE` as usual for vcpkg
          - Or build from source and install
               - Same warning as with using vcpkg this will probably take even longer than 1hour

### Build and run tests

On windows if you installed a pre-built LLVM binary and it's a "release" version (i.e. doesn't end in -dbg) you'll need to use: 
- ```cmake --build . --config Release``` instead of ```cmake --build .```
- ```.\Release\coolang.exe hello.cl``` instead of ```.\Debug\coolang.exe hello.cl```

```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ ctest --verbose
```
then compile a hello world program

##### Linux
```
$ echo "class Main inherits IO { main() : Object { out_string(\"Hello, world\\n\") }; };" > hello.cl
$ ./coolang hello.cl
$ ./hello
Hello, world
```
##### Windows
```
$ [IO.File]::WriteAllLines((Join-Path -Path (Resolve-Path .) -ChildPath "hello.cl"), "class Main inherits IO { main() : Object { out_string(`"Hello, world`\n`") }; };")
$ .\Debug\coolang.exe hello.cl
$ .\hello.exe
Hello, world
```

This is not an official Google product. 
