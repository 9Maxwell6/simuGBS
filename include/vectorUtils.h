#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H
#include <vector>

using namespace std;

template<typename T>
T dot_product(vector<T> v1, vector<T> v2){
    assert(v1.size()==v2.size());
    T product = 0;
 
    // Loop for calculate dot product
    for (int i = 0; i < v1.size(); i++)
        product +=  v1[i] * v2[i];
    return product;
}

#include "../src/utils/vectorUtils.cpp"

#endif