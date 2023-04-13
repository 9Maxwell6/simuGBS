#include <vector>
#include <map>
#include <string>
#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <chrono>

#include <eigen3/Eigen/Dense>

using namespace std;

#if defined(_WIN16) | defined(_WIN32) | defined(_WIN64)
#define SEPERATOR "\\"
#else
#define SEPERATOR "/"
#endif

#ifndef ProbCalculator_H   // #include guards
#define ProbCalculator_H

/**
 * deprecated class, has been replaced probCalculator.h (function librilary)
 * possibly reuse
 * 
 **/
class ProbCalculator
{
    protected:
    //Eigen::MatrixXd cov;
    //int d; // dimension
    double detection_prob(vector<int> bitString, Eigen::MatrixXd cov);

    public:
    //ProbCalculator(Eigen::MatrixXd cov);
    ProbCalculator() = default;
    void display_outputs(vector<vector<int>> output_list);
};

#endif