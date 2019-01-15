#pragma once
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/date_chooser.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/msgbox.hpp>

#include "HeatMapConfiguration.h"

using namespace std;
using namespace nana;

class MapQuestKeyGUI : public form {
private:
	place layout{ *this };
	
	label instructionsLabel{ *this };

	label inputLabel{ *this };

	textbox keyBox{ *this };

	button saveButton{ *this };
	button cancelButton{ *this };

	bool result;

public:
	bool present();
	MapQuestKeyGUI(form &frm);
};