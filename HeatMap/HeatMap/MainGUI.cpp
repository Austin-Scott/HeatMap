#include "MainGUI.h"
#include <nana/gui/state_cursor.hpp>
#include <nana/system/platform.hpp>

bool overlaps(GeographicCoordinate lla, GeographicCoordinate ura, GeographicCoordinate llb, GeographicCoordinate urb) {
	if (ura.getLon() < llb.getLon() || urb.getLon() < lla.getLon())
		return false;

	if (ura.getLat() < llb.getLat() || urb.getLat() < lla.getLat())
		return false;

	return true;
}

//return true if b is fully contained within a
bool fullyContains(GeographicCoordinate lla, GeographicCoordinate ura, GeographicCoordinate llb, GeographicCoordinate urb) {
	if (llb.getLon() < lla.getLon() || urb.getLon() > ura.getLon() || llb.getLat() < lla.getLat() || urb.getLat() > ura.getLat())
		return false;

	return true;
}

string getFormatedNumber(int n) {
	if (n >= 10) {
		return "<bold color=green>" + to_string(n) + "</>";
	}
	else if (n > 0) {
		return "<bold color=0x808000>" + to_string(n) + "</>";
	}

	return "<bold color=red>" + to_string(n) + "</>";
}

void MainGUI::setStatusLabel(int activitiesLoaded, int activitiesPastFilter, int activitiesOnScreen, int activitiesFullyOnScreen)
{
	string caption = "<bold color=0x434a4d>Valid Activities Loaded: </>" + getFormatedNumber(activitiesLoaded) + "\n<bold color=0x434a4d>Activities not filtered: </>" + getFormatedNumber(activitiesPastFilter) + "\n<bold color=0x434a4d>Fully/Partially visible: </>" + getFormatedNumber(activitiesFullyOnScreen) + "<bold color=0x434a4d>/</>" + getFormatedNumber(activitiesOnScreen);
	statusLabel.caption(caption);
	statusLabel.format(true);
}

MainGUI::MainGUI() : form(API::make_center(900, 600), form::appear::decorate<form::appear::minimize>())
{
	caption("Heat Map Generator v1.1");
	layout.div("<><vert weight=95% <weight=2%><configViewportGUI><<filterByDateGUI><filterBySpeedGUI><filterByActivityTypeGUI>><weight=45% <weight=65% configRendererGUI><<><vert weight=95% <><weight=40% <<><weight=75% statusLabel><>>><saveLoadLabel><<><saveButton><weight=10%><loadButton><>><><weight=25% <<><weight=75% renderButton><>>><websiteLabel>><>>><weight=2%>><>");
	
	//statusLabel.caption("<bold color=0x434a4d>Valid Activities Loaded: </><bold color=green>290</>\n<bold color=0x434a4d>Activities not filtered: </><bold color=green>130</>\n<bold color=0x434a4d>Activities visible in viewport: </><bold color=green>75</>");
	//statusLabel.format(true);
	layout["statusLabel"] << statusLabel;

	renderButton.caption("Render Heat Map");
	layout["renderButton"] << renderButton;
	saveLoadLabel.caption("Save/Load Current Configuration:");
	saveLoadLabel.text_align(align::center, align_v::bottom);
	layout["saveLoadLabel"] << saveLoadLabel;
	saveButton.caption("Save");
	layout["saveButton"] << saveButton;
	loadButton.caption("Load");
	layout["loadButton"] << loadButton;
	websiteLabel.caption("<color=blue url=\"https://github.com/Austin-Scott/HeatMap\">https://github.com/Austin-Scott/HeatMap</>");
	websiteLabel.format(true);
	websiteLabel.text_align(align::center, align_v::bottom);
	layout["websiteLabel"] << websiteLabel;
	layout["configViewportGUI"] << configViewportGUI;
	layout["configRendererGUI"] << configRendererGUI;
	layout["filterByDateGUI"] << filterByDateGUI;
	layout["filterBySpeedGUI"] << filterBySpeedGUI;
	layout["filterByActivityTypeGUI"] << filterByActivityTypeGUI;
	layout.collocate();

	events().unload([&](const arg_unload &arg) {
		msgbox prompt(*this, "Exit application", msgbox::yes_no);
		prompt.icon(msgbox::icon_question) << "Are you sure you want to exit?";
		if (prompt.show() == msgbox::pick_yes) {
			nanaTime.stop();
			for (Activity* p : activities) {
				delete p;
			}
			API::exit();
		}
		else {
			arg.cancel = true;
		}
	});

	nanaTime.interval(100);
	nanaTime.elapse([&]() {


		configViewportGUI.saveChanges();
		configRendererGUI.saveChanges();
		filterByDateGUI.saveChanges();
		filterBySpeedGUI.saveChanges();
		filterByActivityTypeGUI.saveChanges();

		if (!(previousConfiguration == heatMapConfiguration)) {
			//Configuration has changed since last tick, update display
			int width = heatMapConfiguration.width; 
			int height = heatMapConfiguration.height; 
			GeographicCoordinate lowerLeft = heatMapConfiguration.lowerLeft;
			GeographicCoordinate upperRight = heatMapConfiguration.upperRight;

			if (heatMapConfiguration.downloadMap) {
				width= min(max(width, 170), 1920); //min: 170, max: 1920
				height= min(max(height, 30), 1920); //min: 30, max: 1920

				MapQuestConfig mapConfig = getMapConfig(lowerLeft, upperRight, width, height);
				lowerLeft = mapConfig.lowerLeft;
				upperRight = mapConfig.upperRight;
			}

			
			GeographicCoordinate medianStartPoint;

			int validActivities = 0;
			int numberOfActivitiesOnViewport = 0;
			int numberOfActivitiesFullyOnViewport = 0;

			vector<Activity*> filteredActivities;
			vector<double> startLats;
			vector<double> startLons;
			for (Activity* a : activities) {
				if (a->getTrack().size() > 0) {
					validActivities++;

					if (includeActivity(*a, heatMapConfiguration)) {

						if (overlaps(lowerLeft, upperRight, a->lowerLeft(), a->upperRight())) {
							numberOfActivitiesOnViewport++;
							if (fullyContains(lowerLeft, upperRight, a->lowerLeft(), a->upperRight())) {
								numberOfActivitiesFullyOnViewport++;
							}
						}

						startLats.push_back(a->getTrack()[0].getLat());
						startLons.push_back(a->getTrack()[0].getLon());
						filteredActivities.push_back(a);

					}
				}
			}

			if (filteredActivities.size() > 0) {

				sort(startLats.begin(), startLats.end());
				sort(startLons.begin(), startLons.end());

				medianStartPoint = geoCoord(startLats[startLats.size() / 2], startLons[startLons.size() / 2]);
			}
			configViewportGUI.setMedianStartPoint(medianStartPoint);

			setStatusLabel(validActivities, filteredActivities.size(), numberOfActivitiesOnViewport, numberOfActivitiesFullyOnViewport);



			previousConfiguration = heatMapConfiguration;
		}


	});

	renderButton.events().click([&]() {
		
		ifstream keyFile("key.txt");
		string key = "";
		if (keyFile) {
			getline(keyFile, key);
			keyFile.close();
		}
		if (key.length() != 32) {
			MapQuestKeyGUI keyGUI(*this);
			if (!keyGUI.present()) {
				return;
			}
		}

		RenderMapGUI renderMapGUI(*this);
		fut = async(renderHeatMap, heatMapConfiguration, activities);
		renderMapGUI.present(&fut, currentProgress, shouldCancel, progressKnown, statusMutex, statusString);
		if (!(*this->shouldCancel)) {
			Image* image = fut.get();
			if (image != nullptr) {
				filebox saveDialog(*this, false);
				saveDialog.add_filter("Image File", "*.png");
				if (saveDialog()) {
					state_cursor* cursor = new state_cursor(*this, cursor::wait);
					string saveFileName = saveDialog.file();
					string error = image->saveImage(saveFileName);
					delete cursor;
					if (error == "") {
						msgbox dialog(*this, "Heat Map Saved", msgbox::yes_no);
						dialog.icon(msgbox::icon_information) << "Your Heat Map has been successfully saved.\n\nWould you like to view it now?";
						if (dialog.show() == msgbox::pick_yes) {
							wstring wstr = wstring(saveFileName.begin(), saveFileName.end());
							ShellExecute(0, 0, wstr.c_str(), 0, 0, SW_SHOW);
						}
					}
					else {
						msgbox dialog(*this, "Heat Map Saving Failed");
						dialog.icon(msgbox::icon_error) << error;
						dialog.show();
					}
				}
				
				delete image;
			}
		}
	});
}

