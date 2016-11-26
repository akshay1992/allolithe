#ifndef AL_PATCHER_GUI_HPP
#define AL_PATCHER_GUI_HPP

#include "GLV/glv.h"
#include "GLV/glv_binding.h"

#include "allolithe/al_allolithe.hpp"
#include "allolithe/al_PatcherGUIComponents.hpp"

#include <iostream> 
using namespace std;

#define VERTICES_SIZE 2
namespace al{

struct PatchInfo
{
	Inlets* inlets;
	int inlet_index;
	Outlets* outlets;
	int outlet_index;
};


// class Patcher : public glv::View3D
// {
// public:
// 	Patcher(void)
// 	{
// 		PatchCable* p = new PatchCable;
// 		p->vertices[0] = glv::Point2(0,0);
// 		p->vertices[1] = glv::Point2(100,100);

// 		patches.push_back(*p);
// 	}
// 	virtual void onDraw2D(glv::GLV& g) override;

// 	// const int  = 2;

// 	struct PatchCable {
// 		PatchInfo info;
// 		glv::Point2 vertices[VERTICES_SIZE];
// 	};
	
// 	std::vector< PatchCable > patches;

// };


/** @brief A UI for patching allolithe Modules

	This UI runs a SoundEngine in it's backend and is written using GLV. 
*/
class PatcherGUI : public glv::GLV
{
public:
	PatcherGUI(al::SoundEngine& sound_engine = al::DefaultSoundEngine());
	void openWindow(void);

	virtual void onDraw(glv::GLV& g) override;

	static void onPatch(const glv::Notification &n)
	{
		cout << " Trying to patch something here" << endl;
		// PatchInfo* p = (PatchInfo*) n.data();
		// cout << p->outlet_index << " <---->  " ;
		// cout << p->inlet_index << endl;;
	}

	al::ViewpointWindow& createAlloWindow(void);

	~PatcherGUI(void);

	// void setParentWindow(al::Window& win) { top_view->parentWindow(win); }

	al::GLVDetachable* top_view;

private:
	al::SoundEngine& sound_engine_ref;
	RunStopButton run_button;
	QuitButton quit_button;
	ModuleList module_selector;
	// Patcher patcher;

	al::ViewpointWindow* allo_win;
	glv::Window* win;
};

}; // namespace al

#endif // INCLUDE_PATCHER_GUI_HPP