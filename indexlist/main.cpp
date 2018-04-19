#include "indexList.h"
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <variant>



static int counter = 0;

int main(){
    //v.emplace("33");
    IndexList<int> il;

    std::cout<<"Sizeof IndexList:" << sizeof(il)<<'\n';
    int a = 5; 
   /* il.push_back(5);
    il.push_back(a+2);
    il.push_back(1);
    il.push_back(5);
    il.push_back(4);
    il.push_back(6);
    */
    //IndexIterator<int> it = il.begin();
    
    il.emplace_back(5);
    il.emplace_back(a+2);
    il.emplace_back(1);
    il.emplace_back(5);
    il.emplace_back(4);
    il.emplace_back(6);
    auto c = []{return 5;};

    for(int i =0 ; i < il._pool.size(); i++)
    {
        printf("pool ele: [%d;%d] data: %d\n", il._pool[i].getPrevious(), il._pool[i].getNext(), il._pool[i].getData());
    }

    std::cout<<"List constains:\n";

 
   // ++it;
   /*
    for(auto it = il.begin();it != il.end(); ++it){
        std::cout<<"Element: "<< *it <<'\n';
    }*/
    
    //std::rotate(il.begin(), il.begin()++, il.end());

    IndexIterator<int> i;
    for(auto it = il.begin();it != il.end(); ++it){
         std::cout<<"Element: "<< *it<<" Index: "<<it.getCurrentIndex() <<'\n';
    }

    il.erase(il.begin());
    il.erase(il.begin());
    auto e = il.end();
    --e;
    il.erase(e);
    
    il.insert(il.begin(),10);    

    std::cout<<"\n";
    /*for (auto it = il.begin();it != il.end(); ++it) {
        
        //std::iter_swap(il.begin(), ++il.begin());
        //std::rotate(il.begin(), il.begin()++, il.end());
        std::rotate(std::upper_bound(il.begin(), it, *it), it, std::next(it,1));
    }*/

    for(auto it = il.begin();it != il.end(); ++it){
        std::cout<<"Element: "<< *it<<" Index: "<<it.getCurrentIndex() <<'\n';
    }
    std::cout<<"\n";

    e = il.end();
    e--;
    e--;
    il.erase(e);
    il.push_front(20); 
    
    //il.clear();
    
    il.push_back(50);
    il.push_back(50);
    il.push_back(50); 
    il.push_back(50);

    

    il.push_back(66);
  
    //il.resize(11);
    il.resize(5);
     il.reorder();
//il.resize(11);
  //  il.shrink_to_fit();
    for(auto it = il.begin();it != il.end(); ++it){
        std::cout<<"Element: "<< *it <<" Index: "<<it.getCurrentIndex()<<'\n';
    }

    for(int i =0 ; i < il._pool.size(); i++)
    {
        printf("pool ele[%d]: [%d;%d] data: %d\n",i, il._pool[i].getPrevious(), il._pool[i].getNext(), il._pool[i].getData());
    }
    std::cout<<"Front: "<< il.front()<< "  Back: "<<il.back() << "  Size: "<<il.size()<<"  Capacity: "<<il.capacity()<<'\n';
}