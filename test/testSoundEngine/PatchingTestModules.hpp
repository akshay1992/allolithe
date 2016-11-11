#ifndef PATCHING_TEST_MODULES
#define PATCHING_TEST_MODULES

al::SoundEngine PatchingTest_SoundEngine;

class Module1 : public al::Module
{
public:
	Module1(void) : al::Module(1, 1, 0) {}
	virtual void instantiate_parameters(void) override{	}
	static int moduleID;
	static std::string moduleName;
};


class Sink1 : public al::SinkModule
{
public:
	Sink1(void) : al::SinkModule(2, 0) {}
	virtual void instantiate_parameters(void) override {	}
	static int moduleID;
	static std::string moduleName;
};

std::string Module1::moduleName = "Module1";
int Module1::moduleID = PatchingTest_SoundEngine.RegisterModule<Module1>(Module1::moduleName);


std::string Sink1::moduleName = "Sink1";
int Sink1::moduleID = PatchingTest_SoundEngine.RegisterModule<Sink1>(Sink1::moduleName);


#endif