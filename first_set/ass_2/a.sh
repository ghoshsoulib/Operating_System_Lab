#!/bin/bash


#count of files
FILECOUNT="$(find -maxdepth 1 -type f | wc -l)"

#count of subdirectory including current directory
DIRCOUNT="$(find -maxdepth 1 -type d | wc -l)"
let "DIRCOUNT--"

#print the answer
echo "Number of files in the current directory is:  $FILECOUNT"
echo "Number of subfolders in the current directory is:  $DIRCOUNT"

#count of total files and folders in directory and sub directory
FILES="$(find . | wc -l)"
count=$((FILES-DIRCOUNT))
let "count--"
echo "Number of files in the current directory + subdirectory is:  $count"


find . -type d -print0 | while read -d '' -r dir; do
    files=("$dir"/*)
    printf "Number of files in directory %s is %5d \n" "$dir" "${#files[@]}" 
done


echo "List of modified files in last 2 days in current folder and sub folder"
find . -type f -mtime -2 -printf "%TR %TD %p\n"
