#!/usr/bin/python3

import subprocess
import os
import sys
import re

class Result:
    @classmethod
    def from_buffer(cls, string):
        string = str(string)
        string = string[2:-3]  # weird buffer stuff
        parts = string.split(" ")
        seconds = float(parts[0])
        memory = int(parts[1])
        return Result(seconds, memory)
    
    def __init__(self, seconds, memory):
        self.seconds = seconds
        self.memory = memory
    
    def __add__(self, other):
        return Result(self.seconds+other.seconds, self.memory+other.memory)
    
    def __ladd__(self, other):
        self.seconds += other.seconds
        self.memory += other.seconds

def sumresults(l):
    totalseconds = 0.0
    totalmemory = 0
    for r in l:
        totalmemory += r.memory
        totalseconds += r.seconds
    return Result(totalseconds, totalmemory)

def average(l):
    num = len(l)
    total = sumresults(l)
    
    total.memory /= num
    total.seconds /= num
    return total
        

class Test:
    def __init__(self, path):
        self.path = path
    
    def run(self, exe, command="suffix"):
        # returns wall clock as string, memory in KB
        # /bin/time --format "%U %M" ./convertor suffix sometest
        out = subprocess.check_output(['/bin/time', '--format', '%U %M', exe.path, command, self.path], stderr=subprocess.STDOUT)
        return Result.from_buffer(out)
        
    

class Executable:
    def __init__(self, path):
        self.path = path
        self.results = {}


def main():
    # ./
    #   ./execs/ [convertor, ...]
    #   ./test_cases/ [perf_test_150000]
    
    execs = []
    for file in os.listdir("./execs"):
        execs.append(Executable("./execs/"+file))
    
    tests = []
    for file in os.listdir("./testcases"):
        if file.endswith(".test"):
            tests.append(Test("./testcases/"+file))
    # TODO Regex matching?
    
    for e in execs:
        print("[{}]".format(e.path))
        results = []
        for t in tests:
            res = t.run(e)
            results.append(res)
            print("    {0}:\t{1.seconds}\t{1.memory} KB".format(t.path, res))
        total = sumresults(results)
        print("    ---> total:\t{0.seconds}\t{0.memory} KB".format(total))
    
    
if __name__=='__main__':
    main()
