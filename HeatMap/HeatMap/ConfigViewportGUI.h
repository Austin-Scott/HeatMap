#pragma once
#include <nana/gui.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/tabbar.hpp>

#include "HeatMapConfiguration.h"

using namespace std;
using namespace nana;

class ConfigViewportGUI : public group {
private:
	HeatMapConfiguration* config;
	bool unsavedChanges;

	place layout{ *this };

	label labelOne{ *this };
	label labelTwo{ *this };
	label labelThree{ *this };
	label labelFour{ *this };
	label labelFive{ *this };
	label labelSix{ *this };

	label labelMedian{ *this };

	spinbox spinboxOne{ *this };
	spinbox spinboxTwo{ *this };
	spinbox spinboxThree{ *this };
	spinbox spinboxFour{ *this };

	label labelTwelve{ *this };

	spinbox spinboxEight{ *this };

	button buttonTwo{ *this };

	label labelSeven{ *this };
	label labelEight{ *this };
	label labelNine{ *this };
	label labelTen{ *this };

	spinbox spinboxFive{ *this };
	spinbox spinboxSix{ *this };
	spinbox spinboxSeven{ *this };

	button buttonOne{ *this };

	label helpTextOne{ *this };
	label helpTextTwo{ *this };

	timer nanaTime;

	form* parentFrm;
	vector<Activity*> activities;
public:
	void setConfig(HeatMapConfiguration* config, vector<Activity*> activities, form* parentFrm);
	ConfigViewportGUI(form &frm);
	~ConfigViewportGUI();
	void saveChanges();
	void discardChanges();
	bool hasUnsavedChanges();
};