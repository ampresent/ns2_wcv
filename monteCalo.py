import math
import random

##################
#   Parameters
##################

(X, Y)      = (100, 100)        # Rectangle

(x0, y0, r) = (15, 10, 30)      # Circle's center

##################

eps = 1e-5

s = 0
n = 0
for x0 in range(X):
    for y0 in range(Y):
        positive = 0
        iteration = 0
        lastE = -2
        E = -1
        while iteration < 100 or abs(E-lastE) > eps:

            x = random.uniform(0, X)
            y = random.uniform(0, Y)

            if (math.sqrt((x-x0)**2 + (y-y0)**2) < r):
                positive += 1

            iteration += 1

            lastE = E
            E = positive / iteration
        s += E
        n += 1

print('n: {}\tE: {}'.format(n, X*Y*1.0 * s / n))

