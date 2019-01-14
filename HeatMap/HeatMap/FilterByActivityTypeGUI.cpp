#include "FilterByActivityTypeGUI.h"

void FilterByActivityTypeGUI::setConfig(HeatMapConfiguration * config, form* parentFrm)
{
	this->config = config;
	this->parentFrm = parentFrm;
}

FilterByActivityTypeGUI::FilterByActivityTypeGUI(form & frm) : group(frm)
{
	caption("Filter Activity Type");
	layout.div("<><vert weight=95% <><filterUnknown><filterRunning><filterCycling><filterWalking><filterSwimming><weight=10%>><>");
	filterUnknown.caption("Filter activities of Unknown type");
	filterUnknown.events().checked([&]() { unsavedChanges = true; });
	layout["filterUnknown"] << filterUnknown;
	filterRunning.caption("Filter activities of Running type");
	filterRunning.events().checked([&]() { unsavedChanges = true; });
	layout["filterRunning"] << filterRunning;
	filterCycling.caption("Filter activities of Cycling type");
	filterCycling.events().checked([&]() { unsavedChanges = true; });
	layout["filterCycling"] << filterCycling;
	filterWalking.caption("Filter activities of Walking type");
	filterWalking.events().checked([&]() { unsavedChanges = true; });
	layout["filterWalking"] << filterWalking;
	filterSwimming.caption("Filter activities of Swimming type");
	filterSwimming.events().checked([&]() { unsavedChanges = true; });
	layout["filterSwimming"] << filterSwimming;
	layout.collocate();

	nanaTime.interval(100);
	nanaTime.elapse([&]() {
		
	});
	nanaTime.start();

}

void FilterByActivityTypeGUI::saveChanges()
{
	vector<ActivityType> filters;
	if (filterUnknown.checked()) {
		filters.push_back(ActivityType::Unknown);
	}
	if (filterRunning.checked()) {
		filters.push_back(ActivityType::Running);
	}
	if (filterCycling.checked()) {
		filters.push_back(ActivityType::Cycling);
	}
	if (filterWalking.checked()) {
		filters.push_back(ActivityType::Walking);
	}
	if (filterSwimming.checked()) {
		filters.push_back(ActivityType::Swimming);
	}
	config->activityFilters = filters;

	unsavedChanges = false;
}

void FilterByActivityTypeGUI::discardChanges()
{
	bool fUnknown = false;
	bool fRunning = false;
	bool fCycling = false;
	bool fWalking = false;
	bool fSwimming = false;
	for (ActivityType t : config->activityFilters) {
		switch (t) {
		case ActivityType::Unknown:
			fUnknown = true;
			break;
		case ActivityType::Running:
			fRunning = true;
			break;
		case ActivityType::Cycling:
			fCycling = true;
			break;
		case ActivityType::Walking:
			fWalking = true;
			break;
		case ActivityType::Swimming:
			fSwimming = true;
			break;
		}
	}
	filterUnknown.check(fUnknown);
	filterRunning.check(fRunning);
	filterCycling.check(fCycling);
	filterWalking.check(fWalking);
	filterSwimming.check(fSwimming);

	unsavedChanges = false;
}

bool FilterByActivityTypeGUI::hasUnsavedChanges()
{
	return unsavedChanges;
}
