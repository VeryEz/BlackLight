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

int			g_iSpectateEnabled = 0, g_iSpectateLock = 0, g_iSpectatePlayerID = -1;
int			g_iJoiningServer = 0;
int			iClickWarpEnabled = 0;
int			g_iNumPlayersMuted = 0;
bool		g_bPlayerMuted[SAMP_MAX_PLAYERS];

void sampMainCheat(void)
{
	traceLastFunc("sampMainCheat()");

	// g_Vehicles & g_Players pointers need to be refreshed or nulled
	if (isBadPtr_writeAny(g_SAMP->pPools, sizeof(stSAMPPools)))
	{
		g_Vehicles = NULL;
		g_Players = NULL;
	}
	else if (g_Vehicles != g_SAMP->pPools->pVehicle || g_Players != g_SAMP->pPools->pPlayer)
	{
		if (isBadPtr_writeAny(g_SAMP->pPools->pVehicle, sizeof(stVehiclePool)))
			g_Vehicles = NULL;
		else
			g_Vehicles = g_SAMP->pPools->pVehicle;
		if (isBadPtr_writeAny(g_SAMP->pPools->pPlayer, sizeof(stPlayerPool)))
			g_Players = NULL;
		else
			g_Players = g_SAMP->pPools->pPlayer;
	}

	// update GTA to SAMP translation structures
	update_translateGTASAMP_vehiclePool();
	update_translateGTASAMP_pedPool();

	spectateHandle();
	sampAntiHijack();

	// start chatbox logging
	if (BlackLightFuncs->bChatboxLog)
	{
		static int	chatbox_init;
		if (!chatbox_init)
		{
			SYSTEMTIME	time;
			GetLocalTime(&time);
			LogChatbox(false, "Session started at %02d/%02d/%02d", time.wDay, time.wMonth, time.wYear);
			chatbox_init = 1;
		}
	}

	if (KEYCOMBO_PRESSED(set.key_rejoin))
	{
		restartGame();
		disconnect(500);
		cheat_state_text("Rejoining in %d seconds...", set.rejoin_delay / 1000);
		cheat_state->_generic.rejoinTick = GetTickCount();
	}

	if (KEYCOMBO_PRESSED(set.key_respawn))
		playerSpawn();
	
	if (KEYCOMBO_DOWN(set.chat_secondary_key))
	{
		int			i, key, spam;
		const char	*msg;
		for (i = 0; i < INI_CHATMSGS_MAX; i++)
		{
			struct chat_msg *msg_item = &set.chat[i];
			if (msg_item->key == NULL)
				continue;
			if (msg_item->msg == NULL)
				continue;
			if (msg_item->key != key_being_pressed)
				continue;
			key = msg_item->key;
			msg = msg_item->msg;
			spam = msg_item->spam;
			if (spam)
			{
				if (msg)
					if (KEY_DOWN(key))
						say("%s", msg);
			}
			else
			{
				if (msg)
					if (KEY_PRESSED(key))
						say("%s", msg);
			}
		}
	}
	if (BlackLightFuncs->bClickWarp && iIsSAMPSupported)
	{
		if (KEYCOMBO_PRESSED(set.key_clickwarp_enable))
		{
			g_iCursorEnabled ^= 1;
			toggleSAMPCursor(g_iCursorEnabled);
		}
		if (g_iCursorEnabled && KEYCOMBO_PRESSED(set.key_clickwarp_click))
		{
			iClickWarpEnabled = 1;
		}
	}

	static int	iSAMPHooksInstalled;
	if (!iSAMPHooksInstalled)
	{
		installSAMPHooks();
		iSAMPHooksInstalled = 1;
	}

	if (cheat_state->_generic.rejoinTick && cheat_state->_generic.rejoinTick < (GetTickCount() - set.rejoin_delay))
	{
		g_SAMP->iGameState = GAMESTATE_WAIT_CONNECT;
		cheat_state->_generic.rejoinTick = 0;
	}

	if (g_iJoiningServer == 1)
	{
		restartGame();
		disconnect(500);
		cheat_state_text("Joining server in %d seconds...", set.rejoin_delay / 1000);
		cheat_state->_generic.join_serverTick = GetTickCount();
		g_iJoiningServer = 2;
	}

	if (g_iJoiningServer == 2
		&& cheat_state->_generic.join_serverTick
		&&	 cheat_state->_generic.join_serverTick < (GetTickCount() - set.rejoin_delay))
	{
		g_SAMP->iGameState = GAMESTATE_WAIT_CONNECT;
		g_iJoiningServer = 0;
		cheat_state->_generic.join_serverTick = 0;
	}
}

void spectateHandle(void)
{
	if (g_iSpectateEnabled)
	{
		if (g_iSpectateLock) return;

		if (g_iSpectatePlayerID != -1)
		{
			if (g_Players->iIsListed[g_iSpectatePlayerID] != 0)
			{
				if (g_Players->pRemotePlayer[g_iSpectatePlayerID] != NULL)
				{
					int iState = getPlayerState(g_iSpectatePlayerID);

					if (iState == PLAYER_STATE_ONFOOT)
					{
						struct actor_info *pPlayer = getGTAPedFromSAMPPlayerID(g_iSpectatePlayerID);
						if (pPlayer == NULL) return;
						GTAfunc_CameraOnActor(pPlayer);
						g_iSpectateLock = 1;
					}
					else if (iState == PLAYER_STATE_DRIVER)
					{
						struct vehicle_info *pPlayerVehicleID = g_Players->pRemotePlayer[g_iSpectatePlayerID]->pPlayerData->pSAMP_Vehicle->pGTA_Vehicle;
						if (pPlayerVehicleID == NULL) return;
						GTAfunc_CameraOnVehicle(pPlayerVehicleID);
						g_iSpectateLock = 1;
					}
					else if (iState == PLAYER_STATE_PASSENGER)
					{
						struct vehicle_info *pPlayerVehicleID = g_Players->pRemotePlayer[g_iSpectatePlayerID]->pPlayerData->pSAMP_Vehicle->pGTA_Vehicle;
						if (pPlayerVehicleID == NULL) return;
						GTAfunc_CameraOnVehicle(pPlayerVehicleID);
						g_iSpectateLock = 1;
					}
				}
				else
				{
					cheat_state_text("Player is not streamed in");
					g_iSpectateEnabled = 0;
				}
			}
		}
	}
}

void spectatePlayer(int iPlayerID)
{
	if (iPlayerID == -1)
	{
		GTAfunc_TogglePlayerControllable(0);
		GTAfunc_LockActor(0);
		pGameInterface->GetCamera()->RestoreWithJumpCut();

		g_iSpectateEnabled = 0;
		g_iSpectateLock = 0;
		g_iSpectatePlayerID = -1;
		return;
	}

	g_iSpectatePlayerID = iPlayerID;
	g_iSpectateLock = 0;
	g_iSpectateEnabled = 1;
}

void sampAntiHijack(void)
{
	if (g_SAMP == NULL) return;
	if (g_Vehicles == NULL) return;
	traceLastFunc("sampAntiHijack()");

	vehicle_info *veh = vehicle_info_get(VEHICLE_SELF, VEHICLE_ALIVE);
	if (BlackLightFuncs->bAntiCarJack && veh == NULL)
	{
		if (cheat_state->_generic.got_vehicle_id)
			cheat_state->_generic.got_vehicle_id = false;
		if (cheat_state->_generic.anti_carjackTick
			&&	 cheat_state->_generic.anti_carjackTick < (GetTickCount() - 500)
			&& cheat_state->_generic.car_jacked)
		{
			if (g_SAMP->pPools->pPlayer->pLocalPlayer->sLastVehicleID == 0)
			{
				GTAfunc_showStyledText("~r~Unable To Unjack~w~!", 1000, 5);
				cheat_state->_generic.anti_carjackTick = 0;
				cheat_state->_generic.car_jacked = false;
				return;
			}

			cheat_state->_generic.anti_carjackTick = 0;
			cheat_state->_generic.car_jacked = false;
			GTAfunc_PutActorInCar(GetVehicleByGtaId(g_SAMP->pPools->pPlayer->pLocalPlayer->sLastVehicleID));

			struct vehicle_info *veh = GetVehicleByGtaId(g_SAMP->pPools->pPlayer->pLocalPlayer->sLastVehicleID);
			if ( veh != NULL )
		 	vect3_copy( cheat_state->_generic.car_jacked_lastPos, &veh->base.matrix[4 * 3] );
			GTAfunc_showStyledText("~r~Car Unjacked~w~!", 1000, 5);
			return;
		}
	}
	else if (BlackLightFuncs->bAntiCarJack)
	{
		if (veh->passengers[0] == actor_info_get(ACTOR_SELF, 0))
		{
			if (!cheat_state->_generic.got_vehicle_id)
			{
				g_SAMP->pPools->pPlayer->pLocalPlayer->sLastVehicleID = getPlayerVehicleGTAScriptingID(ACTOR_SELF);
				if (g_SAMP->pPools->pPlayer->pLocalPlayer->sLastVehicleID > 0)
					cheat_state->_generic.got_vehicle_id = true;
			}
		}
	}
}

