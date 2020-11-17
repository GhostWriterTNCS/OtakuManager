#include <iostream>

#include "HtmlDoc.h"
#include "MyCurl.h"
#include "MyLua.h"
#include "MyPhantomJS\MyPhantomJS.h"
#include "OMA.h"
#include "Website.h"

namespace MyLua {

static int lua_UrlToString(lua_State* L) {
	// get first argument.
	luaL_checkstring(L, 1);
	std::string url = lua_tostring(L, 1);

	// push result.
	std::string s = MyCurl::urlToString(url, OMA::Settings::getCurlVerbose(),
										OMA::Settings::getCurlUserAgent().toStdString());
	lua_pushlstring(L, s.c_str(), s.length());

	// return the number of results.
	return 1;
}

static int lua_UrlToStringJS(lua_State* L) {
	// get first argument.
	luaL_checkstring(L, 1);
	std::string url = lua_tostring(L, 1);
	std::string query = lua_tostring(L, 2);

	// push result.
	std::string s = MyPhantomJS::urlToStringJS(url, query);
	// std::cout << s << std::endl;
	lua_pushlstring(L, s.c_str(), s.length());

	// return the number of results.
	return 1;
}

static int lua_HtmlDoc(lua_State* L) {
	luaL_checkstring(L, 1);
	std::string content = lua_tostring(L, 1);

	HtmlDoc* doc;
	if (lua_gettop(L) == 2) {
		std::string encoding = lua_tostring(L, 2);
		doc = new HtmlDoc(content, encoding);
	} else {
		doc = new HtmlDoc(content);
	}

	lua_pushlightuserdata(L, doc);
	return 1;
}

static int lua_GetString(lua_State* L) {
	HtmlDoc* doc = (HtmlDoc*)lua_touserdata(L, 1);
	if (!doc) {
		std::cerr << "HtmlDoc not valid." << std::endl;
	}
	luaL_checkstring(L, 2);
	std::string xpath = lua_tostring(L, 2);

	std::string result = doc->GetString(xpath);

	lua_pushstring(L, result.c_str());
	return 1;
}

static int lua_GetAllStrings(lua_State* L) {
	HtmlDoc* doc = (HtmlDoc*)lua_touserdata(L, 1);
	if (!doc) {
		std::cerr << "HtmlDoc not valid." << std::endl;
	}
	luaL_checkstring(L, 2);
	std::string xpath = lua_tostring(L, 2);

	std::vector<std::string> result = doc->GetAllStrings(xpath);
	lua_newtable(L); // put result in a table
	for (int i = 0; i < result.size(); i++) {
		lua_pushlstring(L, result[i].c_str(), result[i].length());
		lua_rawseti(L, -2, i + 1);
	}
	return 1;
}

static int lua_Substring(lua_State* L) {
	luaL_checkstring(L, 1);
	QString str = QString(lua_tostring(L, 1));
	luaL_checkstring(L, 2);
	QString start = QString(lua_tostring(L, 2));
	luaL_checkstring(L, 3);
	QString end = QString(lua_tostring(L, 3));

	std::string result = MyUtils::substring(str, start, end).toStdString();

	lua_pushstring(L, result.c_str());
	return 1;
}

static int lua_SubstringFromEnd(lua_State* L) {
	luaL_checkstring(L, 1);
	QString str = QString(lua_tostring(L, 1));
	luaL_checkstring(L, 2);
	QString start = QString(lua_tostring(L, 2));
	luaL_checkstring(L, 3);
	QString end = QString(lua_tostring(L, 3));

	std::string result = MyUtils::substringFromEnd(str, start, end).toStdString();

	lua_pushstring(L, result.c_str());
	return 1;
}

// Episode class
static int lua_Episode_garbageCollector(lua_State* L) {
	delete *static_cast<Episode**>(luaL_checkudata(L, 1, "Episode"));
	return 0;
}

static int lua_Episode_SetName(lua_State* L) {
	Episode** ud = static_cast<Episode**>(luaL_checkudata(L, 1, "Episode"));
	std::string s = luaL_checkstring(L, 2);
	(*ud)->name = QString::fromLocal8Bit(s.c_str());
	return 0;
}

static int lua_Episode_SetUrl(lua_State* L) {
	Episode** ud = static_cast<Episode**>(luaL_checkudata(L, 1, "Episode"));
	std::string s = luaL_checkstring(L, 2);
	(*ud)->url = QString::fromLocal8Bit(s.c_str());
	return 0;
}

static int lua_Episode_SetDownload(lua_State* L) {
	Episode** ud = static_cast<Episode**>(luaL_checkudata(L, 1, "Episode"));
	std::string s = luaL_checkstring(L, 2);
	(*ud)->downloadLink = QString::fromLocal8Bit(s.c_str());
	return 0;
}

static int lua_Episode_SetMagnet(lua_State* L) {
	Episode** ud = static_cast<Episode**>(luaL_checkudata(L, 1, "Episode"));
	std::string s = luaL_checkstring(L, 2);
	(*ud)->magnetLink = QString::fromLocal8Bit(s.c_str());
	return 0;
}

static int lua_Episode_HasDownload(lua_State* L) {
	Episode** ud = static_cast<Episode**>(luaL_checkudata(L, 1, "Episode"));
	bool b = lua_toboolean(L, 2);
	(*ud)->hasDownload = b;
	return 0;
}

static int lua_Episode(lua_State* L) {
	*static_cast<Episode**>(lua_newuserdata(L, sizeof(Episode*))) = new Episode();
	if (luaL_newmetatable(L, "Episode")) {
		static const luaL_Reg functions[] = {{"__gc", lua_Episode_garbageCollector},
											 {"SetName", lua_Episode_SetName},
											 {"SetUrl", lua_Episode_SetUrl},
											 {"SetDownload", lua_Episode_SetDownload},
											 {"SetMagnet", lua_Episode_SetMagnet},
											 {"HasDownload", lua_Episode_HasDownload},
											 {nullptr, nullptr}};
		luaL_setfuncs(L, functions, 0);
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
	}
	lua_setmetatable(L, -2);
	return 1;
}

static int lua_print(lua_State* L) {
	switch (lua_type(L, lua_gettop(L))) {
		case LUA_TNUMBER:
			std::cout << lua_tonumber(L, lua_gettop(L)) << std::endl;
			break;
		case LUA_TTABLE:
			std::cout << "script returned a table" << std::endl;
			lua_pushnil(L);
			while (lua_next(L, -2) != 0) {
				std::cout << lua_tostring(L, -2) << "=" << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);
			}
			break;
		case LUA_TSTRING:
			std::cout << lua_tostring(L, lua_gettop(L)) << std::endl;
			break;
		case LUA_TBOOLEAN:
			std::cout << lua_toboolean(L, lua_gettop(L)) << std::endl;
			break;
		default:
			std::cout << "script returned unknown param" << std::endl;
			break;
	}
	lua_pop(L, 1);
	return 0;
}

