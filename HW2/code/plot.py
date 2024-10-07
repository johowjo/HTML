import matplotlib.pyplot as plt
import numpy as np

# P11
x_vals = []
y_vals = []
val = []

with open("11data.txt", "r") as file:
    for line in file:
        x, y = map(float, line.split())
        x_vals.append(x)
        y_vals.append(y)

for i in range(2000):
    val.append(y_vals[i] - x_vals[i])

np.sort(val)
print((val[999] + val[1000]) / 2)

plt.scatter(x_vals, y_vals, s = 10)

plt.xlabel("E_in")
plt.ylabel("E_out")
plt.show()

# P12
# x_vals = []
# y_vals = []
# val = []
#
# with open("12data.txt", "r") as file:
#     for line in file:
#         x, y = map(float, line.split())
#         x_vals.append(x)
#         y_vals.append(y)
#
# for i in range(2000):
#     val.append(y_vals[i] - x_vals[i])
#
# np.sort(val)
# print((val[999] + val[1000]) / 2)
#
# plt.scatter(x_vals, y_vals, s=10)
#
# plt.xlabel("E_in")
# plt.ylabel("E_out")
# plt.show()
