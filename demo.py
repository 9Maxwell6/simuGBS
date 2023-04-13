import strawberryfields as sf
from strawberryfields.ops import *
from strawberryfields.utils import random_interferometer
d = 16 # dimension of the system
U = random_interferometer(d)  # generate random unitary matrix
gbs = sf.Program(d)
eng = sf.Engine(backend="gaussian")
with gbs.context as q:
    # prepare the input squeezed gate
    S = Sgate(1)  
    for i in range(d):
        # apply squeeze operation on vacuum states
        S | q[i]
    # passinf the states to linear interferometer
    Interferometer(U) | q
results = eng.run(gbs)

# obtain covariance matrix
cov = results.state.cov()
