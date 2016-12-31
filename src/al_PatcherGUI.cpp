#include "allolithe/al_PatcherGUI.hpp"

#include <iostream>
using namespace std;

namespace al{

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


// void Patcher::onDraw2D(glv::GLV& g)
// {
// 	using namespace glv::draw;
// 	stroke(2);

// 	for( PatchCable& p : patches)
// 	{
// 		paint( 1, p.vertices, VERTICES_SIZE);
// 	}

// }

void PatcherGUI::onPatch(const glv::Notification &n)
{
	cout << " Trying to patch something here" << endl;
	shared_ptr<al::PatchInfo> p = *n.receiver<shared_ptr<PatchInfo>>();
	// p->print();
}

void PatcherGUI::openWindow(void)
{
	win.reset(new glv::Window(800,600, "LitheModular", this));
	glv::Application::run();
}

void PatcherGUI::onDraw(glv::GLV& g)
{

}

PatcherGUI::~PatcherGUI(void)
{
}


}; //namespace