#!/bin/python3

# usage: ./test_file.py size [alphabet_size]

import sys
from random import *

alphabet = ['A', 'C', 'G', 'T', 'B', 'D', 'E', 'F', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'U', 'V', 'X', 'Y', 'Z']

f = open('perftest_'+sys.argv[1]+'.test', 'w')
s = ""

chars = 4

if (len(sys.argv) > 2):
    chars = int(sys.argv[2])

alphabet = alphabet[:chars]

for i in range(int(sys.argv[1])):
    s += choice(alphabet)

s += "$"

f.write(s)
f.close()
