#ifndef MODULE_ONE_HPP
#define MODULE_ONE_HPP

#include "allolithe/al_Module.hpp"

class ModuleOne : public al::Module
{
public:
	ModuleOne(void) : al::Module(2, 1, 1)
	{
	}

	virtual void instantiate_parameters(void) override
	{
	}

};

#endif