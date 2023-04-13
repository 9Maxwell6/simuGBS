#include <string>
#include <iostream>
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


#include <eigen3/Eigen/Dense>

#include "Node.h"
#include "../include/stringUtils.h"
#include "../include/FT_probCalculator.h"



//#include "ProbCalculator.h"

#if defined(_WIN16) | defined(_WIN32) | defined(_WIN64)
#define SEPERATOR "\\"
#else
#define SEPERATOR "/"
#endif

/*
struct node{
    node *zero;
    node *one;
    //node *previous;
    double prob;

    // Destructor to delete the tree
    ~node(){
        destroy(this);
    }

    void destroy(node *n)
    {
        if (n)
        {
            destroy(n->one);
            destroy(n->zero);
            delete n;
        }
    }


};
*/

class ProbTreeFourier //: public ProbCalculator
{
    private:
    int Fourier_order;
    node *root;
    Eigen::MatrixXd cov;
    Eigen::MatrixXd *cov_sets;
    int d;
    map<vector<int>,double> map_Fourier; // Fourier coefficient map
    vector<vector<int>> samples;
    int output_counter = 0;
    double time_cost;

    // generate and return one sample
    string loop();
    void construct(int output_dimension, int idx, node *ptr = NULL);
    void cov_1_to_d();
    double get_node_prob(vector<int> bits);

    public:
    ProbTreeFourier(Eigen::MatrixXd cov, int highest_order);
    std::string getBitString(node *);
    double execute(int num);

    void release();
};

