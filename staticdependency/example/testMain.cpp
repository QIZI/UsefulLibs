 #include "../staticdependency.h"
#include "baseclass.h"
#include <iostream>

#include <vector>

int main(){
	

	std::vector<BaseClass*>& vec = staticDependency<std::vector<BaseClass*> >();
	
	std::cout<<"static dependency list size: "<<vec.size()<<'\n';

	for(int i = 0; i < vec.size(); i++){
		vec[i]->run();
	}

	
	return 0;
}


