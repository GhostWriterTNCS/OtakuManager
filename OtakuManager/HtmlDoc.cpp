#include "HtmlDoc.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

std::string ltrim(std::string str) {
	auto it2 = std::find_if(str.begin(), str.end(), [](char ch) { return !std::isspace(ch); });
	str.erase(str.begin(), it2);
	return str;
}
std::string rtrim(std::string str) {
	auto it1 = std::find_if(str.rbegin(), str.rend(), [](char ch) { return !std::isspace(ch); });
	str.erase(it1.base(), str.end());
	return str;
}
std::string trim(std::string str) {
	return ltrim(rtrim(str));
}

HtmlDoc::HtmlDoc(std::string content, std::string encoding) {
	doc = htmlReadDoc((xmlChar*)content.c_str(), NULL, encoding.c_str(),
					  HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING |
						  HTML_PARSE_NONET);
}

HtmlDoc::~HtmlDoc() {
	xmlFreeDoc(doc);
}

std::string HtmlDoc::GetString(std::string xpath_string, bool recursive, bool trimmed) {
	xmlChar* xpath = (xmlChar*)xpath_string.c_str();
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	context = xmlXPathNewContext(doc);
	result = xmlXPathEvalExpression(xpath, context);
	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		std::cerr << "Empty node." << std::endl;
		return "";
	}
	xmlXPathFreeContext(context);

	std::string s = "";
	xmlNodePtr node = result->nodesetval->nodeTab[0]->xmlChildrenNode;
	do {
		s += (char*)xmlNodeGetContent(node);
	} while (recursive && (node = node->next));
	if (trimmed) {
		s = trim(s);
	}
	return s;
}

std::vector<std::string> HtmlDoc::GetAllStrings(std::string xpath_string, bool recursive,
												bool trimmed) {
	xmlChar* xpath = (xmlChar*)xpath_string.c_str();
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	std::vector<std::string> list = std::vector<std::string>();

	context = xmlXPathNewContext(doc);
	result = xmlXPathEvalExpression(xpath, context);
	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		return list;
	}
	xmlXPathFreeContext(context);

	for (int i = 0; i < result->nodesetval->nodeNr; i++) {
		std::string s = "";
		xmlNodePtr node = result->nodesetval->nodeTab[i]->xmlChildrenNode;
		do {
			s += (char*)xmlNodeGetContent(node);
		} while (recursive && (node = node->next));
		if (trimmed) {
			s = trim(s);
		}
		list.push_back(s);
	}

	return list;
}
