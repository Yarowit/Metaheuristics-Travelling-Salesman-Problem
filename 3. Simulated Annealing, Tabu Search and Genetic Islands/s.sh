#!/usr/bin/bash
# make clean
# make cleanG
make

for file in $(ls big); do
    echo "$file"
    time ./main big $file >> results
    echo
done
# echo BIG
# for file in $(ls big); do
#     echo "$file"
#     ./main big $file
#     echo
# done