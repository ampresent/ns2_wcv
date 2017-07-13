#!/usr/bin/env python
import math
import sys

LIGHT_SPEED = 300000000

##########-e-############

x = 200
y = 200

#########################

Pr = 2.13643e-07
Pt = 0.28183815
Gt = 1.0
Gr = 1.0
hr = 1.5
ht = 1.5
L  = 0.5
l  = LIGHT_SPEED/2.4e+9


#########################

for i in sys.argv[1:]:
    if i.startswith('output='):
        r = math.sqrt((Pt * Gt * Gr * l**2) / (L * Pr)) / (4 * math.pi) + 5;
        p = ((r**4*(3*math.pi + 11))/3 + 2*(y - 2*r)*(math.pi*r**3 - (2*r**3)/3) + 4*r**4*(math.pi - 4/3) - 2*(2*r - x)*(math.pi*r**3 - (2*r**3)/3) - r**2*math.pi*(y - 2*r)*(2*r - x))/(y*x)**2

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
