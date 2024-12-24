import numpy as np
import random

line_num = 11876
sub_train = "./py_sub_train.txt"
val = "./py_val.txt"
file1 = open(sub_train, "w")
file2 = open(val, "w")
for i in range(1126) :
    print(i)
    np.random.seed(i)
    rand_nums = random.sample(range(0, 11876), 8000)
    for j in range(line_num):
        if j in rand_nums:
            file1.write(f"{j} ")
        else:
            file2.write(f"{j} ")
    file1.write(f"\n")
    file2.write(f"\n")

