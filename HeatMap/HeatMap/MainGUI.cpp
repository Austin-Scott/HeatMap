#include "MainGUI.h"

MainGUI::MainGUI()
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
		
	});
}

void MainGUI::present(function<Image*(HeatMapConfiguration, vector<Activity*>)> renderHeatMap, vector<Activity*> activities)
{
}
