
#include "properties.h"
#include <iostream>

void propertyInfo(Property& property){
    std::cout<<"PropertyName: "<<property.getPropertyName()<<"\nContains: ";
    auto& tmpData = property.get();

    if(std::holds_alternative<bool>(tmpData)){
        std::cout<<"bool = "<<std::get<bool>(tmpData);
    }

    else if(std::holds_alternative<CONSTANT<bool>>(tmpData)){
        std::cout<<"constant bool = "<<std::get<CONSTANT<bool>>(tmpData);
    }

    else if(std::holds_alternative<int8_t>(tmpData)){
        std::cout<<"int8_t = "<<std::get<int8_t>(tmpData);
    }
    
    else if(std::holds_alternative<uint8_t>(tmpData)){
        std::cout<<"uint8_t = "<<std::get<uint8_t>(tmpData);
    }
    
    else if(std::holds_alternative<int16_t>(tmpData)){
        std::cout<<"int16_t = "<<std::get<int16_t>(tmpData);
    }
    else if(std::holds_alternative<uint16_t>(tmpData)){
        std::cout<<"uint16_t = "<<std::get<uint16_t>(tmpData);
    }
    
    else if(std::holds_alternative<int32_t>(tmpData)){
        
        std::cout<<"int32_t = "<<std::get<int32_t>(tmpData);
       // std::get<int32_t>(tmpData)++;
    }
    else if(std::holds_alternative<uint32_t>(tmpData)){
        std::cout<<"uint32_t = "<<std::get<uint32_t>(tmpData);
    }
    
    else if(std::holds_alternative<int64_t>(tmpData)){
        std::cout<<"int64_t = "<<std::get<int64_t>(tmpData);
    }
    else if(std::holds_alternative<uint64_t>(tmpData)){
        std::cout<<"uint64_t = "<<std::get<uint64_t>(tmpData);
    }
    
    else if(std::holds_alternative<float>(tmpData)){
        std::cout<<"float = "<<std::get<float>(tmpData);
    }
    else if(std::holds_alternative<CONSTANT<float>>(tmpData)){
        std::cout<<"const float = "<<std::get<CONSTANT<float>>(tmpData);
        std::get<CONSTANT<float>>(tmpData).value * (-1);
    }
    
    else if(std::holds_alternative<double>(tmpData)){
        std::cout<<"double = "<<std::get<double>(tmpData);
    }
    
    else if(std::holds_alternative<double>(tmpData)){
        std::cout<<"double = "<<std::get<double>(tmpData);
    }
    
    else if(std::holds_alternative<std::string_view>(tmpData)){
        std::cout<<"string = "<<std::get<std::string_view>(tmpData);
    }
    else if(std::holds_alternative<CONSTANT<std::string_view>>(tmpData)){
        std::cout<<"const string = "<<std::get<CONSTANT<std::string_view>>(tmpData).value;
    }
    else if(std::holds_alternative<Entity*>(tmpData)){
        //std::cout<<"property* = "<<std::get<Property*>(tmpData);
    }
    else{
        std::cout<<"unknown type";
    }
    
    
    std::cout<<"\n\n";
    
    
}


void processProperties(Entity& ent){
     PropertyList tmpProList;
     
     ent.getProperties(tmpProList);
     
     std::cout<<"Number of Properties: "<<tmpProList.size()<<'\n';
     for(int i = 0; i < tmpProList.size();i++){
        propertyInfo(tmpProList[i]);
        
     }
     //tmpProList[1].getData() = 12.f;
     ent.setProperties(tmpProList);
     
     
}

struct Texture : public Entity{
    PROPERTIES_GET(
        {"TextureName", ReadOnly("Picture")},
        {"Width", width},
        {"Height",height},
        
    )
    PROPERTIES_SET(
        ReadOnly(),
        width,
        {[&](Property& property){setSize(width*2, property.get<int>()*2);}}
    )
    
    
    void setSize(int width, int height){
        this->width     = width;
        this->height    = height;
        glBlackMagicFuncForSettingTheTextureSize(id,width,height);
    }
    void glBlackMagicFuncForSettingTheTextureSize(int id, int width, int height){}
    
    int id;
    int width = 600;
    int height = 400;
};

class Panel : public Entity{
    public:
    
    PROPERTIES_GET(
        {"x", ReadOnly(getX())},
        {"y", y},
        {"scale", scale},
        {"DisplayName", ReadOnly(displayName)},
        {"Condition",false},
        {"Character", (int8_t)'A'},
        {"UCK_T", ReadOnly((uint16_t) 8)},
        {"Int cislo", 5}
    )
    PROPERTIES_SET(
        ReadOnly(),
        {
            [this](Property& property){ 
                setPos(x+1.f,property.get<float>());
            }
        },
        scale,
        ReadOnly()
    )

    
    
    void setPos(float x, float y){
        this->x = x;
        this->y = y;
    }
    
    float getX(){return x;}
    private:
    float x = 8.2f,y;
    float fakeY = -5;
    int scale;
    std::string displayName = "Panel";
    
};



int main()
{
    Panel pan;
    Texture tex;
   /* PropertyList pList;
    
    pan.getProperties(pList);*/
    processProperties(pan);
    processProperties(pan);
    processProperties(tex);
    processProperties(tex);
   // propertyInfo(pList[0]);
    
    std::cout << "Notting " <<sizeof(std::variant<std::string_view>) + sizeof(std::string_view) <<std::endl;
}
