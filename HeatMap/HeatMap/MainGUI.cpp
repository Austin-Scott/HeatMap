#include "MainGUI.h"
#include <nana/gui/state_cursor.hpp>
#include <nana/system/platform.hpp>

MainGUI::MainGUI() : form(API::make_center(900, 600), form::appear::decorate<form::appear::minimize>())
{
	caption("Heat Map Generator v1.0 BETA");
	layout.div("<><vert weight=95% <weight=2%><configViewportGUI><<filterByDateGUI><filterBySpeedGUI><filterByActivityTypeGUI>><<configRendererGUI><<><vert weight=95% <><weight=40% renderButton><><websiteButton><>><>>><weight=2%>><>");
	
	renderButton.caption("Render and Save Heat Map");
	layout["renderButton"] << renderButton;
	websiteButton.caption("Visit Project Website");
	websiteButton.events().click([&]() {
		nana::system::open_url("https://github.com/Austin-Scott/HeatMap");
	});
	layout["websiteButton"] << websiteButton;
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
		
	});
	nanaTime.start();

	renderButton.events().click([&]() {
		if (configViewportGUI.hasUnsavedChanges() ||
			configRendererGUI.hasUnsavedChanges() ||
			filterByDateGUI.hasUnsavedChanges() ||
			filterBySpeedGUI.hasUnsavedChanges() ||
			filterByActivityTypeGUI.hasUnsavedChanges()) {

			msgbox confirm(*this, "Unapplied changes", msgbox::button_t::yes_no_cancel);
			confirm.icon(msgbox::icon_question) << "You have unapplied changes. Apply all and then proceed?";
			auto answer = confirm.show();
			if (answer == msgbox::pick_t::pick_cancel) {
				return;
			}
			else if (answer == msgbox::pick_t::pick_yes) {
				configViewportGUI.saveChanges();
				configRendererGUI.saveChanges();
				filterByDateGUI.saveChanges();
				filterBySpeedGUI.saveChanges();
				filterByActivityTypeGUI.saveChanges();
			}
		}

		RenderMapGUI renderMapGUI(*this);
		fut = async(renderHeatMap, heatMapConfiguration, activities);
		renderMapGUI.present(&fut, currentProgress, shouldCancel, progressKnown);
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
						msgbox dialog(*this, "Heat Map Saved");
						dialog.icon(msgbox::icon_information) << "Your Heat Map has been successfully saved to: \"" << saveFileName << "\"";
						dialog.show();
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

void MainGUI::present(function<Image*(HeatMapConfiguration, vector<Activity*>)> renderHeatMap, atomic<unsigned int>* currentProgress, atomic<bool>* shouldCancel, atomic<bool>* progressKnown, vector<Activity*> activities)
{
	this->renderHeatMap = renderHeatMap;
	this->currentProgress = currentProgress;
	this->shouldCancel = shouldCancel;
	this->progressKnown = progressKnown;
	this->activities = activities;

	filterByActivityTypeGUI.setConfig(&heatMapConfiguration, this);
	filterByDateGUI.setConfig(&heatMapConfiguration, this);
	filterBySpeedGUI.setConfig(&heatMapConfiguration, this);
	configViewportGUI.setConfig(&heatMapConfiguration, activities, this);
	configRendererGUI.setConfig(&heatMapConfiguration, this);

	heatMapConfiguration = HeatMapConfiguration(1920, 1080);
	heatMapConfiguration.setRenderer(true, Color("#000000FF"), Color("#FF000080"), Color("#FFFFFFFF"));
	heatMapConfiguration.includeUnknownDates = true;
	heatMapConfiguration.includeUnknownSpeeds = true;

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


	show();
}
