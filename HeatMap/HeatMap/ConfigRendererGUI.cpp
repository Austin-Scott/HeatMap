#include "ConfigRendererGUI.h"

void ConfigRendererGUI::setSubWindowInteractive(bool value)
{
	enabled(value);
}

void ConfigRendererGUI::setConfig(HeatMapConfiguration * config)
{
	this->config = config;
}

ConfigRendererGUI::ConfigRendererGUI()
{
	caption("Configure Renderer");
	events().unload([&](const arg_unload &arg) {
		arg.cancel = true;
		hide();
	});
}
