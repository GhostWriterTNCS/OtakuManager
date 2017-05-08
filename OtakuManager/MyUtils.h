#pragma once
#include <QString>

namespace MyUtils {

QString substring(QString str, QString start, QString end = "");
QString substringFromEnd(QString str, QString start, QString end = "");
QString advancedReplace(QString str, QString startReplace, QString endReplace, QString newText);

QString urlToQString(QString url);
QString urlToQStringJS(QString url);
bool urlToFile(QString url, QString filename);
QString redirectedUrlQt(QString url);
QString decodeHtml(QString html);

QString simplify(QString s);
QString simplifyOneLine(QString s);
QString simplifyEp(QString s);

} // namespace MyUtils
