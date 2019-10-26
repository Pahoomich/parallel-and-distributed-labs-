#!/bin/bash
echo -e "Hello my name is lab2-seq \v"
export OMP_SCHEDULE=guided
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp $var
done

export OMP_SCHEDULE=guided,1
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp $var
done

export OMP_SCHEDULE=guided,10
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp $var
done

export OMP_SCHEDULE=guided,100
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp $var
done

export OMP_SCHEDULE=guided,1000
for (( var=700; var <= 31000; var += 3030 ))
do
./lab3_omp $var
done
