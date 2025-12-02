/*

	PROJECT:		mod_sa
	LICENSE:		See LICENSE in the top level directory
	COPYRIGHT:		Copyright we_sux, BlastHack

	mod_sa is available from https://github.com/BlastHackNet/mod_s0beit_sa/

	mod_sa is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	mod_sa is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with mod_sa.  If not, see <http://www.gnu.org/licenses/>.

*/

/*
//R3
#define SAMP_CHAT_INFO_OFFSET                        0x26E8C8
#define SAMP_CHAT_INPUT_INFO_OFFSET                    0x26E8CC
#define SAMP_KILL_INFO_OFFSET                        0x26E8D0
#define SAMP_INFO_OFFSET                            0x26E8DC
#define SAMP_MISC_INFO                                0x26E8F4
#define SAMP_SCOREBOARD_INFO                        0x26E894
#define SAMP_COLOR_OFFSET                            0x151578

#define SAMP_FUNC_ADDTOCHATWND                        0x679F0
#define SAMP_FUNC_REQUEST_SPAWN                        0x3A20
#define SAMP_FUNC_REQUEST_CLASS                        0x56E0
#define SAMP_FUNC_DEATH                                0x5620
#define SAMP_FUNC_SAY                                0x5820
#define SAMP_FUNC_SENDCMD                            0x69190
#define SAMP_FUNC_ADDCLIENTCMD                        0x69000
#define SAMP_FUNC_RESTARTGAME                        0xA1E0
#define SAMP_FUNC_TOGGLECURSOR                        0x9FFE0
#define SAMP_FUNC_CURSORUNLOCKACTORCAM                0x9FEC0
#define SAMP_FUNC_UPDATESCOREBOARDDATA                0x8BA0
#define SAMP_FUNC_CNETGAMEDESTRUCTOR                0x9510
#define SAMP_FUNC_SENDINTERIOR                        0x5780
#define SAMP_FUNC_DISABLE_SCOREBOARD                0x6E270

#define SAMP_HOOKENTER_STATECHANGE                    0x8190

#define SAMP_PATCH_SCOREBOARDTOGGLEON                0x6E960
*/


/* R4
#define SAMP_DIALOG_INFO_OFFSET           0x26E9C8 //additional

//main
#define SAMP_CHAT_INFO_OFFSET              0x26E9F8
#define SAMP_CHAT_INPUT_INFO_OFFSET  0x26E9FC
#define SAMP_KILL_INFO_OFFSET                0x26EA00
#define SAMP_INFO_OFFSET                          0x26EA0C
#define SAMP_MISC_INFO                              0x26EA24
#define SAMP_SCOREBOARD_INFO              0x26E9C4
//missing color offset

//missing SAMP_DRAWING_DISTANCE
//missing SAMP_DRAWTEXTLABELS
//missing SAMP_ONFOOTSENDRATE
//missing SAMP_INCARSENDRATE
//missing SAMP_AIMSENDRATE

#define SAMP_FUNC_ADDTOCHATWND        0x680B0
#define SAMP_FUNC_SPAWN                         0x3BE0
//missing SAMP_FUNC_REQUEST_SPAWN
//missing SAMP_FUNC_REQUEST_CLASS
//missing SAMP_FUNC_DEATH	
#define SAMP_FUNC_SETSPECIALACTION    0x30F0
#define SAMP_FUNC_SAY                                0x5A00
#define SAMP_FUNC_SENDCMD                     0x698C0
#define SAMP_FUNC_ADDCLIENTCMD           0x69730
//missing SAMP_FUNC_NAMECHANGE
//missing SAMP_FUNC_RESTARTGAME	
//missing SAMP_FUNC_ENCRYPT_PORT
//missing SAMP_FUNC_TOGGLECURSOR
//missing SAMP_FUNC_CURSORUNLOCKACTORCAM
#define SAMP_FUNC_UPDATESCOREBOARDDATA 0x8F00
//missing SAMP_FUNC_CNETGAMEDESTRUCTOR
//missing SAMP_FUNC_SENDINTERIOR
//missing SAMP_FUNC_DISABLE_SCOREBOARD

//missing SAMP_HOOKENTER_STATECHANGE				
#define SAMP_HOOKENTER_HANDLE_RPC   0x3AEDD
#define SAMP_HOOKENTER_HANDLE_RPC2 0x3AE69
//missing SAMP_HOOKENTER_CNETGAME_DESTR				
//missing SAMP_HOOKENTER_CNETGAME_DESTR2			
//missing SAMP_HOOKEXIT_ANTICARJACK				
//missing SAMP_HOOKEXIT_HANDLE_RPC			
//missing SAMP_HOOKEXIT_HANDLE_RPC2			

//missing SAMP_PATCH_DISABLE_NAMETAGS					
//missing SAMP_PATCH_DISABLE_NAMETAGS_HP				
//missing SAMP_PATCH_SKIPSENDINTERIOR				
//missing SAMP_PATCH_SCOREBOARDTOGGLEON			
//missing SAMP_PATCH_SCOREBOARDTOGGLEONKEYLOCK		
//missing SAMP_PATCH_CHATINPUTADJUST_Y				
//missing SAMP_PATCH_CHATINPUTADJUST_X			

*/

#ifdef SAMP_R1
#define SAMP_CHAT_INFO_OFFSET						0x21A0E4
#define SAMP_CHAT_INPUT_INFO_OFFSET					0x21A0E8
#define SAMP_KILL_INFO_OFFSET						0x21A0EC
#define SAMP_INFO_OFFSET							0x21A0F8
#define SAMP_MISC_INFO								0x21A10C
#define SAMP_SCOREBOARD_INFO						0x21A0B4
#define SAMP_COLOR_OFFSET							0x216378
#define SAMP_DRAWING_DISTANCE						0xD3DCC

#define SAMP_DRAWTEXTLABELS							0x71471
#define SAMP_ONFOOTSENDRATE							0xEC0A8
#define SAMP_INCARSENDRATE							0xEC0AC
#define SAMP_AIMSENDRATE							0xEC0B0

#define SAMP_FUNC_ADDTOCHATWND						0x64010
#define SAMP_FUNC_SPAWN								0x3AD0
#define SAMP_FUNC_REQUEST_SPAWN						0x3EC0
#define SAMP_FUNC_REQUEST_CLASS						0x56A0
#define SAMP_FUNC_DEATH								0x55E0
#define SAMP_FUNC_SETSPECIALACTION					0x30C0
#define SAMP_FUNC_SAY								0x57F0
#define SAMP_FUNC_SENDCMD							0x65C60
#define SAMP_FUNC_ADDCLIENTCMD						0x65AD0
#define SAMP_FUNC_NAMECHANGE						0xB290
#define SAMP_FUNC_RESTARTGAME						0xA060
#define SAMP_FUNC_ENCRYPT_PORT						0x1BC20
#define SAMP_FUNC_TOGGLECURSOR						0x9BD30
#define SAMP_FUNC_CURSORUNLOCKACTORCAM				0x9BC10
#define SAMP_FUNC_UPDATESCOREBOARDDATA				0x8A10
#define SAMP_FUNC_CNETGAMEDESTRUCTOR				0x9380
#define SAMP_FUNC_SENDINTERIOR						0x5740
#define SAMP_FUNC_DISABLE_SCOREBOARD				0x6A320

#define SAMP_HOOKENTER_STATECHANGE					0x12B82
#define SAMP_HOOKENTER_HANDLE_RPC					0x3743D
#define SAMP_HOOKENTER_HANDLE_RPC2					0x373C9
#define SAMP_HOOKENTER_CNETGAME_DESTR				0xB2893
#define SAMP_HOOKENTER_CNETGAME_DESTR2				0xB3A32
#define SAMP_HOOKEXIT_ANTICARJACK					0x12B93
#define SAMP_HOOKEXIT_HANDLE_RPC					0x37443
#define SAMP_HOOKEXIT_HANDLE_RPC2					0x37451

#define SAMP_PATCH_DISABLE_NAMETAGS					0x70D40
#define SAMP_PATCH_DISABLE_NAMETAGS_HP				0x6FC30
#define SAMP_PATCH_SKIPSENDINTERIOR					0x7668
#define SAMP_PATCH_SCOREBOARDTOGGLEON				0x6AA10
#define SAMP_PATCH_SCOREBOARDTOGGLEONKEYLOCK		0x6AD30
#define SAMP_PATCH_CHATINPUTADJUST_Y				0x63FE6
#define SAMP_PATCH_CHATINPUTADJUST_X				0x65855
#endif

#ifdef SAMP_R5
#define SAMP_CHAT_INFO_OFFSET						0x26EB80
#define SAMP_CHAT_INPUT_INFO_OFFSET					0x26EB84
#define SAMP_KILL_INFO_OFFSET						0x26EB88
#define SAMP_INFO_OFFSET							0x26EB94
#define SAMP_MISC_INFO								0x26EBAC
#define SAMP_SCOREBOARD_INFO						0x26EB4C
#define SAMP_COLOR_OFFSET							0x151828
#define SAMP_DRAWING_DISTANCE						0xE60A0

#define SAMP_DRAWTEXTLABELS							0x75A70
#define SAMP_ONFOOTSENDRATE							0xFE0A8
#define SAMP_INCARSENDRATE							0xFE0AC
#define SAMP_AIMSENDRATE							0xFE0B0

#define SAMP_FUNC_ADDTOCHATWND						0x67BE0
#define SAMP_FUNC_SPAWN								0x3C20
#define SAMP_FUNC_REQUEST_SPAWN						0x4060
#define SAMP_FUNC_REQUEST_CLASS						0x58D0
#define SAMP_FUNC_DEATH								0x5810
#define SAMP_FUNC_SETSPECIALACTION					0x30F0
#define SAMP_FUNC_SAY								0x5A10
#define SAMP_FUNC_SENDCMD							0x69900
#define SAMP_FUNC_ADDCLIENTCMD						0x69770
#define SAMP_FUNC_NAMECHANGE						0xB6C0
#define SAMP_FUNC_RESTARTGAME						0xA540
#define SAMP_FUNC_ENCRYPT_PORT						0x1F710
#define SAMP_FUNC_TOGGLECURSOR						0xA06F0
#define SAMP_FUNC_CURSORUNLOCKACTORCAM				0xA05D0
#define SAMP_FUNC_UPDATESCOREBOARDDATA				0x8F10
#define SAMP_FUNC_CNETGAMEDESTRUCTOR				0x9880
#define SAMP_FUNC_SENDINTERIOR						0x5970
#define SAMP_FUNC_DISABLE_SCOREBOARD				0x6E9E0

#define SAMP_HOOKENTER_STATECHANGE					0x1630D
#define SAMP_HOOKENTER_HANDLE_RPC					0x3AF2D
#define SAMP_HOOKENTER_HANDLE_RPC2					0x3AEB9
#define SAMP_HOOKENTER_CNETGAME_DESTR				0xC3E63
#define SAMP_HOOKENTER_CNETGAME_DESTR2				0xC5076
#define SAMP_HOOKEXIT_ANTICARJACK					0x1631E
#define SAMP_HOOKEXIT_HANDLE_RPC					0x3AF33
#define SAMP_HOOKEXIT_HANDLE_RPC2					0x3AF41

