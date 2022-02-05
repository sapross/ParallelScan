#!/usr/bin/env bash

if [ ! -d "build" ]; then
	mkdir build
fi

cmake -DCMAKE_BUILD_TYPE=Release -DARCHSPECIFIC=true -S . -B build
cmake --build build
