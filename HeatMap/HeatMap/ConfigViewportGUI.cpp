#include "ConfigViewportGUI.h"

void ConfigViewportGUI::setSubWindowInteractive(bool value)
{
	enabled(value);
}

ConfigViewportGUI::ConfigViewportGUI()
{
	caption("Configure Viewport");
	events().unload([&](const arg_unload &arg) {
		arg.cancel = true;
		hide();
	});
}
