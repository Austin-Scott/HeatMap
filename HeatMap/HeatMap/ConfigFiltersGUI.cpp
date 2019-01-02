#include "ConfigFiltersGUI.h"

void ConfigFiltersGUI::setSubWindowInteractive(bool value)
{
	enabled(value);
	filterByActivityTypeGUI.setSubWindowInteractive(value);
	filterByDateGUI.setSubWindowInteractive(value);
	filterBySpeedGUI.setSubWindowInteractive(value);
}

ConfigFiltersGUI::ConfigFiltersGUI()
{
	caption("Configure Activity Filters");
	layout.div("<><vert weight=80% <><filterByDate><filterBySpeed><filterByActivityType><>><>");
	filterByDate.caption("Filter By Date");
	layout["filterByDate"] << filterByDate;
	filterBySpeed.caption("Filter By Average Speed");
	layout["filterBySpeed"] << filterBySpeed;
	filterByActivityType.caption("Filter By Activity Type");
	layout["filterByActivityType"] << filterByActivityType;
	layout.collocate();

	events().unload([&](const arg_unload &arg) {
		arg.cancel = true;
		hide();
	});

	filterByDate.events().click([&]() {
		filterByDateGUI.show();
	});

	filterBySpeed.events().click([&]() {
		filterBySpeedGUI.show();
	});

	filterByActivityType.events().click([&]() {
		filterByActivityTypeGUI.show();
	});
}
