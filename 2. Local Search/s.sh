#!/usr/bin/bash
make clean
# make cleanG
make

for file in $(ls data); do
    echo "$file"
    ./main data $file
    echo
done
echo BIG
for file in $(ls big); do
    echo "$file"
    ./main big $file
    echo
done