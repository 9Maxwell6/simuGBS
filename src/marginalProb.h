#include <string>
#include <iostream>

#include <eigen3/Eigen/Dense>

#include "../include/stringUtils.h"
#include "../include/probCalculator.h"
#include "Node.h"

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

class ProbTree //: public ProbCalculator
{
    private:
    node *root;
    Eigen::MatrixXd cov;
    Eigen::MatrixXd *cov_sets;
    int d;
    map<vector<int>,double> output_probs;
    vector<vector<int>> samples;
    int output_counter = 0;
    double time_cost;

    // generate and return one sample
    string loop();
    void construct(int output_dimension, int idx, node *ptr = NULL);
    void cov_1_to_d();
    double get_node_prob(vector<int> bits);

    public:
    ProbTree(Eigen::MatrixXd cov);
    void displayBinTree();
    void printBinTree(node *);
    std::string getBitString(node *);
    void write_csv();
    //void sampling(int num); // not needed
    void load_samples(std::string filename = "");
    void execute(int num,  bool record_time=false);

    void release();
};