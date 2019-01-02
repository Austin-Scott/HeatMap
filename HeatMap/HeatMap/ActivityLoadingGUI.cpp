#include "ActivityLoadingGUI.h"

ActivityLoadingGUI::ActivityLoadingGUI(form &frm) : form(frm)
{
	caption("Loading Activities...");
	message.caption("This may take a few minutes.");
	cancel.caption("Cancel");
	cancel.events().click([&]() {
		*shouldCancel = true;
	});
	layout.div("<><vert weight=80% <message><><status><bar><><cancel><>><>");
	layout["message"] << message;
	layout["bar"] << bar;
	layout["cancel"] << cancel;
	layout["status"] << statusLabel;
	layout.collocate();
	this->events().unload([&]() {
		if(!finished)
			*this->shouldCancel = true;
		time.stop();
	});
}

void ActivityLoadingGUI::present(future<vector<Activity*>>* fut, atomic<unsigned int>* currentProgress, atomic<bool>* shouldCancel, atomic<bool>* progressKnown, string* statusString, mutex* statusMutex)
{
	finished = false;
	this->fut = fut;
	this->currentProgress = currentProgress;
	this->shouldCancel = shouldCancel;
	*(this->shouldCancel) = false;
	this->progressKnown = progressKnown;
	this->statusString = statusString;
	this->statusMutex = statusMutex;

	time.interval(100);
	time.elapse([&]() {
		if (*shouldCancel == true) {
			close();
		}

		future_status status = fut->wait_for(chrono::milliseconds(0));
		if (!(status == future_status::ready)) {
			statusMutex->lock();
			statusLabel.caption(*statusString);
			statusMutex->unlock();
			if (*this->progressKnown) {
				bar.unknown(false);
				unsigned int value = *this->currentProgress;
				if (value >= 0 && value <= 100) {
					bar.value(value);
				}
			}
			else {
				bar.unknown(true);
				bar.inc();
			}
		}
		else {
			finished = true;
			close();
		}
	});

	time.start();
	modality();
}
