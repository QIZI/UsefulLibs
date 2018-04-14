#include "derived.h"

FACTORY_REGISTER_CLASS(A, Base) //register class A
void A:: run() {
		std::cout<<"instance_of_A\n";
}




class Egg :  public Base{
	void run(){
		std::cout<<"Password: 123\n";
	}
};
FACTORY_REGISTER_CLASS(Egg, Base)


