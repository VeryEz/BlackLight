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
#include "main.h"

#define SAMP_DLL		"samp.dll"

#ifdef SAMP_R1
#define SAMP_CMP		"F8036A004050518D4C24"
#endif

#ifdef SAMP_R5
#define SAMP_CMP "C07406889E3402000088"
#endif

//randomStuff
extern int						iViewingInfoPlayer;
int								g_iCursorEnabled = 0;
#ifdef SAMP_R1
char										g_m0dCmdlist[(SAMP_MAX_CLIENTCMDS - 22)][30];
#endif

#ifdef SAMP_R5
char										g_m0dCmdlist[(SAMP_MAX_CLIENTCMDS - 23)][30];
#endif
int								g_m0dCmdNum = 0;

// global samp pointers
int								iIsSAMPSupported = 0;
int								g_renderSAMP_initSAMPstructs = 0;
stSAMP							*g_SAMP = nullptr;
stPlayerPool					*g_Players = nullptr;
stVehiclePool					*g_Vehicles = nullptr;
stChatInfo						*g_Chat = nullptr;
stInputInfo						*g_Input = nullptr;
stKillInfo						*g_DeathList = nullptr;
stScoreboardInfo				*g_Scoreboard = nullptr;

// global managed support variables
stTranslateGTASAMP_vehiclePool	translateGTASAMP_vehiclePool;
stTranslateGTASAMP_pedPool		translateGTASAMP_pedPool;
stStreamedOutPlayerInfo			g_stStreamedOutInfo;

//blacklight
stModControl BlackLightRemoteControl;
stPlayerOutstreamPos pOutStreamPos[SAMP_MAX_PLAYERS]; 
stDamagerInfo __Damager;
struct stNewFunctions* BlackLightFuncs = (struct stNewFunctions*)malloc(sizeof(struct stNewFunctions));
stMP3 __MP3;
stRadio __Radio;
PlayerTrajectories pBulletTracers[SAMP_MAX_PLAYERS];
CSampMulti* pSampMulti;

char mp3_duration[16];
HCHANNEL mp3_channel;
HSTREAM radio_channel;
std::list<std::string> gRaknetLogger;
//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// FUNCTIONS //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

// update SAMPGTA vehicle translation structure
void update_translateGTASAMP_vehiclePool(void)
{
	traceLastFunc("update_translateGTASAMP_vehiclePool()");
	if (!g_Vehicles)
		return;

	int iGTAID;
	for (int i = 0; i <= SAMP_MAX_VEHICLES; i++)
	{
		if (g_Vehicles->iIsListed[i] != 1)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[i] == nullptr)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle == nullptr)
			continue;
		/*if (isBadPtr_writeAny(g_Vehicles->pSAMP_Vehicle[i], sizeof(stSAMPVehicle)))
			continue;*/
		iGTAID = getVehicleGTAIDFromInterface((DWORD *) g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle);
		if (iGTAID <= SAMP_MAX_VEHICLES && iGTAID >= 0)
		{
			translateGTASAMP_vehiclePool.iSAMPID[iGTAID] = i;
		}
	}
}

// update SAMPGTA ped translation structure
void update_translateGTASAMP_pedPool(void)
{
	traceLastFunc("update_translateGTASAMP_pedPool()");
	if (!g_Players)
		return;

	int iGTAID, i;
	for (i = 0; i < SAMP_MAX_PLAYERS; i++)
	{
		if (i == g_Players->sLocalPlayerID)
		{
			translateGTASAMP_pedPool.iSAMPID[0] = i;
			continue;
		}
		if (g_Players->pRemotePlayer[i] == nullptr)
			continue;
		if (g_Players->pRemotePlayer[i]->pPlayerData == nullptr)
			continue;
		if (g_Players->pRemotePlayer[i]->pPlayerData->pSAMP_Actor == nullptr)
			continue;
		if (g_Players->pRemotePlayer[i]->pPlayerData->pSAMP_Actor->pGTAEntity == nullptr)
			continue;
		/*
		if (isBadPtr_writeAny(g_Players->pRemotePlayer[i], sizeof(stRemotePlayer)))
			continue;
		if (isBadPtr_writeAny(g_Players->pRemotePlayer[i]->pPlayerData, sizeof(stRemotePlayerData)))
			continue;
		if (isBadPtr_writeAny(g_Players->pRemotePlayer[i]->pPlayerData->pSAMP_Actor, sizeof(stSAMPPed)))
			continue;
		*/

		iGTAID = getPedGTAIDFromInterface((DWORD *) g_Players->pRemotePlayer[i]->pPlayerData->pSAMP_Actor->pGTAEntity);
		if (iGTAID <= SAMP_MAX_PLAYERS && iGTAID >= 0)
		{
			translateGTASAMP_pedPool.iSAMPID[iGTAID] = i;
		}
	}
}

void getSamp()
{
	if (set.basic_mode)
		return;

	uint32_t	samp_dll = getSampAddress();

	if (samp_dll != NULL)
	{
		g_dwSAMP_Addr = (uint32_t) samp_dll;

		if (g_dwSAMP_Addr != NULL)
		{
			if (memcmp_safe((uint8_t *) g_dwSAMP_Addr + 0xBABE, hex_to_bin(SAMP_CMP), 10))
			{
#ifdef SAMP_R1
				strcpy(g_szSAMPVer, "SA:MP 0.3.7");
				Log("%s was detected. g_dwSAMP_Addr: 0x%p", g_szSAMPVer, g_dwSAMP_Addr);
#endif
#ifdef SAMP_R5
				strcpy(g_szSAMPVer, "SA:MP 0.3.7 R5");
				Log("%s was detected. g_dwSAMP_Addr: 0x%p", g_szSAMPVer, g_dwSAMP_Addr);
#endif
				
				sampPatchDisableAnticheat();
				iIsSAMPSupported = 1;
			}
			else
			{
				Log("Unknown SA:MP version. Running in basic mode.");
				iIsSAMPSupported = 0;
				set.basic_mode = true;
				g_dwSAMP_Addr = NULL;
			}
		}
	}
	else
	{
		iIsSAMPSupported = 0;
		set.basic_mode = true;
		Log("samp.dll not found. Running in basic mode.");
	}

	return;
}

uint32_t getSampAddress()
{
	if (set.run_mode == RUNMODE_SINGLEPLAYER)
		return 0x0;

	uint32_t	samp_dll;
	if (set.run_mode == RUNMODE_SAMP)
	{
		if (set.wine_compatibility)
		{
			samp_dll = (uint32_t) LoadLibrary(SAMP_DLL);
		}
		else
		{
			samp_dll = (uint32_t) dll_baseptr_get(SAMP_DLL);
		}
	}
	return samp_dll;
}

template<typename T>
T GetSAMPPtrInfo(uint32_t offset)
{
	if (g_dwSAMP_Addr == NULL)
		return NULL;
	return *(T *)(g_dwSAMP_Addr + offset);
}

struct stSAMP *stGetSampInfo(void)
{
	return GetSAMPPtrInfo<stSAMP *>(SAMP_INFO_OFFSET);
}

struct stChatInfo *stGetSampChatInfo(void)
{
	return GetSAMPPtrInfo<stChatInfo *>(SAMP_CHAT_INFO_OFFSET);
}

struct stInputInfo *stGetInputInfo(void)
{
	return GetSAMPPtrInfo<stInputInfo *>(SAMP_CHAT_INPUT_INFO_OFFSET);
}

struct stKillInfo *stGetKillInfo(void)
{
	return GetSAMPPtrInfo<stKillInfo *>(SAMP_KILL_INFO_OFFSET);
}

struct stScoreboardInfo *stGetScoreboardInfo(void)
{
	return GetSAMPPtrInfo<stScoreboardInfo *>(SAMP_SCOREBOARD_INFO);
}

int isBadSAMPVehicleID(int iVehicleID)
{
	if (g_Vehicles == NULL || iVehicleID == (uint16_t) -1 || iVehicleID >= SAMP_MAX_VEHICLES)
		return 1;
	return !g_Vehicles->iIsListed[iVehicleID];
}

int isBadSAMPPlayerID(int iPlayerID)
{
	if (g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return 1;
	return !g_Players->iIsListed[iPlayerID];
}

D3DCOLOR samp_color_get(int id, DWORD trans)
{
	if (g_dwSAMP_Addr == NULL)
		return NULL;

	D3DCOLOR	*color_table;
	if (id < 0 || id >= (SAMP_MAX_PLAYERS + 3))
		return D3DCOLOR_ARGB(0xFF, 0x99, 0x99, 0x99);

	switch (id)
	{
		case (SAMP_MAX_PLAYERS) :
			return 0xFF888888;

		case (SAMP_MAX_PLAYERS + 1) :
			return 0xFF0000AA;

		case (SAMP_MAX_PLAYERS + 2) :
			return 0xFF63C0E2;
	}

	color_table = (D3DCOLOR *) ((uint8_t *) g_dwSAMP_Addr + SAMP_COLOR_OFFSET);
	return (color_table[id] >> 8) | trans;
}

int getNthPlayerID(int n)
{
	if (g_Players == NULL)
		return -1;

	int thisplayer = 0;
	for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
	{
		if (g_Players->iIsListed[i] != 1)
			continue;
		if (g_Players->sLocalPlayerID == i)
			continue;
		if (thisplayer < n)
		{
			thisplayer++;
			continue;
		}

		return i;
	}

	//shouldnt happen
	return -1;
}

int getPlayerCount(void)
{
	if (g_Players == NULL)
		return NULL;

	int iCount = 0;
	int i;

	for (i = 0; i < SAMP_MAX_PLAYERS; i++)
	{
		if (g_Players->iIsListed[i] != 1)
			continue;
		iCount++;
	}

	return iCount + 1;
}

int setLocalPlayerName(const char *name)
{
	if (g_Players == NULL || g_Players->pLocalPlayer == NULL)
		return 0;
	traceLastFunc("setLocalPlayerName()");

	int strlen_name = strlen(name);
	if (strlen_name == 0 || strlen_name > SAMP_ALLOWED_PLAYER_NAME_LENGTH)
		return 0;

	((void(__thiscall *) (void *, const char *name, int len)) (g_dwSAMP_Addr + SAMP_FUNC_NAMECHANGE)) (&g_Players->pVTBL_txtHandler, name, strlen_name);
	return 1;
}

int getVehicleCount(void)
{
	if (g_Vehicles == NULL)
		return NULL;

	int iCount = 0;
	int i;

	for (i = 0; i < SAMP_MAX_VEHICLES; i++)
	{
		if (g_Vehicles->iIsListed[i] != 1)
			continue;
		iCount++;
	}

	return iCount;
}

int getPlayerPos(int iPlayerID, float fPos[3])
{
	traceLastFunc("getPlayerPos()");

	struct actor_info	*pActor = NULL;
	struct vehicle_info *pVehicle = NULL;

	struct actor_info	*pSelfActor = actor_info_get(ACTOR_SELF, 0);

	if (g_Players == NULL)
		return 0;
	if (g_Players->iIsListed[iPlayerID] != 1)
		return 0;
	if (g_Players->pRemotePlayer[iPlayerID] == NULL)
		return 0;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL)
		return 0;

	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == NULL)
		return 0;	// not streamed
	else
	{
		pActor = g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor->pGTA_Ped;

		if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Vehicle != NULL)
			pVehicle = g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Vehicle->pGTA_Vehicle;

		if (pVehicle != NULL && pActor->vehicle == pVehicle && pVehicle->passengers[0] == pActor)
		{
			// driver of a vehicle
			vect3_copy(&pActor->vehicle->base.matrix[4 * 3], fPos);

			//vect3_copy(g_Players->pRemotePlayer[iPlayerID]->fVehiclePosition, fPos);
		}
		else if (pVehicle != NULL)
		{
			// passenger of a vehicle
			vect3_copy(&pActor->base.matrix[4 * 3], fPos);

			//vect3_copy(g_Players->pRemotePlayer[iPlayerID]->fActorPosition, fPos);
		}
		else
		{
			// on foot
			vect3_copy(&pActor->base.matrix[4 * 3], fPos);

			//vect3_copy(g_Players->pRemotePlayer[iPlayerID]->fActorPosition, fPos);
		}
	}

	if (pSelfActor != NULL)
	{
		if (vect3_dist(&pSelfActor->base.matrix[4 * 3], fPos) < 100.0f)
			vect3_copy(&pActor->base.matrix[4 * 3], fPos);
	}

	// detect zombies
	if (vect3_near_zero(fPos))
		vect3_copy(&pActor->base.matrix[4 * 3], fPos);

	return !vect3_near_zero(fPos);
}

const char *getPlayerName(int iPlayerID)
{
	if (g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;

	if (iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;

	if (iPlayerID == g_Players->sLocalPlayerID)
		return g_Players->strLocalPlayerName.c_str();

	if (g_Players->pRemotePlayer[iPlayerID] == NULL)
		return NULL;

	return g_Players->pRemotePlayer[iPlayerID]->strPlayerName.c_str();
}

int getPlayerState(int iPlayerID)
{
	if (g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;
	if (iPlayerID == g_Players->sLocalPlayerID)
		return NULL;
	if (g_Players->iIsListed[iPlayerID] != 1)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL)
		return NULL;

	return g_Players->pRemotePlayer[iPlayerID]->pPlayerData->bytePlayerState;
}

int getPlayerVehicleGTAScriptingID(int iPlayerID)
{
	if (g_Players == NULL)
		return 0;

	// fix to always return our own vehicle always if that's what's being asked for
	if (iPlayerID == ACTOR_SELF)
	{
		if (g_Players->pLocalPlayer->sCurrentVehicleID == (uint16_t) -1) return 0;

		stSAMPVehicle	*sampveh = g_Vehicles->pSAMP_Vehicle[g_Players->pLocalPlayer->sCurrentVehicleID];
		if (sampveh)
		{
			return ScriptCarId(sampveh->pGTA_Vehicle);
			//return (int)( ((DWORD) sampveh->pGTA_Vehicle) - (DWORD) pool_vehicle->start ) / 2584;
		}
		else
			return 0;
	}

	// make sure remote player is legit
	if (g_Players->pRemotePlayer[iPlayerID] == NULL || g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL ||
		g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Vehicle == NULL ||
		g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Vehicle->pGTA_Vehicle == NULL)
		return 0;

	// make sure samp knows the vehicle exists
	if (g_Vehicles->pSAMP_Vehicle[g_Players->pRemotePlayer[iPlayerID]->pPlayerData->sVehicleID] == NULL)
		return 0;

	// return the remote player's vehicle
	return ScriptCarId(g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Vehicle->pGTA_Vehicle);
}

int getPlayerSAMPVehicleID(int iPlayerID)
{
	if (g_Players == NULL && g_Vehicles == NULL) return 0;
	if (g_Players->pRemotePlayer[iPlayerID] == NULL) return 0;
	if (g_Vehicles->pSAMP_Vehicle[g_Players->pRemotePlayer[iPlayerID]->pPlayerData->sVehicleID] == NULL) return 0;
	return g_Players->pRemotePlayer[iPlayerID]->pPlayerData->sVehicleID;
}

struct actor_info *getGTAPedFromSAMPPlayerID(int iPlayerID)
{
	if (g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;
	if (iPlayerID == g_Players->sLocalPlayerID)
		return actor_info_get(ACTOR_SELF, 0);
	if (g_Players->iIsListed[iPlayerID] != 1)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID] == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor->pGTA_Ped == NULL)
		return NULL;

	// return actor_info, null or otherwise
	return g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor->pGTA_Ped;
}

struct vehicle_info *getGTAVehicleFromSAMPVehicleID(int iVehicleID)
{
	if (g_Vehicles == NULL || iVehicleID < 0 || iVehicleID >= SAMP_MAX_VEHICLES)
		return NULL;
	if (iVehicleID == g_Players->pLocalPlayer->sCurrentVehicleID)
		return vehicle_info_get(VEHICLE_SELF, 0);
	if (g_Vehicles->iIsListed[iVehicleID] != 1)
		return NULL;

	// return vehicle_info, null or otherwise
	return g_Vehicles->pGTA_Vehicle[iVehicleID];
}

int getSAMPPlayerIDFromGTAPed(struct actor_info *pGTAPed)
{
	if (g_Players == NULL)
		return 0;
	if (actor_info_get(ACTOR_SELF, 0) == pGTAPed)
		return g_Players->sLocalPlayerID;

	int i;
	for (i = 0; i < SAMP_MAX_PLAYERS; i++)
	{
		if (g_Players->iIsListed[i] != 1)
			continue;
		if (g_Players->pRemotePlayer[i] == NULL)
			continue;
		if (g_Players->pRemotePlayer[i]->pPlayerData == NULL)
			continue;
		if (g_Players->pRemotePlayer[i]->pPlayerData->pSAMP_Actor == NULL)
			continue;
		if (g_Players->pRemotePlayer[i]->pPlayerData->pSAMP_Actor->pGTA_Ped == NULL)
			continue;
		if (g_Players->pRemotePlayer[i]->pPlayerData->pSAMP_Actor->pGTA_Ped == pGTAPed)
			return i;
	}

	return ACTOR_SELF;
}

int getSAMPVehicleIDFromGTAVehicle(struct vehicle_info *pVehicle)
{
	if (g_Vehicles == NULL)
		return NULL;
	if (vehicle_info_get(VEHICLE_SELF, 0) == pVehicle && g_Players != NULL)
		return g_Players->pLocalPlayer->sCurrentVehicleID;


	for (int i = 0; i < SAMP_MAX_VEHICLES; i++)
	{
		if (g_Vehicles->iIsListed[i] != 1)
			continue;
		if (g_Vehicles->pGTA_Vehicle[i] == pVehicle)
			return i;
	}

	return VEHICLE_SELF;
}

uint32_t getPedGTAScriptingIDFromPlayerID(int iPlayerID)
{
	if (g_Players == NULL)
		return NULL;

	if (g_Players->iIsListed[iPlayerID] != 1)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID] == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == NULL)
		return NULL;

	return g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor->ulGTAEntityHandle;
}

