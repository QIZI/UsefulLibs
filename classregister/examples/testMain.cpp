#include <iostream>


#include "../classregister.h"
#include "base.h"


using namespace ul;


struct notting {

};


FACTORY_REGISTER_CLASS(notting,void)

int main(){

	std::cout<<"List of registered classes: \n";
	for(const auto& cr : factory::ClassRegister<Base>::getRaw()){
		std::cout<<"ClassName: "<<cr.first<<'\n';
	}			


	
	Base* b = factory::ClassRegister<Base>::get("A").create();
	b->run();
}