#define SAMP_PATCH_DISABLE_NAMETAGS					0x75330
#define SAMP_PATCH_DISABLE_NAMETAGS_HP				0x74210
#define SAMP_PATCH_SKIPSENDINTERIOR					0x797F
#define SAMP_PATCH_SCOREBOARDTOGGLEON				0x6F0B0
#define SAMP_PATCH_SCOREBOARDTOGGLEONKEYLOCK		0x6F3D0
#define SAMP_PATCH_CHATINPUTADJUST_Y				0x67BB6
#define SAMP_PATCH_CHATINPUTADJUST_X				0x694F5
#endif

enum DialogStyle
{
	DIALOG_STYLE_MSGBOX,
	DIALOG_STYLE_INPUT,
	DIALOG_STYLE_LIST,
	DIALOG_STYLE_PASSWORD,
#ifdef SAMP_R5
	DIALOG_STYLE_TABLIST,
	DIALOG_STYLE_TABLIST_HEADERS
#endif
};

enum Limits
{
	SAMP_MAX_ACTORS = 1000,
	SAMP_MAX_PLAYERS = 1004,
	SAMP_MAX_VEHICLES = 2000,
	SAMP_MAX_PICKUPS = 4096,
#ifdef SAMP_R1
	SAMP_MAX_OBJECTS = 1000,
#endif
#ifdef SAMP_R5
	SAMP_MAX_OBJECTS = 2100,
#endif
	SAMP_MAX_GANGZONES = 1024,
	SAMP_MAX_3DTEXTS = 2048,
	SAMP_MAX_TEXTDRAWS = 2048,
	SAMP_MAX_PLAYERTEXTDRAWS = 256,
	SAMP_MAX_CLIENTCMDS = 144,
	SAMP_MAX_MENUS = 128,
	SAMP_MAX_PLAYER_NAME = 24,
	SAMP_ALLOWED_PLAYER_NAME_LENGTH = 20,
	SAMP_MAX_MAPICONS = 100,
};

enum ChatMessageType
{
	CHAT_TYPE_NONE = 0,
	CHAT_TYPE_CHAT = 2,
	CHAT_TYPE_INFO = 4,
	CHAT_TYPE_DEBUG = 8
};

enum ChatDisplayMode
{
	CHAT_WINDOW_MODE_OFF = 0,
	CHAT_WINDOW_MODE_LIGHT = 1,
	CHAT_WINDOW_MODE_FULL = 2
};

enum Gamestate
{
	GAMESTATE_WAIT_CONNECT = 9,
	GAMESTATE_CONNECTING = 13,
	GAMESTATE_AWAIT_JOIN = 15,
	GAMESTATE_CONNECTED = 14,
	GAMESTATE_RESTARTING = 18
};

enum PlayerState
{
	PLAYER_STATE_NONE = 0,
	PLAYER_STATE_ONFOOT = 17,
	PLAYER_STATE_PASSENGER,
	PLAYER_STATE_DRIVER,
	PLAYER_STATE_WASTED = 32,
	PLAYER_STATE_SPAWNED
};

enum MarkersMode
{
	PLAYER_MARKERS_MODE_OFF,
	PLAYER_MARKERS_MODE_GLOBAL,
	PLAYER_MARKERS_MODE_STREAMED,
};

enum SpecialAction
{
	SPECIAL_ACTION_NONE,
	SPECIAL_ACTION_DUCK,
	SPECIAL_ACTION_USEJETPACK,
	SPECIAL_ACTION_ENTER_VEHICLE,
	SPECIAL_ACTION_EXIT_VEHICLE,
	SPECIAL_ACTION_DANCE1,
	SPECIAL_ACTION_DANCE2,
	SPECIAL_ACTION_DANCE3,
	SPECIAL_ACTION_DANCE4,
	SPECIAL_ACTION_HANDSUP,
	SPECIAL_ACTION_USECELLPHONE,
	SPECIAL_ACTION_SITTING,
	SPECIAL_ACTION_STOPUSECELLPHONE,
	SPECIAL_ACTION_DRINK_BEER = 20,
	SPECIAL_ACTION_SMOKE_CIGGY,
	SPECIAL_ACTION_DRINK_WINE,
	SPECIAL_ACTION_DRINK_SPRUNK,
	SPECIAL_ACTION_CUFFED,
	SPECIAL_ACTION_CARRY,
};


//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// SAMP STRUCTURES ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#ifdef SAMP_R1
#pragma pack(push, 1)
struct stSAMPPools
{
	struct stActorPool* pActor;
	struct stObjectPool* pObject;
	struct stGangzonePool* pGangzone;
	struct stTextLabelPool* pText3D;
	struct stTextdrawPool* pTextdraw;
	void* pPlayerLabels;
	struct stPlayerPool* pPlayer;
	struct stVehiclePool* pVehicle;
	struct stPickupPool* pPickup;
};

struct stSAMP
{
	void* pUnk0;
	struct stServerInfo* pServerInfo;
	uint8_t					byteSpace[24];
	char					szIP[257];
	char					szHostname[259];
	bool					bNametagStatus; // changes by /nametagstatus
	uint32_t				ulPort;
	uint32_t				ulMapIcons[100];
	int						iLanMode;
	int						iGameState;
	uint32_t				ulConnectTick;
	struct stServerPresets* pSettings;
	void* pRakClientInterface;
	struct stSAMPPools* pPools;
};

struct stServerInfo {
	uint32_t 			uiIP;
	uint16_t 			usPort;

	// ...

};

struct stServerPresets
{
	uint8_t byteCJWalk;
	int m_iDeathDropMoney;
	float	fWorldBoundaries[4];
	bool m_bAllowWeapons;
	float	fGravity;
	uint8_t byteDisableInteriorEnterExits;
	uint32_t ulVehicleFriendlyFire;
	bool m_byteHoldTime;
	bool m_bInstagib;
	bool m_bZoneNames;
	bool m_byteFriendlyFire;
	int		iClassesAvailable;
	float	fNameTagsDistance;
	bool m_bManualVehicleEngineAndLight;
	uint8_t byteWorldTime_Hour;
	uint8_t byteWorldTime_Minute;
	uint8_t byteWeather;
	uint8_t byteNoNametagsBehindWalls;
	int iPlayerMarkersMode;
	float	fGlobalChatRadiusLimit;
	uint8_t byteShowNameTags;
	bool m_bLimitGlobalChatRadius;
};



struct stDialogInfo
{
	IDirect3DDevice9* m_pD3DDevice;
	int	iTextPoxX;
	int	iTextPoxY;
	uint32_t	uiDialogSizeX;
	uint32_t	uiDialogSizeY;
	int	iBtnOffsetX;
	int	iBtnOffsetY;
	class _CDXUTDialog* pDialog;
	class _CDXUTListBox* pList;
	class _CDXUTIMEEditBox* pEditBox;
	int	iIsActive;
	int	iType;
	uint32_t	DialogID;
	char* pText;
	uint32_t	uiTextWidth;
	uint32_t	uiTextHeight;
	char		szCaption[65];
	int		bServerside;
};

struct stTextDrawTransmit
{
	union
	{
		BYTE byteFlags;
		struct
		{
			BYTE byteBox : 1;
			BYTE byteLeft : 1;
			BYTE byteRight : 1;
			BYTE byteCenter : 1;
			BYTE byteProportional : 1;
			BYTE bytePadding : 3;
		};
	};
	float fLetterWidth;
	float fLetterHeight;
	DWORD dwLetterColor;
	float fBoxWidth;
	float fBoxHeight;
	DWORD dwBoxColor;
	BYTE byteShadow;
	BYTE byteOutline;
	DWORD dwBackgroundColor;
	BYTE byteStyle;
	BYTE byteUNK;
	float fX;
	float fY;
	uint16_t sModel;
	float fRot[3];
	float fZoom;
	WORD sColor[2];
};

struct stTextdraw
{
	char		szText[800 + 1];
	char		szString[1600 + 2];
	float		fLetterWidth;
	float		fLetterHeight;
	DWORD		dwLetterColor;
	uint8_t		byte_unk;	// always = 01 (?)
	BYTE		byteCenter;
	BYTE		byteBox;
	float		fBoxSizeX;
	float		fBoxSizeY;
	DWORD		dwBoxColor;
	BYTE		byteProportional;
	DWORD		dwShadowColor;
	BYTE		byteShadowSize;
	BYTE		byteOutline;
	BYTE		byteLeft;
	BYTE		byteRight;
	int			iStyle;		// font style/texture/model
	float		fX;
	float		fY;
	byte		unk[8];
	DWORD		dword99B;	// -1 by default
	DWORD		dword99F;	// -1 by default
	DWORD		index;		// -1 if bad
	BYTE		byte9A7;	// = 1; 0 by default
	uint16_t	sModel;
	float		fRot[3];
	float		fZoom;
	WORD		sColor[2];
	BYTE		f9BE;
	BYTE		byte9BF;
	BYTE		byte9C0;
	DWORD		dword9C1;
	DWORD		dword9C5;
	DWORD		dword9C9;
	DWORD		dword9CD;
	BYTE		byte9D1;
	DWORD		dword9D2;
};

struct stTextdrawPool
{
	int					iIsListed[SAMP_MAX_TEXTDRAWS];
	int					iPlayerTextDraw[SAMP_MAX_PLAYERTEXTDRAWS];
	struct stTextdraw* textdraw[SAMP_MAX_TEXTDRAWS];
	struct stTextdraw* playerTextdraw[SAMP_MAX_PLAYERTEXTDRAWS];
};

struct stPickup
{
	int		iModelID;
	int		iType;
	float	fPosition[3];
};

struct stPickupPool
{
	int				iPickupsCount;
	uint32_t		ul_GTA_PickupID[SAMP_MAX_PICKUPS];
	int				iPickupID[SAMP_MAX_PICKUPS];
	int				iTimePickup[SAMP_MAX_PICKUPS];
	uint8_t			unk[SAMP_MAX_PICKUPS * 3];
	struct stPickup pickup[SAMP_MAX_PICKUPS];
};

struct stPlayerPool
{
	uint32_t				ulMaxPlayerID;
	uint16_t				sLocalPlayerID;
	void* pVTBL_txtHandler;
	std::string				strLocalPlayerName;
	struct stLocalPlayer* pLocalPlayer;
	int						iLocalPlayerPing;
	int						iLocalPlayerScore;
	struct stRemotePlayer* pRemotePlayer[SAMP_MAX_PLAYERS];
	int						iIsListed[SAMP_MAX_PLAYERS];
	DWORD						dwPlayerIP[SAMP_MAX_PLAYERS]; // always 0
};

struct stSAMPKeys
{
	uint8_t keys_primaryFire : 1;
	uint8_t keys_horn__crouch : 1;
	uint8_t keys_secondaryFire__shoot : 1;
	uint8_t keys_accel__zoomOut : 1;
	uint8_t keys_enterExitCar : 1;
	uint8_t keys_decel__jump : 1;			// on foot: jump or zoom in
	uint8_t keys_circleRight : 1;
	uint8_t keys_aim : 1;					// hydra auto aim or on foot aim
	uint8_t keys_circleLeft : 1;
	uint8_t keys_landingGear__lookback : 1;
	uint8_t keys_unknown__walkSlow : 1;
	uint8_t keys_specialCtrlUp : 1;
	uint8_t keys_specialCtrlDown : 1;
	uint8_t keys_specialCtrlLeft : 1;
	uint8_t keys_specialCtrlRight : 1;
	uint8_t keys__unused : 1;
};

