import matplotlib.pyplot as plt
import numpy as np

data_file = "./P9data.txt";

with open(data_file, 'r') as file:
    data_string = file.read();

data_list = list(map(int, data_string.split()));

# Create the histogram
plt.hist(data_list, bins=30, alpha=0.7, color='blue', edgecolor='black');
plt.show();
