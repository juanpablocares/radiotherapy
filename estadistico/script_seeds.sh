#!/bin/bash

MainDir=/home/jp/Documents/tesis/radiotherapy/radiotherapy/results/own+hc
OutDir=/home/jp/Documents/tesis/radiotherapy/radiotherapy/estadistico/output

cd $MainDir

ls | \
    (while read i;
    do 
    out=$OutDir"/"${i};
    mkdir -p $out;
    echo $out;
    done)
        
find -name "out*" -print0 | \
    (while read -d $'\0' i;
    do
    
    i=${i/"./test"/"test"};
    ejecutar=$i
    i=${i/"/"out*/""};
    mkdir -p "../../estadistico/output/"$i;
    ./../../estadistico/main < $ejecutar > "../../estadistico/output/"$ejecutar
    done)
