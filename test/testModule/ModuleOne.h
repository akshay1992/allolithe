#ifndef MODULE_ONE
#define MODULE_ONE

class ModuleOne : public al::Module
{
public:
	ModuleOne(int numInlets, int numOutlets, int numParameters) : 
		al::Module(numInlets, numOutlets, numParameters)
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



#endif 