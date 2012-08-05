#! /bin/bash
# Released under GPL licence v2 or upper
#
# When executed ($ ./tests.sh), no errors should occur in the
# two first sections.  The third one should result in only error messages,
# each one different. Segfault or default error messages must not appear.
#
# The following lines are the expected results:
#	Use argv
#2 
#3 
#Roll #1: (2 ) = 2 
#Roll #1: (1 ) = 1 
#Roll #1: (1 ) = 1 
#2 
#2 
#	Use stdin
#1 
#1 
#4 
#Roll #1: (1 ) = 1 
#Roll #1: (1 ) = 1 
#Roll #1: (2 1 ) + 2 = 5 
#	Error messages handle numbers that are too large
#rolldice: Requested number of dice faces is too large
#rolldice: Requested number of dropped dice is too large
#rolldice: Requested number of rolled dice is too large
#rolldice: Requested multiplier is too large
#rolldice: Requested add modifier is too large
#rolldice: Requested minus modifier is too large


function check_result {
    RES=`../rolldice $1`

    if echo ${RES} | grep -q $2
        then
            echo ${1}": OK"
        else
            echo ${1}": ERROR ("${RES}")"
    fi
}


echo -e "\tResult between right limits"
check_result "1d2" "^[12]"
check_result "1d2+1" "^[23]"
check_result "1d2*2" "^[24]"
check_result "1d2-1" "^[01]"


echo -e "\tUse argv"
../rolldice 1d2
../rolldice 1d2+1
../rolldice -s 1d2
../rolldice -s -u 1d2
../rolldice -s -r 1d2
../rolldice 1d2 1d3
../rolldice 1d%
../rolldice 1d%+1

echo -e "\tUse stdin"
cat rollfile | ../rolldice
cat rollfile | ../rolldice -s 

echo -e "\tError messages handle numbers that are too large"
../rolldice 1d123456789
../rolldice 2d3s123456789
../rolldice 123456789x2d2
../rolldice 2d2*123456789
../rolldice 2d2+123456789
../rolldice 2d2-123456789

