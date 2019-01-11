# author: Tran Quang Loc (darkkcyan)
# editorial: https://codeforces.com/blog/entry/8166
# Note: I switched to python for this problem because I want my check function to always use integer number
#       I tried to solve this problem using C++ and got overflow even with long long number
#       (and really, never change it to unsigned long long because there are subtractions in the checking equation).

from collections import deque

class line:
    def __init__(self, k, b):
        self.k = k
        self.b = b

    def get(self, x):
        return self.k * x + self.b

def check(l1, l2, nl):
    return (nl.b - l2.b) * (l1.k - l2.k) - (nl.k - l2.k) * (l1.b - l2.b) <= 0

n = int(input())
a = list(map(int, input().split()))
b = list(map(int, input().split()))
ans = 0  # we don't even need the entire dp array, because the deque store the value of all previous ones.
         # this variable contains the current computed dp value

hull = deque()
hull.append(line(b[0], 0))
for i in range(1, n):
    arg = a[i]
    while len(hull) > 1 and hull[0].get(arg) >= hull[1].get(arg):
        hull.popleft()
    ans = hull[0].get(arg)
    nl = line(b[i], ans)
    while len(hull) > 1 and check(hull[-2], hull[-1], nl):
        hull.pop()
    hull.append(nl)

print(ans)
