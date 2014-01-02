#!/bin/bash

MainDir=/home/jp/Documents/tesis/radiotherapy/radiotherapy/tests
OutDir=/home/jp/Documents/tesis/radiotherapy/radiotherapy/results/own+hc

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
    ./../algorithm/own+HC/main 1 100 50 1000 < ${i:2} > ${out};
    done)
