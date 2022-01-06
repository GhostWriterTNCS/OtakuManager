# How to add a website (Lua)

Create a new .lua module and put it in the *lua* folder.

## Parameters

###  `homepage`
The page OMA will open when you press on the website title.

### `group`
Where to put the module in the *Preferences* window.

### `hasStreaming` *default True*
If the website can show the *Streaming* button for the episodes.

### `hasDownload` *default True*
If the website can show the *Download* button for the episodes.

### `hasTorrent` *default False*
If the website can show the *Torrent* button for the episodes.

### `hasMagnet` *default False*
If the website can show the *Magnet* button for the episodes.

### `hasInfo` *default True*
If the website can show the *Anime info* button for the episodes.

## Methods

### `getEpisodes()`
Used to get the list of episodes.

Use `ep = Espidode()` to initialize the episode object.

Use `ep:SetName(value)` to set the name that will be shown.

Use `ep:SetUrl(value)` to set the url that will be sent to `goToEpisode(episodeUrl, buttonType)`.

Use `ep:SetDownload(value)` to set a different url for the *Download* button (optional).

Use `ep:SetMagnet(value)` to set a different url for the *Magnet* button (optional).

Use `ep:HasDownload(value)` to specify if the episode can show the *Download* button (optional, *default True*).

### `goToEpisode(episodeUrl, buttonType)` *optional*
Used to get the open the proper url from the various buttons.

If not defined, OMA will open the url provided in `getEpisodes()` `ep:SetUrl(value)`.

## Available methods
When creating a lua module, you can call the following methods from OMA.

### `UrlToString(url)`
Returns the HTML of the page at `url`.

### `UrlToStringJS(url)`
Returns the HTML of the page at `url` after the JavaScripts have been executed (slower).

### `HtmlDoc(html, encoding)`
Returns an `HtmlDoc` object that you can use in `GetString` and `GetAllStrings`.

### `GetString(htmlDoc, xpath)`
Returns the first content of the first node that satisfies the `xpath`.

### `GetAllStrings(htmlDoc, xpath)`
Returns a list (lua table) of all the content of the nodes that satisfy the `xpath`.

### `Substring(str, start, end)`
Returns the substring of `str` between `start`and `end`.

### `SubstringFromEnd(str, start, end)`
Returns the substring of `str` between `start`and `end`. It searches for `start` and `end` from the end of `str`.

### `Split(str, separator)`
Returns a list (lua table) of strings.

### `print(value)`
Prints `value` in the OMA console.

`value` can be a lua table.
