#include "allolithe/al_ModuleGUI.hpp"
#include "allolithe/al_PatcherGUI.hpp"

namespace al {

int Inlets::last_selected_inlet_index = -1;
Inlets* Inlets::last_selected_inlets_ref = NULL;

bool MouseUpInletEvent::active = false;

ModuleGUIKeyDownEvent::ModuleGUIKeyDownEvent(ModuleGUI& moduleGUI) : moduleGUI_ref(moduleGUI)
{
	notifier.attach(al::PatcherGUI::onUnPatch, glv::Update::Action);
}

bool ModuleGUIKeyDownEvent::onEvent(glv::View &v, glv::GLV &g)
{
	if( g.keyboard().key(glv::Key::t::Backspace) )
	{
		// Delete module
		for( PatchInfo& p : moduleGUI_ref.patches)
		{
			notifier.notify(glv::Update::Action, &p); // Notify the PatcherGUI to draw the patch chord
		}

		delete &moduleGUI_ref;
	}
}


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

		lithe::Patcher::connect(
			Inlets::last_selected_inlets_ref->moduleGUI_ref.module_ref.getInlet(Inlets::last_selected_inlet_index), 
			outlets_ref.moduleGUI_ref.module_ref.getOutlet(outlets_ref.selected_outlet) 
			);

		PatchInfo* p = new PatchInfo(*Inlets::last_selected_inlets_ref, outlets_ref);
		p->inlet_index = Inlets::last_selected_inlet_index;
		p->destination_nodeID = Inlets::last_selected_inlets_ref->moduleGUI_ref.module_ref.getNodeID();
		p->source_nodeID = outlets_ref.moduleGUI_ref.module_ref.getNodeID();
		p->outlet_index = outlets_ref.selected_outlet;

		notifier.notify(glv::Update::Action, p); // Notify the PatcherGUI to draw the patch chord

		p->inlets_ref.moduleGUI_ref.addPatch(*p);
		p->outlets_ref.moduleGUI_ref.addPatch(*p);


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


Outlets::Outlets(al::SoundEngine& se, al::ModuleGUI& moduleGUI, int size) : 
	soundengine_ref(se),
	moduleGUI_ref(moduleGUI), 
	mouseUpOutletEvent(*this),
	size(size)
{
	name("Outlets");
	enable(glv::Property::Controllable);
	disable(glv::DrawGrid);

	numOutlets = moduleGUI_ref.module_ref.numOutlets();
	buttons.reset( new glv::Buttons(glv::Rect(numOutlets*size, size), numOutlets, 1, true, true) );
	buttons->addHandler(glv::Event::MouseUp, mouseUpOutletEvent);

	buttons->enable(glv::Property::Controllable);
	buttons->padding(5);

	(*this) << buttons.get();
	fit();
}

/// Gets to abs co-ordinate of the center of a given outlet
glv::Point2 Outlets::getPatchPoint(int index)
{
	float x= size*(index+0.5), y=size/2;
	toAbs(x,y);
	return glv::Point2(x, y);
}

void Outlets::onDraw( glv::GLV& g)
{
	for(int i=0; i<buttons->size(); ++i)
	{
		if(buttons->getValue(i) == 1)
		{
			selected_outlet = i;
			return;
		}
	}
	selected_outlet = -1;
}

Inlets::Inlets(al::ModuleGUI& moduleGUI, int size) : 
	moduleGUI_ref(moduleGUI), 
	mouseUpInletEvent(*this),
	size(size)
{
	name("Inlets");
	enable(glv::Property::Controllable);
	disable(glv::DrawGrid);

	numInlets = moduleGUI_ref.module_ref.numInlets();
	buttons.reset(new glv::Buttons(glv::Rect(numInlets*size, size), numInlets, 1, true, false));
	buttons->addHandler(glv::Event::MouseUp, mouseUpInletEvent);

	buttons->enable(glv::Property::Controllable);
	buttons->padding(5);

	(*this) << buttons.get();
	fit();
}

glv::Point2 Inlets::getPatchPoint(int index)
{
	float x= size*(index+0.5), y=size/2;
	toAbs(x,y);
	return glv::Point2(x, y);
}

void Inlets::onDraw( glv::GLV& g) 
{
	getPatchPoint(3);

	for(int i=0; i<buttons->size(); ++i)
	{
		if(buttons->getValue(i) == 1)
		{
			selected_inlet = i;
			return;
		}
	}
	selected_inlet = -1;
}

}; //namespace al

