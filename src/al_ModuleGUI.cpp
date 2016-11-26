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
		// TODO: move this to a patching callback in PatcherGUI 
		// (so that drawing can happen simultaneously)

		lithe::Patcher::connect(
			Inlets::last_selected_inlets_ref->module.getInlet(Inlets::last_selected_inlet_index), 
			outlets_ref.module.getOutlet(outlets_ref.selected_outlet) 
			);


		// std::unique_ptr<PatchInfo> p( & PatchInfo() );
		// p->inlet_index = Inlets::last_selected_inlet_index;
		// p->inlets = Inlets::last_selected_inlets_ref;
		// p->outlets = &outlets_ref;
		// p->outlet_index = outlets_ref.selected_outlet;

		// notifier.notify(glv::Update::User, (void* )p);
		MouseUpInletEvent::active = false;
	}
	else
	{
		outlets_ref.buttons->setValue(0, outlets_ref.selected_outlet);
	}
	return false;
}

}; //namespace al

