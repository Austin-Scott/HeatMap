#pragma once
#include <nana/gui.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/slider.hpp>

#include "HeatMapConfiguration.h"
#include "ColorPickerGUI.h"

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
	label labelLow{ *this };
	label labelHigh{ *this };
	label labelMapStyle{ *this };
	label labelHeatTransparency{ *this };
	label labelTransparent{ *this };
	label labelOpaque{ *this };
	spinbox spinboxOne{ *this };
	spinbox spinboxTwo{ *this };

	checkbox downloadMap{ *this };
	
	slider transparencySlider{ *this };

	combox mapStyle{ *this };

	textbox textboxOne{ *this };
	textbox textboxTwo{ *this };
	textbox textboxThree{ *this };
	textbox textboxLow{ *this };
	textbox textboxHigh{ *this };
	button buttonOne{ *this };
	button buttonTwo{ *this };
	button buttonLow{ *this };
	button buttonHigh{ *this };
	button buttonThree{ *this };

	

	timer nanaTime;

public:
	void setConfig(HeatMapConfiguration* config, form* parentFrm);
	ConfigRendererGUI(form& frm);
	void updateHighlight();
	void saveChanges();
	void discardChanges();
	bool hasUnsavedChanges();
};