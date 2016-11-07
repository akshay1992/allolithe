#ifndef MODULE_SOURCE_HPP
#define MODULE_SOURCE_HPP

#include "allolithe/al_allolithe.hpp"
#include <iostream>

class ModuleSource : public al::Module
{
public:
	ModuleSource(void) : al::Module(0, 1, 0){}

	virtual void instantiate_parameters(void) override {}

	virtual void DSP(void) override
	{
		getOutlet(0).setSample(samp);
	}

	void inject(lithe::Sample s)
	{ 
		samp = lithe::Sample(s);
	}

private:
	lithe::Sample samp;
};

#endif // MODULE_SOURCE_HPP