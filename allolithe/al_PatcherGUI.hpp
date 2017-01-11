#ifndef AL_PATCHER_GUI_HPP
#define AL_PATCHER_GUI_HPP

#include "GLV/glv.h"
#include "GLV/glv_binding.h"

#include "alloGLV/al_ControlGLV.hpp"
#include "allolithe/al_allolithe.hpp"
#include "allolithe/al_PatcherGUIComponents.hpp"

#include <iostream> 
#include <memory>
using namespace std;

#define VERTICES_SIZE 2

namespace al{

class ModuleGUI;

/** @brief A UI for patching allolithe Modules

	This UI runs a SoundEngine in it's backend and is written using GLV. 
*/
class PatcherGUI : public al::GLVDetachable
{
public:
	PatcherGUI(al::SoundEngine& sound_engine = al::DefaultSoundEngine());

	void openWindow(void);

	virtual void onDraw(glv::GLV& g) override;

	/// @brief Called internally by the GUI components whenever a patch needs to be made
	static void onPatch(const glv::Notification &n);

	static void onUnPatch(const glv::Notification &n);

	al::ModuleGUI& instantiateModule(std::string moduleName);

	~PatcherGUI(void);

	friend class ModuleGUI;

	al::SoundEngine& soundEngine(void){
		return sound_engine_ref;
	}
	
private:
	static PatchChords patchChords;
	al::SoundEngine& sound_engine_ref;
	RunStopButton run_button;
	QuitButton quit_button;
	ModuleList module_selector;
	unique_ptr<glv::Window> win;

};

}; // namespace al

#endif // INCLUDE_PATCHER_GUI_HPP