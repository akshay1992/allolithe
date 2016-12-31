#ifndef AL_PATCHER_GUI_COMPONENTS_HPP
#define AL_PATCHER_GUI_COMPONENTS_HPP

#include "GLV/glv.h"
#include "GLV/glv_binding.h"
#include "allolithe/al_ModuleGUI.hpp"

/// @brief THis file contains all the GUI components used by al::PatcherGUI

namespace al{

struct InstantiateModuleEvent : public glv::EventHandler
{
	InstantiateModuleEvent(al::SoundEngine& se, int& selected_module_id, glv::Buttons& b);

	virtual bool onEvent(glv::View &v, glv::GLV &g);

	glv::Buttons& buttons;
	al::SoundEngine& sound_engine_ref;
	int& selected_module_id;
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
	int selected_module_id = -1;
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