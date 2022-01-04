#!/usr/bin/env bash

if [ ! -d "build" ]; then
    mkdir build
fi

# if [ ! -d "cmake-3.18.4-Linux-x86_64/bin/" ]; then
#     echo "Installing recent CMake.."
#     wget https://github.com/Kitware/CMake/releases/download/v3.18.4/cmake-3.18.4-Linux-x86_64.sh
#     chmod u+x cmake-3.18.4-Linux-x86_64.sh
#     ./cmake-3.18.4-Linux-x86_64.sh --prefix=. --skip-license --include-subdir
# fi
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build build

