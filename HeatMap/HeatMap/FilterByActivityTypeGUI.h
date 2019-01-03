#pragma once
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/group.hpp>

#include "HeatMapConfiguration.h"

using namespace std;
using namespace nana;

class FilterByActivityTypeGUI : public group {
private:
	HeatMapConfiguration* config;
	bool unsavedChanges;

	form* parentFrm;
public:
	FilterByActivityTypeGUI(form &frm);
	void setConfig(HeatMapConfiguration* config, form* parentFrm);
	void saveChanges();
	void discardChanges();
	bool hasUnsavedChanges();
};