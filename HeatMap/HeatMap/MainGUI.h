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
#include "ConfigRendererGUI.h"
#include "RenderMapGUI.h"
#include "FilterByDateGUI.h"
#include "FilterBySpeedGUI.h"
#include "FilterByActivityTypeGUI.h"

#include "HeatMapConfiguration.h"
#include "Image.h"
#include "MapQuestKeyGUI.h"

#include <fstream>
#include <Windows.h>

using namespace std;
using namespace nana;

class MainGUI : public form {
private:
	place layout{ *this };
	button renderButton{ *this };
	button saveButton{ *this };
	button loadButton{ *this };
	label saveLoadLabel{ *this };
	label websiteLabel{ *this };

	label statusLabel{ *this };

	timer nanaTime;

	ConfigViewportGUI configViewportGUI{ *this };
	ConfigRendererGUI configRendererGUI{ *this };

	FilterByDateGUI filterByDateGUI{ *this };
	FilterBySpeedGUI filterBySpeedGUI{ *this };
	FilterByActivityTypeGUI filterByActivityTypeGUI{ *this };

	HeatMapConfiguration heatMapConfiguration;
	function<Image*(HeatMapConfiguration, vector<Activity*>)> renderHeatMap;
	future<Image*> fut;
	atomic<unsigned int>* currentProgress;
	atomic<bool>* shouldCancel;
	atomic<bool>* progressKnown;
	mutex* statusMutex;
	string* statusString;

	vector<Activity*> activities;

public:
	MainGUI();
	void present(function<Image*(HeatMapConfiguration, vector<Activity*>)> renderHeatMap, atomic<unsigned int>* currentProgress, atomic<bool>* shouldCancel, atomic<bool>* progressKnown, mutex* statusMutex, string* statusString, vector<Activity*> activities);
};