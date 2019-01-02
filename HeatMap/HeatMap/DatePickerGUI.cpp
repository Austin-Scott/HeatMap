#include "DatePickerGUI.h"

Date DatePickerGUI::present()
{
	modality();
	return selected;
}

DatePickerGUI::DatePickerGUI(form & frm) : form(frm)
{
	caption("Please select a date");
	layout.div("vert <weight=80% chooser><><pickDate>");
	layout["chooser"] << chooser;
	pickDate.caption("Pick selected date");
	layout["pickDate"] << pickDate;
	layout.collocate();

	pickDate.events().click([&]() {
		if (chooser.chose()) {
			date::value d = chooser.read().read();
			selected = Date(d.year, static_cast<Month>(d.month), d.day, 0, 0, 0);
		}
		close();
	});
}
