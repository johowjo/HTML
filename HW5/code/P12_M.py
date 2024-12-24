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

combined = list(zip(y_train, x_train))
random.shuffle(combined)
y_train_shuffled, x_train_shuffled = zip(*combined)
y_train_shuffled = list(y_train_shuffled)
x_train_shuffled = list(x_train_shuffled)

eout_trials = []
blambda = []
for k in tqdm(range(num_trials), desc = "Running 2nd part..."):
    np.random.seed(k)
    best_lambda, best_Ecv, best_model = None, float('inf'), None
    for lambda_val in lambda_values:
        C = 1 / (lambda_val)  # Relationship between lambda and C
        # Run multiple trials
        fold_size = len(y_train) // 3
        fold_errors = []
        for fold in range(3):
            start, end = fold*fold_size, (fold+1)*fold_size
            if fold == 2:
                end = len(y_train)
            x_val, y_val = x_train_shuffled[start:end], y_train_shuffled[start:end]
            x_tr = x_train_shuffled[:start] + x_train_shuffled[end:]
            y_tr = y_train_shuffled[:start] + y_train_shuffled[end:]
            model = train(y_tr, x_tr, f'-s 6 -c {C} -B 1 -q')
            p_label, _, _ = predict( y_val, x_val,model, f'-q')
            Ecvfold = calculate_error(y_val, p_label)
            fold_errors.append(Ecvfold)
        Ecv = np.mean(fold_errors)
        if Ecv < best_Ecv or (Ecv == best_Ecv and lambda_val > best_lambda):
            best_lambda, best_Ecv, best_model = lambda_val, Ecv, model
        print(best_lambda)
    print("The best lambda we choose: ", best_lambda)
    blambda.append(best_lambda)
    model = train(y_train, x_train, f'-s 6 -c {1/(best_lambda)} -B 1 -q')
    p_label, _, _ = predict(y_test, x_test, model, '-q')
    eout_trials.append(calculate_error(y_test, p_label))

plt.figure()
plt.hist(eout_trials, bins=30)
plt.title("Histogram of Eout over trials")
plt.savefig("P12_1_M.png")
print('Average Eout fo g is: ', np.mean(eout_trials))
bin_size = 0.01
bins = np.arange(0, max(blambda) + bin_size, bin_size)
plt.figure()
plt.hist(blambda, bins=bins)
plt.title("Histogram of best lambda over trials")
plt.savefig("P12_2_M.png")