import numpy as np
from tqdm import tqdm
from liblinear.liblinearutil import *
import matplotlib.pyplot as plt


# Load data
y_train, x_train = svm_read_problem('train_2_6.scale')
print(len(y_train))
print(len(x_train))
print("read training file")
y_test, x_test = svm_read_problem('test_2_6.scale')
print(len(y_test))
print(len(x_test))
print("read testing file")

# Define the range of lambda values and calculate corresponding C
lambda_values = [10**i for i in range(-2, 4)]
best_lambda, best_Ein, best_model = None, float('inf'), None
num_trials = 1126

# Helper function to get Ein error
def calculate_error(y_true, y_pred):
    # zero one error
    return np.mean(np.array(y_true) != np.array(y_pred))
'''
# Loop through lambda values to select the best one
avgEin = []
best_lambda, best_Ein, best_model = None, float('inf'), None
for lambda_val in lambda_values:
    C = 1 / (lambda_val)  # Relationship between lambda and C
    errors = []
    print(C)
    model = train(y_train, x_train, f'-s 6 -c {C} -B 1 -q')
    p_label, _, _ = predict(y_train, x_train, model, '-q')
    Ein = calculate_error(y_train, p_label)
    if Ein < best_Ein or (Ein == best_Ein and lambda_val > best_lambda):
        best_lambda, best_Ein, best_model = lambda_val, Ein, model

# Calculate Eout for the best model
y_pred, _, _ = predict(y_test, x_test, best_model)
Eout = calculate_error(y_test, y_pred)
print(f"Best lambda: {best_lambda}, Ein: {best_Ein}, Eout: {Eout}")
loglambda_values = [-2, -1, 0, 1, 2, 3]
plt.figure()
plt.plot(loglambda_values, avgEin, marker='o', linestyle='-', color='b', label="Data Points")
plt.title("Ein v.s. log(lambda)")
plt.xlabel("log of lambda")
plt.ylabel("average error of training sample")
plt.savefig("P10_1.png")
# Optional: Histogram of Eout and non-zero components
'''
eout_trials = []
blambda = []
non_zero_counts = []
num_trials = 1126

for k in tqdm(range(num_trials), desc = "Running..."):
    np.random.seed(k)
    best_lambda, best_Ein, best_model = None, float('inf'), None
    for lambda_val in lambda_values:
        C = 1 / (lambda_val)  # Relationship between lambda and C
        errors = []
        model = train(y_train, x_train, f'-s 6 -c {C} -B 1 -q')
        p_label, _, _ = predict(y_train, x_train, model, '-q')
        Ein = calculate_error(y_train, p_label)
        if Ein < best_Ein or (Ein == best_Ein and lambda_val > best_lambda):
            best_lambda, best_Ein, best_model = lambda_val, Ein, model
    blambda.append(best_lambda)
    p_label, _, _ = predict(y_test, x_test, best_model, '-q')
    eout_trials.append(calculate_error(y_test, p_label))
    weights = np.ctypeslib.as_array(best_model.w, shape=(best_model.nr_feature,))
    non_zero_counts.append(sum(1 for coef in weights if coef != 0))

plt.figure()
plt.hist(eout_trials, bins=30)
plt.title("Histogram of Eout over trials")
plt.savefig("P10_1_M.png")
print('Average Eout fo g is: ', np.mean(eout_trials))

plt.figure()
plt.hist(non_zero_counts, bins=30)
plt.title("Histogram of non-zero components in w")
plt.savefig("P10_2_M.png")
print('Average non-zero count is: ', np.mean(non_zero_counts))

plt.figure()
bin_size = 0.01
bins = np.arange(0, max(blambda) + bin_size, bin_size)
plt.hist(blambda, bins=bins)
plt.title("Best lambda over tirals")
plt.savefig("P10_3_M.png")