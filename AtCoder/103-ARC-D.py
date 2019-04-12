# Author: Tran Quang Loc (darkkcyan)
# This solution is the same as in the editorial.

from sys import stdin, exit
 
def l2(num):
    return 0 if num == 1 else l2(num // 2) + 1
 
def go(x, y, lv):
    return (
            '' if x == y == 0
            else go(x, y - (1 << lv), lv - 1) + 'U' if abs(y) > abs(x) and y > 0
            else go(x, y + (1 << lv), lv - 1) + 'D' if abs(y) > abs(x) and y < 0
            else go(x + (1 << lv), y, lv - 1) + 'L' if abs(x) > abs(y) and x < 0
            else go(x - (1 << lv), y, lv - 1) + 'R'
    )
 
n = int(stdin.readline())
a = [list(map(int, stdin.readline().split())) for i in range(n)]
 
parity = (a[0][0] + a[0][1]) % 2
for (x, y) in a:
    if (x + y) % 2 != parity:
        print("-1")
        exit(0)
 
base_ans = ''
d = []
if parity == 0:
    base_ans = 'U'
    d.append(1)
    a = list(map(lambda u: (u[0], u[1] - 1), a))
 
mx = max(map(lambda u: abs(u[0]) + abs(u[1]), a))
max_lv = l2(mx)
d.extend(1 << i for i in range(max_lv + 1))
 
print(len(d))
print(' '.join(map(str, d)))
for (x, y) in a:
    print(base_ans + go(x, y, max_lv))
