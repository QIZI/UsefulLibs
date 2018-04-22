#include <iostream>
#include <vector>

#include "../classregister.h"
#include "base.h"


using namespace ul;


struct notting {

};


FACTORY_REGISTER_CLASS(notting,void)

int main(){

	/*std::cout<<"List of registered classes: \n";
	for(const auto& cr : factory::ClassRegister<Base>::getRaw()){
		std::cout<<"ClassName: "<<cr.first<<'\n';
	}		*/	

	std::cout<<"Classes are:\n";
	for(const auto& rgClass: factory::ClassRegister<Base>::getRaw()){
		std::cout<<rgClass.first<<'\n';
	}

	std::vector<Base*> bases;
	std::string className;

	while(className != "exit"){
		for(const auto& base : bases){
			base->run();
			
		}

		std::cout<<"enter the class name: ";
		std::cin>>className;
		if(const auto& entry = factory::ClassRegister<Base>::get(className)){
			bases.push_back(entry->create());
		}
		else{
			std::cout<<"Class not found!\n";
		}
		
		std::cout<<"\n";
	}


	if(const auto& entry = factory::ClassRegister<Base>::get("Egg")){
		Base* b = entry->create();
		b->run();
	}
}
