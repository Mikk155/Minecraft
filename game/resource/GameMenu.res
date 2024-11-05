"GameMenu"
{
	"1"
	{
		"label" "#GameUI_GameMenu_ResumeGame"
		"command" "ResumeGame"
		"OnlyInGame" "1"
	}
	"2"
	{
		"label" "#GameUI_GameMenu_Disconnect"
		"command" "Disconnect"
		"OnlyInGame" "1"
		"notsingle" "1"
	}
	"3"
	{
		"label" "Test maps"
		"command" "engine disconnect; maxplayers 1; map test_sky"
	}
	"4"
	{
		"label" "#GameUI_GameMenu_LeaveGame"
		"command" "Disconnect"
		"OnlyInGame" "1"
		"notmulti" "1"
	}
	"5"
	{
		"label" "#GameUI_GameMenu_PlayerList"
		"command" "OpenPlayerListDialog"
		"OnlyInGame" "1"
		"notsingle" "1"
	}
	"6"
	{
		"label" ""
		"command" ""
		"OnlyInGame" "1"
	}
//	"7"
//	"8"
//	"9"
	"10"
	{
		"label" ""
		"command" ""
		"notmulti" "1"
	}
	"11"
	{
		"label" "#GameUI_GameMenu_FindServers"
		"command" "OpenServerBrowser"
		"notsingle" "1"
	}
	"12"
	{
		"label" "#GameUI_GameMenu_CreateServer"
		"command" "OpenCreateMultiplayerGameDialog"
		"notsingle" "1"
	}
//	"13"
	"14"
	{
		"label" ""
		"command" ""
		"notsingle" "1"
	}
	//"15"
	"16"
	{
		"label" "#GameUI_GameMenu_Options"
		"command" "OpenOptionsDialog"
	}
	"17"
	{
		"label" "#GameUI_GameMenu_Quit"
		"command" "Quit"
	}
}
