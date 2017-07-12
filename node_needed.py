#!/usr/bin/env python
import math
import sys

LIGHT_SPEED = 300000000

##########-e-############

x = 200
y = 200

#########################

Pr = 2.13643e-07
Pt = 0.281838
Gt = 1.0
Gr = 1.0
hr = 1.5
ht = 1.5
L  = 0.5
l  = LIGHT_SPEED/2.4e+9

r = math.sqrt((Pt * Gt**2 * l**2) / (L * Pr)) / (4 * math.pi) + 5;

#########################

for i in sys.argv[1:]:
    if i.startswith('output='):
        p = (math.pi*r**2)/(x*y)

        mode = i[7:]
        if mode == 'num':
            num = (neighbors-1) / p + 1
            print(math.ceil(num))
        elif mode == 'neighbors':
            neighbors = (num-1) * p + 1
            print(neighbors)
        else:
            print(eval(mode))
    else:
        locals()[i[:i.index('=')]] = eval(i[i.index('=')+1:])


#P没有一个邻居 = (1 - ((pi*r^2)/((x-2*r)*(y-2*r))
#E邻居 = P是邻居 * n
