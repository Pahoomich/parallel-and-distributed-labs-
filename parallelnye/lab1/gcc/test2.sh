#!/bin/bash
echo -e "Hello my name is lab1-seq \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab1-seq $var
done

echo -e "Hello my name is lab1-par-1 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab1-par-1 $var
done

echo -e "Hello my name is lab1-par-2 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab1-par-2 $var
done

echo -e "Hello my name is lab1-par-4 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab1-par-4 $var
done

echo -e "Hello my name is lab1-par-6 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab1-par-6 $var
done
