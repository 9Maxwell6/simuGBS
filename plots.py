import matplotlib.pyplot as plt
import numpy as np

x = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]
strawberry = [1.91, 1.95, 1.91, 1.94, 
              3.57, 2.0, 3.51, 2.68,
              2.15, 5.31, 12.16, 20.16,
              73, 209.43, 452.8, 1360.72]

direct = [0.05, 0.03, 0.02,0.044,
          0.045,0.031,0.11,0.72,
          0.173,0.828,5.3,20.42,
          70.15,222.78,714.1,2300]

marginal = [0.031,0.032,0.036,0.018,
            0.015,0.0196,0.0668,0.043,
            0.0878,1.443, 4.032,12.162,
            38.12, 123.94,389.48,1393.06]

plt.figure(num = 3, figsize=(8, 5))
plt.plot(x, strawberry,label="strawberry")
plt.plot(x, direct, color='red',  label="direct",alpha=0.5)
plt.xlabel("#bits")
plt.ylabel("time [s]")
plt.legend()

plt.show()


plt.figure(num = 3, figsize=(8, 5))
plt.plot(x, strawberry,label="strawberry")
plt.plot(x, direct, color='red',  label="direct",alpha=0.5)
plt.plot(x, marginal,   label="marginal",alpha=0.5)

plt.xlabel("#bits")
plt.ylabel("time [s]")
plt.legend()

plt.show()