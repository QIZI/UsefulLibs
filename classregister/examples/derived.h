#pragma once


#include <iostream>
#include "../classregister.h"

#include "base.h"

class A : public Base{
	public:
	void run();

}; 

class B : public Base{
	public:
	void run() override{
		std::cout<<"instance_of_B\n";
	}
	

}; FACTORY_REGISTER_CLASS(B, Base) 




