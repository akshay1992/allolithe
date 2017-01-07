#include "allolithe/al_PatcherGUI.hpp"

#include <iostream>
using namespace std;

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
	const PatchInfo& p = *n.data<PatchInfo>();
	patchChords.addPatch(p);
}

void PatcherGUI::onUnPatch(const glv::Notification &n)
{
	const PatchInfo& p = *n.data<PatchInfo>();
	patchChords.removePatch(p);
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