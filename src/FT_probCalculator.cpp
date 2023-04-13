#include "../include/FT_probCalculator.h"



double fourier_coefficient(vector<int> s, Eigen::MatrixXd cov){
    int d = cov.rows()/2;
    std::vector<int> index_1;
    for (int &i: s){
        if (i){
            // record the index of bit-1
            #pragma omp critical
            index_1.push_back(&i-&s[0]);
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
    //std::vector<int> index_0;
    //combo_all.push_back(index_0);
    //cout<<combo_all.size()<<endl;
    //if(!combo_all.size()){
    std::vector<int> index_0;
    combo_all.push_back(index_0);
    //}
    

    double fourier_coefficient = 0;

    for (vector<int> combo: combo_all){
        int sub_cov_d = combo.size(); // dimension of sub cov matrix.  
        Eigen::MatrixXd sub_cov = cov(combo,combo);
        Eigen::MatrixXd I = Eigen::MatrixXd::Identity(sub_cov_d, sub_cov_d);
        // combo.size() are divided by 2 because we insert extra elements to deal with 2dx2d cov matrix    
        //fourier_coefficient = fourier_coefficient + pow(-1,index_1.size()-combo.size()/2)*pow(2,combo.size()/2)*pow(2,combo.size()/2)/sqrt((sub_cov+I).determinant());
        fourier_coefficient = fourier_coefficient + pow(-1,index_1.size()-combo.size()/2)*pow(2,combo.size())/sqrt((sub_cov+I).determinant());
        //fourier_coefficient = fourier_coefficient + pow(-1,index_1.size()-combo.size()/2)*pow(2,combo.size()/2)/sqrt((sub_cov+I).determinant());
        //fourier_coefficient = fourier_coefficient + pow(-1,index_1.size()-combo.size()/2)/sqrt((sub_cov+I).determinant());

        
        ///*
        //printf("%d",(index_1.size()-combo.size()/2)%2);  
        //cout<<sub_cov<<endl;
        //cout<<1/sqrt((sub_cov+I).determinant())<<endl;
        //cout<<"--------------------"<<endl;
        /*
        if ((index_1.size()-combo.size()/2) % 2){  // if odd
            //fourier_coefficient = fourier_coefficient - pow(2,combo.size()/2)/sqrt((sub_cov+I).determinant());
            fourier_coefficient = fourier_coefficient - 1/sqrt((sub_cov+I).determinant());

        }else{  //if even
            //fourier_coefficient = fourier_coefficient + pow(2,combo.size()/2)/sqrt((sub_cov+I).determinant());
            fourier_coefficient = fourier_coefficient + 1/sqrt((sub_cov+I).determinant());
        }
        */
        
    }


    return fourier_coefficient/pow(2,d);
}



double FT_detection_prob(vector<int> bitstring, Eigen::MatrixXd cov, int highest_order){
    int d = cov.rows()/2;
    vector<vector<int>> ss;
    vector<int> bits;
    generateAllBitStrings(d, bits,0,highest_order, &ss);

    double prob = 0;

    for(vector<int> s: ss){
        
        //double temp = fourier_coefficient(s,cov)*std::pow(-1,dot_product(s,bitstring));

        //if(temp < 0) temp = 1+temp;
        //prob += temp;
        //std::cout<<fourier_coefficient(s,cov)*std::pow(-1,dot_product(s,bitstring))/pow(2,d)<<std::endl;
        prob += fourier_coefficient(s,cov)*std::pow(-1,dot_product(s,bitstring));
        //prob += fourier_coefficient(s,cov)*std::pow(-1,dot_product(s,bitstring))/pow(2,d);
    }

    // deal with 0-order
    //prob += 1/pow(2,d);



    //if(prob < 0) prob = 1+prob;
    return prob;
    
}


/**
 * \brief construct every possible bit-string that has number of 1 less than specific number, 
 *  
 **/
void generateAllBitStrings(int d, vector<int> bits, int i, int num_1, vector<vector<int>>* s){
    if (i == d) {
        #pragma omp critical
        s->push_back(bits);
        //for(int dd:bits){
        //    std::cout<<dd<<std::endl;
        //}
        return;
    }

 
    // First assign "0" at ith position
    // and try for all other permutations
    // for remaining positions
    #pragma omp critical
    bits.push_back(0);
    
    generateAllBitStrings(d, bits, i + 1, num_1, s);

    
    if(num_1 > 0){
        // And then assign "1" at ith position
        // and try for all other permutations
        // for remaining positions
        #pragma omp critical
        bits.back() = 1;
        generateAllBitStrings(d, bits, i + 1, num_1-1, s);
    }
    
}
