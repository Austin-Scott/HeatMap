#include "ColorPickerGUI.h"

void ColorPickerGUI::updateDisplay()
{
	this->color.setR(sliderOne.value());
	labelTwo.caption(to_string(sliderOne.value()));
	this->color.setG(sliderTwo.value()); 
	labelFour.caption(to_string(sliderTwo.value()));
	this->color.setB(sliderThree.value()); 
	labelSix.caption(to_string(sliderThree.value()));
	this->color.setA(sliderFour.value()); 
	labelEight.caption(to_string(sliderFour.value()));
	colorPreviewGUI.setColor(color);
}

ColorPickerGUI::ColorPickerGUI(form & frm) : form(frm)
{
	caption("Choose Color");
	layout.div("<><vert weight=95% <weight=3%><<><colorPreviewGUI><>><<labelOne><weight=65% sliderOne><labelTwo>><<labelThree><weight=65% sliderTwo><labelFour>><<labelFive><weight=65% sliderThree><labelSix>><<labelSeven><weight=65% sliderFour><labelEight>><<><okButton><>><weight=3%>><>");
	
	layout["colorPreviewGUI"] << colorPreviewGUI;
	
	labelOne.caption("Red: ");
	labelOne.text_align(align::right);
	layout["labelOne"] << labelOne;
	labelTwo.caption("0");
	layout["labelTwo"] << labelTwo;
	labelThree.caption("Green: ");
	labelThree.text_align(align::right);
	layout["labelThree"] << labelThree;
	labelFour.caption("0");
	layout["labelFour"] << labelFour;
	labelFive.caption("Blue: ");
	labelFive.text_align(align::right);
	layout["labelFive"] << labelFive;
	labelSix.caption("0");
	layout["labelSix"] << labelSix;
	labelSeven.caption("Alpha: ");
	labelSeven.text_align(align::right);
	layout["labelSeven"] << labelSeven;
	labelEight.caption("0");
	layout["labelEight"] << labelEight;

	sliderOne.maximum(255);
	sliderOne.events().value_changed([&]() { updateDisplay(); });
	layout["sliderOne"] << sliderOne;
	sliderTwo.maximum(255);
	sliderTwo.events().value_changed([&]() { updateDisplay(); });
	layout["sliderTwo"] << sliderTwo;
	sliderThree.maximum(255);
	sliderThree.events().value_changed([&]() { updateDisplay(); });
	layout["sliderThree"] << sliderThree;
	sliderFour.maximum(255);
	sliderFour.events().value_changed([&]() { updateDisplay(); });
	layout["sliderFour"] << sliderFour;

	okButton.caption("Select Color");
	okButton.events().click([&]() { close(); });
	layout["okButton"] << okButton;

	layout.collocate();
}

Color ColorPickerGUI::present(Color init)
{
	color = init;
	sliderOne.value(color.getR());
	sliderTwo.value(color.getG());
	sliderThree.value(color.getB());
	sliderFour.value(color.getA());
	updateDisplay();

	modality();
	return color;
}
