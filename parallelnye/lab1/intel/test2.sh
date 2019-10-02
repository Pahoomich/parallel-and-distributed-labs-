#!/bin/bash
echo -e "Hello my name is lab1-seq \v"
for (( var=1500; var <= 23500; var += 2200 ))
do
./lab1_icc $var
done

echo -e "Hello my name is lab1-par-1 \v"
for (( var=1500; var <= 23500; var += 2200 ))
do
./lab1_icc_par_1 $var
done

echo -e "Hello my name is lab1-par-2 \v"
for (( var=1500; var <= 23500; var += 2200 ))
do
./lab1_icc_par_2 $var
done

echo -e "Hello my name is lab1-par-4 \v"
for (( var=1500; var <= 23500; var += 2200 ))
do
./lab1_icc_par_4 $var
done

echo -e "Hello my name is lab1-par-6 \v"
for (( var=1500; var <= 23500; var += 2200 ))
do
./lab1_icc_par_6 $var
done
