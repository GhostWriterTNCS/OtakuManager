#pragma once

#include <string>

/**
	A small collection of functions working with PhantomJS.
*/
namespace MyPhantomJS {

/**
	Parse the html by url and return the content as string.
*/
std::string urlToString(std::string url);

} // namespace MyPhantomJS
