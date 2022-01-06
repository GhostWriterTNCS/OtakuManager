#include <iostream>
#include <QFileInfo>
#include <QMessageBox>
#include "MyLua.h"
#include "OMA.h"
#include "Website.h"

Website::Website(QString website) {
	name = website;
	QFileInfo check_file("lua/" + website + ".lua");
	if (check_file.exists() && check_file.isFile() &&
		MyLua::getString(name, "homepage").size() > 0) {
		homepage = MyLua::getString(name, "homepage");
		hasStreaming = MyLua::getBool(name, "hasStreaming", true);
		hasDownload = MyLua::getBool(name, "hasDownload", true);
		hasTorrent = MyLua::getBool(name, "hasTorrent", false);
		hasMagnet = MyLua::getBool(name, "hasMagnet", false);
		hasInfo = MyLua::getBool(name, "hasInfo", true);
		return;
	}

	if (OMA::Settings::getFeedNames().contains(website)) {
		initialize_Feed();
	} else {
		QMessageBox::warning(OMA::getMainWindow(), "Error", "Website not valid: " + website);
	}
}

bool Website::getEpisodes() {
	episodes.clear();
	bool succesful = MyLua::getEpisodes(name, &episodes);
	if (!succesful && getEpisodesFunction != nullptr) {
		succesful = getEpisodesFunction();
	}
	if (episodes.size() > 0) {
		QString lastEp = OMA::Settings::getLastEp(name);
		for (int i = 0; i < episodes.size(); i++) {
			if (episodes[i].name.remove("|") != lastEp) {
				episodes[i].isNew = true;
			} else {
				break;
			}
		}
		OMA::Settings::setLastEp(name, episodes[0].name);
	} else {
		std::cout << "No episodes for " << name.toStdString().c_str() << std::endl;
	}
	return succesful;
}

bool Website::goToEpisode(Episode* episode, QString type) {
	if (type == OMA::linkTypes[LinkTypes::torrent]) {
		QString dir = OMA::Settings::getTorrentDir();
		if (!dir.isEmpty()) {
			dir = dir + "\\";
			dir = dir.replace("/", "\\");
		}
		MyUtils::urlToFile(episode->downloadLink, dir + episode->name + ".torrent");
		return true;
	}

	QString url = episode->url;
	if (type == OMA::linkTypes[LinkTypes::magnet]) {
		url = episode->magnetLink;
	} else if (type != OMA::linkTypes[LinkTypes::post] &&
			   type != OMA::linkTypes[LinkTypes::customLink]) {
		url = MyLua::goToEpisode(name, episode, type);
		if (url.isEmpty() && goToEpisodeFunction != nullptr) {
			url = goToEpisodeFunction(episode, type);
		}
	}

	if (url.startsWith("https://www.youtube.com/watch?v=")) {
		if (OMA::Settings::getYoutubeToUmmy()) {
			url = url.replace("https://", "ummy://");
		} else if (!OMA::Settings::getYoutubeCustomLink().isEmpty()) {
			url = OMA::Settings::getYoutubeCustomLink().replace("*", url);
		}
	}

	return OMA::upenUrl(url);
}

/*
	Feeds handler
*/
void Website::initialize_Feed() {
	QList<Feed> feeds = OMA::Settings::getFeeds();
	for (int i = 0; i < feeds.size(); i++) {
		if (feeds[i].name == name) {
			episodesPage = feeds[i].url;
			if (feeds[i].homepage.isEmpty()) {
				homepage = feeds[i].url;
			} else {
				homepage = feeds[i].homepage;
			}
		}
	}
	hasStreaming = false;
	hasDownload = false;
	hasInfo = false;
	isFeed = true;
	getEpisodesFunction = std::bind(&Website::getEpisodes_Feed, this);
	goToEpisodeFunction =
		std::bind(&Website::goToEpisode_Feed, this, std::placeholders::_1, std::placeholders::_2);
}

bool Website::getEpisodes_Feed() {
	QString html = MyUtils::urlToQString(episodesPage);
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

QString Website::goToEpisode_Feed(Episode* episode, QString type) {
	return "";
}
