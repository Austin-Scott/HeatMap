#include "FilterBySpeedGUI.h"

void FilterBySpeedGUI::setConfig(HeatMapConfiguration * config, form* parentFrm)
{
	this->config = config;
	this->parentFrm = parentFrm;
}

FilterBySpeedGUI::FilterBySpeedGUI(form & frm) : group(frm)
{
	caption("Filter Activity Speed");
}

void FilterBySpeedGUI::saveChanges()
{
}

void FilterBySpeedGUI::discardChanges()
{
}

bool FilterBySpeedGUI::hasUnsavedChanges()
{
	return unsavedChanges;
}
