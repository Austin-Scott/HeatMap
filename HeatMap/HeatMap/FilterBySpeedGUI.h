#pragma once
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/combox.hpp>

#include "HeatMapConfiguration.h"

using namespace std;
using namespace nana;

class FilterBySpeedGUI : public group {
private:
	HeatMapConfiguration* config;

	place layout{ *this };

	checkbox includeUnknown{ *this };
	checkbox filterSlower{ *this };
	checkbox filterFaster{ *this };
	combox comboxOne{ *this };
	combox comboxTwo{ *this };
	spinbox spinboxOne{ *this };
	spinbox spinboxTwo{ *this };
	button saveChangesButton{ *this };
	button discardChangesButton{ *this };

	void setupCombox(combox &c);

	bool unsavedChanges;

	form* parentFrm;
public:
	FilterBySpeedGUI(form &frm);
	void setConfig(HeatMapConfiguration* config, form* parentFrm);
	void saveChanges();
	void discardChanges();
	bool hasUnsavedChanges();
};