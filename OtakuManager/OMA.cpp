#include <iostream>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>
#include "MyUtils.h"
#include "OMA.h"

LastEpisode::LastEpisode(QStringList list) {
	this->website = list[0];
	this->episode = list[1];
}

FollowedAnime::FollowedAnime(QString anime, bool regex, QString website, QString customLink) {
	this->anime = anime;
	this->regex = regex;
	this->website = website;
	this->customLink = customLink;
}
FollowedAnime::FollowedAnime(QStringList list) {
	this->anime = list[0];
	this->regex = (list[1] == "true");
	this->website = list[2];
	this->customLink = list[3];
}

namespace OMA {

MainWindow* window;
MainWindow* getMainWindow() {
	return window;
}
void setMainWindow(MainWindow* mainWindow) {
	window = mainWindow;
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
	name = " " + name + " ";
	QList<FollowedAnime> list = Settings::getFollowed();
	for (int i = 0; i < list.size(); i++) {
		if (list[i].regex) {
			if (name.contains(QRegExp(list[i].anime, Qt::CaseInsensitive))) {
				if (website.isEmpty() || list[i].website == "*" || website == list[i].website) {
					return true;
				}
			}
		} else {
			if (MyUtils::simplify(name).contains(list[i].anime, Qt::CaseInsensitive)) {
				if (website.isEmpty() || list[i].website == "*" || website == list[i].website) {
					return true;
				}
			}
		}
	}
	return false;
}

bool upenUrl(QString url) {
	if (!QDesktopServices::openUrl(QUrl(url))) {
		QMessageBox::warning(OMA::getMainWindow(), "Error", "Unable to open url: " + url);
		return false;
	}
	return true;
}

} // namespace OMA
