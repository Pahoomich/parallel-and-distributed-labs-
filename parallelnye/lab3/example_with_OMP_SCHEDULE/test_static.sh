#!/bin/bash
echo -e "Hello my name is lab2-seq \v"
export OMP_SCHEDULE=static
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp $var
done

export OMP_SCHEDULE=static,1
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp $var
done

export OMP_SCHEDULE=static,10
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp $var
done

export OMP_SCHEDULE=static,100
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp $var
done

export OMP_SCHEDULE=static,1000
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp $var
done