struct stOnFootData
{
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float		fPosition[3];
	float		fQuaternion[4];
	uint8_t		byteHealth;
	uint8_t		byteArmor;
	uint8_t		byteCurrentWeapon;
	uint8_t		byteSpecialAction;
	float		fMoveSpeed[3];
	float		fSurfingOffsets[3];
	uint16_t	sSurfingVehicleID;
	short		sCurrentAnimationID;
	short		sAnimFlags;
};

struct stInCarData
{
	uint16_t	sVehicleID;
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float		fQuaternion[4];
	float		fPosition[3];
	float		fMoveSpeed[3];
	float		fVehicleHealth;
	uint8_t		bytePlayerHealth;
	uint8_t		byteArmor;
	uint8_t		byteCurrentWeapon;
	uint8_t		byteSiren;
	uint8_t		byteLandingGearState;
	uint16_t	sTrailerID;
	union
	{
		uint16_t	HydraThrustAngle[2];	//nearly same value
		float		fTrainSpeed;
	};
};

struct stAimData
{
	BYTE	byteCamMode;
	float	vecAimf1[3];
	float	vecAimPos[3];
	float	fAimZ;
	BYTE	byteCamExtZoom : 6;		// 0-63 normalized
	BYTE	byteWeaponState : 2;	// see eWeaponState
	BYTE	bUnk;
};

struct stTrailerData
{
	uint16_t	sTrailerID;
	float		fPosition[3];
	float		fQuaternion[4];
	float		fSpeed[3];
	float		fSpin[3];
};

struct stPassengerData
{
	uint16_t	sVehicleID;
	uint8_t		byteSeatID : 6;
	uint8_t		byteDriveBy : 1;
	uint8_t		byteCuffed : 1;
	uint8_t		byteCurrentWeapon;
	uint8_t		byteHealth;
	uint8_t		byteArmor;
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float	fPosition[3];
};

struct stDamageData
{
	uint16_t	sVehicleID_lastDamageProcessed;
	int			iBumperDamage;
	int			iDoorDamage;
	uint8_t		byteLightDamage;
	uint8_t		byteWheelDamage;
};

struct stSurfData
{
	int			iIsSurfing;
	float		fSurfPosition[3];
	int			iUnk0;
	uint16_t	sSurfingVehicleID;
	uint32_t	ulSurfTick;
	struct stSAMPVehicle* pSurfingVehicle;
	int			iUnk1;
	int			iSurfMode;	//0 = not surfing, 1 = moving (unstable surf), 2 = fixed on vehicle
};

struct stUnoccupiedData
{
	int16_t sVehicleID;
	uint8_t byteSeatID;
	float	fRoll[3];
	float	fDirection[3];
	float	fPosition[3];
	float	fMoveSpeed[3];
	float	fTurnSpeed[3];
	float	fHealth;
};

struct stBulletData
{
	uint8_t		byteType;
	uint16_t	sTargetID;
	float		fOrigin[3];
	float		fTarget[3];
	float		fCenter[3];
	uint8_t		byteWeaponID;
};

struct stSpectatorData
{
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float	fPosition[3];
};

struct stStatsData
{
	int iMoney;
	int iAmmo;	// ?
};

struct stHeadSync
{
	float	fHeadSync[3];
	int		iHeadSyncUpdateTick;
	int		iHeadSyncLookTick;
};

struct stLocalPlayer
{
	struct stSAMPPed* pSAMP_Actor;
	uint16_t				sCurrentAnimID;
	uint16_t				sAnimFlags;
	uint32_t				ulUnk0;
	int						iIsActive;
	int						iIsWasted;
	uint16_t				sCurrentVehicleID;
	uint16_t				sLastVehicleID;
	struct stOnFootData		onFootData;
	struct stPassengerData	passengerData;
	struct stTrailerData	trailerData;
	struct stInCarData		inCarData;
	struct stAimData		aimData;
	uint8_t					byteTeamID;
	int						iSpawnSkin;
	uint8_t					byteUnk1;
	float					fSpawnPos[3];
	float					fSpawnRot;
	int						iSpawnWeapon[3];
	int						iSpawnAmmo[3];
	int						iIsActorAlive;
	int						iSpawnClassLoaded;
	uint32_t				ulSpawnSelectionTick;
	uint32_t				ulSpawnSelectionStart;
	int						iIsSpectating;
	uint8_t					byteTeamID2;
	uint16_t				usUnk2;
	uint32_t				ulSendTick;
	uint32_t				ulSpectateTick;
	uint32_t				ulAimTick;
	uint32_t				ulStatsUpdateTick;
	uint32_t				ulWeapUpdateTick;
	uint16_t				sAimingAtPid;
	uint16_t				usUnk3;
	uint8_t					byteCurrentWeapon;
	uint8_t					byteWeaponInventory[13];
	int						iWeaponAmmo[13];
	int						iPassengerDriveBy;
	uint8_t					byteCurrentInterior;
	int						iIsInRCVehicle;
	uint16_t				sTargetObjectID;
	uint16_t				sTargetVehicleID;
	uint16_t				sTargetPlayerID;
	struct stHeadSync		headSyncData;
	uint32_t				ulHeadSyncTick;
	BYTE					byteSpace3[260];
	struct stSurfData		surfData;
	int						iClassSelectionOnDeath;
	int						iSpawnClassID;
	int						iRequestToSpawn;
	int						iIsInSpawnScreen;
	uint32_t				ulUnk4;
	uint8_t					byteSpectateMode;		// 3 = vehicle, 4 = player, side = 14, fixed = 15
	uint8_t					byteSpectateType;		// 0 = none, 1 = player, 2 = vehicle
	int						iSpectateID;
	int						iInitiatedSpectating;
	struct stDamageData		vehicleDamageData;
};

struct stRemotePlayerData
{
	struct stSAMPPed* pSAMP_Actor;
	struct stSAMPVehicle* pSAMP_Vehicle;
	uint8_t					byteTeamID;
	uint8_t					bytePlayerState;
	uint8_t					byteSeatID;
	uint32_t				ulUnk3;
	int						iPassengerDriveBy;
	void* pUnk0;
	uint8_t					byteUnk1[60];
	float					fSomething[3];
	float					fVehicleRoll[4];
	uint32_t				ulUnk2[3];
	float					fOnFootPos[3];
	float					fOnFootMoveSpeed[3];
	float					fVehiclePosition[3];
	float					fVehicleMoveSpeed[3];
	uint16_t				sPlayerID;
	uint16_t				sVehicleID;
	uint32_t				ulUnk5;
	int						iShowNameTag;
	int						iHasJetPack;
	uint8_t					byteSpecialAction;
	uint32_t				ulUnk4[3];
	struct stOnFootData		onFootData;
	struct stInCarData		inCarData;
	struct stTrailerData	trailerData;
	struct stPassengerData	passengerData;
	struct stAimData		aimData;
	float					fActorArmor;
	float					fActorHealth;
	uint32_t				ulUnk10;
	uint8_t					byteUnk9;
	uint32_t				dwTick;
	uint32_t				dwLastStreamedInTick;	// is 0 when currently streamed in
	uint32_t				ulUnk7;
	int						iAFKState;
	struct stHeadSync		headSyncData;
	int						iGlobalMarkerLoaded;
	int						iGlobalMarkerLocation[3];
	uint32_t				ulGlobalMarker_GTAID;
};

struct stRemotePlayer
{
	stRemotePlayerData* pPlayerData;
	int					iIsNPC;
	void* pVTBL_txtHandler;
	std::string			strPlayerName;
	int					iScore;
	int					iPing;
};

template <typename T>
struct stSAMPEntity
{
	void* pVTBL;
	uint8_t		byteUnk0[60]; // game CEntity object maybe. always empty.
	T* pGTAEntity;
	uint32_t	ulGTAEntityHandle;
};

struct stSAMPPed : public stSAMPEntity < actor_info >
{
	int					usingCellPhone;
	uint8_t				byteUnk0[600];
	struct actor_info* pGTA_Ped;
	uint8_t				byteUnk1[22];
	uint8_t				byteKeysId;
	uint16_t			ulGTA_UrinateParticle_ID;
	int					DrinkingOrSmoking;
	int					object_in_hand;
	int					drunkLevel;
	uint8_t				byteUnk2[5];
	int					isDancing;
	int					danceStyle;
	int					danceMove;
	uint8_t				byteUnk3[20];
	int					isUrinating;
};

struct stVehiclePool
{
	int						iVehicleCount;
	void* pUnk0;
	uint8_t					byteSpace1[0x112C];
	struct stSAMPVehicle* pSAMP_Vehicle[SAMP_MAX_VEHICLES];
	int						iIsListed[SAMP_MAX_VEHICLES];
	struct vehicle_info* pGTA_Vehicle[SAMP_MAX_VEHICLES];
	uint8_t					byteSpace2[SAMP_MAX_VEHICLES * 6];
	uint32_t				ulShit[SAMP_MAX_VEHICLES];
	int						iIsListed2[SAMP_MAX_VEHICLES];
	uint32_t				byteSpace3[SAMP_MAX_VEHICLES * 2];
	float					fSpawnPos[SAMP_MAX_VEHICLES][3];
	int						iInitiated;
};

struct stSAMPVehicle : public stSAMPEntity < vehicle_info >
{
	uint32_t			bUnk0;
	struct vehicle_info* pGTA_Vehicle;
	uint8_t				byteUnk1[8];
	int					bIsMotorOn;
	int					iIsLightsOn;
	int					iIsLocked;
	uint8_t				byteIsObjective;
	int					iObjectiveBlipCreated;
	uint8_t				byteUnk2[16];
	uint8_t				byteColor[2];
	int					iColorSync;
	int					iColor_something;
};

struct stObject : public stSAMPEntity < object_info >
{
	uint8_t				byteUnk0[2];
	uint32_t			ulUnk1;
	int					iModel;
	uint16_t			byteUnk2;
	float				fDrawDistance;
	float				fUnk;
	float				fPos[3];
	uint8_t				byteUnk3[68];
	uint8_t				byteUnk4;
	float				fRot[3];
	// ...
};

struct stObjectPool
{
	int				iObjectCount;
	int				iIsListed[SAMP_MAX_OBJECTS];
	struct stObject* object[SAMP_MAX_OBJECTS];
};

struct stGangzone
{
	float	fPosition[4];
	DWORD	dwColor;
	DWORD	dwAltColor;
};

struct stGangzonePool
{
	struct stGangzone* pGangzone[SAMP_MAX_GANGZONES];
	int					iIsListed[SAMP_MAX_GANGZONES];
};

struct stTextLabel
{
	char* pText;
	DWORD		color;
	float		fPosition[3];
	float		fMaxViewDistance;
	uint8_t		byteShowBehindWalls;
	uint16_t	sAttachedToPlayerID;
	uint16_t	sAttachedToVehicleID;
};

