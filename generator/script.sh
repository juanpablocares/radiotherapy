#!/bin/bash

S=5
MainDir=/home/jp/Documents/radiotherapy/generator/
OutDir=output

cd $MainDir

ls test | \
    (while read i;
    do 
out=$OutDir"/"$i
mkdir $out
    done)
    
ls test | \
    (while read i;
    do
for seed in $(seq 1 $S)
do
out=$OutDir"/"${i}"/"${i}"_"${seed}".rad";
./main $seed < "test/"$i > $out
done
    done)