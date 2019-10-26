#!/bin/bash
echo -e "Hello my name is lab2-seq \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp $var
done
