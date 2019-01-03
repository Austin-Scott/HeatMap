#include "ConfigViewportGUI.h"

void ConfigViewportGUI::setConfig(HeatMapConfiguration * config, form* parentFrm)
{
	this->config = config;
	this->parentFrm = parentFrm;
}

ConfigViewportGUI::ConfigViewportGUI(form & frm) : group(frm)
{
	caption("Configure Viewport");
	layout.div("<vert <><labelOne><<labelTwo><spinboxOne><labelThree><spinboxTwo>><labelFour><<labelFive><spinboxThree><labelSix><spinboxFour>><><<acceptChangesButton><discardChangesButton>><>>|<vert <><<labelSeven><spinboxFive>><labelEight><<labelNine><spinboxSix><labelTen><spinboxSeven>><><buttonOne><>>|<vert <><labelEleven><<spinboxEight><labelTwelve>><><buttonTwo><>>");

	labelEleven.caption("Distance from median start point: ");
	layout["labelEleven"] << labelEleven;
	labelTwelve.caption("kilometers");
	layout["labelTwelve"] << labelTwelve;
	spinboxEight.range(1.0, 300.0, 0.1);
	layout["spinboxEight"] << spinboxEight;
	buttonTwo.caption("Compute");
	layout["buttonTwo"] << buttonTwo;
	
	labelOne.caption("Coordinate of lower left corner:");
	layout["labelOne"] << labelOne;
	labelTwo.caption("Latitude: ");
	layout["labelTwo"] << labelTwo;
	labelThree.caption("Longitude: ");
	layout["labelThree"] << labelThree;
	labelFour.caption("Coordinate of upper right corner:");
	layout["labelFour"] << labelFour;
	labelFive.caption("Latitude: ");
	layout["labelFive"] << labelFive;
	labelSix.caption("Longitude: ");
	layout["labelSix"] << labelSix;

	labelSeven.caption("Highest latitude: ");
	layout["labelSeven"] << labelSeven;
	labelEight.caption("Coordinate of bottom center:");
	layout["labelEight"] << labelEight;
	labelNine.caption("Latitude: ");
	layout["labelNine"] << labelNine;
	labelTen.caption("Longitude: ");
	layout["labelTen"] << labelTen;
	spinboxFive.range(-90.0, 90.0, 0.01);
	spinboxFive.caption("0.0");
	layout["spinboxFive"] << spinboxFive;
	spinboxSix.range(-90.0, 90.0, 0.01);
	spinboxSix.caption("0.0");
	layout["spinboxSix"] << spinboxSix;
	spinboxSeven.range(-180.0, 180.0, 0.01);
	spinboxSeven.caption("0.0");
	layout["spinboxSeven"] << spinboxSeven;
	buttonOne.caption("Compute");
	layout["buttonOne"] << buttonOne;

	spinboxOne.range(-90.0, 90.0, 0.01);
	spinboxOne.caption("0.0");
	layout["spinboxOne"] << spinboxOne;
	spinboxTwo.range(-180.0, 180.0, 0.01);
	spinboxTwo.caption("0.0");
	layout["spinboxTwo"] << spinboxTwo;
	spinboxThree.range(-90.0, 90.0, 0.01);
	spinboxThree.caption("0.0");
	layout["spinboxThree"] << spinboxThree;
	spinboxFour.range(-180.0, 180.0, 0.01);
	spinboxFour.caption("0.0");
	layout["spinboxFour"] << spinboxFour;

	acceptChangesButton.caption("Apply");
	layout["acceptChangesButton"] << acceptChangesButton;
	discardChangesButton.caption("Discard");
	layout["discardChangesButton"] << discardChangesButton;

	layout.collocate();
}

ConfigViewportGUI::~ConfigViewportGUI()
{
	
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
