#pragma once
#include <thread>
#include <atomic>
#include <vector>
#include <future>

#include <nana/gui.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/progress.hpp>

#include "Activity.h"

using namespace std;
using namespace nana;

class ActivityLoadingGUI : public form {
private:
	place layout{ *this };
	label message{ *this };
	progress bar{ *this };
	button cancel{ *this };

	timer time;

	future<vector<Activity*>>* fut;

	atomic<unsigned int>* currentProgress;
	atomic<bool>* shouldCancel;
	atomic<bool>* progressKnown;
public:
	ActivityLoadingGUI(form &frm);
	void present(future<vector<Activity*>>* fut, atomic<unsigned int>* currentProgress, atomic<bool>* shouldCancel, atomic<bool>* progressKnown);
};