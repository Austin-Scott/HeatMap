#pragma once
#include <thread>
#include <atomic>
#include <vector>
#include <future>
#include <mutex>

#include <nana/gui.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/progress.hpp>

#include "Activity.h"
#include "Image.h"

using namespace std;
using namespace nana;

class RenderMapGUI : public form {
private:
	place layout{ *this };
	label message{ *this };
	progress bar{ *this };
	button cancel{ *this };

	timer time;

	future<Image*>* fut;

	bool finished;

	atomic<unsigned int>* currentProgress;
	atomic<bool>* shouldCancel;
	atomic<bool>* progressKnown;
public:
	RenderMapGUI(form &frm);
	void present(future<Image*>* fut, atomic<unsigned int>* currentProgress, atomic<bool>* shouldCancel, atomic<bool>* progressKnown);
};