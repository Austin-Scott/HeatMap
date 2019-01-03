#include "FilterByDateGUI.h"

FilterByDateGUI::FilterByDateGUI()
{
	caption("Filter By Activity Date");
	events().unload([&](const arg_unload &arg) {
		arg.cancel = true;
		hide();
	});

	layout.div("<><vert weight=80% <><includeUnknown><filterEarlier><<labelOne><textboxOne><buttonOne>><filterLater><<labelTwo><textboxTwo><buttonTwo>><><<applyChanges><discardChangesButton>><>><>");
	includeUnknown.caption("Include activities with unknown dates");
	includeUnknown.events().checked([&]() {unsavedChanges = true; });
	layout["includeUnknown"] << includeUnknown;
	filterEarlier.caption("Filter activities with start dates earlier than");
	filterEarlier.events().checked([&]() {unsavedChanges = true; });
	layout["filterEarlier"] << filterEarlier;
	labelOne.caption("YYYY-MM-DD");
	layout["labelOne"] << labelOne;
	textboxOne.events().text_changed([&]() {unsavedChanges = true; });
	layout["textboxOne"] << textboxOne;
	buttonOne.caption("Choose");
	buttonOne.events().click([&]() {
		DatePickerGUI picker(*this);
		Date choosen = picker.present();
		if (choosen.isDateSet()) {
			unsavedChanges = true;
			textboxOne.caption(choosen.toFormalString());
		}
	});
	layout["buttonOne"] << buttonOne;
	filterLater.caption("Filter activities with start dates later than");
	filterLater.events().checked([&]() {unsavedChanges = true; });
	layout["filterLater"] << filterLater;
	labelTwo.caption("YYYY-MM-DD");
	layout["labelTwo"] << labelTwo;
	textboxTwo.events().text_changed([&]() {unsavedChanges = true; });
	layout["textboxTwo"] << textboxTwo;
	buttonTwo.caption("Choose");
	buttonTwo.events().click([&]() {
		DatePickerGUI picker(*this);
		Date choosen = picker.present();
		if (choosen.isDateSet()) {
			unsavedChanges = true;
			textboxTwo.caption(choosen.toFormalString());
		}
	});
	layout["buttonTwo"] << buttonTwo;
	applyChanges.caption("Apply");
	applyChanges.events().click([&]() {
		saveChanges();
	});
	layout["applyChanges"] << applyChanges;
	discardChangesButton.caption("Discard");
	discardChangesButton.events().click([&]() {
		discardChanges();
	});
	layout["discardChangesButton"] << discardChangesButton;
	layout.collocate();

	nanaTime.interval(100);
	nanaTime.elapse([&]() {
		if (unsavedChanges) {
			if (!applyChanges.enabled()) {
				applyChanges.enabled(true);
				discardChangesButton.enabled(true);
				caption("*Filter By Activity Date");
			}
		}
		else {
			if (applyChanges.enabled()) {
				applyChanges.enabled(false);
				discardChangesButton.enabled(false);
				caption("Filter By Activity Date");
			}
		}
	});
	nanaTime.start();

	unsavedChanges = false;
}

void FilterByDateGUI::setConfig(HeatMapConfiguration * config)
{
	this->config = config;

}

void FilterByDateGUI::setSubWindowInteractive(bool value)
{
	enabled(value);
}

void FilterByDateGUI::saveChanges()
{
	bool shouldIncludeUnknown = includeUnknown.checked();
	bool filterOne = false;
	Date dateOne;
	bool filterTwo = false;
	Date dateTwo;

	if (filterEarlier.checked()) {
		filterOne = true;
		dateOne = Date::fromFormalString(textboxOne.caption());
	}
	if (filterLater.checked()) {
		filterTwo = true;
		dateTwo = Date::fromFormalString(textboxTwo.caption());
	}
	config->setDateFilter(filterOne, dateOne, filterTwo, dateTwo, shouldIncludeUnknown);
	unsavedChanges = false;
}

void FilterByDateGUI::discardChanges()
{
	includeUnknown.check(config->includeUnknownDates);
	filterEarlier.check(config->useDateFilteringOne);
	filterLater.check(config->useDateFilteringTwo);
	if (config->startDate.isDateSet()) {
		textboxOne.caption(config->startDate.toFormalString());
	}
	else {
		textboxOne.caption("");
	}
	if (config->endDate.isDateSet()) {
		textboxTwo.caption(config->endDate.toFormalString());
	}
	else {
		textboxTwo.caption("");
	}

	unsavedChanges = false;
}

bool FilterByDateGUI::hasUnsavedChanges()
{
	return unsavedChanges;
}
