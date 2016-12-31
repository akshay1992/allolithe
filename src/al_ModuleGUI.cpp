#include "allolithe/al_ModuleGUI.hpp"
#include "allolithe/al_PatcherGUI.hpp"

namespace al {

int Inlets::last_selected_inlet_index = -1;
Inlets* Inlets::last_selected_inlets_ref = NULL;

bool MouseUpInletEvent::active = false;

MouseUpOutletEvent::MouseUpOutletEvent(Outlets& outlets) : outlets_ref(outlets)
{	
	notifier.attach(al::PatcherGUI::onPatch, glv::Update::Action);
}

bool MouseUpOutletEvent::onEvent(glv::View &v, glv::GLV &g)
{	
	if ( MouseUpInletEvent::active )
	{
		// TODO: move this to a patching callback in PatcherGUI 
		// (so that drawing can happen simultaneously)

		// lithe::Patcher::connect(
		// 	Inlets::last_selected_inlets_ref->module.getInlet(Inlets::last_selected_inlet_index), 
		// 	outlets_ref.module.getOutlet(outlets_ref.selected_outlet) 
		// 	);

		// std::shared_ptr<al::PatchInfo> p(new PatchInfo);
		// p->inlet_index = Inlets::last_selected_inlet_index;
		// p->destination_nodeID = Inlets::last_selected_inlets_ref->module.getNodeID();
		// p->source_nodeID = outlets_ref.soundengine_ref.getModuleInfo(outlets_ref.module_ref.getNodeID()).moduleID;
		// p->outlet_index = outlets_ref.selected_outlet;

		// p->print();

		// notifier.notify(glv::Update::Action, &p);
		MouseUpInletEvent::active = false;
	}
	else
	{
		outlets_ref.buttons->setValue(0, outlets_ref.selected_outlet);
	}
	return false;
}

bool MouseUpInletEvent::onEvent(glv::View &v, glv::GLV &g)
{	
	if( ! MouseUpInletEvent::active )
	{
		Inlets::last_selected_inlet_index = inlets_ref.selected_inlet;
		Inlets::last_selected_inlets_ref = &inlets_ref;
		MouseUpInletEvent::active = true;
	}
	else
	{
		return false;
	}
}

}; //namespace al

