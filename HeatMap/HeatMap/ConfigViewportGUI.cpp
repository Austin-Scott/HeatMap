#include "ConfigViewportGUI.h"

void ConfigViewportGUI::setSubWindowInteractive(bool value)
{
	enabled(value);
}

void ConfigViewportGUI::setConfig(HeatMapConfiguration * config)
{
	this->config = config;
}

ConfigViewportGUI::ConfigViewportGUI()
{
	caption("Configure Viewport");
	events().unload([&](const arg_unload &arg) {
		arg.cancel = true;
		hide();
	});
}
