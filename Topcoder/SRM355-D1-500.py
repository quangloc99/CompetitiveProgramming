# Author: Tran Quang Loc
# Idea:
# Let's call 4 points A, B, C, D.
# First I used triangle inequality to check for each triple of points.
# If the above is true, then we got triangle ABC. Now I check if 3 spheres (A, AD), (B, BD) and (C, CD) are intersected or not.
# It is easy to see that they are intersected iff the intersection circel of (A, AD) and (B, BD) must intersect with 
# intersection circel of (A, AD) and (C, CD). I project these sphares and circles on the plane that contains triangle ABC.
# The nice thing is that now the intersection circles are transformed into lne, or more precise, they are chord of circle (A, AD) 
# on the plane. The problem now is to check if these two chord intersect or not.
#
# See this for better understanding https://www.geogebra.org/3d/cs3xj35d
# Or these images if the link dies: 
#   https://github.com/quangloc99/CompetitiveProgramming/blob/master/Topcoder/srm355-500-perspective.png
#   https://github.com/quangloc99/CompetitiveProgramming/blob/master/Topcoder/srm355-500-top-down.png
#
# In the code I don't need to find the exact intesection point, I just need to find the angle of the point in polar coordinates
# and then check using those angles.
from math import *
from itertools import *

EPS = 1e-8
class Tetrahedron:

    def exists(self, d):
        dis = [[int(i) for i in s.strip().split()] for s in d]
        def can_be_triangle(a, b, c):
            for (m, n, p) in permutations((a, b, c), 3):
                if dis[m][n] + dis[m][p] < dis[n][p]:
                    return False
            return True

        for (a, b, c) in combinations((0, 1, 2, 3), 3):
            if not can_be_triangle(a, b, c):
                return "NO"
        
        def find_angle(a, b, c):
            return acos((b * b + c * c - a * a) * 1.0 / (2 * b * c))

        ang301 = find_angle(dis[1][3], dis[0][1], dis[0][3])
        ang302 = find_angle(dis[2][3], dis[0][2], dis[0][3])
        ang102 = find_angle(dis[1][2], dis[1][0], dis[0][2])
        print(ang301 / pi, ang302 / pi, ang102 / pi)

        def ensure180(a):
            while a > pi - EPS: a -= pi * 2
            while a < -pi + EPS: a += pi * 2
            return a

        def inside(x, a, b):
            if abs(x - a) < EPS or abs(x - b) < EPS:
                return 0
            if a - EPS < x < b + EPS:
                return 1
            return -1

        u = (-ang301, ang301)
        v = (ensure180(-ang302 + ang102), ensure180(ang302 + ang102))
        print(u, v)
        print(inside(v[0], u[0], u[1]), inside(v[1], u[0], u[1]))
        if inside(v[0], u[0], u[1]) * inside(v[1], u[0], u[1]) < 1:
            return "YES"

        return "NO"
