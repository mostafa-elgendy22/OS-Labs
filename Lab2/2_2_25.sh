#!bin/bash

function CheckPattern
{
	str=$1
	str=`tr '[A-Z]' '[a-z]' <<< $str`
	n=${#str}
	isPattern=0
	pattern=''
	for i in `seq 1 1 $(expr $n / 2)`
	do
		if [ `expr $n % $i` == 0 ]
		then
			temp=`expr $n / $i`
			for j in `seq 0 1 $(expr $temp - 1)`
			do
				pattern="$pattern${str:0:i}"
			done
			if [ $str == $pattern ]
			then
				isPattern=1
			fi
			pattern=''
		fi 
	done
	echo $isPattern
}
