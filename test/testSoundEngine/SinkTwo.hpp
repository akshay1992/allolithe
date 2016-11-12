#ifndef SINK_TWO_HPP
#define SINK_TWO_HPP

#include "allolithe/al_allolithe.hpp"

class SinkTwo : public al::SinkModule
{
public:
	SinkTwo(void) : al::SinkModule(1, 0) {}

	virtual void instantiate_parameters(void) override {}
};

int SinkTwoFactory(void)
{
	return 2020;
}

#endif