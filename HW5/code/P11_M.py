import numpy as np
import random
from tqdm import tqdm
from liblinear.liblinearutil import *
import matplotlib.pyplot as plt

def random_test_train_split(y_train, x_train, num_sub_train):
    random_indices = random.sample(range(len(y_train)), num_sub_train)
    y_sub_train = [y_train[i] for i in random_indices]
    x_sub_train = [x_train[i] for i in random_indices]
    y_rest = [y_train[i] for i in range(len(y_train)) if i not in random_indices]
    x_rest = [x_train[i] for i in range(len(x_train)) if i not in random_indices]
    return y_sub_train, x_sub_train, y_rest, x_rest

# Load data
y_train, x_train = svm_read_problem('train_2_6.scale')
print(len(y_train))
print(len(x_train))
print("read training file")
num_sub_train = 8000
y_sub_train, x_sub_train, y_validation, x_validation = random_test_train_split(y_train, x_train, num_sub_train)
print(len(y_sub_train))
print(len(x_sub_train))
print(len(y_validation))
print(len(x_validation))

y_test, x_test = svm_read_problem('test_2_6.scale')
print(len(y_test))
print(len(x_test))
print("read testing file")

# Define the range of lambda values and calculate corresponding C
lambda_values = [10**i for i in range(-2, 4)]
num_trials = 1126

# Helper function to get Ein error
def calculate_error(y_true, y_pred):
    # zero one error
    return np.mean(np.array(y_true) != np.array(y_pred))
blambda = []
eout_trials = []
for k in tqdm(range(num_trials), desc = "Running 2nd part..."):
    best_lambda, best_Eval, best_model = None, float('inf'), None
    np.random.seed(k)
    for lambda_val in lambda_values:
        C = 1 / (lambda_val)  # Relationship between lambda and C
        model = train(y_sub_train, x_sub_train, f'-s 6 -c {C} -B 1 -q')
        p_label, _, _ = predict(y_validation, x_validation, model, '-q')
        Eval = calculate_error(y_validation, p_label)
        if Eval < best_Eval or (Eval == best_Eval and lambda_val > best_lambda):
            best_lambda, best_Eval, best_model = lambda_val, Eval, model
        print(best_lambda)
    print(best_lambda)
    blambda.append(best_lambda)
    model = train(y_train, x_train, f'-s 6 -c {1/(best_lambda)} -B 1 -q')
    p_label, _, _ = predict(y_test, x_test, model, '-q')
    eout_trials.append(calculate_error(y_test, p_label))
plt.figure()
plt.hist(eout_trials, bins=30)
plt.title("Histogram of Eout over trials")
plt.savefig("P11_1_M.png")
print('Average Eout fo g is: ', np.mean(eout_trials))

bin_size = 0.01
bins = np.arange(0, max(blambda) + bin_size, bin_size)
plt.figure()
plt.hist(blambda, bins=bins)
plt.title("Histogram of best lambda over trials")
plt.savefig("P11_2_M.png")
