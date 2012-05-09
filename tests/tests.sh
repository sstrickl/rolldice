#! /bin/sh
# Released under GPL licence v2 or upper
#
# When executed ($ ./tests.sh), no error should occur and
# results like following lines have to be printed:
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


echo "\tUse argv"
../rolldice 1d2
../rolldice 1d2+1
../rolldice -s 1d2
../rolldice -s -u 1d2
../rolldice -s -r 1d2
../rolldice 1d2 1d3

echo "\tUse stdin"
cat rollfile | ../rolldice
cat rollfile | ../rolldice -s 

