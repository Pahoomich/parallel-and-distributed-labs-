#!/bin/bash
echo -e "dynamic \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_dynamic_def $var
done

echo -e "dynamic 1 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_dynamic $var
done

echo -e "dynamic 10 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_dynamic $var 10
done

echo -e "dynamic 100 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_dynamic $var 100
done

echo -e "dynamic 1000 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_dynamic $var 1000
done
