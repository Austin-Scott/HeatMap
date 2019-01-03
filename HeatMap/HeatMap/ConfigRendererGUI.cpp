#include "ConfigRendererGUI.h"


void ConfigRendererGUI::setConfig(HeatMapConfiguration * config, form* parentFrm)
{
	this->config = config;
	this->parentFrm = parentFrm;
}

ConfigRendererGUI::ConfigRendererGUI(form &frm) : group(frm)
{
	caption("Configure Renderer");
	
}

void ConfigRendererGUI::saveChanges()
{
}

void ConfigRendererGUI::discardChanges()
{
}

bool ConfigRendererGUI::hasUnsavedChanges()
{
	return unsavedChanges;
}
