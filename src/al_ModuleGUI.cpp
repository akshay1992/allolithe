#include "allolithe/al_ModuleGUI.hpp"
#include "allolithe/al_PatcherGUI.hpp"

#include <memory>
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
		try
		{
			// if( ! moduleGUI_ref.parentPatcherGUI_ref.soundEngine().getSink().isRunning() )
				moduleGUI_ref.deleteSelf();
		}
		catch(al::SinkNotSetException)
		{
			// Sink doesn't exist, it's okay to delete
			moduleGUI_ref.deleteSelf();
		}
	}
}


MouseUpOutletEvent::MouseUpOutletEvent(Outlets& outlets) : outlets_ref(outlets)
{	
}

bool MouseUpOutletEvent::onEvent(glv::View &v, glv::GLV &g)
{
	int index = outlets_ref.buttons->selected();
	int value = outlets_ref.buttons->getValue();

	if ( MouseUpInletEvent::active )
	{
		lithe::Patcher::connect(
			Inlets::last_selected_inlets_ref->moduleGUI_ref.module_ref.getInlet(Inlets::last_selected_inlet_index), 
			outlets_ref.moduleGUI_ref.module_ref.getOutlet(index) 
			);

		PatchInfo p(Inlets::last_selected_inlets_ref, &outlets_ref);
		p.inlet_index = Inlets::last_selected_inlet_index;
		p.destination_nodeID = Inlets::last_selected_inlets_ref->moduleGUI_ref.module_ref.getNodeID();
		p.source_nodeID = outlets_ref.moduleGUI_ref.module_ref.getNodeID();
		p.outlet_index = index;

		// Notify the PatcherGUI to that it has to draw the patch chord
		outlets_ref.moduleGUI_ref.patch_notifier.notify(glv::Update::Action, &p); 

		MouseUpInletEvent::active = false;
	}
	outlets_ref.updateState(index);
	return false;
}

bool MouseUpInletEvent::onEvent(glv::View &v, glv::GLV &g)
{	
	int index = inlets_ref.buttons->selected();
	int value = inlets_ref.buttons->getValue();

	if( ! active )
	{
		if( value == 1)
		{
			Inlets::last_selected_inlet_index = index;
			Inlets::last_selected_inlets_ref = &inlets_ref;
			active = true;
		}
		else
		{
			// If it's patched to something, unpatch
			if (inlets_ref.moduleGUI_ref.module_ref.getInlet(index).isConnected() ) 
			{
				// inlets_ref.moduleGUI_ref.module_ref.getInlet(index).disconnect();
				inlets_ref.buttons->setValue(0, index);

				UnpatchMessage m;
				m.patcherGUI = &inlets_ref.moduleGUI_ref.parentPatcherGUI_ref;
				m.patch_index = inlets_ref.patch_indices[index];

				inlets_ref.moduleGUI_ref.unpatch_notifier.notify(glv::Update::Action, &m);
			}
		}
	}
	else
	{
		if( &inlets_ref == Inlets::last_selected_inlets_ref )
		{
			if ( value == 1 && index !=Inlets::last_selected_inlet_index )
			{
				inlets_ref.buttons->setValue(0, Inlets::last_selected_inlet_index);
				Inlets::last_selected_inlet_index = index;
			}
			else
			{
				active = false;
				inlets_ref.buttons->setValue(0, index);
				inlets_ref.buttons->setValue(0, Inlets::last_selected_inlet_index);

				// If it's patched to something, unpatch
				if (inlets_ref.moduleGUI_ref.module_ref.getInlet(index).isConnected() ) 
				{
					UnpatchMessage m;
					m.patcherGUI = &inlets_ref.moduleGUI_ref.parentPatcherGUI_ref;
					m.patch_index = inlets_ref.patch_indices[index];

					// inlets_ref.moduleGUI_ref.module_ref.getInlet(index).disconnect();
					inlets_ref.moduleGUI_ref.unpatch_notifier.notify(glv::Update::Action, &m);
				}
			}
		}
		else
		{
			Inlets::last_selected_inlets_ref->buttons->setValue(0, Inlets::last_selected_inlet_index);

			Inlets::last_selected_inlet_index = index;
			Inlets::last_selected_inlets_ref = &inlets_ref;
		}
	}
	return false;
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

	for( int i=0; i< numOutlets; ++i)
	{
		patch_indices[i] = std::vector<int>();
	}
}

/// Gets to abs co-ordinate of the center of a given outlet
glv::Point2 Outlets::getPatchPoint(int index)
{
	float x= size*(index+0.5);
	float y= size/2;
	toAbs(x,y);
	return glv::Point2(x, y);
}

void Outlets::updateState(int outletIndex)
{	
	if( patch_indices.at(outletIndex).size() == 0)	
	{
		buttons->setValue(0, outletIndex);
	}
	else
	{
		buttons->setValue(1, outletIndex);
	}
}

void Outlets::dropAllPatchesAtOutlet(int outletIndex)
{
	for( int i : patch_indices.at(outletIndex) )
	{
		UnpatchMessage m;
		m.patcherGUI = &moduleGUI_ref.parentPatcherGUI_ref;
		m.patch_index = i;
		moduleGUI_ref.unpatch_notifier.notify(glv::Update::Action, &m);
	}
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
	buttons.reset(new glv::Buttons(glv::Rect(numInlets*size, size), numInlets, 1, false, false));
	buttons->addHandler(glv::Event::MouseUp, mouseUpInletEvent);

	buttons->enable(glv::Property::Controllable);
	buttons->padding(5);

	(*this) << buttons.get();
	fit();

	patch_indices.reserve(numInlets);
	for( int i=0; i< numInlets; ++i)
	{
		patch_indices.push_back(-1);
	}
}

