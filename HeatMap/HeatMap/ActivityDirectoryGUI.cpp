#include "ActivityDirectoryGUI.h"

ActivityDirectoryGUI::ActivityDirectoryGUI() : form(API::make_center(500, 200), form::appear::decorate<form::appear::minimize>())
{
	caption("Select Activity Directory");
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
	events().unload([&]() {
		if (!nextPhase) {
			API::exit();
		}
	});
	loadButton.caption("Load Activities");
	loadButton.events().click([&]() {
		ActivityLoadingGUI activityLoadingGUI(*this);
		fut = async(this->workCallback, pathToDirectory.caption(), decompress.checked());
		activityLoadingGUI.present(&fut, this->currentProgress, this->shouldCancel, this->progressKnown, this->statusString, this->statusMutex);

		if (!(*this->shouldCancel)) {
			nextPhase = true;
			vector<Activity*> activities = fut.get();
			
			if (activities.size() == 0) {
				nextPhase = false;
				msgbox box(*this, "Unable to proceed");
				box.icon(msgbox::icon_information) << "No supported activities were found.";
				box.show();
			}
			else {
				bool containsGPSData = false;
				for (Activity* p : activities) {
					if (p->getTrack().size() > 0) {
						containsGPSData = true;
						break;
					}
				}
				if (!containsGPSData) {
					nextPhase = false;
					for (Activity* p : activities) {
						delete p;
					}
					msgbox box(*this, "Unable to proceed");
					box.icon(msgbox::icon_information) << "None of the loaded activities contain GPS data.";
					box.show();
				}
			}

			if (nextPhase) {
				hide();
				finishedCallback(activities, nextForm);
			}
		}
	});
	layout.div("<><vert weight=80% <><instructions><<weight=75% pathToDirectory> <browseButton>><decompress><><loadButton><>><>");
	layout["instructions"] << instructions;
	layout["pathToDirectory"] << pathToDirectory;
	layout["browseButton"] << browseButton;
	layout["decompress"] << decompress;
	layout["loadButton"] << loadButton;
	layout.collocate();

	nextPhase = false;
}

void ActivityDirectoryGUI::present(function<vector<Activity*>(string, bool)> workCallback, function<void(vector<Activity*>, MainGUI*)> finishedCallback, atomic<unsigned int>* currentProgress, atomic<bool>* shouldCancel, atomic<bool>* progressKnown, string* statusString, mutex* statusMutex, MainGUI* nextForm)
{
	this->workCallback = workCallback;
	this->finishedCallback = finishedCallback;
	this->currentProgress = currentProgress;
	this->shouldCancel = shouldCancel;
	this->progressKnown = progressKnown;
	this->statusString = statusString;
	this->statusMutex = statusMutex;
	this->nextForm = nextForm;
	show();
}