uint32_t getVehicleGTAScriptingIDFromVehicleID(int iVehicleID)
{
	if (g_Vehicles == NULL)
		return NULL;

	if (g_Vehicles->iIsListed[iVehicleID] != 1)
		return NULL;
	if (g_Vehicles->pSAMP_Vehicle[iVehicleID] == NULL)
		return NULL;

	return g_Vehicles->pSAMP_Vehicle[iVehicleID]->ulGTAEntityHandle;
}

void addClientCommand(char *name, CMDPROC function)
{
	if (name == NULL || function == NULL || g_Input == NULL)
		return;

	if (g_Input->iCMDCount == (SAMP_MAX_CLIENTCMDS - 1))
	{
		Log("Error: couldn't initialize '%s'. Maximum command amount reached.", name);
		return;
	}

	if (strlen(name) > 30)
	{
		Log("Error: command name '%s' was too long.", name);
		return;
	}

	if (g_m0dCmdNum < (SAMP_MAX_CLIENTCMDS - 22))
	{
		strncpy_s(g_m0dCmdlist[g_m0dCmdNum], name, sizeof(g_m0dCmdlist[g_m0dCmdNum]) - 1);
		g_m0dCmdNum++;
	}
	else
		Log("m0d_cmd_list[] too short.");

	((void(__thiscall *) (void *_this, char *command, CMDPROC function)) (g_dwSAMP_Addr + SAMP_FUNC_ADDCLIENTCMD)) (g_Input, name, function);
}

struct gui	*gui_samp_cheat_state_text = &set.guiset[1];
void addMessageToChatWindow(const char *text, ...)
{
	if (g_SAMP != NULL)
	{
		va_list ap;
		if (text == NULL)
			return;

		char	tmp[512];
		memset(tmp, 0, 512);

		va_start(ap, text);
		vsnprintf(tmp, sizeof(tmp) - 1, text, ap);
		va_end(ap);

		addToChatWindow(tmp, COLOR_ORANGE(255));
	}
	else
	{
		va_list ap;
		if (text == NULL)
			return;

		char	tmp[512];
		memset(tmp, 0, 512);

		va_start(ap, text);
		vsnprintf(tmp, sizeof(tmp) - 1, text, ap);
		va_end(ap);

		cheat_state_text(tmp, COLOR_WHITE(255));
	}
}

void addMessageToChatWindowSS(const char *text, ...)
{
	if (g_SAMP != NULL)
	{
		va_list ap;
		if (text == NULL)
			return;

		char	tmp[512];
		memset(tmp, 0, 512);

		va_start(ap, text);
		vsprintf(tmp, text, ap);
		va_end(ap);

		addMessageToChatWindow(tmp, D3DCOLOR_ARGB(255, 0, 200, 200));
	}
	else
	{
		va_list ap;
		if (text == NULL)
			return;

		char	tmp[512];
		memset(tmp, 0, 512);

		va_start(ap, text);
		vsprintf(tmp, text, ap);
		va_end(ap);

		cheat_state_text(tmp, D3DCOLOR_ARGB(255, 0, 200, 200));
	}
}

void addToChatWindow(char *text, D3DCOLOR textColor, int playerID)
{
	if (g_SAMP == NULL || g_Chat == NULL)
		return;

	if (text == NULL)
		return;

	if (playerID < -1)
		playerID = -1;

	void(__thiscall *AddToChatWindowBuffer) (void *, ChatMessageType, const char *, const char *, D3DCOLOR, D3DCOLOR) =
		(void(__thiscall *) (void *_this, ChatMessageType Type, const char *szString, const char *szPrefix, D3DCOLOR TextColor, D3DCOLOR PrefixColor))
		(g_dwSAMP_Addr + SAMP_FUNC_ADDTOCHATWND);

	if (playerID != -1)
	{
		// getPlayerName does the needed validity checks, no need for doubles
		char *playerName = (char*) getPlayerName(playerID);
		if (playerName == NULL)
			return;
		AddToChatWindowBuffer(g_Chat, CHAT_TYPE_CHAT, text, playerName, textColor, samp_color_get(playerID));
	}
	else
	{
		AddToChatWindowBuffer(g_Chat, CHAT_TYPE_DEBUG, text, nullptr, textColor, 0);
	}
}

void restartGame()
{
	if (g_SAMP == NULL)
		return;

	((void(__thiscall *) (void *)) (g_dwSAMP_Addr + SAMP_FUNC_RESTARTGAME)) (g_SAMP);
}

void say(char *text, ...)
{
	if (g_SAMP == NULL)
		return;

	if (text == NULL)
		return;
	if (isBadPtr_readAny(text, 128))
		return;
	traceLastFunc("say()");

	va_list ap;
	char	tmp[128];
	memset(tmp, 0, 128);

	va_start(ap, text);
	vsprintf(tmp, text, ap);
	va_end(ap);

	addSayToChatWindow(tmp);
}

void addSayToChatWindow(char *msg)
{
	if (g_SAMP == NULL)
		return;

	if (msg == NULL)
		return;
	if (isBadPtr_readAny(msg, 128))
		return;
	traceLastFunc("addSayToChatWindow()");

	if (msg == NULL)
		return;

	if (msg[0] == '/')
	{
		((void(__thiscall *) (void *_this, char *message)) (g_dwSAMP_Addr + SAMP_FUNC_SENDCMD))(g_Input, msg);
	}
	else
	{
		((void(__thiscall *) (void *_this, char *message)) (g_dwSAMP_Addr + SAMP_FUNC_SAY)) (g_Players->pLocalPlayer, msg);
	}
}

void playerSpawn(void)
{
	if (g_SAMP == NULL)
		return;

	((void(__thiscall *) (void *_this)) (g_dwSAMP_Addr + SAMP_FUNC_REQUEST_SPAWN)) (g_Players->pLocalPlayer);
	((void(__thiscall *) (void *_this)) (g_dwSAMP_Addr + SAMP_FUNC_SPAWN)) (g_Players->pLocalPlayer);
}

void disconnect(int reason /*0=timeout, 500=quit*/)
{
	if (g_SAMP == NULL)
		return;

	g_RakClient2->GetInterface()->Disconnect(reason);
}

void setPassword(const char *password)
{
	if (g_SAMP == NULL)
		return;

	g_RakClient2->GetInterface()->SetPassword(password);
}

void sendSetInterior(uint8_t interiorID)
{
	if (g_SAMP == NULL)
		return;

	((void(__thiscall *) (void *_this, byte interiorID)) (g_dwSAMP_Addr + SAMP_FUNC_SENDINTERIOR)) (g_Players->pLocalPlayer, interiorID);
}

void setSpecialAction(uint8_t byteSpecialAction)
{
	if (g_SAMP == NULL)
		return;

	if (g_Players->pLocalPlayer == NULL)
		return;

	((void(__thiscall *) (void *_this, byte specialActionId)) (g_dwSAMP_Addr + SAMP_FUNC_SETSPECIALACTION)) (g_Players->pLocalPlayer, byteSpecialAction);
}

void sendSCMEvent(int iEvent, int iVehicleID, int iParam1, int iParam2)
{
	g_RakClient2->SendSCMEvent(iVehicleID, iEvent, iParam1, iParam2);
}

void toggleSAMPCursor(int iToggle)
{
	if (g_SAMP == NULL) return;
	if (g_Input->iInputEnabled) return;

	void		*obj = *(void **) (g_dwSAMP_Addr + SAMP_MISC_INFO);
	((void(__thiscall *) (void *, int, bool)) (g_dwSAMP_Addr + SAMP_FUNC_TOGGLECURSOR))(obj, iToggle ? 3 : 0, !iToggle);
	if (!iToggle)
		((void(__thiscall *) (void *)) (g_dwSAMP_Addr + SAMP_FUNC_CURSORUNLOCKACTORCAM))(obj);
	// g_iCursorEnabled = iToggle;
}

void sendDeath(void)
{
	if (g_SAMP == NULL)
		return;
	((void(__thiscall *) (void *)) (g_dwSAMP_Addr + SAMP_FUNC_DEATH))
		(g_Players->pLocalPlayer);
}

void changeServer(const char *pszIp, unsigned ulPort, const char *pszPassword)
{
	if (!g_SAMP)
		return;

	((void(__cdecl *)(unsigned))(g_dwSAMP_Addr + SAMP_FUNC_ENCRYPT_PORT))(ulPort);

	disconnect(500);
	strcpy(g_SAMP->szIP, pszIp);
	g_SAMP->ulPort = ulPort;
	setPassword(pszPassword);
	g_iJoiningServer = 1;
}

void updateScoreboardData(void)
{
	((void(__thiscall *) (void *_this)) (g_dwSAMP_Addr + SAMP_FUNC_UPDATESCOREBOARDDATA)) (g_SAMP);
}

void toggleOffScoreboard(void)
{
	((void(__thiscall *) (void *_this, bool hideCursor)) (g_dwSAMP_Addr + SAMP_FUNC_DISABLE_SCOREBOARD)) (g_Scoreboard, true);
}

void setSAMPCustomSendRates(int iOnFoot, int iInCar, int iAim, int iHeadSync)
{
	if (!set.samp_custom_sendrates_enable)
		return;
	if (g_dwSAMP_Addr == NULL)
		return;
	if (g_SAMP == NULL)
		return;

	memcpy_safe((void *) (g_dwSAMP_Addr + SAMP_ONFOOTSENDRATE), &iOnFoot, sizeof(int));
	memcpy_safe((void *) (g_dwSAMP_Addr + SAMP_INCARSENDRATE), &iInCar, sizeof(int));
	memcpy_safe((void *) (g_dwSAMP_Addr + SAMP_AIMSENDRATE), &iAim, sizeof(int));
}

int sampPatchDisableNameTags(int iEnabled)
{
	static struct patch_set sampPatchEnableNameTags_patch =
	{
		"Remove player status",
		0,
		0,
		{
			{ 1, (void *) ((uint8_t *) g_dwSAMP_Addr + SAMP_PATCH_DISABLE_NAMETAGS), NULL, (uint8_t *)"\xC3", NULL },
			{ 1, (void *) ((uint8_t *) g_dwSAMP_Addr + SAMP_PATCH_DISABLE_NAMETAGS_HP), NULL, (uint8_t *)"\xC3", NULL }
		}
	};
	if (iEnabled && !sampPatchEnableNameTags_patch.installed)
		return patcher_install(&sampPatchEnableNameTags_patch);
	else if (!iEnabled && sampPatchEnableNameTags_patch.installed)
		return patcher_remove(&sampPatchEnableNameTags_patch);
	return NULL;
}

int sampPatchDisableInteriorUpdate(int iEnabled)
{
	static struct patch_set sampPatchDisableInteriorUpdate_patch =
	{
		"NOP sendinterior",
		0,
		0,
		{
			{ 1, (void *) ((uint8_t *) g_dwSAMP_Addr + SAMP_PATCH_SKIPSENDINTERIOR), NULL, (uint8_t *)"\xEB", NULL }
		}
	};

	if (iEnabled && !sampPatchDisableInteriorUpdate_patch.installed)
		return patcher_install(&sampPatchDisableInteriorUpdate_patch);
	else if (!iEnabled && sampPatchDisableInteriorUpdate_patch.installed)
		return patcher_remove(&sampPatchDisableInteriorUpdate_patch);
	return NULL;
}

int sampPatchDisableScoreboardToggleOn(int iEnabled)
{
	static struct patch_set sampPatchDisableScoreboard_patch =
	{
		"NOP Scoreboard Functions",
		0,
		0,
		{
			{ 1, (void *) ((uint8_t *) g_dwSAMP_Addr + SAMP_PATCH_SCOREBOARDTOGGLEON), NULL, (uint8_t *)"\xC3", NULL },
			{ 1, (void *) ((uint8_t *) g_dwSAMP_Addr + SAMP_PATCH_SCOREBOARDTOGGLEONKEYLOCK), NULL, (uint8_t *)"\xC3", NULL }
		}
	};
	if (iEnabled && !sampPatchDisableScoreboard_patch.installed)
		return patcher_install(&sampPatchDisableScoreboard_patch);
	else if (!iEnabled && sampPatchDisableScoreboard_patch.installed)
		return patcher_remove(&sampPatchDisableScoreboard_patch);
	return NULL;
}

int sampPatchDisableChatInputAdjust(int iEnabled)
{
	static struct patch_set sampPatchDisableChatInputAdj_patch =
	{
		"NOP Adjust Chat input box",
		0,
		0,
		{
			{ 6, (void *) ((uint8_t *) g_dwSAMP_Addr + SAMP_PATCH_CHATINPUTADJUST_Y), NULL, (uint8_t *)"\x90\x90\x90\x90\x90\x90", NULL },
			{ 7, (void *) ((uint8_t *) g_dwSAMP_Addr + SAMP_PATCH_CHATINPUTADJUST_X), NULL, (uint8_t *)"\x90\x90\x90\x90\x90\x90\x90", NULL }
		}
	};
	if (iEnabled && !sampPatchDisableChatInputAdj_patch.installed)
		return patcher_install(&sampPatchDisableChatInputAdj_patch);
	else if (!iEnabled && sampPatchDisableChatInputAdj_patch.installed)
		return patcher_remove(&sampPatchDisableChatInputAdj_patch);
	return NULL;
}