glv::Point2 Inlets::getPatchPoint(int index)
{
	float x= size*(index+0.5); 
	float y= size/2;
	toAbs(x,y);
	return glv::Point2(x, y);
}

void Inlets::updateState(int inletIndex)
{	
	if( patch_indices[inletIndex] == -1)	
	{
		buttons->setValue(0, inletIndex);
	}
	else
	{
		buttons->setValue(1, inletIndex);
	}
}

void Inlets::dropPatch(int inletIndex)
{
	if (moduleGUI_ref.module_ref.getInlet(inletIndex).isConnected() )
	{
		UnpatchMessage m;
		m.patcherGUI = &moduleGUI_ref.parentPatcherGUI_ref;
		m.patch_index = patch_indices[inletIndex];
		// moduleGUI_ref.module_ref.getInlet(inlet_index).disconnect();	
		moduleGUI_ref.unpatch_notifier.notify(glv::Update::Action, &m);
	}
	buttons->setValue(0, inletIndex);
}

ModuleGUI::ModuleGUI(al::PatcherGUI& gui, al::SoundEngine& se, al::Module& module, std::string& moduleName) :
	parentPatcherGUI_ref(gui),
	soundengine_ref(se), 
	module_ref(module),
	moduleGUIKeyDownEvent(*this)
{
	mBox.reset(new glv::Box(glv::Direction::S) );
	mBox->pos(glv::Place::TL, 10, 0);
	
	addInlets();
	addName(moduleName);
	addParameters();
	addOutlets();

	top.reset(new glv::View(glv::Rect(0, 0, mBox->width() + 2*pad, mBox->height())) );
	*top << mBox.get();

	top->addHandler(glv::Event::MouseDrag, glv::Behavior::mouseMove);
	top->enable(glv::Property::Controllable);
	top->enable(glv::Property::DrawBorder);

	top->addHandler(glv::Event::KeyDown, moduleGUIKeyDownEvent);
	unpatch_notifier.attach(al::PatcherGUI::onUnPatch, glv::Update::Action);
	patch_notifier.attach(al::PatcherGUI::onPatch, glv::Update::Action);
}

void ModuleGUI::deleteSelf(void)
{
	for( int i=0; i< outlets->numOutlets; ++i)
	{
		outlets->dropAllPatchesAtOutlet(i);
	}

	for( int i=0; i< inlets->numInlets; ++i)
	{
		inlets->dropPatch(i);
	}

	soundengine_ref.deleteModuleInstance(module_ref.getNodeID());

	top->remove();

	// delete this;
}

ModuleGUI::~ModuleGUI()
{
}

void ModuleGUI::setParentPatcherGUI(PatcherGUI& patcherGUI)
{
	parentPatcherGUI_ref;
}

glv::View& ModuleGUI::getView(void) { return *top; }

void ModuleGUI::addInlets(void)
{
	inlets = new Inlets(*this, 15);
	inlets->pos(glv::Place::TL, 0, 0);
	(*mBox) << inlets;
}

void ModuleGUI::addOutlets(void)
{	
	outlets = new Outlets(soundengine_ref, *this, 15);
	outlets->pos(glv::Place::TL, 0, 0);
	(*mBox) << outlets;
}

void ModuleGUI::addName(std::string& moduleName, bool show_node_id)
{
	if(show_node_id)
		name_label.reset( new glv::Label(moduleName+" NodeID: " +std::to_string(module_ref.getNodeID())) );
	else
		name_label.reset( new glv::Label(moduleName) );
	name_label->pos(10, 0);
	(*mBox) << name_label.get();
}


void ModuleGUI::addParameters(void)
{
	for( int i=0; i < module_ref.numParams(); ++i)
	{
		addParameter(module_ref.parameter(i));
	}
}

void ModuleGUI::addParameter(al::Parameter& parameter)
{
	int numInt = 2 + ceil(log10(fmax(fabs(parameter.max()), fabs(parameter.min()))));
	glv::NumberDialer *number  = new glv::NumberDialer(numInt, 7 - numInt,
	                                                   parameter.max(),
	                                                   parameter.min());
	number->setValue(parameter.get());
	WidgetWrapper *wrapper = new WidgetWrapper;
	wrapper->parameter = &parameter;
	wrapper->lock = &mParameterGUILock;
	wrapper->widget = static_cast<glv::Widget *>(number);
	mWrappers.push_back(wrapper);
	number->attach(al::ModuleGUI::widgetChangeCallback, glv::Update::Value, wrapper);

	glv::Box *box = new glv::Box;
	*box << number << new glv::Label(parameter.getName());
	box->fit();
	(*mBox) << box;
	mBox->fit();
	// mBox.fit();
	parameter.registerChangeCallback(al::ModuleGUI::valueChangeCallback, wrapper);

	// return *this;
}

void ModuleGUI::widgetChangeCallback(const glv::Notification& n) {
	glv::Widget &sender = *n.sender<glv::Widget>();
	WidgetWrapper &receiver = *n.receiver<WidgetWrapper>();
	receiver.lock->lock();
	double value = sender.getValue<double>();
	receiver.lock->unlock();
	receiver.parameter->setNoCalls(value, &sender);
}

void ModuleGUI::valueChangeCallback(float value, void *sender, void *wrapper, void *blockThis) {
		WidgetWrapper *w = static_cast<WidgetWrapper *>(wrapper);
		if (wrapper != blockThis) {
			w->lock->lock(); // Is the setting of the value from the mouse also protected? Do we need to protect that?
			glv::Data &d = w->widget->data();
			d.assign<double>(value); // We need to assign this way to avoid triggering callbacks.
			w->lock->unlock();
		}

}

}; //namespace al

