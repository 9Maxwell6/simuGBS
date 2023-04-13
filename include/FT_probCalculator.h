#ifndef FT_probCalculator_H   // #include guards
#define FT_probCalculator_H

#include <vector>
#include <string>
#include <iostream>

#include <eigen3/Eigen/Dense>

#include "vectorUtils.h"


using namespace std;






double fourier_coefficient(vector<int> s, Eigen::MatrixXd cov);

double FT_detection_prob(vector<int> bitstring, Eigen::MatrixXd cov, int highest_order);

void generateAllBitStrings(int d, vector<int> bits, int i, int num_1, vector<vector<int>>* s);


void display_outputs(vector<vector<int>> output_list);

#endif