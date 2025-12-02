#pragma once
//_=Gigant=_ was here 
#define INVALID					0xFFFF

#define MAX_SPRINT_ENERGY 3147.0f
#define FUNC_SA_DEFAULT_GRAVITY 0.0080
#define DEFAULT_GAME_SPEED 1.0f
#define DEFAULT_PED_ROTATION_SPEED 7.5f
#define SAMP_MAX_SCREENSHOTS 1000
#define MAX_DISTANCE_ESP_DRAW 300.0f
#define BULLET_HIT_TYPE_PLAYER          1
#define BULLET_HIT_TYPE_VEHICLE         2

#define ANIM_FLAG_ID_MOSTLY_USED 4356

#define DEATH_REASON_VEHICLE_RAM_BY 49
#define DEATH_REASON_HELICOPTER_BLADE 50
#define DEATH_REASON_EXPLOSION 51
#define DEATH_REASON_DROWN 53
#define DEATH_REASON_ACCIDENT 54
#define DEATH_REASON_USELESS1 200
#define DEATH_REASON_USELESS2 201
#define DEATH_REASON_SUICIDE 255

#define PLAYER_FLY_ANIM_BOSS 1
#define PLAYER_FLY_ANIM_CYCLE 2
#define PLAYER_FLY_ANIM_SWIM 3

#define CLICK_LMB 0//left mouse
#define CLICK_RMB 1 //right mouse
#define CLICK_MMB 2 //middle mouse

//#define FILE_MODE_WRITE std::fstream::out | std::fstream::app
//#define FILE_MODE_READ std::fstream::in

//#define AFK_STATE 2 //g_Players->pRemotePlayer[playerid]->pPlayerData->iAFKState == AFK_STATE
//#define IS_PLAYER_AFK(id) g_Players->pRemotePlayer[id]->pPlayerData->iAFKState == AFK_STATE
//#define IS_MAIN_MENU_ACTIVE  pSampMulti->IsMenuActive(NewFunctions->Menu.bMain_Menu)
//#define IS_PLAYER_DEAD(id) g_Players->pRemotePlayer[id]->pPlayerData->pSAMP_Actor->pGTA_Ped->state == ACTOR_STATE_DEAD
//#define IS_PLAYER_DYING(id) g_Players->pRemotePlayer[id]->pPlayerData->pSAMP_Actor->pGTA_Ped->state == ACTOR_STATE_DYING
//#define IS_OUR_PLAYER_DEAD g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->state == ACTOR_STATE_DEAD
#define IS_CHEAT_PANIC_ACTIVE cheat_state->_generic.cheat_panic_enabled

#define COLOR_WHITE(a) D3DCOLOR_ARGB(a, 255, 255, 255)
#define COLOR_RED(a) D3DCOLOR_ARGB(a, 216, 0, 0)
#define COLOR_GREEN(a) D3DCOLOR_ARGB(a, 15, 216, 0)
#define COLOR_ORANGE(a) D3DCOLOR_ARGB(a, 255, 114, 0)
#define COLOR_BLACK(a) D3DCOLOR_ARGB(a, 31, 31, 31)
#define COLOR_BLUE(a) D3DCOLOR_ARGB(a, 3, 106, 223)
#define COLOR_BLUE_V2(a) D3DCOLOR_ARGB(a, 40, 173, 206)
#define COLOR_BLACK_V2(a) D3DCOLOR_ARGB(a, 0, 0, 0)
#define COLOR_YELLOW(a) D3DCOLOR_ARGB(a, 240, 209, 0)
#define COLOR_PURPLE(a) D3DCOLOR_ARGB(a, 76, 4, 122)
#define COLOR_TEGET_BLUE(a) D3DCOLOR_ARGB(a, 11, 1, 112)
#define COLOR_PINK(a) D3DCOLOR_ARGB(a, 223, 0, 220)
#define COLOR_BROWN(a) D3DCOLOR_ARGB(a, 124, 77, 31)
#define COLOR_DARK_GREEN(a) D3DCOLOR_ARGB(a, 0, 124, 44)
#define COLOR_DARK_RED(a) D3DCOLOR_ARGB(a, 122, 0, 0)

//custom rainbow color ImGui::ColorEdit3
extern ImVec4 custom_color_car
, custom_color_skin
, custom_color_rb_sky
, custom_color_radar
, custom_color_nicknames
, custom_color_hud
, custom_color_killList
, custom_color_carLights
, custom_color_crosshair
, custom_color_chat;

extern ImVec4 CC_EXTRAS_PlayersTracers, CC_EXTRAS_PlayersBones,
CC_EXTRAS_PlayersBox, CC_EXTRAS_VehicleTracers;

void StartUpFuncs(void); //defined in cheat_samp.cpp init commands
void KeySystem(void);

