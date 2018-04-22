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


#include <unordered_map>
#include <string>
#include <optional>


#define FACTORY_REGISTER_CLASS(className, baseClassName) inline ul::factory::RegisterTheClass<className, baseClassName> __Class_##className##_with_BaseClass_##baseClassName##__(std::string{#className});


namespace ul{namespace factory{


template <class BaseClass>
class ClassRegister{
typedef BaseClass* (*ConstructionFunction)(void);

public:
	typedef BaseClass* base_type;

	ConstructionFunction create;
	
	constexpr ClassRegister(ConstructionFunction Create) : create(Create) {}

	static constexpr void AddToClassRegister(std::string&& className, ConstructionFunction Create){
	    staticDependency().emplace(className ,ClassRegister<BaseClass>(Create));
	}
	
	static const std::unordered_map<std::string, ClassRegister<BaseClass> >& getRaw(){
		return staticDependency();
	}

	static const std::optional<ClassRegister<BaseClass>> get(const std::string& className){
		const auto& result = staticDependency().find(className);
		if(result != staticDependency().end()){
			return staticDependency().find(className)->second;
		}
		else {
			return std::nullopt;
		}
	}
	
private:
	static std::unordered_map<std::string, ClassRegister<BaseClass> >& staticDependency();
};

template <class T>
std::unordered_map<std::string, ClassRegister<T> >& ClassRegister<T>::staticDependency(){
	static std::unordered_map<std::string, ClassRegister<T> > dependency;
	return dependency;
}


template <class DerivedClass,class BaseClass>
struct RegisterTheClass{
	constexpr RegisterTheClass(std::string&& className){
		ClassRegister<BaseClass>::AddToClassRegister(std::move(className), [](void)->BaseClass* {return new DerivedClass;}) ;
	}
};

}}
#endif

