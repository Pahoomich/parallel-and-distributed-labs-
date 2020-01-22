#!/bin/bash
echo -e "Hello my name is lab6 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab6_ocl.o $var
done
