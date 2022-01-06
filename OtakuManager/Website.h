#pragma once
#include <functional>
#include <QString>
#include <QStringList>
#include <QtWidgets/QMainWindow>
#include "MyUtils.h"

class Episode {
public:
	QString url;
	QString name;
	QString streamingLink;
	QString downloadLink;
	QString magnetLink;
	bool isNew = false;
	bool hasDownload = true;
}; // class Episode

class Anime {
public:
	QString url;
	QString name;
}; // class Anime

class Website {
public:
	QString name;
	QString homepage;
	QString episodesPage;
	bool hasStreaming = true;
	bool hasDownload = true;
	bool hasTorrent = false;
	bool hasMagnet = false;
	bool hasInfo = true;
	bool isFeed = false;
	std::function<bool()> getEpisodesFunction;
	std::function<QString(Episode*, QString)> goToEpisodeFunction;

	QList<Episode> episodes;

	Website(QString website);

	/**
		Get the episodes.
	*/
	bool getEpisodes();

	/**
		Perform an action according to the chosen type (streaming, download, anime info...)
	*/
	bool goToEpisode(Episode* episode, QString type);

private:
	void initialize_Feed();
	bool getEpisodes_Feed();
	QString goToEpisode_Feed(Episode* episode, QString type);

}; // class Website
