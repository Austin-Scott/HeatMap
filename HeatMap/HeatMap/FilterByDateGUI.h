#pragma once
#include <nana/gui.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/textbox.hpp>

#include "HeatMapConfiguration.h"

#include "DatePickerGUI.h"

using namespace std;
using namespace nana;

class FilterByDateGUI : public group {
private:
	HeatMapConfiguration* config;
	bool unsavedChanges;

	place layout{ *this };
	checkbox filterEarlier{ *this };
	checkbox filterLater{ *this };
	checkbox includeUnknown{ *this };
	textbox textboxOne{ *this };
	textbox textboxTwo{ *this };
	button buttonOne{ *this };
	button buttonTwo{ *this };
	label labelOne{ *this };
	label labelTwo{ *this };
	timer nanaTime;

	form* parentFrm;
	
public:
	FilterByDateGUI(form &frm);
	void setConfig(HeatMapConfiguration* config, form* parentFrm);
	void saveChanges();
	void discardChanges();
	bool hasUnsavedChanges();
};