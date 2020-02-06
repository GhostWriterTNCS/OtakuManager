#include "OMA.h"
#include "Website.h"

void Website::initialize_KissAnime_EN() {
	homepage = "https://kissanime.io/";
	getEpisodesFunction = std::bind(&Website::getEpisodes_KissAnime_EN, this);
	goToEpisodeFunction = std::bind(&Website::goToEpisode_KissAnime_EN, this, std::placeholders::_1,
									std::placeholders::_2);
}

bool Website::getEpisodes_KissAnime_EN() {
	QString html = MyUtils::urlToQStringJS(homepage);
	QString start = "<div class=\"barTitle full\">Latest anime update</div>";
	QString end = "<div class=\"full more_in_tab more_in_rightBox\">";
	if (html.contains(end)) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split("<div class=\"tit\">");
		for (int i = 1; i < list.size(); i++) {
			Episode episode;
			html = list[i];
			html = MyUtils::substring(html, "</h4>", "</a>");
			episode.url = MyUtils::substring(html, "href=\"", "\"");

			html = MyUtils::substring(html, "title=\"", "\"");
			episode.name = html.trimmed();

			episodes.push_back(episode);
		}
		return true;
	}
	return false;
}

QString Website::goToEpisode_KissAnime_EN(Episode* episode, QString type) {
	QString s = episode->url;
	if (type == OMA::linkTypes[LinkTypes::animeInfo]) {
		s = MyUtils::substringFromEnd(episode->url, "", "/");
	} else if (type == OMA::linkTypes[LinkTypes::download]) {
		s = MyUtils::urlToQStringJS(episode->url);
		if (s.contains("<div class=\"clear2 h20\"></div>")) {
			s = MyUtils::substring(s, "id=\"divDownload\"", "</div>");
			s = MyUtils::substring(s, "href=\"", "\"").replace("&amp;", "&");
		} else {
			s = "";
		}
	}
	return s;
}
