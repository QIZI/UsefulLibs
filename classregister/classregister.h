#ifndef CLASSREGISTER_H
#define CLASSREGISTER_H

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

