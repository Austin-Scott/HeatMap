#include "ConfigViewportGUI.h"

void ConfigViewportGUI::setConfig(HeatMapConfiguration * config, form* parentFrm)
{
	this->config = config;
	this->parentFrm = parentFrm;
}

ConfigViewportGUI::ConfigViewportGUI(form & frm) : group(frm)
{
	caption("Configure Viewport");
}

void ConfigViewportGUI::saveChanges()
{
}

void ConfigViewportGUI::discardChanges()
{
}

bool ConfigViewportGUI::hasUnsavedChanges()
{
	return unsavedChanges;
}