static int lua_Split(lua_State* L) {
	const char* s = luaL_checkstring(L, 1);
	const char* sep = luaL_checkstring(L, 2);
	const char* e;
	int i = 1;

	lua_newtable(L); /* result */

	/* repeat for each separator */
	while ((e = strchr(s, *sep)) != NULL) {
		lua_pushlstring(L, s, e - s); /* push substring */
		lua_rawseti(L, -2, i++);
		s = e + 1; /* skip separator */
	}

	/* push last substring */
	lua_pushstring(L, s);
	lua_rawseti(L, -2, i);

	return 1; /* return the table */
}

lua_State* initialize() {
	// initialize Lua
	lua_State* L = luaL_newstate();

	// load Lua base libraries
	luaL_openlibs(L);

	// register my functions
	lua_register(L, "UrlToString", lua_UrlToString);
	lua_register(L, "UrlToStringJS", lua_UrlToStringJS);
	lua_register(L, "HtmlDoc", lua_HtmlDoc);
	lua_register(L, "GetString", lua_GetString);
	lua_register(L, "GetAllStrings", lua_GetAllStrings);
	lua_register(L, "print", lua_print);
	lua_register(L, "Split", lua_Split);
	lua_register(L, "Episode", lua_Episode);
	lua_register(L, "Substring", lua_Substring);
	lua_register(L, "SubstringFromEnd", lua_SubstringFromEnd);

	return L;
}

