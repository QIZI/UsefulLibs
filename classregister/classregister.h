#ifndef CLASSREGISTER_H
#define CLASSREGISTER_H

/******************************  <Zlib>  **************************************
 * Copyright (c) 2017 Martin Baláž
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/


#include <vector>
#include <string>

#define FACTORY_NO_BASECLASS void

#define FACTORY_REGISTER_CLASS(className, baseClassName) ul::factory::RegisterTheClass<className, baseClassName> __Class_##className##_with_BaseClass_##baseClassName##__(std::string{#className});


namespace ul{namespace factory{


template <class BaseClass>
class ClassRegister{
typedef BaseClass* (*ConstructionFunction)(void);

public:

	const std::string className;
	ConstructionFunction create;
	
	static void ConstructClassRegister(std::string&& className,ConstructionFunction Create){
	    staticDependency().emplace_back(ClassRegister<BaseClass>(std::move(className), Create));
	}
	
	static std::vector< ClassRegister<BaseClass> >& getRaw(){
		return staticDependency();
	}

private:
	
	ClassRegister(std::string&& className ,ConstructionFunction Create) : className(std::move(className)), create(Create){}	

	static std::vector<ClassRegister<BaseClass> >& staticDependency(){
		static std::vector<ClassRegister<BaseClass> > dependency;
		return dependency;
	} 
};


template <class DerivedClass,class BaseClass>
struct RegisterTheClass{
	RegisterTheClass(std::string&& className){
		ClassRegister<BaseClass>::ConstructClassRegister(std::move(className), [](void)->BaseClass* {return new DerivedClass;}) ;
	}
};

}}
#endif