struct stTextLabelPool
{
	struct stTextLabel	textLabel[SAMP_MAX_3DTEXTS];
	int					iIsListed[SAMP_MAX_3DTEXTS];
};

struct stChatEntry
{
	uint32_t	SystemTime;
	char		szPrefix[28];
	char		szText[144];
	uint8_t		unknown[64];
	int			iType;			// 2 - text + prefix, 4 - text (server msg), 8 - text (debug)
	D3DCOLOR	clTextColor;
	D3DCOLOR	clPrefixColor;	// or textOnly colour
};

struct stFontRenderer
{
	ID3DXFont* m_pChatFont;
	ID3DXFont* m_pLittleFont;
	ID3DXFont* m_pChatShadowFont;
	ID3DXFont* m_pLittleShadowFont;
	ID3DXFont* m_pCarNumberFont;
	ID3DXSprite* m_pTempSprite;
	IDirect3DDevice9* m_pD3DDevice;
	char* m_pszTextBuffer;
};

struct stChatInfo
{
	int					pagesize;
	char* pLastMsgText;
	int					iChatWindowMode;
	uint8_t				bTimestamps;
	uint32_t			m_iLogFileExist;
	char				logFilePathChatLog[MAX_PATH + 1];
	void* pGameUI; // CDXUTDialog
	void* pEditBackground; // CDXUTEditBox
	void* pDXUTScrollBar;
	D3DCOLOR			clTextColor;
	D3DCOLOR			clInfoColor;
	D3DCOLOR			clDebugColor;
	DWORD				m_lChatWindowBottom;
	struct stChatEntry	chatEntry[100];
	stFontRenderer* m_pFontRenderer;
	ID3DXSprite* m_pChatTextSprite;
	ID3DXSprite* m_pSprite;
	IDirect3DDevice9* m_pD3DDevice;
	int				m_iRenderMode; // 0 - Direct Mode (slow), 1 - Normal mode
	ID3DXRenderToSurface* pID3DXRenderToSurface;
	IDirect3DTexture9* m_pTexture;
	IDirect3DSurface9* pSurface;
	D3DDISPLAYMODE* pD3DDisplayMode;
	int					iUnk1[3];
	int					iUnk2; // smth related to drawing in direct mode
	int					m_iRedraw;
	int					m_nPrevScrollBarPosition;
	int					m_iFontSizeY;
	int					m_iTimestampWidth;
	int					m_iTimeStampTextOffset;
};

struct stInputBox
{
	void* pUnknown;
	uint8_t	bIsChatboxOpen;
	uint8_t	bIsMouseInChatbox;
	uint8_t	bMouseClick_related;
	uint8_t	unk;
	DWORD	dwPosChatInput[2];
	uint8_t	unk2[263];
	int		iCursorPosition;
	uint8_t	unk3;
	int		iMarkedText_startPos; // Highlighted text between this and iCursorPosition
	uint8_t	unk4[20];
	int		iMouseLeftButton;
};

typedef void(__cdecl* CMDPROC) (PCHAR);
struct stInputInfo
{
	void* pD3DDevice;
	void* pDXUTDialog;
	stInputBox* pDXUTEditBox;
	CMDPROC				pCMDs[SAMP_MAX_CLIENTCMDS];
	char				szCMDNames[SAMP_MAX_CLIENTCMDS][33];
	int					iCMDCount;
	int					iInputEnabled;
	char				szInputBuffer[129];
	char				szRecallBufffer[10][129];
	char				szCurrentBuffer[129];
	int					iCurrentRecall;
	int					iTotalRecalls;
	CMDPROC				pszDefaultCMD;
};

struct stKillEntry
{
	char		szKiller[25];
	char		szVictim[25];
	D3DCOLOR	clKillerColor;
	D3DCOLOR	clVictimColor;
	uint8_t		byteType;
};

struct stKillInfo
{
	int					iEnabled;
	struct stKillEntry	killEntry[5];
	int 			iLongestNickLength;
	int 			iOffsetX;
	int 			iOffsetY;

	ID3DXFont* pD3DFont;
	ID3DXFont* pWeaponFont1;
	ID3DXFont* pWeaponFont2;
	ID3DXSprite* pSprite;
	IDirect3DDevice9* pD3DDevice;
	int 			iAuxFontInited;
	ID3DXFont* pAuxFont1;
	ID3DXFont* pAuxFont2;
};

struct stChatPlayer
{
	int		iCreated;
	char	probablyTheText[256];
	DWORD	dwTickCreated;
	DWORD	dwLiveLength;
	DWORD	dwColor;
	float	fDrawDistance;
	DWORD	dwUnknown;
};

struct stAudio {
	int	iSoundState; // 0 - Finished, 1 - Loaded, 2 - Playing
};

struct stGameInfo
{
	struct stCamera
	{
		void* pEntity; // attached entity
		class CMatrix_Padded* matrix;
	};

	stAudio* pAudio;
	stCamera* pCamera;
	stSAMPPed* pLocalPlayerPed;
	float		fCheckpointPos[3];
	float		fCheckpointExtent[3];
	int			bCheckpointsEnabled;

	// not tested
	DWORD		dwCheckpointMarker;
	float		fRaceCheckpointPos[3];
	float		fRaceCheckpointNext[3];
	float		m_fRaceCheckpointSize;
	uint8_t		byteRaceType;

	int			bRaceCheckpointsEnabled;

	// not tested
	DWORD		dwRaceCheckpointMarker;
	DWORD		dwRaceCheckpointHandle;

	int			iCursorMode;
	uint32_t	ulUnk1;
	int			bClockEnabled;
	uint32_t	ulUnk2;
	int			bHeadMove;
	uint32_t		ulFpsLimit;
	uint8_t		byteUnk3;
	uint8_t		byteVehicleModels[212];
};

struct stScoreboardInfo
{
	int					iIsEnabled;
	int					iPlayersCount;
	float				fTextOffset[2];
	float				fScalar;
	float				fSize[2];
	float				fUnk0[5];
	IDirect3DDevice9* pDirectDevice;
	class _CDXUTDialog* pDialog;
	class _CDXUTListBox* pList;
	int					iOffset;		// ?
	int					iIsSorted;		// ?
};

struct stActorPool
{
	int					iLastActorID;
	stSAMPEntity<void>* pActor[SAMP_MAX_ACTORS]; // ?
	int					iIsListed[SAMP_MAX_ACTORS];
	struct actor_info* pGTAPed[SAMP_MAX_ACTORS];
	uint32_t			ulUnk0[SAMP_MAX_ACTORS];
	uint32_t			ulUnk1[SAMP_MAX_ACTORS];
};

struct stChatBubbleInfo
{
	struct stChatPlayer	chatBubble[SAMP_MAX_PLAYERS];
};

struct stStreamedOutPlayerInfo
{
	int		iPlayerID[SAMP_MAX_PLAYERS];
	float	fPlayerPos[SAMP_MAX_PLAYERS][3];
};
#pragma pack(pop)
#endif

#ifdef SAMP_R5
#pragma pack(push, 1)
struct stSAMPPools
{
	struct stVehiclePool* pVehicle;
	struct stPlayerPool* pPlayer;
	struct stPickupPool* pPickup;
	struct stObjectPool* pObject;
	struct stActorPool* pActor;
	struct stGangzonePool* pGangzone;
	struct stTextLabelPool* pText3D;
	struct stTextdrawPool* pTextdraw;
	void* pMenu;
};

struct stSAMP
{
	void* pRakClientInterface;
	uint8_t                  	_pad0[20];
	void* pUnk0;
	struct stServerInfo* pServerInfo;
	uint8_t				_pad1[16];
	char				szIP[256 + 1];
	char				szHostname[256 + 1];
	uint8_t                 	_pad2;
	bool                    	m_bUpdateCameraTarget;
	bool				m_bNoNameTagStatus;
	uint32_t			ulPort;
	BOOL				m_bLanMode;
	uint32_t			ulMapIcons[SAMP_MAX_MAPICONS];
	Gamestate			iGameState;
	uint32_t			ulConnectTick;
	struct stServerPresets* pSettings;
	uint8_t                 	_pad3[5];
	struct stSAMPPools* pPools;
};

struct stServerInfo {
	uint32_t 			uiIP;
	uint16_t 			usPort;

	// ...

};

struct stServerPresets
{
	uint8_t byteCJWalk;
	int m_iDeathDropMoney;
	float	fWorldBoundaries[4];
	bool m_bAllowWeapons;
	float	fGravity;
	uint8_t byteDisableInteriorEnterExits;
	uint32_t ulVehicleFriendlyFire;
	bool m_byteHoldTime;
	bool m_bInstagib;
	bool m_bZoneNames;
	bool m_byteFriendlyFire;
	int		iClassesAvailable;
	float	fNameTagsDistance;
	bool m_bManualVehicleEngineAndLight;
	uint8_t byteWorldTime_Hour;
	uint8_t byteWorldTime_Minute;
	uint8_t byteWeather;
	uint8_t byteNoNametagsBehindWalls;
	int iPlayerMarkersMode;
	float	fGlobalChatRadiusLimit;
	uint8_t byteShowNameTags;
	bool m_bLimitGlobalChatRadius;
};



struct stDialogInfo
{
	IDirect3DDevice9* m_pD3DDevice;
	int	iTextPoxX;
	int	iTextPoxY;
	uint32_t	uiDialogSizeX;
	uint32_t	uiDialogSizeY;
	int	iBtnOffsetX;
	int	iBtnOffsetY;
	class _CDXUTDialog* pDialog;
	class _CDXUTListBox* pList;
	class _CDXUTIMEEditBox* pEditBox;
	int	iIsActive;
	int	iType;
	uint32_t	DialogID;
	char* pText;
	uint32_t	uiTextWidth;
	uint32_t	uiTextHeight;
	char		szCaption[65];
	int		bServerside;
};

struct stTextDrawTransmit
{
	union
	{
		BYTE byteFlags;
		struct
		{
			BYTE byteBox : 1;
			BYTE byteLeft : 1;
			BYTE byteRight : 1;
			BYTE byteCenter : 1;
			BYTE byteProportional : 1;
			BYTE bytePadding : 3;
		};
	};
	float fLetterWidth;
	float fLetterHeight;
	DWORD dwLetterColor;
	float fBoxWidth;
	float fBoxHeight;
	DWORD dwBoxColor;
	BYTE byteShadow;
	BYTE byteOutline;
	DWORD dwBackgroundColor;
	BYTE byteStyle;
	BYTE byteUNK;
	float fX;
	float fY;
	uint16_t sModel;
	float fRot[3];
	float fZoom;
	WORD sColor[2];
};

