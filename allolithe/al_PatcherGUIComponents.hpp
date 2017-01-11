#ifndef AL_PATCHER_GUI_COMPONENTS_HPP
#define AL_PATCHER_GUI_COMPONENTS_HPP

#include "GLV/glv.h"
#include "GLV/glv_binding.h"
#include <algorithm>
#include <memory>

#include "allolithe/al_SoundEngine.hpp"

#include <iostream>
using namespace std;

/// @brief THis file contains all the GUI components used by al::PatcherGUI

namespace al{
	
class Inlets;
class Outlets;
class PatcherGUI;

struct PatchInfo
{
	PatchInfo(Inlets* inlets_ref, Outlets* outlets_ref) : inlets_ref(inlets_ref), outlets_ref(outlets_ref){}
	int source_nodeID;
	int inlet_index;
	Inlets* inlets_ref;
	
	int destination_nodeID;
	int outlet_index;
	Outlets* outlets_ref;

	void print()
	{
		std::cout << "Source:" << source_nodeID << " (" << inlet_index << ") -- Destination:"<< destination_nodeID << " (" << outlet_index  << ")" << std::endl;
	}
};

struct UnpatchMessage
{
	PatcherGUI* patcherGUI;
	int patch_index;
	bool alsoMakeSoundEngineCall = true;
};


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
	virtual void draw(glv::GLV& g);

	void addPatch( PatchInfo& p);

	void removePatchAtIndex(int p_index);

	std::vector<PatchInfo> patches;
	std::vector<int> delete_queue;
};


class ModuleList : public glv::Box
{
public:
	struct MouseUpEvent : public glv::EventHandler
	{
		MouseUpEvent(al::PatcherGUI& gui, ModuleList& moduleList);
		virtual bool onEvent(glv::View &v, glv::GLV &g);
		al::PatcherGUI& gui;
		ModuleList& moduleList;
	};

	ModuleList(al::PatcherGUI& gui, al::SoundEngine& se);

	MouseUpEvent mouseUpEvent;

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