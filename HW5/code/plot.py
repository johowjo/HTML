import matplotlib.pyplot as plt
import numpy as np

with open("./p12_data/best_lambda.txt") as file:
    data_string = file.read();

data_list = list(map(float, data_string.split()))

print(np.mean(data_list))

plt.xlabel("log(\lambda^\star)")
plt.ylabel("frequency")
plt.hist(data_list, bins = 70);
plt.show();

# with open("./p12_data/non_zero_entries.txt") as file:
#     data_string = file.read();
#
# data_list = list(map(float, data_string.split()))
#
# print(np.mean(data_list))
#
# plt.xlabel("number of non-zero components")
# plt.ylabel("frequency")
# plt.hist(data_list, bins = 70);
# plt.show();

with open("./p10_data/p10.txt") as file:
    data_string = file.read();

data_list = list(map(float, data_string.split()))

print(np.mean(data_list));
print(np.var(data_list));

plt.xlabel("Eout")
plt.ylabel("frequency")
plt.hist(data_list, bins = 70);
plt.show();
