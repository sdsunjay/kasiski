CPE 456 Fall 2012
=======

## ftable:
The first program "ftable" creates a frequency analysis table of an input file, which is helpful in cracking a file that has been encrypted with the Vigenere Cipher (which we created in a previous assignment). <br>
usage: ```ftable[-v][-snum][-pnum][infile[outfile]]```  

## ic:
The second program "ic" computes the index of coincidence, which is also helpful in cracking the Vigenere Cipher. <br>
usage: ```ic N l [l 2 [...]]  ```
   


## kasiski:
The third program and perhaps the most complex, is "kasiski". "kasiski" attempts to crack the Vigenere Cipher by finding matching substrings in the input file and then giving the distance between those substrings.  For this program I had to implement a hash table. <br>
usage: ```kasiski [ -v ] [ -m length ] [ infile [ outfile ] ]```

The specifications for the programs are [asgn1.pdf](asgn1.pdf) and [asgn2.pdf](asgn2.pdf)
