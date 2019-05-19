#include <array>
#include <initializer_list>
#include <functional>

#include "propertytypes.h"


class Property{
  
    public:
    Property(){}
    Property(const std::string_view& propertyName, PropertyVariant_t inputData) : propertyName(propertyName), data(inputData){}

    
    template<class T>
    T& get(){return std::get<T>(data);}
    auto& get() {return data;} 
    
    const std::string_view& getPropertyName(){return propertyName;}

    private:
    std::string_view propertyName;
    PropertyVariant_t data = NoType();
  
};

class PropertyList{
    
    std::array<Property,100> propertyArray;  
    size_t _size = 0;
    public:
    PropertyList(){}
    
    
    PropertyList& operator = (const std::initializer_list<Property> propertyList){
        auto it = propertyList.begin();
        for(size_t index = 0; index < propertyList.size(); index++, it++){
            propertyArray.at(index) = (*it);
        }
        _size = propertyList.size();
        return *this;
    }
    
    Property& operator [] (size_t index) {
        return propertyArray[index];   
    }

    Property& at(size_t index) {
        return propertyArray.at(index);   
    }

    size_t size(){return _size;}
    size_t bufferSize(){return propertyArray.size();}

    struct ListSetter{
        const std::function<void(Property&)> setterFunc;

        template<class T>
        ListSetter(T& setVar) : setterFunc ([&setVar](Property& property) {setVar = property.get<T>();})
        {         
        }
        
        template<class T>
        ListSetter(CONSTANT<T> constant) : setterFunc([](Property& property){})
        {
              (void)constant;
        }

        ListSetter(ReadOnly_t constant) : setterFunc([](Property& property){})
        {
              (void)constant;
        }
        
        ListSetter(std::string& setVar) : setterFunc ([&setVar](Property& property) {setVar = property.get<std::string_view>();})
        {    
        }
        
        ListSetter(decltype(setterFunc) func) : setterFunc(func) 
        {
        }
            
        static void set(PropertyList& properties, const std::initializer_list<ListSetter> setterList){
            auto it = setterList.begin();
            
            for(size_t index = 0; index < setterList.size(); index++, it++){
                it->setterFunc(properties.at(index));
            }
        }
    };
    
    
};


class Entity{
    public:
    virtual PROPERTIES_GET(
           // {"Entity",ReadOnly("Empty")}
        )
    virtual PROPERTIES_SET(
        //ReadOnly()
    )
    protected: 
    virtual ~Entity(){};
    
    
};





