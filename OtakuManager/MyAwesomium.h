#pragma once
#include <string>

namespace MyAwesomium {

// Create the WebCore singleton with default configuration.
// void InitializeWebCore();

/**
	Shutdown before exit application.
*/
void ShutdownWebCore();

/**
	Parse the html by url and return the content as string after JavaScript execution.
*/
std::string urlToString(std::string url);

}; // namespace MyAwesomium