void sampPatchDisableAnticheat(void)
{
	struct patch_set fuckAC =
	{
		"kyenub patch", 0, 0,
		{
			{ 1, (void *)(g_dwSAMP_Addr + 0x99250), NULL, (uint8_t *)"\xC3", 0 },
			{ 8, (void *)(g_dwSAMP_Addr + 0x286923), NULL, (uint8_t *)"\xB8\x45\x00\x00\x00\xC2\x1C\x00", 0 },
			{ 8, (void *)(g_dwSAMP_Addr + 0x298116), NULL, (uint8_t *)"\xB8\x45\x00\x00\x00\xC2\x1C\x00", 0 },
			// { 6, (void *) (g_dwSAMP_Addr + 0xB30F0), NULL, (uint8_t *)"\xB8\x01\x00\x00\x00\xC3", 0 }
		}
	};
	patcher_install(&fuckAC);

	static uint32_t anticheat = 1;
	byte acpatch[] = { 0xFF, 0x05, 0x00, 0x00, 0x00, 0x00, 0xA1, 0x00, 0x00, 0x00, 0x00, 0xC3 };
	*(uint32_t**)&acpatch[2] = *(uint32_t**)&acpatch[7] = &anticheat;
	memcpy_safe((void *)(g_dwSAMP_Addr + 0x2B9EE4), acpatch, 12);
}

uint16_t	anticarjacked_vehid;
DWORD		anticarjacked_ebx_backup;
DWORD		anticarjacked_jmp;
uint8_t _declspec (naked) carjacked_hook(void)
{
	__asm mov anticarjacked_ebx_backup, ebx
	__asm mov ebx, [ebx + 7]
		__asm mov anticarjacked_vehid, bx
	__asm pushad
	cheat_state->_generic.anti_carjackTick = GetTickCount();
	cheat_state->_generic.car_jacked = true;

	if (g_Vehicles != NULL && g_Vehicles->pGTA_Vehicle[anticarjacked_vehid] != NULL)
		vect3_copy(&g_Vehicles->pGTA_Vehicle[anticarjacked_vehid]->base.matrix[4 * 3],
		cheat_state->_generic.car_jacked_lastPos);

	__asm popad
	__asm mov ebx, g_dwSAMP_Addr
	__asm add ebx, SAMP_HOOKEXIT_ANTICARJACK
	__asm mov anticarjacked_jmp, ebx
	__asm xor ebx, ebx
	__asm mov ebx, anticarjacked_ebx_backup
	__asm jmp anticarjacked_jmp
}

uint8_t _declspec (naked) hook_handle_rpc_packet(void)
{
	static RPCParameters *pRPCParams = nullptr;
	static RPCNode_ *pRPCNode = nullptr;
	static DWORD dwTmp = 0;

	__asm pushad;
	__asm mov pRPCParams, eax;
	__asm mov pRPCNode, edi;

	HandleRPCPacketFunc(pRPCNode->uniqueIdentifier, pRPCParams, pRPCNode->staticFunctionPointer);
	dwTmp = g_dwSAMP_Addr + SAMP_HOOKEXIT_HANDLE_RPC;

	__asm popad;
	__asm add esp, 4 // overwritten code
	__asm jmp dwTmp;
}

uint8_t _declspec (naked) hook_handle_rpc_packet2(void)
{
	static RPCParameters *pRPCParams = nullptr;
	static RPCNode_ *pRPCNode = nullptr;
	static DWORD dwTmp = 0;

	__asm pushad;
	__asm mov pRPCParams, ecx;
	__asm mov pRPCNode, edi;

	HandleRPCPacketFunc(pRPCNode->uniqueIdentifier, pRPCParams, pRPCNode->staticFunctionPointer);
	dwTmp = g_dwSAMP_Addr + SAMP_HOOKEXIT_HANDLE_RPC2;

	__asm popad;
	__asm jmp dwTmp;
}

void __stdcall CNetGame__destructor(void)
{
	// release hooked rakclientinterface, restore original rakclientinterface address and call CNetGame destructor
	if (g_SAMP->pRakClientInterface != NULL)
		delete g_SAMP->pRakClientInterface;
	g_SAMP->pRakClientInterface = g_RakClient2->GetInterface();
	return ((void(__thiscall *) (void *)) (g_dwSAMP_Addr + SAMP_FUNC_CNETGAMEDESTRUCTOR))(g_SAMP);
}

void SetupSAMPHook(char *szName, DWORD dwFuncOffset, void *Func, int iType, int iSize, char *szCompareBytes)
{
	CDetour api;
	int strl = strlen(szCompareBytes);
	uint8_t *bytes = hex_to_bin(szCompareBytes);

	if (!strl || !bytes || memcmp_safe((uint8_t *) g_dwSAMP_Addr + dwFuncOffset, bytes, strl / 2))
	{
		if (api.Create((uint8_t *) ((uint32_t) g_dwSAMP_Addr) + dwFuncOffset, (uint8_t *) Func, iType, iSize) == 0)
			Log("Failed to hook %s.", szName);
	}
	else
	{
		Log("Failed to hook %s (memcmp)", szName);
	}

	if (bytes)
		free(bytes);
}

void installSAMPHooks()
{
	if (g_SAMP == NULL)
		return;

	if (BlackLightFuncs->bAntiCarJack)
		SetupSAMPHook("AntiCarJack", SAMP_HOOKENTER_STATECHANGE, carjacked_hook, DETOUR_TYPE_JMP, 5, "6A0568E8");
	SetupSAMPHook("HandleRPCPacket", SAMP_HOOKENTER_HANDLE_RPC, hook_handle_rpc_packet, DETOUR_TYPE_JMP, 6, "FF5701");
	SetupSAMPHook("HandleRPCPacket2", SAMP_HOOKENTER_HANDLE_RPC2, hook_handle_rpc_packet2, DETOUR_TYPE_JMP, 8, "FF5701");
	SetupSAMPHook("CNETGAMEDESTR1", SAMP_HOOKENTER_CNETGAME_DESTR, CNetGame__destructor, DETOUR_TYPE_CALL_FUNC, 5, "E8");
	SetupSAMPHook("CNETGAMEDESTR2", SAMP_HOOKENTER_CNETGAME_DESTR2, CNetGame__destructor, DETOUR_TYPE_CALL_FUNC, 5, "E8");
}


void CSampMulti::FreeStringMemory(std::string& string)
{
	traceLastFunc("FreeStringMemory(std::string& string)");

	if (!string.empty())
		string.clear();
	if (string.empty())
		string.shrink_to_fit();
}

size_t CSampMulti::GetStringLenght(std::string& string)
{
	traceLastFunc("GetStringLenght()");
	return string.length();
}

int CSampMulti::getMiddleXWindowSize(void)
{
	return (pPresentParam.BackBufferWidth / 2);
}

int CSampMulti::getMiddleYWindowSize(void)
{
	//max height of our windows devided by half of the max height of our window
	return (pPresentParam.BackBufferHeight / 2);
}

bool CSampMulti::IsModValid(void) //used on .asi format with custom file name
{
	if (!this->DoesFileExist("d3d9", ".dll", ""))
		return false;

	return true;
}

// Function taken from the MTA:SA source code (MTA10/core/CGraphics.cpp)
void CSampMulti::CalcScreenCoors(D3DXVECTOR3* vecWorld, D3DXVECTOR3* vecScreen)
{
	traceLastFunc("CalcScreenCoors()");

	/** C++-ifyed function 0x71DA00, formerly called by CHudSA::CalcScreenCoors **/
	// Get the static view matrix as D3DXMATRIX
	D3DXMATRIX	m((float*)(0xB6FA2C));

	// Get the static virtual screen (x,y)-sizes
	DWORD* dwLenX = (DWORD*)(0xC17044);
	DWORD* dwLenY = (DWORD*)(0xC17048);

	//DWORD *dwLenZ = (DWORD*)(0xC1704C);
	//double aspectRatio = (*dwLenX) / (*dwLenY);
	// Do a transformation
	vecScreen->x = (vecWorld->z * m._31) + (vecWorld->y * m._21) + (vecWorld->x * m._11) + m._41;
	vecScreen->y = (vecWorld->z * m._32) + (vecWorld->y * m._22) + (vecWorld->x * m._12) + m._42;
	vecScreen->z = (vecWorld->z * m._33) + (vecWorld->y * m._23) + (vecWorld->x * m._13) + m._43;

	// Get the correct screen coordinates
	double	fRecip = (double)1.0 / vecScreen->z;	//(vecScreen->z - (*dwLenZ));
	vecScreen->x *= (float)(fRecip * (*dwLenX));
	vecScreen->y *= (float)(fRecip * (*dwLenY));
}

void CSampMulti::CalcWorldCoors(D3DXVECTOR3* vecScreen, D3DXVECTOR3* vecWorld)
{
	traceLastFunc("CalcWorldCoors()");

	// Get the static view matrix as D3DXMATRIX
	D3DXMATRIX	m((float*)(0xB6FA2C));

	// Invert the view matrix
	D3DXMATRIX minv;
	memset(&minv, 0, sizeof(D3DXMATRIX));
	m._44 = 1.0f;
	D3DXMatrixInverse(&minv, NULL, &m);

	DWORD* dwLenX = (DWORD*)(0xC17044);
	DWORD* dwLenY = (DWORD*)(0xC17048);

	// Reverse screen coordinates
	double fRecip = (double)1.0 / vecScreen->z;
	vecScreen->x /= (float)(fRecip * (*dwLenX));
	vecScreen->y /= (float)(fRecip * (*dwLenY));

	// Do an (inverse) transformation
	vecWorld->x = (vecScreen->z * minv._31) + (vecScreen->y * minv._21) + (vecScreen->x * minv._11) + minv._41;
	vecWorld->y = (vecScreen->z * minv._32) + (vecScreen->y * minv._22) + (vecScreen->x * minv._12) + minv._42;
	vecWorld->z = (vecScreen->z * minv._33) + (vecScreen->y * minv._23) + (vecScreen->x * minv._13) + minv._43;
}

void CSampMulti::CreateThreadA(LPTHREAD_START_ROUTINE thread)
{
	traceLastFunc("CreateThreadA");

	DWORD threadID;
	HANDLE thread1 = CreateThread(NULL, NULL, thread, NULL, 0, &threadID);
	CloseHandle(thread1);
}
;

D3DCOLOR CSampMulti::FLOAT4TOD3DCOLOR(float Col[])
{
	traceLastFunc("FLOAT4TOD3DCOLOR()");

	D3DCOLOR col32_no_alpha = ImGui::ColorConvertFloat4ToU32(ImVec4(Col[0], Col[1], Col[2], Col[3]));
	float a = (col32_no_alpha >> 24) & 255;
	float r = (col32_no_alpha >> 16) & 255;
	float g = (col32_no_alpha >> 8) & 255;
	float b = col32_no_alpha & 255;
	return D3DCOLOR_ARGB((int)a, (int)b, (int)g, (int)r);
}

std::string CSampMulti::UTF8_to_CP1251(std::string const& utf8)
{
	if (!utf8.empty())
	{
		int wchlen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), NULL, 0);
		if (wchlen > 0 && wchlen != 0xFFFD)
		{
			std::vector<wchar_t> wbuf(wchlen);
			MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), &wbuf[0], wchlen);
			std::vector<char> buf(wchlen);
			WideCharToMultiByte(1251, 0, &wbuf[0], wchlen, &buf[0], wchlen, 0, 0);

			return std::string(&buf[0], wchlen);
		}
	}
	return std::string();
}

std::string CSampMulti::Ansii_to_UTF8(std::string const ansii)
{
	int size = MultiByteToWideChar(CP_ACP, 0/*MB_COMPOSITE*/, ansii.c_str(),
		ansii.length(), nullptr, 0);
	std::wstring utf16_str(size, '\0');
	MultiByteToWideChar(CP_ACP, 0/*MB_COMPOSITE*/, ansii.c_str(),
		ansii.length(), &utf16_str[0], size);

	int utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
		utf16_str.length(), nullptr, 0,
		nullptr, nullptr);

	std::string utf8_str(utf8_size, '\0');
	WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
		utf16_str.length(), &utf8_str[0], utf8_size,
		nullptr, nullptr);

	return utf8_str;
}

//std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
#if _MSC_VER >= 1900
std::string CSampMulti::utf16_to_utf8(std::u16string utf16_string)
{
	std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
	auto p = reinterpret_cast<const int16_t*>(utf16_string.data());
	return convert.to_bytes(p, p + utf16_string.size());
}
#else
std::string CSampMulti::utf16_to_utf8(std::u16string utf16_string)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	return convert.to_bytes(utf16_string);
}
#endif

std::vector<std::string> CSampMulti::get_filenames(std::experimental::filesystem::path path)
{
	namespace stdfs = std::experimental::filesystem;

	std::vector<std::string> filenames;

	// http://en.cppreference.com/w/cpp/experimental/fs/directory_iterator
	const stdfs::directory_iterator end{};

	for (stdfs::directory_iterator iter{ path }; iter != end; ++iter)
	{
		// http://en.cppreference.com/w/cpp/experimental/fs/is_regular_file 
		if (stdfs::is_regular_file(*iter)) // comment out if all names (names of directories tc.) are required
			filenames.push_back(iter->path().string());
	}

	return filenames;
}

void CSampMulti::CreateFolder(const char* szFolderName)
{
	traceLastFunc("FolderExist()");

	if (!std::experimental::filesystem::exists(szFolderName))
	{
		cheat_state_text("Folder %s not found, creating folder %s", szFolderName, szFolderName);
		std::experimental::filesystem::create_directory(szFolderName);
	}
}

void CSampMulti::DeleteFolder(const char* szPath)
{
	traceLastFunc("FolderExist()");

	RemoveDirectory(szPath);
}

bool CSampMulti::FolderExist(const char* szFolderName)
{
	traceLastFunc("FolderExist()");

	DWORD attribs = ::GetFileAttributesA(szFolderName);
	if (attribs == INVALID_FILE_ATTRIBUTES) {
		return false;
	}
	else return true;
}

const char* CSampMulti::GetPCUserNameF(void)
{
	traceLastFunc("GetPCUserName()");

	static char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserNameA(username, &username_len);

	return username;
}

const char* CSampMulti::GetComputerNameF(void)
{
	traceLastFunc("GetComputerNameF()");

	static char cname[256];
	DWORD nameccc = UNLEN + 1;
	GetComputerNameA(cname, &nameccc);
	return cname;
}

const char* CSampMulti::GetNumberOfProcessorsF(void)
{
	traceLastFunc("GetNumberOfProcessorsF()");

	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return this->SetText("%d", info.dwNumberOfProcessors);
}

