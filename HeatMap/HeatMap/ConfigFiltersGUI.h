#pragma once
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/group.hpp>

#include "HeatMapConfiguration.h"

#include "FilterByActivityTypeGUI.h"
#include "FilterByDateGUI.h"
#include "FilterBySpeedGUI.h"

using namespace std;
using namespace nana;

class ConfigFiltersGUI : public form {
private:
	FilterByActivityTypeGUI filterByActivityTypeGUI;
	FilterByDateGUI filterByDateGUI;
	FilterBySpeedGUI filterBySpeedGUI;

	place layout{ *this };
	button filterByActivityType{ *this };
	button filterByDate{ *this };
	button filterBySpeed{ *this };

public:
	void setSubWindowInteractive(bool value);
	ConfigFiltersGUI();
};