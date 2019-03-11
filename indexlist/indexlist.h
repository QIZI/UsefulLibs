#include <vector>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <cstddef>

template<class T>
class IndexList;

template<class T>
struct IndexNode{
    public:

    void setPrevious(size_t previous){_previous = previous;}
    void setNext(size_t next){_next = next;}

    size_t getPrevious()    const {return _previous;}
    size_t getNext()        const {return _next;}

    T& getData(){return _data;}

    template <class... Args>
    IndexNode(size_t previous, size_t next, Args&&... args) : _previous(previous),_next(next),_data(args...) {}
    IndexNode(size_t previous, size_t next, const T& data)  : _previous(previous),_next(next), _data(data){}
    IndexNode(size_t previous = 0, size_t next = 0)         : _previous(previous),_next(next){}

    bool operator == (const IndexNode& other){
        return (_previous == other.getPrevious() && _next == other.getNext()); 
    }

    private:
    size_t _previous;
    size_t _next;
    T _data;

    friend class IndexList<T>;
};

template <class T, bool isReverse = false>
class IndexIterator{
    public:
    using difference_type   = std::ptrdiff_t;
    using value_type        = std::remove_cv_t<T>;
    using pointer           = T*;
    using reference         = T&;
    using iterator_category = std::bidirectional_iterator_tag;

    IndexIterator() : _iList(nullptr),_current(0){}

    IndexIterator& operator = (const IndexIterator& it){
        _iList      = it._iList;
        _current    = it._current;
        return *this;
    }

    bool operator != (const IndexIterator& it){
        return (&_iList->_pool[_current] != &_iList->_pool[it._current]);
    }

    bool operator == (const IndexIterator& it){
        return (&_iList->_pool[_current] == &_iList->_pool[it._current]);
    }    

  
    IndexIterator& operator++(){ 
        if constexpr(isReverse == true){
            _current = _iList->_pool[_current].getPrevious();
        }
        else{
            _current = _iList->_pool[_current].getNext();
        }
        return *this;
    }
    IndexIterator& operator--(){ 
        if constexpr (isReverse == true){
            _current = _iList->_pool[_current].getNext();
        }
        else{
            _current = _iList->_pool[_current].getPrevious();
        }
        return *this;
    }

    IndexIterator operator--(int){
        auto it = *this;
        --(*this);
        return it;
    }

    IndexIterator operator++(int){
        auto it = *this;
        ++(*this);
        return it;
    }

    IndexIterator operator+(size_t advance) const{
        auto it = *this;
        for(; advance > 0; --advance){
            ++it;
        }
        return it;
    }

    IndexIterator operator-(size_t advance) const{
        auto it = *this;
        for(; advance > 0; --advance){
            --it;
        }
        return it;
    }
   
    T& operator*(void){
        return _iList->_pool[_current].getData();
    }

    size_t getPreviousIndex() const{return _iList->_pool[_current].getPrevious();}
    
    size_t getCurrentIndex() const {return _current;}
    
    size_t getNextIndex() const {return _iList->_pool[_current].getNext();}
    
    
    private:
    IndexList<T>* _iList;
    size_t _current;

    IndexIterator(IndexList<T>* iList, size_t index) : _iList(iList), _current(index) {}
    friend class IndexList<T>;
};
template <class T>
using ReverseIndexIterator = IndexIterator<T,true>;

template <class T>
class IndexList{
    
    public:

    IndexList(){_pool.emplace_back();}
    IndexIterator<T> begin(){
        return IndexIterator<T>(this,_pool[endIndex].getNext());
    } 
    IndexIterator<T> end(){
        return IndexIterator<T>(this,endIndex);
    }
    ReverseIndexIterator<T> rbegin(){
        return ReverseIndexIterator<T>(this,_pool[endIndex].getPrevious());
    } 
    ReverseIndexIterator<T> rend(){
        return ReverseIndexIterator<T>(this,endIndex);
    }

    T& front(){
        return _pool[_pool[endIndex].getNext()].getData();
    }

    T& back(){
        return _pool[_pool[endIndex].getPrevious()].getData();
    } 

    void reserve(size_t nSize){
        _pool.reserve(nSize);
    }

    IndexIterator<T> insert(IndexIterator<T> it, const T& data){
        const size_t current        = it.getCurrentIndex();
        const size_t currentNext    = _pool[current].getNext();
        size_t newIndex             = _pool.size();

        if(_eraseListBegin == endIndex){
            _pool.push_back(IndexNode<T>(current, currentNext, data));
        }
        else{
            newIndex        = _eraseListBegin;
            _eraseListBegin = _pool[_eraseListBegin].getNext();
            _pool[newIndex] = IndexNode<T>(current, currentNext, data);

        }   
        _pool[currentNext].setPrevious(newIndex);
        _pool[current].setNext(newIndex);
        
        _size++;

        return IndexIterator<T>(this, newIndex);
        
       // _pool[current].set
        
    }

    IndexIterator<T> push_front(const T& data){
        return insert(end(),data);
    }

    IndexIterator<T> push_back(const T& data){
        return insert(IndexIterator<T>(this, _pool[endIndex].getPrevious()),data);
    }

    
    

