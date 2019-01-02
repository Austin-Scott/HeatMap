#include "FilterBySpeedGUI.h"

FilterBySpeedGUI::FilterBySpeedGUI()
{
	caption("Filter By Average Speed");
	events().unload([&](const arg_unload &arg) {
		arg.cancel = true;
		hide();
	});
}

void FilterBySpeedGUI::setSubWindowInteractive(bool value)
{
	enabled(value);
}
