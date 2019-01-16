#include "ConfigRendererGUI.h"

vector<string> mapStyles = { "Dark", "Light", "Map", "Satellite", "Hybrid" };
vector<string> mapValues = { "dark", "light", "map", "sat", "hyb" };

void ConfigRendererGUI::setConfig(HeatMapConfiguration * config, form* parentFrm)
{
	this->config = config;
	this->parentFrm = parentFrm;
}

ConfigRendererGUI::ConfigRendererGUI(form &frm) : group(frm)
{
	caption("Configure Renderer");
	layout.div("<><vert weight=45% <><<labelOne><spinboxOne><labelTwo><spinboxTwo>><<weight=50% labelThree><textboxOne><buttonOne>><<weight=50% labelFour><textboxTwo><buttonTwo>><<weight=50% labelLow><textboxLow><buttonLow>><<weight=50% labelHigh><textboxHigh><buttonHigh>><<weight=50% labelFive><textboxThree><buttonThree>><>><><vert weight=45% <><downloadMap><<weight=60% labelMapStyle><mapStyle>><labelHeatTransparency><<labelTransparent><weight=49% transparencySlider><labelOpaque>><><><>><>");
	labelOne.caption("Width: ");
	labelOne.text_align(align::right, align_v::center);
	layout["labelOne"] << labelOne;
	labelTwo.caption("Height: ");
	labelTwo.text_align(align::right, align_v::center);
	layout["labelTwo"] << labelTwo;
	labelThree.caption("Background color: ");
	labelThree.text_align(align::right, align_v::center);
	layout["labelThree"] << labelThree;
	labelFour.caption("Minimum heat color: ");
	labelFour.text_align(align::right, align_v::center);
	layout["labelFour"] << labelFour;
	labelLow.caption("Low heat color: ");
	labelLow.text_align(align::right, align_v::center);
	layout["labelLow"] << labelLow;
	labelHigh.caption("High heat color: ");
	labelHigh.text_align(align::right, align_v::center);
	layout["labelHigh"] << labelHigh;
	labelFive.caption("Maximum heat color: ");
	labelFive.text_align(align::right, align_v::center);
	layout["labelFive"] << labelFive;

	labelMapStyle.caption("Please select map style: ");
	labelMapStyle.text_align(align::right, align_v::center);
	layout["labelMapStyle"] << labelMapStyle;
	labelHeatTransparency.caption("Set heat layer transparency: ");
	labelHeatTransparency.text_align(align::center, align_v::center);
	layout["labelHeatTransparency"] << labelHeatTransparency;
	labelTransparent.caption("Transparent");
	labelTransparent.text_align(align::center, align_v::center);
	layout["labelTransparent"] << labelTransparent;
	labelOpaque.caption("Opaque");
	labelOpaque.text_align(align::left, align_v::center);
	layout["labelOpaque"] << labelOpaque;

	transparencySlider.maximum(255);
	transparencySlider.value(255);
	layout["transparencySlider"] << transparencySlider;

	for (string s : mapStyles) {
		mapStyle.push_back(s);
	}
	mapStyle.option(0);
	layout["mapStyle"] << mapStyle;

	downloadMap.caption("Download map from Map Quest");
	downloadMap.check(true);
	layout["downloadMap"] << downloadMap;

	spinboxOne.range(1, INT_MAX, 1);
	spinboxOne.caption("1920");
	spinboxOne.events().text_changed([&]() { unsavedChanges = true; });
	layout["spinboxOne"] << spinboxOne;
	spinboxTwo.range(1, INT_MAX, 1);
	spinboxTwo.caption("1080");
	spinboxTwo.events().text_changed([&]() { unsavedChanges = true; });
	layout["spinboxTwo"] << spinboxTwo;
	textboxOne.caption("#000000FF");
	textboxOne.events().text_changed([&]() { unsavedChanges = true; updateHighlight(); });
	textboxOne.multi_lines(false);
	layout["textboxOne"] << textboxOne;
	textboxTwo.caption("#FF000080");
	textboxTwo.events().text_changed([&]() { unsavedChanges = true; updateHighlight(); });
	textboxTwo.multi_lines(false);
	layout["textboxTwo"] << textboxTwo;
	textboxLow.caption("#FF000080");
	textboxLow.events().text_changed([&]() { unsavedChanges = true; updateHighlight(); });
	textboxLow.multi_lines(false);
	layout["textboxLow"] << textboxLow;
	textboxHigh.caption("#FF000080");
	textboxHigh.events().text_changed([&]() { unsavedChanges = true; updateHighlight(); });
	textboxHigh.multi_lines(false);
	layout["textboxHigh"] << textboxHigh;
	textboxThree.caption("#FFFFFFFF");
	textboxThree.events().text_changed([&]() { unsavedChanges = true; updateHighlight(); });
	textboxThree.multi_lines(false);
	layout["textboxThree"] << textboxThree;
	buttonOne.caption("Choose");
	buttonOne.events().click([&]() {
		ColorPickerGUI cp(*parentFrm);
		textboxOne.caption(cp.present(Color(textboxOne.caption())).toHex());
	});
	layout["buttonOne"] << buttonOne;
	buttonTwo.caption("Choose");
	buttonTwo.events().click([&]() {
		ColorPickerGUI cp(*parentFrm);
		textboxTwo.caption(cp.present(Color(textboxTwo.caption())).toHex());
	});
	layout["buttonTwo"] << buttonTwo;
	buttonLow.caption("Choose");
	buttonLow.events().click([&]() {
		ColorPickerGUI cp(*parentFrm);
		textboxLow.caption(cp.present(Color(textboxLow.caption())).toHex());
	});
	layout["buttonLow"] << buttonLow;
	buttonHigh.caption("Choose");
	buttonHigh.events().click([&]() {
		ColorPickerGUI cp(*parentFrm);
		textboxHigh.caption(cp.present(Color(textboxHigh.caption())).toHex());
	});
	layout["buttonHigh"] << buttonHigh;
	buttonThree.caption("Choose");
	buttonThree.events().click([&]() {
		ColorPickerGUI cp(*parentFrm);
		textboxThree.caption(cp.present(Color(textboxThree.caption())).toHex());
	});
	layout["buttonThree"] << buttonThree;
	layout.collocate();

	nanaTime.interval(100);
	nanaTime.elapse([&]() {
		
	});
	nanaTime.start();
}

