#include "FilterBySpeedGUI.h"

void FilterBySpeedGUI::setConfig(HeatMapConfiguration * config, form* parentFrm)
{
	this->config = config;
	this->parentFrm = parentFrm;
}

void FilterBySpeedGUI::setupCombox(combox & c)
{
	c.push_back("meters/second");
	c.push_back("miles/hour");
	c.push_back("kilometers/hour");
	c.push_back("minutes/mile");
	c.push_back("minutes/kilometer");

	c.caption("meters/second");
}

FilterBySpeedGUI::FilterBySpeedGUI(form & frm) : group(frm)
{
	caption("Filter Activity Speed");
	layout.div("<><vert weight=95% <><includeUnknown><filterSlower><<spinboxOne><comboxOne>><filterFaster><<spinboxTwo><comboxTwo>><><<saveChangesButton><discardChangesButton>><>><>");
	includeUnknown.caption("Include activities with unknown speeds");
	layout["includeUnknown"] << includeUnknown;
	filterSlower.caption("Filter activities slower than");
	layout["filterSlower"] << filterSlower;
	filterFaster.caption("Filter activities faster than");
	layout["filterFaster"] << filterFaster;
	setupCombox(comboxOne);
	layout["comboxOne"] << comboxOne;
	setupCombox(comboxTwo);
	layout["comboxTwo"] << comboxTwo;
	spinboxOne.range(0.1, 100.0, 0.1);
	layout["spinboxOne"] << spinboxOne;
	spinboxTwo.range(0.1, 100.0, 0.1);
	layout["spinboxTwo"] << spinboxTwo;
	saveChangesButton.caption("Apply");
	layout["saveChangesButton"] << saveChangesButton;
	discardChangesButton.caption("Discard");
	layout["discardChangesButton"] << discardChangesButton;
	layout.collocate();
}

void FilterBySpeedGUI::saveChanges()
{
}

void FilterBySpeedGUI::discardChanges()
{
}

bool FilterBySpeedGUI::hasUnsavedChanges()
{
	return unsavedChanges;
}
