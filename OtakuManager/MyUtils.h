#pragma once
#include <QString>

namespace MyUtils {

/**
	Get a substring using starting and ending delimiters.
	Leave start or end as empty strings to get from the begging of the string or to the end of the
	string.
	Example: substring("Hi, my name is Tom.", "", "is") -> "Hi, my name ".
*/
QString substring(QString str, QString start, QString end = "");

/**
	The same as substring(QString str, QString start, QString end), but start search from the end of
	the string.
*/
QString substringFromEnd(QString str, QString start, QString end = "");

/**
	Replace a substring using starting and ending delimiters.
	Example: advancedReplace("Hi, my name is Tom.", "my", "is", "I'm") -> "Hi, I'm Tom".
*/
QString advancedReplace(QString str, QString startReplace, QString endReplace, QString newText);

/**
	Parse the html by url and return the content as QString.
*/
QString urlToQString(QString url);
/**
	Parse the html by url and return the content as QString after JavaScript exectution.
*/
QString urlToQStringJS(QString url);

/**
	Parse the html by url and create a file from the page content.
	Returns true if success, otherwise false.
*/
bool urlToFile(QString url, QString filename);

/**
	Get the redirected url for an url.
*/
QString redirectedUrlQt(QString url);
/**
	Find and replace html codes in string.
*/
QString decodeHtml(QString html);

/**
	Remove odd characters from a QString.
	Only -, _, &, [, ], spaces and new lines will by mantained.
*/
QString simplify(QString s);
/**
	The same as simplify(QString s), but remove also new lines.
*/
QString simplifyOneLine(QString s);

/**
	Make uniform episodes names.
*/
QString simplifyEp(QString s);

} // namespace MyUtils
