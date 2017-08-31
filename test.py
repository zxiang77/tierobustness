from igraph import *

b = [1, 2]
c = [2,3,4]
a = [b, c]

d = []
d.append(list(a[1]))

del c[0]
d.extend(c)
print d

f = set()
f.add(3)
f.add(3)
f.add(3)
print f
# print type(type(a)) == type.type
