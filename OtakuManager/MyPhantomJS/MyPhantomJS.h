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

/**
	Parse the html by url and return the content as string when the element requested in the query
   is found.
*/
std::string urlToStringJS(std::string url, std::string query);

} // namespace MyPhantomJS
