#ifndef SINK_ONE_HPP
#define SINK_ONE_HPP

#include "allolithe/al_allolithe.hpp"

class SinkOne : public al::SinkModule
{
public:
	SinkOne(void) : al::SinkModule(1, 0) {}

	virtual void instantiate_parameters(void) override {}
};


#endif