#!usr/bin/env/python3

import sys
from random import *


f = open('perftest_'+sys.argv[1], 'w')
s = ""
for i in range(int(sys.argv[1])):
    s += choice(['A','C','T','G'])

s += "$"

f.write(s)
f.close()
