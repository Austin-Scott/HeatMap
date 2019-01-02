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

class ConfigViewportGUI : public form {
private:
	HeatMapConfiguration* config;
public:
	void setSubWindowInteractive(bool value);
	void setConfig(HeatMapConfiguration* config);
	ConfigViewportGUI();
};