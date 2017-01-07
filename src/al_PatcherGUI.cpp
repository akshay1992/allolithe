#include "allolithe/al_PatcherGUI.hpp"

#include <memory>
#include <iostream>
using namespace std;

#include "allolithe/al_ModuleGUI.hpp"

namespace al{

PatchChords PatcherGUI::patchChords=PatchChords();

PatcherGUI::PatcherGUI(al::SoundEngine& sound_engine) : 
	sound_engine_ref(sound_engine),
	module_selector(sound_engine_ref),
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
	const std::shared_ptr<PatchInfo> p = *n.data<std::shared_ptr<PatchInfo>>();
	patchChords.addPatch(p);

	int index = patchChords.patches.size() - 1;

	p->inlets_ref.moduleGUI_ref.patch_indices.push_back( index );
	p->outlets_ref.moduleGUI_ref.patch_indices.push_back( index );
}

void PatcherGUI::onUnPatch(const glv::Notification &n)
{
	const int p_index = *n.data<int>();

	cout << "Here: " << p_index << endl;
	// patchChords.removePatchAtIndex(p_index);
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