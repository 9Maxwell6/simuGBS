
#ifndef Node

#define Node


struct node{
    node *zero;
    node *one;
    //node *previous;
    double prob;

    // Destructor to delete the tree
    ~node(){
        //std::cout<< "111"<<std::endl;
        //std::cout<< (this->zero != nullptr)<<std::endl;
        if (this->one != nullptr) {
            delete this->one;
        }
        if (this->zero != nullptr) {
            delete this->zero;
        }
        //delete this;
        
        //destroy(this);
        /*
        if(zero)
            delete zero;
            zero = NULL;
        if(one)
            delete one;
            one = NULL;
        */
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

#endif