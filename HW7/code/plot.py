# import matplotlib.pyplot as plt
#
# # Function to read numbers from a text file
# def read_numbers_from_file(file_path):
#     with open(file_path, 'r') as file:
#         numbers = [float(line.strip()) for line in file]
#     return numbers
#
# # Function to plot the graph of numbers
# def plot_numbers(numbers):
#     plt.figure(figsize=(8, 5))
#     plt.plot(numbers, marker='o', markersize=6, linestyle='-', linewidth=2, color='b', label='Numbers')
#     plt.xlabel('Index')
#     plt.ylabel('Value')
#     plt.title('Graph of Numbers from File')
#     plt.legend()
#     plt.grid(True)
#     plt.show()
#
# # Replace 'numbers.txt' with the path to your text file
# file_path = 'e_in.txt'
# try:
#     numbers = read_numbers_from_file(file_path)
#     plot_numbers(numbers)
# except FileNotFoundError:
#     print(f"Error: The file '{file_path}' was not found.")
# except ValueError:
#     print("Error: The file contains non-numeric data.")

import matplotlib.pyplot as plt

# Function to read numbers from a text file
def read_numbers_from_file(file_path):
    with open(file_path, 'r') as file:
        numbers = [float(line.strip()) for line in file]
    return numbers

# Function to plot the graph of numbers from two files
def plot_numbers_from_two_files(numbers1, numbers2):
    plt.figure(figsize=(8, 5))
    plt.plot(numbers1, marker='o', markersize=4, linestyle='-', linewidth=2, color='b', label='E_in')
    plt.plot(numbers2, marker='s', markersize=4, linestyle='-', linewidth=2, color='r', label='U_t')
    plt.xlabel('t')
    plt.legend()
    plt.grid(True)
    plt.show()

# Replace 'file1.txt' and 'file2.txt' with the paths to your text files
file_path1 = 'e_in.txt'
file_path2 = 'u.txt'
try:
    numbers1 = read_numbers_from_file(file_path1)
    numbers2 = read_numbers_from_file(file_path2)
    plot_numbers_from_two_files(numbers1, numbers2)
except FileNotFoundError as e:
    print(f"Error: {e}")
except ValueError:
    print("Error: One of the files contains non-numeric data.")

