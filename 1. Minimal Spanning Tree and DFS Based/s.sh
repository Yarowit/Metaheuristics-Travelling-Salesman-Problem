#!/usr/bin/bash
make clean
# make cleanG
make

for file in $(ls data); do
    echo "$file"
    ./main $file
    echo
done