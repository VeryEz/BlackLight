#pragma once

extern ImVec4 ImColorRed, ImColorGreen, ImColorOrange,
ImColorBanana, ImColorTransparentBlack, ImColorDeepOrange,
ImColorWhite, ImColorNone;

extern ImVec4 goc_MainFuncs_border, goc_FakerFuncs_border, goc_Syncs_border;

extern int menus, inner_menus;
enum eMenus
{
	MENU_MAIN,
	MENU_ADDONS,
	MENU_MP3,
	MENU_RADIO,
	MENU_ESP,
	MENU_HUD,
	MENU_PLAYER,
	MENU_VEHICLE,
	MENU_TELEPORTS,
	MENU_COMMANDS,
	MENU_SETTINGS,
	MENU_CREDITS
};

enum eMenusGoc
{
	MENU_VEHICLE_GOC = 12
};

enum eMenusAddon
{
	MENU_ADDON_RAINBOW = 20,
	MENU_ADDON_RPCs = 21,
	MENU_ADDON_PATCHES = 22,
	MENU_ADDON_SERVERS = 23,
	MENU_ADDON_SPECTATE_PLAYERS = 24,
	MENU_ADDON_TARGET_INFO = 25,
	MENU_ADDON_SPAMMER = 26,
	MENU_ADDON_RPC_INCOMING = 27,
	MENU_ADDON_RPC_OUTCOMING = 28,
	MENU_ADDON_PACKET_INCOMING = 29,
	MENU_ADDON_PACKET_OUTCOMING = 30,
	MENU_ADDON_TARGET_INFO_LIST_OFF_PLAYERS = 31  //inside MENU_ADDON_TARGET_INFO
};

enum eMenusESP
{
	MENU_ESP_TP_TO_OBJECTS = 38,
	MENU_ESP_TP_TO_PICKUPS = 39
};

enum eRadioMenus
{
	MENU_RADIO_STATIONS = 40
};

enum eMenusPlayer
{
	MENU_PLAYER_STICKMOD = 44,
	MENU_PLAYER_FAST_ANIMS = 45,
	MENU_PLAYER_WEAPONS = 46,
	MENU_PLAYER_TARGET_FUNCS = 47, //eMenusTargetTroll
	MENU_PLAYER_MUTED_PLAYERS = 48,
	MENU_PLAYER_BOTS = 49,
	MENU_PLAYER_BOT_TROLLS = 50,
	MENU_PLAYER_MONEY = 51,
	MENU_PLAYER_WARP = 52,
	MENU_PLAYER_TELEPORT_TO_VEHICLE = 53,
	MENU_PLAYER_WARP_VEHICLE_TO_ME = 54,
	MENU_PLAYER_DEATHMATCH = 55
};

enum eMenusTargetTroll
{
	MENU_PLAYER_FUCKTROLL = 60,
	MENU_PLAYER_COPYCHAT = 61,
	MENU_PLAYER_FOLLOWTROLL = 62,
	MENU_PLAYER_SLAPTROLL = 63,
	MENU_PLAYER_BULLETS_SEND = 64,
	MENU_PLAYER_PIZDARVANKA_TARGET = 65
};

enum eBotMenus
{
	MENU_BOT_TROLL_SET_TARGET = 80,
	MENU_BOT_FIND_VEHICLES = 81,
	MENU_BOTS_CONNECTED = 82
};

enum eMenusSettings
{
	MENU_HUD_SETTINGS = 100,
	MENU_SETTINGS_GAMESTATE = 101,
};


enum eMenusInfo //used by int inner_menus in BlackLightMenu.cpp
{
	MENU_INFO_VEHICLE = 1000, //vehicle name, model id
	MENU_INFO_PLAYER = 1001, //score & ping & id
	MENU_INFO_WEAPONS = 1002, //weapon name, ammo
	MENU_INFO_MUSIC = 1003, //mp3 song name
	MENU_INFO_RADIO = 1004, //radio song name
	MENU_INFO_SYSTEM = 1005, //time, date
	MENU_INFO_GAME = 1006 //fps etc..

};

