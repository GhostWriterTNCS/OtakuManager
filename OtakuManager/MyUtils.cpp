#include "MyCurl.h"
#include "MyPhantomJS\MyPhantomJS.h"
#include "OMA.h"

namespace MyUtils {

QString substring(QString str, QString start, QString end) {
	if (!start.isEmpty() && str.contains(start))
		str = str.mid(str.indexOf(start) + start.length(),
					  str.length() - str.indexOf(start) - start.length());
	if (!end.isEmpty() && str.contains(end))
		str = str.mid(0, str.indexOf(end));
	return str;
}

QString substringFromEnd(QString str, QString start, QString end) {
	if (!start.isEmpty() && str.contains(start))
		str = str.mid(str.lastIndexOf(start) + start.length(),
					  str.length() - str.lastIndexOf(start) - start.length());
	if (!end.isEmpty() && str.contains(end))
		str = str.mid(0, str.lastIndexOf(end));
	return str;
}

QString advancedReplace(QString str, QString startReplace, QString endReplace, QString newText) {
	int indexOfStart = str.indexOf(startReplace);
	int indexOfEnd = str.indexOf(endReplace, indexOfStart);
	while (indexOfStart >= 0 && indexOfEnd >= 0) {
		str.replace(indexOfStart, indexOfEnd + endReplace.length() - indexOfStart, newText);
		indexOfStart = str.indexOf(startReplace, indexOfStart + newText.length());
		indexOfEnd = str.indexOf(endReplace, indexOfStart + startReplace.length());
	}
	return str;
}

QString urlToQString(QString url) {
	return QString::fromUtf8(MyCurl::urlToString(url.toStdString(), OMA::Settings::getCurlVerbose(),
												 OMA::Settings::getCurlUserAgent().toStdString())
								 .c_str());
	// return QString::fromUtf8(MyPhantomJS::urlToString(url.toStdString()).c_str());
}
QString urlToQStringJS(QString url, QString query) {
	// return QString::fromUtf8(MyAwesomium::urlToString(url.toStdString()).c_str());
	return QString::fromUtf8(
		MyPhantomJS::urlToStringJS(url.toStdString(), query.toStdString()).c_str());
}

bool urlToFile(QString url, QString filename) {
	return MyCurl::urlToFile(url.toStdString(), filename.toStdString());
}

QString redirectedUrlQt(QString url) {
	return QString::fromLocal8Bit(MyCurl::redirectedUrl(url.toStdString()).c_str());
}

QString decodeHtml(QString html) {
	return QString::fromLocal8Bit(MyCurl::decodeHtml(html.toStdString()).c_str());
}

QString simplify(QString s) {
	s = s.replace("ō", "ou");
	QString s1;
	for (int i = 0; i < s.length(); i++) {
		if (s[i].isLetterOrNumber() || ((QString) " -_&[]\n").contains(s[i]))
			s1 += s[i];
	}
	return s1;
}
QString simplifyOneLine(QString s) {
	s = simplify(s);
	return s.replace("\n", "");
}

QString simplifyEp(QString ep) {
	ep = simplify(ep);
	ep = ep.replace(" Episode ", " ");
	ep = ep.replace(" Episodio ", " ");
	ep = ep.replace(" Ep ", " ");
	ep = ep.replace("-END", " END");
	ep = ep.trimmed();
	return ep;
}

} // namespace MyUtils
