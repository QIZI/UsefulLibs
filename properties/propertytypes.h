#ifndef PROPERTYTYPES_H
#define PROPERTYTYPES_H

#include <inttypes.h>
#include <string_view>
#include <variant>





#define PROPERTIES_GET(...) \
void getProperties(PropertyList& properties){ properties = {__VA_ARGS__};}

#define PROPERTIES_SET(...) \
void setProperties(PropertyList& properties){ PropertyList::ListSetter::set(properties, {__VA_ARGS__});}

class Entity;

struct ReadOnly_t{};
struct NoType{};

template <typename T>
struct CONSTANT {
    T value;
    CONSTANT(){}
    operator const T&() const{return value;}
};


struct ParentEntity {
    Entity* entity;
    ParentEntity(Entity* ent) : entity(ent){}
    
};

using PropertyVariant_t =  
    std::variant
    <
        //----------Generic Types------------//
        NoType,
        bool,               CONSTANT<bool>,
        int8_t,             CONSTANT<int8_t>,
        uint8_t,            CONSTANT<uint8_t>, 
        int16_t,            CONSTANT<int16_t>,
        uint16_t,           CONSTANT<uint16_t>, 
        int32_t,            CONSTANT<int32_t>, 
        uint32_t,           CONSTANT<uint32_t>, 
        int64_t,            CONSTANT<int64_t>, 
        uint64_t,           CONSTANT<uint64_t>,
        float,              CONSTANT<float>,
        double,             CONSTANT<double>,
        std::string_view,   CONSTANT<std::string_view>,


        //----------Additional Types------------//
        Entity*,            ParentEntity
        
    >;


template <class T>
constexpr CONSTANT<T> ReadOnly(T value){
    CONSTANT<T> ret;
    ret.value = value;

    return ret;
}
/*
template <size_t N>
constexpr CONSTANT<std::string_view> ReadOnly(const char (&value)[N] ){
    CONSTANT<std::string_view> ret;
    ret.value = value;

    return ret;
}*/

CONSTANT<std::string_view> ReadOnly(const std::string& value){
    CONSTANT<std::string_view> ret;
    ret.value = value;

    return ret;
}


constexpr ReadOnly_t ReadOnly(){
    return ReadOnly_t();
}



#endif