// 1. Template class that takes in a stack and outputs a vector

template <typename T>
vector <T>stacktovector(stack<T> stack){
    vector<T> result;
    while (!stack.empty()){
        result.push_back(stack.top());
        stack.pop()
    }
    reverse(result.begin(),result.end());
    return result;
}

// 2. get vector output stack

template <typename x>
stack vector_to_stack (vector a[]){
    //iterate through vector elements (for loop) and push them into stack?
    stack<x> b;
    int i = 0;
    for( i; i  < a.size(); ++i){
        int temp;
        temp = a[i];
        b.push(temp);
        //b.push(a[i]);
    }
    return b;
}

using namespace std;
template<typename T>
stack<t> vec_to_stk(const vector<t>& vec){
    stack<T> stk;
    for(vector<T>::iterator ...){
        stk.push(*it);
    }
    reutrn stk;
}


// 3. bidirectional iterator to average a list<double>



// 4. use forward iterator to swap second and 4th element of a forward list

temp = new node;//general ptr

temp->data = head->next->data
head->next->data = head->next->next->next->data
head->next->next->next->data = temp->data;



template <typename T>
void swap2and4(std::forward_list list){
    std::forward_list<T>::iterator second = list.begin()++;
    for (int i = 0; i < 3; ++i){
        std::forward_list<T>::iterator fourth = list.begin()++;
    }
    T temp = *second;
    *second = *fourth;
    *fourth = temp;
}

//5. good data structure for our bookstore
    // List | Vector | Stack 
    // Why | Can choose list of vectors & vectors of list & list of stack (combinations)
/*
List of vectors

Vector of Lists:

*/

