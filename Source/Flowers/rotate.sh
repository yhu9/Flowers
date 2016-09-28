#!/bin/bash

clear
	for b in `seq 0 9`;
	do
		for c in `seq 0 9`;
		do
			str1=""
			str2=""
			str3=""
			str4=""
			str5=""
			str1out=""
			str2out=""
			str3out=""
			str4out=""
			str5out=""
			str1+="Shape1" && str1+=$a && str1+="/shape1" && str1+=$a && str1+="-0" && str1+=$b && str1+=$c &&str1+=".ppm"
			str2+="Shape2" && str2+=$a && str2+="/shape2" && str2+=$a && str2+="-0" && str2+=$b && str2+=$c &&str2+=".ppm"
			str3+="Shape3" && str3+=$a && str3+="/shape3" && str3+=$a && str3+="-0" && str3+=$b && str3+=$c &&str3+=".ppm"
			str4+="Shape4" && str4+=$a && str4+="/shape4" && str4+=$a && str4+="-0" && str4+=$b && str4+=$c &&str4+=".ppm"
			str5+="Shape5" && str5+=$a && str5+="/shape5" && str5+=$a && str5+="-0" && str5+=$b && str5+=$c &&str5+=".ppm"
			str1out+="Shape1" && str1out+=$a && str1out+="/shape1" && str1out+=$a && str1out+="-1" && str1out+=$b && str1out+=$c && str1out+=".ppm"
			str2out+="Shape2" && str2out+=$a && str2out+="/shape2" && str2out+=$a && str2out+="-1" && str2out+=$b && str2out+=$c && str2out+=".ppm"
			str3out+="Shape3" && str3out+=$a && str3out+="/shape3" && str3out+=$a && str3out+="-1" && str3out+=$b && str3out+=$c && str3out+=".ppm"
			str4out+="Shape4" && str4out+=$a && str4out+="/shape4" && str4out+=$a && str4out+="-1" && str4out+=$b && str4out+=$c && str4out+=".ppm"
			str5out+="Shape5" && str5out+=$a && str5out+="/shape5" && str5out+=$a && str5out+="-1" && str5out+=$b && str5out+=$c && str5out+=".ppm"

			convert $str1 -flip $str1out
			convert $str2 -flip $str2out
			convert $str3 -flip $str3out
			convert $str4 -flip $str4out
			convert $str5 -flip $str5out
			echo $str1 $str1out
			echo $str2 $str2out
			echo $str3 $str3out
			echo $str4 $str4out
			echo $str5 $str5out
		done
	done
