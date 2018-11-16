#!/bin/python
# Author: Tran Quang Loc (darkkcyan)
# Editorial is included in the problem link:
# https://www.hackerrank.com/contests/hourrank-30/challenges/diverse-strings/copy-from/1311385094
import math
import os
import random
import re
import sys

#
# Complete the 'solve' function below.
#
# The function is expected to return a STRING.
# The function accepts following parameters:
#  1. INTEGER n
#  2. INTEGER k
#

preans = ['a']

def findnext(s):
    return 'aa' + ''.join([
        chr(ord(i) + 1) + ('a' if i == 'a' else '')
        for i in s
    ]) + 'a'

def solve(n, k):
    if n < len(preans[k - 1]):
        return "NONE"
    return 'a' * (n - len(preans[k - 1])) + preans[k - 1]

if __name__ == '__main__':
    while len(preans) < 27:
        preans.append(findnext(preans[-1]))
        
    fptr = open(os.environ['OUTPUT_PATH'], 'w')

    t = int(raw_input().strip())

    for t_itr in xrange(t):
        first_multiple_input = raw_input().rstrip().split()

        n = int(first_multiple_input[0])

        k = int(first_multiple_input[1])

        res = solve(n, k)

        fptr.write(res + '\n')

    fptr.close()
