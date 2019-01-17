#include "FilterByDateGUI.h"

FilterByDateGUI::FilterByDateGUI(form &frm) : group(frm)
{
	caption("Filter By Activity Date");
	

	layout.div("<><vert weight=95% <><includeUnknown><filterEarlier><<labelOne><textboxOne><weight=25% buttonOne>><filterLater><<labelTwo><textboxTwo><weight=25% buttonTwo>><weight=10%>><>");
	includeUnknown.caption("Include activities with unknown dates");
	includeUnknown.transparent(true);
	includeUnknown.events().checked([&]() {unsavedChanges = true; });
	layout["includeUnknown"] << includeUnknown;
	filterEarlier.caption("Filter activities with start dates earlier than");
	filterEarlier.transparent(true);
	filterEarlier.events().checked([&]() {unsavedChanges = true; });
	layout["filterEarlier"] << filterEarlier;
	labelOne.caption("YYYY-MM-DD");
	labelOne.text_align(align::right);
	layout["labelOne"] << labelOne;
	textboxOne.events().text_changed([&]() {unsavedChanges = true; });
	layout["textboxOne"] << textboxOne;
	buttonOne.caption("Choose");
	buttonOne.events().click([&]() {
		DatePickerGUI picker(*parentFrm);
		Date choosen = picker.present();
		if (choosen.isDateSet()) {
			unsavedChanges = true;
			textboxOne.caption(choosen.toFormalString());
		}
	});
	layout["buttonOne"] << buttonOne;
	filterLater.caption("Filter activities with start dates later than");
	filterLater.transparent(true);
	filterLater.events().checked([&]() {unsavedChanges = true; });
	layout["filterLater"] << filterLater;
	labelTwo.caption("YYYY-MM-DD");
	labelTwo.text_align(align::right);
	layout["labelTwo"] << labelTwo;
	textboxTwo.events().text_changed([&]() {unsavedChanges = true; });
	layout["textboxTwo"] << textboxTwo;
	buttonTwo.caption("Choose");
	buttonTwo.events().click([&]() {
		DatePickerGUI picker(*parentFrm);
		Date choosen = picker.present();
		if (choosen.isDateSet()) {
			unsavedChanges = true;
			textboxTwo.caption(choosen.toFormalString());
		}
	});
	layout["buttonTwo"] << buttonTwo;
	
	layout.collocate();

	nanaTime.interval(100);
	nanaTime.elapse([&]() {
		
	});
	nanaTime.start();

	unsavedChanges = false;
}

void FilterByDateGUI::setConfig(HeatMapConfiguration * config, form* parentFrm)
{
	this->config = config;
	this->parentFrm = parentFrm;
}

void FilterByDateGUI::saveChanges()
{
	config->includeUnknownDates = includeUnknown.checked();

	config->useDateFilteringOne = filterEarlier.checked();
	config->startDate = Date::fromFormalString(textboxOne.caption());
	config->useDateFilteringTwo = filterLater.checked();
	config->endDate = Date::fromFormalString(textboxTwo.caption());
	
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
