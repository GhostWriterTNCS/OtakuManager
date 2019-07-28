#pragma once

#include <string>
#include <vector>

#include <libxml/HTMLparser.h>

class HtmlDoc {
public:
	htmlDocPtr doc;

public:
	HtmlDoc(std::string content, std::string encoding = "utf-8");
	~HtmlDoc();

	std::string GetString(std::string xpath, bool recursive = true, bool trimmed = true);
	std::vector<std::string> GetAllStrings(std::string xpath, bool recursive = true,
										   bool trimmed = true);
};
