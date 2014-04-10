#!/bin/bash

MainDir=/home/jp/Documents/tesis/radiotherapy/radiotherapy/tests
OutDir=/home/jp/Documents/tesis/radiotherapy/radiotherapy/results/own+hc

cd $MainDir

ls | \
    (while read i;
    do 
    out=$OutDir"/"${i};
    mkdir $out;
    done)
        
find -name "*.rad" -print0 | \
    (while read -d $'\0' i;
    do
    i=${i/"./test"/"test"}
    mkdir $OutDir"/"${i/.rad/};
    COUNTER=1
         (while [  $COUNTER -le 20 ]; do
	     out=$OutDir"/"${i/.rad/};
	     out=$out"/out_"$COUNTER;
	     ./../algorithm/own+HC/main $COUNTER 100 50 1000 < ${i} > ${out};
             let COUNTER=COUNTER+1
         done)
    done)
