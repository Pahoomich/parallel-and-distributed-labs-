#!/bin/bash
echo -e "Hello my name is lab4 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab4 $var
done
