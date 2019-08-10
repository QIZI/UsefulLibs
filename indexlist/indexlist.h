#include <vector>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <cstddef>
#include <utility>

template<class T>
class IndexList;

using index_t = size_t;
template<class T>
struct IndexNode{
    public:
    

    void setPrevious(index_t previous){_previous = previous;}
    void setNext(index_t next){_next = next;}

    index_t getPrevious()    const {return _previous;}
    index_t getNext()        const {return _next;}

    T& getData(){return _data;}

    template <class... Args>
    IndexNode(index_t previous, index_t next, Args&&... args) : _previous(previous),_next(next),_data(args...) {}
    IndexNode(index_t previous, index_t next, const T& data)  : _previous(previous),_next(next), _data(data){}
    IndexNode(index_t previous = 0, index_t next = 0)         : _previous(previous),_next(next){}
    IndexNode(const IndexNode&) = default;
    IndexNode(IndexNode&& other) noexcept = default;
    IndexNode& operator = (const IndexNode&) = default;
    IndexNode& operator = (IndexNode&&) noexcept = default;

    bool operator == (const IndexNode& other){
        return (_previous == other.getPrevious() && _next == other.getNext()); 
    }

    private:
    index_t _previous;
    index_t _next;
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

    constexpr IndexIterator() : _iList(nullptr),_current(0){}

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

    IndexIterator operator+(index_t advance) const{
        auto it = *this;
        for(; advance > 0; --advance){
            ++it;
        }
        return it;
    }

    IndexIterator operator-(index_t advance) const{
        auto it = *this;
        for(; advance > 0; --advance){
            --it;
        }
        return it;
    }
   
    T& operator*(void){
        return _iList->_pool[_current].getData();
    }

    index_t getPreviousIndex() const{return _iList->_pool[_current].getPrevious();}
    
    index_t getCurrentIndex() const {return _current;}
    
    index_t getNextIndex() const {return _iList->_pool[_current].getNext();}
    
    
    private:
    IndexList<T>* _iList;
    index_t _current;

    constexpr IndexIterator(IndexList<T>* iList, index_t index) : _iList(iList), _current(index) {}
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

    void reserve(index_t nSize){
        _pool.reserve(nSize+1);
    }

    IndexIterator<T> insert(IndexIterator<T> it, const T& data){
        const index_t current        = it.getCurrentIndex();
        const index_t currentNext    = _pool[current].getNext();
        index_t newIndex             = _pool.size();

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
    IndexIterator<T> emplace(const IndexIterator<T>& it, Args&&... args){
        const index_t current        = it.getCurrentIndex();
        const index_t currentNext    = _pool[current].getNext();
        index_t newIndex             = _pool.size();
        
        
        if(_eraseListBegin == emptyEraseList){
            _pool.emplace_back(current, currentNext, args...);
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
    IndexIterator<T> emplace( index_t current, Args&&... args){
        
        index_t currentNext    = _pool[current].getNext();
        index_t newIndex       = _pool.size();
        
        
        if(_eraseListBegin == emptyEraseList){
            _pool.emplace_back(current, currentNext, args...);
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
       
        return emplace( _pool[endIndex].getPrevious(),args...);
    }

    
    void erase(IndexIterator<T> it){
        if(it != end()){
            const index_t current    = it.getCurrentIndex();
            const index_t previous   = _pool[current].getPrevious();
            const index_t next       = _pool[current].getNext();
            
            _pool[next].setPrevious(previous);
            _pool[previous].setNext(next);

            _pool[current] = IndexNode<T>(0,0,{});//_pool[current] = IndexNode<T>(0,0,{});
            _pool[current].setNext(_eraseListBegin);
            _pool[current].setPrevious(current);

            _eraseListBegin = current;         
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

    void resize(index_t newSize){
        if(_size < newSize){
            for(index_t i = 0, sizeDiff = newSize - _size ; i < sizeDiff; i++){
                emplace_back();
            }
        }
        else if(_size > newSize){
            for(index_t i = 0, sizeDiff = _size - newSize ; i < sizeDiff; i++){
                pop_back();
            }
        }
        
    }


    void shrink_to_fit(){
        (void)std::partition(_pool.begin() + 1, _pool.end(), [this](const auto& node){
            return !isNodeErased(node);
        });

        for(index_t index = 1; index <= _size; ++index){
            _pool[index]._next      = index + 1;
            _pool[index]._previous  = index - 1;
        }

        _eraseListBegin = emptyEraseList;

        _pool.resize(_size+1);
        _pool.shrink_to_fit();

        _pool[endIndex].setPrevious(_size);
        _pool[endIndex].setNext(endIndex+1);
        _pool[_size].setNext(endIndex);
    }

    void reorder(){
        (void)std::partition(_pool.begin() + 1, _pool.end(), [this](const auto& node){
            return !isNodeErased(node);
        });

        for(index_t index = 1; index <= _size; ++index){
            _pool[index]._next      = index + 1;
            _pool[index]._previous  = index - 1;
        }

        _eraseListBegin = _pool.size() - 1;

        for(index_t end = _pool.size() - 1; end > _size; --end){
            _pool[end]._next        = end - 1;
            _pool[end]._previous    = end;
        }

        _pool[_size + 1]._next = 0;

        _pool[endIndex].setPrevious(_size);
        _pool[endIndex].setNext(endIndex+1);
        _pool[_size].setNext(endIndex);
    }
    

    index_t size() const{
        return _size;
    }
    index_t capacity() const{
        return (_pool.capacity() - 1);
    }

    bool empty() const{
        return (_size == 0);
    }

    
    T& operator [] (index_t index){
        
        return _pool[index + (!index)].getData();    
    }
    ~IndexList(){

    }
    bool isNodeErased(const IndexNode<T>& node) const{
        return (_pool[node.getPrevious()].getNext() == node.getNext());
    }
    index_t getNodeIndex(const IndexNode<T>& node) const{
        return _pool[node.getPrevious()].getNext();
    }

    std::vector< IndexNode<T>> _pool;
    //private:

    constexpr static index_t endIndex        = 0;
    constexpr static index_t emptyEraseList  = 0;
    index_t _eraseListBegin = 0;
    index_t _size = 0;

    friend class IndexIterator<T>;
};