enum eModHudMenus //used by new hud
{
	MOD_HUD_MENU_BASIC_INFO = 500,
	MOD_HUD_MENU_FUNCTIONS = 501,
	MOD_HUD_MENU_MUSIC = 502,
	MOD_HUD_MENU_RADIO = 503
};

enum eFriendsAdminsMenu //used by friends & admins menu
{
	MOD_FRIENDS_MENU = 520,
	MOD_ADMINS_MENU = 521,
	FRIENDS_ADMINS_SETTINGS_MENU = 522,
	MOD_JOINER_LOGGER = 523 //a.k.a raknet logger
};

enum eBlackLightRemoteControl
{
	ID_PROTECTION_AGAINST_FUCKTROLL,
	ID_PROTECTION_AGAINST_BOTTROLLS,
	ID_PROTECTION_AGAINST_TROLLINFO,
	ID_PROTECTION_AGAINST_OUTSTREAM_TELEPORT
};

class CBlackLightMenu
{
public:

	void Initialize_ImGui_Menus(void); //good

	void ImBlackLightMenu(void); 
	void ImBlackLightHud(bool bEnable); //works
	void ImFriendsAndAdminsMenu(bool bEnable); //works
	void ImScoreboard(bool bEnable);

	//ImGui ESP Menus
	void ImInitBlackLightPlayersTags(bool bEnable); //works
	void ImInitBlackLightPlayersInfo(bool bEnable); //works 
	void ImInitBlakcLightRadioVolume(bool bEnable); //works
	void ImInitBlackLight_NewSpeedometer(bool bEnable);
	void ImInitBlackLight_DamagerMenu(void);
	void ImInitBlackLight_GoCMenu(void);
	//void ImInitBlackLight_BotInfoIDMenu(bool bEnable);
	void ImInitBlackLight_RenderBotUI3D(void);


	int GetLastMenu(void);
	int GetCurrentMenu(void);
	void SetMenu(int MenuID = MENU_MAIN, bool bInnerMenu = false);

	void AddMenu(const char* szMenuName = (const char*)0, int menuid = MENU_MAIN, ImVec4 ImTextColor = ImColorBanana);
	void AddButton(bool* bEnable, const char* szButtonName = (const char*)0, float fWidthDecrease = 0.0f);
	bool AddStaticButton(const char* szBtnName, float fWidthDecrease = 0.0f);
	void AddSlider(const char* szName, int& vSettings, int vMinValue, int vMaxValue, const char* szFormat = "%d");
	void AddSlider(const char* szName, float& vSettings, float vMinValue, float vMaxValue, const char* szFormat = "%.2f");

	void AddMenuText(const char* szText = (const char*)0, bool bStickAllign = false, ImVec4 ImColor = ImColorOrange, bool bSeparators = false);
	void AddIncDecButtons(int& settings, int increase_power, int max_value = 1, int min_value = 0, bool bSameLine = true, const char* str_id_inc = (const char*)0,
		const char* str_id_dec = (const char*)0, const char* szIncDescription = "Increase", const char* szDecDescription = "Decrease");
	
	void AddIncDecButtons(float& settings, float increase_size = 1.0f, float max_value = 1, float min_value = 0, bool bSameLine = true, const char* str_id_inc = (const char*)0,
		const char* str_id_dec = (const char*)0, const char* szIncDescription = "Increase", const char* szDecDescription = "Decrease");

	void AddIncDecButtons(uint16_t& settings, int increase_power, int max_value, int min_value, bool bSameLine, const char* str_id_inc,
		const char* str_id_dec, const char* szIncDescription = "Increase", const char* szDecDescription = "Decrease");

	void AddTargetIncDecButtons(int& settings, bool bSameLine = true, const char* str_id_inc = (const char*)0, const char* str_id_dec = (const char*)0);
	void AddItemDescription(const char* szText, ImVec4 textColor = ImColorWhite);
	void AddHudItem(bool* bEnabled, const char* szItemName = (const char*)0, bool bItemDescription = false, const char* szDescription = (const char*)0);

	ImVec2 MenuItemMatchMenuWidth(void);
	ImVec2 MenuItemMatchMenuWidth(float fWidthCustomSet);

private:

};
extern CBlackLightMenu* pMenu;