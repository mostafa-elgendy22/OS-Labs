#! /bin/bash

#1
rm -r -f ~/Desktop/Lab1

#2
mkdir ~/Desktop/Lab1

#3
cp ~/Desktop/words.txt ~/Desktop/numbers.txt ~/Desktop/Lab1

#4
paste ~/Desktop/Lab1/words.txt ~/Desktop/Lab1/numbers.txt > ~/Desktop/Lab1/MergedContent.txt

#5
head -n 3 ~/Desktop/Lab1/MergedContent.txt

#6
sort ~/Desktop/Lab1/MergedContent.txt > ~/Desktop/Lab1/SortedMergedContent.txt

#7
echo 'The sorted file is :'

#8
cat ~/Desktop/Lab1/SortedMergedContent.txt

#9
chmod a-r ~/Desktop/Lab1/SortedMergedContent.txt

#10
sort ~/Desktop/Lab1/MergedContent.txt | uniq

#11
cat ~/Desktop/Lab1/SortedMergedContent.txt | tr '[a-z]' '[A-Z]' 

#12
echo 'Nobody can read the SortedMergedContent.txt file because after the execution of command number 9, The read permissions of the file changed so that no user can read it'

#13
chmod a+r ~/Desktop/Lab1/SortedMergedContent.txt
cat ~/Desktop/Lab1/SortedMergedContent.txt | tr '[a-z]' '[A-Z]' 

#14
grep -n "^w.*[0-9]$" ~/Desktop/Lab1/MergedContent.txt

#15
cat ~/Desktop/Lab1/MergedContent.txt | tr 'i' 'o' > ~/Desktop/Lab1/NewMergedContent.txt

#16
paste ~/Desktop/Lab1/MergedContent.txt ~/Desktop/Lab1/NewMergedContent.txt
