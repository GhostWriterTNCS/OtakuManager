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
bool running = false;
bool shutdown = false;

void executeUrlToString() {}

// Awesomium does not support multithread.
void loopFunction() {
	WebCore* web_core = WebCore::Initialize(WebConfig());
	while (!shutdown) {
		if (localUrl == "") {
			Sleep(500);
		} else {
			std::string tempUrl = localUrl;
			localUrl = "";

			// Create a new WebView instance with a certain width and height.
			WebView* view = web_core->CreateWebView(800, 600);

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
			WebString webString =
				view->ExecuteJavascriptWithResult(
						WSLit("document.getElementsByTagName('html')[0].innerHTML"), WSLit(""))
					.ToString();

			view->Destroy();

			result = "<html>\n" + ToString(webString) + "\n</html>";
			running = false;
		}
	}
	WebCore::Shutdown();
}

void InitializeWebCore() {
	std::thread* t = new std::thread(loopFunction);
}
void ShutdownWebCore() {
	shutdown = true;
}

std::string urlToString(std::string url) {
	std::cout << "[Awesomium] Started: " << url << std::endl;
	while (running) {
		Sleep(250);
	}

	localUrl = url;
	running = true;

	while (running) {
		Sleep(250);
	}
	std::cout << "[Awesomium] Finished: " << url << std::endl;
	return result;
}

} // namespace MyAwesomium
