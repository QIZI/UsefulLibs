#include "eventsystem.h"
#include <iostream>
struct Test{
    
    void testEvent()
    {
        std::cout<<"Event Triggered by "<<oName<<'\n';
    }
    void anotherTestEvent()
    {
        std::cout<<"Another Event Triggered by "<<oName<<'\n';
    }
    
    Test(const std::string& name) : oName(name){
        EventManager::AddListener("test_trigger",this, &Test::testEvent);
        EventManager::AddListener("test_trigger",this, &Test::anotherTestEvent);
    }
    void rm(){
        EventManager::RemoveListener("test_trigger",this,&Test::testEvent);
    }
    ~Test(){
        EventManager::RemoveAllListeners("test_trigger", this);
    }

    std::string oName;
};
struct SecondTest{
    
    void testEvent()
    {
        std::cout<<"Event Triggered by "<<oName<<'\n';
    }
    void anotherTestEvent()
    {
        std::cout<<"Another Event Triggered by "<<oName<<'\n';
    }
    
    SecondTest(const std::string& name) : oName(name){
        EventManager::AddListener("test_trigger",this, &SecondTest::testEvent);
        EventManager::AddListener("test_trigger",this, &SecondTest::anotherTestEvent);
    }
    void rm(){
        EventManager::RemoveListener("test_trigger",this,&SecondTest::anotherTestEvent);
        //EventManager::RemoveAllListeners("test_trigger", this);
    }
    ~SecondTest(){
        EventManager::RemoveAllListeners("test_trigger", this);
    }

    std::string oName;
};
int main()
{
    
    
    Test t("First"),t2("Second");
    t2.rm();
    SecondTest sT("Third");
    sT.rm();
    EventManager::Trigger("test_trigger");
}

