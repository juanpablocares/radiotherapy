#!/bin/bash

MainDir=/home/jp/Documents/radiotherapy/tests
OutDir=/home/jp/Documents/radiotherapy/results/own

cd $MainDir

ls | \
    (while read i;
    do 
out=$OutDir"/"$i
    mkdir $out;
    done)

find -name "*.rad" -print0 | \
    (while read -d $'\0' i;
    do
out=$OutDir"/"${i/rad/out};
    ./../algorithm/own/main 1 < ${i:2} > ${out};
    done)
