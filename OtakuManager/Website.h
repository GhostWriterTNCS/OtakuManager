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
	QString seriesPage;
	bool hasDownload = true;
	bool hasDoubleButtons = false;
	std::function<void()> getEpisodesFunction;
	std::function<void()> getSeriesFunction;
	std::function<QString(Episode*, QString)> goToEpisodeFunction;

	QList<Episode> episodes;
	QList<Anime> series;

	Website(QString website);
	QList<Episode> getEpisodes();
	QList<Anime> getSeries();
	bool goToEpisode(Episode* episode, QString type);

private:
	void getEpisodes_AnimeHeaven_EN();
	void getSeries_AnimeHeaven_EN();
	QString goToEpisode_AnimeHeaven_EN(Episode* episode, QString type);

	void getEpisodes_AnimeForce_IT();
	void getSeries_AnimeForce_IT();
	QString goToEpisode_AnimeForce_IT(Episode* episode, QString type);

	void getEpisodes_RedAnimeDatabase_IT();
	void getSeries_RedAnimeDatabase_IT();
	QString goToEpisode_RedAnimeDatabase_IT(Episode* episode, QString type);

	void getEpisodes_WebAnimex_IT();
	void getSeries_WebAnimex_IT();
	QString goToEpisode_WebAnimex_IT(Episode* episode, QString type);
}; // class Website
