#include "allolithe/al_PatcherGUI.hpp"

#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include "allolithe/al_ModuleGUI.hpp"

namespace al{

PatchChords PatcherGUI::patchChords=PatchChords();

PatcherGUI::PatcherGUI(al::SoundEngine& sound_engine) : 
	sound_engine_ref(sound_engine),
	module_selector(*this, sound_engine_ref),
	run_button(sound_engine_ref)
{
	name("LithePatcherGUI");
	module_selector.refreshLlist();
	(*this).add(module_selector);
	(*this).add(run_button);
	(*this).add(quit_button);
}

void PatcherGUI::onPatch(const glv::Notification &n)
{
	const PatchInfo& p = *n.data<PatchInfo>();
	PatchInfo p_copy = p;

	patchChords.addPatch(p_copy);

	int index = patchChords.patches.size() - 1;

	p_copy.print();

	// Add patch to inlets and outlets
	p_copy.outlets_ref->patch_indices.at(p_copy.outlet_index).push_back( index );
	p_copy.inlets_ref->patch_indices[p_copy.inlet_index] = index;
}

void PatcherGUI::onUnPatch(const glv::Notification &n)
{
	const int p_index = *n.data<int>();
	PatchInfo& p = patchChords.patches[p_index];

	// Remove patch from inlet
	p.inlets_ref->patch_indices[p.inlet_index] = -1;;
	p.inlets_ref->updateState(p.inlet_index);

	// Remove patch from outlet
	std::vector<int>& patches_at_outlet = p.outlets_ref->patch_indices.at(p.outlet_index);
	for( int i=0; i< patches_at_outlet.size(); ++i)
	{
		if( patches_at_outlet[i] == p_index)
		{
		    swap(patches_at_outlet[i], patches_at_outlet.back());
		    patches_at_outlet.pop_back();
		}
	}
	// patches_at_outlet.erase(std::remove(patches_at_outlet.begin(), patches_at_outlet.end(), p_index), patches_at_outlet.end());
	cout <<patches_at_outlet.size() << endl;
    p.outlets_ref->updateState(p.outlet_index);

    // Remove PatchChord from drawing list
	patchChords.removePatchAtIndex(p_index);
}

al::ModuleGUI& PatcherGUI::instantiateModule(std::string moduleName)
{
	cout << "instantiating Module" << endl;

	NodeInfo node = sound_engine_ref.instantiateModule(moduleName);

	if( ! sound_engine_ref.sinkIsSet() && 
		sound_engine_ref.getModuleInfo(node.moduleName).isASink )
	{
		sound_engine_ref.setSink(node.nodeID, node.moduleName);
	}

	al::Module& module_ref = al::Module::getModuleRef(node.nodeID);
	al::ModuleGUI* module_gui = new ModuleGUI(*this, sound_engine_ref, module_ref, node.moduleName);
	glv::View* module_gui_view = &module_gui->getView();
	add(*module_gui_view);

	return *module_gui;
}

void PatcherGUI::openWindow(void)
{
	win.reset(new glv::Window(800,600, "LitheModular", this));
	glv::Application::run();
}

void PatcherGUI::onDraw(glv::GLV& g)
{
	patchChords.draw(g);
}

PatcherGUI::~PatcherGUI(void)
{
}


}; //namespace