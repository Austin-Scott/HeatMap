#include "ConfigViewportGUI.h"

void ConfigViewportGUI::setConfig(HeatMapConfiguration * config, vector<Activity*> activities, form* parentFrm)
{
	this->config = config;
	this->parentFrm = parentFrm;
	this->activities = activities;
}

ConfigViewportGUI::ConfigViewportGUI(form & frm) : group(frm)
{
	caption("Configure Viewport");
	layout.div("<><weight=97% <<vert <><manualOpt><labelOne><<labelTwo><spinboxOne><labelThree><spinboxTwo>><labelFour><<labelFive><spinboxThree><labelSix><spinboxFour>><weight=10%>>     <weight=3%>     <vert <><ratioOpt><helpTextOne><<labelSeven><spinboxFive>><labelEight><<labelNine><spinboxSix><labelTen><spinboxSeven>><weight=5%><weight=5%>>     <weight=3%>     <vert <><autoOpt><weight=35.5% helpTextTwo><weight=10% labelMedian><<spinboxEight><labelTwelve>><weight=5%><weight=5%>>>><>");

	rgroup.add(manualOpt);
	manualOpt.caption("Manual");
	manualOpt.transparent(true);
	layout["manualOpt"] << manualOpt;

	rgroup.add(ratioOpt);
	ratioOpt.caption("Ratio");
	ratioOpt.transparent(true);
	ratioOpt.events().checked([&]() {
		
	});
	layout["ratioOpt"] << ratioOpt;

	rgroup.add(autoOpt);
	autoOpt.caption("Automatic");
	autoOpt.transparent(true);
	autoOpt.events().checked([&]() {
		
	});
	autoOpt.check(true);
	layout["autoOpt"] << autoOpt;

	labelMedian.caption("Latitude: 44.9345, Longitude: 93.6532");
	labelMedian.text_align(align::center, align_v::top);
	layout["labelMedian"] << labelMedian;

	labelTwelve.caption(" radius in kilometers");
	labelTwelve.text_align(align::left, align_v::center);
	layout["labelTwelve"] << labelTwelve;
	spinboxEight.range(1.0, 300.0, 0.1);
	spinboxEight.value("10.0");
	spinboxEight.events().text_changed([&]() {
		updateAutoCoordinates();
	});
	layout["spinboxEight"] << spinboxEight;

	helpTextOne.caption("<bold>Compute bounds that maintain aspect ratio:</>");
	helpTextOne.format(true);
	layout["helpTextOne"] << helpTextOne;
	helpTextTwo.caption("<bold>Don't want to mess with coordinates? Make all activities that start within a specified radius of the median start point completely visible.</>");
	helpTextTwo.format(true);
	layout["helpTextTwo"] << helpTextTwo;
	
	labelOne.caption("<bold>Coordinate of lower left corner:</>");
	labelOne.format(true);
	labelOne.text_align(align::left, align_v::bottom);
	layout["labelOne"] << labelOne;
	labelTwo.caption("Latitude: ");
	labelTwo.text_align(align::right, align_v::center);
	layout["labelTwo"] << labelTwo;
	labelThree.caption("Longitude: ");
	labelThree.text_align(align::right, align_v::center);
	layout["labelThree"] << labelThree;
	labelFour.caption("<bold>Coordinate of upper right corner:</>");
	labelFour.format(true);
	labelFour.text_align(align::left, align_v::bottom);
	layout["labelFour"] << labelFour;
	labelFive.caption("Latitude: ");
	labelFive.text_align(align::right, align_v::center);
	layout["labelFive"] << labelFive;
	labelSix.caption("Longitude: ");
	labelSix.text_align(align::right, align_v::center);
	layout["labelSix"] << labelSix;

	labelSeven.caption("Highest latitude: ");
	labelSeven.text_align(align::right, align_v::center);
	layout["labelSeven"] << labelSeven;
	labelEight.caption("Coordinate of bottom center:");
	labelEight.text_align(align::left, align_v::bottom);
	layout["labelEight"] << labelEight;
	labelNine.caption("Latitude: ");
	labelNine.text_align(align::right, align_v::center);
	layout["labelNine"] << labelNine;
	labelTen.caption("Longitude: ");
	labelTen.text_align(align::right, align_v::center);
	layout["labelTen"] << labelTen;
	spinboxFive.range(-90.0, 90.0, 0.01);
	spinboxFive.caption("0.0");
	spinboxFive.events().text_changed([&]() {
		updateRatioCoordinates();
	});
	layout["spinboxFive"] << spinboxFive;
	spinboxSix.range(-90.0, 90.0, 0.01);
	spinboxSix.caption("0.0");
	spinboxSix.events().text_changed([&]() {
		updateRatioCoordinates();
	});
	layout["spinboxSix"] << spinboxSix;
	spinboxSeven.range(-180.0, 180.0, 0.01);
	spinboxSeven.caption("0.0");
	spinboxSeven.events().text_changed([&]() {
		updateRatioCoordinates();
	});
	layout["spinboxSeven"] << spinboxSeven;

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

	layout.collocate();

	nanaTime.interval(100);
	nanaTime.elapse([&]() {
		bool enableMan = false;
		bool enableRat = false;
		bool enableAut = false;
		if (manualOpt.checked()) {
			enableMan = true;
		}
		else if (ratioOpt.checked()) {
			enableRat = true;
		}
		else {
			enableAut = true;
		}

		if (spinboxOne.enabled() != enableMan) {
			spinboxOne.enabled(enableMan);
			spinboxTwo.enabled(enableMan);
			spinboxThree.enabled(enableMan);
			spinboxFour.enabled(enableMan);
		}
		if (spinboxFive.enabled() != enableRat) {
			spinboxFive.enabled(enableRat);
			spinboxSix.enabled(enableRat);
			spinboxSeven.enabled(enableRat);
		}
		if (spinboxEight.enabled() != enableAut) {
			spinboxEight.enabled(enableAut);
		}
	});
	nanaTime.start();
}