const char* CSampMulti::RadioUpdateSongTitle(HSTREAM Stream)
{
	const char* meta = BASS_ChannelGetTags(Stream, BASS_TAG_META);
	if (meta)
	{
		const char* p = strstr(meta, "StreamTitle='");
		if (p)
		{
			const char* p2 = strstr(p, "';");
			if (p2)
			{
				// Extract substring safely with formatting
				size_t len = p2 - (p + 13);

				if (len > 0 && len < 500)
				{
					char title[512] = { 0 };
					strncpy(title, p + 13, len);
					title[len] = 0;

					return pSampMulti->SetText("%s", title);
				}
			}
		}
	}
	else
	{
		meta = BASS_ChannelGetTags(Stream, BASS_TAG_OGG);
		if (meta)
		{
			const char* artist = NULL, * title = NULL, * p = meta;

			for (; *p; p += strlen(p) + 1)
			{
				if (!_strnicmp(p, "artist=", 7)) artist = p + 7;
				if (!_strnicmp(p, "title=", 6)) title = p + 6;
			}

			if (title)
			{
				if (artist)
					return pSampMulti->SetText("%s - %s", artist, title);
				else
					return pSampMulti->SetText("%s", title);
			}
		}
		else
		{
			meta = BASS_ChannelGetTags(Stream, 0x14000);
			if (meta)
			{
				const char* p = strchr(meta, ',');
				if (p)
					return pSampMulti->SetText("%s", p + 1);
			}
		}
	}

	return "N/A";
}


bool CSampMulti::AreNumbersIdentical(const char* text_to_scan, const char* text_for_scan)
{
	int result = strspn(text_to_scan, text_for_scan);
	if (result > 0)
		return true;
	else return false;
	//if (g_SAMP && result > 0)
		//addMessageToChatWindow("Defined text has %d identical numbers", result);
}

//https://stackoverflow.com/users/87234/gmannickg
bool CSampMulti::IsFileEmpty(std::fstream& szFile)
{
	return szFile.peek() == std::ifstream::traits_type::eof();
}

void CSampMulti::IntDuplicate(int& dup_target, int duplicate_size)
{
	traceLastFunc("IntDuplicate()");

	dup_target *= duplicate_size;
}

int CSampMulti::GetFilesCount(const char* szFolder)
{
	traceLastFunc("GetFilesCount()");

	int files = 0;
	for (const auto& rfiles : std::experimental::filesystem::directory_iterator(szFolder))
		files++;

	return files;
}

int CSampMulti::GetFileItemsCount(const char* szFile)
{
	traceLastFunc("GetFileItemsCount()");

	int file_iC = 0;
	std::fstream file;
	file.open(szFile, std::fstream::in);
	if (file.is_open())
	{
		for (std::string lines; std::getline(file, lines);)
		{
			if (!lines.empty())
				file_iC++;
		}
		file.close();
	}
	return file_iC;
}

int CSampMulti::GetSongsCount()
{
	traceLastFunc("GetSongsCount()");

	int songs_count = 0;
	for (const auto& entry : std::experimental::filesystem::directory_iterator(MOD_FOLDER_MP3))
		songs_count++;

	return songs_count;
}

int CSampMulti::GetAdminsCount(void)
{
	traceLastFunc("GetAdminsCount()");

	int AdminsCount = 0, AdminsCountOnline_c = 0;

	if (BlackLightFuncs->bViewAllAdmins)
	{
		if (this->DoesFileExist("admins", ".ini", M0D_FOLDER))
		{
			std::fstream file;
			file.open("BlackLight\\admins.ini", std::fstream::in);
			if (file.is_open())
			{
				for (std::string szAdmins; std::getline(file, szAdmins);)
				{
					if (!szAdmins.empty())
						AdminsCount++;
				}
				file.close();
			}
			return AdminsCount;
		}
	}
	else if (!BlackLightFuncs->bViewAllAdmins)
	{
		for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
		{
			if (g_Players->iIsListed[i] != 1) //avoid bad players
				continue;

			if (g_Players->pRemotePlayer[i] == NULL)
				continue;

			if (BlackLightFuncs->bIsAdmin[i])
				AdminsCountOnline_c++;
		}
		if (BlackLightFuncs->bIsAdmin[g_Players->sLocalPlayerID])
			return AdminsCountOnline_c + 1;
		else return AdminsCountOnline_c;
	}
	return 0;

}

int CSampMulti::GetFriendsCount(void)
{
	traceLastFunc("GetFriendsCount()");

	int FriendsCount = 0, FriendOnline_c = 0;

	if (BlackLightFuncs->bViewAllFriends)
	{
		if (this->DoesFileExist("friends", ".ini", M0D_FOLDER))
		{
			std::fstream ffile;
			ffile.open("BlackLight\\friends.ini", std::fstream::in);
			if (ffile.is_open())
			{
				for (std::string szFriends; std::getline(ffile, szFriends);)
				{
					if (!szFriends.empty())
						FriendsCount++;
				}
				ffile.close();
			}
			return FriendsCount;
		}
	}
	else if (!BlackLightFuncs->bViewAllFriends)
	{
		for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
		{
			if (g_Players->iIsListed[i] != 1) //avoid bad players
				continue;

			if (g_Players->pRemotePlayer[i] == NULL)
				continue;

			if (BlackLightFuncs->bIsFriend[i])
				FriendOnline_c++;
		}
		if (BlackLightFuncs->bIsFriend[g_Players->sLocalPlayerID])
			return FriendOnline_c + 1;
		else return FriendOnline_c;
	}
	return 0;
}

int CSampMulti::GetRadioStationsCount(void)
{
	traceLastFunc("GetRadioStationsCount()");

	int stations = 0;

	for (const auto& rstations : std::experimental::filesystem::directory_iterator(MOD_FOLDER_RADIO))
		stations++;

	return stations;
}

int CSampMulti::GetPlayersInStream(void)
{
	if (g_SAMP == NULL)
		return NULL;

	if (g_Players == NULL)
		return NULL;

	if (gta_menu_active())
		return NULL;

	traceLastFunc("GetPlayersInStream()");

	int count = 0;
	for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
	{
		if (g_Players->iIsListed[players] != 1)
			continue;

		if (this->IsPlayerStreamed(players))
			count++;
	}

	return count;
}

int CSampMulti::GetAdminsInStream(void)
{
	if (g_SAMP == NULL)
		return NULL;

	if (g_Players == NULL)
		return NULL;

	if (gta_menu_active())
		return NULL;

	traceLastFunc("GetAdminsInStream()");

	int count = 0;
	for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
	{
		if (this->IsPlayerStreamed(players) && BlackLightFuncs->bIsAdmin[players])
			count++;
	}

	return count;
}

int CSampMulti::GetFriendsInStream(void)
{
	if (g_SAMP == NULL)
		return NULL;

	if (g_Players == NULL)
		return NULL;

	if (gta_menu_active())
		return NULL;

	traceLastFunc("GetFriendsInStream()");

	int count = 0;
	for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
	{
		if (this->IsPlayerStreamed(players) && BlackLightFuncs->bIsFriend[players])
			count++;
	}

	return count;
}

int CSampMulti::GetPlayersInVehicleCount(void)
{
	if (g_SAMP == NULL)
		return NULL;

	if (g_Players == NULL)
		return NULL;

	if (gta_menu_active())
		return NULL;

	traceLastFunc("GetPlayersInVehicleCount()");

	int count = 0;
	for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
	{
		if (this->IsPlayerStreamed(players) && this->IsPlayerInCar(players))
			count++;
	}

	return pSampMulti->IsOurPlayerInCar() ? count +1 : count;
}

int CSampMulti::GetAFKPlayersCount(void)
{
	if (g_SAMP == NULL)
		return NULL;

	if (g_Players == NULL)
		return NULL;

	if (gta_menu_active())
		return NULL;

	traceLastFunc("GetPlayersInVehicleCount()");

	int count = 0;
	for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
	{
		if (g_Players->iIsListed[players] != 1)
			continue;

		if (this->IsPlayerStreamed(players) && g_Players->pRemotePlayer[players]->pPlayerData->iAFKState == 2)
			count++;
	}

	return count;
}

int CSampMulti::GetPassengersInVehicleCount(void)
{
	if (g_SAMP == NULL)
		return NULL;

	if (g_Players == NULL)
		return NULL;

	if (gta_menu_active())
		return NULL;

	traceLastFunc("GetPassengersInVehicleCount()");

	int count = 0;
	for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
	{
		if (g_Players->iIsListed[players] != 1)
			continue;

		if (this->IsPlayerStreamed(players) && getPlayerState(players) == PLAYER_STATE_PASSENGER)
			count++;
	}

	return count;
}

int CSampMulti::GetVehiclesInStreamCount(void)
{
	traceLastFunc("GetVehiclesInStreamCount()");

	if (!pGameInterface)
		return 0;

	return (int)pGameInterface->GetPools()->GetVehicleCount();
}

int CSampMulti::GetMutedPlayersCount(void)
{
	if (!g_SAMP || !g_Players)
		return 0;

	int icount = 0;
	for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
	{
		if (g_Players->iIsListed[i] != 1)
			continue;
		if (g_Players->pRemotePlayer[i] == NULL)
			continue;

		if (BlackLightFuncs->Mute.bMutedPlayers[i] == true)
		{
			icount++;
		}
	}
	return icount;
}

int CSampMulti::GetObjectsStreamedCount(void)
{
	traceLastFunc("GetObjectsStreamedCount()");

	if (!g_SAMP)
		return 0;
	if (g_SAMP->pPools->pObject == NULL)
		return 0;

	int total_count = 0;
	for (int objects = 0; objects < SAMP_MAX_OBJECTS; objects++)
	{
		if (g_SAMP->pPools->pObject->iIsListed[objects] != 1)
			continue;
		if (g_SAMP->pPools->pObject->object[objects] == NULL)
			continue;
		if (g_SAMP->pPools->pObject->object[objects]->pGTAEntity == NULL)
			continue;

		if (this->IsObjectStreamed(objects))
			total_count++;
	}
	return total_count;
}

const char* CSampMulti::GetPlayerInfoStatus(int player_id)
{
	traceLastFunc("GetPlayerInfoStatus()");

	if (g_SAMP == NULL)
		return "Bad g_SAMP data";

	if (g_Players == NULL)
		return "Bad g_Players data";

	if (BlackLightFuncs->bIsAdmin[player_id] && !BlackLightFuncs->bIsFriend[player_id])
		return "Admin";
	if (BlackLightFuncs->bIsAdmin[player_id] && BlackLightFuncs->bIsFriend[player_id])
		return "Admin & Friend";
	if (!BlackLightFuncs->bIsAdmin[player_id] && BlackLightFuncs->bIsFriend[player_id])
		return "Friend";
	if (pSampMulti->IsModDeveloper(player_id))
		return "ModDeveloper";
	if (!BlackLightFuncs->bIsAdmin[player_id] && !BlackLightFuncs->bIsFriend[player_id] && !pSampMulti->IsModDeveloper(player_id))
		return "Regular Player";

	return "N/A";
}

void CSampMulti::TeleportToObject(int object_id)
{
	traceLastFunc("TeleportToObject()");

	if (!g_SAMP)
		return;
	if (!g_SAMP->pPools)
		return;

	if (g_SAMP->pPools->pPlayer->pLocalPlayer->iIsActorAlive == 1)
	{
		float fpos[3];
		fpos[0] = g_SAMP->pPools->pObject->object[object_id]->fPos[0];
		fpos[1] = g_SAMP->pPools->pObject->object[object_id]->fPos[1];
		fpos[2] = g_SAMP->pPools->pObject->object[object_id]->fPos[2];

		cheat_teleport(fpos, g_SAMP->pPools->pObject->object[object_id]->pGTAEntity->base.interior_id);
		addMessageToChatWindow("Teleported to Object Model: %s", g_SAMP->pPools->pObject->object[object_id]->pGTAEntity->base.interior_id);
	}
	else addMessageToChatWindow("You can't teleport if you are dead");
}

void CSampMulti::FileDelete(const char* szFile)
{
	traceLastFunc("FileDelete()");
	remove(szFile);
}

void CSampMulti::AddText_to_Text(const char* szDefineText, char szTextoAdd)
{
	traceLastFunc("AddText_to_Text()");

	std::string str = szDefineText;
	str.back() = szTextoAdd;
}

bool CSampMulti::TextInFileExists(const char* file_path, const char* szTextToCheck)
{
	std::fstream file;
	file.open(file_path, std::fstream::in);

	if (file.is_open())
	{
		std::string file_lines;
		std::getline(file, file_lines);
			if (!file_lines.empty())
			{
				if (this->IsTextIdentical(file_lines.c_str(), szTextToCheck))
					return true;
			}
		file.close();
	}
	return false;
}

bool CSampMulti::IsTextIdentical(const char* str1, const char* str2)
{
	if (strcmp(str1, str2) == 0)
		return true;

	return false;
}

const char* CSampMulti::SetText(const char* szText, ...)
{
	va_list ap;

	static char tmp[512];
	memset(tmp, 0, sizeof(tmp));

	va_start(ap, szText);
	vsnprintf(tmp, sizeof(tmp), szText, ap);
	va_end(ap);

	return tmp;
}

void CSampMulti::WriteToFile(const char* filename, const char* szText)
{
	std::fstream file;
	file.open(filename, std::fstream::out | std::fstream::app);
	if (file.is_open())
	{
		file << szText << std::endl;
		ZeroMemory(&szText, sizeof(szText));
		file.close();
	}
}

const char* CSampMulti::ReadFromFile(const char* szFile)
{
	std::string sztextl;
	std::fstream file(szFile);

	if (!file.is_open()) file.open(szFile, std::fstream::in);
	if (file.good() && file.is_open())
	{
		for (sztextl; std::getline(file, sztextl);)
		{
			if (!sztextl.empty())
				return this->SetText("%s\n", sztextl.c_str());
		}
		file.close();
	}

	return "N/A";
}

const char* CSampMulti::ReplaceText(const char* szText, const char* szTextToDelete)
{
	std::string text = szText;

	text.replace(text.find(szTextToDelete), strlen(szTextToDelete), "").c_str();
	return text.c_str();
}

const char* CSampMulti::ModifyText(const char* szText, const char* szTextToDelete, const char* szNewText)
{
	return std::string(szText).replace(std::string(szText).find(szTextToDelete), strlen(szTextToDelete), szNewText).c_str();
}

void CSampMulti::ImSpacing(int spaces)
{
	for (int spc = 0; spc < spaces; spc++)
		ImGui::Spacing();
}

void CSampMulti::ImAllingItem(float fpos)
{
	ImGui::Text("");
	ImGui::SameLine(fpos);
}

bool CSampMulti::IsKeyHolded(int VK_KEY)
{
	if (!pGameInterface)
		return false;

	if (GetAsyncKeyState(VK_KEY) < 0)
		return true;

	return false;
}

bool CSampMulti::DoesFileExist(const char* szFile, const char* szFileExt, const char* szPath)
{
	traceLastFunc("DoesFileExist()");

	std::string	srcFile;
	srcFile += szPath;
	srcFile += szFile;
	srcFile += szFileExt;

	if (std::experimental::filesystem::exists(srcFile.c_str()))
		return true;
	else
		return false;
}

bool CSampMulti::MP3FileExist(const char* szFile, bool bIncludePath)
{
	traceLastFunc("MP3FileExist()");

	std::string path;
	path = MOD_FOLDER_MP3;
	path += szFile;

	if (std::experimental::filesystem::exists(bIncludePath ? path.c_str() : szFile))
		return true;

	return false;
}

const char* CSampMulti::AddText(const char* toAdd, const char* location)
{
	traceLastFunc("AddText()");

	return std::string(location).insert(0, toAdd).c_str();
}

void CSampMulti::replaceAll(std::string& source, const std::string& from, const std::string& to)
{
	std::string newString;
	newString.reserve(source.length());  // avoids a few memory allocations

	std::string::size_type lastPos = 0;
	std::string::size_type findPos;

	while (std::string::npos != (findPos = source.find(from, lastPos)))
	{
		newString.append(source, lastPos, findPos - lastPos);
		newString += to;
		lastPos = findPos + from.length();
	}

	// Care for the rest after last occurrence
	newString += source.substr(lastPos);
	source.swap(newString);
}

