#include "FilterByActivityTypeGUI.h"

void FilterByActivityTypeGUI::setConfig(HeatMapConfiguration * config, form* parentFrm)
{
	this->config = config;
	this->parentFrm = parentFrm;
}

FilterByActivityTypeGUI::FilterByActivityTypeGUI(form & frm) : group(frm)
{
	caption("Filter Activity Type");
}

void FilterByActivityTypeGUI::saveChanges()
{
}

void FilterByActivityTypeGUI::discardChanges()
{
}

bool FilterByActivityTypeGUI::hasUnsavedChanges()
{
	return unsavedChanges;
}
