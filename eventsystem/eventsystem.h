#ifndef _EVENT_SYSYEM_H
#define _EVENT_SYSYEM_H


#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>


struct EventActionInterface{
    virtual void operator()() = 0;
    virtual bool operator == (const EventActionInterface&) = 0;
};


template < typename ThisTypePtr, typename FuncTypePtr>
struct EventAction : public EventActionInterface{

    ThisTypePtr thisPtr;
    FuncTypePtr funcPtr;
    
    constexpr EventAction(ThisTypePtr thisPointer, const FuncTypePtr& func): thisPtr(thisPointer), funcPtr(func){}

    virtual void operator()(){
        std::invoke(funcPtr, thisPtr);
    }

    virtual bool operator ==(const EventActionInterface& other){
        auto& otherAction = (EventAction<ThisTypePtr, FuncTypePtr>&)other;       

        return (
            (void*)thisPtr == (void*)otherAction.thisPtr 
            &&
            (funcPtr == otherAction.funcPtr)
        );
    }

};

template <typename ThisTypePtr, typename FuncTypePtr>
auto make_event_action(ThisTypePtr thisPtr, FuncTypePtr funcPtr){
    return EventAction<ThisTypePtr, FuncTypePtr>(thisPtr, funcPtr);
}

template <typename ThisTypePtr, typename FuncTypePtr>
std::unique_ptr<EventActionInterface> make_event_action_ptr(ThisTypePtr thisPtr, FuncTypePtr funcPtr){
    return std::make_unique<EventAction<ThisTypePtr, FuncTypePtr>>(thisPtr, funcPtr);
}


class EventListeners{
    using EventActionInterface_ptr = std::unique_ptr<EventActionInterface>;
    std::vector<EventActionInterface_ptr> listeners;
    public:
    
    template <typename ThisTypePtr, typename FuncTypePtr>
    void addListener(ThisTypePtr owner, const FuncTypePtr& action){
        auto eventAction = make_event_action(owner, action);
        for(const auto& listener : listeners){
            if(eventAction == (*listener)){
                return;
            }
        }
        listeners.emplace_back(make_event_action_ptr(owner, action));   
        
    }

 
    template <typename ThisTypePtr, typename FuncTypePtr>
    void removeListener(ThisTypePtr owner, const FuncTypePtr& action){
        auto eventAction = make_event_action(owner, action);
        
        for(auto findIt = listeners.begin(); findIt != listeners.end(); findIt++){
            if( (*findIt->get()) == eventAction){
                listeners.erase(findIt);
                break;
            }
        }

    }
    template <typename ThisTypePtr>
    void removeAllListeners(ThisTypePtr owner){

        auto compare = [](ThisTypePtr owner, const EventActionInterface& other){
            using DummyFunc = void (*)(ThisTypePtr);// this will never be executed
            auto& otherAction = (EventAction<ThisTypePtr, DummyFunc>&)other; 

            return ((void*)owner == (void*)otherAction.thisPtr);
        };
        
        for(size_t idx = 0; idx < listeners.size(); idx++){
            if(compare(owner, *listeners[idx])){
                listeners[idx] = std::move(listeners.back());
                listeners.pop_back();
                idx--;
            }
        }
    }

    void removeAllListeners(){
        listeners.clear();
    }
    
    void operator = ( EventActionInterface_ptr&& eventAction){
        for(const auto& listener : listeners){
            if((*eventAction) == (*listener)){
                return;
            }
        }
        listeners.emplace_back(std::move(eventAction));  
         
        
    }
    
    void invoke(){
        for(const EventActionInterface_ptr& listener : listeners){
            (*listener)();   
        }
    }
    
};


//static std::unordered_map<std::string, EventAction> events; //enable me
class EventManager{
    
    
    private:

    static auto& instance(){
        static std::unordered_map<std::string, EventListeners> e;
        return e;
    }
    
    public:
    template <typename ThisTypePtr, typename FuncTypePtr>
    static void AddListener(const std::string& eventName ,ThisTypePtr owner, const FuncTypePtr& action){
        instance()[eventName] = make_event_action_ptr(owner, action);
    }

    template <typename ThisTypePtr, typename FuncTypePtr>
    static void RemoveListener(const std::string& eventName, ThisTypePtr owner, const FuncTypePtr& action){
        auto findIt = instance().find(eventName);
        if(findIt != instance().end()){

            findIt->second.removeListener(owner, action);
        }
    }
    static void RemoveAllListeners(const std::string& eventName, void* owner){
        auto findIt = instance().find(eventName);
        if(findIt != instance().end()){

            findIt->second.removeAllListeners(owner);
        }
    }

    static void RemoveAllListeners(const std::string& eventName){
        auto findIt = instance().find(eventName);
        if(findIt != instance().end()){

            findIt->second.removeAllListeners();
        }
    }

    static bool Trigger(const std::string& eventName){
        auto findIt = instance().find(eventName);

        if(findIt != instance().end()){

            findIt->second.invoke();
            return true;
        }
        else{
            return false;
        }
    }

    
};


#endif