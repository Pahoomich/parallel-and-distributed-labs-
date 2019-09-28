#!/bin/bash
for (( var=32; var <= 8800; var += 877 ))
do
./lab1-seq $var
done