//incoming rpc
void HandleRPCPacketFunc(unsigned char id, RPCParameters *rpcParams, void(*callback) (RPCParameters *))
{
	if (!isCheatPanicEnabled())
	{
		if (set.netPatchAssoc[id][INCOMING_RPC] != nullptr && set.netPatchAssoc[id][INCOMING_RPC]->enabled)
			return;
	}
	if (rpcParams != nullptr && rpcParams->numberOfBitsOfData >= 8)
	{
		switch (id)
		{

			case RPC_SetPlayerHealth:
			{
				if (isCheatPanicEnabled() || !set.enable_extra_godmode || !cheat_state->_generic.hp_cheat) break;

				actor_info *self = actor_info_get(ACTOR_SELF, NULL);
				if (self)
				{
					BitStream bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);
					float fHealth;
					bsData.Read(fHealth);
					if (fHealth < self->hitpoints)
					{
						cheat_state_text("Warning: Server tried change your health to %0.1f", fHealth);
						return; // exit from the function without processing RPC
					}
				}
				break;
			}
			case RPC_SetVehicleHealth:
			{
				if (isCheatPanicEnabled() || !set.enable_extra_godmode || !cheat_state->_generic.hp_cheat) break;

				vehicle_info *vself = vehicle_info_get(VEHICLE_SELF, NULL);
				if (vself)
				{
					BitStream bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);
					short sId;
					float fHealth;
					bsData.Read(sId);
					bsData.Read(fHealth);
					if (sId == g_Players->pLocalPlayer->sCurrentVehicleID && fHealth < vself->hitpoints)
					{
						cheat_state_text("Warning: Server tried change your vehicle health to %0.1f", fHealth);
						return; // exit from the function without processing RPC
					}
				}
				break;
			}
			case RPC_ClientMessage:
			{
				if (isCheatPanicEnabled()) break;

				BitStream		bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);
				uint32_t		dwStrLen, dwColor;
				char			szMsg[256];
				static char		last_servermsg[256];

				bsData.Read(dwColor);
				bsData.Read(dwStrLen);
				if (dwStrLen >= sizeof(szMsg)) dwStrLen = sizeof(szMsg) - 1;
				bsData.Read(szMsg, dwStrLen);
				szMsg[dwStrLen] = '\0';

				if (BlackLightFuncs->bChatboxLog) LogChatbox(false, "%s", szMsg);

				break;
			}
			case RPC_Chat:
			{
				if (isCheatPanicEnabled()) break;

				BitStream		bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);
				static char		last_clientmsg[SAMP_MAX_PLAYERS][256];
				uint16_t		playerId = uint16_t(-1);
				uint8_t			byteTextLen;
				char			szText[256];

				if (cheat_state->_generic.cheat_panic_enabled)
					break;

				bsData.Read(playerId);
				if (isBadSAMPPlayerID(playerId))
					break;

				bsData.Read(byteTextLen);
				bsData.Read(szText, byteTextLen);
				szText[byteTextLen] = '\0';

				if (BlackLightFuncs->bChatboxLog)
					LogChatbox(false, "%s: %s", getPlayerName(playerId), szText);

				uint8_t startBracket = 0, endBracket = 0;
				bool bCheckID = false;
				std::string message(szText);

				bool bOpenBracket = false;
				for (int i = 0; i < 256; i++)
				{
					if (bOpenBracket)
					{
						if (szText[i] == ')')
						{
							bCheckID = true;
							endBracket = i;
							break;
						}
					}
					if (szText[i] == '(')
					{
						startBracket = i;
						bOpenBracket = true;
					}
				}
				if (bCheckID)
					message.replace(startBracket, endBracket - startBracket + 1, "");
				strcpy(szText, message.c_str());

				//if (message.find(">blok", strlen(message.c_str())))
					//addMessageToChatWindow("found");

				if (BlackLightFuncs->Mute.bMutedPlayers[playerId]) //is muted
				{
					if (BlackLightFuncs->bMutedPlayerChatting)
					{
						if (message.find(getPlayerName(playerId), strlen(message.c_str())) ||
							message.find(pSampMulti->SetText("%d", g_Players->sLocalPlayerID), strlen(message.c_str())))
							addMessageToChatWindow("Muted Player %d", playerId);
					}
					return;
				}

				if (BlackLightFuncs->_CopyChat.bCopyChat[playerId])
				{
					if (set.BlackLight.Target.TargetPlayer_CopyChat == playerId)
					{
						if (pSampMulti->IsModDeveloper(playerId))
							return addMessageToChatWindow("You can't troll Mod Developer %s", getPlayerName(playerId));
						if (BlackLightFuncs->bIsFriend[playerId])
							return addMessageToChatWindow("You can't troll players from your friend list!");

						if (message.length() == message.find(getPlayerName(g_Players->sLocalPlayerID), message.length()))
						{
							std::string textf;
							textf = szText;
							textf.replace(textf.find(getPlayerName(g_Players->sLocalPlayerID), strlen(textf.c_str())), strlen(textf.c_str()), getPlayerName(playerId));
							say("%s", textf.c_str());
						}
						else
							say(szText);
					}
				}

				if (g_BotFuncs->BotSettings.bFollow
					&& g_BotFuncs->BotSettings.bCopyChat
					&& g_BotFuncs->BotSettings.UseFakeBot
					&& set.BlackLight.Target.TargetPlayer_ForBots == playerId)
				{
					BitStream bsSend;
					BYTE byteTextLen = (BYTE)strlen(szText);
					bsSend.Write(byteTextLen);
					bsSend.Write(szText, byteTextLen);
					for (int i = g_BotFuncs->BotSettings.ClientCount - 1; i >= 0; i--)
						g_BotFuncs->Bot_SendRPC(i, RPC_Chat, bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);

				}

				break;
			}

			case RPC_DeathMessage:
			{
				if (isCheatPanicEnabled()) break;

				BitStream bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);

				UINT16 wKillerID;
				UINT16 wPlayerID;
				UINT8 reason;

				bsData.ResetReadPointer();
				bsData.Read(wKillerID);
				bsData.Read(wPlayerID);
				bsData.Read(reason);

				if (g_Players->iIsListed[wKillerID] != 1 && g_Players->iIsListed[wPlayerID] != 1)
					break;

				if (BlackLightFuncs->bDeathLogChat)
				{
					if (reason == DEATH_REASON_ACCIDENT) addMessageToChatWindow("{00FF00}%s{FF0F0F}(%d){FFFFFF} was killed in an accident", getPlayerName(wPlayerID), wPlayerID);
					else if (reason == DEATH_REASON_DROWN) addMessageToChatWindow("{00FF00}%s{FF0F0F}(%d){FFFFFF} died from drowning :(", getPlayerName(wPlayerID), wPlayerID);
					else if (reason == DEATH_REASON_EXPLOSION) addMessageToChatWindow("{00FF00}%s{FF0F0F}(%d){FFFFFF} died in an explosion", getPlayerName(wPlayerID), wPlayerID);
					else if (reason == DEATH_REASON_HELICOPTER_BLADE) addMessageToChatWindow("{00FF00}%s{FF0F0F}(%d){FFFFFF} has been killed by helicopter blade", getPlayerName(wPlayerID), wPlayerID);
					else if (reason == DEATH_REASON_SUICIDE) addMessageToChatWindow("{00FF00}%s{FF0F0F}(%d){FFFFFF} has commited suicide", getPlayerName(wPlayerID), wPlayerID);
					else if(reason == DEATH_REASON_USELESS1 || reason == DEATH_REASON_USELESS2) { }
					else addMessageToChatWindow("{00FF00}%s(%d) {FF0F0F}killed {F0F0FF}%s(%d) {FFFFFF}(Weapon %s[%d])", getPlayerName(wKillerID), wKillerID, getPlayerName(wPlayerID), wPlayerID, pSampMulti->getWeaponNameByModelID(reason), reason);
				}

				/*if (BlackLightFuncs->bShowKillCounterInsideHud == true)
				{
				}*/
				if (g_Players->sLocalPlayerID == wKillerID)
				{
					set.BlackLight.kill_counter++;
				}

				break;
			}

			/*case RPC_SetPlayerName:
			{
				BitStream bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);
				short int p_id;
				byte p_nameLen;
				char p_name[SAMP_MAX_PLAYER_NAME];

				bsData.Read(p_id);
				bsData.Read(p_nameLen);
				bsData.Read(p_name, p_nameLen);
				p_name[p_nameLen] = '\0';

				const char* p_szPlayerName = getPlayerName(p_id);

				addMessageToChatWindow("%s[%d] -> %s", p_szPlayerName, p_id, p_name);

				break;
			}*/
			case RPC_ServerJoin:
			{
				if (isCheatPanicEnabled() || !BlackLightFuncs->bPlayerJoiner) break;

				BitStream bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);

				short int p_id;
				D3DCOLOR p_color;
				byte p_nameLen, p_isNPC;
				char p_szPlayerName[SAMP_MAX_PLAYER_NAME];

				bsData.ResetReadPointer();
				bsData.Read(p_id);
				bsData.Read(p_color);
				bsData.Read(p_isNPC);
				bsData.Read(p_nameLen);
				bsData.Read(p_szPlayerName, p_nameLen);
				p_szPlayerName[p_nameLen] = '\0';

				if (!p_isNPC)
				{
					if (set.raknet_lines_limit == set.BlackLight.player_joiner_lines_size)
						gRaknetLogger.push_front(pSampMulti->SetText("%s(%d) connected to server", p_szPlayerName, p_id));
					else
						gRaknetLogger.push_back(pSampMulti->SetText("%s(%d) connected to server", p_szPlayerName, p_id));
				}
				break;
			}

			case RPC_ServerQuit:
			{
				traceLastFunc("HandleRPC::RPC_ServerQuit");

				if (isCheatPanicEnabled() || !BlackLightFuncs->bPlayerJoiner) break;

				BitStream bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);

				short int p_id; byte reasonID;

				bsData.ResetReadPointer();
				bsData.Read(p_id);
				bsData.Read(reasonID);
				if (isBadSAMPPlayerID(p_id)) break;

				static char szDisconnectReason[3][10] =
				{
					"crash",
					"exit",
					"kick/ban"
				};

				if (set.raknet_lines_limit == set.BlackLight.player_joiner_lines_size)
					gRaknetLogger.push_front(pSampMulti->SetText("%s(%d) has left the server -> %s", getPlayerName(p_id), p_id, szDisconnectReason[reasonID]));
				else
					gRaknetLogger.push_back(pSampMulti->SetText("%s(%d) has left the server -> %s", getPlayerName(p_id), p_id, szDisconnectReason[reasonID]));

				break;
			}
		} // switch
	}
	callback(rpcParams);
}

