#ifndef AL_MODULE_HPP
#define AL_MODULE_HPP

#include "Lithe/LitheCore.h"
#include "allocore/ui/al_Parameter.hpp"
#include "alloGLV/al_ParameterGUI.hpp"
#include <vector>

namespace al{

/** @brief Base class for a single node in a lithe audiograph. 
	
	This is a decorated lithe::Node to include parameters and an automatically 
	generated GLV view.

	@ingroup allolithe\
*/

class Module : public lithe::Node
{
public:
	Module(int numInlets, int numOutlets, int numParameters);

	~Module(void);

	/** @brief Use this to fill up the parameters vector

		Instantiation using new keyword is supported since ~Module cleans this up using
		the deletes

		Note: This is not called by default. Make sure to call this in the 
		constructor of any derived class.
	*/
	virtual void instantiate_parameters(void) =0;

	int numParams(void) {return numParameters;}
	
	/// @brief Gets the al::Parameter specified by index
	al::Parameter& parameter(int index);

	/// @brief returns true if parameters have been instantiated
	bool paramsInstnatiated(void) { return (( parameters.size() == 0) ? false : true);}

	static Module& getModuleRef(int nodeID);

	/// @brief NodeID is the ID of this module as a node in the audiograph
	int getNodeID() { return getID(); }

	/// @brief ModuleID is the ID given to a module that is TODO: TESTS for this
	int getModuleID() { return moduleID; }

	/// @brief Creates a basic GLV View of showing all the parameters
	glv::View& createDefaultView(bool debugging=false);

	void setName(std::string name) { module_name = name; }
	std::string& getName(void) { return module_name; }

protected:
	std::vector<al::Parameter*> parameters;
	const int numParameters;

private:
	using lithe::Node::getID;
	using lithe::Node::getNodeRef;
	std::string module_name;
	int moduleID = -1; 
};

}; // namespace al


#endif // AL_MODULE_HPP