#!/bin/sh

echo

echo -n "File Name: "
read file						

echo -n "String: "
read str							

occurance=$(grep -o $str $file | wc -l)
if [ $occurance == 0 ]						
then
	echo "String not found \"$str\" in File \"$file\""
	exit
fi
echo
echo "Number of presence is: $occurance"		

arr=(`grep -o -n $str $file | cut -d : -f 1 | uniq -c`)

echo

echo -e "    Line Number       Number of presence   "

echo

for (( i=0; i<${#arr[@]}; i+=2 ))				
do
    echo -e "\t${arr[$i+1]}\t\t${arr[$i]}\t"
done

echo
