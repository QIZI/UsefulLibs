#include "../indexlist.h"


#include <iostream>
#include <chrono>
#include <inttypes.h>
#include <thread>
#include <list>
#include <forward_list>

template <typename T>
void printList(IndexList<T> list, bool raw = true){

    if(raw){
        for(int i = 0; i < list._pool.size(); i++){
            std::cout<<"Pool Value["<<i<<"]: "<<list._pool[i].getData()<<" ["<<list._pool[i].getPrevious()<<"|"<<list._pool[i].getNext()<<"]";
            if(i == list._pool[i].getPrevious()){
                std::cout<<"(deleted)\n";
            }
            else{
                std::cout<<"\n";
            }
        }
    }
    else{
        for(auto it = list.begin(); it != list.end(); it++){
            std::cout<<"List Value["<<it.getCurrentIndex()<<"]: "<<*it<<" ["<<it.getPreviousIndex()<<"|"<<it.getNextIndex()<<"]\n";
        }
        if(list._eraseListBegin != 0){
           std::cout<<"\n";
            size_t current = list._eraseListBegin;
            do{
                std::cout<<"Erased Value["<<current<<"]: "<<list._pool[current].getData()<<" ["<<list._pool[current].getPrevious()<<"|"<<list._pool[current].getNext()<<"]\n";
                current = list._pool[current].getNext();
            }
            while(current != 0);
        }

    }
        std::cout<<"\n\nPool Size: "<<list._pool.size()<<" Pool Capacity: "<<list._pool.capacity()<<'\n';
    std::cout<<"Size: "<<list.size()<<" Capacity: "<<list.capacity()<<'\n'<<'\n';
}


namespace chrono = std::chrono;

struct A{
    uint32_t _[10];
    A(uint32_t a) {_[0] = a;} 
    A(){}
};

int main(){
    
    IndexList<uint32_t> list;
    std::vector<uint32_t> vec;
    std::forward_list<uint32_t> realThing; 

    list.reserve(400000); 
    vec.reserve(400000);

    //IndexList<A> list;
    auto start =  chrono::high_resolution_clock::now();
     auto end = chrono::high_resolution_clock::now();

    start =  chrono::high_resolution_clock::now();
    for(size_t count = 0; count < 400000; count++){
        list.emplace_back();
    }
   
    end = chrono::high_resolution_clock::now();
    auto timeDiff = chrono::duration_cast<chrono::microseconds>(end - start).count();

    std::cout<<"ListEmplaceBack "<<std::endl;
    std::cout<<"Time difference: "<<timeDiff<<" us\n";
    
    //auto it = list.begin();
    start =  chrono::high_resolution_clock::now();

    for(size_t count = 0; count < 400000; count++){
        vec.push_back(0);
    }
    end = chrono::high_resolution_clock::now();

    timeDiff = chrono::duration_cast<chrono::microseconds>(end - start).count();

    std::cout<<"VectorEmplaceBack "<<std::endl;
    std::cout<<"Time difference: "<<timeDiff<<" us\n";


    start =  chrono::high_resolution_clock::now();

    for(size_t count = 0; count < 400000; count++){
        vec.push_back(0);
    }
    end = chrono::high_resolution_clock::now();

    timeDiff = chrono::duration_cast<chrono::microseconds>(end - start).count();

    std::cout<<"RealListEmplaceBack "<<std::endl;
    std::cout<<"Time difference: "<<timeDiff<<" us\n";
    
    #ifdef ENABLE
  //  printList(list);
    
    while(it != list.end()){
        auto tmp = it;
        it = it + 7;
        list.erase(tmp);
        
    }    
    #endif
    /*while(it != list.end()){
        auto tmp = it;
        it=it+2;
        list.erase(tmp);
    }*/

    
    //list.reorder();
    list.shrink_to_fit();
    //std::this_thread::sleep_for(chrono::milliseconds(20));

    



}