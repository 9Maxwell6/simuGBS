import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


marginal_12d = pd.read_csv('./data/simuGBS_12d_10000000samples.csv',dtype={'samples': str}).iloc[0:].astype(str)
porb_distribution = pd.read_csv('./data/simuGBS_12d_direct.csv').iloc[2:].astype(str)



#while True:
#    continue

fs = []
fs_error = []
ds = []
ds_error = []
f = []
d = []
x = np.arange(1,13)


#"""

for n in x:
    marginal_12d = pd.read_csv('./data/simuGBS_12d_10000000samples_Fourier_'+str(n)+'.csv',dtype={'samples': str}).iloc[0:].astype(str)
    print(n)
    for idx, row in porb_distribution.iterrows():
        count = (marginal_12d.iloc[:,0].astype(str)==row[0]).sum()
        p = count/len(marginal_12d.index)
        q = float(row[1])
    #print(row[0])
        #print(marginal_12d.iloc[:,0])
        f.append(np.sqrt(p*q))
        d.append(np.abs(p-q)/2)
        #print(p)
        #print(q)
        #print("=================")
    fs.append(np.sum(f))
    fs_error.append(np.std(f))
    ds.append(np.sum(d))
    ds_error.append(np.std(d))
    f=[]
    d=[]

fig, ax1 = plt.subplots()
#ax1.set_xscale('log')

# Plot the first line on the left y-axis
ax1.errorbar(x, fs,yerr=fs_error,fmt='--o', color='red',ecolor='black', capsize=2.5, elinewidth=1, capthick=1,alpha=0.8)
ax1.set_xlabel('Highest order of correlation')
ax1.set_ylabel('Fidelity', color='red')
ax1.tick_params(axis='y', labelcolor='red')

# Create a second y-axis on the right side of the figure
ax2 = ax1.twinx()
#ax2.set_xscale('log')
# Plot the second line on the right y-axis
ax2.errorbar(x, ds,yerr=ds_error, fmt='--o', color='blue',ecolor='black', capsize=2.5, elinewidth=1, capthick=1,alpha=0.8)
ax2.set_ylabel('Total variation distance', color='blue')
ax2.tick_params(axis='y', labelcolor='blue')
ax1.grid(True,linestyle='-')
plt.xlim([1,12])
ax1.set_ylim([0, 1.05])
ax2.set_ylim([0, 1.05])

plt.show()





    


#"""