void CSampMulti::AddAdmin(int PlayerID, bool bNotifyMsg)
{
	if (g_SAMP == NULL)
		return;

	if (g_Players == NULL)
		return;

	if (PlayerID != g_Players->sLocalPlayerID && g_Players->pRemotePlayer[PlayerID] == NULL)
		return addMessageToChatWindowSS("Error: something is wrong with id(%d), try another one", PlayerID);

	std::fstream admins_file;
	admins_file.open("BlackLight\\admins.ini", std::fstream::out | std::fstream::app);

	if (admins_file.is_open())
	{
		if (BlackLightFuncs->bIsAdmin[PlayerID])
			addMessageToChatWindow("%s[%d] is already in your Admin list", getPlayerName(PlayerID), PlayerID);
		else admins_file << getPlayerName(PlayerID) << std::endl;
		if (!BlackLightFuncs->bIsAdmin[PlayerID] && bNotifyMsg)
			addMessageToChatWindow("Added %s(%d) to Admin list", getPlayerName(PlayerID), PlayerID);

		admins_file.close();
	}
}

void CSampMulti::AddFriend(int PlayerID, bool bNotifyMsg)
{
	if (g_SAMP == NULL)
		return;

	if (g_Players == NULL)
		return;

	if (PlayerID != g_Players->sLocalPlayerID && g_Players->pRemotePlayer[PlayerID] == NULL)
		return addMessageToChatWindowSS("Error: something is wrong with id(%d), try another one", PlayerID);

	std::fstream friends_file;
	friends_file.open("BlackLight\\friends.ini", std::fstream::out | std::fstream::app);

	if (friends_file.is_open())
	{
		if (BlackLightFuncs->bIsFriend[PlayerID])
			addMessageToChatWindow("%s[%d] is already in your friend list", getPlayerName(PlayerID), PlayerID);
		else friends_file << getPlayerName(PlayerID) << std::endl;
		if (!BlackLightFuncs->bIsFriend[PlayerID] && bNotifyMsg)
			addMessageToChatWindow("Added %s(%d) to friend list", getPlayerName(PlayerID), PlayerID);

		friends_file.close();
	}
}

void CSampMulti::ClearFile(const char* filename, const char* szMsg)
{
	traceLastFunc("ClearFile()");

	std::fstream file; file.open(filename, std::fstream::out);
	if (file.is_open()) { file.clear(); file.close(); }

	if (g_SAMP && g_Chat && strlen(szMsg))
		addMessageToChatWindow(szMsg);
}

void CSampMulti::sampNop(int id, int type, bool enabled)
{
	traceLastFunc("sampNop()");

	for (int i = 0; i < 1024; i++)
	{
		if (set.netPatch[i].type == type && set.netPatch[i].id == id)
		{
			set.netPatch[i].enabled = enabled;
			break;
		}
	}
	return;
}

void CSampMulti::DeleteAdmin(int PlayerID)
{
	traceLastFunc("RemoveAdmin()");

	if (!g_SAMP || !g_Players)
		return;

	if (PlayerID != g_Players->sLocalPlayerID && g_Players->pRemotePlayer[PlayerID] == NULL)
		return addMessageToChatWindowSS("Error: player not found", PlayerID);

	if (!BlackLightFuncs->bIsAdmin[PlayerID])
		return addMessageToChatWindow("Player %s(%d) isn't in your admin list.", getPlayerName(PlayerID), PlayerID);

	std::fstream inputFile;
	std::ofstream outputFile;
	char filename[64];
	std::string strName(getPlayerName(PlayerID));
	std::string strUpdatedLine;
	std::string strCurrentLine;

	snprintf(filename, sizeof(filename), "%s\\" M0D_NAME "%s", g_szWorkingDirectory, "admins.ini");
	inputFile.open(filename, std::ios::in);
	snprintf(filename, sizeof(filename), "%s\\" M0D_NAME "%s", g_szWorkingDirectory, "admins_temp.ini");
	outputFile.open(filename);
	strUpdatedLine += "";
	while (std::getline(inputFile, strCurrentLine))
	{
		if (strCurrentLine.find(strName) != std::string::npos)
		{
			strCurrentLine = strUpdatedLine;
		}
		outputFile << strCurrentLine << std::endl;
	}
	inputFile.close();
	outputFile.close();

	snprintf(filename, sizeof(filename), "%s" M0D_NAME "%s", g_szWorkingDirectory, "admins_temp.ini");
	inputFile.open(filename, std::ios::in);
	snprintf(filename, sizeof(filename), "%s" M0D_NAME "%s", g_szWorkingDirectory, "admins.ini");
	outputFile.open(filename);

	while (std::getline(inputFile, strCurrentLine))
		outputFile << strCurrentLine << std::endl;

	inputFile.close();
	outputFile.close();

	remove("BlackLight\\admins_temp.ini");
	addMessageToChatWindow("%s(%d) removed from your admin list", getPlayerName(PlayerID), PlayerID);
}

void CSampMulti::DeleteFileText(const char* szPath, const char* szFile, const char* szFileExt, const char* szText, bool bMsg, bool bNoExtension)
{
	traceLastFunc("DeleteFileText()");

	std::fstream inputFile;
	std::ofstream outputFile;

	std::string strText(szText);
	std::string strFile, strFileCopy;
	std::string strUpdatedLine;
	std::string strCurrentLine;

	strFile += szPath;
	strFile += szFile;
	strFile += szFileExt;
	inputFile.open(strFile.c_str(), std::ios::in);

	strFileCopy += M0D_FOLDER;
	if (bNoExtension)
	{
		strFileCopy += "temp_";
		strFileCopy += szFile;
	}
	else
	{
		strFileCopy += "_temp";
		strFileCopy += szFileExt;
	}
	outputFile.open(strFileCopy.c_str());

	strUpdatedLine += "";
	while (std::getline(inputFile, strCurrentLine))
	{
		if (strCurrentLine.find(strText) != std::string::npos)
			strCurrentLine = strUpdatedLine;

		outputFile << strCurrentLine << std::endl;
	}
	inputFile.close();
	outputFile.close();

	inputFile.open(strFileCopy.c_str(), std::ios::in);
	outputFile.open(strFile.c_str());
	while (std::getline(inputFile, strCurrentLine))
		outputFile << strCurrentLine << std::endl;

	inputFile.close();
	outputFile.close();

	remove(strFileCopy.c_str());

	if (bMsg == 1)
		addMessageToChatWindow("Deleted %s from file %s%s located in %s\\", szText, szFile, szFileExt, szPath);

	ZeroMemory(&szText, sizeof(szText)); 	ZeroMemory(&szFile, sizeof(szFile)); 	ZeroMemory(&szFileExt, sizeof(szFileExt)); 	ZeroMemory(&szPath, sizeof(szPath));
}

void CSampMulti::DeleteFriend(int PlayerID)
{
	traceLastFunc("DeleteFriend()");

	if (!g_SAMP || !g_Players)
		return;

	if (PlayerID != g_Players->sLocalPlayerID && g_Players->pRemotePlayer[PlayerID] == NULL)
		return addMessageToChatWindowSS("Error: something is wrong with id(%d), try another one", PlayerID);

	if (!BlackLightFuncs->bIsFriend[PlayerID])
		return addMessageToChatWindow("Player %s(%d) isn't in your friend list.", getPlayerName(PlayerID), PlayerID);

	std::fstream inputFile;
	std::ofstream outputFile;
	char filename[64];
	std::string strName(getPlayerName(PlayerID));
	std::string strUpdatedLine;
	std::string strCurrentLine;

	snprintf(filename, sizeof(filename), "%s\\" M0D_FOLDER "%s", g_szWorkingDirectory, "friends.ini");
	inputFile.open(filename, std::ios::in);
	snprintf(filename, sizeof(filename), "%s\\" M0D_FOLDER "%s", g_szWorkingDirectory, "friends_temp.ini");
	outputFile.open(filename);
	strUpdatedLine += "";
	while (std::getline(inputFile, strCurrentLine))
	{
		if (strCurrentLine.find(strName) != std::string::npos)
			strCurrentLine = strUpdatedLine;
		outputFile << strCurrentLine << std::endl;
	}
	inputFile.close();
	outputFile.close();

	snprintf(filename, sizeof(filename), "%s\\" M0D_FOLDER "%s", g_szWorkingDirectory, "friends_temp.ini");
	inputFile.open(filename, std::ios::in);
	snprintf(filename, sizeof(filename), "%s\\" M0D_FOLDER "%s", g_szWorkingDirectory, "friends.ini");
	outputFile.open(filename);

	while (std::getline(inputFile, strCurrentLine))
	{
		outputFile << strCurrentLine << std::endl;
	}
	inputFile.close();
	outputFile.close();

	this->FileDelete("BlackLight\\friends_temp.ini");
	addMessageToChatWindow("%s(%d) removed from your friend list", getPlayerName(PlayerID), PlayerID);
}

static float fCol;
static bool start = false;
static DWORD tick = 0;
ImVec4 CSampMulti::ColorOpacityPulse(ImVec4 ImColorID, float OpacityPower, float OpacityMax, float OpacityMin, int TimePulse)
{
	if (GetTickCount() - tick > TimePulse)
	{
		if (!start && fCol <= OpacityMin)
			start = true;
		else if (start && fCol >= OpacityMax)
			start = false;

		if (start)
			fCol += OpacityPower;
		else
			fCol -= OpacityPower;
		tick = GetTickCount();

	}
	return ImVec4(ImColorID.x, ImColorID.y, ImColorID.z, fCol);
}

ImColor CSampMulti::TotalRainbow(int speed)
{
	ImColor color; float misc; static float rainbow;
	rainbow += misc = 0.0001f * (int)speed;
	if (rainbow > 1.f)  rainbow = 0.0f;
	color = Color2::ToImColor(Color2::FromHSB(rainbow, 0.5f, 1.0f));
	return color;
}

D3DCOLOR CSampMulti::CustomImVec4_D3DCOLOR(ImVec4 color) //use in ImGui::ColorEdit's
{
	return D3DCOLOR_RGBA((int)(color.x * 255), (int)(color.y * 255), (int)(color.z * 255), (int)(color.w * 255));
}


int CSampMulti::GetDirFileCount(const char* szDir)
{
	traceLastFunc("GetDirFileCount()");

	int count = 0;
	for (const auto& entry : std::experimental::filesystem::directory_iterator(szDir))
		count++;

	return count;
}

//By ThreadShakur
char* CSampMulti::BinToHex(BYTE* bin, size_t size, char* out)
{
	if (bin == nullptr || size == 0 || out == nullptr)
		return "";
	out[0] = '\0';

	char	b[4];
	for (size_t i = 0; i < size; i++)
	{
		_snprintf_s(b, sizeof(b) - 1, "%02X", bin[i]);
		strcat(out, b);
	}
	return out;
}

const char* CSampMulti::GetSystemUpTime(void)
{
	DWORD ticks = GetTickCount();
	//DWORD milliseconds = ticks % 1000;
	ticks /= 1000;
	static DWORD seconds = ticks % 60;
	ticks /= 60;
	static DWORD minutes = ticks % 60;
	ticks /= 60;
	static DWORD hours = ticks; // may exceed 24 hours.

	return this->SetText("%02d:%02d:%02d", hours, minutes, seconds - 2);
}

uint8_t CSampMulti::getPlayerWeaponModelID(int iPlayerID)
{
	if (g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;
	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		if (g_Players->pLocalPlayer->pSAMP_Actor == NULL)
			return NULL;

		return this->IsPlayerDriver(iPlayerID) ? g_Players->pLocalPlayer->inCarData.byteCurrentWeapon : this->IsPlayerPassenger(g_Players->sLocalPlayerID) ? g_Players->pLocalPlayer->passengerData.byteCurrentWeapon : g_Players->pLocalPlayer->onFootData.byteCurrentWeapon;
	}
	if (g_Players->pRemotePlayer[iPlayerID] == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL)
		return NULL;

	return this->IsPlayerInCar(iPlayerID) ? g_Players->pRemotePlayer[iPlayerID]->pPlayerData->inCarData.byteCurrentWeapon : this->IsPlayerPassenger(g_Players->sLocalPlayerID) ? g_Players->pLocalPlayer->passengerData.byteCurrentWeapon : g_Players->pRemotePlayer[iPlayerID]->pPlayerData->onFootData.byteCurrentWeapon;
}

float CSampMulti::getPlayerSprintEnergy(int iPlayerID)
{
	if (g_Players == nullptr || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;

	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		if (g_Players->pLocalPlayer == NULL)
			return NULL;
		if (g_Players->pLocalPlayer->pSAMP_Actor == NULL)
			return NULL;

		return (float)g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->pPedIntelligence->pPed->pPlayerData->m_fSprintEnergy / 31.47;
	}

	if (g_Players->pRemotePlayer[iPlayerID] == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL)
		return NULL;
	if (!g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor)
		return NULL;

	return (float)g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor->pGTA_Ped->pPedIntelligence->pPed->pPlayerData->m_fSprintEnergy / 31.47;
}

const char* CSampMulti::getPlayerPosition(int iPlayerID)
{
	traceLastFunc("getPlayerPosition()");

	if (!g_SAMP || !g_Players)
		return nullptr;

	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		if (!g_Players->pLocalPlayer)
			return nullptr;
		if (!g_Players->pLocalPlayer->pSAMP_Actor)
			return nullptr;

		return pSampMulti->SetText("X: %0.2f | Y: %0.2f | Z: %0.2f", g_Players->pLocalPlayer->onFootData.fPosition[0], g_Players->pLocalPlayer->onFootData.fPosition[1], g_Players->pLocalPlayer->onFootData.fPosition[2]);
	}

	if (!this->IsPlayerStreamed(iPlayerID)) return nullptr;
	else {
		return pSampMulti->SetText("X: %0.2f | Y: %0.2f | Z: %0.2f",
			g_Players->pRemotePlayer[iPlayerID]->pPlayerData->onFootData.fPosition[0],
			g_Players->pLocalPlayer->onFootData.fPosition[1],
			g_Players->pLocalPlayer->onFootData.fPosition[2]); }
	
	return nullptr;
}

void CSampMulti::ShowPlayerNameTag(int iPlayerID, bool bShow)
{
	traceLastFunc("ShowPlayerNameTag()");

	if (!this->IsPlayerStreamed(iPlayerID))
		return /*pSampMulti->LogImInfoMenu("Target player not streamed.")*/;

	if (iPlayerID == g_Players->sLocalPlayerID)
		return /*pSampMulti->LogImInfoMenu("That's your id, try another one.")*/;

	if (bShow)
	{
		g_Players->pRemotePlayer[iPlayerID]->pPlayerData->iShowNameTag = 1;
		//pSampMulti->LogImInfoMenu("Disabled name tag for %s[%d]", getPlayerName(iPlayerID), iPlayerID);
	}
	else
	{
		g_Players->pRemotePlayer[iPlayerID]->pPlayerData->iShowNameTag = 0;
		//pSampMulti->LogImInfoMenu("Showing name tag for %s[%d]", getPlayerName(iPlayerID), iPlayerID);
	}
}

