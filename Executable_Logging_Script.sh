#!/bin/bash

(($#)) || echo "Usage: $0 [-hudea] file_name";

date=$(date +"%Y%m%dT%H%M%SZ");
unique=$(cat /dev/urandom|tr -dc 'a-zA-Z0-9'|fold -w 8|head -n 1);
count_u=0;
count_d=0;

file_name="suhas_hw3.log";	

count_help=0;
count_author=0;
count_unique=0;
count_date=0;
count_error=0;
count_a=0;
count_h=0;


function_help()
{
	echo "Usage: $0 [-hudea] file_name";
	echo -e "\t -h: Help Information for the program is provided";
	echo -e "\t -u: Use a unique, temporary file";
	echo -e "\t -d: Print the date and time in the file name";
	echo -e "\t -e: Log error output to the file as well";
	echo -e "\t -a: Print Authorship information and terminate the program";
	
	if [ $count_help -lt 1 ]; 
	then
		if [ $count_d -lt 1 ] && [ $count_u -lt 1 ]; then
			exit;
		elif [ $count_d -gt 0 ] && [ $count_u -lt 1 ]; then
			function_date;
		elif [ $count_d -lt 1 ] && [ $count_u -gt 0 ]; then
			function_unique;
		else
			return;					
		fi
	else
		exit;
	fi
}


function_author()
{
		
	echo -e "\nAuthorship Information:\tWritten by Suhas Murthy\n";
	if [ $count_author -lt 1 ]; 
	then
		if [ $count_d -lt 1 ] && [ $count_u -lt 1 ]; then
			exit;
		elif [ $count_d -gt 0 ] && [ $count_u -lt 1 ]; then
			function_date;
		elif [ $count_d -lt 1 ] && [ $count_u -gt 0 ]; then
			function_unique;
		else
			return;					
		fi
	else
		exit;
	fi
}


function_unique()
{	
	if [ $count_unique -lt 1 ]; 
	then	
		count_u=1;
		if [ $count_d -eq 1 ];
		then
			file_name=hw3.$unique.$date.log;			
		else
			file_name=hw3.$unique.log;
		fi	
		count_d=2;
	
	else
		return;
	fi	
}



function_date()
{	

	if [ $count_date -lt 1 ]; 
	then
		count_d=1;	
		if [ $count_u -eq 1 ];
		then
			file_name=hw3.$unique.$date.log;
		else
			file_name=hw3.$date.log;
		fi	
		count_u=2;
	else
		return;
	fi
}



function_error()
{
	if [ $count_error -ge 1 ]; 
	then
		return;
	fi
}


while getopts "hadue" opt; do
		
	case $opt in
		
		h) 	
			function_help;
			count_help=$((count_help+1))
		;;
	
		a) 	
			function_author;
			count_a=1;
			count_author=$((count_author+1))	
		;;
	
		u) 
			function_unique;
			count_unique=$((count_unique+1))			
		;;

		d) 
			function_date;
			count_date=$((count_date+1))
		
		;;

		e) 	
			function_error;	
			count_error=$((count_error+1))				
		;; 

		*) 
			echo "Invalid Option $OPTARG entered";
			exit;
		;;
	esac

done

shift $(($OPTIND -1))
y=$1;
x=$(($OPTIND));

if [ ! -f $1 ];
then
	echo "the input file $1 does not exist "
	exit 1	
fi

if [ ! -x $1 ];
then
	echo "the input file $1 is not an executable file"
	exit 1
fi

if [ $y ]; then
	echo "The file is named  $file_name";
	echo `$y 2>&1` >>$file_name;
fi


