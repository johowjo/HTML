import matplotlib.pyplot as plt
import numpy as np

# P10
data_file = "./P10_data.txt";

with open(data_file, 'r') as file:
    data_string = file.read();

data_list = list(map(int, data_string.split()));

# Create the histogram
plt.hist(data_list, bins=30, alpha=0.7, color='blue', edgecolor='black');
plt.show();

# P11
# data_file = "./P11_data.txt";
# int_list = list(range(51));
# with open(data_file, 'r') as file:
#     for data_string in file:
#         data_list = [float(num) for num in data_string.split()]
#         plt.plot(data_list);
#
# plt.show();
 
# P12
# data_file = "./P12_data.txt";
#
# with open(data_file, 'r') as file:
#     data_string = file.read();
#
# data_list = list (map(int, data_string.split()));
#
# plt.hist(data_list, bins = 30, alpha = 0.7, color = 'blue', edgecolor = 'black');
# plt.show();
#