void CSampMulti::SayPlayerInfo(int iPlayerID)
{
	if (!g_Players || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return;

	traceLastFunc("SayPlayerInfo()");
	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		say(pSampMulti->IsCurrentServer() ? "## My Info >>ID: %d Skin: %d, Health: %u, Armor: %u, Weapon: %s(Model: %d), Vehicle: %s" : "%s(%d) Info >> Skin: %d, Health: %u, Armor: %u, Weapon: %s(Model: %d), Vehicle: %s",
			g_Players->sLocalPlayerID,
			pSampMulti->getPlayerSkin(g_Players->sLocalPlayerID),
			pSampMulti->IsPlayerInCar(g_Players->sLocalPlayerID) ? pSampMulti->getPlayerHealthInVehicle(g_Players->sLocalPlayerID) : pSampMulti->getPlayerHealth(g_Players->sLocalPlayerID),
			pSampMulti->IsPlayerInCar(g_Players->sLocalPlayerID) ? pSampMulti->getPlayerArmorInVehicle(g_Players->sLocalPlayerID) : pSampMulti->getPlayerArmor(g_Players->sLocalPlayerID),
			pSampMulti->getPlayerWeapon(g_Players->sLocalPlayerID),
			pSampMulti->getPlayerWeaponModelID(g_Players->sLocalPlayerID),
			pSampMulti->IsOurPlayerInCar() ? pSampMulti->getPlayerVehicleName(g_Players->sLocalPlayerID) : "Not in vehicle.");
	}
	else
	{
		if (!pSampMulti->IsPlayerStreamed(iPlayerID))
			return addMessageToChatWindow("Player %s(%d) isn't streamed", getPlayerName(iPlayerID), iPlayerID);

		say(pSampMulti->IsCurrentServer() ? "## %s(%d) Info >> Skin: %d, Health: %u, Armor: %u, Weapon: %s(Model: %d), Vehicle: %s" : "%s(%d) Info >> Skin: %d, Health: %u, Armor: %u, Weapon: %s(Model: %d), Vehicle: %s",
			getPlayerName(iPlayerID),
			iPlayerID,
			pSampMulti->getPlayerSkin(iPlayerID),
			pSampMulti->IsPlayerInCar(iPlayerID) ? pSampMulti->getPlayerHealthInVehicle(iPlayerID) : pSampMulti->getPlayerHealth(iPlayerID),
			pSampMulti->IsPlayerInCar(iPlayerID) ? pSampMulti->getPlayerArmorInVehicle(iPlayerID) : pSampMulti->getPlayerArmor(iPlayerID),
			pSampMulti->getPlayerWeapon(iPlayerID),
			pSampMulti->getPlayerWeaponModelID(iPlayerID),
			pSampMulti->IsPlayerInCar(iPlayerID) ? pSampMulti->getPlayerVehicleName(iPlayerID) : "Not in vehicle.");
	}
}

const char* CSampMulti::getServerIp(void)
{
	return pSampMulti->SetText("%s:%d", g_SAMP->szIP, g_SAMP->ulPort);
}

float CSampMulti::getGameFPS(void)
{
	traceLastFunc("getGameFPS(void)");
	if (!pGameInterface)
		return 0.0f;

	static DWORD LastTick = time_get();
	if ((time_get() - LastTick) >= MSEC_TO_TIME(1000))
	{
		return pGameInterface->GetFPS();
		LastTick = time_get();
	}
	return 0.0f;
}

CVector CSampMulti::CVecPlayerPos(int PlayerID)
{
	traceLastFunc("CVecPlayerPos()");

	if (!g_Players || PlayerID < 0 || PlayerID > SAMP_MAX_PLAYERS)
		return CVector();

	CVector pPos;

	if (PlayerID == g_Players->sLocalPlayerID)
	{
		float* fposme = &g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->base.matrix[4 * 3];
		pPos.fX = fposme[0];
		pPos.fY = fposme[1];
		pPos.fZ = fposme[2];

		return pPos;
	}

	float* fpos = &g_Players->pRemotePlayer[PlayerID]->pPlayerData->pSAMP_Actor->pGTA_Ped->base.matrix[4 * 3];
	pPos.fX = fpos[0];
	pPos.fY = fpos[1];
	pPos.fZ = fpos[2];

	return pPos;
}

CVector CSampMulti::CVecGetBonePos(int PlayerID, eBone bone)
{
	traceLastFunc("CVecGetBonePos()");

	if (!g_Players || PlayerID < 0 || PlayerID > SAMP_MAX_PLAYERS)
		return CVector();

	CVector pBonePos;

	if (PlayerID == g_Players->sLocalPlayerID)
	{
		pPedSelf->GetBonePosition(bone, &pBonePos);
		return pBonePos;
	}

	CPed* ped = pGameInterface->GetPools()->GetPed((DWORD*)getGTAPedFromSAMPPlayerID(PlayerID));

	if (isBadPtr_GTA_pPed(ped))
		return CVector();

	ped->GetBonePosition(bone, &pBonePos);
	return pBonePos;
}

bool CSampMulti::IsPickupStreamed(int PickupID)
{
	traceLastFunc("IsPickupStreamed()");

	if (g_SAMP->pPools->pPickup == NULL || PickupID < 0 || PickupID > SAMP_MAX_PICKUPS)
		return false;
	if (g_SAMP->pPools->pPickup->pickup[PickupID].iModelID == NULL)
		return false;
	if (g_SAMP->pPools->pPickup->pickup[PickupID].iType == NULL)
		return false;

	return true;
}

int CSampMulti::getPickupModelD(int PickupID)
{
	traceLastFunc("getPickupModelD()");

	if (g_SAMP->pPools->pPickup == NULL || PickupID < 0 || PickupID > SAMP_MAX_PICKUPS)
		return NULL;

	return g_SAMP->pPools->pPickup->pickup[PickupID].iModelID;
}

CVector CSampMulti::CVecGetPickupPos(int PickupID)
{
	traceLastFunc("CVecGetPickupPos()");

	if (g_SAMP->pPools->pPickup == NULL || PickupID < 0 || PickupID > SAMP_MAX_PICKUPS)
		return CVector();

	CVector pickup_pos;
	float* fppos = g_SAMP->pPools->pPickup->pickup[PickupID].fPosition;
	pickup_pos.fX = fppos[0];
	pickup_pos.fY = fppos[1];
	pickup_pos.fZ = fppos[2];

	return pickup_pos;
}

void CSampMulti::GameCameraSet(float x, float y)
{
	traceLastFunc("GameCameraSet()");

	if (!pGameInterface)
		return;

	CVector mypos;
	CVector enpos;
	CVector vector;
	enpos.fX = x;
	enpos.fY = y;

	CCamera* pCamera = pGameInterface->GetCamera();
	mypos = *pCamera->GetCam(pCamera->GetActiveCam())->GetSource();
	vector = mypos - enpos;
	float AngleX = atan2f(vector.fY, -vector.fX) - M_PI / 2;

	*(float*)0xB6F258 = -(AngleX - M_PI / 2);
}

const char* CSampMulti::getWeatherName(int value_assign)
{
	const struct
	{
		int		id;
		char* name;
	}

	weather_map[] =
	{
		0,
		"Blue sky",
		1,
		"Blue Sky Lighten",
		2,
		"Gray Sky",
		3,
		"Deep Lighten Sky",
		4,
		"Cold style air",
		5,
		"Hard blue sky",
		6,
		"Hard blue sky v2",
		7,
		"Cold style air v2",
		8,
		"Stormy",
		9,
		"Cloudy and foggy",
		10,
		"Clear blue sky",
		11,
		"Scorching hot (heat waves)",
		12,
		"Very dull, colorless, hazy",
		16,
		"Dull, cloudy, rainy",
		17,
		"Scorching hot",
		19,
		"Sandstorm",
		20,
		"Greenish fog",
		21,
		"Very dark, gradiented skyline, purple",
		22,
		"Very dark, gradiented skyline, green",
		23,
		"Variations of pale orange",
		27,
		"Variations of fresh blue",
		30,
		"Variations of dark, cloudy, teal",
		33,
		"Dark, cloudy, brown",
		34,
		"Blue, purple, regular",
		35,
		"Dull brown",
		36,
		"Extremely bright",
		40,
		"Blue, purple, cloudy",
		43,
		"Dark toxic clouds",
		44,
		"Black, white sky",
		45,
		"Black, purple sky",
		-1,
		NULL
	};

	for (int i = 0;; i++)
	{
		if (weather_map[i].id == -1)
			return nullptr;

		if(i == value_assign)
		return weather_map[i].name;
	}

	return nullptr;
}


float CSampMulti::getVehicleSpeed(int iPlayerID, float fMultipler)
{
	if (!g_Players || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return 0.0;

	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		if (this->IsOurPlayerInCar())
			return (float)(vect3_length(g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->vehicle->speed) * 274.0f);
	}
	else 	if (iPlayerID != g_Players->sLocalPlayerID)
	{
		if (this->IsPlayerInCar(iPlayerID))
			return (float)(vect3_length(g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Vehicle->pGTA_Vehicle->speed) * 274.0f);
	}

	return 0.0;
}

const char* CSampMulti::getVehicleNameBySAMPID(int CarID)
{
	if (!g_Vehicles || CarID < 0 || CarID > SAMP_MAX_VEHICLES)
		return nullptr;
	if (g_Vehicles->iIsListed[CarID] != 1)
		return nullptr;
	if (g_Vehicles->pSAMP_Vehicle[CarID] == NULL)
		return nullptr;
	if (g_Vehicles->pSAMP_Vehicle[CarID]->pGTA_Vehicle == NULL)
		return nullptr;


	const struct vehicle_entry* get_vehicles;
	if (CarID == g_Players->pLocalPlayer->sCurrentVehicleID)
	{
		get_vehicles = gta_vehicle_get_by_id(g_Vehicles->pSAMP_Vehicle[g_Players->pLocalPlayer->sCurrentVehicleID]->pGTA_Vehicle->base.model_alt_id);
		return get_vehicles->name;
	}
	else
	{
		get_vehicles = gta_vehicle_get_by_id(g_Vehicles->pSAMP_Vehicle[CarID]->pGTA_Vehicle->base.model_alt_id);
		return get_vehicles->name;
	}

	return nullptr;
}

int CSampMulti::getVehicleModelBySAMPID(int CarID)
{
	if (!g_Vehicles || CarID < 0 || CarID > SAMP_MAX_VEHICLES)
		return 0;
	if (g_Vehicles->iIsListed[CarID] != 1)
		return 0;
	if (g_Vehicles->pSAMP_Vehicle[CarID] == NULL)
		return 0;
	if (g_Vehicles->pSAMP_Vehicle[CarID]->pGTA_Vehicle == NULL)
		return 0;

	if (CarID == g_Players->pLocalPlayer->sCurrentVehicleID)
	{
		return g_Vehicles->pSAMP_Vehicle[g_Players->pLocalPlayer->sCurrentVehicleID]->pGTA_Vehicle->base.model_alt_id;
	}
	else
	{
		return g_Vehicles->pSAMP_Vehicle[CarID]->pGTA_Vehicle->base.model_alt_id;
	}

	return 0;
}

int CSampMulti::getDistanceCrossed(void)
{
	if (!g_SAMP)
		return 0;
	if (!g_Players->pLocalPlayer)
		return 0;

	static int m = 0;
	for (int pos = 0; pos < 3; pos++)
	{
		if (g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->step_pos[pos] > g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->step_pos_before[pos])
		{
			m++;
		}
	}
	return m;
}

const char* CSampMulti::getPlayerWeapon(int iPlayerID)
{
	if (!g_Players || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return nullptr;

	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		for (int wep = 0; weapon_list[wep].name != nullptr; wep++)
		{
			const struct weapon_entry* weapon = &weapon_list[wep];

			if (weapon->id == getPlayerWeaponModelID(iPlayerID))
				return weapon->name;
		}
	}
	else
	{
		if (g_Players->pRemotePlayer[iPlayerID] == nullptr)
			return nullptr;

		if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == nullptr)
			return nullptr;

		for (int wepp = 0; weapon_list[wepp].name != nullptr; wepp++)
		{
			const struct weapon_entry* weaponp = &weapon_list[wepp];

			if (weaponp->id == getPlayerWeaponModelID(iPlayerID))
				return weaponp->name;
		}
	}
	return nullptr;
}

bool CSampMulti::GetPlayerQuaternion(uint16_t playerID, float* fQuaternion)
{

	switch (g_Players->pRemotePlayer[playerID]->pPlayerData->bytePlayerState)
	{
	case PLAYER_STATE_DRIVER:
		vect4_copy(g_Players->pRemotePlayer[playerID]->pPlayerData->inCarData.fQuaternion, fQuaternion);
		return true;
		break;
	default:
		vect4_copy(g_Players->pRemotePlayer[playerID]->pPlayerData->onFootData.fQuaternion, fQuaternion);
		return true;
		break;
	}
}

std::string CSampMulti::GetCurrentTimeA(void)
{
	SYSTEMTIME timeA;
	GetLocalTime(&timeA);
	return std::string(this->SetText("%02d:%02d:%02d", timeA.wHour, timeA.wMinute, timeA.wSecond));
}

std::string CSampMulti::GetCurrentDateA(void)
{
	SYSTEMTIME dateA;
	GetLocalTime(&dateA);
	return std::string(this->SetText("%02d.%02d.%02d", dateA.wDay, dateA.wMonth, dateA.wYear));
}

const char* CSampMulti::getTimeCounter(void)
{
	// hours, minutes, seconds of timer 
	static int hours = 0;
	static int minutes = 0;
	static int seconds = 0;
	static int ms = 0;

	static DWORD tickg = 0;
	DWORD tock = GetTickCount();

	if (tock - tickg >= 1000)
	{
		tickg = tock;
		ms += 1000;
	}

	if (ms == 1000)
	{
		ms = 0;
		seconds += 1;
	}
	// if seconds reaches 60 
	if (seconds == 60) {

		// increment minutes 
		minutes++;

		// if minutes reaches 60 
		if (minutes == 60) {

			// increment hours 
			hours++;
			minutes = 0;
		}
		seconds = 0;
	}

	std::string h, m, s;
	if (hours >= 10)
	{
		h.replace(0, strlen(h.c_str()), this->SetText("%s:", std::to_string(hours).c_str()));
	}
	else if (hours >= 0)
	{
		h.append(this->SetText("0%s:", std::to_string(hours).c_str()));
	}

	if (minutes >= 10)
	{
		m.replace(0, strlen(m.c_str()), this->SetText("%s:", std::to_string(minutes).c_str()));
	}
	else if (minutes >= 0)
	{
		m.append(this->SetText("0%s:", std::to_string(minutes).c_str()));
	}

	if (seconds >= 10)
	{
		s.replace(0, strlen(s.c_str()), std::to_string(seconds).c_str());
	}
	else if (seconds >= 0)
	{
		s.append(this->SetText("0%s", std::to_string(seconds).c_str()));
	}

	return this->SetText("%s%s%s", h.c_str(), m.c_str(), s.c_str());
}

int CSampMulti::getPlayerInteriorID(int iPlayerID)
{
	if (!g_Players || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;
	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		if (g_Players->pLocalPlayer->pSAMP_Actor == nullptr)
			return NULL;

		return g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->base.interior_id;
	}
	if (g_Players->pRemotePlayer == nullptr)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == nullptr)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == nullptr)
		return NULL;

	return g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor->pGTA_Ped->base.interior_id;
}

int CSampMulti::getPlayerSkin(int iPlayerID)
{
	if (!g_Players || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;
	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		if (g_Players->pLocalPlayer->pSAMP_Actor == nullptr)
			return NULL;

		return g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->base.model_alt_id;
	}
	if (g_Players->pRemotePlayer == nullptr)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == nullptr)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == nullptr)
		return NULL;

	return g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor->pGTA_Ped->base.model_alt_id;
}

