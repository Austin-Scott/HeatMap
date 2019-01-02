#include "FilterByActivityTypeGUI.h"

FilterByActivityTypeGUI::FilterByActivityTypeGUI()
{
	caption("Filter By Activity Type");
	events().unload([&](const arg_unload &arg) {
		arg.cancel = true;
		hide();
	});
}

void FilterByActivityTypeGUI::setConfig(HeatMapConfiguration * config)
{
	this->config = config;
}

void FilterByActivityTypeGUI::setSubWindowInteractive(bool value)
{
	enabled(value);
}
