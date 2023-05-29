#include <iostream>
#include <eigen3/Eigen/Dense>

#include "simuGBS_Config.h"
#include "covMatrix_reader.h"
#include "FT_marginalProb.h"
#include "marginalProb.h"
#include "DirectProb.h"
#include "../include/FT_probCalculator.h"
#include "../include/probCalculator.h"

int main (int argc, char *argv[]){

  std::cout << "simuGBS Version " << simuGBS_VERSION_MAJOR << "."
            << simuGBS_VERSION_MINOR << std::endl;

  std::cout << "Hello, from GBS-simulator!\n\n";
  //Eigen::MatrixXd cov = getCovMatrix();

  Eigen::IOFormat OctaveFmt(Eigen::StreamPrecision, 0, ", ", ";\n", "", "", "[", "]");

  //std::cout << "covariance matrix is:\n";
  //std::cout << cov.format(OctaveFmt) << '\n' << std::endl;

  /*
  ProbTree mpTree;
  //std::cout << mpTree.root->prob << std::endl;
  mpTree.insert("1",3);
  mpTree.insert("11",2);
  mpTree.insert("110",5);
  mpTree.insert("0",5);
  mpTree.insert("01",0.2);

  mpTree.displayBinTree();
  */


  //DirectProb d4(cov);
  //d4.calculate_probe();
  //d4.write_csv();









  /*
  Eigen::MatrixXd cov = getCovMatrix(12);
  ProbTree pt(cov);
  //pt.execute(pow(2,cov.rows()),true);
  pt.execute(1000,false);
  pt.execute(10000,false);
  pt.execute(100000,false);
  pt.execute(1000000,false);
  pt.execute(10000000,false);

  //pt.release();
 

  //*/

  /*
  for(int i=0; i<=4; i++){
    // enable Fourier coefficients
    DirectProb d4(cov,i,true);
    d4.calculate_probe();
    d4.write_csv();
  }
  */

  /*
  Eigen::MatrixXd cov = getCovMatrix(3);
  //for(int i=0; i<12;i++){}
  ProbTree pt(cov);
  pt.execute(10000000);
  pt.release();

  //for(int i=0; i<12;i++){}
  ProbTreeFourier ptf(cov,3);
  ptf.execute(10000000);
  ptf.release();
  ProbTreeFourier ptfh(cov,2);
  ptfh.execute(10000000);
  ptfh.release();
  std::cout<<'\n'<<"=========================="<<std::endl;
  cov = getCovMatrix(6);
  //for(int i=0; i<12;i++){}
  ProbTree pt1(cov);
  pt1.execute(10000000);
  pt1.release();

  //for(int i=0; i<12;i++){}
  ProbTreeFourier ptf1(cov,6);
  ptf1.execute(10000000);
  ptf1.release();
  ProbTreeFourier ptf1h(cov,3);
  ptf1h.execute(10000000);
  ptf1h.release();
  std::cout<<'\n'<<"=========================="<<std::endl;
  cov = getCovMatrix(9);
  //for(int i=0; i<12;i++){}
  ProbTree pt2(cov);
  pt2.execute(10000000);
  pt2.release();

  //for(int i=0; i<12;i++){}
  ProbTreeFourier ptf2(cov,9);
  ptf2.execute(10000000);
  ptf2.release();
  ProbTreeFourier ptf2h(cov,5);
  ptf2h.execute(10000000);
  ptf2h.release();
  std::cout<<'\n'<<"=========================="<<std::endl;
  cov = getCovMatrix(12);
  //for(int i=0; i<12;i++){}
  ProbTree pt3(cov);
  pt3.execute(10000000);
  pt3.release();

  //for(int i=0; i<12;i++){}
  ProbTreeFourier ptf3(cov,12);
  ptf3.execute(10000000);
  ptf3.release();
  ProbTreeFourier ptf3h(cov,6);
  ptf3h.execute(10000000);
  ptf3h.release();
  std::cout<<'\n'<<"=========================="<<std::endl;
  cov = getCovMatrix(15);
  //for(int i=0; i<12;i++){}
  ProbTree pt4(cov);
  pt4.execute(10000000);
  pt4.release();

  //for(int i=0; i<12;i++){}
  ProbTreeFourier ptf4(cov,15);
  ptf4.execute(10000000);
  ptf4.release();
  ProbTreeFourier ptf4h(cov,8);
  ptf4h.execute(10000000);
  ptf4h.release();
  /*/
  Eigen::MatrixXd cov = getCovMatrix(15);
  ProbTree pt4(cov);
  pt4.execute(1);
  pt4.execute(1);
  pt4.release();
  ProbTreeFourier ptf4(cov,15);
  ptf4.execute(10000000);
  //ptf4.execute(1);
  ptf4.release();

  

  // Experiment 
  /*
  
  Eigen::MatrixXd cov11 = getCovMatrix(16);
  for(int j=1;j<=12;j++){
    ProbTreeFourier ptf(cov11,j);
    std::cout<<10<<','<<j<<'\n';
    double time = ptf.execute(1000000);
    ptf.release();
    std::cout<<time<<'\n'<<"=========================="<<std::endl;
  }
  //*/

  /*
  //string filename = "Fourier_order_dimension_time_experiment.dat";
  //std::ofstream txt;
  //txt.open(std::string("..") + SEPERATOR + "data" + SEPERATOR + filename, fstream::trunc);

  ofstream dat;
  dat.open("experiment_0-15_1.dat");
  if (dat.is_open()) { 
    for(int i=1; i<15; i++){
      Eigen::MatrixXd cov = getCovMatrix(i);
      for(int j=1;j<=i;j++){
        ProbTreeFourier ptf(cov,j);
        double time = ptf.execute(10000000);  // generate 10000000 samples
        //double time = ptf.execute(1);         // generate 1 samples
        //double time2 = ptf.execute(1); 
        //dat<<i<<' '<<j<<' '<<time<<' '<<time2<<'\n';
        //std::cout<<i<<' '<<j<<' '<<time<<' '<<time2<<'\n';
        std::cout<<i<<' '<<j<<' '<<time<<'\n';
        //std::cout<<"=========================="<<std::endl;

        ptf.release();

        if(time>200){
          break;
        }
        //txt<<i<<','<<j<<','<<time<<'\n';
      }
    }
    dat.close();
  }
  //txt.close();
  //*/
  
}