QString getString(QString website, QString name) {
	lua_State* L = initialize();

	// load the script
	int l = luaL_dofile(L, ("lua/" + website + ".lua").toStdString().c_str());
	if (l != 0) {
		/*emit OMA::getMainWindow()->showMessageBoxSignal(QMessageBox::Icon::Warning, "Error",
														"Lua module not found: " + website);*/
		std::cerr << "Lua module not found: " + website.toStdString() << std::endl;
		lua_close(L);
		return false;
	}

	int stack_size = lua_gettop(L);

	lua_getglobal(L, name.toStdString().c_str());
	QString luavar = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_close(L);
	return luavar;
}

bool getBool(QString website, QString name) {
	lua_State* L = initialize();

	// load the script
	int l = luaL_dofile(L, ("lua/" + website + ".lua").toStdString().c_str());
	if (l != 0) {
		/*emit OMA::getMainWindow()->showMessageBoxSignal(QMessageBox::Icon::Warning, "Error",
														"Lua module not found: " + website);*/
		std::cerr << "Lua module not found: " + website.toStdString() << std::endl;
		lua_close(L);
		return false;
	}

	int stack_size = lua_gettop(L);

	lua_getglobal(L, name.toStdString().c_str());
	bool luavar = lua_toboolean(L, -1);
	lua_pop(L, 1);

	lua_close(L);
	return luavar;
}

bool getEpisodes(QString website, QList<Episode>* episodes) {
	lua_State* L = initialize();

	// load the script
	int l = luaL_dofile(L, ("lua/" + website + ".lua").toStdString().c_str());
	if (l != 0) {
		/*emit OMA::getMainWindow()->showMessageBoxSignal(QMessageBox::Icon::Warning, "Error",
														"Lua module not found: " + website);*/
		std::cerr << "Lua module not found: " + website.toStdString() << std::endl;
		lua_close(L);
		return false;
	}

	int stack_size = lua_gettop(L);

	// the function name
	lua_getglobal(L, "getEpisodes");

	// call the function with 0 arguments, return 1 result
	lua_pcall(L, 0, 1, 0);

	// get the results
	int num_returns = lua_gettop(L) - stack_size;
	std::cout << num_returns << " results." << std::endl;

	if (lua_istable(L, -1)) {
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			/* uses 'key' (at index -2) and 'value' (at index -1) */
			Episode** ud = static_cast<Episode**>(luaL_checkudata(L, -1, "Episode"));
			// std::cout << (*ud)->name.toStdString() << " " << (*ud)->url.toStdString() <<
			// std::endl;
			episodes->append(**ud);
			/* removes 'value'; keeps 'key' for next iteration */
			lua_pop(L, 1);
		}
	} else {
		printf("1 isn't a table\n");
	}

	/*while (lua_gettop(L)) {
		if (lua_isstring(L, -1))
			std::cout << lua_tostring(L, lua_gettop(L)) << std::endl;
		else
			std::cout << "Non-string result" << std::endl;
		lua_pop(L, 1);
	}*/
	lua_pop(L, 1);

	lua_close(L);
	return true;
}

QString goToEpisode(QString website, Episode* episode, QString type) {
	lua_State* L = initialize();

	// load the script
	int l = luaL_dofile(L, ("lua/" + website + ".lua").toStdString().c_str());
	if (l != 0) {
		/*emit OMA::getMainWindow()->showMessageBoxSignal(QMessageBox::Icon::Warning, "Error",
														"Lua module not found: " + website);*/
		std::cerr << "Lua module not found: " + website.toStdString() << std::endl;
		lua_close(L);
		return "";
	}

	int stack_size = lua_gettop(L);

	// the function name
	lua_getglobal(L, "goToEpisode");
	lua_pushstring(L, episode->url.toStdString().c_str());
	lua_pushstring(L, type.toStdString().c_str());

	// call the function with 2 arguments, return 1 result
	lua_pcall(L, 2, 1, 0);

	// get the results
	int num_returns = lua_gettop(L) - stack_size;
	std::cout << num_returns << " results." << std::endl;

	std::string url = "";
	if (lua_isstring(L, -1)) {
		url = lua_tostring(L, lua_gettop(L));
		// std::cout << url << std::endl;
	} else {
		std::cout << "Result is not a string." << std::endl;
	}

	/*while (lua_gettop(L)) {
		if (lua_isstring(L, -1))
			std::cout << lua_tostring(L, lua_gettop(L)) << std::endl;
		else
			std::cout << "Non-string result" << std::endl;
		lua_pop(L, 1);
	}*/
	lua_pop(L, 1);

	lua_close(L);
	return QString::fromStdString(url);
}
} // namespace MyLua
