from math import floor
from math import log

max_interval = 64
array_size = 32;
a = [floor(max_interval - log(x) * max_interval / log(array_size)) for x in range(1, array_size + 1)]
a.reverse()
print(len(a))
print(a);