struct stTextdraw
{
	char		szText[800 + 1];
	char		szString[1600 + 2];
	float		fLetterWidth;
	float		fLetterHeight;
	DWORD		dwLetterColor;
	uint8_t		byte_unk;	// always = 01 (?)
	BYTE		byteCenter;
	BYTE		byteBox;
	float		fBoxSizeX;
	float		fBoxSizeY;
	DWORD		dwBoxColor;
	BYTE		byteProportional;
	DWORD		dwShadowColor;
	BYTE		byteShadowSize;
	BYTE		byteOutline;
	BYTE		byteLeft;
	BYTE		byteRight;
	int			iStyle;		// font style/texture/model
	float		fX;
	float		fY;
	byte		unk[8];
	DWORD		dword99B;	// -1 by default
	DWORD		dword99F;	// -1 by default
	DWORD		index;		// -1 if bad
	BYTE		byte9A7;	// = 1; 0 by default
	uint16_t	sModel;
	float		fRot[3];
	float		fZoom;
	WORD		sColor[2];
	BYTE		f9BE;
	BYTE		byte9BF;
	BYTE		byte9C0;
	DWORD		dword9C1;
	DWORD		dword9C5;
	DWORD		dword9C9;
	DWORD		dword9CD;
	BYTE		byte9D1;
	DWORD		dword9D2;
};

struct stTextdrawPool
{
	int					iIsListed[SAMP_MAX_TEXTDRAWS];
	int					iPlayerTextDraw[SAMP_MAX_PLAYERTEXTDRAWS];
	struct stTextdraw* textdraw[SAMP_MAX_TEXTDRAWS];
	struct stTextdraw* playerTextdraw[SAMP_MAX_PLAYERTEXTDRAWS];
};

struct stPickup
{
	int		iModelID;
	int		iType;
	float	fPosition[3];
};

struct stPickupPool
{
	int				iPickupsCount;
	uint32_t		ul_GTA_PickupID[SAMP_MAX_PICKUPS];
	int				iPickupID[SAMP_MAX_PICKUPS];
	int				iTimePickup[SAMP_MAX_PICKUPS];
	uint8_t			unk[SAMP_MAX_PICKUPS * 3];
	struct stPickup pickup[SAMP_MAX_PICKUPS];
};

struct stPlayerPool
{
	//uint32_t				ulMaxPlayerID;
	int						iLocalPlayerScore;
	uint16_t				sLocalPlayerID;
	void* pVTBL_txtHandler;
	std::string				strLocalPlayerName;
	int						iLocalPlayerPing;
	struct stLocalPlayer* pLocalPlayer;
	int						iIsListed[SAMP_MAX_PLAYERS];
	BOOL					bSavedCheckCollision[SAMP_MAX_PLAYERS];
	struct stRemotePlayer* pRemotePlayer[SAMP_MAX_PLAYERS];
	int						largestID;
};

struct stSAMPKeys
{
	uint8_t keys_primaryFire : 1;
	uint8_t keys_horn__crouch : 1;
	uint8_t keys_secondaryFire__shoot : 1;
	uint8_t keys_accel__zoomOut : 1;
	uint8_t keys_enterExitCar : 1;
	uint8_t keys_decel__jump : 1;			// on foot: jump or zoom in
	uint8_t keys_circleRight : 1;
	uint8_t keys_aim : 1;					// hydra auto aim or on foot aim
	uint8_t keys_circleLeft : 1;
	uint8_t keys_landingGear__lookback : 1;
	uint8_t keys_unknown__walkSlow : 1;
	uint8_t keys_specialCtrlUp : 1;
	uint8_t keys_specialCtrlDown : 1;
	uint8_t keys_specialCtrlLeft : 1;
	uint8_t keys_specialCtrlRight : 1;
	uint8_t keys__unused : 1;
};

struct stOnFootData
{
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float		fPosition[3];
	float		fQuaternion[4];
	uint8_t		byteHealth;
	uint8_t		byteArmor;
	uint8_t		byteCurrentWeapon;
	uint8_t		byteSpecialAction;
	float		fMoveSpeed[3];
	float		fSurfingOffsets[3];
	uint16_t	sSurfingVehicleID;
	short		sCurrentAnimationID;
	short		sAnimFlags;
};

struct stInCarData
{
	uint16_t	sVehicleID;
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float		fQuaternion[4];
	float		fPosition[3];
	float		fMoveSpeed[3];
	float		fVehicleHealth;
	uint8_t		bytePlayerHealth;
	uint8_t		byteArmor;
	uint8_t		byteCurrentWeapon;
	uint8_t		byteSiren;
	uint8_t		byteLandingGearState;
	uint16_t	sTrailerID;
	union
	{
		uint16_t	HydraThrustAngle[2];	//nearly same value
		float		fTrainSpeed;
		float       fBikeSideAngle;
	};
};

struct stAimData
{
	BYTE	byteCamMode;
	float	vecAimf1[3];
	float	vecAimPos[3];
	float	fAimZ;
	BYTE	byteCamExtZoom : 6;		// 0-63 normalized
	BYTE	byteWeaponState : 2;	// see eWeaponState
	BYTE	byteAspectRatio;
};

struct stTrailerData
{
	uint16_t	sTrailerID;
	float		fPosition[3];
	float		fQuaternion[4];
	float		fSpeed[3];
	float		fSpin[3];
};

struct stPassengerData
{
	uint16_t	sVehicleID;
	uint8_t		byteSeatID;
	uint8_t		byteCurrentWeapon;
	uint8_t		byteHealth;
	uint8_t		byteArmor;
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float	fPosition[3];
};

struct stDamageData
{
	uint16_t	sVehicleID_lastDamageProcessed;
	int			iBumperDamage;
	int			iDoorDamage;
	uint8_t		byteLightDamage;
	uint8_t		byteWheelDamage;
};

struct stSurfData
{
	int			iIsSurfing;
	float		fSurfPosition[3];
	int			iUnk0;
	uint16_t	sSurfingVehicleID;
	uint32_t	ulSurfTick;
	struct stSAMPVehicle* pSurfingVehicle;
	int			iUnk1;
	int			iSurfMode;	//0 = not surfing, 1 = moving (unstable surf), 2 = fixed on vehicle
};

struct stUnoccupiedData
{
	int16_t sVehicleID;
	uint8_t byteSeatID;
	float	fRoll[3];
	float	fDirection[3];
	float	fPosition[3];
	float	fMoveSpeed[3];
	float	fTurnSpeed[3];
	float	fHealth;
};

struct stBulletData
{
	uint8_t		byteType;
	uint16_t	sTargetID;
	float		fOrigin[3];
	float		fTarget[3];
	float		fCenter[3];
	uint8_t		byteWeaponID;
};

struct stSpectatorData
{
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float	fPosition[3];
};

struct stStatsData
{
	int iMoney;
	int iAmmo;	// ?
};

struct stHeadSync
{
	CVector	fHeadSync;
	unsigned long		iHeadSyncUpdateTick;
	unsigned long		iHeadSyncLookTick;
};

struct CAMERA_TARGET_AIM
{
	uint16_t ObjectID;
	uint16_t VehicleID;
	uint16_t PlayerID;
	uint16_t ActorID;
};

struct PLAYER_SPAWN_INFO
{
	BYTE byteTeam;
	int iSkin;
	int iCustomModel;
	BYTE unk;
	VECTOR vecPos;
	float fRotation;
	int iSpawnWeapons[3];
	int iSpawnWeaponsAmmo[3];
};

struct stLocalPlayer
{
	struct stInCarData		inCarData;
	struct stAimData		aimData;
	struct stTrailerData		trailerData;
	struct stOnFootData		onFootData;
	struct stPassengerData		passengerData;
	int				iIsActive;
	int				iIsWasted;
	uint16_t			sCurrentVehicleID;
	uint16_t			sLastVehicleID;
	uint16_t			sCurrentAnimID;
	uint16_t			sAnimFlags;
	uint32_t			ulUnk0;
	struct stSAMPPed* pSAMP_Actor;
	int				iIsSpectating;
	uint8_t				byteTeamID;
	int				iSpawnClassID;
	CAMERA_TARGET_AIM       	cameraTarget;
	uint32_t			ulCameraTargetTick;
	struct stHeadSync		headSyncData;
	uint32_t			ulHeadSyncTick;
	uint16_t			usUnk2;
	uint32_t			ulSendTick;
	uint32_t			ulSpectateTick;
	uint32_t			ulAimTick;
	uint32_t			ulStatsUpdateTick;
	int				iSpawnClassLoaded;
	uint32_t			ulSpawnSelectionTick;
	uint32_t			ulSpawnSelectionStart;
	PLAYER_SPAWN_INFO      		spawnInfo;
	int				iIsActorAlive;
	uint32_t			ulWeapUpdateTick;
	uint16_t			sAimingAtPlayerID;
	uint16_t			sAimingAtActorID;
	uint8_t				byteCurrentWeapon;
	uint8_t				byteWeaponInventory[13];
	int				iWeaponAmmo[13];
	int				iPassengerDriveBy;
	uint8_t				byteCurrentInterior;
	int				iIsInRCVehicle;
	char                    	szPlayerName[256];
	struct stSurfData		surfData;
	int				iClassSelectionOnDeath;
	int				iRequestToSpawn;
	int				iIsInSpawnScreen;
	uint32_t			ulDisplayZoneTick;
	uint8_t				byteSpectateMode;		// 3 = vehicle, 4 = player, side = 14, fixed = 15
	uint8_t				byteSpectateType;		// 0 = none, 1 = player, 2 = vehicle
	int				iSpectateID;
	int				iInitiatedSpectating;
	struct stDamageData		vehicleDamageData;
};

struct stRemotePlayerData
{
	int							field_1;
	BOOL                    	iShowNameTag;
	BOOL                   	 	bUsingJetPack;
	uint8_t						byteSpecialAction;
	char						pad_2[12];
	struct stInCarData			inCarData;
	struct stTrailerData		trailerData;
	struct stAimData			aimData;
	struct stPassengerData		passengerData;
	struct stOnFootData			onFootData;
	uint8_t						byteTeamID;
	uint8_t						bytePlayerState;
	uint8_t						byteSeatID;
	int							field_3;
	int							iPassengerDriveBy;
	char						pad_1[76];
	CVector						m_positionDifference;
	float						real;
	CVector						imag;
	CVector						fOnFootPos;
	CVector						fOnFootMoveSpeed;
	CVector						fVehiclePosition;
	CVector						fVehicleMoveSpeed;
	float						fActorArmor;
	float						fActorHealth;
	int							iLastAnimationID;
	unsigned char						byteUpdateFromNetwork;
	unsigned long					dwTick;
	unsigned long					dwLastStreamedInTick;	// is 0 when currently streamed in
	BOOL                	ulUnk2;
	int							iAFKState;
	struct stHeadSync			headSyncData;
	struct stSAMPPed* pSAMP_Actor;
	struct stSAMPVehicle* pSAMP_Vehicle;
	uint16_t					sPlayerID;
	uint16_t					sVehicleID;
	int							iGlobalMarkerLoaded;
	int							iGlobalMarkerLocation[3];
	uint32_t					ulGlobalMarker_GTAID;
};

struct stRemotePlayer
{
	int				pad0;
	int				iScore;
	int				iIsNPC;
	int				iPing;
	stRemotePlayerData* pPlayerData;
	void* pVTBL_txtHandler;
	std::string			strPlayerName;
};

template <typename T>
struct stSAMPEntity
{
	void* pVTBL;
	uint8_t		byteUnk0[60]; // game CEntity object maybe. always empty.
	T* pGTAEntity;
	uint32_t	ulGTAEntityHandle;
};

