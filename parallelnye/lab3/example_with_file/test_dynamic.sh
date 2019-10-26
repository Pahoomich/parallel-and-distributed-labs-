#!/bin/bash
echo -e "dynamic \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_dynamic $var
done

echo -e "dynamic 1 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_dynamic_1 $var
done

echo -e "dynamic 10 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_dynamic_10 $var
done

echo -e "dynamic 100 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_dynamic_100 $var
done

echo -e "dynamic 1000 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_dynamic_1000 $var
done
