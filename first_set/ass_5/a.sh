#!/bin/sh

echo

echo -n "File Name : "
read file                                       	

echo -n "First String: "
read str                                        		

occurance=$(grep -o $str $file | wc -l)				
if [ $occurance == 0 ]
then
    	echo "String does not exist \"$str\" in File \"$file\""
        exit
fi

echo -n "Second String: "
read newstr			

sed -i 's/\<'"$str"'\>/'"$newstr"'/g' $file

