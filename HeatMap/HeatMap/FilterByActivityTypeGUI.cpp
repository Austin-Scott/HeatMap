#include "FilterByActivityTypeGUI.h"

void FilterByActivityTypeGUI::setConfig(HeatMapConfiguration * config, form* parentFrm)
{
	this->config = config;
	this->parentFrm = parentFrm;
}

FilterByActivityTypeGUI::FilterByActivityTypeGUI(form & frm) : group(frm)
{
	caption("Filter Activity Type");
	layout.div("<><vert weight=95% <><filterUnknown><filterRunning><filterCycling><filterWalking><filterSwimming><><<saveChangesButton><discardChangesButton>><>><>");
	filterUnknown.caption("Filter activities of Unknown type");
	layout["filterUnknown"] << filterUnknown;
	filterRunning.caption("Filter activities of Runnning type");
	layout["filterRunning"] << filterRunning;
	filterCycling.caption("Filter activities of Cycling type");
	layout["filterCycling"] << filterCycling;
	filterWalking.caption("Filter activities of Walking type");
	layout["filterWalking"] << filterWalking;
	filterSwimming.caption("Filter activities of Swimming type");
	layout["filterSwimming"] << filterSwimming;
	saveChangesButton.caption("Accept");
	layout["saveChangesButton"] << saveChangesButton;
	discardChangesButton.caption("Discard");
	layout["discardChangesButton"] << discardChangesButton;
	layout.collocate();

}

void FilterByActivityTypeGUI::saveChanges()
{
}

void FilterByActivityTypeGUI::discardChanges()
{
}

bool FilterByActivityTypeGUI::hasUnsavedChanges()
{
	return unsavedChanges;
}
