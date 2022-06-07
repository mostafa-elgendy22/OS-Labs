#!/bin/bash 


function FindPrime
{
	N=$1
	X=$2
	len=${#X}
	if [ $len = 0 ]
	then
		X=$N
		N=3
	fi
	length=${#X}
	size=$(($length-$N))
	count=0
	max=0
	for i in $( seq 0 1 $size )
	do
		num=${X:$i:$N}
		isPrime=1
		if [ $num = 1 ]
		then
			isPrime=0
		fi

		r=`expr $num - 1`
		for j in $( seq 2 1 $r )
		do
			temp=`expr $num % $j`
			if [ $temp = 0 ]
			then
				isPrime=0
			fi
		done
		
		if [ $isPrime = 1 ]
		then
			count=`expr $count + 1`
			if [ $num -gt $max ]
			then
				max=$num	
			fi
		fi 
	done 
	echo $count $max
}

