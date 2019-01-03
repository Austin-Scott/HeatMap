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

class ConfigRendererGUI : public group {
private:
	HeatMapConfiguration* config;
	bool unsavedChanges;
	form* parentFrm;
public:
	void setConfig(HeatMapConfiguration* config, form* parentFrm);
	ConfigRendererGUI(form& frm);
	void saveChanges();
	void discardChanges();
	bool hasUnsavedChanges();
};