void setHighlight(textbox &box) {
	Color c = Color(box.caption());
	box.erase_highlight("main");
	Color background = c;
	Color inverse = c.getInverse();
	unsigned char gray = (inverse.getR() + inverse.getG() + inverse.getG()) / 3;
	Color foreground = Color(gray, gray, gray, 255);
	box.set_highlight("main", color(foreground.getR(), foreground.getG(), foreground.getB()), color(background.getR(), background.getG(), background.getB()));
	box.set_keywords("main", false, true, { box.caption() });
}

void ConfigRendererGUI::updateHighlight()
{
	setHighlight(textboxOne);
	setHighlight(textboxTwo);
	setHighlight(textboxLow);
	setHighlight(textboxHigh);
	setHighlight(textboxThree);
}

void ConfigRendererGUI::saveChanges()
{
	config->width = spinboxOne.to_int();
	config->height = spinboxTwo.to_int();

	config->backgroundColor = Color(textboxOne.caption());
	

	config->minimumActivityColor = Color(textboxTwo.caption());
	

	config->activity33Color = Color(textboxLow.caption());
	

	config->activity66Color = Color(textboxHigh.caption());
	

	config->maximumActivityColor = Color(textboxThree.caption());
	
	config->downloadMap = downloadMap.checked();
	
	string selected = mapStyle.caption();
	for (int i = 0; i < mapStyles.size(); i++) {
		if (mapStyles[i] == selected) {
			config->mapType = mapValues[i];
			break;
		}
	}

	config->heatLayerTransparency = transparencySlider.value();

	unsavedChanges = false;
}

void ConfigRendererGUI::discardChanges()
{
	spinboxOne.value(to_string(config->width));
	spinboxTwo.value(to_string(config->height));
	textboxOne.caption(config->backgroundColor.toHex());
	textboxTwo.caption(config->minimumActivityColor.toHex());
	textboxLow.caption(config->activity33Color.toHex());
	textboxHigh.caption(config->activity66Color.toHex());
	textboxThree.caption(config->maximumActivityColor.toHex());

	for (int i = 0; i < mapStyles.size(); i++) {
		if (mapValues[i] == config->mapType) {
			mapStyle.option(i);
			break;
		}
	}
	transparencySlider.value(config->heatLayerTransparency);

	updateHighlight();

	unsavedChanges = false;
}

bool ConfigRendererGUI::hasUnsavedChanges()
{
	return unsavedChanges;
}
