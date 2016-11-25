#include "allolithe/al_ModuleGUI.hpp"

namespace al {

int Inlets::last_selected_inlet_index = -1;
int Inlets::last_selected_node_id = -1;

bool MouseUpInletEvent::active = false;

bool MouseUpInletEvent::onEvent(glv::View &v, glv::GLV &g)
{	
	if( ! MouseUpInletEvent::active )
	{
		cout << "  INLET " << inlets_ref.selected_inlet << endl;;
		Inlets::last_selected_inlet_index = inlets_ref.selected_inlet;
		Inlets::last_selected_node_id = inlets_ref.module.getNodeID();
		MouseUpInletEvent::active = true;
	}	
	return false;
}

bool MouseUpOutletEvent::onEvent(glv::View &v, glv::GLV &g)
{	
	if ( MouseUpInletEvent::active )
	{
		cout << " Trying to connect inlet : " << Inlets::last_selected_inlet_index << "  NODE: " << Inlets::last_selected_node_id << endl;
		cout << "..With " << outlets_ref.selected_outlet << outlets_ref.module.getNodeID() << endl;

		cout << "Connected" << endl;

		MouseUpInletEvent::active = false;
	}

	return false;
}

}; //namespace al

