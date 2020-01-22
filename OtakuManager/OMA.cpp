#include <iostream>
#include <QDesktopServices>
#include <QMessageBox>
#include <QProcess>
#include <QUrl>
#include <QtCore\qdir.h>
#include "MyLua.h"
#include "MyUtils.h"
#include "OMA.h"

LastEpisode::LastEpisode(QStringList list) {
	this->website = list[0];
	this->episode = list[1];
}

FollowedAnime::FollowedAnime() {}
FollowedAnime::FollowedAnime(QString anime, bool regex, QString website, QString customLink) {
	this->anime = anime;
	this->regex = regex;
	this->website = website;
	this->customLink = customLink;
}
FollowedAnime::FollowedAnime(QStringList list) {
	if (list.size() > 3) {
		this->anime = list[0];
		this->regex = (list[1] == "true");
		this->website = list[2];
		this->customLink = list[3];
	}
}

Feed::Feed() {}
Feed::Feed(QStringList list) {
	if (list.size() > 1) {
		this->name = list[0];
		this->url = list[1];
	}
}
Feed::Feed(QString name, QString url, QString oldName) {
	this->name = name;
	this->url = url;
	this->oldName = oldName;
}

namespace OMA {

MainWindow* mainWindow;
QHash<QString, QStringList> websites_;

QHash<QString, QStringList> websites() {
	if (websites_.keys().size() > 0) {
		return websites_;
	}
	websites_["English"] = QStringList({"Anime Heaven", "KissAnime"});
	websites_["Italian"] = QStringList({"AnimeForce", "RedAnimeDatabase", "WebAnimex"});
	websites_["Torrents"] = QStringList({"AniDex", "Nyaa"});

	QDir directory("lua");
	QStringList files = directory.entryList(QStringList() << "*.lua", QDir::Files);
	foreach (QString filename, files) {
		filename = filename.replace(".lua", "");
		QString group = MyLua::getString(filename, "group");
		if (!websites_.contains(group)) {
			websites_[group] = QStringList({filename});
		} else if (!websites_[group].contains(filename)) {
			websites_[group].append(filename);
		}
	}
	websites_["Feeds"] = OMA::Settings::getFeedNames();
	foreach (QString group, websites_.keys()) { websites_[group].sort(); }

	return websites_;
}

MainWindow* getMainWindow() {
	return mainWindow;
}
void setMainWindow(MainWindow* window) {
	mainWindow = window;
}

QStringList messages;
void addStatusMessage(QString mex) {
	messages.append(mex);
	QString s = messages[0];
	for (int i = 1; i < messages.count(); i++) {
		s += " | " + messages[i];
	}
	mainWindow->ui.statusBar->showMessage("[" + QString::number(messages.count()) + "] " + s);
}
void removeStatusMessage(QString mex) {
	messages.removeOne(mex);
	if (messages.count() > 0) {
		QString s = messages[0];
		for (int i = 1; i < messages.count(); i++) {
			s += " | " + messages[i];
		}
		mainWindow->ui.statusBar->showMessage("[" + QString::number(messages.count()) + "] " + s);
	} else {
		mainWindow->ui.statusBar->clearMessage();
	}
}

bool isSeen(QString ep) {
	ep = MyUtils::simplifyEp(ep);
	QString seen = "|" + Settings::getSeen() + "|";
	if (seen.contains("|" + ep + "|"))
		return true;
	else
		return false;
}

bool isFollowed(QString name, QString website) {
	FollowedAnime followed = Settings::getFollowed(name, website);
	return !followed.anime.isEmpty();
}

bool upenUrl(QString url) {
	if (url.isEmpty()) {
		emit mainWindow->showMessageBoxSignal(QMessageBox::Icon::Warning, "Error", "Url is empty.");
		// QMessageBox::warning(mainWindow, "Error", "Url is empty.");
		return false;
	} else if (!QDesktopServices::openUrl(QUrl(url))) {
		QProcess process;
		if (!process.startDetached(url.replace("file:///", ""))) {
			emit mainWindow->showMessageBoxSignal(QMessageBox::Icon::Warning, "Error",
												  "Unable to open " + url);
			// QMessageBox::warning(mainWindow, "Error", "Unable to open " + url);
			return false;
		}
	}
	return true;
}

} // namespace OMA
