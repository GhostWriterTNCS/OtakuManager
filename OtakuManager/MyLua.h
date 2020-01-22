#pragma once

extern "C" {
#include <lua\lauxlib.h>
#include <lua\lua.hpp>
#include <lua\lualib.h>
}

#include <QString>
#include "Website.h"

namespace MyLua {

QString getString(QString website, QString name);

bool getBool(QString website, QString name);

/**
	Get the episodes.
*/
bool getEpisodes(QString website, QList<Episode>* episodes);

/**
	Get the series.
*/
bool getSeries(QString website);

/**
	Perform an action according to the chosen type (streaming, download, anime info...)
*/
QString goToEpisode(QString website, Episode* episode, QString type);

} // namespace MyLua
