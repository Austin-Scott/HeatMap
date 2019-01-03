#pragma once
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/group.hpp>

#include "HeatMapConfiguration.h"

using namespace std;
using namespace nana;

class ConfigRendererGUI : public group {
private:
	HeatMapConfiguration* config;
	bool unsavedChanges;
	form* parentFrm;

	place layout{ *this };

	label labelOne{ *this };
	label labelTwo{ *this };
	label labelThree{ *this };
	label labelFour{ *this };
	label labelFive{ *this };
	spinbox spinboxOne{ *this };
	spinbox spinboxTwo{ *this };
	checkbox useAnitAliasing{ *this };
	textbox textboxOne{ *this };
	textbox textboxTwo{ *this };
	textbox textboxThree{ *this };
	button buttonOne{ *this };
	button buttonTwo{ *this };
	button buttonThree{ *this };
	button buttonSaveChanges{ *this };
	button buttonDiscardChanges{ *this };

public:
	void setConfig(HeatMapConfiguration* config, form* parentFrm);
	ConfigRendererGUI(form& frm);
	void saveChanges();
	void discardChanges();
	bool hasUnsavedChanges();
};