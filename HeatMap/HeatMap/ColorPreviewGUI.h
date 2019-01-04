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

using namespace std;
using namespace nana;

class ColorPreviewGUI : public panel<false> {
private:

	Color c;

	drawing* d;
	
public:
	ColorPreviewGUI(form &frm);
	~ColorPreviewGUI();
	void setColor(Color init);
};