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

	c.events().text_changed([&]() {unsavedChanges = true; });
}

FilterBySpeedGUI::FilterBySpeedGUI(form & frm) : group(frm)
{
	caption("Filter Activity Speed");
	layout.div("<><vert weight=95% <><includeUnknown><filterSlower><<spinboxOne><comboxOne>><filterFaster><<spinboxTwo><comboxTwo>><><<saveChangesButton><discardChangesButton>><>><>");
	includeUnknown.caption("Include activities with unknown speeds");
	includeUnknown.events().checked([&]() {unsavedChanges = true; });
	layout["includeUnknown"] << includeUnknown;
	filterSlower.caption("Filter activities slower than");
	filterSlower.events().checked([&]() {unsavedChanges = true; });
	layout["filterSlower"] << filterSlower;
	filterFaster.caption("Filter activities faster than");
	filterFaster.events().checked([&]() {unsavedChanges = true; });
	layout["filterFaster"] << filterFaster;
	setupCombox(comboxOne);
	layout["comboxOne"] << comboxOne;
	setupCombox(comboxTwo);
	layout["comboxTwo"] << comboxTwo;
	spinboxOne.range(0.1, 100.0, 0.1);
	spinboxOne.events().text_changed([&]() {unsavedChanges = true; });
	layout["spinboxOne"] << spinboxOne;
	spinboxTwo.range(0.1, 100.0, 0.1);
	spinboxTwo.events().text_changed([&]() {unsavedChanges = true; });
	layout["spinboxTwo"] << spinboxTwo;
	saveChangesButton.caption("Apply");
	saveChangesButton.events().click([&]() {saveChanges(); });
	layout["saveChangesButton"] << saveChangesButton;
	discardChangesButton.caption("Discard");
	discardChangesButton.events().click([&]() {discardChanges(); });
	layout["discardChangesButton"] << discardChangesButton;
	layout.collocate();

	nanaTime.interval(100);
	nanaTime.elapse([&]() {
		if (unsavedChanges) {
			if (!saveChangesButton.enabled()) {
				saveChangesButton.enabled(true);
				discardChangesButton.enabled(true);
				caption("*Filter Activity Speed");
			}
		}
		else {
			if (saveChangesButton.enabled()) {
				saveChangesButton.enabled(false);
				discardChangesButton.enabled(false);
				caption("Filter Activity Speed");
			}
		}
	});
	nanaTime.start();
}

void FilterBySpeedGUI::saveChanges()
{
	if (includeUnknown.checked()) {
		config->includeUnknownSpeeds = true;
	}
	if (filterSlower.checked()) {
		config->useAverageSpeedFilteringOne = true;
		SpeedUnits units = SpeedUnits::MetersPerSecond;
		for (int i = 0; i < comboxOptions.size(); i++) {
			if (comboxOne.caption() == comboxOptions[i]) {
				units = comboxUnits[i];
				break;
			}
		}
		config->slowestSpeed = Speed(spinboxOne.to_double(), units);
	}
	else {
		config->useAverageSpeedFilteringOne = false;
	}
	if (filterFaster.checked()) {
		config->useAverageSpeedFilteringTwo = true;
		SpeedUnits units = SpeedUnits::MetersPerSecond;
		for (int i = 0; i < comboxOptions.size(); i++) {
			if (comboxTwo.caption() == comboxOptions[i]) {
				units = comboxUnits[i];
				break;
			}
		}
		config->fastestSpeed = Speed(spinboxTwo.to_double(), units);
	}
	else {
		config->useAverageSpeedFilteringTwo = false;
	}

	unsavedChanges = false;
}

void FilterBySpeedGUI::discardChanges()
{
	includeUnknown.check(config->includeUnknownSpeeds);

	
	filterSlower.check(config->useAverageSpeedFilteringOne);
	if (config->slowestSpeed.isSpeedSet()) {
		int caption = 0;
		for (int i = 0; i < comboxUnits.size(); i++) {
			if (config->slowestSpeed.getUnits() == comboxUnits[i]) {
				caption = i;
				break;
			}
		}
		comboxOne.option(caption);
		spinboxOne.value(to_string(config->slowestSpeed.getSpeed(config->slowestSpeed.getUnits())));
	}
	else {
		comboxOne.option(0);
		spinboxOne.value("0.1");
	}
	
	filterFaster.check(config->useAverageSpeedFilteringTwo);
	if (config->fastestSpeed.isSpeedSet()) {
		int caption = 0;
		for (int i = 0; i < comboxUnits.size(); i++) {
			if (config->fastestSpeed.getUnits() == comboxUnits[i]) {
				caption = i;
				break;
			}
		}
		comboxTwo.option(caption);
		spinboxTwo.value(to_string(config->fastestSpeed.getSpeed(config->fastestSpeed.getUnits())));
	}
	else {
		comboxTwo.option(0);
		spinboxTwo.value("0.1");
	}

	unsavedChanges = false;
}

bool FilterBySpeedGUI::hasUnsavedChanges()
{
	return unsavedChanges;
}
