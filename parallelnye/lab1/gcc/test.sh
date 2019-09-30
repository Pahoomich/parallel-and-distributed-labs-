#!/bin/bash
for (( var=700; var <= 31000; var += 3030 ))
do
./lab1-seq $var
done
