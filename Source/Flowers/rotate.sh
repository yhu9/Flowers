#!/bin/bash

clear

num=81
shapename="hape2"

	for b in `seq 0 1`;
	do
		for c in `seq 0 9`;
		do
			str1=""
			str2=""
			str1+="S" && str1+=$shapename && str1+=$a && str1+="/s" && str1+=$shapename && str1+=$a && str1+="-0" && str1+=$b && str1+=$c &&str1+=".ppm"
			str2+="S" && str2+=$shapename && str2+=$a && str2+="/s" && str2+=$shapename && str2+=$a && str2+="-0" && str2+=$num && str2+=".ppm"
			num=$(($num + 1))

			convert $str1 -rotate 135 $str2
			echo $str1
			echo $str2
		done
	done
