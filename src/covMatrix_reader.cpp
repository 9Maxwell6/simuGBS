#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <eigen3/Eigen/Dense>
#include "../include/stringUtils.h"
using namespace std;


/** 
  * \brief read covariance matrix in /data generated by covMatrix_generator.py and convert it to eigen3 matrix.
  *
  * @return The covariance matrix
  */
Eigen::MatrixXd getCovMatrix(int d) {
  vector<string> seglist;
  fstream covFile;
  if(d==0){
    covFile.open("../data/covMatrix.txt",ios::in);
  }else{
    covFile.open("../data/covMatrix_"+std::to_string(d)+"d.txt",ios::in);
  }
  if (covFile.is_open()){   
    string row;
    
    while(getline(covFile, row)){
      seglist.push_back(row);
    }
    covFile.close();
  }

  int nDim = seglist.size();

  // define covarience matrix
  Eigen::MatrixXd cov(nDim,nDim);
  

  for (string &i: seglist){
    std::vector<std::string> rowlist = split(i,' ');
    for (string &j: rowlist){
      double temp = stod(j);
      cov(&i - &seglist[0],&j - &rowlist[0]) = temp;
    }
  }

  /*
  display matrix
  */
  //Eigen::IOFormat OctaveFmt(Eigen::StreamPrecision, 0, ", ", ";\n", "", "", "[", "]");
  //std::cout << cov.format(OctaveFmt) << std::endl;
  return cov;
}