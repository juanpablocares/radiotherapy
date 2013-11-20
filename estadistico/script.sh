#!/bin/bash

MainDir=/home/jp/Documents/radiotherapy/results/jit
OutDir=/home/jp/Documents/radiotherapy/estadistico/output

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
    ./../../estadistico/main < ${i:2} > ${out};
    done)