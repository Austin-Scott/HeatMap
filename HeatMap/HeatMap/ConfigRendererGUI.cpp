#include "ConfigRendererGUI.h"


void ConfigRendererGUI::setConfig(HeatMapConfiguration * config, form* parentFrm)
{
	this->config = config;
	this->parentFrm = parentFrm;
}

ConfigRendererGUI::ConfigRendererGUI(form &frm) : group(frm)
{
	caption("Configure Renderer");
	layout.div("<><vert weight=95% <><<labelOne><spinboxOne><labelTwo><spinboxTwo>><useAntiAliasing><<weight=50% labelThree><textboxOne><buttonOne>><<weight=50% labelFour><textboxTwo><buttonTwo>><<weight=50% labelFive><textboxThree><buttonThree>><><<buttonSaveChanges><buttonDiscardChanges>><>><>");
	labelOne.caption("Width: ");
	labelOne.text_align(align::right);
	layout["labelOne"] << labelOne;
	labelTwo.caption("Height: ");
	labelTwo.text_align(align::right);
	layout["labelTwo"] << labelTwo;
	labelThree.caption("Background hex color: ");
	layout["labelThree"] << labelThree;
	labelFour.caption("Minimum activity hex color: ");
	layout["labelFour"] << labelFour;
	labelFive.caption("Maximum activity hex color: ");
	layout["labelFive"] << labelFive;
	spinboxOne.range(1, INT_MAX, 1);
	spinboxOne.caption("1920");
	spinboxOne.events().text_changed([&]() { unsavedChanges = true; });
	layout["spinboxOne"] << spinboxOne;
	spinboxTwo.range(1, INT_MAX, 1);
	spinboxTwo.caption("1080");
	spinboxTwo.events().text_changed([&]() { unsavedChanges = true; });
	layout["spinboxTwo"] << spinboxTwo;
	useAntiAliasing.caption("Use Anti-Aliasing (Recommended)");
	useAntiAliasing.check(true);
	useAntiAliasing.events().checked([&]() {unsavedChanges = true; });
	layout["useAntiAliasing"] << useAntiAliasing;
	textboxOne.caption("#000000FF");
	textboxOne.events().text_changed([&]() { unsavedChanges = true; });
	layout["textboxOne"] << textboxOne;
	textboxTwo.caption("#FF000080");
	textboxTwo.events().text_changed([&]() { unsavedChanges = true; });
	layout["textboxTwo"] << textboxTwo;
	textboxThree.caption("#FFFFFFFF");
	textboxThree.events().text_changed([&]() { unsavedChanges = true; });
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
	buttonThree.caption("Choose");
	buttonThree.events().click([&]() {
		ColorPickerGUI cp(*parentFrm);
		textboxThree.caption(cp.present(Color(textboxThree.caption())).toHex());
	});
	layout["buttonThree"] << buttonThree;
	buttonSaveChanges.caption("Apply");
	buttonSaveChanges.events().click([&]() {saveChanges(); });
	layout["buttonSaveChanges"] << buttonSaveChanges;
	buttonDiscardChanges.caption("Discard");
	buttonDiscardChanges.events().click([&]() {discardChanges(); });
	layout["buttonDiscardChanges"] << buttonDiscardChanges;
	layout.collocate();

	nanaTime.interval(100);
	nanaTime.elapse([&]() {
		if (unsavedChanges) {
			if (!buttonSaveChanges.enabled()) {
				buttonSaveChanges.enabled(true);
				buttonDiscardChanges.enabled(true);
				caption("*Configure Renderer");
			}
		}
		else {
			if (buttonSaveChanges.enabled()) {
				buttonSaveChanges.enabled(false);
				buttonDiscardChanges.enabled(false);
				caption("Configure Renderer");
			}
		}
	});
	nanaTime.start();
}

void ConfigRendererGUI::saveChanges()
{
	config->width = spinboxOne.to_int();
	config->height = spinboxTwo.to_int();
	config->useAntiAliasing = useAntiAliasing.checked();
	config->backgroundColor = Color(textboxOne.caption());
	config->minimumActivityColor = Color(textboxTwo.caption());
	config->maximumActivityColor = Color(textboxThree.caption());

	unsavedChanges = false;
}

void ConfigRendererGUI::discardChanges()
{
	spinboxOne.value(to_string(config->width));
	spinboxTwo.value(to_string(config->height));
	useAntiAliasing.check(config->useAntiAliasing);
	textboxOne.caption(config->backgroundColor.toHex());
	textboxTwo.caption(config->minimumActivityColor.toHex());
	textboxThree.caption(config->maximumActivityColor.toHex());

	unsavedChanges = false;
}

bool ConfigRendererGUI::hasUnsavedChanges()
{
	return unsavedChanges;
}
