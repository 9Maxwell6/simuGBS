#include <vector>
#include <map>
#include <string>
#include <bits/stdc++.h>
#include <iostream>
#include <algorithm>
#include <math.h> 
#include <fstream>
#include <iomanip>
#include <chrono>

//#include "ProbCalculator.h"
#include "../include/probCalculator.h"
#include "../include/FT_probCalculator.h"

#include <eigen3/Eigen/Dense>
using namespace std;


#if defined(_WIN16) | defined(_WIN32) | defined(_WIN64)
#define SEPERATOR "\\"
#else
#define SEPERATOR "/"
#endif


class DirectProb //: public ProbCalculator
{
    private:
    bool useFourier;
    int Fourier_order;
    Eigen::MatrixXd cov;
    int d; // dimension
    double time_cost;
    vector<vector<int>> output_list;
    vector<double> output_prob;
    //map<string,double> output_probs;
    map<vector<int>,double> output_probs;

    void generateAllBitStrings(int d, vector<int> arr, int i);

    public:
    DirectProb(Eigen::MatrixXd cov, int Fourier_order=0, bool useFourier=false);
    void calculate_probe();
    //double detection_prob(vector<int> bitString);
    void write_csv();
}; 