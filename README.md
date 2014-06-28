AutoUpdater
===========
It is a simple automatic updater and file synronizer for game servers.
- cfg/apps.cfg - put the apps here you want to keep updated. The format is json.


Example:
========
"tf2_srv": // Custom name (The download directory name will be the same)
{
	"device":	"steamcmd", // The updater you want to use
	"synclist":	"cfg/tf2_servers.cfg" // File that contains the list of the directories where to sync the files
	// Updater specific variables
	"appid":	"232250", // TF2 Server appid for download
	"api_appid":	"440", // TF2 Client appid to check app version
	"login":	"anonymous", // Steamcmd login name and password
	"app_set_config":	"0", // its for gold source games
	"mod":		"0" // again for gold source games
}

Main Features
========
- Dynamic libraries to run game specific updaters
- Always up to date server files, easy to create new servers by copying it to a new location
- Only updates the changed files (Less IO tasks)
- Download is proceed only once / game (Less network usage)

Support
========
- SteamCMD


Todo
========
- Fix Socket/HTTP delay
- Add command line support for manual requests
- Update notifications & custom script execution when update is starting, in progress and finished.
- Give me feature requests if you want something to be added.

