##### To Run This Program #####
Run one of the lines below replacing arguments as necessary:

./bloom_filter.py -d dictionary.txt -i input.txt -o output3.txt output5.txt
OR
python3 bloom_filter.py -d dictionary.txt -i input.txt -o output3.txt output5.txt

Both of the above commands have been tested on this program on the OSU Flip servers.
There is no guarantee that this will run on any computers other than these servers!!!
No makefile is needed for this program.

##### Answers to Questions #####
a.
The functions I chose were ripemd160, sha256, whirlpool, md5, and DSA. These are all cryptographic hashes. I chose
them because they are less likely to generate collisions than non-cryptographic ones (at the expense of being slower),
as well as because they are the ones build into the hashlib library for python3 on the flip servers, which guarantees
that the grader will be able to run the program without having to install or include additional libraries.

b.
ripemd160: 0.000006914
sha256: 0.00001025
whirlpool: 0.00001121
md5: 0.00001025
DSA: 0.000009298

ripemd160 and DSA are the fastest, though not by much. They perform better as their algorithms to generate the hash
computer more quickly than the others. It is also likely that the length of the hash output is shorter than ones like
sha256, which are quite large.

c.
The probability of false positives is 1% as I set the hash bit array size to 5976456, which I calculated using a
dictionary size of 623518. The result of the false positive equation -((623518 * log(0.01)/(log^2(2))) = 5976456.
The probability of false negatives is 0%. It is not possible to have a false negative with a bloom filter.

d.
The rate of false positives can be reduced by increasing the number of storable positions for the the hash bit array, or
by reducing the number of hash functions used to reduce collisions.