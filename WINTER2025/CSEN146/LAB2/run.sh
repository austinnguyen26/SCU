#!/bin/bash

echo "----------Make--------"
make

echo
echo "----------Creating Files----------"
for i in {1..3}; do
    size=$(( (RANDOM % 10 + 1) * 100000 ))
    cat /dev/random | head -c $size > "src_$i.dat"
done

echo "----------Running Multi-threaded Copy----------"
./1

echo "----------Cleaning Up----------"
ls -l 

echo "---------Persistent---------"
./2
make clean
rm -f src_*.dat dst_*.dat


echo "All tasks completed."