ConfigViewportGUI::~ConfigViewportGUI()
{
	
}

void ConfigViewportGUI::saveChanges()
{
	config->lowerLeft = geoCoord(spinboxOne.to_double(), spinboxTwo.to_double());
	config->upperRight = geoCoord(spinboxThree.to_double(), spinboxFour.to_double());

	if (manualOpt.checked()) {
		config->viewportMode = 0;
	}
	else if (ratioOpt.checked()) {
		config->viewportMode = 1;
	}
	else {
		config->viewportMode = 2;
	}

	config->highestLatitude = spinboxFive.to_double();
	config->bottomCenter = geoCoord(spinboxSix.to_double(), spinboxSeven.to_double());
	config->radius = spinboxEight.to_double();

	unsavedChanges = false;
}

void ConfigViewportGUI::discardChanges()
{
	spinboxOne.value(to_string(config->lowerLeft.getLat()));
	spinboxTwo.value(to_string(config->lowerLeft.getLon()));
	spinboxThree.value(to_string(config->upperRight.getLat()));
	spinboxFour.value(to_string(config->upperRight.getLon()));

	if (config->viewportMode == 0) {
		manualOpt.check(true);
	}
	else if (config->viewportMode == 1) {
		ratioOpt.check(true);
	}
	else {
		autoOpt.check(true);
	}

	spinboxFive.caption(to_string(config->highestLatitude));
	spinboxSix.caption(to_string(config->bottomCenter.getLat()));
	spinboxSeven.caption(to_string(config->bottomCenter.getLon()));
	spinboxEight.caption(to_string(config->radius));

	unsavedChanges = false;
}

bool ConfigViewportGUI::hasUnsavedChanges()
{
	return unsavedChanges;
}

void ConfigViewportGUI::updateAutoCoordinates()
{
	if (config == nullptr)
		return;

	vector<GeographicCoordinate> bounds = guessBounds(activities, *config, spinboxEight.to_double());
	if (bounds.size() == 2) {
		spinboxOne.value(to_string(bounds[0].getLat()));
		spinboxTwo.value(to_string(bounds[0].getLon()));
		spinboxThree.value(to_string(bounds[1].getLat()));
		spinboxFour.value(to_string(bounds[1].getLon()));
	}
}

void ConfigViewportGUI::updateRatioCoordinates()
{
	if (config == nullptr)
		return;

	vector<GeographicCoordinate> bounds = computeBoundingBoxVertical(geoCoord(spinboxSix.to_double(), spinboxSeven.to_double()), *config, spinboxFive.to_double());
	if (bounds.size() == 2) {
		spinboxOne.value(to_string(bounds[0].getLat()));
		spinboxTwo.value(to_string(bounds[0].getLon()));
		spinboxThree.value(to_string(bounds[1].getLat()));
		spinboxFour.value(to_string(bounds[1].getLon()));
	}
}

void ConfigViewportGUI::setMedianStartPoint(GeographicCoordinate median)
{
	labelMedian.caption(median.toString());
}
