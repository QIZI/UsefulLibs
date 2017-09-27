 
#include "baseclass.h"

#include <iostream>
class Another : public BaseClass{
	void run() override{
		std::cout<<"Another\n";
	}

} another; // global declaration 
