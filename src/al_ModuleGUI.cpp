#include "allolithe/al_ModuleGUI.hpp"
#include "allolithe/al_PatcherGUI.hpp"

namespace al {

int Inlets::last_selected_inlet_index = -1;
Inlets* Inlets::last_selected_inlets_ref = NULL;

bool MouseUpInletEvent::active = false;

MouseUpOutletEvent::MouseUpOutletEvent(Outlets& outlets) : outlets_ref(outlets)
{	
	notifier.attach(al::PatcherGUI::onPatch, glv::Update::User);
}

bool MouseUpInletEvent::onEvent(glv::View &v, glv::GLV &g)
{	
	if( ! MouseUpInletEvent::active )
	{
		Inlets::last_selected_inlet_index = inlets_ref.selected_inlet;
		Inlets::last_selected_inlets_ref = &inlets_ref;
		MouseUpInletEvent::active = true;
	}	
	return false;
}

bool MouseUpOutletEvent::onEvent(glv::View &v, glv::GLV &g)
{	
	if ( MouseUpInletEvent::active )
	{
		PatchInfo* p = new PatchInfo ;
		p->inlet_index = Inlets::last_selected_inlet_index;
		p->inlets = Inlets::last_selected_inlets_ref;
		p->outlets = &outlets_ref;
		p->outlet_index = outlets_ref.selected_outlet;

		notifier.notify(glv::Update::User, p);
		MouseUpInletEvent::active = false;
	}
	return false;
}

}; //namespace al

