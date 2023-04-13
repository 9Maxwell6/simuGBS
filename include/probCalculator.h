#include <vector>
#include <map>
#include <string>
#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <chrono>


#include <eigen3/Eigen/Dense>

using namespace std;


#ifndef ProbCalculator_H   // #include guards
#define ProbCalculator_H


double detection_prob(vector<int> bitString, Eigen::MatrixXd cov);


void display_outputs(vector<vector<int>> output_list);

#endif