uint8_t CSampMulti::getPlayerHealth(int iPlayerID)
{
	if (g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;
	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		if (g_Players->pLocalPlayer->pSAMP_Actor == NULL)
			return NULL;

		return pSampMulti->IsOurPlayerInCar() ? g_Players->pLocalPlayer->inCarData.bytePlayerHealth : g_Players->pLocalPlayer->onFootData.byteHealth;
	}
	if (g_Players->pRemotePlayer == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == NULL)
		return NULL;

	return pSampMulti->IsPlayerInCar(iPlayerID) ? g_Players->pRemotePlayer[iPlayerID]->pPlayerData->inCarData.bytePlayerHealth : g_Players->pRemotePlayer[iPlayerID]->pPlayerData->onFootData.byteHealth;
}

uint8_t CSampMulti::getPlayerHealthInVehicle(int iPlayerID)
{
	if (g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;
	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		if (g_Players->pLocalPlayer->pSAMP_Actor == NULL)
			return NULL;

		return this->IsPlayerPassenger(g_Players->sLocalPlayerID) ? g_Players->pLocalPlayer->passengerData.byteHealth : g_Players->pLocalPlayer->inCarData.bytePlayerHealth;
	}
	if (g_Players->pRemotePlayer == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == NULL)
		return NULL;

	return this->IsPlayerPassenger(iPlayerID) ? g_Players->pRemotePlayer[iPlayerID]->pPlayerData->passengerData.byteHealth : g_Players->pRemotePlayer[iPlayerID]->pPlayerData->inCarData.bytePlayerHealth;
}

float CSampMulti::getPlayerVehicleHealth(int iPlayerID)
{
	traceLastFunc("getPlayerVehicleHealth()");

	if (g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;
	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		if (g_Players->pLocalPlayer->pSAMP_Actor == NULL)
			return NULL;

		static float myvehp = 0;
		myvehp = (g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->vehicle->hitpoints / 10.0f);

		return myvehp;
	}
	if (g_Players->pRemotePlayer == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == NULL)
		return NULL;

	static float hp = 0;
	hp = (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor->pGTA_Ped->vehicle->hitpoints / 10.0f);

	return hp;
}

uint8_t CSampMulti::getPlayerArmorInVehicle(int iPlayerID)
{
	traceLastFunc("getPlayerArmorInVehicle()");
	if (g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;
	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		if (g_Players->pLocalPlayer->pSAMP_Actor == NULL)
			return NULL;

		return this->IsPlayerPassenger(g_Players->sLocalPlayerID) ? g_Players->pLocalPlayer->passengerData.byteArmor : g_Players->pLocalPlayer->inCarData.byteArmor;
	}
	if (g_Players->pRemotePlayer == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == NULL)
		return NULL;

	return this->IsPlayerPassenger(iPlayerID) ? g_Players->pRemotePlayer[iPlayerID]->pPlayerData->passengerData.byteArmor : g_Players->pRemotePlayer[iPlayerID]->pPlayerData->inCarData.byteArmor;
}

int CSampMulti::getPlayerScore(int iPlayerID)
{
	if (g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;
	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		if (g_Players->pLocalPlayer->pSAMP_Actor == NULL)
			return NULL;

		return g_Players->iLocalPlayerScore;
	}
	if (g_Players->pRemotePlayer == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == NULL)
		return NULL;

	return g_Players->pRemotePlayer[iPlayerID]->iScore;
}

int CSampMulti::getPlayersOnServerCount(void)
{
	if (g_Players == NULL)
		return NULL;

	int iCount = 0;

	for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
	{
		if (g_Players->iIsListed[i] != 1)
			continue;
		iCount++;
	}

	return iCount + 1;
}

int CSampMulti::getPlayerPing(int iPlayerID)
{
	if (g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;
	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		if (g_Players->pLocalPlayer->pSAMP_Actor == NULL)
			return NULL;

		return g_Players->iLocalPlayerPing;
	}
	else
	{
		if (g_Players->pRemotePlayer == NULL)
			return NULL;
		if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL)
			return NULL;
		if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == NULL)
			return NULL;

		return g_Players->pRemotePlayer[iPlayerID]->iPing;
	}
	return NULL;
}

uint8_t CSampMulti::getPlayerArmor(int iPlayerID)
{
	if (g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;
	if (iPlayerID == g_Players->sLocalPlayerID)
	{
		if (g_Players->pLocalPlayer->pSAMP_Actor == NULL)
			return NULL;

		return pSampMulti->IsOurPlayerInCar() ? g_Players->pLocalPlayer->inCarData.byteArmor : g_Players->pLocalPlayer->onFootData.byteArmor;
	}
	if (g_Players->pRemotePlayer == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == NULL)
		return NULL;

	return pSampMulti->IsPlayerInCar(iPlayerID) ? g_Players->pRemotePlayer[iPlayerID]->pPlayerData->inCarData.byteArmor : g_Players->pRemotePlayer[iPlayerID]->pPlayerData->onFootData.byteArmor;
}


bool CSampMulti::IsPlayerInCar(int PlayerID)
{
	if (g_Players == NULL)
		return NULL;
	if (PlayerID == g_Players->sLocalPlayerID)
		return NULL;
	if (g_Players->pRemotePlayer == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[PlayerID]->pPlayerData == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[PlayerID]->pPlayerData->pSAMP_Actor == NULL)
		return NULL;

	return g_Players->pRemotePlayer[PlayerID]->pPlayerData->pSAMP_Actor->pGTA_Ped->pedFlags.bInVehicle;
};

int CSampMulti::GetNextPlayerAsTarget(int iplayerID, int increase)
{
	if (increase == -1)
	{
		for (int playerID = iplayerID - 1; playerID >= 0; playerID--)
		{
			if (g_Players->iIsListed[playerID] != 1)
				continue;
			if (g_Players->pRemotePlayer[playerID] == NULL)
				continue;
			if (g_Players->pRemotePlayer[playerID]->pPlayerData == NULL)
				continue;
			if (g_Players->pRemotePlayer[playerID]->pPlayerData->pSAMP_Actor == NULL)
				continue;	// not streamed
			return playerID;
		}
	}
	else
	{
		for (int playerID = iplayerID + 1; playerID < SAMP_MAX_PLAYERS; playerID++)
		{
			if (g_Players->iIsListed[playerID] != 1)
				continue;
			if (g_Players->pRemotePlayer[playerID] == NULL)
				continue;
			if (g_Players->pRemotePlayer[playerID]->pPlayerData == NULL)
				continue;
			if (g_Players->pRemotePlayer[playerID]->pPlayerData->pSAMP_Actor == NULL)
				continue;	// not streamed
			return playerID;
		}
	}

	return iplayerID;
}

int CSampMulti::GetNearestPlayer(void)
{
	if (!g_SAMP)
		return NULL;

	actor_info* self = actor_info_get(ACTOR_SELF, 0);

	if (!self) return NULL;

	float fNearestDistance = -1.0f;
	int iplayerID = -1;

	for (int playerID = 0; playerID < SAMP_MAX_PLAYERS; playerID++)
	{
		if (g_Players->iIsListed[playerID] != 1)
			continue;
		if (g_Players->pRemotePlayer[playerID] == NULL)
			continue;
		if (g_Players->pRemotePlayer[playerID]->pPlayerData == NULL)
			continue;
		if (g_Players->pRemotePlayer[playerID]->pPlayerData->pSAMP_Actor == NULL)
			continue;	// not streamed
		float fDistance = vect3_dist(&self->base.matrix[12], &g_Players->pRemotePlayer[playerID]->pPlayerData->pSAMP_Actor->pGTA_Ped->base.matrix[12]);
		if (fNearestDistance == -1.0f || fDistance < fNearestDistance)
		{
			iplayerID = playerID;
			fNearestDistance = fDistance;
		}
	}

	return iplayerID;
}

uint16_t CSampMulti::getPlayerVehicleModelID_V2(int iPlayerID)
{
	if (g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_MAX_PLAYERS)
		return NULL;
	if (iPlayerID == g_Players->sLocalPlayerID && this->IsOurPlayerInCar())
	{
		if (g_Players->pLocalPlayer->pSAMP_Actor == NULL
			|| g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->vehicle == NULL)
			return NULL;

		if (this->IsOurPlayerInCar())
			return g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->vehicle->base.model_alt_id;
	}
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Vehicle == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Vehicle->pGTA_Vehicle == NULL)
		return NULL;

	if (this->IsPlayerInCar(iPlayerID))
		return g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Vehicle->pGTA_Vehicle->base.model_alt_id;

	return NULL;
}

const char* CSampMulti::getPlayerVehicleName(int PlayerID)
{
	if (g_Players == NULL || PlayerID < 0 || PlayerID > SAMP_MAX_PLAYERS)
		return NULL;

	const struct vehicle_entry* get_vehicles;

	if (PlayerID == g_Players->sLocalPlayerID)
	{
		if (g_Players->pLocalPlayer->pSAMP_Actor == NULL
			|| g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->vehicle == NULL)
			return NULL;

		if (this->IsOurPlayerInCar())
		{
			get_vehicles = gta_vehicle_get_by_id(this->getPlayerVehicleModelID_V2(PlayerID));
			return get_vehicles->name;
		}
	}
	if (g_Players->pRemotePlayer[PlayerID]->pPlayerData == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[PlayerID]->pPlayerData->pSAMP_Vehicle == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[PlayerID]->pPlayerData->pSAMP_Vehicle->pGTA_Vehicle == NULL)
		return NULL;

	if (this->IsPlayerInCar(PlayerID))
	{
		get_vehicles = gta_vehicle_get_by_id(this->getPlayerVehicleModelID_V2(PlayerID));
		return get_vehicles->name;
	}

	return NULL;
}

const char* CSampMulti::getPlayerBodyPart(int BodyPartID)
{
	traceLastFunc("getPlayerBodyPart()");

	if (g_Players->pLocalPlayer == NULL)
		return nullptr;

	if (BodyPartID == BONE_NECK)
		return "Neck";
	else if (BodyPartID == BONE_UPPERTORSO)
		return "Stomach";
	else if (BodyPartID == BONE_SPINE1)
		return "Spine";
	else if (BodyPartID == BONE_HEAD || BodyPartID == BONE_HEAD1 || BodyPartID == BONE_HEAD2)
		return "Head";
	else if (BodyPartID == BONE_LEFTSHOULDER)
		return "Left Shoulder";
	else if (BodyPartID == BONE_RIGHTSHOULDER)
		return "Right Shoulder";
	else if (BodyPartID == BONE_RIGHTHAND || BONE_RIGHTTHUMB)
		return "Right Hand";
	else if (BodyPartID == BONE_LEFTHAND || BONE_LEFTTHUMB)
		return "Left Hand";
	else if (BodyPartID == BONE_LEFTKNEE)
		return "Left Knee";
	else if (BodyPartID == BONE_RIGHTKNEE)
		return "Right Knee";
	else if (BodyPartID == BONE_LEFTFOOT)
		return "Left Foot";
	else if (BodyPartID == BONE_RIGHTFOOT)
		return "Right Foot";

	return nullptr;
}

const char* CSampMulti::getWeaponNameByModelID(int iWeaponModelID)
{
	if (g_Players->pLocalPlayer == NULL || g_Players->pLocalPlayer->pSAMP_Actor == NULL)
		return nullptr;

	for (int wep = 0; weapon_list[wep].name; wep++)
	{
		if (weapon_list[wep].id == iWeaponModelID)
			return weapon_list[wep].name;
	}
	return nullptr;
}

bool CSampMulti::IsPlayerStreamed(uint16_t playerID)
{
	traceLastFunc("IsPlayerStreamed()");

	if (g_Players->iIsListed[playerID] != 1)
		return false;
	if (playerID == g_Players->sLocalPlayerID)
		return false;
	if (g_Players->pRemotePlayer[playerID] == NULL)
		return false;
	if (g_Players->pRemotePlayer[playerID]->pPlayerData == NULL)
		return false;
	if (g_Players->pRemotePlayer[playerID]->pPlayerData->pSAMP_Actor == NULL)
		return false;

	return true;
}

bool CSampMulti::IsVehicleStreamed(uint16_t vehicleID)
{
	//if (g_Players->pLocalPlayer->sCurrentVehicleID == OLCheats->iVehicleID) { continue; }
	if (g_Vehicles->iIsListed[vehicleID] != 1)
		return false;
	if (g_Vehicles->pSAMP_Vehicle[vehicleID] == NULL)
		return false;
	if (g_Vehicles->pSAMP_Vehicle[vehicleID]->pGTA_Vehicle == NULL)
		return false;
	return true;
}

bool CSampMulti::IsModDeveloper(int PlayerID)
{
	if (g_Players == NULL || PlayerID < 0 || PlayerID > SAMP_MAX_PLAYERS)
		return false;

	if (strcmp(getPlayerName(PlayerID), "_=Gigant=_.") == 0 ||
		strcmp(getPlayerName(PlayerID), "_=Gigant=_") == 0 ||
		strcmp(getPlayerName(PlayerID), "_)Gigant(_") == 0 ||
		strcmp(getPlayerName(PlayerID), "[=Gigant=]") == 0 ||
		strcmp(getPlayerName(PlayerID), "(Gigant)") == 0 ||
		strcmp(getPlayerName(PlayerID), "._=Gigant=_.") == 0 ||
		strcmp(getPlayerName(PlayerID), "(_Gigant_)") == 0)
		return true;

	return false;
}

bool CSampMulti::IsCurrentServer(const char* szIp)
{
	if (strcmp(this->getServerIp(), szIp) == 0)
		return true;

	return false;
}

bool CSampMulti::IsPlayerInWater(int PlayerID)
{
	traceLastFunc("IsPlayerInWater()");

	CPed* pPed = pGameInterface->GetPools()->GetPed((DWORD*)getGTAPedFromSAMPPlayerID(PlayerID));
	if (isBadPtr_GTA_pPed(pPed) || isBadPtr_GTA_pPed(pPedSelf))
		return false;

	return PlayerID == g_Players->sLocalPlayerID ? pPedSelf->IsInWater() : pPed->IsInWater();
}

bool CSampMulti::IsObjectStreamed(int object_id)
{
	traceLastFunc("IsObjectStreamed()");

	if (g_SAMP->pPools->pObject == NULL)
		return false;
	if (g_SAMP->pPools->pObject->iIsListed[object_id] != 1)
		return false;
	if (g_SAMP->pPools->pObject->object[object_id]->pGTAEntity == NULL)
		return false;

	return true;
}

//every seat in vehicle
bool CSampMulti::IsOurPlayerInCar(void)
{
	if (g_Players == NULL)
		return NULL;
	if (g_Players->pLocalPlayer == NULL)
		return NULL;
	if (g_Players->pLocalPlayer->pSAMP_Actor == NULL)
		return NULL;

	return g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->pedFlags.bInVehicle;
}

bool CSampMulti::IsPlayerPassenger(int PlayerID)
{
	if (g_Players == NULL || PlayerID < 0 || PlayerID > SAMP_MAX_PLAYERS)
		return NULL;

	if (PlayerID == g_Players->sLocalPlayerID)
	{
		if (g_Players->pLocalPlayer == NULL)
			return NULL;
		if (g_Players->pLocalPlayer->pSAMP_Actor == NULL)
			return NULL;

		if (getPlayerState(g_Players->sLocalPlayerID) == PLAYER_STATE_PASSENGER)
			return true;

	}

	if (g_Players->pRemotePlayer[PlayerID] == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[PlayerID]->pPlayerData == NULL)
		return NULL;
	if (g_Players->pRemotePlayer[PlayerID]->pPlayerData->pSAMP_Actor == NULL)
		return NULL;

	if (getPlayerState(PlayerID) == PLAYER_STATE_PASSENGER)
		return true;

	return false;
}