struct stSAMPPed : public stSAMPEntity < actor_info >
{
	int					usingCellPhone;
	uint8_t				byteUnk0[600]; // attached objects info - (state + info + ptr) * 10
	struct actor_info* pGTA_Ped;
	uint8_t				byteUnk1[22];
	uint8_t				byteKeysId;
	uint16_t			ulGTA_UrinateParticle_ID;
	int					DrinkingOrSmoking;
	int					object_in_hand;
	int					drunkLevel;
	uint8_t				byteUnk2[5];
	int					isDancing;
	int					danceStyle;
	int					danceMove;
	uint8_t				byteUnk3[20];
	int					isUrinating;
};

struct stVehiclePool
{
	int						iVehicleCount;
	void* pUnk0;
	uint8_t					byteSpace1[0x112C];
	struct stSAMPVehicle* pSAMP_Vehicle[SAMP_MAX_VEHICLES];
	int						iIsListed[SAMP_MAX_VEHICLES];
	struct vehicle_info* pGTA_Vehicle[SAMP_MAX_VEHICLES];
	uint8_t					byteSpace2[SAMP_MAX_VEHICLES * 6];
	uint32_t				ulShit[SAMP_MAX_VEHICLES];
	int						iIsListed2[SAMP_MAX_VEHICLES];
	uint32_t				byteSpace3[SAMP_MAX_VEHICLES * 2];
	float					fSpawnPos[SAMP_MAX_VEHICLES][3];
	int						iInitiated;
};

struct stSAMPVehicle : public stSAMPEntity < vehicle_info >
{
	uint32_t			bUnk0;
	struct vehicle_info* pGTA_Vehicle;
	uint8_t				byteUnk1[8];
	int					bIsMotorOn;
	int					iIsLightsOn;
	int					iIsLocked;
	uint8_t				byteIsObjective;
	int					iObjectiveBlipCreated;
	uint8_t				byteUnk2[16];
	uint8_t				byteColor[2];
	int					iColorSync;
	int					iColor_something;
};

struct stObject : public stSAMPEntity < object_info >
{
	uint8_t				byteUnk0[2];
	uint32_t			ulUnk1;
	int					iModel;
	uint16_t			byteUnk2;
	float				fDrawDistance;
	float				fUnk;
	float				fPos[3];
	uint8_t				byteUnk3[68];
	uint8_t				byteUnk4;
	float				fRot[3];
	// ...
};

struct stObjectPool
{
	int				iObjectCount;
	int				iIsListed[SAMP_MAX_OBJECTS];
	struct stObject* object[SAMP_MAX_OBJECTS];
};

struct stGangzone
{
	float	fPosition[4];
	DWORD	dwColor;
	DWORD	dwAltColor;
};

struct stGangzonePool
{
	struct stGangzone* pGangzone[SAMP_MAX_GANGZONES];
	int					iIsListed[SAMP_MAX_GANGZONES];
};

struct stTextLabel
{
	char* pText;
	DWORD		color;
	float		fPosition[3];
	float		fMaxViewDistance;
	uint8_t		byteShowBehindWalls;
	uint16_t	sAttachedToPlayerID;
	uint16_t	sAttachedToVehicleID;
};

struct stTextLabelPool
{
	struct stTextLabel	textLabel[SAMP_MAX_3DTEXTS];
	int					iIsListed[SAMP_MAX_3DTEXTS];
};

struct stChatEntry
{
	uint32_t	SystemTime;
	char		szPrefix[28];
	char		szText[144];
	uint8_t		unknown[64];
	int			iType;			// 2 - text + prefix, 4 - text (server msg), 8 - text (debug)
	D3DCOLOR	clTextColor;
	D3DCOLOR	clPrefixColor;	// or textOnly colour
};

struct stFontRenderer
{
	ID3DXFont* m_pChatFont;
	ID3DXFont* m_pLittleFont;
	ID3DXFont* m_pChatShadowFont;
	ID3DXFont* m_pLittleShadowFont;
	ID3DXFont* m_pCarNumberFont;
	ID3DXSprite* m_pTempSprite;
	IDirect3DDevice9* m_pD3DDevice;
	char* m_pszTextBuffer;
};

struct stChatInfo
{
	int					pagesize;
	char* pLastMsgText;
	int					iChatWindowMode;
	uint8_t				bTimestamps;
	uint32_t			m_iLogFileExist;
	char				logFilePathChatLog[MAX_PATH + 1];
	void* pGameUI; // CDXUTDialog
	void* pEditBackground; // CDXUTEditBox
	void* pDXUTScrollBar;
	D3DCOLOR			clTextColor;
	D3DCOLOR			clInfoColor;
	D3DCOLOR			clDebugColor;
	DWORD				m_lChatWindowBottom;
	struct stChatEntry	chatEntry[100];
	stFontRenderer* m_pFontRenderer;
	ID3DXSprite* m_pChatTextSprite;
	ID3DXSprite* m_pSprite;
	IDirect3DDevice9* m_pD3DDevice;
	int				m_iRenderMode; // 0 - Direct Mode (slow), 1 - Normal mode
	ID3DXRenderToSurface* pID3DXRenderToSurface;
	IDirect3DTexture9* m_pTexture;
	IDirect3DSurface9* pSurface;
	D3DDISPLAYMODE* pD3DDisplayMode;
	int					iUnk1[3];
	int					iUnk2; // smth related to drawing in direct mode
	int					m_iRedraw;
	int					m_nPrevScrollBarPosition;
	int					m_iFontSizeY;
	int					m_iTimestampWidth;
	int					m_iTimeStampTextOffset;
};

struct stInputBox
{
	void* pUnknown;
	uint8_t	bIsChatboxOpen;
	uint8_t	bIsMouseInChatbox;
	uint8_t	bMouseClick_related;
	uint8_t	unk;
	DWORD	dwPosChatInput[2];
	uint8_t	unk2[263];
	int		iCursorPosition;
	uint8_t	unk3;
	int		iMarkedText_startPos; // Highlighted text between this and iCursorPosition
	uint8_t	unk4[20];
	int		iMouseLeftButton;
};

typedef void(__cdecl* CMDPROC) (PCHAR);
struct stInputInfo
{
	void* pD3DDevice;
	void* pDXUTDialog;
	stInputBox* pDXUTEditBox;
	CMDPROC				pCMDs[SAMP_MAX_CLIENTCMDS];
	char				szCMDNames[SAMP_MAX_CLIENTCMDS][33];
	int					iCMDCount;
	int					iInputEnabled;
	char				szInputBuffer[129];
	char				szRecallBufffer[10][129];
	char				szCurrentBuffer[129];
	int					iCurrentRecall;
	int					iTotalRecalls;
	CMDPROC				pszDefaultCMD;
};

struct stKillEntry
{
	char		szKiller[25];
	char		szVictim[25];
	D3DCOLOR	clKillerColor;
	D3DCOLOR	clVictimColor;
	uint8_t		byteType;
};

struct stKillInfo
{
	int					iEnabled;
	struct stKillEntry	killEntry[5];
	int 			iLongestNickLength;
	int 			iOffsetX;
	int 			iOffsetY;

	ID3DXFont* pD3DFont;
	ID3DXFont* pWeaponFont1;
	ID3DXFont* pWeaponFont2;
	ID3DXSprite* pSprite;
	IDirect3DDevice9* pD3DDevice;
	int 			iAuxFontInited;
	ID3DXFont* pAuxFont1;
	ID3DXFont* pAuxFont2;
};

struct stChatPlayer
{
	int		iCreated;
	char	probablyTheText[256];
	DWORD	dwTickCreated;
	DWORD	dwLiveLength;
	DWORD	dwColor;
	float	fDrawDistance;
	DWORD	dwUnknown;
};

struct stAudio {
	int	iSoundState; // 0 - Finished, 1 - Loaded, 2 - Playing
	bool bStopInteriourAmbientSounds;
};

struct stGameInfo
{
	struct stCamera
	{
		void* pEntity; // attached entity
		class CMatrix_Padded* matrix;
	};

	stAudio* pAudio;
	stCamera* pCamera;
	stSAMPPed* pLocalPlayerPed;
	float		fRaceCheckpointPos[3];
	float		fRaceCheckpointNext[3];
	float		m_fRaceCheckpointSize;
	uint8_t		byteRaceType;
	int			bRaceCheckpointsEnabled;
	DWORD		dwRaceCheckpointMarker;
	DWORD		dwRaceCheckpointHandle;
	float		fCheckpointPos[3];
	float		fCheckpointExtent[3];
	int			bCheckpointsEnabled;
	DWORD		dwCheckpointMarker;
	uint32_t	ulUnk2;
	int			bHeadMove;
	uint32_t    ulFpsLimit;
	int			iCursorMode;
	uint32_t	ulUnk1;
	int			bClockEnabled;
	uint8_t		byteUnk3;
	uint8_t		byteVehicleModels[212];
};

struct stScoreboardInfo
{
	int					iIsEnabled;
	int					iPlayersCount;
	float				fTextOffset[2];
	float				fScalar;
	float				fSize[2];
	float				fUnk0[5];
	IDirect3DDevice9* pDirectDevice;
	class _CDXUTDialog* pDialog;
	class _CDXUTListBox* pList;
	int					iOffset;		// ?
	int					iIsSorted;		// ?
};

struct stActorPool
{
	int					iLastActorID;
	stSAMPEntity<void>* pActor[SAMP_MAX_ACTORS]; // ?
	int					iIsListed[SAMP_MAX_ACTORS];
	struct actor_info* pGTAPed[SAMP_MAX_ACTORS];
	uint32_t			ulUnk0[SAMP_MAX_ACTORS];
	uint32_t			ulUnk1[SAMP_MAX_ACTORS];
};

struct stChatBubbleInfo
{
	struct stChatPlayer	chatBubble[SAMP_MAX_PLAYERS];
};

struct stStreamedOutPlayerInfo
{
	int		iPlayerID[SAMP_MAX_PLAYERS];
	float	fPlayerPos[SAMP_MAX_PLAYERS][3];
};
#pragma pack(pop)

#endif


struct stNewFunctions //dont use string in here
{
	struct stMenu
	{
		bool bIsMainMenuFocused;
		bool bMain_Menu;
		bool bImGuiDemoWindow;
		bool bImGuiHudMenu;
		bool bImFriendAndAdminsMenu;
		bool bRadioVolume; //radio & mp3 volume slider menu
		bool bImMenuNewSpeedometer;
		bool b3DPlayerInfo;
		bool bImScoreboad;

		struct stSpeialESP
		{
			bool bImBlackLightPlayersTags;
			bool bImBlackLightPlayersInfo;
		} ESPMenus;

	} Menu;

	int iAimClientBot;
	//server
	BYTE byteLagComp;

	bool bGoCfaker; //for target player
	bool bModControl[64];
	bool bTargetInfo[SAMP_MAX_PLAYERS]; //for MENU_ADDON_TARGET_INFO_LIST_OFF_PLAYERS
	bool bFuckTroll[SAMP_MAX_PLAYERS];
	bool bStickTroll[SAMP_MAX_PLAYERS];
	bool bFollowTroll[SAMP_MAX_PLAYERS];
	bool bSlapTroll[SAMP_MAX_PLAYERS];

