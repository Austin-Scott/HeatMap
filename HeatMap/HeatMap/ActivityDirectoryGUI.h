#pragma once
#include <functional>
#include <string>
#include <future>
#include <atomic>
#include <mutex>


#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/filebox.hpp>

#include "Activity.h"
#include "ActivityLoadingGUI.h"

using namespace std;
using namespace nana;

class ActivityDirectoryGUI : public form {
private:
	place layout{ *this };
	label instructions{*this};
	textbox pathToDirectory{*this};
	folderbox browser{ *this };
	checkbox decompress{ *this };
	button browseButton{*this};
	button loadButton{*this};

	function<vector<Activity*>(string, bool)> workCallback;
	function<void(vector<Activity*>)> finishedCallback;

	atomic<unsigned int>* currentProgress;
	atomic<bool>* shouldCancel;
	atomic<bool>* progressKnown;
	string* statusString;
	mutex* statusMutex;

	future<vector<Activity*>> fut;
public:
	ActivityDirectoryGUI();
	void present(function<vector<Activity*>(string, bool)> workCallback, function<void(vector<Activity*>)> finishedCallback, atomic<unsigned int>* currentProgress, atomic<bool>* shouldCancel, atomic<bool>* progressKnown, string* statusString, mutex* statusMutex);
};