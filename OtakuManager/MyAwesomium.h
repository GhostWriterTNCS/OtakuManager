#pragma once
#include <string>

namespace MyAwesomium {

// Create the WebCore singleton with default configuration.
void InitializeWebCore();
// Shutdown before exit application.
void ShutdownWebCore();

std::string urlToString(std::string url);

}; // namespace MyAwesomium
