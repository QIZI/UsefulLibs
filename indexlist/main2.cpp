#include <iostream>
#include "indexlist.h"
template <typename T>
void printList(IndexList<T> list, bool raw = true){
    std::cout<<"\n\nPool Size: "<<list._pool.size()<<" Pool Capacity: "<<list._pool.capacity()<<'\n';
    std::cout<<"Size: "<<list.size()<<" Capacity: "<<list.capacity()<<'\n'<<'\n';
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
}

int main(){

    IndexList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);
    list.push_back(6);


    printList(list);

    list.erase(list.begin()+3);
    list.erase(list.begin()+3);

 printList(list);
        list.push_back(7);
        list.push_back(8);
        list.push_back(9);
  printList(list);
  
  list.erase(list.begin()+1);
    list.erase(list.begin()+3);
    list.erase(list.begin());
    list.erase(list.end()-1);

    printList(list);
    //list.shrink_to_fit();
    list.reorder();
    
printList(list,false);

    for(auto rit = list.rbegin(); rit != list.rend(); rit++){
        std::cout<<"Value: "<<*rit<<std::endl;
    }
}