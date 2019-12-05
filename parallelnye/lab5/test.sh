#!/bin/bash
echo -e "Hello my name is lab5 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab5 $var
done
