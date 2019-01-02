#include "ConfigRendererGUI.h"

void ConfigRendererGUI::setSubWindowInteractive(bool value)
{
	enabled(value);
}

ConfigRendererGUI::ConfigRendererGUI()
{
	caption("Configure Renderer");
	events().unload([&](const arg_unload &arg) {
		arg.cancel = true;
		hide();
	});
}
