#include "FilterByDateGUI.h"

FilterByDateGUI::FilterByDateGUI()
{
	caption("Filter By Activity Date");
	events().unload([&](const arg_unload &arg) {
		arg.cancel = true;
		hide();
	});
}

void FilterByDateGUI::setSubWindowInteractive(bool value)
{
	enabled(value);
}