void MainGUI::present(function<Image*(HeatMapConfiguration, vector<Activity*>)> renderHeatMap, atomic<unsigned int>* currentProgress, atomic<bool>* shouldCancel, atomic<bool>* progressKnown, mutex* statusMutex, string* statusString, vector<Activity*> activities)
{
	this->renderHeatMap = renderHeatMap;
	this->currentProgress = currentProgress;
	this->shouldCancel = shouldCancel;
	this->progressKnown = progressKnown;
	this->statusMutex = statusMutex;
	this->statusString = statusString;
	this->activities = activities;

	filterByActivityTypeGUI.setConfig(&heatMapConfiguration, this);
	filterByDateGUI.setConfig(&heatMapConfiguration, this);
	filterBySpeedGUI.setConfig(&heatMapConfiguration, this);
	configViewportGUI.setConfig(&heatMapConfiguration, activities, this);
	configRendererGUI.setConfig(&heatMapConfiguration, this);

	heatMapConfiguration = HeatMapConfiguration(1920, 1080);
	heatMapConfiguration.setRenderer(Color("#000000FF"), Color("#33003380"), Color("#e60000a0"), Color("#ff944dF0"), Color("#FFFFFFFF"));
	heatMapConfiguration.includeUnknownDates = true;
	heatMapConfiguration.includeUnknownSpeeds = true;
	heatMapConfiguration.downloadMap = true;
	heatMapConfiguration.heatLayerTransparency = 255;
	heatMapConfiguration.mapType = "dark";

	vector<GeographicCoordinate> bounds = guessBounds(activities, heatMapConfiguration, 10.0);
	if (bounds.size() == 2) {
		heatMapConfiguration.lowerLeft = bounds[0];
		heatMapConfiguration.upperRight = bounds[1];
	}

	configViewportGUI.discardChanges();
	configRendererGUI.discardChanges();
	filterByDateGUI.discardChanges();
	filterBySpeedGUI.discardChanges();
	filterByActivityTypeGUI.discardChanges();

	previousConfiguration = heatMapConfiguration;

	nanaTime.start();

	show();
}
