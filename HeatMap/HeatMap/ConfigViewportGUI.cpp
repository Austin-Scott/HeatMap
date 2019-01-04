#include "ConfigViewportGUI.h"

void ConfigViewportGUI::setConfig(HeatMapConfiguration * config, form* parentFrm)
{
	this->config = config;
	this->parentFrm = parentFrm;
}

ConfigViewportGUI::ConfigViewportGUI(form & frm) : group(frm)
{
	caption("Configure Viewport");
	layout.div("<><weight=97% <<vert <><labelOne><<labelTwo><spinboxOne><labelThree><spinboxTwo>><labelFour><<labelFive><spinboxThree><labelSix><spinboxFour>><><<acceptChangesButton><discardChangesButton>><>>|<vert <><helpTextOne><<labelSeven><spinboxFive>><labelEight><<labelNine><spinboxSix><labelTen><spinboxSeven>><><buttonOne><>>|<vert <><weight=37.5% helpTextTwo><<spinboxEight><labelTwelve>><><buttonTwo><>>>><>");

	labelTwelve.caption("radius in kilometers");
	layout["labelTwelve"] << labelTwelve;
	spinboxEight.range(1.0, 300.0, 0.1);
	layout["spinboxEight"] << spinboxEight;
	buttonTwo.caption("Compute");
	layout["buttonTwo"] << buttonTwo;

	helpTextOne.caption("Compute coordinates that maintain aspect ratio:");
	layout["helpTextOne"] << helpTextOne;
	helpTextTwo.caption("Don't want to mess with coordinates? This tool will compute the viewport dimensions based on the bounds of activities that start within a specified radius of the median activity start point.");
	layout["helpTextTwo"] << helpTextTwo;
	
	labelOne.caption("Coordinate of lower left corner:");
	layout["labelOne"] << labelOne;
	labelTwo.caption("Latitude: ");
	labelTwo.text_align(align::right);
	layout["labelTwo"] << labelTwo;
	labelThree.caption("Longitude: ");
	labelThree.text_align(align::right);
	layout["labelThree"] << labelThree;
	labelFour.caption("Coordinate of upper right corner:");
	layout["labelFour"] << labelFour;
	labelFive.caption("Latitude: ");
	labelFive.text_align(align::right);
	layout["labelFive"] << labelFive;
	labelSix.caption("Longitude: ");
	labelSix.text_align(align::right);
	layout["labelSix"] << labelSix;

	labelSeven.caption("Highest latitude: ");
	labelSeven.text_align(align::right);
	layout["labelSeven"] << labelSeven;
	labelEight.caption("Coordinate of bottom center:");
	layout["labelEight"] << labelEight;
	labelNine.caption("Latitude: ");
	labelNine.text_align(align::right);
	layout["labelNine"] << labelNine;
	labelTen.caption("Longitude: ");
	labelTen.text_align(align::right);
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
	spinboxOne.events().text_changed([&]() { unsavedChanges = true;  });
	layout["spinboxOne"] << spinboxOne;
	spinboxTwo.range(-180.0, 180.0, 0.01);
	spinboxTwo.caption("0.0");
	spinboxTwo.events().text_changed([&]() { unsavedChanges = true;  });
	layout["spinboxTwo"] << spinboxTwo;
	spinboxThree.range(-90.0, 90.0, 0.01);
	spinboxThree.caption("0.0");
	spinboxThree.events().text_changed([&]() { unsavedChanges = true;  });
	layout["spinboxThree"] << spinboxThree;
	spinboxFour.range(-180.0, 180.0, 0.01);
	spinboxFour.caption("0.0");
	spinboxFour.events().text_changed([&]() { unsavedChanges = true;  });
	layout["spinboxFour"] << spinboxFour;

	acceptChangesButton.caption("Apply");
	acceptChangesButton.events().click([&]() {saveChanges(); });
	layout["acceptChangesButton"] << acceptChangesButton;
	discardChangesButton.caption("Discard");
	discardChangesButton.events().click([&]() {discardChanges(); });
	layout["discardChangesButton"] << discardChangesButton;

	layout.collocate();

	nanaTime.interval(100);
	nanaTime.elapse([&]() {
		if (unsavedChanges) {
			if (!acceptChangesButton.enabled()) {
				acceptChangesButton.enabled(true);
				discardChangesButton.enabled(true);
				caption("*Configure Viewport");
			}
		}
		else {
			if (acceptChangesButton.enabled()) {
				acceptChangesButton.enabled(false);
				discardChangesButton.enabled(false);
				caption("Configure Viewport");
			}
		}
	});
	nanaTime.start();
}

ConfigViewportGUI::~ConfigViewportGUI()
{
	
}

void ConfigViewportGUI::saveChanges()
{
	this->config->lowerLeft = geoCoord(spinboxOne.to_double(), spinboxTwo.to_double());
	this->config->upperRight = geoCoord(spinboxThree.to_double(), spinboxFour.to_double());

	unsavedChanges = false;
}

void ConfigViewportGUI::discardChanges()
{
	spinboxOne.value(to_string(config->lowerLeft.getLat()));
	spinboxTwo.value(to_string(config->lowerLeft.getLon()));
	spinboxThree.value(to_string(config->upperRight.getLat()));
	spinboxFour.value(to_string(config->upperRight.getLon()));

	unsavedChanges = false;
}

bool ConfigViewportGUI::hasUnsavedChanges()
{
	return unsavedChanges;
}
