#include <Awesomium\STLHelpers.h>
#include <Awesomium\WebCore.h>
#include <Awesomium\WebString.h>
#include "MyAwesomium.h"

#include <iostream>
#include <thread>

using namespace Awesomium;

namespace MyAwesomium {

std::string localUrl = "";
std::string result = "";
bool started = false;
bool running = false;

// Awesomium does not support multithread.
void loopFunction() {
	WebCore* web_core = WebCore::Initialize(WebConfig());
	while (started) {
		if (localUrl == "") {
			Sleep(500);
		} else {
			std::string tempUrl = localUrl;
			localUrl = "";
			result = "";

			WebSession* session = web_core->CreateWebSession(WSLit(""), WebPreferences());
			WebView* view = web_core->CreateWebView(800, 600, session);

			// Load a certain URL into our WebView instance.
			view->LoadURL(WebURL(WSLit(tempUrl.c_str())));

			// Wait for our WebView to finish loading.
			while (view->IsLoading()) {
				web_core->Update();
			}

			// Sleep a bit and update once more to give scripts and plugins
			// on the page a chance to finish loading.
			Sleep(300);

			// Wait for websites that reloads after few seconds.
			while (view->title() == WSLit("Just a moment...") && !view->IsLoading()) {
				Sleep(100);
				web_core->Update();
			}

			// Wait for our WebView to finish loading.
			while (view->IsLoading()) {
				web_core->Update();
			}

			if (view->IsCrashed())
				return;

			// Get the page source.
			WebString webString = view->ExecuteJavascriptWithResult(
										  WSLit("document.documentElement.outerHTML"), WSLit(""))
									  .ToString();

			view->Destroy();

			result = "<html>\n" + ToString(webString) + "\n</html>";
			running = false;
		}
	}
	WebCore::Shutdown();
}

void ShutdownWebCore() {
	started = false;
}

std::string urlToString(std::string url) {
	if (!started) {
		std::thread* t = new std::thread(loopFunction);
		started = true;
	}

	std::cout << "[MyAwesomium] Started " << url << std::endl;
	while (running) {
		Sleep(250);
	}

	localUrl = url;
	running = true;

	int i = 0;
	while (running && i < 120) { // wait max 30sec
		Sleep(250);
		i++;
	}
	if (i == 120) {
		std::cout << "[MyAwesomium] Aborted " << url << std::endl;
	} else {
		std::cout << "[MyAwesomium] Finished " << url << std::endl;
	}
	return result;
}

} // namespace MyAwesomium
