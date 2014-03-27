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
    out=$OutDir"/"${i/.rad/};
    mkdir $out
	 COUNTER=1
         (while [  $COUNTER -lt 21 ]; do
             let COUNTER=COUNTER+1 
             ./../algorithm/own+hc/main $count < ${i:2} >> ${out};
             echo $out"/"${i};
         done)
    done)
