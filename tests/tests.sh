#! /bin/sh
# Released under GPL licence v2 or upper
#
# When executed ($ ./tests.sh), no error should occur for the
# two first sections.  The third one should have only errors messages,
# each one different. Segfault or default error messages must not appear.
#
# The results like following lines have to be printed:
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
#	Error messages handle too big numbers
#rolldice: Requested dice faces too big
#rolldice: Requested dropped dices too big
#rolldice: Requested rolled dices too big
#rolldice: Requested multiplier too big
#rolldice: Requested add modifier too big
#rolldice: Requested minus modifier too big

echo "\tUse argv"
../rolldice 1d2
../rolldice 1d2+1
../rolldice -s 1d2
../rolldice -s -u 1d2
../rolldice -s -r 1d2
../rolldice 1d2 1d3
../rolldice 1d%
../rolldice 1d%+1

echo "\tUse stdin"
cat rollfile | ../rolldice
cat rollfile | ../rolldice -s 

echo "\tError messages handle too big numbers"
../rolldice 1d123456789
../rolldice 2d3s123456789
../rolldice 123456789x2d2
../rolldice 2d2*123456789
../rolldice 2d2+123456789
../rolldice 2d2-123456789

