import numpy as np
import time
import sys



# set the random seed
np.random.seed(42)

# import Strawberry Fields
import strawberryfields as sf
from strawberryfields.ops import *
from strawberryfields.utils import random_interferometer
from pathlib import Path


from thewalrus import threshold_detection_prob

class StrawberrySampler(object):
   def __init__(self, unitaryMatrix):
      self.time_cost = 0
      self.U = unitaryMatrix
      self.d = len(unitaryMatrix[0])
      self.output_prob = []
      self.output_list = []
      gbs = sf.Program(self.d)
      with gbs.context as q:
         # prepare the squeeze state
         S = Sgate(1)
         for i in range(self.d):
            # apply squeeze operator on vacuum states
            S | q[i]
         # linear interferometer
         Interferometer(U) | q
      
      eng = sf.Engine(backend="gaussian")
      # execute the circuits
      results = eng.run(gbs,shots=3)
      state = results.state
      self.mu = state.means()
      print(state)
      print(self.mu)
      #sys.exit()
      # the 2N×2N covariance matrix.
      # https://strawberryfields.readthedocs.io/en/stable/code/api/strawberryfields.backends.BaseGaussianState.html?highlight=.cov()#strawberryfields.backends.BaseGaussianState.cov
      self.cov = state.cov()
      print("covariance matrix is: ")
      print(self.cov)
      self.record_cov()
      

   def generateAllBitStrings(self, d:int,  bits:list, i:int)->list:
      if (i == d):
         self.output_list.append(bits)
         return
      bits.append(0)
      self.generateAllBitStrings(d, bits[:], i + 1)
   
      bits[-1] = 1
      self.generateAllBitStrings(d, bits[:], i + 1)

   def get_prob(self):
      self.generateAllBitStrings(self.d, [], 0)
      print("\nprobability of every output mode:")
      start = time.time()
      for i in self.output_list:
         #prob = results.state.fock_prob(i)
         #print("|{}>: {}".format("".join(str(j) for j in i), prob))
         p_click = np.real_if_close(threshold_detection_prob(self.mu, self.cov, i))
         self.output_prob.append(p_click)
         print("|{}>: {}".format("".join(str(j) for j in i), p_click))
      self.time_cost = time.time()- start
      print("Time cost: "+ str(self.time_cost)+'[s]')



   def record_cov(self):
      data_folder = Path("./data/")
      file = data_folder / "covMatrix.txt"
      with open(file, 'w+') as fp:
         for row in self.cov:
            fp.write(' '.join(map(str, row)))
            fp.write('\n')

      filename = "covMatrix_"+str(self.d)+"d.txt"
      file2 = data_folder / filename
      with open(file2, 'w+') as fp:
         for row in self.cov:
            fp.write(' '.join(map(str, row)))
            fp.write('\n')

   def write_to_csv(self):
      data_folder = Path("./data/")
      file = data_folder / ("strawberry_" + str(self.d) +"d.csv")
      with open(file, 'w+') as fp:
         fp.write("strawberryField\n")
         fp.write("time cost:,"+str(self.time_cost)+'[s]\n')
         fp.write("output, probability\n")
         for i,row in enumerate(self.output_list):
            fp.write(''.join(str(x) for x in self.output_list[i])+','+
                     str(self.output_prob[i])+'\n')




# Main entry point of the program
if __name__ == "__main__":

   # define the linear interferometer
   
   U = np.array([
   [ 0.219546940711-0.256534554457j, 0.611076853957+0.524178937791j, -0.102700187435+0.474478834685j,-0.027250232925+0.03729094623j],
   [ 0.451281863394+0.602582912475j, 0.456952590016+0.01230749109j,  0.131625867435-0.450417744715j, 0.035283194078-0.053244267184j],
   [ 0.038710094355+0.492715562066j,-0.019212744068-0.321842852355j,-0.240776471286+0.524432833034j,-0.458388143039+0.329633367819j],
   [-0.156619083736+0.224568570065j, 0.109992223305-0.163750223027j,-0.421179844245+0.183644837982j, 0.818769184612+0.068015658737j]
   ])

   d = 4
   try:
      d=int(sys.argv[1]) 
   except:
      print("Usage python covMatrix_generator d\n default interferometer dimension d=4\n")
      pass
   

   U = random_interferometer(d)
   #print(U)
   #print("===================")
   cov = StrawberrySampler(U)
   #cov.get_prob()
   #cov.write_to_csv()

