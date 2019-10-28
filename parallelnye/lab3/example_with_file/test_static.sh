#!/bin/bash
echo -e "static 1 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_static $var
done

echo -e "static 10 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_static $var 10
done

echo -e "static 100 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_static $var 100
done

echo -e "static 1000 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_static $var 1000
done