class CNewMods
{
public:

	void BlackLight_InitMods(void); //initialized in cheat.cpp
	void BlackLight_InitESPFuncs(void); //initialized in proxyIDirect3DDecive9.cpp
	void BlackLight_InitRainbowFuncs(void);  //initialized in proxyIDirect3DDecive9.cpp
	void BlackLight_RegularFuncs(void);

	void SetSprintEnergy(bool bEnable, float set_sprint_eng);
	void ActorNoFall(bool bEnable);
	void UIFIslands(bool bEnable);
	void Slowmotion(bool bEnable, bool bEnableViaKey);
	void CameraOverview(bool bEnable, float fCamOverview);
	void VehicleDisableWheels(bool bEnable);
	void VehicleDisableFrame(bool bEnable);
	void FastRotation(bool bEnable, float fRotationSpeed);
	void CrazyRoter(bool bEnable);
	void MouseDrive(bool bEnable);
	void NoSpread(bool bEnable);
	void RapidFire(bool bEnable);
	void AntiStun(bool bEnable);
	void NoReload(bool bEnable);
	void FastCrosshair(bool bEnable);
	void AutoShoot(bool bEnable);
	void AutoShootWheel(bool bEnable);
	void MoveAllNearbyVehicles_MASS(bool bEnable);
    void SampFastConnect(bool bEnable); //standalone func loaded into cheat.cpp
	void DisableWaterWaves(bool bEnable);
	void CustomTimeWeatherSet(bool bEnableCustomTime, bool bEnableCustomWeather);
	void FastAnims(void);
	void HudThread(void);
	void PlayerFuckTroll(void);
	void Slapper(void);
	void TargetSendBullets(void);
	void RPCs(void);
	void AutoScroll(bool bEnable);
	void AutoCBug(bool bEnable);
	void PickVehicle(bool bEnable);
	void ComponentFlood(bool bEnable);
	void WheelsStatusPulse(bool bEnable);
	void PlayerPulsators(void); //health, armor, weapons
	void BlinkCarColors(bool bEnable);
	void WheelsPulsator(bool bEnable);
	void VehicleOnFire(bool bEnable);
	void PizdarvankaTargetTroll(void);
	void PizdarvankaPlayer(bool bEnable);
	void PizdarvankaVehicle(bool bEnable);
	void FakeKillFlood(bool bEnable);
	void FakeBulletsFlood(bool bEnable);
	void SkinChanger(bool bEnable);
	void VehicleWheelsVisibilityPulsator(bool bEnable);
	void CarHardFlip(bool bEnable);
	void NoStun(bool bEnable);
	void FreezePlayers(bool bEnable);
	void FollowTroll(void);
	void VehiclesFugga(void);
	void RecolorVehicles(void);
	void Dronemode(void);
	void BE_Surfer(void);
	void BE_Flysurf();
	//Rainbow
	void RainbowVehicle(bool bEnable);
	void RainbowSky(bool bEnable);
	void RainbowChat(bool bEnable);
	void RainbowHud(bool bEnable);
	void RainbowCrosshair(bool bEnable);
	void RainbowCarLights(bool bEnable);
	void RainbowDeathList(bool bEnable);
	void RainbowRadar(bool bEnable);
	void RainbowNicknames(bool bEnable);

	//esp
	void renderPlayer3DBox(bool bEnable);
	void renderVehicleWheelESP(bool bEnable);
	void renderDriverInfo(bool bEnable);
	void renderShowDoorsStatus(bool bEnable);
	void renderPlayersTracers(bool bEnable);
	void renderPlayerBones(bool bEnable);
	void renderPlayerBox(bool bEnable);
	void renderHelicopterTracers(bool bEnable);
	void renderVehicleTracers(bool bEnable);
	void renderPlayers2DInfo(bool bEnable);
	void renderVehicleInfo(bool bEnable); //2D Vehicles Info
	void renderPolicePedsDetector(bool bEnable);
	void renderPoliceVehiclesDetector(bool bEnable);
	void renderOutStreamPlayers(bool bEnable);
	void renderBulletTracers(bool bEnable);
	void renderTrailerTracers(bool bEnable);
	void renderWheelsInfo(bool bEnable);

	//other
	void AdminsAutoUpdate(void);
	void FriendsAutoUpdate(void);
	void MutedPlayerListUpdate(void);
	void AntiLoading(void);
	void GameShutdownOnTime(void);

private:

};
extern class CNewMods* pBlackLightMods;

int UnOccupiedDataHook(stUnoccupiedData* data);
int  PassengerDataHook(stPassengerData* data);
int  OnFootDataHook(stOnFootData* data);
int  IncarDataHook(stInCarData* data);
int  AimDataDataHook(stAimData* data);
int  BulletDataHook(stBulletData* data);