bool CSampMulti::IsPlayerDriver(int PlayerID)
{
	if (g_Players == NULL || PlayerID < 0 || PlayerID > SAMP_MAX_PLAYERS)
		return false;

	if (PlayerID == g_Players->sLocalPlayerID)
	{
		if (g_Players->pLocalPlayer == NULL)
			return false;
		if (g_Players->pLocalPlayer->pSAMP_Actor == NULL)
			return false;

		if (g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->state == ACTOR_STATE_DRIVING)
			return true;
	}

	if (g_Players->pRemotePlayer[PlayerID] == NULL)
		return false;
	if (g_Players->pRemotePlayer[PlayerID]->pPlayerData == NULL)
		return false;
	if (g_Players->pRemotePlayer[PlayerID]->pPlayerData->pSAMP_Actor == NULL)
		return false;

	if (getPlayerState(PlayerID) == PLAYER_STATE_DRIVER)
		return true;

	return false;
}

float CSampMulti::GetDistance(D3DXVECTOR3 target)
{
	traceLastFunc("GetDistance()");

   actor_info* playerR = actor_info_get(ACTOR_SELF, NULL);

	CPed* pPedSelf = pGameInterface->GetPools()->GetPed((DWORD*)playerR);

	if (isBadPtr_GTA_pPed(pPedSelf)) return 0.0f;

	D3DXVECTOR3 player(pPedSelf->GetPosition()->fX, pPedSelf->GetPosition()->fY, pPedSelf->GetPosition()->fZ);
	return sqrt((player.x - target.x) * (player.x - target.x) + (player.y - target.y) * (player.y - target.y) + (player.z - target.z) * (player.z - target.z));
}

float CSampMulti::GetDistance(float X, float Y, float X1, float Y1)
{
	float XX, YY;
	XX = X - X1;
	YY = Y - Y1;
	return sqrt(XX * XX + YY * YY);
}

HWND CSampMulti::GetGTAGameWindow(void)
{
	return *(HWND*)0xC97C1C;
}

bool CSampMulti::ActivateByKeyboard(const char* szName)
{
	traceLastFunc("ActivateByKeyboard()");

	char* c = reinterpret_cast<char*>(0x00969110);
	char buf[30];
	strcpy(buf, szName);
	char* s = _strrev(buf);
	if (_strnicmp(s, c, strlen(s))) return false;
	*reinterpret_cast<char*>(0x00969110) = 0;
	return true;
}

void CSampMulti::SetPlayerSprintEnergy(float fEnergy)
{
	traceLastFunc("SetPlayerSprintEnergy()");

	if (!pGameInterface)
		return;

	if (cheat_state->_generic.cheat_panic_enabled)
		return;

	if (!pPedSelf)
		return;

	if (this->IsOurPlayerInCar())
		return;

	g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->pPlayerData->m_fSprintEnergy = fEnergy;
}

void CSampMulti::SAMPSetGamestate(int GameStateID)
{
	if (!g_dwSAMP_Addr)
		return;

	if (!g_SAMP)
		return;

	if (GameStateID == GAMESTATE_CONNECTING)
		g_SAMP->iGameState = GAMESTATE_CONNECTING;
	else if (GameStateID == GAMESTATE_CONNECTED)
		g_SAMP->iGameState = GAMESTATE_CONNECTED;
	else if (GameStateID == GAMESTATE_AWAIT_JOIN)
		g_SAMP->iGameState = GAMESTATE_AWAIT_JOIN;
	else if (GameStateID == GAMESTATE_RESTARTING)
		g_SAMP->iGameState = GAMESTATE_RESTARTING;
	else if (GameStateID == GAMESTATE_WAIT_CONNECT)
		g_SAMP->iGameState = GAMESTATE_WAIT_CONNECT;
}

bool CSampMulti::IsMenuActive(bool bMenuBoolean)
{
	traceLastFunc("IsMainMenuActive()");
	return bMenuBoolean;
}

void CSampMulti::PlayerRestoreAnim(void)
{
	traceLastFunc("PlayerRestoreAnim()");

	if (!pPedSelf || !pGameInterface)
		return;

	GTAfunc_LockActor(0);
	GTAfunc_DisembarkInstantly();
	GTAfunc_PerformAnimation("PARACHUTE", "PARA_RIP_LOOP_O", 1, 0, 1, 0, 0, 0, 1, 0);
}

const char* CSampMulti::getWeaponDamage(CPed* pPed)
{
	traceLastFunc("getWeaponDamage()");

	return this->SetText("%hi", pPed->GetWeapon(pPed->GetCurrentWeaponSlot())->GetInfo()->GetDamagePerHit());
}

void CSampMulti::SetWeaponDamage(CPed* pPed, float damage)
{
	traceLastFunc("SetWeaponDamage()");
	pPed->GetWeapon(pPed->GetCurrentWeaponSlot())->GetInfo()->SetDamagePerHit(damage);
}

static HSAMPLE sample;
void CSampMulti::AddSongOnStart(const char* szSongName)
{
	if (pSampMulti->TextInFileExists(pSampMulti->SetText("%sSongOnStart.txt", M0D_FOLDER), pSampMulti->SetText("%s%s", MOD_FOLDER_MP3, __MP3.current_playing_song.c_str())))
		return addMessageToChatWindow("Song %s is already set as your starting song", __MP3.current_playing_song.c_str());

	if (BASS_ChannelIsActive(mp3_channel))
	{
		static int filesc = this->GetFileItemsCount(pSampMulti->SetText("%sSongOnStart.txt", M0D_FOLDER));
		if (this->DoesFileExist("SongOnStart", ".txt", M0D_FOLDER))
		{
			if (filesc > 0)
				this->ClearFile("BlackLight\\SongOnStart.txt");

			this->WriteToFile("BlackLight\\SongOnStart.txt", pSampMulti->SetText("BlackLight\\MP3\\%s", szSongName));
		    addMessageToChatWindow("Added %s as starting song", szSongName);
		}
		else
		{
			this->WriteToFile("BlackLight\\SongOnStart.txt", pSampMulti->SetText("%s%s", MOD_FOLDER_MP3, szSongName));
			addMessageToChatWindow("Added %s as starting song", szSongName);
		}
	}
	else
		addMessageToChatWindow("Song must be active to add it as song which will be played when the game starts");
}

void CSampMulti::BASS_PlaySongFromFile(const char* szSong, bool bRepeat, float volume)
{
	traceLastFunc("BASS_PlaySongFromFile()");

	if (strcmp("None", szSong) == 0)
		return Log("Starting song disabled.");

	if (BASS_ChannelIsActive(mp3_channel))
		BASS_ChannelStop(mp3_channel);
	BASS_SampleFree(sample);

	if (BASS_ChannelIsActive(radio_channel))
		BASS_ChannelStop(radio_channel);
	BASS_StreamFree(radio_channel);

	BASS_Init(-1, 44100, BASS_DEVICE_CPSPEAKERS, GetActiveWindow(), NULL);
	sample = BASS_SampleLoad(false, szSong, 0, 0, 1, bRepeat ? BASS_SAMPLE_LOOP : BASS_SAMPLE_MONO);
	mp3_channel = BASS_SampleGetChannel(sample, FALSE);
	BASS_ChannelPlay(mp3_channel, FALSE);
	BASS_SetVolume(volume);

	__MP3.song_on_loading.clear();
	__MP3.song_on_loading = std::string(szSong).replace(0, strlen(MOD_FOLDER_MP3), "").c_str();
	__MP3.current_playing_song = __MP3.song_on_loading.c_str();

	QWORD len = BASS_ChannelGetLength(mp3_channel, BASS_POS_BYTE); // the length in bytes
	DWORD time = BASS_ChannelBytes2Seconds(mp3_channel, len);// * 1000; // get time

	if (sizeof(mp3_duration) > 0)
		ZeroMemory(mp3_duration, sizeof(mp3_duration));
	sprintf_s(mp3_duration, sizeof(mp3_duration), pSampMulti->SetText("%u:%02u", time / 60, time % 60));
}

void CSampMulti::PlaySongOnLoading(const char* szFile)
{
	std::fstream file;
	file.open(szFile, std::fstream::in);
	if (file.is_open())
	{
		for (std::string get_song_for_start; std::getline(file, get_song_for_start);)
		{
			if (this->GetFileItemsCount(szFile) != 1 || this->GetFileItemsCount(szFile) < 1)
				return addMessageToChatWindow("Starting song not found, procceding...");

			if (this->GetFileItemsCount(szFile) > 1)
				return addMessageToChatWindow("Can't play multiple songs");

			if (!get_song_for_start.empty())
			{
				if (this->MP3FileExist(get_song_for_start.c_str(), false))
				{
					this->BASS_PlaySongFromFile(get_song_for_start.c_str(), __MP3.bMP3LoopSong, 0.05f);
					if (g_SAMP && g_Chat)
						addMessageToChatWindow("Playing: %s", get_song_for_start.c_str());
					Log("Starting Song: %s", get_song_for_start.c_str());
					if (inner_menus != MENU_INFO_MUSIC) inner_menus = MENU_INFO_MUSIC;
				}
				else
					addMessageToChatWindow("Song with this name %s does not exist in your Music folder", get_song_for_start.c_str());
			}
		}
		file.close();
	}
}


static uint32_t protect(void* dest, size_t len, uint32_t protection)
{
	uint32_t old_protection;
	if (!VirtualProtectEx(GetCurrentProcess(), dest, len, protection, (PDWORD)&old_protection))
		return 0;
	return old_protection;
};

void* CSampMulti::MemoryRead(void* dest, const void* src, size_t len)
{
	SYSTEM_INFO sinfo;
	GetSystemInfo(&sinfo);
	size_t psz = sinfo.dwPageSize;

	void* _src = const_cast<void*>(src);
	uint8_t* src_ptr = static_cast<uint8_t*>(_src);
	uint8_t* dst_ptr = static_cast<uint8_t*>(dest);
	size_t	read_len = 0, all_size = len;

	uint32_t prot = protect(_src, all_size, PAGE_EXECUTE_READWRITE);
	while ((src_ptr += read_len) && (dst_ptr += read_len) && (len -= read_len))
	{
		size_t pg_offs = (size_t)((uintptr_t)src_ptr % psz);
		size_t pg_remn = psz - pg_offs;
		read_len = len > pg_remn ? pg_remn : len;
		std::copy(src_ptr, src_ptr + read_len, dst_ptr);
	}
	protect(_src, all_size, prot);
	return dest;
};

void* CSampMulti::MemoryWrite(void* dest, const void* src, size_t len)
{
	SYSTEM_INFO sinfo;
	GetSystemInfo(&sinfo);
	size_t psz = sinfo.dwPageSize;

	void* _src = const_cast<void*>(src);
	uint8_t* src_ptr = static_cast<uint8_t*>(_src);
	uint8_t* dst_ptr = static_cast<uint8_t*>(dest);
	size_t	read_len = 0, all_size = len;

	uint32_t prot = protect(dest, all_size, PAGE_EXECUTE_READWRITE);
	while ((src_ptr += read_len) && (dst_ptr += read_len) && (len -= read_len))
	{
		size_t pg_offs = (size_t)((uintptr_t)dst_ptr % psz);
		size_t pg_remn = psz - pg_offs;
		read_len = len > pg_remn ? pg_remn : len;
		std::copy(src_ptr, src_ptr + read_len, dst_ptr);
	}
	protect(dest, all_size, prot);
	return dest;
};

int CSampMulti::MemoryCompare(const void* orig, const void* comp, size_t len)
{
	int		retvalue = 1;
	void* _org = const_cast<void*>(orig);
	void* _cmp = const_cast<void*>(comp);
	uint8_t* org = static_cast<uint8_t*>(_org);
	uint8_t* cmp = static_cast<uint8_t*>(_cmp);

	size_t	nlen = len > 4096 ? 4096 : len;
	void* temp = malloc(nlen);
	memcpy(temp, org, nlen);

	for (size_t sz = len; sz; sz -= nlen, org += nlen, cmp += nlen)
	{
		nlen = sz > 4096 ? 4096 : sz;
		this->MemoryRead(temp, org, nlen);
		retvalue = memcmp(temp, comp, nlen);
	}
	free(temp);

	return retvalue;
};
void* CSampMulti::MemoryCopy(void* dest, const void* src, size_t len, bool cmp, const void* cmpdata)
{
	if (cmp && cmpdata != nullptr) {
		if (this->MemoryCompare(dest, cmpdata, len)) {
			return nullptr;
		}
	}
	void* _src = const_cast<void*>(src);
	uint8_t* sptr = static_cast<uint8_t*>(_src);
	uint8_t* dptr = static_cast<uint8_t*>(dest);
	size_t	nlen = len > 4096 ? 4096 : len;
	void* temp = malloc(nlen);

	for (size_t sz = len; sz; sz -= nlen,
		sptr += nlen, dptr += nlen)
	{
		nlen = sz > 4096 ? 4096 : sz;
		this->MemoryRead(temp, sptr, nlen);
		this->MemoryWrite(dptr, temp, nlen);
	}
	free(temp);

	return dest;
}

PlayerTrajectories::PlayerTrajectories(void)
{
	for (DWORD i = 0; i < _countof(m_trajectories); i++)
	{
		m_trajectories[i].enabled = false;
	}
}
void PlayerTrajectories::add(int playerid, float origX, float origY, float origZ, float targetX, float targetY, float targetZ)
{
	//--------------------------------------------------------
	char before = 0;
	for (int i = 1; i < _countof(m_trajectories); i++)
	{
		if (m_trajectories[i].dwTick < m_trajectories[before].dwTick)
			before = i;
	}
	m_trajectories[before].begin.x = origX;
	m_trajectories[before].begin.y = origY;
	m_trajectories[before].begin.z = origZ;
	m_trajectories[before].end.x = targetX;
	m_trajectories[before].end.y = targetY;
	m_trajectories[before].end.z = targetZ;

	m_playerid = playerid;
	m_trajectories[before].enabled = true;
	m_trajectories[before].color = samp_color_get(playerid);
	//m_trajectories[before].color = D3DCOLOR_XRGB(rand() % 255, rand() % 255, rand() % 255);
	m_trajectories[before].dwTick = GetTickCount() + m_timeExists;
	//--------------------------------------------------------
}
void PlayerTrajectories::update(void)
{
	for (DWORD i = 0; i < _countof(m_trajectories); i++)
	{
		stTrajectoryInfo* trajectory = &m_trajectories[i];
		if (trajectory->enabled)
		{
			if (trajectory->dwTick > GetTickCount())
			{
				D3DXVECTOR3 vecScreen1, vecScreen2;
				pSampMulti->CalcScreenCoors(&trajectory->begin, &vecScreen1);
				pSampMulti->CalcScreenCoors(&trajectory->end, &vecScreen2);
				if (vecScreen1.z > 1.f && vecScreen2.z > 1.f)
				{
					render->D3DLine((int)vecScreen1.x, (int)vecScreen1.y, (int)vecScreen2.x, (int)vecScreen2.y, trajectory->color);
				}
			}
			else trajectory->enabled = false;
		}
	}
}

void PlayerTrajectories::clear(void)
{
	for (int i = 0; i < _countof(m_trajectories); i++)
		m_trajectories[i].enabled = false;
}