#include "ConfigRendererGUI.h"


void ConfigRendererGUI::setConfig(HeatMapConfiguration * config, form* parentFrm)
{
	this->config = config;
	this->parentFrm = parentFrm;
}

ConfigRendererGUI::ConfigRendererGUI(form &frm) : group(frm)
{
	caption("Configure Renderer");
	layout.div("<><vert weight=95% <><<labelOne><spinboxOne><labelTwo><spinboxTwo>><useAnitAliasing><<weight=50% labelThree><textboxOne><buttonOne>><<weight=50% labelFour><textboxTwo><buttonTwo>><<weight=50% labelFive><textboxThree><buttonThree>><><<buttonSaveChanges><buttonDiscardChanges>><>><>");
	labelOne.caption("Width:");
	layout["labelOne"] << labelOne;
	labelTwo.caption("Height:");
	layout["labelTwo"] << labelTwo;
	labelThree.caption("Background hex color: ");
	layout["labelThree"] << labelThree;
	labelFour.caption("Minimum activity hex color: ");
	layout["labelFour"] << labelFour;
	labelFive.caption("Maximum activity hex color: ");
	layout["labelFive"] << labelFive;
	spinboxOne.range(1, INT_MAX, 1);
	layout["spinboxOne"] << spinboxOne;
	spinboxTwo.range(1, INT_MAX, 1);
	layout["spinboxTwo"] << spinboxTwo;
	useAnitAliasing.caption("Use Anti-Aliasing (Recommended)");
	useAnitAliasing.check(true);
	layout["useAnitAliasing"] << useAnitAliasing;
	textboxOne.caption("#000000FF");
	layout["textboxOne"] << textboxOne;
	textboxTwo.caption("#FF000080");
	layout["textboxTwo"] << textboxTwo;
	textboxThree.caption("#FFFFFFFF");
	layout["textboxThree"] << textboxThree;
	buttonOne.caption("Choose");
	layout["buttonOne"] << buttonOne;
	buttonTwo.caption("Choose");
	layout["buttonTwo"] << buttonTwo;
	buttonThree.caption("Choose");
	layout["buttonThree"] << buttonThree;
	buttonSaveChanges.caption("Apply");
	layout["buttonSaveChanges"] << buttonSaveChanges;
	buttonDiscardChanges.caption("Discard");
	layout["buttonDiscardChanges"] << buttonDiscardChanges;
	layout.collocate();
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
