#include "DirectProb.h"

DirectProb::DirectProb(Eigen::MatrixXd cov, int Fourier_order, bool useFourier){ //:ProbCalculator(){
    this->useFourier = useFourier;
    this->Fourier_order = Fourier_order;
    this->cov = cov;
    this->d = cov.rows()/2;
    vector<int> bits;
    generateAllBitStrings(d, bits, 0);

    // openMP with eigen3
    Eigen::initParallel();
}


/**
 * \brief construct every output bit-string, store it to ProbCalculator::output_list
 *  
 * @throws invalid_argument if empty input or input string is not bit-string.
 * @return The covariance matrix
 **/
void DirectProb::generateAllBitStrings(int d, vector<int> bits, int i){
    if (i == d) {
        this->output_list.push_back(bits);
        return;
    }
 
    // First assign "0" at ith position
    // and try for all other permutations
    // for remaining positions
    bits.push_back(0);
    
    generateAllBitStrings(d, bits, i + 1);
 
    // And then assign "1" at ith position
    // and try for all other permutations
    // for remaining positions
    bits.back() = 1;
    generateAllBitStrings(d, bits, i + 1);
    
}


/**
 * \brief calculate probability of all possible output bit-string
 * 
 * The algorithm is:
 * 
 * 
 * 
 * for example, given a 4-channel output
 * 1. list every possible output. (0000, 0001, 0010, 0100, 0011, etc.)
 *      for every output, label with index 0,1,2,3
 * 2. store indices of bit-1 and bit-0 in two separate vector
 * 3. for vector with bits-1, find all possible combination.
 *      e.g, output 1101, the vector of bits-1 contain 0,1,3, represents the index of bit-1, 
 *                        the vector of bits-1 contain 2
 *           the possible combination of indices of bit-1 are (0, 01, 03, 13, 013)
 * 4. append indices of bit-0 to every combination above: (02, 012, 032, 132, 0132)
 * 5. add one more combination of only indices of bit-0:  (02, 012, 032, 132, 0132, 2)
 * 6. for every comination, remove rows and columns in the covariance matrix with indices that do not appear in the comination
 * 7. if number of index in a combination represnets bit-1 is odd number, 
 *           subtract (0132 has 3 indices represent bit-1, there is a minus sign)
 *  
 * @throws invalid_argument if empty input or input string is not bit-string.
 * @return The covariance matrix
 **/