	bool bIsFriend[SAMP_MAX_PLAYERS], bIsAdmin[SAMP_MAX_PLAYERS];
	int iFriendID[SAMP_MAX_PLAYERS], iAdminID[SAMP_MAX_PLAYERS];
	bool bViewAllAdmins; //from file list
	bool bViewAllFriends; //from file list
	bool bUsePlayersColor; //for friends & admins list
	bool bSpeedmeterBackground;

	//breakdance system
	bool bFrontnBackFlip; //key I
	bool bPlayerHop; //key B
	bool bNoSpread, bRapidFire, bNoReload, bNoStunV2, bFastCrosshair, bDroneMode;
	bool bAutoShoot;
	bool bPlayerJoiner; //aka raknet logger
	bool bSpeedometer;
	bool bCustomSprintEnergy;
	bool bActorNoFall;
	bool bSlowmotion, bSlowmotionViaKey;
	bool bCameraOverview;
	bool bVehicleDisableWheels, bVehicleDisableFrame;
	bool bFastRotation;
	bool bCrazyRoter;
	bool bMouseDrive;
	bool bWalkOnWater;
	bool bFakeAfk;
	bool bFirstPerson;
	bool bSmartInvis;
	bool bSampFastConnect;
	bool bDisableWaterWaves;
	bool bCustomColor_ESP;
	bool bInvertWalk, bInverDrive;
	bool bWheelWalk, bWheelDrive;
	bool bCrazyWalk, bCrazyDrive;
	bool bAirDrive, bAirWalk;
	bool bBackwardWalk, bBackwardDrive;
	bool bCustomTime, bCustomWeather;
	bool bFastWalk;
	bool bFastRun;
	bool bFastSprint;
	bool bFastSwim;
	bool bFasterRunArmed;
	bool bFastReload;
	bool bFastDeagle;
	bool bIsDeformedAnimActive; //for cmd
	bool bAirbreakPlayer, bAirbreakVehicle;
	bool bPTPAntiEndRoundFreeze; //fix
	bool bVehicleJump;
	bool bVehicleBreakDance;
	bool bVehicleFly, bFlyHeliMode;
	bool bPlayerFly;
	bool bCustomRunAnim;
	bool bVehicleFastExit;
	bool bVehicleQuickTurn;
	bool bWorldCollision;
	bool bPlayerGhost, bVehicleNoCollision;
	bool bAutoReconnect;
	bool bVehicleBoost;
	bool bVehicleInstantBrake;
	bool bVehicleSpider;
	bool bVehicleFreeze;
	bool bPlayerSurf;
	bool bObjectsNoCollision;
	bool bPickVehiclesMass;
	bool bAutoScroll, bAutoCBug;
	bool bPickVehicle;
	bool bComponentFlood;
	bool bWheelsStatusPulse;
	bool bFakeBullets[SAMP_MAX_PLAYERS];
	bool bHealthPulsator, bArmorPulsator, bWeaponsPulsator;
	bool bBlinkColors, bWheelsPulsator;
	bool bVehicleFakeFire;
	bool bFakeJetpack;
	bool bInvertedBackLinear, bInvertedFrontLinear;
	bool bRPCInterior;
	bool bWeaponsEnable, brestore_weapons_after_death, bWeaponsAmmoRandom;
	bool bEnableMoneyCheat;
	//bool bChangeGPCI;
	bool bPizdarvankaTarget[SAMP_MAX_PLAYERS];
	bool bPizdarvankaPlayer, bPizdarvankaVehicle;
	bool bWindowedMode;
	bool bFakeBulletsFlood; //for all in stream
	bool bFakeKillFlood;
	bool bArabDrift;
	bool bSkinChanger;
	bool bGodModMain;
	bool bGodModePlayer, bGodModeVehicle, bGodModeHeliBlade;
	bool bClickWarp;
	bool bTornadoDriving, bTornadoDrill;
	bool bPlayerDrillWalk, bPlayerCircleWalk;
	bool bVehicleWheelsVisibilityPulsator;
	bool bUpDownDrive, bBikeWheelie;
	bool bCarHardFlip;
	bool bCustomGravity, bCustomGameSpeed;
	bool bNoStun;
	bool bQuickWarp, bVehicleQuickWarp;
	bool bFreezePlayers;
	bool bPlayersElevator;
	bool bVehiclesFugga;
	bool bFlySurf;
	bool bNoCollisionSurf;

	//anti crashers
	bool bAntiWeaponCrasher;
	bool bAntiCarJack;

	//rainbow
	bool bRainbow_Radar;
	bool bRainbow_Vehicle;
	bool bRainbow_Hud;
	bool bRainbow_Crosshair;
	bool bRainbow_KillList;
	bool bRainbow_Sky;
	bool bRainbow_Chat;
	bool bRainbow_Nicknames;
	bool bRainbow_CarLights;
	bool bCustomRainbowColorSet;

	//esp
	bool bWheelESP;
	bool bPlayer3DBoxes;
	bool bSeeDrivers;
	bool bCarsDoorsStatus;
	bool bPlayersTracers;
	bool bPlayersBones;
	bool bPlayerBox;
	bool bPlayersInfo;
	bool bVehicleTracers;
	bool bVehicleInfo, bVehicle2DInfo_MyCarInclude;
	bool bHelicopterTracers;
	bool bDefaultModNametags, bDefaultModVehicleTags;
	bool bVehiclesPoliceDetector, bPoliceDetectorPeds;
	bool bOutStreamTags;
	bool bBulletTracers;
	bool bPickupESP, bObjectsESP;
	bool bDeathLogChat;
	bool bDefaultChams, bPlayersThroughWalls;
	bool bPlayersMap;
	bool bTrailerTracers;
	bool bTeleportTextSet;
	bool bWheelsInfo;
	bool bSurfer; //be

	//goc
	bool bSuperGun;
    bool bTornadoGun;

	//sync
	bool bCarRammer;
	bool bSilentInvis;
	bool bVehicleWheelAim;
	//settings
	bool bFriendTags;
	bool bAdminsTags;
	bool bChatboxLog;
	bool bMutedPlayerChatting;
	bool bShowKillCounterInsideHud;
	bool bMapIncludeVehicles;
	bool bVehicleTrailerSupport;
	bool bAntiLoading;
	bool bUseNewScoreboard;
	bool bPulseButtonsBorders; //for active buttons

	struct stPlayerMute
	{
		bool bMutedPlayers[SAMP_MAX_PLAYERS];
	} Mute;

	struct stCopyChat
	{
		bool bCopyChat[SAMP_MAX_PLAYERS];
	} _CopyChat;

	struct stUIF
	{
		struct stIslands
		{
			bool bCustomObjectKeyControls;

		} Islands;
	} UIF;

	struct stGtaHud
	{
		bool bDisabelAll;
		bool bDisableMoney;
		bool bDisableWeaponIcon;
		bool bDisableHealthBar;
		bool bDisableArmorBar;
		bool bDisableBreathBar;
		bool bDisableVitalStats;
		bool bDisableAmmo;
		bool bDisableRadar;
		bool bDisableClock;

	} GtaHUD;
};
extern struct stNewFunctions* BlackLightFuncs;

struct stModControl
{
	char time_end[16]; //shutdown game
	bool bTimeEndGame;
};
extern struct stModControl BlackLightRemoteControl;

struct stMP3
{
	float mp3_volume;
	bool bMP3LoopSong;

	std::string current_playing_song;
	std::string song_on_loading;

};
extern struct stMP3 __MP3;

struct stRadio
{
	float radio_volume;

	std::string	radio_station_path; //for radio station list
	std::string	radio_st_name_no_path; //receive ext but no path
	std::string	radio_st_name_original; //no path no file extension 

};
extern struct stRadio __Radio;

class CSampMulti
{
public:

	void FreeStringMemory(std::string& string);
	size_t GetStringLenght(std::string& string);

	int getMiddleYWindowSize(void);
	int getMiddleXWindowSize(void);

	D3DCOLOR FLOAT4TOD3DCOLOR(float Col[]);
	std::string UTF8_to_CP1251(std::string const& utf8); //masterzero
	std::string Ansii_to_UTF8(std::string const ansii); //masterzero
	std::string utf16_to_utf8(std::u16string utf16_string); //masterzero
	std::vector<std::string> get_filenames(std::experimental::filesystem::path path); //stackoverflow 
	char* BinToHex(BYTE* bin, size_t size, char* out);
	const char* GetSystemUpTime(void); //Task Manager -> Up Time

	void ImSpacing(int spaces);
	void ImAllingItem(float fpos);
	bool IsKeyHolded(int VK_KEY);
	bool DoesFileExist(const char* szFile, const char* szFileExt, const char* szPath); //for .txt, .ini, text files mostly
	bool AreNumbersIdentical(const char* text_to_scan, const char* text_for_scan);
	bool IsFileEmpty(std::fstream& szFile);
	int GetDirFileCount(const char* szDir = M0D_FOLDER); // = 0 empty
	void CreateFolder(const char* szFolderName);
	void DeleteFolder(const char* szPath);
	bool FolderExist(const char* szFolderName);
	const char* ReplaceText(const char* szText, const char* szTextToDelete);
	const char* ModifyText(const char* szText, const char* szTextToDelete, const char* szNewText);
	const char* AddText(const char* toAdd, const char* location);
	void replaceAll(std::string& source, const std::string& from, const std::string& to);
	bool MP3FileExist(const char* szFile, bool bIncludePath = true); //only for music, szfile pass parameters path+file_name+file_extension(.mp3, .wav(supported bass ext. in this case))
	void FileDelete(const char* szFile);
	void AddText_to_Text(const char* szDefineText, char szTextoAdd); //adds szTextoAdd to the latest character of szDefineText
	bool TextInFileExists(const char* file_path, const char* szTextToCheck);
	bool IsTextIdentical(const char* str1, const char* str2);
	const char* SetText(const char* szText, ...);
	void DeleteFileText(const char* szPath = M0D_FOLDER, const char* szFile = "", const char* szFileExt = "", const char* szText = "", bool bMsg = true, bool bNoExtension = false);
	void WriteToFile(const char* filename = "", const char* szText = "");
	const char* ReadFromFile(const char* szFile);
	void AddAdmin(int PlayerID, bool bNotifyMsg = true);
	void AddFriend(int PlayerID, bool bNotifyMsg = true);
	void ClearFile(const char* filename, const char* szMsg = "");
	void DeleteAdmin(int PlayerID);
	void DeleteFriend(int PlayerID);
	ImVec4 ColorOpacityPulse(ImVec4 ImColorID, float OpacityPower = 0.050f,float OpacityMax = 1.000f, float OpacityMin = 0.050f, int TimePulse = 80);
	ImColor TotalRainbow(int speed);

	D3DCOLOR CustomImVec4_D3DCOLOR(ImVec4 color);

	void IntDuplicate(int& dup_target, int duplicate_size = 2);
	int GetFilesCount(const char* szFolder);
	int GetFileItemsCount(const char* szFile);
	int GetSongsCount();
	int GetAdminsCount(void); //from file
	int GetFriendsCount(void); //from file
	int GetRadioStationsCount(void);
	int GetPlayersInStream(void);
	int GetAdminsInStream(void);
	int GetFriendsInStream(void);
	int GetPlayersInVehicleCount(void);
	int GetAFKPlayersCount(void);
	int GetPassengersInVehicleCount(void);
	int GetVehiclesInStreamCount(void);
	int GetMutedPlayersCount(void);

