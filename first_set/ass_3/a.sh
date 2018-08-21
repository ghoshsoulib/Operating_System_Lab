#!/bin/sh

echo

files=($1 $2 $3 $4)									
strings=("printf" "scanf" "int")								


echo -e "\tFile     \t\t${strings[0]}\t\t${strings[1]}\t\t${strings[2]}\t"
echo 

for file in ${files[*]}									
do
	echo -n -e "\t$file\t"
	for string in ${strings[*]}							
	do
		occurance=$(grep -c "$string" $file)					
		echo -n -e "\t$occurance\t"						
	done
	echo
done

echo
