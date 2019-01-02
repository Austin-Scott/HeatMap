#pragma once
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/date_chooser.hpp>

#include "HeatMapConfiguration.h"
#include "Date.h"

using namespace std;
using namespace nana;

class DatePickerGUI : public form {
private:
	place layout{ *this };
	date_chooser chooser{ *this };
	button pickDate{ *this };
	Date selected;

public:
	Date present();
	DatePickerGUI(form &frm);
};