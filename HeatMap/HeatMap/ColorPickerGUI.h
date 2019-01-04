#pragma once
#include <nana/gui.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/slider.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/group.hpp>

#include "Color.h"
#include "ColorPreviewGUI.h"

using namespace std;
using namespace nana;

class ColorPickerGUI : public form {
private:

	ColorPreviewGUI colorPreviewGUI{ *this };

	place layout{ *this };

	label labelOne{ *this };
	label labelTwo{ *this };
	label labelThree{ *this };
	label labelFour{ *this };
	label labelFive{ *this };
	label labelSix{ *this };
	label labelSeven{ *this };
	label labelEight{ *this };

	slider sliderOne{ *this };
	slider sliderTwo{ *this };
	slider sliderThree{ *this };
	slider sliderFour{ *this };

	button okButton{ *this };

	Color color;

	void updateDisplay();

public:
	ColorPickerGUI(form &frm);
	Color present(Color init);
};