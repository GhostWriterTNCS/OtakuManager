#pragma once
#include <QHash>
#include <QSettings>
#include "MainWindow.h"

enum LinkTypes {
	customLink,
	streaming,
	streamingIfNoDownload,
	download,
	torrent,
	magnet,
	animeInfo,
	post,
	postIfNoDownload
};

class LastEpisode {
public:
	QString website = "";
	QString episode = "";
	LastEpisode(QStringList list);
};

class FollowedAnime {
public:
	QString anime = "";
	bool regex = false;
	QString website = "*";
	QString customLink = "";
	FollowedAnime();
	FollowedAnime(QStringList list);
	FollowedAnime(QString anime, bool regex, QString website, QString customLink);
};

class Feed {
public:
	QString name = "";
	QString url = "";
	QString homepage = "";
	QString oldName = "";
	Feed();
	Feed(QStringList list);
	Feed(QString name, QString url, QString homepage, QString oldName = "");
};

namespace OMA {

const QString version = "3.8";

const QStringList linkTypes = {"Custom link", "Streaming", "StreamingIfNoDownload",
							   "Download",	  "Torrent",   "Magnet",
							   "Anime info",  "Post",	   "PostIfNoDownload"};

const QStringList feedTypes = {"Atom", "RSS"};

QHash<QString, QStringList> websites(bool forceRefresh = false);

const QString errorMex = "<h3 style='text-align: center;'>Error during connection :(</h3><p "
						 "style='text-align: center;'>If problem persists, check your internet "
						 "connection or try another website.</p>";

const QString torrentDownloaded = "OMA::torrentDownloaded";

MainWindow* getMainWindow();
void setMainWindow(MainWindow* mainWindow);

/**
	Add a message in the status bar.
*/
void addStatusMessage(QString message);
/**
	Remove a message from the status bar.
*/
void removeStatusMessage(QString message);

/**
	Check if an episode is seen.
*/
bool isSeen(QString episode);

/**
	Check if an episode is followed.
*/
bool isFollowed(QString name, QString website = "");

/**
	Open an url in the default browser.
*/
bool upenUrl(QString url);

namespace Settings {

/**
	Save settings in file 'settings.ini'.
*/
void sync();

/**
	Fix changes between versions.
*/
void fix();

/*
	Get main window size.
*/
QList<int> getSize();
void setSize();

// [General]

/**
	Get the version of the software that created the file 'settings.ini'.
*/
QString getVersion();
/**
	Set the current software version.
*/
void setVersion();

/**
	Check for updates on startup?
*/
bool getCheckForUpdates();
void setCheckForUpdates(bool b);

bool getShowFollowedTab();
void setShowFollowedTab(bool b);

bool getTabsOnSide();
void setTabsOnSide(bool b);

/**
	Websites to show.
*/
QString getWebsitesQString();
QStringList getWebsites();
void setWebsites(QStringList list);

/**
	Buttons to show.
*/
QStringList getButtons();
void setButtons(QString hidden, bool hide = true);

/**
	Feeds.
*/
QList<Feed> getFeeds();
QStringList getFeedNames();
/**
	Edit a feed.
*/
void setFeeds(QString name, QString url, QString homepage, QString oldName = "");
/**
	Set feeds.
*/
void setFeeds(QList<Feed> list);

// [Followed]

/**
	Get the followed anime/keywords.
*/
QList<FollowedAnime> getFollowed();
/**
	Return the FollowedAnime structure for an episode name and website. Leave website as an empty
	string to search in all websites. If the episode is not followed or it is not followed for the
	specified website, return FollowedAnime().
*/
FollowedAnime getFollowed(QString name, QString website = "");
/**
	Add a followed anime/keyword.
*/
void setFollowed(QString name, bool regex = false, QString website = "*", QString customLink = "",
				 bool followed = true);
/**
	Bulk add followed anime/keywords.
*/
void setFollowed(QList<FollowedAnime> list);

// [Other]

/**
	Direct download torrent files?
*/
// bool getDownloadTorrent();
// void setDownloadTorrent(bool value);

/**
	Torrent files download directory.
*/
QString getTorrentDir();
void setTorrentDir(QString dir);

/**
	Open YouTube links with Ummy?
*/
bool getYoutubeToUmmy();
void setYoutubeToUmmy(bool value);

/**
	Use a custom link for YouTube videos.
*/
QString getYoutubeCustomLink();
void setYoutubeCustomLink(QString value);

/**
	Show console?
*/
bool getConsole();
void setConsole(bool value);

/**
	cURL verbose?
*/
bool getCurlVerbose();
void setCurlVerbose(bool value);

QString getCurlUserAgent();
void setCurlUserAgent(QString value);

// [Log]

/**
	Get the last parsed episodes for each website.
*/
QList<LastEpisode> getLastEps();
/**
	Get the last parsed episode for a website.
*/
QString getLastEp(QString website);
/**
	Set the last parsed episode for a website.
*/
void setLastEp(QString website, QString episode);

/**
	Get the seen episodes.
*/
QString getSeen();
/**
	Set if a episodes is seen.
*/
void setSeen(QString ep, bool seen = true);

} // namespace Settings

} // namespace OMA
