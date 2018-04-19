#include "derived.h"

FACTORY_REGISTER_CLASS(A, Base) //register class A
void A:: run() {
		std::cout<<"Something that would A class run\n";
}




class Egg :  public Base{
	void run(){
		std::cout<<"Something that would Egg class run\n";
	}
};
FACTORY_REGISTER_CLASS(Egg, Base)


