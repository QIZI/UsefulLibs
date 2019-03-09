#include <iostream>
#include "indexlist.h"
template <typename T>
void printList(IndexList<T> list){
    std::cout<<"\n\nPool Size: "<<list._pool.size()<<" Pool Capacity: "<<list._pool.capacity()<<'\n';
    std::cout<<"Size: "<<list.size()<<" Capacity: "<<list.capacity()<<'\n'<<'\n';
    for(int i = 0; i < list._pool.size(); i++){
        std::cout<<"Value["<<i<<"]: "<<list._pool[i].getData()<<" ["<<list._pool[i].getPrevious()<<"|"<<list._pool[i].getNext()<<"]\n";
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
    //list.erase(list.begin()+3);

 printList(list);
        list.push_back(7);
        list.push_back(8);
        list.push_back(9);
  printList(list);

    list.reorder();
printList(list);
}