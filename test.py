from igraph import *

b = [1, 2]
c = [2,3,4]
a = [b, c]

d = []
d.append(list(a[1]))

del c[0]

print d

# print type(type(a)) == type.type
