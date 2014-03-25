#!/bin/bash

MainDir=/home/jp/Documents/tesis/radiotherapy/radiotherapy/results/own+hc
OutDir=/home/jp/Documents/tesis/radiotherapy/radiotherapy/validator/output

cd $MainDir

ls | \
    (while read i;
    do 
out=$OutDir"/"$i
    mkdir $out;
    done)

find -name "*.out" -print0 | \
    (while read -d $'\0' i;
    do
out=$OutDir"/"${i/out/est};
    ./../../validator/main < ${i:2} > ${out};
    done)
