#include "ActivityLoadingGUI.h"

ActivityLoadingGUI::ActivityLoadingGUI(form &frm) : form(frm)
{
	caption("Heat Map");
	message.caption("Loading activities. This may take a few minutes.");
	cancel.caption("Cancel");
	cancel.events().click([&]() {
		*shouldCancel = true;
	});
	layout.div("<><vert weight=80% <><message><bar><><cancel><>><>");
	layout["message"] << message;
	layout["bar"] << bar;
	layout["cancel"] << cancel;
	layout.collocate();
}

void ActivityLoadingGUI::present(future<vector<Activity*>>* fut, atomic<unsigned int>* currentProgress, atomic<bool>* shouldCancel, atomic<bool>* progressKnown)
{
	this->fut = fut;
	this->currentProgress = currentProgress;
	this->shouldCancel = shouldCancel;
	*(this->shouldCancel) = false;
	this->progressKnown = progressKnown;

	time.interval(100);
	time.elapse([&]() {
		if (*shouldCancel == true) {
			time.stop();
			close();
		}

		future_status status = fut->wait_for(chrono::milliseconds(0));
		if (!(status == future_status::ready)) {
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
			time.stop();
			close();
		}
	});

	time.start();
	modality();
}
