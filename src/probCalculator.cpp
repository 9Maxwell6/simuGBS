//#include "ProbCalculator.h"
#include "../include/probCalculator.h"




double detection_prob(vector<int> output, Eigen::MatrixXd cov){
    int d = cov.rows()/2;
    std::vector<int> index_1;
    std::vector<int> index_0;
    for (int &i: output){

        if (i){
            // record the index of bit-1
            #pragma omp critical
            index_1.push_back(&i-&output[0]);
        }else{
            // record the index of bit-0
            #pragma omp critical
            index_0.push_back(&i-&output[0]);
            //since the shape of cov matrix is 2dx2d due to phase space quadrature
            // this operation for index_1 will be performed later because some extra operations needed for index_1
            #pragma omp critical
            index_0.push_back(&i-&output[0] + d);
        }
    }

    std::vector<vector<int>> combo_all;
    int n = index_1.size();
    int cap = 1 << n; // 2^n
    for( int i=1; i<cap; ++i )
    {
        vector<int> combo_1;
        for( int j=0; j<n; ++j )
        {
            // construct possible combination between indices of bit-1
            if( i & (1<<j) ){
                #pragma omp critical
                combo_1.push_back(index_1[j]);
                //since the shape of cov matrix is 2dx2d due to phase space quadrature
                #pragma omp critical
                combo_1.push_back(index_1[j] + d);
            }
        }
        // add combination of bit-1 (combo_1) to final possible combination (combo_all)
        #pragma omp critical
        combo_all.push_back(combo_1);
    }

    // append index of bit-0 to every combo
    for (vector<int> &combo: combo_all){
        for (int idx0: index_0){
            #pragma omp critical
            combo.push_back(idx0);
        }   
        #pragma omp critical
        std::sort(combo.begin(), combo.end());
        #pragma omp critical
        combo_all.at(&combo-&combo_all[0]) = combo;
    }
    // add one more combo that only consist of indices of bit-0 
    #pragma omp critical
    combo_all.push_back(index_0);

    double prob = 0;

    for (vector<int> combo: combo_all){
        int sub_cov_d = combo.size(); // dimension of sub cov matrix.  
        Eigen::MatrixXd sub_cov = cov(combo,combo);
        Eigen::MatrixXd I = Eigen::MatrixXd::Identity(sub_cov_d, sub_cov_d);
        // combo.size() and index_0.size() are divided by 2 because we insert extra elements to deal with 2dx2d cov matrix   
   
        if ((combo.size()/2-index_0.size()/2) % 2){  // if odd
            prob = prob - pow(2,combo.size()/2)/sqrt((sub_cov+I).determinant());
        }else{  //if even
            prob = prob + pow(2,combo.size()/2)/sqrt((sub_cov+I).determinant());
        }
        
    }
    /*
    cout << '|';
    for(int i:output){
            cout << i;
        }
    cout.precision(20);
    cout << ">: "<<prob<<'\n';
    */

    return prob;
    
}





void display_outputs(vector<vector<int>> output_list){
    for (vector<int> &output: output_list){
        for (int i: output)
            cout << i << ' ';
        cout << "\n";
    }
}