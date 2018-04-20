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
		std::cout<<"Something that would B class run\n";
	}
	

}; REGISTER_CLASS(B, Base) 