#include "allolithe/al_Module.hpp"

namespace al{

Module::Module(int numInlets, int numOutlets, int numParameters) :
	lithe::Node(numInlets, numOutlets),
	numParameters(numParameters)
{
}

al::Parameter& Module::parameter(int index)
{
	if( index < 0 || index >= numParams() )
	{
		throw std::range_error("Parameter index not valid");
	}
	else
	{
		return *parameters[index];
	}
}

Module::~Module(void)
{
	for(int i=0; i<parameters.size(); ++i)
	{
		if(parameters[i] != NULL) delete parameters[i];
	}
}

glv::View& Module::createView(bool debugging)
{
	return createDefaultView(*this, debugging);
}

Module& Module::getModuleRef(int nodeID)
{
	try
	{
		al::Module* module_ref = static_cast<al::Module*>(lithe::Node::getNodeRef(nodeID));
		if(module_ref == NULL)
			throw std::runtime_error("Module instance not found or instance was destroyed. NodeID: "+std::to_string(nodeID));
		else
			return *module_ref;
	}
	catch(const char * str)
	{
		throw std::runtime_error(str);
	}
}

glv::View& createDefaultView(al::Module& module, bool debugging)
{
	std::string name_label = "";
	bool show_nodeID = true;
	static int padX = 40;
	static int padY = 40;
	// int posX = 40;
	// int posY = 40;
	
	// Parameter controls and label go here	
	glv::View* control_view = new glv::View();
	glv::Box* control_box = new glv::Box(glv::Direction::S);
	
	// The label
	glv::View* label_view = new glv::View();
	std::string node_label;
	if( name_label == "" )
	{
		node_label = "Node"; // Default
		show_nodeID = true;
	}
	else
	{
		node_label = name_label;		
	}
	if(show_nodeID)
		node_label = node_label + std::string(" ID: ")+std::to_string(module.getID());

	glv::Label* moduleNameLabel = new glv::Label(node_label);
	(*label_view) << *moduleNameLabel;
	label_view->disable(glv::Property::DrawBorder);
	label_view->fit();
	// (*control_box) << (*label_view);
	*control_view << *label_view;
	label_view->anchor(glv::Place::TL);
	label_view->pos(glv::Place::TL, 20, 40);
	// moduleNameLabel->pos(10, 20); 

	// The parameters
	al::ParameterGUI* parameterGUI = new al::ParameterGUI();
	for(int i=0; i<module.parameters.size(); i++)
	{ 
		parameterGUI->addParameter(*module.parameters[i]);
	}
	// parameterGUI->getDetachableView().pos(10, 40);
	// (*control_box) << parameterGUI->getDetachableView();
	// control_box->fit();
	// *control_view << *control_box;	
	*control_view << parameterGUI->getDetachableView();
	parameterGUI->getDetachableView().anchor(glv::Place::TL);
	parameterGUI->getDetachableView().pos(glv::Place::TL, 60, 60);
	control_view->fit();

	// std::cout << control_box->width() << "  " << control_box->height() << std::endl;
 	// std::cout << control_view->width() << "  " << control_view->height() << std::endl;
	glv::View* top_view = new glv::View(glv::Rect(0, 0, control_view->width() + 2*padX, control_view->height() + 2*padY));
	
	*top_view << *control_view;
	control_view->anchor(glv::Place::TL);
	control_view->pos(glv::Place::TL, padX, padY);
	top_view->addHandler(glv::Event::MouseDrag, glv::Behavior::mouseMove);	

 	// std::cout << top_view->width() << "  " << top_view->height() << std::endl;
	return *top_view;
}


}; // namespace al