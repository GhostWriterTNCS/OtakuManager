#include "OMA.h"
#include "Website.h"

void Website::getEpisodes_AnimeForce_IT() {
	QString html = MyUtils::urlToQString(homepage);
	QString start = "<div class=\"content-wrapper\">";
	QString end = "<div class=\"pagination pagination-mobile\">";
	if (html.contains(end)) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split("<h2>");
		for (int i = 1; i < list.size(); i++) {
			Episode episode;
			html = list[i];
			html = MyUtils::substring(html, "href=\"", "\"");
			episode.url = html;

			html = list[i];
			html = MyUtils::advancedReplace(html, "<", ">", "");
			// html = html.replace("Episodio", "");
			episode.name = html.trimmed();

			if (!episode.name.contains("Download"))
				episode.hasDownload = false;
			episode.name = MyUtils::substring(episode.name, "", " Sub Ita");
			episodes.push_back(episode);
		}
	}
}

void Website::getSeries_AnimeForce_IT() {
	QStringList list = MyUtils::urlToQString(seriesPage).split("<li><strong>");
	for (int i = 1; i < list.size(); i++) {
		Anime anime;
		QString s = list[i];
		s = MyUtils::substring(s, "href=\"", "\"");
		anime.url = s;

		s = list[i];
		s = MyUtils::substring(s, "", " Sub Ita");
		s = MyUtils::advancedReplace(s, "<h2>", "</h2>", "");
		s = MyUtils::advancedReplace(s, "<", ">", "");
		anime.name = s.trimmed();
		series.push_back(anime);
	}
}

QString Website::goToEpisode_AnimeForce_IT(Episode* episode, QString type) {
	QString url = episode->url;
	QStringList codes = {"-episodio-", "-oav-", "-movie-"};
	int index, charsToReplace;
	for (int i = 0; i < codes.size(); i++) {
		if (url.contains(codes[i])) {
			index = url.lastIndexOf(codes[i]) + 1;
			charsToReplace = codes[i].length();
			while (isdigit(url.toStdString()[index + charsToReplace]) ||
				   url[index + charsToReplace] == '-') {
				charsToReplace++;
			}
			break;
		}
	}
	url = url.replace(index, charsToReplace, "");
	url = MyUtils::substring(url, "", "-sub-ita") + "-sub-ita";

	if (type != OMA::linkTypes[LinkTypes::animeInfo]) {
		QString name = MyUtils::substring(episode->name, "", " Sub Ita");
		QStringList otherCodes = {"Episodio", "OAV", "Movie"};
		for (int i = 0; i < otherCodes.size(); i++) {
			if (name.contains(otherCodes[i]))
				name = otherCodes[i] + MyUtils::substringFromEnd(name, otherCodes[i]);
		}

		QString s = MyUtils::urlToQString(url);
		s = MyUtils::substring(s, "<h6>Streaming</h6>");
		QStringList list = s.split("<tr>");
		s = "";
		for (int i = 0; i < list.size(); i++) {
			if (list[i].contains(name)) {
				s = list[i];
				break;
			}
		}
		s = MyUtils::substring(s, "href=\"", "\"");
		if (s.indexOf("/ds") == 0)
			s = "http://www.animeforce.org" + s;
		if (type == OMA::linkTypes[LinkTypes::streaming]) {
			url = s;
		} else if (type == OMA::linkTypes[LinkTypes::download]) {
			url = MyUtils::redirectedUrlQt(s);
			url = MyUtils::advancedReplace(
				url, "", ".php?file=", "http://go.animeforce.org/1706398/http://");
		}
	}
	return url;
}
