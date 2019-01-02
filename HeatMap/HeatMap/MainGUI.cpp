#include "MainGUI.h"
#include <nana/gui/state_cursor.hpp>

MainGUI::MainGUI() : form(API::make_center(300, 450), form::appear::decorate<form::appear::minimize>())
{
	caption("Heat Map Generator v1.0");
	layout.div("<><vert weight=80% <><configViewport><configFilters><configRenderer><><renderButton><>><>");
	configViewport.caption("Configure Viewport");
	configFilters.caption("Configure Activity Filters");
	configRenderer.caption("Configure Renderer");
	renderButton.caption("Render and Save Heat Map");
	layout["configViewport"] << configViewport;
	layout["configFilters"] << configFilters;
	layout["configRenderer"] << configRenderer;
	layout["renderButton"] << renderButton;
	layout.collocate();

	events().unload([&](const arg_unload &arg) {
		msgbox prompt(*this, "Exit application", msgbox::yes_no);
		prompt.icon(msgbox::icon_question) << "Are you sure you want to exit?";
		if (prompt.show() == msgbox::pick_yes) {
			time.stop();
			configViewportGUI.hide();
			configFiltersGUI.hide();
			configRendererGUI.hide();
			for (Activity* p : activities) {
				delete p;
			}
			API::exit();
		}
		else {
			arg.cancel = true;
		}
	});

	time.interval(100);
	time.elapse([&]() {
		
	});

	configViewport.events().click([&]() {
		configViewportGUI.show();
	});

	configFilters.events().click([&]() {
		configFiltersGUI.show();
	});

	configRenderer.events().click([&]() {
		configRendererGUI.show();
	});

	renderButton.events().click([&]() {
		setSubWindowInteractive(false);
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
		configViewportGUI.enabled(true);
		setSubWindowInteractive(true);
	});


	heatMapConfiguration.width = 1920;
	heatMapConfiguration.height = 1080;
	heatMapConfiguration.computeBoundingBox(geoCoord(44.846595, -91.897108), 44.938059);
	heatMapConfiguration.setRenderer(true, Color("#000000FF"), Color("#FF000080"), Color("#FFFFFFFF"));
}

void MainGUI::setSubWindowInteractive(bool value)
{
	configViewportGUI.setSubWindowInteractive(value);
	configFiltersGUI.setSubWindowInteractive(value);
	configRendererGUI.setSubWindowInteractive(value);
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
