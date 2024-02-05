#!/bin/bash

if [ "$1" == "rush" ]; then
    # Compile the rush.c file and run the executable
    cd src
    make
    ./rush
elif [ "$1" == "test" ]; then
    # Compile the test.c file and run the executable
    gcc -o test test.c
    ./test
else
    echo "Invalid argument. Please use either 'rush' or 'test'."
fi