import math


def rshift(x): return x/2


it = 20

rotation_list = []
cos_list = []

two_pow = 1.0

for i in range(it):
    rad = math.atan(two_pow)
    rotation_list.append(f"{rad:.10f}")
    cos_list.append(math.cos(rad))
    two_pow = rshift(two_pow)

k = 1
for i in cos_list:
    k *= i

print("{", ",".join(rotation_list), "}")
print(k)
