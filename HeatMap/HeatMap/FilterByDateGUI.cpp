#include "FilterByDateGUI.h"

FilterByDateGUI::FilterByDateGUI()
{
	caption("Filter By Activity Date");
	events().unload([&](const arg_unload &arg) {
		arg.cancel = true;
		hide();
	});

	layout.div("<><vert weight=80% <><includeUnknown><filterEarlier><<labelOne><textboxOne><buttonOne>><filterLater><<labelTwo><textboxTwo><buttonTwo>><><applyChanges><>><>");
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
	applyChanges.caption("Apply changes");
	applyChanges.events().click([&]() {
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
	});
	layout["applyChanges"] << applyChanges;
	layout.collocate();

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

bool FilterByDateGUI::hasUnsavedChanges()
{
	return unsavedChanges;
}
