#!/bin/bash
echo -e "Hello my name is lab2-seq \v"
for (( var=4; var <= 20; var += 1 ))
do
./lab2-seq 31000 $var
done
