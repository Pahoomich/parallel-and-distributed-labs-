#!/bin/bash
echo -e "guided \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_guided_def $var
done

echo -e "guided 1 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_guided $var
done

echo -e "guided 10 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_guided $var 10
done

echo -e "guided 100 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_guided $var 100
done

echo -e "guided 1000 \v"
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp_guided $var 1000
done
