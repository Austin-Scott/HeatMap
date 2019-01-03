#include "MainGUI.h"
#include <nana/gui/state_cursor.hpp>

MainGUI::MainGUI() : form(API::make_center(900, 600), form::appear::decorate<form::appear::minimize>())
{
	caption("Heat Map Generator v1.0");
	layout.div("<><vert weight=95% <weight=2%><<weight=40% configRendererGUI><configViewportGUI>><<filterByDateGUI><filterBySpeedGUI><filterByActivityTypeGUI>><weight=10%><weight=10% renderButton><weight=2%>><>");
	
	renderButton.caption("Render and Save Heat Map");
	layout["renderButton"] << renderButton;
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


	heatMapConfiguration.width = 1920;
	heatMapConfiguration.height = 1080;
	heatMapConfiguration.computeBoundingBox(geoCoord(44.846595, -91.897108), 44.938059);
	heatMapConfiguration.setRenderer(true, Color("#000000FF"), Color("#FF000080"), Color("#FFFFFFFF"));

	filterByActivityTypeGUI.setConfig(&heatMapConfiguration, this);
	filterByDateGUI.setConfig(&heatMapConfiguration, this);
	filterBySpeedGUI.setConfig(&heatMapConfiguration, this);
	configViewportGUI.setConfig(&heatMapConfiguration, this);
	configRendererGUI.setConfig(&heatMapConfiguration, this);
}

void MainGUI::present(function<Image*(HeatMapConfiguration, vector<Activity*>)> renderHeatMap, atomic<unsigned int>* currentProgress, atomic<bool>* shouldCancel, atomic<bool>* progressKnown, vector<Activity*> activities)
{
	this->renderHeatMap = renderHeatMap;
	this->currentProgress = currentProgress;
	this->shouldCancel = shouldCancel;
	this->progressKnown = progressKnown;
	this->activities = activities;

	show();
}
