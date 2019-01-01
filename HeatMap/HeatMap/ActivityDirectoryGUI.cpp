#include "ActivityDirectoryGUI.h"

ActivityDirectoryGUI::ActivityDirectoryGUI() : form(API::make_center(500, 200), form::appear::decorate<form::appear::minimize>())
{
	caption("Heat Map");
	instructions.caption("Please select folder containing your activities:");
	decompress.caption("Automatically decompress .gz files");
	decompress.check(true);
	pathToDirectory.multi_lines(false);
	browseButton.caption("Browse");
	browseButton.events().click([&]() {
		auto folder = browser.show();
		if (folder) {
			pathToDirectory.caption(folder.value().string());
		}
	});
	loadButton.caption("Load Activities");
	loadButton.events().click([&]() {
		ActivityLoadingGUI activityLoadingGUI(*this);
		fut = async(this->workCallback, pathToDirectory.caption(), decompress.checked());
		activityLoadingGUI.present(&fut, this->currentProgress, this->shouldCancel, this->progressKnown);

		if (!(*shouldCancel)) {
			hide();
			finishedCallback(fut.get());
		}
	});
	layout.div("<><vert weight=80% <><instructions><<weight=75% pathToDirectory> <browseButton>><decompress><><loadButton><>><>");
	layout["instructions"] << instructions;
	layout["pathToDirectory"] << pathToDirectory;
	layout["browseButton"] << browseButton;
	layout["decompress"] << decompress;
	layout["loadButton"] << loadButton;
	layout.collocate();
}

void ActivityDirectoryGUI::present(function<vector<Activity*>(string, bool)> workCallback, function<void(vector<Activity*>)> finishedCallback, atomic<unsigned int>* currentProgress, atomic<bool>* shouldCancel, atomic<bool>* progressKnown)
{
	this->workCallback = workCallback;
	this->finishedCallback = finishedCallback;
	this->currentProgress = currentProgress;
	this->shouldCancel = shouldCancel;
	this->progressKnown = progressKnown;
	show();
}
