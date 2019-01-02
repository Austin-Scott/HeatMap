#include "FilterByDateGUI.h"

FilterByDateGUI::FilterByDateGUI()
{
	caption("Filter By Activity Date");
	events().unload([&](const arg_unload &arg) {
		arg.cancel = true;
		hide();
	});

	layout.div("<><vert weight=80% <><filterEarlier><<labelOne><textboxOne><buttonOne>><filterLater><<labelTwo><textboxTwo><buttonTwo>><><applyChanges><>><>");
	filterEarlier.caption("Filter activities with start dates earlier than");
	layout["filterEarlier"] << filterEarlier;
	labelOne.caption("YYYY-MM-DD");
	layout["labelOne"] << labelOne;
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
	layout["filterLater"] << filterLater;
	labelTwo.caption("YYYY-MM-DD");
	layout["labelTwo"] << labelTwo;
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
		//TODO
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
