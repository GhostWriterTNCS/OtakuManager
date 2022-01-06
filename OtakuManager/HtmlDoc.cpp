#include "HtmlDoc.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

// trim from start (in place)
static inline void ltrim(std::string& s) {
	s.erase(s.begin(),
			std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); })
				.base(),
			s.end());
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
	ltrim(s);
	return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
	rtrim(s);
	return s;
}

std::string trim(std::string str) {
	return rtrim_copy(str);
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
