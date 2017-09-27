#include "baseclass.h"
#include "../staticdependency.h"


#include <vector>
BaseClass::BaseClass(){
	staticDependency<std::vector<BaseClass*> >().push_back(this); //the first appiarence of staticDependency and static initialization
}
