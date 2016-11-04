#ifndef SINK_MODULE_ONE_H
#define SINK_MODULE_ONE_H

#include "allolithe/al_allolithe.hpp"

class SinkModuleOne : public al::SinkModule
{
	SinkModuleOne(int numOutlets, int numParameters) : 
		al::SinkModule(numOutlets),
		numParameters(numParameters)
	{

	}

	void instantiate_parameters(void)
	{
		for(int i=0; i<numParams(); ++i)
		{
			parameters.push_back( new al::Parameter("Param"+std::to_string(i), "group", i));
		}
	}

};

#endif  // SINK_MODULE_ONE_H