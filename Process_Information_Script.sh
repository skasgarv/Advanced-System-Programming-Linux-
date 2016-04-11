#!/bin/bash

(($#)) || echo -e "Usage: $0 [-hcsfe] file_name \n\t -h: Help Information for the program is provided \n\t -c: Script shall print the command line used to start the program \n\t -e: Executable Information related to process id \n\t -s: Script shall print the status information \n\t -f: Script shall print all proces id that might have a given file opened";

func_command=0;
func_execute=0;
func_status=0;
func_process=0;
func_usage=0;
count=0;
option_check="0"

OPT='';

process_check()
{
	VAR=$(ls -l /proc/$x 2>/dev/null);
	if [ -z "$VAR" ];
	then
		echo "Process $x cannot be started";
		exit;
	else	
		echo "Process $x is running";
	fi
}

check_opts()
{

if [ "$func_usage" == "1" ]; then

	echo "Usage: $0 [-hcesf] file_name";
	echo -e "\t -h: Help Information for the program is provided";
	echo -e "\t -c: Script shall print the command line used to start the program";
	echo -e "\t -e: Executable Information related to process id";
	echo -e "\t -s: Script shall print the status information";
	echo -e "\t -f: Script shall print all proces id that might have a given file opened";
exit;
fi


if [ "$func_command" == "1" ]; then

	VAR1=$(cat /proc/$x/cmdline 2>/dev/null |tr "\0" " ");
	if [ -z "$VAR1" ];
	then
		echo "Process $x cannot be started";
		exit;
	else	
		echo "Process $x was started as $VAR1";
	fi
fi

if [ "$func_execute" == "1" ]; then

	VAR=$(ls -l /proc/$x/exe 2>/dev/null);
	if [ -z "$VAR" ];
	then
		echo "Process $x cannot be started";
		exit;
	else	
		result=$(echo "$VAR"|grep -o ">.*");
		echo "Process $x was started by $result";
fi
fi


if [ "$func_status" == "1" ]; then
	result=$(grep "$OPT" /proc/$x/status);
	echo -e "Status of the file is:\n$result";
fi

var1="";
if [ "$func_process" == "1" ]; then
	
	var=$(ls -l /proc/*/fd | grep $OPT1);
	var_res=$(echo "$var"|grep -o "/.*");
	store=( $var_res );
	for i in "${store[@]}"; do
		var1=$(find -L /proc/*/fd -xtype l -samefile $i 2>/dev/null);	
		var2=${var1:6}; #finds the process id from the sixth element	
	echo "Process id's are :"	
	echo `expr "$var2" : '\([0-9]*[0-9]\)'`; #finds the process id from sixth element to the end
	done	
	
fi

}



while getopts ":hcs:f:e" opt; do
		count=1;		
	case $opt in

		h) 	
			func_usage=1;
			option_check="1"
		;;
	
		c) 	
			func_command=1;
		option_check="1"
		;;
	
		s) 
			func_status=1;
			OPT=$OPTARG;
		option_check="1"
		;;

		f) 
			func_process=1;
			OPT1=$OPTARG			
		option_check="1"
		;;

		e) 	
			func_execute=1;	
		option_check="1"
		;; 

		*) 	
			if [ "$OPTARG" == 's' ]; 
			then
			  echo "Argument required for option 's', exiting the program";
			elif [ "$OPTARG" == 'f' ];
			then
			  echo "Filename required for option 'f', exiting the program";
			else			
			  echo "Invalid Option $OPTARG entered, exiting the program";
			fi
			exit;			
			;;
	esac
			count=$((count+1));
done

if [ "$option_check" = "1" ]
then
shift $(($OPTIND -1))
fi
x=$1;

if [ "$func_execute" = "0" ]
then
process_check
fi

check_opts;