//https://github.com/BrunoBM16/samp-packet-list/wiki/RPC-List
bool OnSendRPC(int uniqueID, BitStream *parameters, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp)
{
	if (!isCheatPanicEnabled())
	{
		if (set.netPatchAssoc[uniqueID][OUTCOMING_RPC] != nullptr && set.netPatchAssoc[uniqueID][OUTCOMING_RPC]->enabled)
			return false;
	}

	/*if (uniqueID == RPC_ClientJoin)
	{
		BYTE byteAuthBSLen;
		char auth_bs[64] = { 0 };
		parameters->Read(byteAuthBSLen);
		parameters->Read(auth_bs, byteAuthBSLen);
		parameters->Reset();
		Log("byteAuthBSLen %d", byteAuthBSLen);
		Log("auth_bs %s", auth_bs);

		//Log("%d", &OLCheats->bChangeGPCI);
		if (BlackLightFuncs->bChangeGPCI)
			gen_rand_gpci(auth_bs, 0x3E9);

		byteAuthBSLen = (BYTE)strlen(auth_bs);

		parameters->Write(byteAuthBSLen);
		parameters->Write(auth_bs, byteAuthBSLen);
		cheat_state_text("serial spoofed");
	}*/

	if (uniqueID == RPC_Chat && g_Players != nullptr)
	{
		uint8_t byteTextLen;
		char szText[256];

		parameters->Read(byteTextLen);
		parameters->Read(szText, byteTextLen);
		szText[byteTextLen] = '\0';

		if (BlackLightFuncs->bChatboxLog)
			LogChatbox(false, "%s: %s", getPlayerName(g_Players->sLocalPlayerID), szText);

		if (g_BotFuncs->BotSettings.bCopyChat
			&& !g_BotFuncs->BotSettings.UseFakeBot
			&& g_BotFuncs->BotSettings.bFollow)
		{
			BitStream bsSend;
			BYTE byteTextLen = (BYTE)strlen(szText);
			bsSend.Write(byteTextLen);
			bsSend.Write(szText, byteTextLen);
			for (int i = g_BotFuncs->BotSettings.ClientCount - 1; i >= 0; i--)
				g_BotFuncs->Bot_SendRPC(i, RPC_Chat, bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
		}

	}

	if (uniqueID == RPC_GiveTakeDamage && g_Players != nullptr)
	{
		traceLastFunc("HandleRPC::RPC_GiveTakeDamage");

		if (isCheatPanicEnabled()) return 0;

		bool type;
		WORD playerID;
		float damage;
		int gun, bodyID;

		parameters->ResetReadPointer();
		parameters->Read(type);
		parameters->Read(playerID);
		parameters->Read(damage);
		parameters->Read(gun);
		parameters->Read(bodyID);

		//if (!KEY_PRESSED(VK_LBUTTON) || !KEY_DOWN(VK_LBUTTON) || !KEY_PRESSED(VK_RBUTTON) && !KEY_DOWN(VK_LBUTTON))
			//return false; //check if we are shooting

		if (__Damager.bShowInChat)
		{
			addMessageToChatWindow(pSampMulti->SetText("{FFFFFF}Damage {FF0F0F}%.1f{FFFFFF} to{23E7CF} %s(%d){FFFFFF} weapon{F68F2F} %s{FFFFFF} >{C62FF6} %s{FFFFFF} Distance %.1f"
				, damage
				, getPlayerName(playerID), playerID
				, pSampMulti->getPlayerWeapon(g_Players->sLocalPlayerID)
				, pSampMulti->getPlayerBodyPart(bodyID)
				, pSampMulti->GetDistance(CVecToD3DXVEC(pSampMulti->CVecPlayerPos(playerID)))));
		}

		if (__Damager.bShowOnTarget)
		{
			if (set.BlackLight.get_damaged_player_for_imgui == playerID)
			{
				__Damager.bTarget[set.BlackLight.get_damaged_player_for_imgui] = false;
				return 0;
			}
			set.BlackLight.get_damaged_player_for_imgui = playerID;

			__Damager.dmg_info_delivered = pSampMulti->SetText("%.1f", damage);
			__Damager.dmg_info_player = pSampMulti->SetText("%s(%d)", getPlayerName(playerID), playerID);
			__Damager.dmg_info_weapon = pSampMulti->SetText("%s", pSampMulti->getPlayerWeapon(g_Players->sLocalPlayerID));
			__Damager.dmg_info_body_part = pSampMulti->SetText("%s", pSampMulti->getPlayerBodyPart(bodyID));
			__Damager.dmg_info_distance = pSampMulti->SetText("%.1f", pSampMulti->GetDistance(CVecToD3DXVEC(pSampMulti->CVecPlayerPos(playerID))));
		}
	}


	// prevent invulnerability detection
	if (uniqueID == RPC_ClientCheck && cheat_state && cheat_state->_generic.hp_cheat)
	{
		uint8_t type = 0;
		parameters->Read(type);
		if (type == 2)
		{
			uint32_t arg = 0;
			uint8_t response = 0;
			parameters->Read(arg);
			parameters->Read(response);

			// remove invulnerability flags from our real flags
			uint32_t fakeFlags = arg & (0xFF00FFFF | ((~ACTOR_FLAGS_INVULNERABLE) << 16));

			// reform packet data
			parameters->SetWriteOffset(0);
			parameters->Write(type);
			parameters->Write(fakeFlags);
			parameters->Write(response);
		}
	}

	if (g_BotFuncs->BotSettings.bCopyRPC
		&& g_BotFuncs->BotSettings.bFollow)
	{
		for (int i = g_BotFuncs->BotSettings.ClientCount - 1; i >= 0; i--)
			g_BotFuncs->Bot_SendRPC(i, uniqueID, *parameters, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
	}

	return true;
}

bool OnSendPacket(BitStream *parameters, PacketPriority priority, PacketReliability reliability, char orderingChannel)
{
	if (!isCheatPanicEnabled())
	{
		uint8_t packetId;
		parameters->Read(packetId);
		if (set.netPatchAssoc[packetId][OUTCOMING_PACKET] != nullptr && set.netPatchAssoc[packetId][OUTCOMING_PACKET]->enabled)
			return false;
	}
	return true;
}

bool OnReceivePacket(Packet *p)
{
	if (p->data == nullptr || p->length == 0)
		return true;

	if (!isCheatPanicEnabled())
	{
		if (set.netPatchAssoc[p->data[0]][INCOMING_PACKET] != nullptr && set.netPatchAssoc[p->data[0]][INCOMING_PACKET]->enabled)
			return false;
	}

	if (p->data[0] == ID_CONNECTION_LOST || p->data[0] == ID_DISCONNECTION_NOTIFICATION) 
	{
		if (BlackLightFuncs->bAutoReconnect)
		{
			disconnect(set.BlackLight.auto_reconnect_delay);
			g_SAMP->iGameState = GAMESTATE_WAIT_CONNECT;
		}
	}

	if (p->data[0] == ID_CONNECTION_BANNED)
	{
		if (BlackLightFuncs->bAutoReconnect) {
			disconnect(set.BlackLight.auto_reconnect_delay);
			g_SAMP->iGameState = GAMESTATE_WAIT_CONNECT;
		}
	}


	/*if (p->data[0] == ID_MARKERS_SYNC) // packetId
	{
		BitStream	bs(p->data, p->length, false);
		int			iNumberOfPlayers = 0;
		uint16_t	playerID = uint16_t(-1);
		short		sPos[3] = { 0, 0, 0 };
		bool		bIsPlayerActive = false;

		bs.IgnoreBits(8);
		bs.Read(iNumberOfPlayers);
		if (iNumberOfPlayers < 0 || iNumberOfPlayers > SAMP_MAX_PLAYERS)
			return true;

		for (int i = 0; i < iNumberOfPlayers; i++)
		{
			bs.Read(playerID);
			bs.ReadCompressed(bIsPlayerActive);
			if (bIsPlayerActive == 0 || isBadSAMPPlayerID(playerID))
				continue;
			bs.Read(sPos);

			g_stStreamedOutInfo.iPlayerID[playerID] = playerID;
			g_stStreamedOutInfo.fPlayerPos[playerID][0] = sPos[0];
			g_stStreamedOutInfo.fPlayerPos[playerID][1] = sPos[1];
			g_stStreamedOutInfo.fPlayerPos[playerID][2] = sPos[2];
		}
	}*/

	if (p->data[0] == ID_CONNECTION_LOST || p->data[0] == ID_DISCONNECTION_NOTIFICATION) {
		if (BlackLightFuncs->bAutoReconnect)
			p->data[0] = ID_CONNECTION_LOST;
	}
	if (p->data[0] == ID_CONNECTION_BANNED) {
		if (BlackLightFuncs->bAutoReconnect) {
			disconnect(set.BlackLight.auto_reconnect_delay);
			g_SAMP->iGameState = GAMESTATE_WAIT_CONNECT;
		}
	}

	return true;
}

// commands below

static void cmd_change_server(char *param)	// 127.0.0.1 7777 Username Password
{
	traceLastFunc("cmd_change_server()");

	bool	success = false;

	char	IP[128], Nick[SAMP_MAX_PLAYER_NAME], Password[128] = "", Port[128];
	int		iPort;

	int ipc = sscanf(param, "%s%s%s%s", IP, Port, Nick, Password);
	if (ipc < 2)
	{
		addMessageToChatWindow("USAGE: /.bl_csrv <ip> <port> <Username> <Server Password>");
		addMessageToChatWindow("Variables that are set to \"NULL\" (capitalized) will be ignored.");
		addMessageToChatWindow("If you set the Password to \"NULL\" it is set to <no server password>.");
		addMessageToChatWindow("Username and password can also be left out completely.");
		return;
	}
	if (stricmp(IP, "NULL") == NULL)
		strcpy(IP, g_SAMP->szIP);

	if (stricmp(Port, "NULL") == NULL)
		iPort = g_SAMP->ulPort;
	else
		iPort = atoi(Port);

	if (ipc > 2)
	{
		if (stricmp(Nick, "NULL") != NULL)
		{
			if (strlen(Nick) > SAMP_ALLOWED_PLAYER_NAME_LENGTH)
				Nick[SAMP_ALLOWED_PLAYER_NAME_LENGTH] = '\0';
			setLocalPlayerName(Nick);
		}
	}
	if (ipc > 3)
	{
		if (stricmp(Password, "NULL") == NULL)
			strcpy(Password, "");
	}

	changeServer(IP, iPort, Password);
}

static void cmd_change_server_fav(char *param)
{
	traceLastFunc("cmd_change_server_fav()");

	if (strlen(param) == 0)
	{
		addMessageToChatWindow(".bl.favsrv <server name/part of server name>");
		addMessageToChatWindow("In order to see the favorite server list type: /mod_fav_server list");
		return;
	}

	if (strncmp(param, "list", 4) == 0)
	{
		int count = 0;
		for (int i = 0; i < INI_SERVERS_MAX; i++)
		{
			if (set.server[i].server_name == NULL)
				continue;

			count++;
			addMessageToChatWindow("%s", set.server[i].server_name);
		}
		if (count == 0)
			addMessageToChatWindow("No servers in favorite server list. Edit the ini file to add some.");
		return;
	}

	for (int i = 0; i < INI_SERVERS_MAX; i++)
	{
		if (set.server[i].server_name == NULL || set.server[i].ip == NULL
			|| strlen(set.server[i].ip) < 7 || set.server[i].port == 0)
			continue;

		if (!findstrinstr((char *)set.server[i].server_name, param))
			continue;

		if (!set.use_current_name)
			setLocalPlayerName(set.server[i].nickname);

		changeServer(set.server[i].ip, set.server[i].port, set.server[i].password);

		return;
	}

	addMessageToChatWindow("/.bl.favsrv <server name/part of server name>");
	return;
}

static void cmd_current_server(char *param)
{
	addMessageToChatWindow("Server Name: %s", g_SAMP->szHostname);
	addMessageToChatWindow("Server Address: %s:%i", g_SAMP->szIP, g_SAMP->ulPort);
	addMessageToChatWindow("Username: %s", getPlayerName(g_Players->sLocalPlayerID));
}

static void cmd_tele_loc(char *param)
{
	if (strlen(param) == 0)
	{
		addMessageToChatWindow("USAGE: /.bl.tele_loc <location name>");
		addMessageToChatWindow("Use /.bl.allteleloc to show the location names.");
		addMessageToChatWindow("The more specific you are on location name the better the result.");
		return;
	}

	for (int i = 0; i < STATIC_TELEPORT_MAX; i++)
	{
		if (strlen(set.static_teleport_name[i]) == 0 || vect3_near_zero(set.static_teleport[i].pos))
			continue;

		if (!findstrinstr(set.static_teleport_name[i], param))
			continue;

		addMessageToChatWindow("Teleported to: %s.", set.static_teleport_name[i]);
		cheat_teleport(set.static_teleport[i].pos, set.static_teleport[i].interior_id);
		return;
	}

	addMessageToChatWindow("USAGE: /.bl.tele_loc <location name>");
	addMessageToChatWindow("Use /.bl.allteleloc to show the location names.");
	addMessageToChatWindow("The more specific you are on location name the better the result.");
}

static void cmd_tele_locations(char *)
{
	for (int i = 0; i < STATIC_TELEPORT_MAX; i++)
	{
		if (strlen(set.static_teleport_name[i]) == 0 || vect3_near_zero(set.static_teleport[i].pos))
			continue;

		addMessageToChatWindow("%s", set.static_teleport_name[i]);
	}

	addMessageToChatWindow("To teleport use the mod menu or: /.bl.tele_loc <location name>");
}

static void cmd_playaudiostream(char* param)
{
	if (!strlen(param))
	{
		cheat_state_text("Usage: /.bl.url <url> ");
		return;
	}

	g_RakClient2->PlayAudioStream(param);
}

static void cmd_pickup(char *params)
{
	if (!strlen(params))
	{
		addMessageToChatWindow("USAGE: /.bl.pickup <pickup id>");
		return;
	}

	g_RakClient2->SendPickUp(atoi(params));
}

static void cmd_setclass(char *params)
{
	if (!strlen(params))
	{
		addMessageToChatWindow("USAGE: /.bl.setclass <class id>");
		return;
	}

	g_RakClient2->RequestClass(atoi(params));
	g_RakClient2->SendSpawn();
}

static void cmd_fakekill(char *params)
{
	int killer, reason, amount;
	if (!strlen(params) || sscanf(params, "%d%d%d", &killer, &reason, &amount) < 3)
		return addMessageToChatWindow("USAGE: /.bl.fakekill <killer id> <reason> <amount>");

	if (amount < 1 || killer < 0 || killer > SAMP_MAX_PLAYERS)
		return;

	for (int i = 0; i < amount; i++)
		g_RakClient2->SendDeath(killer, reason);
}

static void cmd_warp(char *params)
{
	if (params[0] == '\0')
		return addMessageToChatWindow("USAGE: /.bl.warp <player id>");

	int playerid = atoi(params);
	if (isBadSAMPPlayerID(playerid) || g_Players->iIsListed[playerid] != 1)
		return addMessageToChatWindow("Player does not exist.");

	if (pSampMulti->IsModDeveloper(playerid))
		return addMessageToChatWindow("You can't use this on Mod Developer");

	float pos[3];
	actor_info *actor = nullptr;
	if (g_Players->pRemotePlayer[playerid]->pPlayerData == nullptr
		|| g_Players->pRemotePlayer[playerid]->pPlayerData->pSAMP_Actor == nullptr)
	{
		if (vect3_near_zero(g_stStreamedOutInfo.fPlayerPos[playerid]))
			return addMessageToChatWindow("Player is not streamed in.");

		vect3_copy(g_stStreamedOutInfo.fPlayerPos[playerid], pos);
		pos[1] += 1.0f;
		cheat_teleport(pos, 0);
		return;
	}

	if (!getPlayerPos(playerid, pos) ||
		g_Players->pRemotePlayer[playerid]->pPlayerData == nullptr ||
		g_Players->pRemotePlayer[playerid]->pPlayerData->pSAMP_Actor == nullptr ||
		(actor = g_Players->pRemotePlayer[playerid]->pPlayerData->pSAMP_Actor->pGTA_Ped) == nullptr)
	{
		return addMessageToChatWindow("Bad player info.");
	}

	pos[1] += 1.0f;
	cheat_teleport(pos, actor->base.interior_id);
}

static void cmd_showCMDS(char *)
{
	for (int i = 0; i < g_m0dCmdNum; i++)
	{
		addMessageToChatWindow("%s", g_m0dCmdlist[i]);
	}
}

static void cmd_clear_chat(char* param)
{
	if (!g_SAMP)
		return;
	if (!g_Chat)
		return;

	if (!strlen(param))
	{
		for (int ichat = 0; ichat < g_Chat->pagesize; ichat++)
			addMessageToChatWindow("");
	}
	else addMessageToChatWindow("Usage: /.bl.cc to clear chat");
}

static void cmd_copy_chat(char* param)
{
	if (!g_SAMP)
		return;
	if (!g_Chat)
		return;

	int idget = atoi(param);
	if (!strlen(param))
	{
		return addMessageToChatWindow("Usage: /.bl.cp player id");
	}
	else
	{
		if (isBadSAMPPlayerID(idget))
			return addMessageToChatWindow("Player does not exist.");

		if (idget == g_Players->sLocalPlayerID)
			return addMessageToChatWindow("You can't use copy chat on yourself");

		if (pSampMulti->IsModDeveloper(idget))
			return addMessageToChatWindow("Won't work on mod developer");

		if (BlackLightFuncs->bIsFriend[idget])
			return addMessageToChatWindow("Friends are protected.");

		if (g_BotFuncs->uiIsBot[idget][0] == 1)
			return addMessageToChatWindow("Can't copy chat from your bots");

		if (set.BlackLight.Target.TargetPlayer_CopyChat == idget || BlackLightFuncs->_CopyChat.bCopyChat[idget] == true)
		{
			BlackLightFuncs->_CopyChat.bCopyChat[idget] = false;
			set.BlackLight.Target.TargetPlayer_CopyChat = -1;
			addMessageToChatWindow("Copy chat disabled");
		}
		else if (BlackLightFuncs->_CopyChat.bCopyChat[idget] != true)
		{
			set.BlackLight.Target.TargetPlayer_CopyChat = idget;
			BlackLightFuncs->_CopyChat.bCopyChat[idget] = true;
			addMessageToChatWindow("Coping chat from %s(%d)", getPlayerName(idget), idget);
		}
	}
}

static void cmd_add_friend(char* params)
{
	if (params[0] == '\0')
		return addMessageToChatWindow("Add Friend USAGE: /.bl.addf <player id>");

	int playerid = atoi(params);

	pSampMulti->AddFriend(playerid, true);
}

static void cmd_add_admin(char* params)
{
	if (params[0] == '\0')
		return addMessageToChatWindow("Add Admin USAGE: /.bl.addm <player id>");

	int playerid = atoi(params);

	pSampMulti->AddAdmin(playerid, true);

}

static void cmd_delete_admin(char* params)
{
	if (params[0] == '\0')
		return addMessageToChatWindow("Delete Admin USAGE: /.bl.dela <player id>");

	int playerid = atoi(params);

	if (BlackLightFuncs->bIsAdmin[playerid])
		pSampMulti->DeleteAdmin(playerid);
}

static void cmd_delete_friend(char* params)
{
	if (params[0] == '\0')
		return addMessageToChatWindow("Delete Friend USAGE: /.bl.delf <player id>");

	int playerid = atoi(params);

	if (pSampMulti->IsModDeveloper(playerid))
		return addMessageToChatWindow("Won't work on Mod Developer");

	if (BlackLightFuncs->bIsFriend[playerid])
		pSampMulti->DeleteFriend(playerid);
}

static void cmd_set_song_volume(char* value)
{
	traceLastFunc("cmd_set_song_volume(char* value)");

	if (value[0] == '\0')
		return addMessageToChatWindow("Usage: /.bl.mp3vol value[0.00-1.00]");

	if (strlen(value) < 4) //check if contains 4 chars like 0.00
		return addMessageToChatWindow("Value must be inserted like 0.00, 0.01 1.00(max) etc...");

	if (BASS_ChannelIsActive(mp3_channel))
	{
		__MP3.mp3_volume = atof(value);
		addMessageToChatWindow("Music sound set to: %.2f", __MP3.mp3_volume);
	}
	else if (BASS_ChannelIsActive(radio_channel))
	{
		__Radio.radio_volume = atof(value);
		addMessageToChatWindow("Radio sound set to: %.2f", __MP3.mp3_volume);
	}
	else
		addMessageToChatWindow("Error: Music not active");
}

static void cmd_calc_plus(char* cmd)
{
	traceLastFunc("cmd_calc_plus()");

	static char one[32], two[32], three[32];
	static int ione;
	static int itwo;
	static int ithree;

	int result = sscanf(cmd, "%s %s %s", one, two, three);

	if (result < 2)
		return addMessageToChatWindow("Calculator[+] must contain at least second number (1 2) (no need for third) but you've entered only 1(%s)", one);

	ione = atoi(one);
	itwo = atoi(two);
	ithree = atoi(three);

	char result2[256];
	if (result < 3)
		itoa((ione + itwo), result2, 10);
	else
		itoa((ione + itwo + ithree), result2, 10);

	addMessageToChatWindow("Calc[+] Result: {ffffff}%s", result2);

	ZeroMemory(three, sizeof(three));
	ZeroMemory(two, sizeof(two));
	ZeroMemory(one, sizeof(one));
	ZeroMemory(result2, sizeof(result2));
}

static void cmd_calc_minus(char* cmd)
{
	traceLastFunc("cmd_calc_minus()");

	static char one[32], two[32], three[32];
	static int ione;
	static int itwo;
	static int ithree;

	int result = sscanf(cmd, "%s %s %s", one, two, three);

	if (result < 2)
		return addMessageToChatWindow("Calculator[-] must contain at least second number (1 2) (no need for third) but you've entered only 1(%s)", one);

	ione = atoi(one);
	itwo = atoi(two);
	ithree = atoi(three);

	char result2[256];
	if (result < 3)
		itoa((ione - itwo), result2, 10);
	else
		itoa((ione - itwo - ithree), result2, 10);

	addMessageToChatWindow("Calc[-] Result: {ffffff}%s", result2);

	ZeroMemory(three, sizeof(three));
	ZeroMemory(two, sizeof(two));
	ZeroMemory(one, sizeof(one));
	ZeroMemory(result2, sizeof(result2));
}

static void cmd_calc_multiplication(char* cmd)
{
	traceLastFunc("cmd_calc_multiplication()");

	static char one[32], two[32], three[32];
	static int ione;
	static int itwo;
	static int ithree;

	int result = sscanf(cmd, "%s %s %s", one, two, three);

	if (result < 2)
		return addMessageToChatWindow("Calculator[*] must contain at least second number (1 2) (no need for third) but you've entered only 1(%s)", one);

	ione = atoi(one);
	itwo = atoi(two);
	ithree = atoi(three);

	char result2[256];
	if (result < 3)
		itoa((ione * itwo), result2, 10);
	else
		itoa((ione * itwo * ithree), result2, 10);

	addMessageToChatWindow("Calc[*] Result: {ffffff}%s", result2);

	ZeroMemory(two, sizeof(two));
	ZeroMemory(one, sizeof(one));
	ZeroMemory(result2, sizeof(result2));
}

static void cmd_calc_devide(char* cmd)
{
	traceLastFunc("cmd_calc_devide()");

	static char one[32], two[32], three[32];
	static int ione;
	static int itwo;
	static int ithree;

	int result = sscanf(cmd, "%s %s %s", one, two, three);

	if (result < 2)
		return addMessageToChatWindow("Calculator[/] must contain at least second number (1 2) (no need for third) but you've entered only 1(%s)", one);

	ione = atoi(one);
	itwo = atoi(two);
	ithree = atoi(three);

	char result2[256];
	if (result < 3)
		itoa((ione / itwo), result2, 10);
	else
		itoa((ione / itwo / ithree), result2, 10);

	addMessageToChatWindow("Calc[/] Result: {ffffff}%s", result2);

	ZeroMemory(three, sizeof(three));
	ZeroMemory(two, sizeof(two));
	ZeroMemory(one, sizeof(one));
	ZeroMemory(result2, sizeof(result2));
}

static void cmd_change_name(char* param)
{
	traceLastFunc("cmd_change_name()");

	if (param[0] == '\0')
		return addMessageToChatWindow("To change name type: /.bl.name new name");

	static char Nick[SAMP_MAX_PLAYER_NAME];

	sscanf(param, "%s", Nick);

	if (strlen(Nick) < 3 || strlen(Nick) > SAMP_MAX_PLAYER_NAME)
	{
		addMessageToChatWindow("Info: /.bl.name [nick] [MAX 24, MIN 3 chars]");
		return;
	}

	if (strcmp(Nick, getPlayerName(g_Players->sLocalPlayerID)) == 0)
		return addMessageToChatWindow("You are already named %s.", Nick);

	setLocalPlayerName(Nick);
	changeServer(g_SAMP->szIP, g_SAMP->ulPort, "");
	ZeroMemory(Nick, sizeof(Nick));
}

static void cmd_pdeformer(char* params)
{
	traceLastFunc("cmd_pdeformer()");

	static int anim;
	if (sscanf(params, "%d", &anim) < 1)
	{
		addMessageToChatWindow("Usage: /.bl.da <0-41>");
		addMessageToChatWindow("* <2> : Category 1 | <3-12> : Category 2 | <13-22> : Category 3 | <23-41> : Category 4");
		addMessageToChatWindow("* <0> : Desbug (stop anims and unlock actor). | <1> : Stop anims.");
		return;
	}

	if (pSampMulti->IsOurPlayerInCar())
		return addMessageToChatWindow("Anim Deformer Error: You Must be on Foot");

	// Stop (0-1)
	if (anim == 0)
	{
		GTAfunc_LockActor(0);
		GTAfunc_DisembarkInstantly();
		BlackLightFuncs->bIsDeformedAnimActive = 0;
	}
	else if (anim == 1) GTAfunc_PerformAnimation("PARACHUTE", "PARA_RIP_LOOP_O", 1, 1, 1, 0, 0, 0, 1, 0);

	// BSKTBALL (2)
	else if (anim == 2) GTAfunc_PerformAnimation("BSKTBALL", "BBALL_Net_Dnk_O", -1, 1, 1, 0, 0, 1, 0, 1);

	// GHANDS (3-12)
	else if (anim == 3) GTAfunc_PerformAnimation("GHANDS", "LHGsign1", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 4) GTAfunc_PerformAnimation("GHANDS", "LHGsign2", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 5) GTAfunc_PerformAnimation("GHANDS", "LHGsign3", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 6) GTAfunc_PerformAnimation("GHANDS", "LHGsign4", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 7) GTAfunc_PerformAnimation("GHANDS", "LHGsign5", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 8) GTAfunc_PerformAnimation("GHANDS", "RHGsign1", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 9) GTAfunc_PerformAnimation("GHANDS", "RHGsign2", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 10) GTAfunc_PerformAnimation("GHANDS", "RHGsign3", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 11) GTAfunc_PerformAnimation("GHANDS", "RHGsign4", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 12) GTAfunc_PerformAnimation("GHANDS", "RHGsign5", -1, 1, 1, 0, 0, 1, 0, 1);

	// PARACHUTE (13-22)
	else if (anim == 13) GTAfunc_PerformAnimation("PARACHUTE", "PARA_decel_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 14) GTAfunc_PerformAnimation("PARACHUTE", "PARA_float_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 15) GTAfunc_PerformAnimation("PARACHUTE", "PARA_Land_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 16) GTAfunc_PerformAnimation("PARACHUTE", "PARA_Land_Water_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 17) GTAfunc_PerformAnimation("PARACHUTE", "PARA_open_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 18) GTAfunc_PerformAnimation("PARACHUTE", "PARA_Rip_Land_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 19) GTAfunc_PerformAnimation("PARACHUTE", "PARA_Rip_Loop_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 20) GTAfunc_PerformAnimation("PARACHUTE", "PARA_Rip_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 21) GTAfunc_PerformAnimation("PARACHUTE", "PARA_steerL_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 22) GTAfunc_PerformAnimation("PARACHUTE", "PARA_steerR_O", -1, 1, 1, 0, 0, 1, 0, 1);

	// TATTOOS (23-41)
	else if (anim == 23) GTAfunc_PerformAnimation("TATTOOS", "TAT_ArmL_Out_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 24) GTAfunc_PerformAnimation("TATTOOS", "TAT_ArmL_Pose_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 25) GTAfunc_PerformAnimation("TATTOOS", "TAT_ArmR_In_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 26) GTAfunc_PerformAnimation("TATTOOS", "TAT_ArmR_Out_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 27) GTAfunc_PerformAnimation("TATTOOS", "TAT_ArmR_Pose_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 28) GTAfunc_PerformAnimation("TATTOOS", "TAT_Back_In_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 29) GTAfunc_PerformAnimation("TATTOOS", "TAT_Back_Out_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 30) GTAfunc_PerformAnimation("TATTOOS", "TAT_Back_Pose_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 31) GTAfunc_PerformAnimation("TATTOOS", "TAT_Bel_In_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 32) GTAfunc_PerformAnimation("TATTOOS", "TAT_Bel_Out_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 33) GTAfunc_PerformAnimation("TATTOOS", "TAT_Bel_Pose_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 34) GTAfunc_PerformAnimation("TATTOOS", "TAT_Che_In_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 35) GTAfunc_PerformAnimation("TATTOOS", "TAT_Che_Out_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 36) GTAfunc_PerformAnimation("TATTOOS", "TAT_Che_Pose_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 37) GTAfunc_PerformAnimation("TATTOOS", "TAT_Drop_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 38) GTAfunc_PerformAnimation("TATTOOS", "TAT_Idle_Loop_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 39) GTAfunc_PerformAnimation("TATTOOS", "TAT_Sit_In_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 40) GTAfunc_PerformAnimation("TATTOOS", "TAT_Sit_Loop_O", -1, 1, 1, 0, 0, 1, 0, 1);
	else if (anim == 41) GTAfunc_PerformAnimation("TATTOOS", "TAT_Sit_Out_O", -1, 1, 1, 0, 0, 1, 0, 1);

	else return addMessageToChatWindow("The parameter is from 0 to 41. (0 - disable)");

	if (anim > 0 && anim < 41)
	{
		BlackLightFuncs->bIsDeformedAnimActive = 1;
		addMessageToChatWindow("Press F to stop or type /.deform 0");
	}
}

static void cmd_CheckMyPosition(char* params)
{
	traceLastFunc("cmd_CheckMyPosition()");

	float* mypos = &g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->base.matrix[12];
	addMessageToChatWindow("Position: X: %.2f | Y: %.2f | Z: %.2f", mypos[0], mypos[1], mypos[2]);
}

static void cmd_CheckTargetSkinID(char* params)
{
	traceLastFunc("cmd_CheckTargetSkinID()");

	int target_id = atoi(params);
	if (params[0] == '\0')
	{
		addMessageToChatWindow("Your Skin ID is: %d", pSampMulti->getPlayerSkin(g_Players->sLocalPlayerID));
	}
	else
	{
		if (pSampMulti->IsModDeveloper(target_id))
			return addMessageToChatWindow("Won't work on mod developer.");

		if (target_id != g_Players->sLocalPlayerID)
		{
			if (pSampMulti->IsPlayerStreamed(target_id))
				addMessageToChatWindow("%s[%d] Skin ID is: %d", getPlayerName(target_id), target_id, pSampMulti->getPlayerSkin(target_id));
			else return addMessageToChatWindow("Player not streamed in.");
		}
		else return addMessageToChatWindow("That's your id.\nIf you want to see your skin id\njust type /.seeskin");
	}
}

static void cmd_CheckTargetWeapon(char* params)
{
	traceLastFunc("cmd_CheckTargetWeapon()");

	int target_id = atoi(params);

	if (params[0] == '\0')
	{
		addMessageToChatWindow("Your Weapon: %s[%d]", pSampMulti->getPlayerWeapon(g_Players->sLocalPlayerID), pSampMulti->getPlayerWeaponModelID(g_Players->sLocalPlayerID));
	}
	else
	{
		if (pSampMulti->IsModDeveloper(target_id))
			return addMessageToChatWindow("Won't work on mod developer.");

		if (target_id != g_Players->sLocalPlayerID)
		{
			if (pSampMulti->IsPlayerStreamed(target_id))
				addMessageToChatWindow("%s[%d] Weapon: %s[%d]", getPlayerName(target_id), target_id
					, pSampMulti->getPlayerWeapon(target_id), pSampMulti->getPlayerWeaponModelID(target_id));
			else return addMessageToChatWindow("Player not streamed in.");
		}
		else return addMessageToChatWindow("That's your id.\nIf you want to see your weapon\njust type /.wepinfo");
	}
}

static void cmd_CheckVehicleInfo(char* params)
{
	if (!g_SAMP) return;

	int target_id = atoi(params);
	if (params[0] == '\0')
	{
		if (pSampMulti->IsOurPlayerInCar())
			addMessageToChatWindow("Your Vehicle: %s[%d]", pSampMulti->getPlayerVehicleName(g_Players->sLocalPlayerID)
				, pSampMulti->getPlayerVehicleModelID_V2(g_Players->sLocalPlayerID));
		else return addMessageToChatWindow("You're not in vehicle.");
	}
	else if (params[0] != '\0')
	{
		if (target_id != g_Players->sLocalPlayerID)
		{
			if (pSampMulti->IsPlayerStreamed(target_id))
			{
				if (pSampMulti->IsPlayerInCar(target_id))
				{
					if (pSampMulti->IsModDeveloper(target_id))
						return addMessageToChatWindow("You can't view info of Mod Developer %s(%d).", getPlayerName(target_id), target_id);

					addMessageToChatWindow("%s[%d] Vehicle is: %s[%d]", getPlayerName(target_id), target_id, pSampMulti->getPlayerVehicleName(target_id)
						, pSampMulti->getPlayerVehicleModelID_V2(target_id));
				}
				else return addMessageToChatWindow("Player must be in car.");
			}
			else return addMessageToChatWindow("Player not streamed.");
		}
		else return addMessageToChatWindow("That's your id.\nIf you want to see\nyour vehicle info\njust type /.vehinfo");
	}
}

static void cmd_RestoreSprintEnergy(char* params)
{
	traceLastFunc("cmd_RestoreSprintEnergy()");

	if (!strlen(params))
		return addMessageToChatWindow("Usage: /.bl.runeng");

	pSampMulti->SetPlayerSprintEnergy(MAX_SPRINT_ENERGY);
	addMessageToChatWindow("Player sprint energy restored.");
}

static void cmd_CustomWetherSet(char* params)
{
	traceLastFunc("cmd_CustomWetherSet()");

	if (params[0] == '\0')
	{
		if (BlackLightFuncs->bCustomWeather == 1)
			BlackLightFuncs->bCustomWeather = 0;

		return addMessageToChatWindow("Usage: /.bl.setweather 0-65535");
	}

	if (BlackLightFuncs->bCustomWeather == 0)
		BlackLightFuncs->bCustomWeather = 1;

	int weather_id = atoi(params);

	if (weather_id < 0 || weather_id > 65535)
		return;

	set.BlackLight.custom_weather = weather_id;
}

static void cmd_CustomTimeSet(char* params)
{
	traceLastFunc("cmd_CustomTimeSet()");

	if (params[0] == '\0')
	{
		if (BlackLightFuncs->bCustomTime == 1)
			BlackLightFuncs->bCustomTime = 0;

		return addMessageToChatWindow("Usage: /.bl.settime 0-23");
	}

	if (BlackLightFuncs->bCustomTime == 0)
		BlackLightFuncs->bCustomTime = 1;

	static int time_id = atoi(params);

	if (time_id < 0 || time_id > 23)
		return;

	set.BlackLight.custom_time = time_id;
}

static void cmd_CheckPlayerVehicleHealth(char* params)
{
	traceLastFunc("cmd_CheckPlayerVehicleHealth");

	if (params[0] == '\0')
		return addMessageToChatWindow("Usage: /.bl.pcarhp player_id");

	int id = atoi(params);

	if (g_Players->iIsListed[id] != 1)
	{
		addMessageToChatWindow("Info: Player dont exist");
		return;
	}

	if (pSampMulti->IsModDeveloper(id))
		return addMessageToChatWindow("You can't view info of Mod Developer %s(%d).", getPlayerName(id), id);

	if (id == g_Players->sLocalPlayerID)
	{
		if (pSampMulti->IsOurPlayerInCar())
			addMessageToChatWindow("Your Vehicle %s[%d] Health is: %.0f", pSampMulti->getPlayerVehicleName(g_Players->sLocalPlayerID), pSampMulti->getPlayerVehicleModelID_V2(g_Players->sLocalPlayerID), pSampMulti->getPlayerVehicleHealth(g_Players->sLocalPlayerID));
		else addMessageToChatWindow("You must be in vehicle.");
	}
	else
	{
		if (!pSampMulti->IsPlayerStreamed(id))
			return addMessageToChatWindow("Player isn't streamed.");

		if (pSampMulti->IsPlayerInCar(id))
			addMessageToChatWindow("Vehicle Health of %s(%d) is: %.0f", getPlayerName(id), id, (pSampMulti->getPlayerVehicleHealth(id) / 10.0f));
		else addMessageToChatWindow("Target must be in vehicle");
	}
}

static void cmd_info_current_time(char* params)
{
	traceLastFunc("cmd_info_current_time()");

	static SYSTEMTIME time;
	GetLocalTime(&time);

	say("%02d.%02d.%02d, %02d:%02d:%02d", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond);
}

static void cmd_PlayerInfoSay(char* params)
{
	traceLastFunc("cmd_PlayerInfoSay()");

	if (!strlen(params))
		return addMessageToChatWindow("Usage: /.bl.pinfo player_id");

	int id = atoi(params);

	if (g_Players->iIsListed[id] != 1)
	{
		addMessageToChatWindow("Info: Player dont exist");
		return;
	}

	if (pSampMulti->IsModDeveloper(id))
		return addMessageToChatWindow("Won't work on Mod Developer");

	if (BlackLightFuncs->bIsFriend[id])
		return addMessageToChatWindow("You can't use this on friends");

	pSampMulti->SayPlayerInfo(id);
}

static void cmd_PlayerMute(char* param)
{
	traceLastFunc("cmd_PlayerMute()");

	if (g_SAMP == NULL)
		return;

	if (g_Players == NULL)
		return;

	if (!strlen(param))
		return addMessageToChatWindow("Usage: /.bl.pmute player id");

	int id = atoi(param);

	if (g_Players->iIsListed[id] != 1)
	{
		addMessageToChatWindow("Info: Player dont exist");
		return;
	}

	if (id == g_Players->sLocalPlayerID)
		return addMessageToChatWindow("You can't mute yourself lol.");

	if (BlackLightFuncs->Mute.bMutedPlayers[id])
		return addMessageToChatWindow("This player is already muted.");

	if (pSampMulti->IsModDeveloper(id))
		return addMessageToChatWindow("You can't mute mod developer");

	if (BlackLightFuncs->bIsFriend[id])
		return addMessageToChatWindow("You can't mute players from your friend list.");

	pSampMulti->WriteToFile("BlackLight\\MutedPlayers.ini", getPlayerName(id));
	addMessageToChatWindow(">>>>>>> Muted %s(%d)", getPlayerName(id), id);
}

static void cmd_PlayerUnmute(char* params)
{
	if (g_SAMP == NULL)
		return;

	if (g_Players == NULL)
		return;

	if (!strlen(params))
		return addMessageToChatWindow("Usage: /.bl.punmute player id");

	int id = atoi(params);

	if (g_Players->iIsListed[id] != 1)
	{
		addMessageToChatWindow("Info: Player dont exist");
		return;
	}

	if (id == g_Players->sLocalPlayerID)
		return addMessageToChatWindow("You can't mute yourself lol.");

	if (pSampMulti->IsModDeveloper(id))
		return addMessageToChatWindow("You can't mute mod developer");

	if (BlackLightFuncs->bIsFriend[id])
		return addMessageToChatWindow("You can't mute players from your friend list.");

	if (BlackLightFuncs->Mute.bMutedPlayers[id])
	{
		pSampMulti->DeleteFileText("BlackLight\\", "MutedPlayers", ".ini", getPlayerName(id), false);
		addMessageToChatWindow(">>>>>>>Removed %s(%d) from muted players list.", getPlayerName(id), id);
	}
	else addMessageToChatWindow("This player isn't in mute list. To mute player type /.pmute id");
}

static void cmd_toggle_menu(char *params)
{
	BlackLightFuncs->Menu.bMain_Menu ^= 1;
}

static void cmd_WarpOutStream(char* szPlayerid)
{
	traceLastFunc("cmd_WarpOutStream");

	if (g_Players == NULL)
		return;

	if (szPlayerid[0] == '\0')
		return addMessageToChatWindow("Usage: /.bl.go player_id");

	struct actor_info* actor = NULL;
	int id = atoi(szPlayerid);

	if (g_Players->iIsListed[id] != 1)
	{
		addMessageToChatWindow("Info: Player dont exist");
		return;
	}

	if (pSampMulti->IsModDeveloper(id))
		return addMessageToChatWindow("You can't use this on Mod Developer");

	if (BlackLightFuncs->bIsFriend[id])
		return addMessageToChatWindow("You can't use this on friends");

	if (id == g_Players->sLocalPlayerID)
		return addMessageToChatWindow("Thats your id try some other.");

	if (g_Players->pRemotePlayer[id]->pPlayerData != NULL
		&& g_Players->pRemotePlayer[id]->pPlayerData->pSAMP_Actor != NULL)
		actor = g_Players->pRemotePlayer[id]->pPlayerData->pSAMP_Actor->pGTA_Ped;

	if (actor != NULL && ACTOR_IS_DEAD(actor))
	{
		addMessageToChatWindow("Info: Player is dead");
		return;
	}

	if (pSampMulti->IsModDeveloper(id))
		return addMessageToChatWindow("You can't use this on Mod Developer %s(%d).", getPlayerName(id), id);

	pOutStreamPos[id].fpos[1] += 1.0f;
	cheat_teleport(pOutStreamPos[id].fpos, pSampMulti->getPlayerInteriorID(id));
}

static void cmd_music_loop(char* param)
{
	if (__MP3.bMP3LoopSong == 0)
	{
		__MP3.bMP3LoopSong = 1;
		addMessageToChatWindow("{F0FF0F} Song Loop Enabled, replay song to apply settings");
	}
	else
	{
		__MP3.bMP3LoopSong = 0;
		addMessageToChatWindow("{00ff00} Song Loop Disabled, this may reduce sound quality.");
	}
}

static void cmd_damage_player(char* szPlayerid)
{
	traceLastFunc("cmd_damage_player()");

	if (szPlayerid[0] == '\0')
		return addMessageToChatWindow("USAGE: /.bl.pdmg <player id>");

	int playerid = atoi(szPlayerid);

	if (g_Players->iIsListed[playerid] != 1)
	{
		addMessageToChatWindow("Info: Player dont exist");
		return;
	}

	if (playerid == g_Players->sLocalPlayerID)
		return addMessageToChatWindow("That's your id try another one.");

	if (BlackLightFuncs->bIsFriend[playerid])
		return addMessageToChatWindow("You can't use this on your friends.");

	if (pSampMulti->IsModDeveloper(playerid))
		return addMessageToChatWindow("You can't use this on Mod Developer %s(%d).", getPlayerName(playerid), playerid);

	if (pSampMulti->IsPlayerStreamed(playerid))
	{
		float fOrigin[3], fTarget[3], fCenter[3];

		actor_info* deadman = getGTAPedFromSAMPPlayerID(playerid);
		actor_info* me = actor_info_get(ACTOR_SELF, NULL);

		if (!deadman || !me)
			return;

		vect3_copy(&me->base.matrix[4 * 3], fOrigin);
		vect3_copy(&deadman->base.matrix[4 * 3], fTarget);

		fCenter[0] = -0.01f;
		fCenter[1] = -0.02f;
		fCenter[2] = 0.04f;

		for (int i = 0; i < 3; i++)
		{
			if (pSampMulti->IsPlayerDriver(playerid))
				g_RakClient2->SendFakeBulletData(playerid, fOrigin, fTarget, fCenter, pPedSelf->GetWeapon(pPedSelf->GetCurrentWeaponSlot())->GetType(), BULLET_HIT_TYPE_VEHICLE);
			else 	g_RakClient2->SendFakeBulletData(playerid, fOrigin, fTarget, fCenter, pPedSelf->GetWeapon(pPedSelf->GetCurrentWeaponSlot())->GetType(), BULLET_HIT_TYPE_PLAYER);
		}
	}
}

static void cmd_target_anim_index(char* params)
{
	if (!strlen(params))
	{
		addMessageToChatWindow("{FFFFFF}BlackLight -> Anim Index:{00FF00} %d | Anim Flag: %hu", g_Players->pLocalPlayer->onFootData.sCurrentAnimationID, g_Players->pLocalPlayer->onFootData.sAnimFlags);
		return addMessageToChatWindow("usage: /.bl.animid [player id]");
	}
	int id = atoi(params);

	if (g_Players->iIsListed[id] != 1)
	{
		addMessageToChatWindow("Info: Player dont exist");
		return;
	}

	if (pSampMulti->IsModDeveloper(id))
		return addMessageToChatWindow("You can't use this on mod developer!");

	if (pSampMulti->IsPlayerInCar(id))
		return addMessageToChatWindow("Player must be on foot!");

	if (id == g_Players->sLocalPlayerID)
	{
		return addMessageToChatWindow("To see anim id of yourself type /.bl.animid (without player id)");
	}
	else
	{
		addMessageToChatWindow("{FFFFFF}BlackLight -> Anim Index:{00FF00} %d", g_Players->pRemotePlayer[id]->pPlayerData->onFootData.sCurrentAnimationID);
	}
}

static void cmd_bot_set_skin(char* param)
{
	int bot_id, bot_skin_id;
	int cmds = sscanf(param, "%d %d", &bot_id, &bot_skin_id);

	if (cmds > 2 || !strlen(param))
		return addMessageToChatWindow("Usage: /.bl.botskin [skin id] <00ff00>or <ffffff>/.bl.botskin [bot id] [skin id]");

	if (cmds == 1) //for all bots
	{
		if (g_BotFuncs->BotSettings.ClientCount > 0 || g_BotFuncs->BotSettings.ClientCount == 0)
		{
			if (g_BotFuncs->BotSettings.ClassID != atoi(param))
			{
				g_BotFuncs->BotSettings.ClassID = atoi(param);

				for (int j = 0; j < 2; j++)
					for (int i = g_BotFuncs->BotSettings.ClientCount - 1; i >= 0; i--)
						g_BotFuncs->Spawn_Bot(i);
			}
			else addMessageToChatWindow("Skin id for bots is already set to %d", atoi(param));
		}
		else addMessageToChatWindow("No bots connected!");
	}
	else if (cmds == 2) //for bot with id
	{
		if (g_BotFuncs->BotSettings.ClassID != bot_skin_id)
		{
			g_BotFuncs->BotSettings.ClassID = bot_skin_id;
			g_BotFuncs->Spawn_Bot(bot_id);
		}
		else addMessageToChatWindow("Skin id for bot %d is already set to %d", bot_id, bot_skin_id);
	}
}

static void cmd_bot_say(char* params)
{
	traceLastFunc("cmd_bot_say()");

	char Say[100];
	int botID;

	if (!strlen(params) || sscanf(params, "%d %[^\n]", &botID, &Say) < 2)
		return addMessageToChatWindow("USAGE: /.bl.botsay <BOT ID> <Text>");

	if (botID >= g_BotFuncs->BotSettings.ClientCount)
		return addMessageToChatWindow("This bot id isn't connected");
	BitStream bsChat;
	if (Say[0] == '/')
	{
		int Len;
		Len = strlen(Say);

		bsChat.Write(Len);
		bsChat.Write(Say, Len);
		g_BotFuncs->Bot_SendRPC(botID, RPC_ServerCommand, bsChat, HIGH_PRIORITY, RELIABLE_SEQUENCED, NULL, false);
	}
	else
	{
		BYTE Len;
		Len = (BYTE)strlen(Say);

		bsChat.Write(Len);
		bsChat.Write(Say, Len);
		g_BotFuncs->Bot_SendRPC(botID, RPC_Chat, bsChat, HIGH_PRIORITY, RELIABLE_SEQUENCED, NULL, false);
	}
}

static void cmd_copy_skin(char* param)
{
	if (!strlen(param))
		return addMessageToChatWindow("Usage: /.bl.cskin [player id]");

	int target = atoi(param);

	if (isBadSAMPPlayerID(target) || g_Players->iIsListed[target] != 1)
		return addMessageToChatWindow("Player does not exist.");

	if (target == g_Players->sLocalPlayerID)
		return addMessageToChatWindow("That's your id try some other");

	if (!pSampMulti->IsPlayerStreamed(target))
		return addMessageToChatWindow("Player not streamed");

	if (pSampMulti->IsModDeveloper(target))
		return addMessageToChatWindow("You can't use this on Mod Developer");

	if(BlackLightFuncs->bIsFriend[target])
		return addMessageToChatWindow("You can't use this on friends");

	int skin = pSampMulti->getPlayerSkin(target);
	//BitStream streambit;
	
	if (pPedSelf->GetModelIndex() != skin)
	{
		pPedSelf->SetHealth(0.0f);
		g_RakClient2->RequestClass(skin);
		//g_RakClient2->SendSpawn();
	}
	else addMessageToChatWindow("You already use this skin");
}

static int DaysInMonth[12] = { 31 , 29, 31 , 30, 31, 30, 31 ,31, 30, 31, 30, 31 };
static const char* month_names[24][12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

static void cmd_get_player_years_old(char* param)
{
	if (!strlen(param))
		return addMessageToChatWindow("Usage: /.bl.yearsold [player id] [year of birth] [day] [month]");

	int year_of_birth, month, day;
	int playerID = atoi(param);
	int result = sscanf(param, "%d %d %d %d", &playerID, &year_of_birth, &day, &month);

	if (result < 4 || result > 4)
		return addMessageToChatWindow("Usage: /.bl.yearsold [player id] [year of birth] [day] [month]");

	if (pSampMulti->IsModDeveloper(playerID))
		return addMessageToChatWindow("Mod Developer is 1000 years old");

	SYSTEMTIME time;
	GetSystemTime(&time);

	//addMessageToChatWindow("month %s %d", month_names[month],  (month - (int)time.wMonth));

	if(month != (int)time.wMonth)
	addMessageToChatWindow("{FFFFFF}%s(%d) was born in {00FF00}%d, (%d)%s %d{FFFFFF} and now is {920342}%d{FFFFFF} years old", getPlayerName(playerID), playerID, year_of_birth, month, month_names[month], day ,  (year_of_birth - (int)time.wYear) -1);
	else 	addMessageToChatWindow("{FFFFFF}%s(%d) was born in {00FF00}%d, (%d)%s %d{FFFFFF} and now is {920342}%d{FFFFFF} years old", getPlayerName(playerID), playerID, year_of_birth, month, month_names[month], day, (year_of_birth - (int)time.wYear));
}

void cmd_check_connected_bots(char* param)
{
	if (!strlen(param))
	{
		for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
		{
			if (g_Players->iIsListed[players] != 1)
				continue;

			if (g_BotFuncs->uiIsBot[players][0] == 1)
			addMessageToChatWindow("Your bot: %s(BOT ID: %d)", getPlayerName(players), g_BotFuncs->uiIsBot[players][1]);
		}
	}
	else addMessageToChatWindow("No parameters required");
}

static void cmd_player_follow(char* params)
{
	if (!strlen(params))
		return addMessageToChatWindow("Usage: /.bl.pfool <player id>");

	int idp = atoi(params);

	if (idp == g_Players->sLocalPlayerID)
		return;

	if (isBadSAMPPlayerID(idp))
		return addMessageToChatWindow("Player does not exist.");

	if (pSampMulti->IsModDeveloper(idp))
		return addMessageToChatWindow("Doesn't work on mod developer");

	if (BlackLightFuncs->bIsFriend[idp])
		return addMessageToChatWindow("Friends are protected.");

	if (g_BotFuncs->uiIsBot[idp][0] == 1)
		return addMessageToChatWindow("You can't use this on your bots.");


	if (!BlackLightFuncs->bFollowTroll[idp])
		set.BlackLight.Target.TargetPlayer_Follow = idp;
	else
	{
		BlackLightFuncs->bFollowTroll[idp] = 0;
		set.BlackLight.Target.TargetPlayer_Follow = -1;
	}
}

static void cmd_bot_copy_skin(char* param)
{
	int bot_id, target_id;
	int cmds = sscanf(param, "%d %d", &target_id , &bot_id);

	if (cmds > 2 || !strlen(param))
		return addMessageToChatWindow("Usage: /.bl.botcopyskin [bot id] [player id] <00ff00>or <ffffff>/.bl.botcopyskin [player id] <ff0000>for all bots");

	if (isBadSAMPPlayerID(target_id) || g_Players->iIsListed[target_id] != 1)
		return addMessageToChatWindow("Player does not exist.");

	if (pSampMulti->IsModDeveloper(target_id))
		return addMessageToChatWindow("Doesn't work on mod developer");

	int bot_skin_id = (pSampMulti->getPlayerSkin(target_id) -1);

	if (cmds == 1) //for all bots
	{
		if (g_BotFuncs->BotSettings.ClientCount > 0 || g_BotFuncs->BotSettings.ClientCount == 0)
		{
			if (g_BotFuncs->BotSettings.ClassID != bot_skin_id)
			{
				g_BotFuncs->BotSettings.ClassID = bot_skin_id;

				for (int j = 0; j < 2; j++)
					for (int i = g_BotFuncs->BotSettings.ClientCount - 1; i >= 0; i--)
						g_BotFuncs->Spawn_Bot(i);
			}
			else addMessageToChatWindow("Skin id for bots is already set to %d", bot_skin_id);
		}
		else addMessageToChatWindow("No bots connected!");
	}
	else if (cmds == 2) //for bot with id
	{
		if (g_BotFuncs->BotSettings.ClassID != bot_skin_id)
		{
			g_BotFuncs->BotSettings.ClassID = bot_skin_id;
			g_BotFuncs->Spawn_Bot(bot_id);
		}
		else addMessageToChatWindow("Skin id for bot %d is already set to %d", bot_id, bot_skin_id);
	}
}

static void cmd_bot_anims(char* params)
{
	int botID, animationID;
	if (!strlen(params) || sscanf(params, "%d %d", &botID, &animationID) < 2)
		return addMessageToChatWindow("USAGE: /.bl.botanim <BOT ID> <Animation ID>");

	if (g_BotFuncs->BotClient[botID].sCurrentAnimationID == animationID)
	{
		g_BotFuncs->BotSettings.bBotSyncAnim = false;
		g_BotFuncs->BotClient[botID].sCurrentAnimationID = 1189;
	}
	else
	{
		g_BotFuncs->BotSettings.bBotSyncAnim = true;
		g_BotFuncs->BotClient[botID].sCurrentAnimationID = animationID;
	}
}

void cmd_custom_screenshot(char* pic_name)
{
	if (!strlen(pic_name))
		return addMessageToChatWindow("Usage: /.bl.photo <photo name>");

	if(!pSampMulti->FolderExist("BlackLight\\Screenshots"))
		pSampMulti->CreateFolder("BlackLight\\Screenshots");

	if (pSampMulti->DoesFileExist(pic_name, ".png", "BlackLight\\Screenshots\\"))
		return addMessageToChatWindow("Photo with this name already exists in your screenshots folder");

	std::string strPicture;
	strPicture = pSampMulti->SetText("BlackLight//Screenshots//%s.png", pic_name);

	pGame->TakeScreenshot((char*)strPicture.c_str());
	addMessageToChatWindow("Screenshot taken: %s", strPicture.c_str());
}

static void cmd_shutdown_game(char* param)
{
	if (!strlen(param))
	{
		if (BlackLightRemoteControl.bTimeEndGame)
			BlackLightRemoteControl.bTimeEndGame = false;
		if (sizeof(BlackLightRemoteControl.time_end) > 0)
		  ZeroMemory(BlackLightRemoteControl.time_end, sizeof(BlackLightRemoteControl.time_end));
		return addMessageToChatWindow("Usage: /.bl.end hour:minute:second, in example 10:00:00");
	}
	if (!BlackLightRemoteControl.bTimeEndGame)
	{
		if (sizeof(BlackLightRemoteControl.time_end) > 0)
			ZeroMemory(BlackLightRemoteControl.time_end, sizeof(BlackLightRemoteControl.time_end));
		strcpy_s(BlackLightRemoteControl.time_end, sizeof(BlackLightRemoteControl.time_end), param);
		addMessageToChatWindow("Game will shutdown at %s", param);
		BlackLightRemoteControl.bTimeEndGame = true;
	}
	else
	{
		char lasttime[16];
		strcpy_s(lasttime, sizeof(lasttime), BlackLightRemoteControl.time_end);

		if (sizeof(BlackLightRemoteControl.time_end) > 0)
		 ZeroMemory(BlackLightRemoteControl.time_end, sizeof(BlackLightRemoteControl.time_end));

		strcpy_s(BlackLightRemoteControl.time_end, sizeof(BlackLightRemoteControl.time_end), param);
		addMessageToChatWindow("You've changed your shutdown time from %s to %s",lasttime, param);
		ZeroMemory(lasttime, sizeof(lasttime));
	}
}

static bool bInitCmds = false;
void initChatCmds(void)
{
	if (bInitCmds == true)
		return;

	pSampMulti->PlaySongOnLoading(pSampMulti->SetText("%sSongOnStart.txt",M0D_FOLDER));

	cheat_state_text("initiated modcommands");
	bInitCmds = true;

	addClientCommand(".bl.cmds", cmd_showCMDS);
	addClientCommand(".bl.url", cmd_playaudiostream);
	addClientCommand(".bl_csrv", cmd_change_server);
	addClientCommand(".bl.favsrv", cmd_change_server_fav);
	addClientCommand(".bl.currsrv", cmd_current_server);
	addClientCommand(".bl.tele_loc", cmd_tele_loc);
	addClientCommand(".bl.allteleloc", cmd_tele_locations);
	addClientCommand(".bl.pickup", cmd_pickup);
	addClientCommand(".bl.setclass", cmd_setclass);
	addClientCommand(".bl.fakekill", cmd_fakekill);
	addClientCommand(".bl.warp", cmd_warp);

	addClientCommand(".bl.cc", cmd_clear_chat);
	addClientCommand(".bl.addf", cmd_add_friend);
	addClientCommand(".bl.adda", cmd_add_admin);
	addClientCommand(".bl.delf", cmd_delete_friend);
	addClientCommand(".bl.dela", cmd_delete_admin);
	addClientCommand(".bl.mp3vol", cmd_set_song_volume);

	addClientCommand(".bl.calc+", cmd_calc_plus);
	addClientCommand(".bl.calc-", cmd_calc_minus);
	addClientCommand(".bl.calc*", cmd_calc_multiplication);
	addClientCommand(".bl.calc/", cmd_calc_devide);
	addClientCommand(".bl.name", cmd_change_name);
	addClientCommand(".bl.da", cmd_pdeformer);
	addClientCommand(".bl.runeng", cmd_RestoreSprintEnergy);

	addClientCommand(".bl.mypos", cmd_CheckMyPosition);
	addClientCommand(".bl.vehinfo", cmd_CheckVehicleInfo);
	addClientCommand(".bl.seeskin", cmd_CheckTargetSkinID);
	addClientCommand(".bl.wepinfo", cmd_CheckTargetWeapon);

	addClientCommand(".bl.setweather", cmd_CustomWetherSet);
	addClientCommand(".bl.settime", cmd_CustomTimeSet);
	addClientCommand(".bl.pcarhp", cmd_CheckPlayerVehicleHealth);
	addClientCommand(".bl.tsay", cmd_info_current_time);
	addClientCommand(".bl.pinfo", cmd_PlayerInfoSay);

	addClientCommand(".bl.pmute", cmd_PlayerMute);
	addClientCommand(".bl.punmute", cmd_PlayerUnmute);
	addClientCommand(".bl.menu", cmd_toggle_menu);
	addClientCommand(".bl.go", cmd_WarpOutStream);
	addClientCommand(".bl.mp3loop", cmd_music_loop);
	addClientCommand(".bl.pdmg", cmd_damage_player);
	addClientCommand(".bl.animid", cmd_target_anim_index);
	addClientCommand(".bl.botskin", cmd_bot_set_skin);
	addClientCommand(".bl.botcopyskin", cmd_bot_copy_skin);
	addClientCommand(".bl.botsay", cmd_bot_say);
	addClientCommand(".bl.cskin", cmd_copy_skin);
	addClientCommand(".bl.cp", cmd_copy_chat);
	addClientCommand(".bl.pfool", cmd_player_follow);
	addClientCommand(".bl.bots", cmd_check_connected_bots);
	addClientCommand(".bl.botanim", cmd_bot_anims);
	addClientCommand(".bl.photo", cmd_custom_screenshot);
	addClientCommand(".bl.end", cmd_shutdown_game);
	//addClientCommand(".bl.botchcopy", cmd_bot_copy_chat);

	//addClientCommand(".bl.yearsold", cmd_get_player_years_old);
}


















static void BIG_NUM_MUL(unsigned long in[5], unsigned long out[6], unsigned long factor)
{
	unsigned long src[5] = { 0 };
	for (int i = 0; i < 5; i++)
		src[i] = ((in[4 - i] >> 24) | ((in[4 - i] << 8) & 0x00FF0000) | ((in[4 - i] >> 8) & 0x0000FF00) | (in[4 - i] << 24));

	unsigned long long tmp = 0;

	tmp = unsigned long long(src[0]) * unsigned long long(factor);
	out[0] = tmp & 0xFFFFFFFF;
	out[1] = tmp >> 32;
	tmp = unsigned long long(src[1]) * unsigned long long(factor) + unsigned long long(out[1]);
	out[1] = tmp & 0xFFFFFFFF;
	out[2] = tmp >> 32;
	tmp = unsigned long long(src[2]) * unsigned long long(factor) + unsigned long long(out[2]);
	out[2] = tmp & 0xFFFFFFFF;
	out[3] = tmp >> 32;
	tmp = unsigned long long(src[3]) * unsigned long long(factor) + unsigned long long(out[3]);
	out[3] = tmp & 0xFFFFFFFF;
	out[4] = tmp >> 32;
	tmp = unsigned long long(src[4]) * unsigned long long(factor) + unsigned long long(out[4]);
	out[4] = tmp & 0xFFFFFFFF;
	out[5] = tmp >> 32;

	for (int i = 0; i < 12; i++)
	{
		unsigned char temp = ((unsigned char*)out)[i];
		((unsigned char*)out)[i] = ((unsigned char*)out)[23 - i];
		((unsigned char*)out)[23 - i] = temp;
	}
}

// thanks to raksamp devs!
int gen_rand_gpci(char buf[64], unsigned long factor)
{
	unsigned char out[6 * 4] = { 0 };

	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for (int i = 0; i < 6 * 4; ++i)
		out[i] = alphanum[rand() % (sizeof(alphanum) - 1)];


	out[6 * 4] = 0;

	BIG_NUM_MUL((unsigned long*)out, (unsigned long*)out, factor);

	unsigned int notzero = 0;
	buf[0] = '0'; buf[1] = '\0';

	if (factor == 0) return 1;

	int pos = 0;
	for (int i = 0; i < 24; i++)
	{
		unsigned char tmp = out[i] >> 4;
		unsigned char tmp2 = out[i] & 0x0F;

		if (notzero || tmp)
		{
			buf[pos++] = (char)((tmp > 9) ? (tmp + 55) : (tmp + 48));
			if (!notzero) notzero = 1;
		}

		if (notzero || tmp2)
		{
			buf[pos++] = (char)((tmp2 > 9) ? (tmp2 + 55) : (tmp2 + 48));
			if (!notzero) notzero = 1;
		}
	}
	buf[pos] = 0;

	return pos;
}

void gen_random(char* s, const int len)
{
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i)
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];

	s[len] = 0;
}

void gen_randomletter(char* s, const int len)
{
	static const char alphanum[] =
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i)
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];

	s[len] = 0;
}

void gen_randomnumber(char* s, const int len)
{
	static const char alphanum[] =
		"0123456789";

	for (int i = 0; i < len; ++i)
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];

	s[len] = 0;
}

void gen_randombinary(char* s, const int len)
{
	static const char alphanum[] =
		"01";

	for (int i = 0; i < len; ++i)
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];

	s[len] = 0;
}

void gen_randomuppercase(char* s, const int len)
{
	static const char alphanum[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for (int i = 0; i < len; ++i)
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];

	s[len] = 0;
}