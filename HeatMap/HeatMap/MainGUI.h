#pragma once
#include <memory>
#include <functional>
#include <future>
#include <thread>
#include <chrono>

#include <nana/gui.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/filebox.hpp>

#include "ConfigViewportGUI.h"
#include "ConfigFiltersGUI.h"
#include "ConfigRendererGUI.h"
#include "RenderMapGUI.h"

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

	timer time;

	ConfigViewportGUI configViewportGUI;
	ConfigFiltersGUI configFiltersGUI;
	ConfigRendererGUI configRendererGUI;

	HeatMapConfiguration heatMapConfiguration;
	function<Image*(HeatMapConfiguration, vector<Activity*>)> renderHeatMap;
	future<Image*> fut;
	atomic<unsigned int>* currentProgress;
	atomic<bool>* shouldCancel;
	atomic<bool>* progressKnown;
	vector<Activity*> activities;

public:
	MainGUI();
	void setSubWindowInteractive(bool value);
	void present(function<Image*(HeatMapConfiguration, vector<Activity*>)> renderHeatMap, atomic<unsigned int>* currentProgress, atomic<bool>* shouldCancel, atomic<bool>* progressKnown, vector<Activity*> activities);
};