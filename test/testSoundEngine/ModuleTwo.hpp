#ifndef MODULE_TWO_HPP
#define MODULE_TWO_HPP

#include "allolithe/al_Module.hpp"

class ModuleTwo : public al::Module
{
public:
	ModuleTwo(void) : al::Module(2, 1, 1)
	{
	}

	virtual void instantiate_parameters(void) override
	{
	}

};

int ModuleTwoFactory(void)
{
	return 1010; // specfial number to check if this was called
}


#endif