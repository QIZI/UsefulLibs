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

    size_t getPrevious() const {return _previous;}
    size_t getNext() const {return _next;}

    T& getData(){return _data;}

    //T& operator *(){return _data;}

    template <class... Args>
    IndexNode(size_t previous, size_t next, Args&&... args) : _previous(previous),_next(next),_data(args...) {}
    IndexNode(size_t previous,size_t next, const T& data) : _previous(previous),_next(next), _data(data){}
    IndexNode(size_t previous = 0,size_t next = 0) : _previous(previous),_next(next){}

    bool operator == (const IndexNode& other){
        return (_previous == other.getPrevious() && _next == other.getNext()); 
    }

    private:
    size_t _previous;
    size_t _next;
    T _data;

    friend class IndexList<T>;
};

template <class T>
class IndexIterator{
    public:
    using difference_type = std::ptrdiff_t;
    using value_type = std::remove_cv_t<T>;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::bidirectional_iterator_tag;

    IndexIterator() : _iList(nullptr),_current(0){}

    IndexIterator& operator = (const IndexIterator<T>& it){
        _iList      = it._iList;
        _current    = it._current;
        return *this;
    }

    bool operator != (const IndexIterator<T>& it){
        return (&_iList->_pool[_current] != &_iList->_pool[it._current]);
    }

    bool operator == (const IndexIterator<T>& it){
        return (&_iList->_pool[_current] == &_iList->_pool[it._current]);
    }    

  
    IndexIterator& operator++(){ 
        _current = _iList->_pool[_current].getNext();
        return *this;
    }
    IndexIterator& operator--(){ 
        _current = _iList->_pool[_current].getPrevious();
        return *this;
    }

    IndexIterator operator--(int){
        const size_t previus = _current;
        --(*this);
        return IndexIterator(_iList, previus);
    }

    IndexIterator operator++(int){
        const size_t previus = _current;
        ++(*this);
        return IndexIterator(_iList, previus);
    }

    IndexIterator operator+(size_t v){
        auto remembered = _current;
        for(; v > 0; --v)
            ++(*this);
        auto advanced = _current;
        _current = remembered;
        return IndexIterator(_iList, advanced);
    }

    IndexIterator operator-(size_t v){
        auto remembered = _current;
        for(; v > 0; --v)
            --(*this);
        auto advanced = _current;
        _current = remembered;
        return IndexIterator(_iList, advanced);
    }
   
