#include "RenderMapGUI.h"

RenderMapGUI::RenderMapGUI(form & frm) : form(frm)
{
	caption("Rendering map...");
	message.caption("This may take a few seconds.");
	cancel.caption("Cancel");
	cancel.events().click([&]() {
		*shouldCancel = true;
	});
	layout.div("<><vert weight=80% <message><><statusLabel><bar><><cancel><>><>");
	layout["statusLabel"] << statusLabel;
	layout["message"] << message;
	layout["bar"] << bar;
	layout["cancel"] << cancel;
	layout.collocate();
	this->events().unload([&]() {
		if (!finished)
			*this->shouldCancel = true;
		time.stop();
	});
}

void RenderMapGUI::present(future<Image*>* fut, atomic<unsigned int>* currentProgress, atomic<bool>* shouldCancel, atomic<bool>* progressKnown, mutex* statusMutex, string* statusString)
{
	finished = false;
	this->fut = fut;
	this->currentProgress = currentProgress;
	this->shouldCancel = shouldCancel;
	*(this->shouldCancel) = false;
	this->progressKnown = progressKnown;
	this->statusMutex = statusMutex;
	this->statusString = statusString;

	time.interval(100);
	time.elapse([&]() {
		statusMutex->lock();
		statusLabel.caption(*statusString);
		statusMutex->unlock();

		if (*shouldCancel == true) {
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
			finished = true;
			close();
		}
	});

	time.start();
	modality();
}