    template <class... Args>
    IndexIterator<T> emplace(IndexIterator<T> it, Args&&... args){
        const size_t current        = it.getCurrentIndex();
        const size_t currentNext    = _pool[current].getNext();
        size_t newIndex             = _pool.size();
        
        
        if(_eraseListBegin == emptyEraseList){
            _pool.emplace_back(IndexNode<T>(current, currentNext, args...));
        }
        else{
            newIndex        = _eraseListBegin;
            _eraseListBegin = _pool[_eraseListBegin].getNext();
            _pool[newIndex] = IndexNode<T>(current, currentNext, args...);
             
        }

        _pool[currentNext].setPrevious(newIndex);
        _pool[current].setNext(newIndex);
        
        _size++;
        
        return IndexIterator<T>(this, newIndex);
    }

    template <class... Args>
    IndexIterator<T> emplace_front(Args&&... args){
        return emplace(end(),args...);
    }

    template <class... Args>
    IndexIterator<T> emplace_back(Args&&... args){
       
        return emplace(IndexIterator<T>(this, _pool[endIndex].getPrevious()),args...);
    }

    
    void erase(IndexIterator<T> it){
        if(it != end()){
            const size_t current    = it.getCurrentIndex();
            const size_t previous   = _pool[current].getPrevious();
            const size_t next       = _pool[current].getNext();
            
            _pool[next].setPrevious(previous);
            _pool[previous].setNext(next);

            _pool[current] = IndexNode<T>(0,0,{});//_pool[current] = IndexNode<T>(0,0,{});
            _pool[current].setNext(_eraseListBegin);
            _pool[current].setPrevious(current);

            _eraseListBegin = current;
            //_pool[current].~IndexNode();
            //std::swap(_pool[current],IndexNode<T>{});
            
            

            _size--;
        }
    }

    void pop_front(){
        erase(begin());
    }

    void pop_back(){
        erase(IndexIterator<T>(this, _pool[endIndex].getPrevious()));
    }
    
    void clear(){
        while(begin() != end()){
            pop_front();
        }
    }

    void resize(size_t newSize){
        if(_size < newSize){
            for(size_t i = 0, sizeDiff = newSize - _size ; i < sizeDiff; i++){
                emplace_back();
            }
        }
        else if(_size > newSize){
            for(size_t i = 0, sizeDiff = _size - newSize ; i < sizeDiff; i++){
                pop_back();
            }
        }
        
    }

    void shrink_to_fit(){
        size_t current = _eraseListBegin;
        size_t swapableSpece = _pool.size()-1;
        while(current != emptyEraseList){
            size_t next = _pool[current].getNext();
            
            while(swapableSpece > endIndex){
                if(current >= swapableSpece){
                    swapableSpece--;
                    break;
                }
                else if(isNodeErased(_pool[swapableSpece])){
                    swapableSpece--;
                }
                else{
                    std::swap(_pool[current],_pool[swapableSpece]);
                    _pool[swapableSpece].setPrevious(swapableSpece);
                    swapableSpece--;

                    _pool[_pool[current].getPrevious()].setNext(current);
                    _pool[_pool[current].getNext()].setPrevious(current);
                    break;
                }
            } 
            
            
            current = next;
        }
        _eraseListBegin = emptyEraseList;
        _pool.resize(_size+1);
        _pool.shrink_to_fit();
    }
    
    void reorder(){
                
        std::sort(_pool.begin() + 1, _pool.end(), [this](const auto& a, const auto& b){
            return !isNodeErased(a);
        });
        _eraseListBegin = emptyEraseList;
        for(size_t index = 1; index < _pool.size(); ++index){
            
            
            if(index <= _size){
                _pool[index]._next = index + 1;
                _pool[index]._previous = index - 1; 
            }
            else{
                _pool[index]._previous = index;
                if(_eraseListBegin == emptyEraseList){
                    _eraseListBegin = index;
                    _pool[index]._next = 0;
                }
                else{
                    _pool[index]._next = _eraseListBegin;
                    _eraseListBegin = index;
                }

            }
        }
        _pool[endIndex].setPrevious(_size);
        _pool[endIndex].setNext(endIndex+1);
        _pool[_size].setNext(endIndex);
        
    }


    size_t size(){
        return _size;
    }
    size_t capacity(){
        return (_pool.capacity() - 1);
    }

    bool empty(){
        return (_size == 0);
    }

    
    T& operator [] (size_t index){
        
        return _pool[index + (!index)].getData();    
    }
    ~IndexList(){

    }
    bool isNodeErased(const IndexNode<T>& node){
        return (_pool[node.getPrevious()] == node);
    }
    size_t getNodeIndex(const IndexNode<T>& node){
        return _pool[node.getPrevious()].getNext();
    }

    std::vector< IndexNode<T>> _pool;
    //private:

    constexpr static size_t endIndex        = 0;
    constexpr static size_t emptyEraseList  = 0;
    size_t _eraseListBegin = 0;
    size_t _size = 0;

    friend class IndexIterator<T>;
};
