#ifndef DUMMY_SINK_HPP
#define DUMMY_SINK_HPP

#include "allolithe/al_SinkModule.hpp"
#include "allolithe/al_SoundEngine.hpp"

class DummySink : public al::SinkModule
{
public:
	DummySink(void) : al::SinkModule(1, 1){}

	virtual void instantiate_parameters(void) {}
	virtual void spatialize(al::AudioIOData& io) {}
	virtual void checkIOcompatibility(al::AudioIOData& io) {}

	static const std::string module_name;
	static const int moduleID;
};

const std::string DummySink::module_name = std::string("DummySink");
const int DummySink::moduleID = al::RegisterModule<DummySink>(DummySink::module_name);

#endif // DUMMY_SINK_HPP