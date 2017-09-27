#include "baseclass.h"
#include <iostream>

struct Other : public BaseClass {
	Other(){

	}
	void run() override{
		std::cout<<"Other\n";
	}

} other; // global declaration 