	int GetObjectsStreamedCount(void);
	const char* GetPlayerInfoStatus(int player_id);
	void TeleportToObject(int object_id);
	bool IsPlayerInCar(int PlayerID);
	float GetDistance(D3DXVECTOR3 target);
	float GetDistance(float X, float Y, float X1, float Y1);
	HWND GetGTAGameWindow(void);
	bool ActivateByKeyboard(const char* szName);
	void SetPlayerSprintEnergy(float fEnergy);
	void SAMPSetGamestate(int GameStateID = GAMESTATE_RESTARTING);
	int GetNextPlayerAsTarget(int iplayerID, int increase);
	int GetNearestPlayer(void);
	uint16_t getPlayerVehicleModelID_V2(int iPlayerID); //streamed
	const char* getPlayerVehicleName(int PlayerID); //streamed
	const char* getPlayerBodyPart(int BodyPartID);
	const char* getWeaponNameByModelID(int iWeaponModelID);
	int          getPlayerInteriorID(int iPlayerID);
	int          getPlayerSkin(int iPlayerID); //streamed
	int          getPlayerPing(int iPlayerID);//streamed
	int          getPlayerScore(int iPlayerID);//streamed
	int          getPlayersOnServerCount(void);
	uint8_t      getPlayerHealth(int iPlayerID);//streamed
	uint8_t      getPlayerArmor(int iPlayerID);//streamed
	uint8_t      getPlayerArmorInVehicle(int iPlayerID);//streamed
	uint8_t      getPlayerHealthInVehicle(int iPlayerID);//streamed
	float           getPlayerVehicleHealth(int iPlayerID);//streamed
	const char* getPlayerWeapon(int iPlayerID);//streamed
	uint8_t      getPlayerWeaponModelID(int iPlayerID);//streamed
	float      getPlayerSprintEnergy(int iPlayerID);//streamed
	const char* getPlayerPosition(int iPlayerID); //streamed
	void       ShowPlayerNameTag(int iPlayerID, bool bShow);//streamed
	void SayPlayerInfo(int iPlayerID); //streamed
	const char* getServerIp(void);
	float getGameFPS(void); //MTA method
	CVector CVecPlayerPos(int PlayerID);
	CVector CVecGetBonePos(int PlayerID, eBone bone = eBone::BONE_HEAD);
	bool IsPickupStreamed(int PickupID);
	int getPickupModelD(int PickupID);
	CVector CVecGetPickupPos(int PickupID);
	void GameCameraSet(float x, float y);
	const char* getWeatherName(int value_assign);
	float getVehicleSpeed(int iPlayerID, float fMultipler = 274.0f);
	const char* getVehicleNameBySAMPID(int CarID);
	int getVehicleModelBySAMPID(int CarID);
	int getDistanceCrossed(void);
	bool GetPlayerQuaternion(uint16_t playerID, float* fQuaternion);
	std::string GetCurrentTimeA(void);
	std::string GetCurrentDateA(void);
	const char* getTimeCounter(void);

	void sampNop(int id, int type, bool enabled);
	bool IsPlayerStreamed(uint16_t playerID); //doesn't count our player
	bool IsVehicleStreamed(uint16_t vehicleID);
	bool IsModDeveloper(int PlayerID);
	bool IsCurrentServer(const char* szIp = "94.23.145.137:7776");
	bool IsPlayerInWater(int PlayerID);
	bool IsObjectStreamed(int object_id);
	bool IsOurPlayerInCar(void); //is in vehicle driver or passenger
	bool IsPlayerPassenger(int PlayerID);
	bool IsPlayerDriver(int PlayerID);
	bool IsMenuActive(bool bMenuBoolean = BlackLightFuncs->Menu.bMain_Menu);
	void PlayerRestoreAnim(void);
	const char* getWeaponDamage(CPed* pPed);
	void SetWeaponDamage(CPed* pPed, float damage);
	void AddSongOnStart(const char* szSongName);
	void BASS_PlaySongFromFile(const char* szSong, bool bRepeat, float volume);
	void PlaySongOnLoading(const char* szFile);
	const char* GetPCUserNameF(void);
	const char* GetComputerNameF(void);
	const char* GetNumberOfProcessorsF(void);
	const char* RadioUpdateSongTitle(HSTREAM Stream);

	bool IsModValid(void); //check if mod is original
	void CalcScreenCoors(D3DXVECTOR3* vecWorld, D3DXVECTOR3* vecScreen);
	void CalcWorldCoors(D3DXVECTOR3* vecScreen, D3DXVECTOR3* vecWorld);

	void CreateThreadA(LPTHREAD_START_ROUTINE thread);
	//Memory stuff
	void* MemoryRead(void* dest, const void* src, size_t len);
	void* MemoryWrite(void* dest, const void* src, size_t len);
	int MemoryCompare(const void* orig, const void* comp, size_t len);
	void* MemoryCopy(void* dest, const void* src, size_t len, bool cmp = false, const void* cmpdata = nullptr);

private:
};
extern CSampMulti* pSampMulti;

class PlayerTrajectories
{
#pragma pack( push, 1 )
	struct stTrajectoryInfo
	{
		bool	enabled = false;
		DWORD	dwTick = 0;
		D3DXVECTOR3	begin;
		D3DXVECTOR3	end;
		D3DCOLOR color;
	};
#pragma pack( pop )
	int m_playerid;
	stTrajectoryInfo m_trajectories[20];
	const DWORD m_timeExists = 5000;
public:
	PlayerTrajectories(void);
	void add(int playerid, float origX, float origY, float origZ, float targetX, float targetY, float targetZ);
	void update(void);
	void clear(void);
};
extern PlayerTrajectories pBulletTracers[SAMP_MAX_PLAYERS];

struct stPlayerOutstreamPos
{
	float fpos[3];
};
extern stPlayerOutstreamPos pOutStreamPos[SAMP_MAX_PLAYERS];

struct stDamagerInfo
{
	bool bTarget[SAMP_MAX_PLAYERS];
	bool bEnableBackground,
		bShowInChat, bShowOnTarget; //imgui windows

	std::string dmg_info_delivered;
	std::string dmg_info_player;
	std::string dmg_info_weapon;
	std::string dmg_info_body_part;
	std::string dmg_info_distance;

};
extern struct stDamagerInfo __Damager;

//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// SUPPORT VARIABLES //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
struct stTranslateGTASAMP_vehiclePool
{
	// the index is the position in the vehicle pool
	//
	// translation data
	int iSAMPID[SAMP_MAX_VEHICLES];
};

struct stTranslateGTASAMP_pedPool
{
	// the index is the position in the ped pool
	//
	// translation data
	int iSAMPID[SAMP_MAX_PLAYERS];
};

//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// FUNCTIONS //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
void											update_translateGTASAMP_vehiclePool(void);
void											update_translateGTASAMP_pedPool(void);

void											getSamp();
uint32_t										getSampAddress();

struct stSAMP									*stGetSampInfo(void);
struct stChatInfo								*stGetSampChatInfo(void);
struct stInputInfo								*stGetInputInfo(void);
struct stKillInfo								*stGetKillInfo(void);
struct stScoreboardInfo							*stGetScoreboardInfo(void);
D3DCOLOR										samp_color_get(int id, DWORD trans = 0xFF000000);

int												getNthPlayerID(int n);
int												getPlayerCount(void);
int												getVehicleCount(void);

int												setLocalPlayerName(const char *name);
int												getPlayerPos(int iPlayerID, float fPos[3]);
const char										*getPlayerName(int iPlayerID);
int												getPlayerState(int iPlayerID);
int												getPlayerVehicleGTAScriptingID(int iPlayerID);
int												getPlayerSAMPVehicleID(int iPlayerID);
int												isBadSAMPVehicleID(int iVehicleID);
int												isBadSAMPPlayerID(int iPlayerID);

struct actor_info								*getGTAPedFromSAMPPlayerID(int iPlayerID);
struct vehicle_info								*getGTAVehicleFromSAMPVehicleID(int iPlayerID);
int												getSAMPPlayerIDFromGTAPed(struct actor_info *info);
int												getSAMPVehicleIDFromGTAVehicle(struct vehicle_info *pVehicle);
uint32_t										getPedGTAScriptingIDFromPlayerID(int iPlayerID);
uint32_t										getVehicleGTAScriptingIDFromVehicleID(int iVehicleID);

void											addClientCommand(char *text, CMDPROC function);
void											addToChatWindow(char *text, D3DCOLOR textColor, int playerID = -1);
void											addMessageToChatWindow(const char *text, ...);
void											addMessageToChatWindowSS(const char *text, ...);
void											restartGame();
void											addSayToChatWindow(char *msg);
void											say(char *text, ...);
void											playerSpawn(void);
void											disconnect(int reason);
void											setPassword(char *password);
void											sendSetInterior(uint8_t interiorID);
void											setSpecialAction(uint8_t byteSpecialAction);
void											sendSCMEvent(int iEvent, int iVehicleID, int iParam1, int iParam2);
void											toggleSAMPCursor(int iToggle);
void											sendDeath(void);
void											changeServer(const char *pszIp, unsigned ulPort, const char *pszPassword);
void											updateScoreboardData(void);
void											toggleOffScoreboard(void);

void											installSAMPHooks();
void											setSAMPCustomSendRates(int iOnFoot, int iInCar, int iAim, int iHeadSync);
int												sampPatchDisableNameTags(int iEnabled);
int												sampPatchDisableInteriorUpdate(int iEnabled);
int												sampPatchDisableScoreboardToggleOn(int iEnabled);
int												sampPatchDisableChatInputAdjust(int iEnabled);
void											sampPatchDisableAnticheat(void);

// global pointer externals
extern int										iIsSAMPSupported;
extern int										g_renderSAMP_initSAMPstructs;
extern struct stSAMP							*g_SAMP;
extern struct stPlayerPool						*g_Players;
extern struct stVehiclePool						*g_Vehicles;
extern struct stChatInfo						*g_Chat;
extern struct stInputInfo						*g_Input;
extern struct stKillInfo						*g_DeathList;
extern struct stScoreboardInfo					*g_Scoreboard;
extern struct stStreamedOutPlayerInfo			g_stStreamedOutInfo;

// global support variables externals
extern struct stTranslateGTASAMP_vehiclePool	translateGTASAMP_vehiclePool;
extern struct stTranslateGTASAMP_pedPool		translateGTASAMP_pedPool;

#ifdef SAMP_R1
extern char										g_m0dCmdlist[(SAMP_MAX_CLIENTCMDS - 22)][30];
#endif
#ifdef SAMP_R5
extern char										g_m0dCmdlist[(SAMP_MAX_CLIENTCMDS - 23)][30];
#endif

extern int										g_m0dCmdNum;

extern char mp3_duration[16];
extern HCHANNEL mp3_channel;
extern HSTREAM radio_channel;
extern std::list<std::string> gRaknetLogger;
