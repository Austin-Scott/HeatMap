#pragma once
#include <memory>
#include <functional>

#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/group.hpp>

#include "ConfigViewportGUI.h"
#include "ConfigFiltersGUI.h"
#include "ConfigRendererGUI.h"

#include "HeatMapConfiguration.h"
#include "Image.h"

using namespace std;
using namespace nana;

class MainGUI : public form {
private:
	place layout{ *this };
	button configViewport{ *this };
	button configFilters{ *this };
	button configRenderer{ *this };
	button renderButton{ *this };

	ConfigViewportGUI configViewportGUI;
	ConfigFiltersGUI configFiltersGUI;
	ConfigRendererGUI configRendererGUI;

	HeatMapConfiguration heatMapConfiguration;
	function<Image*(HeatMapConfiguration, vector<Activity*>)> renderHeatMap;
	vector<Activity*> activities;

public:
	MainGUI();
	void present(function<Image*(HeatMapConfiguration, vector<Activity*>)> renderHeatMap, vector<Activity*> activities);
};