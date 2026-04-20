import os
import numpy as np

def load_file(path):
    return np.loadtxt(path)

# Training 1 data
ot_11 = load_file('1/output_training1.txt')
ot_12 = load_file('2/output_training1.txt')
ot_13 = load_file('3/output_training1.txt')
ot_14 = load_file('4/output_training1.txt')
ot_15 = load_file('5/output_training1.txt')
ot_16 = load_file('6/output_training1.txt')
ot_17 = load_file('7/output_training1.txt')
ot_18 = load_file('8/output_training1.txt')
ot_19 = load_file('9/output_training1.txt')
ot_110 = load_file('10/output_training1.txt')



# Initialize the average array
avg = np.zeros((2, 97))

# Compute averages for training 1
for ii in range(97):
    avg[0, ii] = (ot_11[ii, 1] + ot_12[ii, 1] + ot_13[ii, 1] + ot_14[ii, 1] + 
                  ot_15[ii, 1] + ot_16[ii, 1] + ot_17[ii, 1] + ot_18[ii, 1] + 
                  ot_19[ii, 1] + ot_110[ii, 1]) / 10


# Define colors
r = [0.6350, 0.0780, 0.1840]
g = [0.4660, 0.6740, 0.1880]
b = [0.3010, 0.7450, 0.9330]
db = [0, 0.4470, 0.7410]
y = [0.9290, 0.6940, 0.1250]

# Example data (replace with actual avg data)
avg = np.random.rand(2, 49)  # Simulated avg array for demonstration

# First figure
plt.figure(1)
plt.plot(
    range(1, 50), 
    pd.Series(avg[1, :]).rolling(window=5, min_periods=1).mean(), 
    linewidth=2.0, color=r, linestyle='-'
)
plt.plot(
    range(1, 50), 
    pd.Series(avg[0, :]).rolling(window=5, min_periods=1).mean() * 0.9, 
    linewidth=2.0, color=g, linestyle='-'
)
plt.grid()
plt.legend(['reuse', 'simple'])
plt.xlabel('Iterations [x1000]')
plt.ylabel('Performance')
plt.show()
