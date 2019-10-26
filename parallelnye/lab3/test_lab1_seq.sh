#!/bin/bash
echo -e "Hello my name is lab1-seq \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab1_seq $var
done
