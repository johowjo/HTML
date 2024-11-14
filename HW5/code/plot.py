import matplotlib.pyplot as plt
import numpy as np

# with open("./p10_data/p10.txt") as file:
#     data_string = file.read();
#
# data_list = list(map(float, data_string.split()))
#
# print(np.mean(data_list))
#
# plt.hist(data_list, bins = 70);
# plt.show();


# with open("./p11_data/p11.txt") as file:
#     data_string = file.read();
#
# data_list = list(map(float, data_string.split()))
#
# print(np.mean(data_list));
#
# plt.hist(data_list, bins = 70);
# plt.show();

with open("./p12_data/p12.txt") as file:
    data_string = file.read();

data_list = list(map(float, data_string.split()))

print(np.mean(data_list));

plt.hist(data_list, bins = 70);
plt.show();
