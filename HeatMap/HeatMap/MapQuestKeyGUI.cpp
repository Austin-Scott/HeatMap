#include "MapQuestKeyGUI.h"

bool MapQuestKeyGUI::present()
{
	modality();
	return result;
}

MapQuestKeyGUI::MapQuestKeyGUI(form & frm) : form(frm)
{
	caption("Map Quest API");
	layout.div("<><vert weight=95% <weight=50% instructionsLabel><<inputLabel><weight=70% keyBox>><weight=5%><<><saveButton><><cancelButton><>><weight=5%>><>");

	instructionsLabel.caption("1) Please create a free <bold color=blue url=\"https://developer.mapquest.com/plan_purchase/steps/business_edition/business_edition_free/register\">Map Quest Account</>.\n2) <bold color=blue url=\"https://developer.mapquest.com/user/me/apps\">Add an application</> called \"Heat Map\".\n3) Paste its \"Consumer Key\" into the box below.");
	instructionsLabel.format(true);
	layout["instructionsLabel"] << instructionsLabel;
	inputLabel.caption("<bold>Consumer Key: </>");
	inputLabel.format(true);
	inputLabel.text_align(align::right, align_v::center);
	layout["inputLabel"] << inputLabel;
	keyBox.multi_lines(false);
	layout["keyBox"] << keyBox;
	saveButton.caption("Save");
	saveButton.events().click([&]() {
		if (keyBox.caption().length() == 32) {
			keyBox.store("key.txt");
			result = true;
			close();
		}
		else {
			msgbox notice(*this, "Invalid Key Length");
			notice.icon(msgbox::icon_error) << "Your key should be exactly 32 characters long. Please try again.";
			notice.show();
		}
	});
	layout["saveButton"] << saveButton;
	cancelButton.caption("Cancel");
	cancelButton.events().click([&]() {
		close();
	});
	layout["cancelButton"] << cancelButton;

	layout.collocate();

	result = false;
}