    T& operator * (void){
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
class IndexList{
    
    public:

    IndexList(){_pool.emplace_back();}
    IndexIterator<T> begin(){
        return IndexIterator<T>(this,_pool[_endIndex].getNext());
    } 
    IndexIterator<T> end(){
        return IndexIterator<T>(this,_endIndex);
    }

    T& front(){
        return _pool[_pool[_endIndex].getNext()].getData();
    }

    T& back(){
        return _pool[_pool[_endIndex].getPrevious()].getData();
    } 

    void reserve(size_t nSize){
        _pool.reserve(nSize);
    }

    IndexIterator<T> insert(IndexIterator<T> it, const T& data){
        const size_t current        = it.getCurrentIndex();
        const size_t currentNext    = _pool[current].getNext();
        size_t newIndex             = _pool.size();

        if(_removeListBegin == _endIndex){
            _pool.push_back(IndexNode<T>(current, currentNext, data));
        }
        else{
            newIndex        = _removeListBegin;
            _removeListBegin = _pool[_removeListBegin].getNext();
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
        return insert(IndexIterator<T>(this, _pool[_endIndex].getPrevious()),data);
    }

    
    void setEndValue(T&& value){
        _pool[_endIndex]._data = value;
    }

    template <class... Args>
    IndexIterator<T> emplace(IndexIterator<T> it, Args&&... args){
        const size_t current        = it.getCurrentIndex();
        const size_t currentNext    = _pool[current].getNext();
        size_t newIndex             = _pool.size();
        
        
        if(_removeListBegin == _endIndex){
            _pool.emplace_back(IndexNode<T>(current, currentNext, args...));
        }
        else{
            newIndex        = _removeListBegin;
            _removeListBegin = _pool[_removeListBegin].getNext();
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
       
        return emplace(IndexIterator<T>(this, _pool[_endIndex].getPrevious()),args...);
    }

    
    void erase(IndexIterator<T> it){
        if(it != end()){
            const size_t current    = it.getCurrentIndex();
            const size_t previous   = _pool[current].getPrevious();
            const size_t next       = _pool[current].getNext();
            
            _pool[next].setPrevious(previous);
            _pool[previous].setNext(next);

            _pool[current] = IndexNode<T>(0,0,{});
            _pool[current].setNext(_removeListBegin);
            _pool[current].setPrevious(current);

            _removeListBegin = current;
            //_pool[current].~IndexNode();
            //std::swap(_pool[current],IndexNode<T>{});
            
            

            _size--;
        }
    }

    void pop_front(){
        erase(begin());
    }

    void pop_back(){
        erase(IndexIterator<T>(this, _pool[_endIndex].getPrevious()));
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
        //TBI
        if(_removeListBegin != 0){
            for(size_t index = 0; index < _pool.size(); index++){
                size_t current = _pool[index].getNext();
                if(current != (index + 1)){
                    std::swap(_pool[current], _pool[index + 1]);
                    
                    const size_t previous   = _pool[current].getPrevious();
                    const size_t next       = _pool[current].getNext();

                    _pool[previous].setNext(current);
                    _pool[next].setPrevious(current);   
                    
                    
                    _pool[index].setNext(index + 1);
                }
            }
            
            
            
            _pool.resize(_size);
            _pool.shrink_to_fit();

            _removeListBegin = 0;
        }
    }

    void reorderE(){
        auto isErased = [this](const IndexNode<T>& node){ 
            return (_pool[node.getPrevious()] == node);
        };
        /*std::sort(_pool.begin() + 1, _pool.end(), [&](const auto& a, const auto& b){
            


            return _pool[a.getPrevious()].getNext() < _pool[b.getPrevious()].getNext();
        });

        for(int i = 1; i < _pool.size(); ++i){
            _pool[i]._next = i + 1;
            _pool[i]._previous = i - 1; 
        }

        _pool.back()._next = 0;*/
    }

    void reorder(){ //for better cache locality
        //TBI
        /*for(size_t index = 0; index < _pool.size()-1; index++){
            size_t current = _pool[index].getNext();
           
            
            if( current != (index + 1)){
                std::swap(_pool[current], _pool[index + 1]);
                    
                const size_t previous   = _pool[current].getPrevious();
                const size_t next       = _pool[current].getNext();

                _pool[previous].setNext(current);
                _pool[next].setPrevious(current);   
                    
                    
                _pool[index].setNext(index + 1);
               // _pool[index + 1].setPrevious(index);
            }
            
            
        }*/
        if(_removeListBegin != 0){
            for(size_t index = 0; index < _pool.size(); index++){
                size_t current = _pool[index].getNext();
                if(current != (index + 1)){
                    std::swap(_pool[current], _pool[index + 1]);
                            
                    const size_t previous   = _pool[current].getPrevious();
                    const size_t next       = _pool[current].getNext();

                    _pool[previous].setNext(current);
                    _pool[next].setPrevious(current);   
                            
                            
                    _pool[index].setNext(index + 1);
                }
            }
        }
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

    


    /*void push_back(T data){

        IndexNode<T>& end = _pool[_endIndex];
        const size_t newIndex = _pool.size();

        _pool[end.getPrevious()].setNext(newIndex);
        _pool.push_back(IndexNode<T>(end.getPrevious(),_endIndex,data));
        _pool[_endIndex].setPrevious(newIndex);

    }*/

    
    T& operator [] (size_t index){
        
        return _pool[index + (!index)].getData();    
    }
    ~IndexList(){

    }
    std::vector< IndexNode<T>> _pool;
    private:

    constexpr static size_t _endIndex = 0;
    size_t _removeListBegin = 0;
    size_t _size = 0;

    friend class IndexIterator<T>;
};
