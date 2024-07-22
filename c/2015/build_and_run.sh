#!/bin/bash

TARGET=$1

if [ -z $TARGET ]; then
    echo "Target is required"
    exit 1
fi

cmake --build . --target $TARGET --clean-first

if [ $? -eq 0 ]; then
    ./bin/$TARGET
else
    echo "Build failed!"
fi
