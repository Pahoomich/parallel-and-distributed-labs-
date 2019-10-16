#!/bin/bash
echo -e "Hello my name is lab2-seq \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab2_test $var
done

echo -e "Hello my name is lab2-seq N K = 2 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab2_test $var 2
done

echo -e "Hello my name is lab2-seq N K = 3 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab2_test $var 3
done

echo -e "Hello my name is lab2-seq N K = 4 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab2_test $var 4
done
