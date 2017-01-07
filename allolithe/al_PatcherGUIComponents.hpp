#ifndef AL_PATCHER_GUI_COMPONENTS_HPP
#define AL_PATCHER_GUI_COMPONENTS_HPP

#include "GLV/glv.h"
#include "GLV/glv_binding.h"
#include "allolithe/al_ModuleGUI.hpp"
#include <algorithm>

#include <iostream>
using namespace std;

/// @brief THis file contains all the GUI components used by al::PatcherGUI

namespace al{

struct InstantiateModuleEvent : public glv::EventHandler
{
	InstantiateModuleEvent(al::SoundEngine& se, std::string& selected_module_id, glv::Buttons& b);

	virtual bool onEvent(glv::View &v, glv::GLV &g);

	glv::Buttons& buttons;
	al::SoundEngine& sound_engine_ref;
	std::string& selected_module_id;
};

class PatchChords : glv::View3D
{
public:
	virtual void draw(glv::GLV& g)
	{
		glv::GraphicsData& gd = g.graphicsData();

		for( PatchInfo p : patches)
		{
			glv::Point2 origin, destination;

			destination = p.inlets_ref.getPatchPoint(p.inlet_index);
			origin = p.outlets_ref.getPatchPoint(p.outlet_index);

			gd.addVertex(origin.x, origin.y);
			gd.addColor(glv::HSV(0,1,1));
			gd.addVertex(destination.x, destination.y);
			gd.addColor(glv::HSV(0,1,1));
			glv::draw::paint(glv::draw::Lines, gd);
		}
	}

	void addPatch(PatchInfo& p)
	{
		patches.push_back(p);
	}

	void removePatch(PatchInfo p)
	{
		patches.erase(std::remove(patches.begin(), patches.end(), p), patches.end());

		// cout << "remove patch" << endl;
		// patches.push_back(p);
	}

	std::vector<PatchInfo> patches;
};


class ModuleList : public glv::Box
{
public:
	InstantiateModuleEvent instantiateModuleEvent;

	ModuleList(al::SoundEngine& se);

	void refreshLlist(void);

	void onDraw(glv::GLV& g);

	glv::Buttons buttons;
	std::vector<ModuleInfo> modules;
	al::SoundEngine* sound_engine_ref;
	std::string selected_module_id = "";
};

class QuitButton : public glv::Button
{
public:
	QuitButton(void);

	virtual bool onEvent(glv::Event::t e, glv::GLV& g) override;
};

class RunStopButton : public glv::Button
{
public:
	RunStopButton(al::SoundEngine& se) ;

	virtual bool onEvent(glv::Event::t e, glv::GLV& g) override;

	bool state = false;
	al::SoundEngine& sound_engine;
};

}; // namespace al

#endif // AL_PATCHER_GUI_COMPONENTS_HPP