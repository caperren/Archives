from mykwic import *
from pprint import pprint

for l in open("tocheck.txt"):
    print "="*50
    input = l[:-1]
    print "INPUT:",input
    v = eval("kwic("+input+")")
    print "OUTPUT:"
    pprint