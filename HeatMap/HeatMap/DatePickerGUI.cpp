#include "DatePickerGUI.h"

Date DatePickerGUI::present()
{
	modality();
	return selected;
}

DatePickerGUI::DatePickerGUI(form & frm) : form(frm)
{
	caption("Please select a date");
	layout.div("vert <chooser>");
	layout["chooser"] << chooser;
	layout.collocate();

	events().unload([&]() {
		if (chooser.chose()) {
			date::value d = chooser.read().read();
			selected = Date(d.year, static_cast<Month>(d.month), d.day, 0, 0, 0);
		}
	});

	chooser.events().date_changed([&]() {
		close();
	});
}
