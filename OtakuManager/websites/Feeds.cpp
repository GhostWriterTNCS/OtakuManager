#include "OMA.h"
#include "Website.h"

void Website::initialize_Feed() {
	QList<Feed> feeds = OMA::Settings::getFeeds();
	for (int i = 0; i < feeds.size(); i++) {
		if (feeds[i].name == name) {
			homepage = feeds[i].url;
		}
	}
	isFeed = true;
	hasStreaming = false;
	hasDownload = false;
	getEpisodesFunction = std::bind(&Website::getEpisodes_Feed, this);
	getSeriesFunction = std::bind(&Website::getSeries_Feed, this);
	goToEpisodeFunction =
		std::bind(&Website::goToEpisode_Feed, this, std::placeholders::_1, std::placeholders::_2);
}

bool Website::getEpisodes_Feed() {
	QString html = MyUtils::urlToQString(homepage);
	QString start, end, type;
	if (html.contains("<feed ")) {
		start = "<entry>";
		end = "</feed>";
		type = "Atom";
	} else if (html.contains("<rss ")) {
		start = "<item>";
		end = "</rss>";
		type = "RSS";
	}
	if (!end.isEmpty() && html.contains(end)) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split(start);
		for (int i = 0; i < list.size(); i++) {
			Episode episode;
			list[i] = list[i].replace("\"", "'");
			html = list[i];
			if (type == "Atom") {
				html = MyUtils::substring(html, "<link rel='alternate'", "/>");
				html = MyUtils::substring(html, "href='", "'");
			} else if (type == "RSS") {
				html = MyUtils::substring(html, "<link>", "</link>");
			}
			episode.url = html;

			html = MyUtils::substring(list[i], "<title", "</title>");
			html = MyUtils::substring(html, ">", "<");
			episode.name = MyUtils::decodeHtml(html).trimmed();

			episodes.push_back(episode);
		}
		return true;
	}
	return false;
}

bool Website::getSeries_Feed() {
	return false;
}

QString Website::goToEpisode_Feed(Episode* episode, QString type) {
	return "";
}
