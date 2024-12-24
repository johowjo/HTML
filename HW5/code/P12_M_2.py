import numpy as np
import random
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
num_trials = 1126

# Helper function to get Ein error
def calculate_error(y_true, y_pred):
    # zero one error
    return np.mean(np.array(y_true) != np.array(y_pred))
'''
combined = list(zip(y_train, x_train))
random.shuffle(combined)
y_train_shuffled, x_train_shuffled = zip(*combined)
y_train_shuffled = list(y_train_shuffled)
x_train_shuffled = list(x_train_shuffled)
'''

eout_trials = []
blambda = []
for k in tqdm(range(num_trials), desc = "Running 2nd part..."):
    np.random.seed(k)
    best_lambda, best_Accuracy, best_model = None, float(-1), None
    for lambda_val in lambda_values:
        C = 1 / (lambda_val)  # Relationship between lambda and C
        # Run multiple trials
        Accuracy = train(y_train, x_train, f'-s 6 -c {C} -B 1 -v 3 -q')
        #print(Accuracy)
        if Accuracy > best_Accuracy or (Accuracy == best_Accuracy and lambda_val > best_lambda):
            best_lambda, best_Accuracy, best_model = lambda_val, Accuracy, model
    print("best accuracy = ", best_Accuracy)
    print("best lambda = ", best_lambda)
    blambda.append(best_lambda)
    model = train(y_train, x_train, f'-s 6 -c {1/(best_lambda)} -B 1 -q')
    p_label, _, _ = predict(y_test, x_test, model, '-q')
    eout_trials.append(calculate_error(y_test, p_label))

plt.figure()
plt.hist(eout_trials, bins=30)
plt.title("Histogram of Eout over trials")
plt.savefig("P12_1_M_2.png")
print('Average Eout fo g is: ', np.mean(eout_trials))
plt.figure()
bin_size = 0.01
bins = np.arange(0, max(blambda) + bin_size, bin_size)
plt.hist(blambda, bins=bins)
plt.title("Histogram of best lambda over trials")
plt.savefig("P12_2_M_2.png")
