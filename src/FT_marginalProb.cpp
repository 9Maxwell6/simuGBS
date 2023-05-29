#include "FT_marginalProb.h"

using namespace std;
 
/**
 * initialzation of ProbTree object
 **/
ProbTreeFourier::ProbTreeFourier(Eigen::MatrixXd cov, int order)//:ProbCalculator()
{
    this->Fourier_order = order;
    this->cov = cov;
    this->d = cov.rows()/2;
    this->cov_sets = new Eigen::MatrixXd[this->d];

    //this->cov_1_to_d();
    
    // initialize the root node with probability = 1
    /*
    node *p = new node;
    p->prob = 1;
    p->zero = NULL;
    p->one = NULL;
    root = p;
    */
    root = new node;
    root->prob = 1;
    root->zero = NULL;
    root->one = NULL;

    this->construct(cov.rows()/2, 0);
}



/**
 * \brief get the probability of the node
 * 
 * should only be called when you sure this node has assigned a prob 
 * 
 **/
double ProbTreeFourier::get_node_prob(vector<int> bits){
    node *ptr = root;
    for (int i: bits){
        if (i == 0 && ptr->zero){
            ptr = ptr->zero;
        }else if (i == 1 && ptr->one){
            ptr = ptr->one;
        }else{
            throw std::invalid_argument("invalid bit-string");
        }
    }

    return ptr->prob;

}

double ProbTreeFourier::execute(int n){

    string filename = "simuGBS_" + std::to_string(this->d)+"d_"+std::to_string(n)+"samples_Fourier_"+std::to_string(Fourier_order)+".csv";
    std::ofstream txt;
    //txt.open(std::string("..") + SEPERATOR + "data" + SEPERATOR + filename, fstream::trunc);
    
        
    //txt << "samples\n";
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    //#pragma omp parallel for schedule(runtime)
    for(int i=0; i<n; i++){
        // generate one sample
        string sample = loop();
        // record the sample
        //#pragma omp critical
        //txt << sample << '\n';
    }
    // display the total time cost
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    
    this->time_cost = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
    //this->time_cost = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    cout.precision(9);
    std::cout << "Time cost = " << this->time_cost* pow(10,-9) << "[s]" << std::endl;
    //std::cout << "Time cost = " << this->time_cost* pow(10,-6) << "[s]" << std::endl;
    
    //txt.close();

    
    return this->time_cost* pow(10,-9);
}

string ProbTreeFourier::loop(){
    // root has default probability 1
    node *ptr = root;
    int depth = 0;
    vector<int> bits;
    string bit_string = "";
    //while(ptr->zero){
    
    while(depth<d){
        double prob0 = ((double) rand() / (RAND_MAX)); // generate random number between 0-1
        // if this child node not computed before
        if(ptr->zero->prob == -1){
            double temp = 0;

            vector<vector<int>> ss;
            vector<int> bits_buffer;
            generateAllBitStrings(bits.size(), bits_buffer,0,Fourier_order, &ss);
            #pragma omp parallel for schedule(runtime)
            for(vector<int> s:ss){
                int sign = pow(-1,dot_product(bits,s));

                if (std::accumulate(s.begin(), s.end(),decltype(s)::value_type(0))<Fourier_order){
                    #pragma omp critical
                    s.push_back(1);
                }else{
                    continue;
                }

                for(int i=s.size();i<d; i++ ){
                    #pragma omp critical
                    s.push_back(0);
                }

                if (map_Fourier.find(s) == map_Fourier.end()) {
                    double fc = fourier_coefficient(s,cov);
                    #pragma omp critical
                    map_Fourier[s] = fc;
                    temp += sign*fc;
                }else{
                    temp += sign*map_Fourier[s];
                }


            }

            //#pragma omp critical
            //bits.push_back(0);
            ptr->zero->prob = 0.5*(ptr->prob + pow(2,d-bits.size())*temp);
            //std::cout<< ptr->zero->prob<<std::endl;
            //std::cout<<pow(2,d-bits.size())<<std::endl;
            //std::cout<<temp<<std::endl;
            //bits.pop_back();
            ptr->one->prob = ptr->prob - ptr->zero->prob;
        }
        
        if(prob0 < ptr->zero->prob/ptr->prob){
        //if(prob0 <0.5){
            #pragma omp critical
            bits.push_back(0);
            bit_string += "0";
            ptr = ptr->zero;
        }else{
            #pragma omp critical
            bits.push_back(1);
            bit_string += "1";
            ptr = ptr->one;
        }
        depth += 1;
    }

    //std::cout<< bit_string<<std::endl;
    //std::cout<< ptr->prob<<std::endl;
    //std::cout<< "================="<<std::endl;
    return bit_string;

    
}

/**
 * \brief construct the structure of the binary tree based on the dimension of output
 * 
 **/
void ProbTreeFourier::construct(int d, int i, node *ptr){
    if(i == d){
        return;
    }else if (ptr == NULL){
        ptr = root;
    }

    ptr->zero = new node;
    ptr->one  = new node;
    ptr->zero->prob = -1;
    ptr->zero->one = nullptr;
    ptr->zero->zero = nullptr;
    ptr->one->prob = -1;
    ptr->one->one = nullptr;
    ptr->one->zero = nullptr;

    this->construct(d, i+1, ptr->zero);
    this->construct(d, i+1, ptr->one);
}


/**
 * \brief create all needed sub-covariance matrix and store them to cov_sets
 * 
 **/
void ProbTreeFourier::cov_1_to_d(){
    vector<int> idx;
    for(int i=0; i<d; i++){
        idx.push_back(i);
        idx.push_back(i+d);
        std::sort(idx.begin(),idx.end());
        cov_sets[i] = cov(idx,idx);
    }
}







void ProbTreeFourier::release(){
    delete root;
    //delete root; root has destructor
    //delete[] cov_sets;
}