void DirectProb::calculate_probe(){

    ///*

    if(useFourier){
        // setup timer
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        #pragma omp parallel for schedule(runtime)
        for (vector<int> &output: output_list){

            double prob = FT_detection_prob(output, this->cov, Fourier_order);
            #pragma omp critical
            this->output_probs[output] = prob;

        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        this->time_cost = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() * pow(10,-6);
        cout.precision(6);
        std::cout << "Time cost = " << this->time_cost << "[s]" << std::endl;

        return;
    }

    // setup timer
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    #pragma omp parallel for schedule(runtime)
    for (vector<int> &output: output_list){
        //stringstream out;  // this is slow!!!
        //copy(output.begin(), output.end(), ostream_iterator<int>(out));
        //this->output_probs[out.str()] = detection_prob(output);
        double prob = detection_prob(output, this->cov);

        #pragma omp critical
        this->output_probs[output] = prob;

        //this->output_probs[output] = detection_prob(output, this->cov);

        //this->output_prob.push_back(detection_prob(output));
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    this->time_cost = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() * pow(10,-6);
    cout.precision(6);
    std::cout << "Time cost = " << this->time_cost << "[s]" << std::endl;

    return;
    
}


/*
double DirectProb::detection_prob(vector<int> output){
    std::vector<int> index_1;
    std::vector<int> index_0;
    for (int &i: output){

        if (i){
            // record the index of bit-1
            index_1.push_back(&i-&output[0]);
        }else{
            // record the index of bit-0
            index_0.push_back(&i-&output[0]);
            //since the shape of cov matrix is 2dx2d due to phase space quadrature
            // this operation for index_1 will be performed later because some extra operations needed for index_1
            index_0.push_back(&i-&output[0] + this->d);
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
                combo_1.push_back(index_1[j]);
                //since the shape of cov matrix is 2dx2d due to phase space quadrature
                combo_1.push_back(index_1[j]+this->d);
            }
        }
        // add combination of bit-1 (combo_1) to final possible combination (combo_all)
        combo_all.push_back(combo_1);
    }

    // append index of bit-0 to every combo
    for (vector<int> &combo: combo_all){
        for (int idx0: index_0){
            combo.push_back(idx0);
        }   
        std::sort(combo.begin(), combo.end());
        combo_all.at(&combo-&combo_all[0]) = combo;
    }
    // add one more combo that only consist of indices of bit-0 
    combo_all.push_back(index_0);

    double prob = 0;

    for (vector<int> combo: combo_all){
        int sub_cov_d = combo.size(); // dimension of sub cov matrix.  
        //Eigen::MatrixXd sub_cov(2*sub_cov_d, 2*sub_cov_d);  // x2 because of x and p quadratures
        // row
        //Eigen::MatrixXd A = Eigen::MatrixXd::Random(2*this->d,2*this->d) ;
        //cout << A(Eigen::all,combo);
        //Eigen::MatrixXd sub_cov = A(Eigen::placeholders::all,combo);
        Eigen::MatrixXd sub_cov = this->cov(combo,combo);
        
        for (int i=0; i<sub_cov_d; i++){ 
            cout << combo[i] ;
            // column
            for (int j=0; j<sub_cov_d; j++){
                sub_cov(i,j) = this->cov(combo[i],combo[j]);
                sub_cov(i+sub_cov_d,j) = this->cov(combo[i]+ProbCalculator::d,combo[j]);
                sub_cov(i,j+sub_cov_d) = this->cov(combo[i],combo[j]+ProbCalculator::d);
                sub_cov(i+sub_cov_d,j+sub_cov_d) =this->cov(combo[i]+ProbCalculator::d,combo[j]+ProbCalculator::d);
            }
        }
        
        Eigen::MatrixXd I = Eigen::MatrixXd::Identity(sub_cov_d, sub_cov_d);
        
        // combo.size() and index_0.size() are divided by 2 because we insert extra elements to deal with 2dx2d cov matrix
        if ((combo.size()/2-index_0.size()/2) % 2){  // if odd
            prob = prob - pow(2,combo.size()/2)/sqrt((sub_cov+I).determinant());
        }else{  //if even
            prob = prob + pow(2,combo.size()/2)/sqrt((sub_cov+I).determinant());
        }
        
    }
    ///*
    
    cout << '|';
    for(int i:output){
            cout << i;
        }
    cout.precision(20);
    cout << ">: "<<prob<<'\n';
    //

    return prob;
    
}

*/



void DirectProb::write_csv(){
    std::ofstream csv;
    if(useFourier){
        csv.open(std::string("..") + SEPERATOR + "data" + SEPERATOR + "simuGBS_" + std::to_string(this->d)+"d_Fourier_"+std::to_string(Fourier_order)+"_direct.csv", fstream::trunc);
    }else{
        csv.open(std::string("..") + SEPERATOR + "data" + SEPERATOR + "simuGBS_" + std::to_string(this->d)+"d_direct.csv", fstream::trunc);
    }
    csv << "direct\n";
    csv << "time cost:," << this->time_cost << "[s]\n";
    csv << "output, probability\n";
    /*
    for (vector<int> &output: this->output_list){
        for (int i: output) csv << std::to_string(i);
        csv << ',' << std::setprecision(20) << this->output_prob[&output - &this->output_list[0]] << '\n';
    }
    */
    //map<string,double>::iterator it = this->output_probs.begin();
    map<vector<int>,double>::iterator it = this->output_probs.begin();
    while(it!=output_probs.end()){
        //  csv << string(it->first) << ',' << std::setprecision(20) << it->second << '\n';
        for (int i: it->first) csv << std::to_string(i);
        csv << ',' << std::setprecision(20) << it->second << '\n';
        ++it;
    }
    csv.close();
}