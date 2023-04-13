#include "marginalProb.h"

using namespace std;
 
/**
 * initialzation of ProbTree object
 **/
ProbTree::ProbTree(Eigen::MatrixXd cov)//:ProbCalculator()
{
    this->cov = cov;
    this->d = cov.rows()/2;
    this->cov_sets = new Eigen::MatrixXd[this->d];

    this->cov_1_to_d();
    
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
double ProbTree::get_node_prob(vector<int> bits){
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

void ProbTree::execute(int n, bool record_time){

    

    /*
    #pragma omp parallel
    //while(output_counter != 1 << d){   // check if the number of outputs we have equal to 2^d

    while(output_probs.size() < 1 << d){   // check if the number of outputs we have equal to 2^d
        //cout<<output_counter<<endl;
        //cout<<(1 << d)<<endl;
        vector<int> sample = loop();
        #pragma omp critical
        this->samples.push_back(sample);
        //cout<<output_counter<<'\n';
        //cout<<"cnm"<<endl;
    }
    */

    string filename = "simuGBS_" + std::to_string(this->d)+"d_"+std::to_string(n)+"samples.csv";
    std::ofstream txt;
    txt.open(std::string("..") + SEPERATOR + "data" + SEPERATOR + filename, fstream::trunc);
    
    if(record_time){
        txt << "samples,time\n";
        // setup timer
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        #pragma omp parallel for schedule(runtime)
        for(int i=0; i<n; i++){
            std::chrono::steady_clock::time_point sample_begin = std::chrono::steady_clock::now();
            // generate one sample
            string sample = loop();
            std::chrono::steady_clock::time_point sample_end = std::chrono::steady_clock::now();

            // record the sample and time cost to generate this sample
            #pragma omp critical
            txt << sample << ',' << std::chrono::duration_cast<std::chrono::microseconds>(sample_end - sample_begin).count() * pow(10,-6) << '\n';
        }

        // display the total time cost
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        this->time_cost = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() * pow(10,-6);
        cout.precision(6);
        std::cout << "ProbTree construction complete, number of sampling: " << this->samples.size()<< std::endl;
        std::cout << "Time cost = " << this->time_cost << "[s]" << std::endl;

    }else{
        
        txt << "samples\n";
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        #pragma omp parallel for schedule(runtime)
        for(int i=0; i<n; i++){
            // generate one sample
            string sample = loop();
            // record the sample
            #pragma omp critical
            txt << sample << '\n';
        }
        // display the total time cost
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        this->time_cost = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() * pow(10,-6);
        cout.precision(6);
        std::cout << "Time cost = " << this->time_cost << "[s]" << std::endl;
    }
    txt.close();

    
    return;
}

string ProbTree::loop(){
    // root has default probability 1
    node *ptr = root;
    int depth = 0;
    vector<int> bits;
    string bit_string = "";
    bool map_flag = false;
    //while(ptr->zero){
    while(depth<d){
        double prob0 = ((double) rand() / (RAND_MAX)); // generate random number between 0-1
        // if this child node not computed before
        if(ptr->zero->prob == -1){
            map_flag = true; // the final result will be a new output bit-string, need to record to map
            #pragma omp critical
            bits.push_back(0);
            ptr->zero->prob = detection_prob(bits, cov_sets[depth]);
            bits.pop_back();
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

    // store probability of a new bit-string in to the binary tree
    /*
    if(map_flag){
        output_counter+=2;
        #pragma omp critical
        output_probs[bits] = ptr->prob;
        if(bits.back()==0){
            bits.back() = 1;
        }else{
            bits.back() = 0;
        }
        #pragma omp critical
        output_probs[bits] = get_node_prob(bits);
    }
    */

    return bit_string;

    
}

/**
 * \brief construct the structure of the binary tree based on the dimension of output
 * 
 **/
void ProbTree::construct(int d, int i, node *ptr){
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
void ProbTree::cov_1_to_d(){
    vector<int> idx;
    for(int i=0; i<d; i++){
        idx.push_back(i);
        idx.push_back(i+d);
        std::sort(idx.begin(),idx.end());
        cov_sets[i] = cov(idx,idx);
    }
}



void ProbTree::displayBinTree(){
    printBinTree(root);
    cout << '\n' << endl;
}

void ProbTree::printBinTree(node *ptr){
    if(ptr!=NULL){
        printBinTree(ptr->zero);
        cout << "  " << ptr->prob << "     ";
        printBinTree(ptr->one);
    }
}


void ProbTree::release(){
    delete root; //root has destructor
    delete[] cov_sets;
}


void ProbTree::write_csv(){
    std::ofstream csv;
    csv.open(std::string("..") + SEPERATOR + "data" + SEPERATOR + "simuGBS_" + std::to_string(this->d)+"d_marginal.csv", fstream::trunc);
    csv << "marginal\n";
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

/**
 * \brief generate samples.
 * 
 * should be use after the tree is constructed
 * 
 * 
 * 
 * 
 **/
/*
void ProbTree::sampling(int num){
    
    num = num - this->samples.size();
    #pragma omp parallel for schedule(runtime)
    for(int i=0; i<num; i++){
        //cout<<output_counter<<endl;
        //cout<<(1 << d)<<endl;
        vector<int> sample = loop();
        #pragma omp critical
        this->samples.push_back(sample);
        //cout<<std::to_string(i)<<'\n';
    }
}
*/

void ProbTree::load_samples(std::string filename){
    std::ofstream txt;
    if(filename==""){
        filename = "simuGBS_" + std::to_string(this->d)+"d_samples.txt";
    }
    txt.open(std::string("..") + SEPERATOR + "data" + SEPERATOR + filename, fstream::trunc);
    for(vector<int> sample: this->samples){
        for(int i: sample){
            txt << std::to_string(i);
        }
        txt << '\n';
    }
    txt.close();
}