#include "main.h"
#include <DirectXMath.h>
//_=Gigant=_ was here 

ImVec4 custom_color_car = ImVec4(0.13f, 0.41f, 0.71f, 1.00f)
, custom_color_rb_sky = ImVec4(0.13f, 0.41f, 0.71f, 1.00f)
, custom_color_radar = ImVec4(0.13f, 0.41f, 0.71f, 1.00f)
, custom_color_nicknames = ImVec4(0.13f, 0.41f, 0.71f, 1.00f)
, custom_color_hud = ImVec4(0.13f, 0.41f, 0.71f, 1.00f)
, custom_color_killList = ImVec4(0.13f, 0.41f, 0.71f, 1.00f)
, custom_color_carLights = ImVec4(0.13f, 0.41f, 0.71f, 1.00f)
, custom_color_crosshair = ImVec4(0.13f, 0.41f, 0.71f, 1.00f)
, custom_color_chat = ImVec4(0.13f, 0.41f, 0.71f, 1.00f);

ImVec4 CC_EXTRAS_PlayersTracers, CC_EXTRAS_PlayersBones,
CC_EXTRAS_PlayersBox, CC_EXTRAS_VehicleTracers;

class CNewMods* pBlackLightMods;

void StartUpFuncs(void)
{
	//memset(&BlackLightFuncs[0], 0, sizeof(stNewFunctions)); cant zero this for now

	__MP3.mp3_volume = set.mp3_volume;
	__MP3.bMP3LoopSong = set.loop_mp3_song;
	__Radio.radio_volume = set.radio_volume;

	set.flysurfspeed = 0.5f;
	set.surfspeed = 0.5f;
	set.health_type = 2; //goc
	set.speedkick_speed = 15;
	set.upPower = 55.0f;
	set.rampower = 10;

	BlackLightFuncs->bAntiCarJack = true;
	BlackLightFuncs->bCarRammer = false;
	BlackLightFuncs->bVehicleWheelAim = false;
//	BlackLightRemoteControl.bIsBlackLightUser[g_Players->sLocalPlayerID] = true;
	//BlackLightRemoteControl.user_id = g_Players->sLocalPlayerID;
	BlackLightFuncs->bUseNewScoreboard = set.new_scoreboard;
    BlackLightFuncs->bShowKillCounterInsideHud = set.kill_counter;
	BlackLightFuncs->bDeathLogChat = set.kill_list_chat;
	BlackLightFuncs->bAutoReconnect = set.auto_reconnect;
	BlackLightFuncs->bPulseButtonsBorders = set.buttons_opacity_pulse;
	BlackLightFuncs->bActorNoFall = set.no_fall;
	BlackLightFuncs->bNoStun = set.no_stun;
	BlackLightFuncs->bPlayerJoiner = set.player_joiner;
	BlackLightFuncs->UIF.Islands.bCustomObjectKeyControls = set.uif_islands_keys;
	BlackLightFuncs->bCustomSprintEnergy = set.custom_sprint_energy;
	BlackLightFuncs->bMutedPlayerChatting = set.muted_player_notify_chat;
	BlackLightFuncs->bSlowmotion = set.slowmotion_enable;
	BlackLightFuncs->bSlowmotionViaKey = set.slowmotion_via_key_enable;
	BlackLightFuncs->bCameraOverview = set.camera_overview_enable;
	BlackLightFuncs->bVehicleDisableWheels = set.disable_car_wheels;
	BlackLightFuncs->bVehicleDisableFrame = set.disable_car_frame;
	BlackLightFuncs->bMouseDrive = set.mouse_drive_enable;

	BlackLightFuncs->bGodModMain = false;
	BlackLightFuncs->bCustomRainbowColorSet = false;
	BlackLightFuncs->bCustomColor_ESP = false;
	BlackLightFuncs->bFakeAfk = false;
	BlackLightFuncs->bSmartInvis = false;
	BlackLightFuncs->bVehicleFreeze = false;
	BlackLightFuncs->bPickVehicle = false;
	BlackLightFuncs->bComponentFlood = false;
	BlackLightFuncs->bWheelsStatusPulse = false;
	BlackLightFuncs->bInvertedBackLinear = false;
	BlackLightFuncs->bInvertedFrontLinear = false;
	BlackLightFuncs->bFakeJetpack = false;
	BlackLightFuncs->bRPCInterior = false;
	BlackLightFuncs->bPTPAntiEndRoundFreeze = false;
	BlackLightFuncs->bPizdarvankaPlayer = false;
	BlackLightFuncs->bPizdarvankaVehicle = false;
	BlackLightFuncs->bArabDrift = false;
	BlackLightFuncs->bTornadoDriving = false;
	BlackLightFuncs->bTornadoDrill = false;
	BlackLightFuncs->bPlayerDrillWalk = false;
	BlackLightFuncs->bPlayerCircleWalk = false;
	BlackLightFuncs->bUpDownDrive = false;
	BlackLightFuncs->bWheelDrive = false;
	BlackLightFuncs->bBikeWheelie = false;
	BlackLightFuncs->bVehiclesFugga = false;
	BlackLightFuncs->bPlayersElevator = false;
	BlackLightFuncs->bSurfer = false;
	BlackLightFuncs->bWalkOnWater = false;

	BlackLightFuncs->bFrontnBackFlip = true;
	BlackLightFuncs->bPlayerHop = false;
	BlackLightFuncs->bRapidFire = false;
	BlackLightFuncs->bDroneMode = false;
	BlackLightFuncs->bFastCrosshair = false;
	BlackLightFuncs->bQuickWarp = true;
	BlackLightFuncs->bHealthPulsator = false;
	BlackLightFuncs->bArmorPulsator = false;
	BlackLightFuncs->bWeaponsPulsator = false;
	BlackLightFuncs->bWheelsPulsator= false;
	BlackLightFuncs->bVehicleFakeFire = false;
	BlackLightFuncs->bFakeBulletsFlood = false;
	BlackLightFuncs->bFakeKillFlood = false;
	BlackLightFuncs->bSkinChanger = false;
	BlackLightFuncs->bTrailerTracers = false;
	BlackLightFuncs->bVehicleWheelsVisibilityPulsator = false;
	BlackLightFuncs->bCarHardFlip = true;
	BlackLightFuncs->bVehicleQuickWarp = false;
	BlackLightFuncs->bFreezePlayers = false;
	BlackLightFuncs->bCustomRunAnim = false;

	BlackLightFuncs->bWheelsInfo = false;
	BlackLightFuncs->bCustomGravity = false;
	BlackLightFuncs->bCustomGameSpeed = false;

	BlackLightFuncs->bViewAllAdmins = false;
	BlackLightFuncs->bViewAllFriends = false;
	BlackLightFuncs->bUsePlayersColor = true; //friends & admins list

	BlackLightFuncs->bSpeedmeterBackground = false;

	BlackLightFuncs->bFriendTags = true;
	BlackLightFuncs->bAdminsTags = true;
	BlackLightFuncs->Menu.bRadioVolume = true;
	//BlackLightFuncs->bChangeGPCI = true;
	BlackLightFuncs->bPlayersMap = false;
	BlackLightFuncs->bWindowedMode = false;
	BlackLightFuncs->bGoCfaker = false;

	BlackLightFuncs->GtaHUD.bDisableMoney = false;
	BlackLightFuncs->GtaHUD.bDisableHealthBar = false;
	BlackLightFuncs->GtaHUD.bDisableArmorBar = false;
	BlackLightFuncs->GtaHUD.bDisableWeaponIcon = false;
	BlackLightFuncs->GtaHUD.bDisableBreathBar = false;
	BlackLightFuncs->GtaHUD.bDisableAmmo = false;
	BlackLightFuncs->GtaHUD.bDisableVitalStats = false;
	BlackLightFuncs->GtaHUD.bDisableClock = false;
	BlackLightFuncs->GtaHUD.bDisableRadar = false;
	BlackLightFuncs->GtaHUD.bDisabelAll = false;

	BlackLightFuncs->bSilentInvis = false;
	BlackLightFuncs->bWorldCollision = false;
	BlackLightFuncs->bNoCollisionSurf = false;
	BlackLightFuncs->bSurfer = false;
	BlackLightFuncs->bFlySurf = false;

	BlackLightFuncs->bPlayer3DBoxes = false;
	BlackLightFuncs->bWheelESP = false;

	syncronisations_settings->sync = stSync::sync_type::SYNC_TYPE_DEFAULT;
	::sprintf_s(szSync, sizeof(szSync), "SYNC_TYPE_DEFAULT");
	::sprintf_s(szMainFunc, sizeof(szMainFunc), "GoC: None");
	::sprintf_s(szFakerFunc, sizeof(szFakerFunc), "GoC Faker: OFF");
	GoC->set.ignore_afk.enabled = true;
	GoC->set.ignore_locked.enabled = true;
	GoC->set.ignore_my_passagers.enabled = true;
	GoC->set.ignore_driver.enabled = false;
	GoC->set.incar_only.enabled = true;

	for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
	{
		BlackLightFuncs->_CopyChat.bCopyChat[i] = false;
		BlackLightFuncs->bFuckTroll[i] = false;
		BlackLightFuncs->bStickTroll[i] = false;
		BlackLightFuncs->bSlapTroll[i] = false;
		BlackLightFuncs->bPizdarvankaTarget[i] = false;
		BlackLightFuncs->bTargetInfo[i] = false;
		BlackLightFuncs->bFollowTroll[i] = false;
		BlackLightFuncs->bFakeBullets[i] = false;
	}

	__Damager.bShowInChat = false;
	__Damager.bShowOnTarget = false;
	__Damager.bEnableBackground = true;

	GoC->faker_target = actor_find_nearest(ACTOR_ALIVE | ACTOR_NOT_SAME_VEHICLE);
}

void ImScoreboardCursor(bool bMenu)
{
	if (BlackLightFuncs->Menu.bMain_Menu || BlackLightFuncs->Menu.bImGuiDemoWindow)
		return;

	static int mouse = 2;

	if (bMenu)
	{
		if (GetKeyState(VK_SPACE) < 0)
			toggleSAMPCursor(0);
		else
		{
			toggleSAMPCursor(1);
			mouse = 0;
		}
	}
	else
	{
		if (mouse == 0)
		{
			toggleSAMPCursor(0);
			mouse++;
		}
	}
}

void MainMenuMouse(void)
{
	if (BlackLightFuncs->Menu.bImScoreboad || BlackLightFuncs->Menu.bImGuiDemoWindow)
		return;

	static int mouse_main = 2;

	if (BlackLightFuncs->Menu.bMain_Menu)
	{
		if(mouse_main != 0)
		mouse_main = 0;
		toggleSAMPCursor(1);
	}
	else
	{
		if (mouse_main == 0)
		{
			toggleSAMPCursor(0);
			mouse_main++;
		}
	}
}

void KeySystem(void)
{
	if (!g_SAMP || !pGameInterface)
		return;

	if (gta_menu_active()) return;

	if (IS_CHEAT_PANIC_ACTIVE) return;

	MainMenuMouse();

	if (KEYCOMBO_PRESSED(set.BlackLight.KeyPress.MenuKeys.key_main_menu))
		BlackLightFuncs->Menu.bMain_Menu ^= 1;

	if (KEYCOMBO_PRESSED(set.BlackLight.KeyPress.MenuKeys.key_new_hud))
		BlackLightFuncs->Menu.bImGuiHudMenu ^= 1;

	if (KEYCOMBO_PRESSED(set.BlackLight.key_world_collision))
		BlackLightFuncs->bWorldCollision ^= 1;

	if (KEYCOMBO_PRESSED(set.BlackLight.key_player_ghost))
		BlackLightFuncs->bPlayerGhost ^= 1;

	if (KEYCOMBO_PRESSED(set.BlackLight.key_vehicle_hologram))
		BlackLightFuncs->bVehicleNoCollision ^= 1;

	if (KEYCOMBO_PRESSED(set.key_surf))
		BlackLightFuncs->bPlayerSurf ^= true;

	if (KEYCOMBO_PRESSED(set.BlackLight.key_moveless_players))
		BlackLightFuncs->bFreezePlayers ^= true;

	//New Scoreboard
	static int iScoreboardToggle = 0;
	if (BlackLightFuncs->bUseNewScoreboard == true)
	{
		ImScoreboardCursor(BlackLightFuncs->Menu.bImScoreboad);
		if (iScoreboardToggle == 0)
		{
			iScoreboardToggle = 1;
			toggleOffScoreboard();
			sampPatchDisableScoreboardToggleOn(1);
		}
		if (GetKeyState(VK_TAB) == 1) BlackLightFuncs->Menu.bImScoreboad = 1;
		else BlackLightFuncs->Menu.bImScoreboad = 0;
	}
	else
	{
		if (iScoreboardToggle == 1)
		{
			iScoreboardToggle = 0;
			BlackLightFuncs->Menu.bImScoreboad = false;
			sampPatchDisableScoreboardToggleOn(0);
		}
	}
}

static uint16_t GetAimingBot(void)
{
	static uint16_t NearestCenterPlayerID = -1;
	float NearestDistance = -1;
	float DistPlayerScreen[2], fViewFinderPosition[2];
	for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
	{
		if (i == g_Players->sLocalPlayerID)
			continue;

		if (g_BotFuncs->uiIsBot[i][0] != 1)
			continue;

		actor_info* player = getGTAPedFromSAMPPlayerID(i);
		if (!player)
			continue;

		CPed* pPed = pGameInterface->GetPools()->GetPed((DWORD*)player);
		if (!pPed)
			continue;

		//CVector pos = *pPed->GetPosition();
		CVector pSpinePos;
		D3DXVECTOR3 player_pos, player_pos_on_screen;
		pPed->GetBonePosition(BONE_SPINE1, &pSpinePos);

		player_pos.x = CVecToD3DXVEC(pSpinePos).x;
		player_pos.y = CVecToD3DXVEC(pSpinePos).y;
		player_pos.z = CVecToD3DXVEC(pSpinePos).z;
		pSampMulti->CalcScreenCoors(&player_pos, &player_pos_on_screen);

		if (player_pos_on_screen.z < 1.f)
			continue;

		DistPlayerScreen[0] = abs((fViewFinderPosition[0]) - (player_pos_on_screen.x));
		DistPlayerScreen[1] = abs((fViewFinderPosition[1]) - (player_pos_on_screen.y));

		float CurrentDistance = sqrtf((DistPlayerScreen[0] * DistPlayerScreen[0]) + (DistPlayerScreen[1] * DistPlayerScreen[1]));

		if (NearestDistance == -1)
		{
			NearestDistance = CurrentDistance;
		}
		bool IsPlayerAfk = g_Players->pRemotePlayer[i]->pPlayerData->iAFKState == 2;

		if (CurrentDistance <= NearestDistance)
		{
			NearestDistance = CurrentDistance;
			NearestCenterPlayerID = i;
		}
	}
	return g_BotFuncs->uiIsBot[NearestCenterPlayerID][1];

}

void CNewMods::BlackLight_InitMods(void)
{
	this->BlackLight_RegularFuncs();
	this->SetSprintEnergy(BlackLightFuncs->bCustomSprintEnergy, set.BlackLight.fSprintEng);
	this->ActorNoFall(BlackLightFuncs->bActorNoFall);
	this->UIFIslands(BlackLightFuncs->UIF.Islands.bCustomObjectKeyControls);
	this->Slowmotion(BlackLightFuncs->bSlowmotion, BlackLightFuncs->bSlowmotionViaKey);
	this->CameraOverview(BlackLightFuncs->bCameraOverview, set.BlackLight.fCamOverview);
	this->VehicleDisableWheels(BlackLightFuncs->bVehicleDisableWheels);
	this->VehicleDisableFrame(BlackLightFuncs->bVehicleDisableFrame);
	this->FastRotation(BlackLightFuncs->bFastRotation, set.BlackLight.fFastRotation);
	this->CrazyRoter(BlackLightFuncs->bCrazyRoter);
	this->MouseDrive(BlackLightFuncs->bMouseDrive);
	this->NoSpread(BlackLightFuncs->bNoSpread);
	this->RapidFire(BlackLightFuncs->bRapidFire);
	this->NoReload(BlackLightFuncs->bNoReload);
	this->FastCrosshair(BlackLightFuncs->bFastCrosshair);
	this->AntiStun(BlackLightFuncs->bNoStunV2);
	this->AutoShoot(BlackLightFuncs->bAutoShoot);
	this->AutoShootWheel(BlackLightFuncs->bVehicleWheelAim); //new
	this->SampFastConnect(BlackLightFuncs->bSampFastConnect);
	this->DisableWaterWaves(BlackLightFuncs->bDisableWaterWaves);
	this->CustomTimeWeatherSet(BlackLightFuncs->bCustomTime, BlackLightFuncs->bCustomWeather);
	this->FastAnims();
	this->HudThread(); //defined in cheat.cpp
	this->RPCs();
	this->PlayerFuckTroll();
	this->Slapper();
	this->FollowTroll();
	this->TargetSendBullets();
	this->PlayerPulsators();
	this->MoveAllNearbyVehicles_MASS(BlackLightFuncs->bPickVehiclesMass);
	this->AutoScroll(BlackLightFuncs->bAutoScroll);
	this->AutoCBug(BlackLightFuncs->bAutoCBug);
	this->PickVehicle(BlackLightFuncs->bPickVehicle);
	this->ComponentFlood(BlackLightFuncs->bComponentFlood);
	this->WheelsStatusPulse(BlackLightFuncs->bWheelsStatusPulse);
	this->BlinkCarColors(BlackLightFuncs->bBlinkColors);
	this->WheelsPulsator(BlackLightFuncs->bWheelsPulsator);
	this->VehicleOnFire(BlackLightFuncs->bVehicleFakeFire);
    this->PizdarvankaTargetTroll();
	this->PizdarvankaPlayer(BlackLightFuncs->bPizdarvankaPlayer);
	this->PizdarvankaVehicle(BlackLightFuncs->bPizdarvankaVehicle);
	this->FakeKillFlood(BlackLightFuncs->bFakeKillFlood);
	this->FakeBulletsFlood(BlackLightFuncs->bFakeBulletsFlood);
	this->SkinChanger(BlackLightFuncs->bSkinChanger);
	this->VehicleWheelsVisibilityPulsator(BlackLightFuncs->bVehicleWheelsVisibilityPulsator);
	//this->CarHardFlip(BlackLightFuncs->bCarHardFlip);
	this->NoStun(BlackLightFuncs->bNoStun);
	this->FreezePlayers(BlackLightFuncs->bFreezePlayers);
	this->VehiclesFugga();
	this->AntiLoading();
	this->GameShutdownOnTime();
	this->Dronemode();
	this->BE_Surfer();
	this->BE_Flysurf();

	if (g_BotFuncs->BotSettings.bBotPick) BlackLightFuncs->iAimClientBot = GetAimingBot();
}

void CNewMods::BlackLight_InitESPFuncs(void)
{
	this->renderDriverInfo(BlackLightFuncs->bSeeDrivers);
	this->renderShowDoorsStatus(BlackLightFuncs->bCarsDoorsStatus);
	this->renderPlayersTracers(BlackLightFuncs->bPlayersTracers);
	this->renderPlayerBones(BlackLightFuncs->bPlayersBones);
	this->renderPlayerBox(BlackLightFuncs->bPlayerBox);
	this->renderPlayer3DBox(BlackLightFuncs->bPlayer3DBoxes);
	this->renderPlayers2DInfo(BlackLightFuncs->bPlayersInfo);
	this->renderVehicleInfo(BlackLightFuncs->bVehicleInfo);
	this->renderVehicleTracers(BlackLightFuncs->bVehicleTracers);
	this->renderHelicopterTracers(BlackLightFuncs->bHelicopterTracers);
	this->renderPolicePedsDetector(BlackLightFuncs->bPoliceDetectorPeds);
	this->renderPoliceVehiclesDetector(BlackLightFuncs->bVehiclesPoliceDetector);
	this->renderOutStreamPlayers(BlackLightFuncs->bOutStreamTags);
	this->renderBulletTracers(BlackLightFuncs->bBulletTracers);
	this->renderTrailerTracers(BlackLightFuncs->bTrailerTracers);
	this->renderWheelsInfo(BlackLightFuncs->bWheelsInfo);
	this->renderVehicleWheelESP(BlackLightFuncs->bWheelESP);

	//other
	this->AdminsAutoUpdate();
	this->FriendsAutoUpdate();
	this->MutedPlayerListUpdate();
}

void CNewMods::BlackLight_InitRainbowFuncs(void)
{
	this->RainbowChat(BlackLightFuncs->bRainbow_Chat);
	this->RainbowCrosshair(BlackLightFuncs->bRainbow_Crosshair);
	this->RainbowHud(BlackLightFuncs->bRainbow_Hud);
	this->RainbowRadar(BlackLightFuncs->bRainbow_Radar);
	this->RainbowSky(BlackLightFuncs->bRainbow_Sky);
	this->RainbowVehicle(BlackLightFuncs->bRainbow_Vehicle);
	this->RainbowCarLights(BlackLightFuncs->bRainbow_CarLights);
	this->RainbowDeathList(BlackLightFuncs->bRainbow_KillList);
	this->RainbowNicknames(BlackLightFuncs->bRainbow_Nicknames);
}

void CNewMods::BlackLight_RegularFuncs(void)
{
	traceLastFunc("BlackLight_RegularFuncs()");

	if (!pGameInterface)
		return;

	if (gta_menu_active())
		return;

	if (BlackLightFuncs->bCameraOverview)
	{
		if (KEY_PRESSED(KEY_V))
		{
			BlackLightFuncs->bCameraOverview = false;
			cheat_state_text("Camera Overview: {FF0000}OFF");
		}
	}

	if (BlackLightFuncs->bCustomGameSpeed)
	{
		if (BlackLightFuncs->bSlowmotion)
			return; 

	     cheat_state->game_speed = set.game_speedc;
	}

	if (IS_CHEAT_PANIC_ACTIVE)
	{
		if (BlackLightFuncs->bIsDeformedAnimActive == true)
			BlackLightFuncs->bIsDeformedAnimActive = 0;
		return;
	}
	if (BlackLightFuncs->bIsDeformedAnimActive == true)
	{
		if (KEY_PRESSED(KEY_F))
		{
			GTAfunc_LockActor(0); GTAfunc_DisembarkInstantly();
			addMessageToChatWindow("Deformed anim stopped.");
			BlackLightFuncs->bIsDeformedAnimActive = 0;
		}
	}

}

void CNewMods::SetSprintEnergy(bool bEnable, float set_sprint_eng)
{
	if (!bEnable)
		return;

	traceLastFunc("SetPlayerSprintEnergy()");

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE) return;

	if (!pPedSelf)
		return;

	if (pSampMulti->IsOurPlayerInCar())
		return;

	g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->pPlayerData->m_fSprintEnergy = set_sprint_eng;
}

void CNewMods::ActorNoFall(bool bEnable)
{
	if (!bEnable)
		return;
	if (!g_SAMP || !g_Players->pLocalPlayer)
		return;

	traceLastFunc("PlayerNoFall()");

	if (IS_CHEAT_PANIC_ACTIVE) return;

	if (pSampMulti->IsOurPlayerInCar())
		return;

	if (!pPedSelf)
		return;

	if (g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 0x469
		|| g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 0x4B8)
		GTAfunc_DisembarkInstantly();
}


void CNewMods::UIFIslands(bool bEnable)
{
	if (!bEnable)
		return;
	traceLastFunc("UIFIslands");

	if (gta_menu_active())
		return;

	if (!g_SAMP)
		return;

	if (IS_CHEAT_PANIC_ACTIVE) return;

	if (pSampMulti->IsOurPlayerInCar())
		return;

	if (KEY_PRESSED(KEY_O))
		say("/b");
	if (KEY_PRESSED(KEY_Q))
		say("/bcopy");
	if (KEY_PRESSED(KEY_E))
		say("/bmove");
	if (KEY_PRESSED(KEY_R))
		say("/brem");
}

void CNewMods::Slowmotion(bool bEnable, bool bEnableViaKey)
{
	if (!bEnable)
		return;

	traceLastFunc("Slowmotion()");

	if (IS_CHEAT_PANIC_ACTIVE) return;

	static bool slowmo = false;
	if (bEnable)
	{
		if (bEnableViaKey)
		{
			if (KEYCOMBO_DOWN(set.BlackLight.key_slowmo_on_key))
			{
				if (!slowmo)
				{
					slowmo = true;
				}
				cheat_state->game_speed = set.BlackLight.slowmo_detection;
			}
			else
			{
				if (slowmo)
				{
					slowmo = false;
					cheat_state->game_speed = 1.0f;
				}
			}

		}
		else
		{
			if (!slowmo)
			{
				slowmo = true;
			}
			cheat_state->game_speed = set.BlackLight.slowmo_detection;
		}
	}
	else
	{
		bEnableViaKey = false;

		if (slowmo)
		{
			slowmo = false;
			cheat_state->game_speed = 1.0f;
		}
	}
}

void CNewMods::CameraOverview(bool bEnable, float fCamOverview)
{
	if (!bEnable)
		return;

	traceLastFunc("CameraOverview");

	if (gta_menu_active())
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE) return;

	if (bEnable)
	{
		if (g_Players->pLocalPlayer->iIsActorAlive)
		{
			if (strcmp(pSampMulti->getPlayerWeapon(g_Players->sLocalPlayerID), "Fist") != 0 && KEY_DOWN(VK_RBUTTON))
				return;
			CCamera* pCamera = pGame->GetCamera();
			CCam* pCam = pCamera->GetCam(pCamera->GetActiveCam());

			if (pCam->GetFOV() != fCamOverview) {
				pCam->SetFOV(fCamOverview);
			}  pSampMulti->MemoryCopy((PVOID)0x522F6A, "\x90\x90\x90\x90\x90\x90", 6, 1, "\xD9\x9F\xB4\x00\x00\x00");

		}
	}
	else pSampMulti->MemoryCopy((PVOID)0x522F6A, "\xD9\x9F\xB4\x00\x00\x00", 6, 1, "\x90\x90\x90\x90\x90\x90");
}

void CNewMods::VehicleDisableWheels(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("VehicleDisableWheels()");

	if (!pGameInterface || !g_SAMP)
		return;

	if (IS_CHEAT_PANIC_ACTIVE) return;

	if (!pSampMulti->IsOurPlayerInCar())
		return;

	static int wheels = 0;
	if (bEnable)
	{
		if (wheels == 0) wheels = 1;
		g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->vehicle->wheel_size = 0.0f;
	}
	else
	{
		if (wheels == 1)
		{
			g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->vehicle->wheel_size = 1112;
			wheels = 0;
		}
	}
}

void CNewMods::VehicleDisableFrame(bool bEnable)
{
	traceLastFunc("VehicleDisableFrame()");

	if (!pGameInterface || !g_SAMP)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (!pSampMulti->IsOurPlayerInCar())
		return;

	static int ibody = 0;

	if (bEnable)
	{
		if (ibody == 0)
		{
			ibody = 1;
			*((bool*)0x0096914B) = 1;
			g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->vehicle->base.bDontCastShadowsOn = true;
		}
	}
	else
	{
		if (ibody == 1)
		{
			*((bool*)0x0096914B) = 0;
			g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->vehicle->base.bDontCastShadowsOn = false;
			ibody = 0;
		}
	}
}

void CNewMods::FastRotation(bool bEnable, float fRotationSpeed)
{
	if (!bEnable)
		return;

	traceLastFunc("FastRotation()");

	if (!pGameInterface || !g_SAMP)
		return;

	if (IS_CHEAT_PANIC_ACTIVE) return;

	if (!pPedSelf)
		return;

	//addMessageToChatWindow("%.1f", g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->fRotationSpeed);
	if (BlackLightFuncs->bPlayerFly || BlackLightFuncs->bAirbreakPlayer || BlackLightFuncs->bPlayerSurf)
		return;

	static int irot = 0;
	if (bEnable)
	{
		if (irot == 0) irot = 1;
		g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->fRotationSpeed = fRotationSpeed;
	}
	else
	{
		if (irot == 1)
		{
			g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->fRotationSpeed = DEFAULT_PED_ROTATION_SPEED;
			irot = 0;
		}
	}
}

void CNewMods::CrazyRoter(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("CrazyRoter()");

	if (!pGameInterface || IS_CHEAT_PANIC_ACTIVE || !pPedSelf)
		return;

	if (BlackLightFuncs->bPlayerFly || BlackLightFuncs->bAirbreakPlayer || BlackLightFuncs->bPlayerSurf)
		return;

	if (pSampMulti->IsOurPlayerInCar())
		return;

	static int imsg = 0;

	// Reset message display state if disabled
	if (!bEnable)
	{
		if (imsg != 0) imsg = 0;
		return;
	}

	// Show instruction message
	if (imsg == 0)
	{
		addMessageToChatWindow("Press keys 1, 2, or 3 to spin.");
		imsg = 1;
	}

	// Key 1: Spin constantly around the X-axis
	if (KEY_DOWN(0x02))
	{
		g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->spin[0] += 1.0f;  // Increase rotation speed
	}

	// Key 2: Spin constantly around the Y-axis
	if (KEY_DOWN(KEY_2))
	{
		g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->spin[1] += 1.0f;  // Increase rotation speed
	}

	// Key 3: Spin constantly around the Z-axis (vertical axis)
	if (KEY_DOWN(KEY_3))
	{
		g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->spin[2] += 1.0f;  // Increase rotation speed
	}

	// Stop instantly when keys are released
	if (!KEY_DOWN(KEY_1) && !KEY_DOWN(KEY_2) && !KEY_DOWN(KEY_3))
	{
		g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->spin[0] = 0.0f;  // Stop X-axis rotation
		g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->spin[1] = 0.0f;  // Stop Y-axis rotation
		g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->spin[2] = 0.0f;  // Stop Z-axis rotation
	}
}


void CNewMods::MouseDrive(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("MouseDrive()");

	if (IS_CHEAT_PANIC_ACTIVE) return;

	if (BlackLightFuncs->bAirbreakVehicle || BlackLightFuncs->bVehicleFly || BlackLightFuncs->bVehicleSpider)
		return;

	if (g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->state == ACTOR_STATE_DRIVING)
	{
		g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->fCurrentRotation = -pGame->GetCamera()->GetCameraRotation();
		g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->fTargetRotation = g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->fCurrentRotation;

		g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->vehicle->base.matrix[4] = sinf(-g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->fCurrentRotation);
		g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->vehicle->base.matrix[5] = cosf(-g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->fCurrentRotation);
	}
}

void CNewMods::NoSpread(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("NoSpread()");

	if (IS_CHEAT_PANIC_ACTIVE) return;

	if (!pPedSelf)
		return;

	static bool bSetted = false;
	if (bEnable)
	{
		if (!bSetted)
		{
			for (int addr = 0xC8C450; addr < 0xC8C8B0; addr += 0x70)
				*(DWORD*)addr = 0x40000000;
			bSetted = true;
		}
	}
	else
	{
		if (bSetted)
		{
			for (int addr = 0xC8C450; addr < 0xC8C8B0; addr += 0x70)
				*(DWORD*)addr = 0x3F800000;
			bSetted = false;
		}
	}
}

void CNewMods::SampFastConnect(bool bEnable)
{
	if (!bEnable || !g_SAMP)
		return;

	traceLastFunc("SampFastConnect()");

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (bEnable) {
		pSampMulti->MemoryCopy((void*)(g_dwSAMP_Addr + 0x2A09C6), "\x00\x00\x00\x00", 4, true, "\xB8\x0B\x00\x00");
	}
	else pSampMulti->MemoryCopy((void*)(g_dwSAMP_Addr + 0x2A09C6), "\xB8\x0B\x00\x00", 4, true, NULL);
}


void CNewMods::DisableWaterWaves(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("DisableWaterWaves()");

	if (gta_menu_active())
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE) return;

	static int cwaves = 1;
	if (bEnable)
	{
		if (cwaves != 0)
		{
			pGameInterface->GetWaterManager()->SetWaveLevel(0.0f);
			cwaves = 0;
		}
	}
	else
	{
		if (cwaves == 0)
		{
			pGameInterface->GetWaterManager()->SetWaveLevel(-1.0f);
			cwaves = 1;
		}
	}
}

void CNewMods::CustomTimeWeatherSet(bool bEnableCustomTime, bool bEnableCustomWeather)
{
	traceLastFunc("CustomWeatherTimeSet()");

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (bEnableCustomTime)
	{
		gta_time_hour_set(set.BlackLight.custom_time);
	}

	static int weather = 2;
	if (bEnableCustomWeather)
	{
		if(weather != 0) weather = 0;
			gta_weather_state_set(set.BlackLight.custom_weather);
	}
	else if (!bEnableCustomWeather)
	{
		if (weather == 0)
		{
			weather++;
			gta_weather_state_set(6); //https://open.mp/docs/scripting/resources/weatherid
		}
	}
}

void CNewMods::FastAnims(void)
{
	traceLastFunc("FastAnims()");

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (!g_SAMP)
		return;

	if (!pPedSelf)
		return;

	if (pSampMulti->IsOurPlayerInCar())
		return;

	struct actor_info* fsinfo = actor_info_get(ACTOR_SELF, ACTOR_ALIVE); //nvm

	if (fsinfo)
	{
		if (BlackLightFuncs->bFastDeagle)
		{
			ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(fsinfo), "python_crouchfire", set.BlackLight.fast_deagle);
			ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(fsinfo), "python_fire", set.BlackLight.fast_deagle);
			ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(fsinfo), "python_fire_poor", set.BlackLight.fast_deagle);
		}

		if (BlackLightFuncs->bFastSwim)
			ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(fsinfo), "SWIM_crawl", set.BlackLight.fast_swim_speed);

		if (BlackLightFuncs->bFastReload)
		{
			ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(fsinfo), "colt45_crouchreload", set.BlackLight.fast_reload_speed);
			ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(fsinfo), "colt45_reload", set.BlackLight.fast_reload_speed);
			ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(fsinfo), "sawnoff_reload", set.BlackLight.fast_reload_speed);
			ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(fsinfo), "buddy_crouchreload", set.BlackLight.fast_reload_speed);
			ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(fsinfo), "buddy_reload", set.BlackLight.fast_reload_speed);
		}

		if (BlackLightFuncs->bFastRun)
			ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(fsinfo), "run_player", set.BlackLight.fast_run_speed);

		if (BlackLightFuncs->bFastWalk)
			ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(fsinfo), "WALK_player", set.BlackLight.fast_walk_speed);

		if (BlackLightFuncs->bFastSprint)
			ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(fsinfo), "sprint_civi", set.BlackLight.fast_sprint_speed);

		if (BlackLightFuncs->bFasterRunArmed)
			ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(fsinfo), "run_armed", set.BlackLight.fast_run_armed_speed);
	}
}
void CNewMods::HudThread(void)
{
	traceLastFunc("HudThread()");

	if (!pGame || !pGameInterface)
		return;

	if (gta_menu_active())
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	static int hud_disable_all = 2;
	static int hud_disable_ammo = 2;
	static int hud_disable_healthbar = 2;
	static int hud_disable_armourbar = 2;
	static int hud_disable_breathbar = 2;
	static int hud_disable_money = 2;
	static int hud_disable_clock = 2;
	static int hud_disable_radar = 2;
	static int hud_disable_vitalstats = 2;
	static int hud_disable_wepicon = 2;

	if (BlackLightFuncs->GtaHUD.bDisableAmmo)
	{
		if(hud_disable_ammo != 0)
		hud_disable_ammo = 0;
		if(hud_disable_ammo == 0)
		pGame->GetHud()->DisableAmmo(BlackLightFuncs->GtaHUD.bDisableAmmo);
	}
	else
	{
		if (hud_disable_ammo == 0)
		{
			pGame->GetHud()->DisableAmmo(BlackLightFuncs->GtaHUD.bDisableAmmo);
			hud_disable_ammo++;
		}
	}

	if (BlackLightFuncs->GtaHUD.bDisableWeaponIcon == true)
	{
		if(hud_disable_wepicon != 0)
		hud_disable_wepicon = 0;
		if(hud_disable_wepicon == 0)
		pGame->GetHud()->DisableWeaponIcon(BlackLightFuncs->GtaHUD.bDisableWeaponIcon);
	}
	else
	{
		if (hud_disable_wepicon == 0)
		{
			pGame->GetHud()->DisableWeaponIcon(BlackLightFuncs->GtaHUD.bDisableWeaponIcon);
			hud_disable_wepicon++;
		}
	}

	if (BlackLightFuncs->GtaHUD.bDisableArmorBar)
	{
		if(hud_disable_armourbar != 0)
		hud_disable_armourbar = 0;

		if(hud_disable_armourbar == 0)
		pGame->GetHud()->DisableArmour(BlackLightFuncs->GtaHUD.bDisableArmorBar);
	}
	else
	{
		if (hud_disable_armourbar == 0)
		{
			pGame->GetHud()->DisableArmour(BlackLightFuncs->GtaHUD.bDisableArmorBar);
			hud_disable_armourbar++;
		}
	}

	if (BlackLightFuncs->GtaHUD.bDisableHealthBar)
	{
		if(hud_disable_healthbar != 0)
		hud_disable_healthbar = 0;
		if(hud_disable_healthbar == 0)
		pGame->GetHud()->DisableHealth(BlackLightFuncs->GtaHUD.bDisableHealthBar);
	}
	else
	{
		if (hud_disable_healthbar == 0)
		{
			pGame->GetHud()->DisableHealth(BlackLightFuncs->GtaHUD.bDisableHealthBar);
			hud_disable_healthbar++;
		}
	}

	if (BlackLightFuncs->GtaHUD.bDisableBreathBar)
	{
		if(hud_disable_breathbar != 0)
		hud_disable_breathbar = 0;
		if(hud_disable_breathbar == 0)
		pGame->GetHud()->DisableBreath(BlackLightFuncs->GtaHUD.bDisableBreathBar);
	}
	else
	{
		if (hud_disable_breathbar == 0)
		{
			pGame->GetHud()->DisableBreath(BlackLightFuncs->GtaHUD.bDisableBreathBar);
			hud_disable_breathbar++;
		}
	}

	if (BlackLightFuncs->GtaHUD.bDisableMoney)
	{
		if(hud_disable_money != 0)
		hud_disable_money = 0;
		if(hud_disable_money == 0)
		pGame->GetHud()->DisableMoney(BlackLightFuncs->GtaHUD.bDisableMoney);
	}
	else
	{
		if (hud_disable_money == 0)
		{
			pGame->GetHud()->DisableMoney(BlackLightFuncs->GtaHUD.bDisableMoney);
			hud_disable_money++;
		}
	}

	if (BlackLightFuncs->GtaHUD.bDisableRadar)
	{
		if(hud_disable_radar != 0)
		hud_disable_radar = 0;
		if(hud_disable_radar == 0)
		pGame->GetHud()->DisableRadar(BlackLightFuncs->GtaHUD.bDisableRadar);
	}
	else
	{
		if (hud_disable_radar == 0)
		{
			pGame->GetHud()->DisableRadar(BlackLightFuncs->GtaHUD.bDisableRadar);
			hud_disable_radar++;
		}
	}

	if (BlackLightFuncs->GtaHUD.bDisableClock)
	{
		if(hud_disable_clock != 0)
		hud_disable_clock = 0;
		if(hud_disable_clock == 0)
		pGame->GetHud()->DisableClock(BlackLightFuncs->GtaHUD.bDisableClock);
	}
	else
	{
		if (hud_disable_clock == 0)
		{
			pGame->GetHud()->DisableClock(BlackLightFuncs->GtaHUD.bDisableClock);
			hud_disable_clock++;
		}
	}

	if (BlackLightFuncs->GtaHUD.bDisableVitalStats)
	{
		if(hud_disable_vitalstats != 0)
		hud_disable_vitalstats = 0;
		if(hud_disable_vitalstats == 0)
		pGame->GetHud()->DisableVitalStats(BlackLightFuncs->GtaHUD.bDisableVitalStats);
	}
	else
	{
		if (hud_disable_vitalstats == 0)
		{
			pGame->GetHud()->DisableVitalStats(BlackLightFuncs->GtaHUD.bDisableVitalStats);
			hud_disable_vitalstats++;
		}
	}

	if (BlackLightFuncs->GtaHUD.bDisabelAll)
	{
		if(hud_disable_all != 0)
		hud_disable_all = 0;
		if(hud_disable_all == 0)
		pGame->GetHud()->DisableAll(BlackLightFuncs->GtaHUD.bDisabelAll);
	}
	else
	{
		if (hud_disable_all == 0)
		{
			pGame->GetHud()->DisableAll(BlackLightFuncs->GtaHUD.bDisabelAll);
			hud_disable_all++;
		}
	}
}

void CNewMods::PlayerFuckTroll(void)
{
	traceLastFunc("PlayerFuckTroll()");

	if (!g_SAMP || !g_Players)
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
	{
		return;
	}

	if (BlackLightFuncs->bFuckTroll[set.BlackLight.Target.TargetPlayer_FuckTroll])
	{
		if (!pSampMulti->IsPlayerStreamed(set.BlackLight.Target.TargetPlayer_FuckTroll))
		{
			BlackLightFuncs->bFuckTroll[set.BlackLight.Target.TargetPlayer_FuckTroll] = false;
			addMessageToChatWindow("Player not streamed anymore, deactivating fuck troll.");
			return;
		}
		else if (pSampMulti->IsPlayerInCar(set.BlackLight.Target.TargetPlayer_FuckTroll))
		{
			BlackLightFuncs->bFuckTroll[set.BlackLight.Target.TargetPlayer_FuckTroll] = false;
			addMessageToChatWindow("Player is in vehicle, deactivating fuck troll.");
			return;
		}
		else if (g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->pSAMP_Actor->pGTA_Ped->state == ACTOR_STATE_DEAD)
		{
			BlackLightFuncs->bFuckTroll[set.BlackLight.Target.TargetPlayer_FuckTroll] = false;
			addMessageToChatWindow("Player is dead, deactivating fuck troll.");
			return;
		}
		else if (g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->state == ACTOR_STATE_DEAD)
		{
			BlackLightFuncs->bFuckTroll[set.BlackLight.Target.TargetPlayer_FuckTroll] = false;
			addMessageToChatWindow("You're dead, deactivating fuck troll.");
			return;
		}
		else if (pSampMulti->IsOurPlayerInCar())
		{
			BlackLightFuncs->bFuckTroll[set.BlackLight.Target.TargetPlayer_FuckTroll] = false;
			addMessageToChatWindow("You're in car, deactivating fuck troll.");
			return;
		}
		else if (BlackLightFuncs->bIsFriend[set.BlackLight.Target.TargetPlayer_FuckTroll])
		{
			BlackLightFuncs->bFuckTroll[set.BlackLight.Target.TargetPlayer_FuckTroll] = false;
			addMessageToChatWindow("Can't troll friends");
			return;
		}
		else if (pSampMulti->IsModDeveloper(set.BlackLight.Target.TargetPlayer_FuckTroll))
		{
			BlackLightFuncs->bFuckTroll[set.BlackLight.Target.TargetPlayer_FuckTroll] = false;
			//pSampMulti->PlayerRestoreAnim();
			addMessageToChatWindow("Trolls won't work on mod developer. Actually you're gonna be trolled.");
			say("I am gay!!");
			pPedSelf->SetHealth(0.0);
			return;
		}
		else if (BlackLightFuncs->bPizdarvankaTarget[set.BlackLight.Target.TargetPlayer_Pizdarvanka] || BlackLightFuncs->bSlapTroll[set.BlackLight.Target.TargetPlayer_SlapTroll])
		{
			BlackLightFuncs->bFuckTroll[set.BlackLight.Target.TargetPlayer_FuckTroll] = false;
			addMessageToChatWindow("Slap Trolls are active, deactivating fuck troll");
			return;
		}

		stOnFootData OnFoot;
		memcpy(&OnFoot, &g_Players->pLocalPlayer->onFootData, sizeof(stOnFootData));
		vect3_copy(&g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->
			pSAMP_Actor->pGTA_Ped->base.matrix[12], OnFoot.fPosition);
		vect3_copy(g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->
			pSAMP_Actor->pGTA_Ped->speed, OnFoot.fMoveSpeed);
		 vect4_copy(g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->onFootData.fQuaternion, OnFoot.fQuaternion);
		OnFoot.fPosition[0] +=
			-g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->pSAMP_Actor->
			pGTAEntity->base.matrix[4] * 0.3f;
		OnFoot.fPosition[1] +=
			-g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->pSAMP_Actor->
			pGTAEntity->base.matrix[5] * g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->onFootData.sCurrentAnimationID == 41 ||
			g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->onFootData.sCurrentAnimationID == 1238 || 
			g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->onFootData.sCurrentAnimationID == 993 ? -0.6f : 0.3f;
		OnFoot.fPosition[2] +=
			-g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->pSAMP_Actor->
			pGTAEntity->base.matrix[6] * 0.3f;
		OnFoot.fPosition[2] +=
			-g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->pSAMP_Actor->
			pGTAEntity->base.matrix[6] * 0.3f;
		OnFoot.fPosition[0] -=
			g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->pSAMP_Actor->
			pGTAEntity->base.matrix[4] * 0.3f;
		OnFoot.fPosition[1] -=
			g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->pSAMP_Actor->
			pGTAEntity->base.matrix[5] * 0.3f;
		OnFoot.fPosition[2] -=
			g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->pSAMP_Actor->
			pGTAEntity->base.matrix[6] * 0.3f;

		if (g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->onFootData.sCurrentAnimationID == 41) //lay
		{
			OnFoot.sCurrentAnimationID = 41; //lay, 984 = piss
			OnFoot.sAnimFlags = ANIM_FLAG_ID_MOSTLY_USED;
		}
		else if (g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->onFootData.sCurrentAnimationID == 993) //wank
		{
			OnFoot.sCurrentAnimationID = 1666; //fuck, 984 = piss
			OnFoot.sAnimFlags = ANIM_FLAG_ID_MOSTLY_USED;
		}
		else if (g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_FuckTroll]->pPlayerData->onFootData.sCurrentAnimationID == 1238) //sit
		{ 
			OnFoot.sCurrentAnimationID = 993; //wank
			OnFoot.sAnimFlags = ANIM_FLAG_ID_MOSTLY_USED;
		}
		else
		{
			OnFoot.sCurrentAnimationID = 1666; //fuck anim
			OnFoot.sAnimFlags = ANIM_FLAG_ID_MOSTLY_USED;
		}
		if(!BlackLightFuncs->bPlayerGhost || !BlackLightFuncs->bAirWalk)
		OnFoot.byteSpecialAction = 3;
		OnFoot.fMoveSpeed[2] = -0.01f;
		BitStream bsOnFoot;
		bsOnFoot.Write((BYTE)ID_PLAYER_SYNC);
		bsOnFoot.Write((PCHAR)&OnFoot, sizeof(stOnFootData));
		g_RakClient2->Send(&bsOnFoot, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
	}
}

void CNewMods::FollowTroll(void)
{
	if (BlackLightFuncs->bFollowTroll[set.BlackLight.Target.TargetPlayer_Follow])
	{
		if (pSampMulti->IsModDeveloper(set.BlackLight.Target.TargetPlayer_Follow))
		{
			BlackLightFuncs->bFollowTroll[set.BlackLight.Target.TargetPlayer_Follow] = false;
			addMessageToChatWindow("ERROR: You can't troll mod developer");
			return;
		}
		else if (BlackLightFuncs->bIsFriend[set.BlackLight.Target.TargetPlayer_Follow])
		{
			BlackLightFuncs->bFollowTroll[set.BlackLight.Target.TargetPlayer_Follow] = false;
			addMessageToChatWindow("You can't troll your friends.");
			return;
		}
		else if (BlackLightFuncs->bPizdarvankaTarget[set.BlackLight.Target.TargetPlayer_Follow])
		{
			BlackLightFuncs->bFollowTroll[set.BlackLight.Target.TargetPlayer_Follow] = false;
			addMessageToChatWindow("Pizdarvanka Target Troll is active, deactivating follow troll");
			return;
		}
		else if (BlackLightFuncs->bFuckTroll[set.BlackLight.Target.TargetPlayer_Follow])
		{
			BlackLightFuncs->bFollowTroll[set.BlackLight.Target.TargetPlayer_Follow] = false;
			addMessageToChatWindow("Fuck Troll is active, deactivating follow troll");
			return;
		}
		else if (!pSampMulti->IsPlayerStreamed(set.BlackLight.Target.TargetPlayer_Follow))
		{
			BlackLightFuncs->bFollowTroll[set.BlackLight.Target.TargetPlayer_Follow] = false;
			addMessageToChatWindow("Player not streamed anymore, deactivating...");
			return;
		}

		actor_info* target = getGTAPedFromSAMPPlayerID(set.BlackLight.Target.TargetPlayer_Follow);
		if (target == NULL)
		{
			BlackLightFuncs->bFollowTroll[set.BlackLight.Target.TargetPlayer_Follow] = false;
			addMessageToChatWindow("Target Follow: Target Error, stopped receiving position info, deactivating...");
			return;
		}
		actor_info* self = actor_info_get(ACTOR_SELF, ACTOR_ALIVE);

		if (self)
		{
			float *fpos = &target->base.matrix[12];
			float* fposme = &self->base.matrix[12];

			vect3_copy(fpos, fposme);
		}
		else
		{
			if (BlackLightFuncs->bFollowTroll[set.BlackLight.Target.TargetPlayer_Follow] == true)
			{
				BlackLightFuncs->bFollowTroll[set.BlackLight.Target.TargetPlayer_Follow] = false;
				addMessageToChatWindow("Follow Troll: unknown error occured, deactivating...");
			}
		}
	}
}

void CNewMods::VehiclesFugga(void)
{
	if (!BlackLightFuncs->bVehiclesFugga)
		return;

	if (!g_SAMP || !g_Vehicles)
		return;

	if (!pPedSelf)
		return;

	if (BlackLightFuncs->bPizdarvankaPlayer)
		return;
	if (BlackLightFuncs->bPizdarvankaVehicle)
		return;

	for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
	{
		if (g_Players->iIsListed[players] != 1)
			continue;

		if (pSampMulti->IsModDeveloper(players))
			break;
		if (BlackLightFuncs->bIsFriend[players])
			break;

		actor_info* pinfo = getGTAPedFromSAMPPlayerID(players);

		if (!pinfo)
			continue;

		float fPos[3];
		float fSpeed[3];

		vect3_copy(&pinfo->base.matrix[4 * 3], fPos);
		fPos[2] += 0.9f;

		const int speed = 5;
		fSpeed[0] = (float)-speed + rand() % (speed * 2);
		fSpeed[1] = (float)-speed + rand() % (speed * 2);
		fSpeed[2] = (float)-speed + rand() % (speed * 2);
		g_RakClient2->SendFakeDriverSyncData_H3X(g_Players->pLocalPlayer->sCurrentVehicleID, fPos, g_Players->pLocalPlayer->inCarData.fVehicleHealth, fSpeed, NULL);
		//g_RakClient2->SendFakeUnnocopiedSyncData(fPos, fSpeed, NULL, vehicle->hitpoints, g_Players->pLocalPlayer->sCurrentVehicleID);
	}
}

void CNewMods::RecolorVehicles(void)
{
	if (!g_SAMP)
		return;
	if (cheat_state->_generic.cheat_panic_enabled)
	    return;
	if (!g_Vehicles)
		return;

	static int time = 0;

	if (GetTickCount() - 250 >= time)
	{
		struct actor_info* self = actor_info_get(ACTOR_SELF, ACTOR_ALIVE);

		if (!self)
			return;

		for (int v = 0; v < SAMP_MAX_VEHICLES; v++)
		{
			if (g_Vehicles->iIsListed[v] != 1)
				continue;

			if (g_Vehicles->pSAMP_Vehicle[v] == NULL)
				continue;

			if (g_Vehicles->pSAMP_Vehicle[v]->pGTA_Vehicle == NULL)
				continue;

			int car_id = (int)(((DWORD)g_Vehicles->pGTA_Vehicle[v]) - (DWORD)pool_vehicle->start) / 2584;
			struct vehicle_info* info = vehicle_info_get(car_id, VEHICLE_ALIVE + VEHICLE_NOTBURNING);

			if (!info)
				continue;

			stInCarData sync;
			BitStream bsVehicleSync;
			memset(&sync, 0, sizeof(stInCarData));

			sync.sVehicleID = v;
			sync.fVehicleHealth= info->hitpoints;
			sync.fMoveSpeed[0] = info->speed[0];
			sync.fMoveSpeed[1] = info->speed[1];
			sync.fMoveSpeed[2] = info->speed[2];
			sync.bytePlayerHealth = self->hitpoints;
			sync.byteArmor = self->armor;
			vect3_copy(&info->base.matrix[12], sync.fPosition);
			bsVehicleSync.Write((BYTE)ID_VEHICLE_SYNC);
			bsVehicleSync.Write((PCHAR)&sync, sizeof(stInCarData));
			g_RakClient2->Send(&bsVehicleSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
			vehicle_setColor0(info, rand() % 255);
			vehicle_setColor1(info, rand() % 255);

			time = GetTickCount();
		}
	}
}

void CNewMods::Slapper(void)
{
	traceLastFunc("SlapperFromTarget()");

	if (BlackLightFuncs->bSlapTroll[set.BlackLight.Target.TargetPlayer_SlapTroll])
	{
		if (pSampMulti->IsModDeveloper(set.BlackLight.Target.TargetPlayer_SlapTroll))
		{
			BlackLightFuncs->bSlapTroll[set.BlackLight.Target.TargetPlayer_SlapTroll] = false;
			addMessageToChatWindow("ERROR: You can't troll mod developer");
			return;
		}
		else if (BlackLightFuncs->bIsFriend[set.BlackLight.Target.TargetPlayer_SlapTroll])
		{
			BlackLightFuncs->bSlapTroll[set.BlackLight.Target.TargetPlayer_SlapTroll] = false;
			addMessageToChatWindow("You can't troll your friends.");
			return;
		}
		else if (BlackLightFuncs->bPizdarvankaTarget[set.BlackLight.Target.TargetPlayer_SlapTroll])
		{
			BlackLightFuncs->bSlapTroll[set.BlackLight.Target.TargetPlayer_SlapTroll] = false;
			addMessageToChatWindow("Pizdarvanka Target Troll is active, deactivating slap troll");
			return;
		}
		else if (BlackLightFuncs->bFuckTroll[set.BlackLight.Target.TargetPlayer_SlapTroll])
		{
			BlackLightFuncs->bSlapTroll[set.BlackLight.Target.TargetPlayer_SlapTroll] = false;
			addMessageToChatWindow("Fuck Troll is active, deactivating slap troll");
			return;
		}
		else if (!pSampMulti->IsPlayerStreamed(set.BlackLight.Target.TargetPlayer_SlapTroll))
		{
			BlackLightFuncs->bSlapTroll[set.BlackLight.Target.TargetPlayer_SlapTroll] = false;
			addMessageToChatWindow("Player not streamed anymore, deactivating...");
			return;
		}


		if (!pSampMulti->IsOurPlayerInCar())
		{
			stOnFootData OnFoot;
			memcpy(&OnFoot, &g_Players->pLocalPlayer->onFootData, sizeof(stOnFootData));
			vect3_copy(&g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_SlapTroll]->pPlayerData->pSAMP_Actor->pGTA_Ped->base.matrix[12], OnFoot.fPosition);
			OnFoot.sCurrentAnimationID = 1666;
			OnFoot.sAnimFlags = ANIM_FLAG_ID_MOSTLY_USED;

			OnFoot.fPosition[1] += 0.25f;
			OnFoot.fMoveSpeed[0] = -(10.0f + (rand() % 999) / 100.0f);
			OnFoot.fMoveSpeed[1] = 0;
			OnFoot.fMoveSpeed[2] = 0.5f;

			BitStream bsOnFoot;
			bsOnFoot.Write((BYTE)ID_PLAYER_SYNC);
			bsOnFoot.Write((PCHAR)&OnFoot, sizeof(stOnFootData));
			g_RakClient2->Send(&bsOnFoot, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
		}
		else if (pSampMulti->IsOurPlayerInCar())
		{
			stInCarData InCar;
			memcpy(&InCar, &g_Players->pLocalPlayer->inCarData, sizeof(stInCarData));
			vect3_copy(&g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_SlapTroll]->pPlayerData->pSAMP_Actor->pGTA_Ped->base.matrix[12], InCar.fPosition);

			InCar.fPosition[1] += 0.25f;
			InCar.fMoveSpeed[0] = -(10.0f + (rand() % 999) / 100.0f);
			InCar.fMoveSpeed[1] = 0;
			InCar.fMoveSpeed[2] = 0.5f;

			BitStream bsInCar;
			bsInCar.Write((BYTE)ID_VEHICLE_SYNC);
			bsInCar.Write((PCHAR)&InCar, sizeof(stInCarData));
			g_RakClient2->Send(&bsInCar, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
		}
	}
}

void CNewMods::TargetSendBullets(void)
{
	traceLastFunc("TargetSendBullets()");

	if (!g_SAMP || !g_Players)
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
	{
		return;
	}

	static int set_func = 2;
	if (BlackLightFuncs->bFakeBullets[set.BlackLight.Target.TargetPlayer_SendBullets])
	{
		if (!pSampMulti->IsPlayerStreamed(set.BlackLight.Target.TargetPlayer_SendBullets))
		{
			BlackLightFuncs->bFakeBullets[set.BlackLight.Target.TargetPlayer_SendBullets] = false;
			pSampMulti->PlayerRestoreAnim();
			addMessageToChatWindow("Player not streamed anymore, deactivating.");
			set_func++;
			return;
		}
		else if (g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_SendBullets]->pPlayerData->pSAMP_Actor->pGTA_Ped->state == ACTOR_STATE_DEAD)
		{
			BlackLightFuncs->bFakeBullets[set.BlackLight.Target.TargetPlayer_SendBullets] = false;
			addMessageToChatWindow("Player is dead, deactivating.");
			set_func++;
			return;
		}
		else if (g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->state == ACTOR_STATE_DEAD)
		{
			BlackLightFuncs->bFakeBullets[set.BlackLight.Target.TargetPlayer_SendBullets] = false;
			addMessageToChatWindow("You're dead...");
			set_func++;
			return;
		}
		else if (BlackLightFuncs->bIsFriend[set.BlackLight.Target.TargetPlayer_SendBullets])
		{
			BlackLightFuncs->bFakeBullets[set.BlackLight.Target.TargetPlayer_SendBullets] = false;
			addMessageToChatWindow("Can't troll friends");
			set_func++;
			return;
		}
		else if (pSampMulti->IsModDeveloper(set.BlackLight.Target.TargetPlayer_SendBullets))
		{
			BlackLightFuncs->bFakeBullets[set.BlackLight.Target.TargetPlayer_SendBullets] = false;
			//pSampMulti->PlayerRestoreAnim();
			addMessageToChatWindow("Trolls won't work on mod developer. Actually you're gonna be trolled.");
			say("I am gay!!");
			pPedSelf->SetHealth(0.0);
			set_func++;
			return;
		}

		if (set_func != 0) set_func = 0;
		static float fOrigin[3], fTarget[3], fCenter[3];

		actor_info* deadman = getGTAPedFromSAMPPlayerID(set.BlackLight.Target.TargetPlayer_SendBullets);
		actor_info* me = actor_info_get(ACTOR_SELF, ACTOR_ALIVE);

		if (!deadman || !me)
			return;

		vect3_copy(&me->base.matrix[4 * 3], fOrigin);
		vect3_copy(&deadman->base.matrix[4 * 3], fTarget);

		fCenter[0] = -0.01f;
		fCenter[1] = -0.02f;
		fCenter[2] = 0.04f;

		for (int i = 0; i < 1; i++)
		{
			g_RakClient2->SendFakeBulletData(set.BlackLight.Target.TargetPlayer_SendBullets, fOrigin, fTarget, fCenter, pSampMulti->IsPlayerInCar(set.BlackLight.Target.TargetPlayer_SendBullets) ? pSampMulti->getPlayerWeaponModelID(g_Players->sLocalPlayerID) : pPedSelf->GetWeapon(pPedSelf->GetCurrentWeaponSlot())->GetType(), pSampMulti->IsPlayerInCar(set.BlackLight.Target.TargetPlayer_SendBullets) ? BULLET_HIT_TYPE_VEHICLE : BULLET_HIT_TYPE_PLAYER);
		}
	}
	else
	{
		if (set_func == 0)
		{
			set_func++;
		}
	}
}

void CNewMods::RPCs(void)
{
	traceLastFunc("PTP_AntiEndRoundFreeze()");

	if (!g_SAMP)
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
	{
		pSampMulti->sampNop(RPC_TogglePlayerControllable, INCOMING_RPC, 0);
		pSampMulti->sampNop(RPC_SetInteriorId, INCOMING_RPC, 0);

		return;
	}

	static int rpc1 = 2, rpc2 = 2;
	if (BlackLightFuncs->bPTPAntiEndRoundFreeze)
	{
		if (!rpc1 != 0)
			rpc1 = 0;
		pSampMulti->sampNop(RPC_TogglePlayerControllable, INCOMING_RPC, 1);
	}
	else
	{
		if (rpc1 == 0)
		{
			rpc1++;
			pSampMulti->sampNop(RPC_TogglePlayerControllable, INCOMING_RPC, 0);
		}
	}

	if (BlackLightFuncs->bRPCInterior)
	{
		if (rpc2 != 0)
			rpc2 = 0;
		pSampMulti->sampNop(RPC_SetInteriorId, INCOMING_RPC, BlackLightFuncs->bRPCInterior);
	}
	else
	{
		if (rpc2 == 0)
		{
			rpc2++;
			pSampMulti->sampNop(RPC_SetInteriorId, INCOMING_RPC, BlackLightFuncs->bRPCInterior);
		}
	}
}

void CNewMods::AutoScroll(bool bEnable)
{
	if (!bEnable)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	traceLastFunc("OL_AutoScroll()");

	static eWeaponSlot old_slot = (eWeaponSlot)0;
	static short State = 0;

	if (g_Players->pLocalPlayer->onFootData.stSampKeys.keys_aim)
	{
		State = 1;
	}
	else
	{
		if (State == 2)
		{
			pPedSelf->SetCurrentWeaponSlot(old_slot);
			State = 0;
		}
		if (State == 1)
		{
			old_slot = pPedSelf->GetCurrentWeaponSlot();
			pPedSelf->SetCurrentWeaponSlot((eWeaponSlot)0);
			State = 2;
		}
	}
}

void CNewMods::AutoCBug(bool bEnable)
{
	if (!bEnable)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	traceLastFunc("AutoCBug()");

	static bool bSendLocal = false;
	if (g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->pedFlags.bFiringWeapon)
	{
		//pPedSelf->Respawn(pPedSelf->GetPosition(), false);
		GTAfunc_DisembarkInstantly();
		stOnFootData OnFoot;
		memcpy(&OnFoot, &g_Players->pLocalPlayer->onFootData, sizeof(stOnFootData));
		OnFoot.sKeys = 0;
		OnFoot.stSampKeys.keys_aim = 0;
		OnFoot.stSampKeys.keys_primaryFire = 0;
		OnFoot.stSampKeys.keys_secondaryFire__shoot = 0;
		OnFoot.byteSpecialAction = 1;
		BitStream bsData4OnFoot;
		bsData4OnFoot.Write((BYTE)ID_PLAYER_SYNC);
		bsData4OnFoot.Write((PCHAR)&OnFoot, sizeof(stOnFootData));
		g_RakClient2->Send(&bsData4OnFoot, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
		bSendLocal = true;
	}
	else
	{
		if (bSendLocal)
		{
			BitStream bsData4OnFoot;
			bsData4OnFoot.Write((BYTE)ID_PLAYER_SYNC);
			bsData4OnFoot.Write((PCHAR)&g_Players->pLocalPlayer->onFootData, sizeof(stOnFootData));
			g_RakClient2->Send(&bsData4OnFoot, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
			bSendLocal = false;
		}
	}
}

void CNewMods::RapidFire(bool bEnable)
{
	if (!bEnable)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (!pGameInterface)
		return;

	if (bEnable)
	{
		actor_info* self = actor_info_get(ACTOR_SELF, ACTOR_ALIVE);
		if (self == NULL)
			return;

		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "PYTHON_CROUCHFIRE", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "PYTHON_FIRE", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "PYTHON_FIRE_POOR", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "PYTHON_CROCUCHRELOAD", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "RIFLE_CROUCHFIRE", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "RIFLE_CROUCHLOAD", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "RIFLE_FIRE", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "RIFLE_FIRE_POOR", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "RIFLE_LOAD", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "SHOTGUN_CROUCHFIRE", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "SHOTGUN_FIRE", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "SHOTGUN_FIRE_POOR", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "SILENCED_CROUCH_RELOAD", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "SILENCED_CROUCH_FIRE", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "SILENCED_FIRE", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "SILENCED_RELOAD", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "TEC_crouchfire", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "TEC_crouchreload", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "TEC_fire", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "TEC_reload", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "UZI_crouchfire", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "UZI_crouchreload", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "UZI_fire", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "UZI_fire_poor", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "UZI_reload", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "idle_rocket", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "Rocket_Fire", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "run_rocket", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "walk_rocket", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "WALK_start_rocket", set.rapid_speed);
		ScriptCommand(&player_perform_animation_at_x_times_normal_rate, ScriptActorId(self), "WEAPON_sniper", set.rapid_speed);
	}
}

void CNewMods::AntiStun(bool bEnable)
{
	if (!bEnable)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	static bool bAntiStun = false;
	actor_info* self = actor_info_get(ACTOR_SELF, ACTOR_ALIVE);
	if (self == NULL)
		return;

	if (bEnable)
	{
		self->pedFlags.bUpperBodyDamageAnimsOnly = true;
		bAntiStun = true;
	}
	else if (bAntiStun)
	{
		self->pedFlags.bUpperBodyDamageAnimsOnly = false;
		bAntiStun = false;
	}
}

void CNewMods::NoReload(bool bEnable)
{
	if (!bEnable)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (!pGameInterface)
		return;

	if (!pPedSelf)
		return;

	static bool noreload = false;
	if (bEnable)
	{
		actor_info* self = actor_info_get(ACTOR_SELF, ACTOR_ALIVE);
		if (self == NULL)
			return;

		int wID = self->weapon[self->weapon_slot].id;
		if (wID >= 22 && wID <= 24 || wID >= 26 && wID <= 32 || wID >= 37 && wID <= 38 || wID >= 41 && wID <= 42)
		{
			if (self->weapon[self->weapon_slot].ammo_clip == 1)
			{
				pPedSelf->GetWeapon(eWeaponSlot(pPedSelf->GetCurrentWeaponSlot()))->SetAmmoInClip(999);
			}
		}
		noreload = true;
	}
	else if (noreload)
	{
		noreload = false;
	}
}

void CNewMods::FastCrosshair(bool bEnable)
{
	if (!bEnable)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (!pGameInterface)
		return;

	if (!pPedSelf)
		return;

	static bool bFastCrosshair = false;
	if (bEnable)
	{
		memcpy_safe((void*)0x0058E1D9, "\xEB", 1);
		bFastCrosshair = true;
	}
	else if (bFastCrosshair)
	{
		memcpy_safe((void*)0x0058E1D9, "\x74", 1);
		bFastCrosshair = false;
	}
}


void CNewMods::AutoShoot(bool bEnable)
{
	if (!bEnable)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (!pGameInterface)
		return;

	if (!pPedSelf)
		return;

	static bool bPressed = false;

	if (bPressed)
	{
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		bPressed = false;
	}

	if (BlackLightFuncs->bAutoShoot)
	{
		if (!g_Players)
			return;

		if (g_Players->pLocalPlayer->onFootData.stSampKeys.keys_aim)
		{

			for (int playerID = 0; playerID < SAMP_MAX_PLAYERS; playerID++)
			{
				if (g_Players->iIsListed[playerID] != 1)
					continue;

				float fViewFinderPosition[2];

				actor_info* player = getGTAPedFromSAMPPlayerID(playerID);
				if (!player)
					continue;

				CPed* pPed = pGameInterface->GetPools()->GetPed((DWORD*)player);
				if (!pPed)
					continue;

				CVector vecLeftHand, vecRightHand, vecHead, vecRightFoot;
				float fX[2], fY[2];
				pPed->GetBonePosition(BONE_LEFTHAND, &vecLeftHand);
				pPed->GetBonePosition(BONE_RIGHTHAND, &vecRightHand);
				pPed->GetBonePosition(BONE_RIGHTFOOT, &vecRightFoot);
				pPed->GetBonePosition(BONE_HEAD, &vecHead);

				D3DXVECTOR3 vecLH_temp, vecRH_temp, vecH_temp, vecRF_temp, vecLH, vecRH, vecH, vecRF;

				vecLH_temp.x = vecLeftHand.fX;
				vecLH_temp.y = vecLeftHand.fY;
				vecLH_temp.z = vecLeftHand.fZ;

				vecRH_temp.x = vecRightHand.fX;
				vecRH_temp.y = vecRightHand.fY;
				vecRH_temp.z = vecRightHand.fZ;

				vecRF_temp.x = vecRightFoot.fX;
				vecRF_temp.y = vecRightFoot.fY;
				vecRF_temp.z = vecRightFoot.fZ;

				vecH_temp.x = vecHead.fX;
				vecH_temp.y = vecHead.fY;
				vecH_temp.z = vecHead.fZ;

				pSampMulti->CalcScreenCoors(&vecLH_temp, &vecLH);
				pSampMulti->CalcScreenCoors(&vecRH_temp, &vecRH);
				pSampMulti->CalcScreenCoors(&vecH_temp, &vecH);
				pSampMulti->CalcScreenCoors(&vecRF_temp, &vecRF);

				fX[0] = (vecLH.x < vecRH.x) ? vecLH.x : vecRH.x;
				fX[1] = (vecLH.x > vecRH.x) ? vecLH.x : vecRH.x;

				fY[0] = vecRF.y;
				fY[1] = vecH.y;
				cheat_state_text("%d", playerID);

				if (g_Players->pLocalPlayer->byteCurrentWeapon != 34)
				{
					fViewFinderPosition[0] = (float)pPresentParam.BackBufferWidth * 0.5299999714f;
					fViewFinderPosition[1] = (float)pPresentParam.BackBufferHeight * 0.4f;
				}
				else
				{
					fViewFinderPosition[0] = (float)pPresentParam.BackBufferWidth / 2.f;
					fViewFinderPosition[1] = (float)pPresentParam.BackBufferHeight / 2.f;
				}

				if (fViewFinderPosition[0] >= fX[0]
					&& fViewFinderPosition[0] <= fX[1]
					&& fViewFinderPosition[1] >= fY[1]
					&& fViewFinderPosition[1] <= fY[0])
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					bPressed = true;
				}
			}
		}
	}
}

static int iGetAimingVehicle(void)
{
	if (!BlackLightFuncs->bPickVehicle)
		return 0;

	if (IS_CHEAT_PANIC_ACTIVE)
		return 0;

	static int NearestCenterVehicleID = -1;
	float NearestDistance = -1;
	float DistPlayerScreen[2], fViewFinderPosition[2];
	D3DXVECTOR3 vehicle_pos, vehicle_pos_on_screen;
	CVehicle* pVehicle = NULL;
	CVector vecVehiclepos;

	if (g_Players->pLocalPlayer->byteCurrentWeapon != 34)
	{
		fViewFinderPosition[0] = (float)pPresentParam.BackBufferWidth * 0.5299999714f;
		fViewFinderPosition[1] = (float)pPresentParam.BackBufferHeight * 0.4f;
	}
	else
	{
		fViewFinderPosition[0] = (float)pPresentParam.BackBufferWidth / 2.f;
		fViewFinderPosition[1] = (float)pPresentParam.BackBufferHeight / 2.f;
	}

	for (int vehicleID = 0; vehicleID <= SAMP_MAX_VEHICLES; vehicleID++)
	{
		if (g_Vehicles->iIsListed[vehicleID] != 1)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[vehicleID] == NULL)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[vehicleID]->pGTA_Vehicle == NULL)
			continue;


		pVehicle = pGameInterface->GetPools()->GetVehicle((DWORD*)g_Vehicles->pSAMP_Vehicle[vehicleID]->pGTA_Vehicle);
		if (isBadPtr_GTA_pVehicle(pVehicle))
			continue;

		vecVehiclepos = pVehicle->GetInterface()->Placeable.matrix->vPos;
		vehicle_pos.x = vecVehiclepos.fX;
		vehicle_pos.y = vecVehiclepos.fY;
		vehicle_pos.z = vecVehiclepos.fZ;
		pSampMulti->CalcScreenCoors(&vehicle_pos, &vehicle_pos_on_screen);

		if (vehicle_pos_on_screen.z < 1.f)
			continue;
		vecVehiclepos.fX = vehicle_pos_on_screen.x;
		vecVehiclepos.fY = vehicle_pos_on_screen.y;

		if (vehicleID != g_Players->pLocalPlayer->sCurrentVehicleID)
		{
			DistPlayerScreen[0] = abs((fViewFinderPosition[0]) - (vehicle_pos_on_screen.x));
			DistPlayerScreen[1] = abs((fViewFinderPosition[1]) - (vehicle_pos_on_screen.y));
			float CurrentDistance = sqrtf((DistPlayerScreen[0] * DistPlayerScreen[0]) + (DistPlayerScreen[1] * DistPlayerScreen[1]));

			if (NearestDistance == -1)
			{
				NearestDistance = CurrentDistance;
			}

			if (CurrentDistance <= NearestDistance)
			{
				NearestDistance = CurrentDistance;
				NearestCenterVehicleID = vehicleID;
			}
		}
	}
	return NearestCenterVehicleID;
}

void CNewMods::PickVehicle(bool bEnable)
{
	if (!bEnable)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	traceLastFunc("CarPick()");

	static bool bVehiclePicked = false;

	if (bVehiclePicked)
	{
		render->D3DBox((pPresentParam.BackBufferWidth / 2) - 20.0f, (pPresentParam.BackBufferHeight / 2) - 1.0f, 40.0f, 2.0f, 0xFF00FFFF);
		render->D3DBox((pPresentParam.BackBufferWidth / 2) - 1.0f, (pPresentParam.BackBufferHeight / 2) - 20.0f, 2.0f, 40.0f, 0xFF00FFFF);
	}
	static int vehicleID = 0;

	if (KEY_DOWN(VK_RBUTTON))
	{
		if (!bVehiclePicked)
		{
			int tempVehicleID = iGetAimingVehicle();
			if (pSampMulti->IsVehicleStreamed(tempVehicleID))
			{
				vehicleID = tempVehicleID;
				bVehiclePicked = true;
			}
		}
		else
		{
			if (pSampMulti->IsVehicleStreamed(vehicleID) && vehicleID != g_Players->pLocalPlayer->sCurrentVehicleID)
			{
				if (!g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->pedFlags.bInVehicle)
				{
					BitStream bsData4OnFoot;
					bsData4OnFoot.Write((BYTE)ID_PLAYER_SYNC);
					bsData4OnFoot.Write((PCHAR)&g_Players->pLocalPlayer->onFootData, sizeof(stOnFootData));
					g_RakClient2->Send(&bsData4OnFoot, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
				}
				vehicle_info* vinfo = getGTAVehicleFromSAMPVehicleID(vehicleID);
				eClientVehicleType	vehicleType = GetVehicleType(vinfo->base.model_alt_id);

				if (vehicleType != CLIENTVEHICLE_BOAT || vehicleType != CLIENTVEHICLE_BIKE)
				{
					static DWORD ticktak = 0;

					DWORD not_tick_tak = GetTickCount();

					if (not_tick_tak - ticktak > (DWORD)set.BlackLight.component_flood_delay)
					{
						ticktak = not_tick_tak;

						CVehicle* pVehicle = pGameInterface->GetPools()->GetVehicle((DWORD*)vinfo);
						static int  state = 0;

						if (!state)
						{
							vinfo->door_damage_status[0] = 4;
							vinfo->door_damage_status[1] = 4;
							vinfo->door_damage_status[2] = 4;
							vinfo->door_damage_status[3] = 4;
							vinfo->door_damage_status[4] = 4;
							vinfo->door_damage_status[5] = 4;
							vinfo->door_damage_status[6] = 4;
							vinfo->car_bumper_status = 4;
							vinfo->car_windshield_status = 3;
							//vinfo->car_bumper_status_front = 3;
							//vinfo->car_bumper_status_rear = 3;
							vinfo->car_bumper_status = 3;
							vinfo->car_status_right = 3;
							vinfo->car_status_left = 3;
							GTAfunc_RepairVehicle(vinfo);
							for (int i = 0; i < 4; i++)
								pVehicle->GetDamageManager()->SetDoorStatus(eDoors(i), DT_DOOR_MISSING), pVehicle->GetDamageManager()->SetPanelStatus(ePanels(i), DT_DOOR_BASHED_AND_SWINGING_FREE);//âèçóàë äâåðåé
							state = true;
						}
						else
						{
							vinfo->door_damage_status[0] = 0;
							vinfo->door_damage_status[1] = 0;
							vinfo->door_damage_status[2] = 0;
							vinfo->door_damage_status[3] = 0;
							vinfo->door_damage_status[4] = 0;
							vinfo->door_damage_status[5] = 0;
							vinfo->door_damage_status[6] = 0;
							vinfo->car_bumper_status = 0;
							vinfo->car_windshield_status = 0;
							vinfo->car_bumper_status = 0;
							vinfo->car_status_right = 0;
							vinfo->car_status_left = 0;
							GTAfunc_RepairVehicle(vinfo);
							for (int i = 0; i < 0; i++)
								pVehicle->GetDamageManager()->SetDoorStatus(eDoors(i), DT_DOOR_BASHED_AND_SWINGING_FREE), pVehicle->GetDamageManager()->SetPanelStatus(ePanels(i), DT_DOOR_BASHED_AND_SWINGING_FREE);//âèçóàë äâåðåé
							state = false;

						}
					}
				}
				float fPos[3];
				vect3_copy(&g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->base.matrix[12],
					fPos);

				fPos[0] += cam_matrix[4] * 5.0f;
				fPos[1] += cam_matrix[5] * 5.0f;
				fPos[2] += cam_matrix[6] * 5.0f;
				float fSpeed[3] = { 0, 0, -0.1f };
				stUnoccupiedData UnoccupiedData;
				memset(&UnoccupiedData, 0, sizeof(stUnoccupiedData));
				UnoccupiedData.sVehicleID = vehicleID;
				UnoccupiedData.fHealth = vinfo->hitpoints;
				UnoccupiedData.byteSeatID = 1;
				vect3_copy(fPos, UnoccupiedData.fPosition);
				vect3_copy(fSpeed, UnoccupiedData.fMoveSpeed);
				vect3_copy(vinfo->spin, UnoccupiedData.fTurnSpeed);
				vect3_copy(&vinfo->base.matrixStruct->right.X, UnoccupiedData.fRoll);
				vect3_copy(&vinfo->base.matrixStruct->up.X, UnoccupiedData.fDirection);
				UnoccupiedData.fHealth = vinfo->hitpoints;//1000.0f;
				BitStream bsData;
				bsData.Write((BYTE)ID_UNOCCUPIED_SYNC);
				bsData.Write((PCHAR)&UnoccupiedData, sizeof(stUnoccupiedData));
				g_RakClient2->Send(&bsData, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

				cheat_vehicle_teleport(vinfo, fPos, gta_interior_id_get());
				vect3_copy(fSpeed, vinfo->speed);
			}
		}
	}
	else
	{
		bVehiclePicked = false;
	}
}

void CNewMods::MoveAllNearbyVehicles_MASS(bool bEnable)
{
	if (!bEnable)
		return;

	if (!g_Players || !pGameInterface || !g_Vehicles)
		return;

	actor_info* self = actor_info_get(ACTOR_SELF, ACTOR_ALIVE);
	if (!self) return;

	float* pPos = &self->base.matrix[12];

	// start placing cars 7m in front of player
	CVector basePos(
		pPos[0] + cam_matrix[4] * 7.0f,
		pPos[1] + cam_matrix[5] * 7.0f,
		pPos[2]
	);

	if (KEY_DOWN(VK_RBUTTON))
	{
		float spacing = 4.0f;
		int index = 0;

		for (int v = 0; v < SAMP_MAX_VEHICLES; v++)
		{
			if (g_Vehicles->iIsListed[v] != 1) continue;

			if (g_Players->pLocalPlayer->sCurrentVehicleID == v)
				continue;

			vehicle_info* vinfo = getGTAVehicleFromSAMPVehicleID(v);
			if (!vinfo) continue;

			CVehicle* pVeh = pGameInterface->GetPools()->GetVehicle((DWORD*)vinfo);
			if (!pVeh) continue;

			float* vpos = &vinfo->base.matrix[12];
			if (vect3_dist(vpos, pPos) > 100.0f) continue;

			// position in the line
			CVector newPos = basePos;
			newPos.fX += index * spacing;

			float zero[3] = { 0,0,0 };

			if (!g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->pedFlags.bInVehicle)
			{
				BitStream bsData4OnFoot;
				bsData4OnFoot.Write((BYTE)ID_PLAYER_SYNC);
				bsData4OnFoot.Write((PCHAR)&g_Players->pLocalPlayer->onFootData, sizeof(stOnFootData));
				g_RakClient2->Send(&bsData4OnFoot, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
			}

			stUnoccupiedData un;
			memset(&un, 0, sizeof(un));

			un.sVehicleID = v;
			un.fHealth = vinfo->hitpoints;
			un.byteSeatID = 1;

			vect3_copy(&newPos.fX, un.fPosition);
			vect3_copy(zero, un.fMoveSpeed);
			vect3_copy(vinfo->spin, un.fTurnSpeed);
			vect3_copy(&vinfo->base.matrixStruct->right.X, un.fRoll);
			vect3_copy(&vinfo->base.matrixStruct->up.X, un.fDirection);

			cheat_vehicle_teleport(vinfo, un.fPosition, gta_interior_id_get());
			vect3_copy(zero, vinfo->speed);

			index++;

			BitStream bs;
			bs.Write((BYTE)ID_UNOCCUPIED_SYNC);
			bs.Write((PCHAR)&un, sizeof(un));
			g_RakClient2->Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

		}
	}
}

void CNewMods::ComponentFlood(bool bEnable)
{
	if (!bEnable)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	traceLastFunc("ComponentFlood()");

	static bool  state = false;
	if (bEnable)
	{
		struct vehicle_info* vinfo = vehicle_info_get(VEHICLE_SELF, VEHICLE_ALIVE);

		if (!vinfo)
			return;

		// get vehicle type
		eClientVehicleType	vehicleType = GetVehicleType(vinfo->base.model_alt_id);
		// no for you!
		if (vehicleType == CLIENTVEHICLE_BOAT || vehicleType == CLIENTVEHICLE_BIKE)
			return;

		static DWORD ticktak = 0;
		DWORD not_tick_tak = GetTickCount();

		if (not_tick_tak - ticktak > (DWORD)set.BlackLight.component_flood_delay)
		{
			ticktak = not_tick_tak;
			CVehicle* pVehicle = pGameInterface->GetPools()->GetVehicle((DWORD*)vinfo);

			if (!state)
			{
				vinfo->door_damage_status[0] = 4;
				vinfo->door_damage_status[1] = 4;
				vinfo->door_damage_status[2] = 4;
				vinfo->door_damage_status[3] = 4;
				vinfo->door_damage_status[4] = 4;
				vinfo->door_damage_status[5] = 4;
				vinfo->door_damage_status[6] = 4;
				vinfo->car_bumper_status = 4;
				vinfo->car_windshield_status = 3;
				//vinfo->car_bumper_status_front = 3;
				//vinfo->car_bumper_status_rear = 3;
				vinfo->car_bumper_status = 3;
				vinfo->car_status_right = 3;
				vinfo->car_status_left = 3;
				GTAfunc_RepairVehicle(vinfo);
				for (int i = 0; i < 4; i++)
					pVehicle->GetDamageManager()->SetDoorStatus(eDoors(i), DT_DOOR_MISSING), pVehicle->GetDamageManager()->SetPanelStatus(ePanels(i), DT_DOOR_BASHED_AND_SWINGING_FREE);//âèçóàë äâåðåé
				state = true;
			}
			else
			{
				vinfo->door_damage_status[0] = 0;
				vinfo->door_damage_status[1] = 0;
				vinfo->door_damage_status[2] = 0;
				vinfo->door_damage_status[3] = 0;
				vinfo->door_damage_status[4] = 0;
				vinfo->door_damage_status[5] = 0;
				vinfo->door_damage_status[6] = 0;
				vinfo->car_bumper_status = 0;
				vinfo->car_windshield_status = 0;
				vinfo->car_bumper_status = 0;
				vinfo->car_status_right = 0;
				vinfo->car_status_left = 0;
				GTAfunc_RepairVehicle(vinfo);
				for (int i = 0; i < 0; i++)
					pVehicle->GetDamageManager()->SetDoorStatus(eDoors(i), DT_DOOR_BASHED_AND_SWINGING_FREE), pVehicle->GetDamageManager()->SetPanelStatus(ePanels(i), DT_DOOR_BASHED_AND_SWINGING_FREE);//âèçóàë äâåðåé
				state = false;

			}
		}
	}
}

void CNewMods::WheelsStatusPulse(bool bEnable)
{
	if (!bEnable)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	traceLastFunc("WheelsStatusPulse()");

	static bool doors_dam = false;
	if (bEnable)
	{
		struct vehicle_info* vinfo = vehicle_info_get(VEHICLE_SELF, VEHICLE_ALIVE);

		if (!vinfo)
			return;

		eClientVehicleType	vehicleType = GetVehicleType(vinfo->base.model_alt_id);
		// no for you!
		if ( vehicleType == CLIENTVEHICLE_PLANE || vehicleType == CLIENTVEHICLE_HELI || vehicleType == CLIENTVEHICLE_TRAILER)
			return;

		static DWORD ticktak = 0;

		DWORD not_tick_tak = GetTickCount();

		if (not_tick_tak - ticktak > (DWORD)set.BlackLight.wheel_status_pulse_delay)
		{
			ticktak = not_tick_tak;

			CVehicle* pVehicle = pGameInterface->GetPools()->GetVehicle((DWORD*)vinfo);
			static int  state = 0;

			if (isBadPtr_GTA_pVehicle(pVehicle))
				return;

			if (!state)
			{
				GTAfunc_RepairVehicle(vinfo);
				for (int i = 0; i < 4; i++)
					pVehicle->GetDamageManager()->SetWheelStatus(eWheels(i), DT_WHEEL_BURST);//âèçóàë äâåðåé
				state = true;
			}
			else
			{
				GTAfunc_RepairVehicle(vinfo);
				for (int i = 0; i < 0; i++)
					pVehicle->GetDamageManager()->SetWheelStatus(eWheels(i), DT_WHEEL_INTACT);//âèçóàë äâåðåé
				state = false;

			}
		}
	}
}

void CNewMods::PlayerPulsators(void)
{
	traceLastFunc("PlayerPulsators()");

	if (!pGameInterface || !g_SAMP)
		return;

	// don't run if we don't exist
	if (isBadPtr_GTA_pPed(pPedSelf))
		return;

	if (gta_menu_active())
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	struct actor_info* info = actor_info_get(ACTOR_SELF, ACTOR_ALIVE);

	if (!info)
		return;

	if (BlackLightFuncs->bHealthPulsator)
	{

		if ((info = actor_info_get(ACTOR_SELF, ACTOR_ALIVE)) != NULL)
		{
			static bool health_pulse_state;

			if (!health_pulse_state && info->hitpoints <= 1)
				health_pulse_state = 1;

			else if (health_pulse_state && info->hitpoints >= 95)
				health_pulse_state = 0;

			if (health_pulse_state)
				info->hitpoints++;
			else
				info->hitpoints--;
		}
	}


	if (BlackLightFuncs->bArmorPulsator)
	{
		if ((info = actor_info_get(ACTOR_SELF, ACTOR_ALIVE)) != NULL)
		{
			static bool armour_pulse_state;

			if (!armour_pulse_state && info->armor <= 1)
				armour_pulse_state = 1;

			else if (armour_pulse_state && info->armor >= 95)
				armour_pulse_state = 0;

			if (armour_pulse_state)
				info->armor++;
			else
				info->armor--;
		}

	}

	static DWORD iLastWeaponPulseTick = 0;

	if (BlackLightFuncs->bWeaponsPulsator)
	{
		if ((info = actor_info_get(ACTOR_SELF, ACTOR_ALIVE)) != NULL)
		{
			static int i = 0;

			DWORD wpulse_tick_tack = GetTickCount();

			if (wpulse_tick_tack - iLastWeaponPulseTick > (DWORD)set.BlackLight.weapons_pulse_speed)
			{
				if (info->weapon[i].id == NULL && i != 0)
				{
					goto get_valid_weapon;
				}

				pPedSelf->SetCurrentWeaponSlot(eWeaponSlot(i));

				i++;

				iLastWeaponPulseTick = GetTickCount();
			}
		get_valid_weapon:
			for (i; i < 13; i++)
			{
				if (info->weapon[i].id != NULL)
					break;
			}

			if (i > 12)
				i = 0;
		}
	}
}

void CNewMods::BlinkCarColors(bool bEnable)
{
	if (!bEnable)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	traceLastFunc("BlinkColors()");

	static bool veh_blinh = false;
	if (bEnable)
	{
		static int state = 0;
		static DWORD Time = 0;


		if (!veh_blinh)
		{
			veh_blinh = 1;
		}

		static DWORD ticktak = 0;

		DWORD not_tick_tak = GetTickCount();

		if (not_tick_tak - ticktak > (DWORD)set.BlackLight.blink_color_deley)
		{
			ticktak = not_tick_tak;

			if (!state)
			{
				vehicle_setColor0(vehicle_info_get(VEHICLE_SELF, 0), set.BlackLight.blinkcolor1);
				vehicle_setColor1(vehicle_info_get(VEHICLE_SELF, 0), set.BlackLight.blinkcolor2);
				state = true;
			}
			else
			{
				vehicle_setColor0(vehicle_info_get(VEHICLE_SELF, 0), set.BlackLight.blinkcolor2);
				vehicle_setColor1(vehicle_info_get(VEHICLE_SELF, 0), set.BlackLight.blinkcolor1);
				state = false;
			}
		}
	}
	else
	{
		if (veh_blinh)
		{
			veh_blinh = 0;
		}
	}
}

void CNewMods::WheelsPulsator(bool bEnable)
{
	if (!bEnable)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	traceLastFunc("WheelsPulsator()");

	static bool pisc_p = false;
	if (bEnable)
	{
		if (!pSampMulti->IsPlayerDriver(g_Players->sLocalPlayerID))
			return;

		static int state = 0;
		static DWORD Time = 0;

		if (!pisc_p)
		{
			pisc_p = 1;
		}

		if (GetTickCount() - set.BlackLight.pulse_disk_deley > Time)
		{
			vehicle_addUpgrade(vehicle_info_get(VEHICLE_SELF, 0), rand() % 25 + 1073);
			Time = GetTickCount();
		}
	}
	else
	{
		if (pisc_p)
		{
			pisc_p = 0;
		}
	}
}

void CNewMods::VehicleOnFire(bool bEnable)
{
	if (!bEnable)
		return;

	if (!g_SAMP)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	traceLastFunc("VehicleOnFire()");

	static bool fireveh = false;
	if (bEnable)
	{
		static int state = 0;
		static DWORD Time = 0;

		if (!fireveh)
		{
			fireveh = 1;
		}

		struct vehicle_info* vinfo = vehicle_info_get(VEHICLE_SELF, VEHICLE_ALIVE);

		if (!vinfo)
			return;

		vinfo->hitpoints = 200;

		if (GetTickCount() - 200 > Time)
		{
			vinfo->hitpoints = 1000;
			Time = GetTickCount();
		}
	}
	else
	{
		if (fireveh)
		{
			fireveh = 0;
			pPedSelf->GetVehicle()->Fix();
		}
	}
}

void CNewMods::PizdarvankaTargetTroll(void)
{
	traceLastFunc("PizdarvankaTargetTroll()");

	if (!g_SAMP || !g_Players)
		return;

	if (gta_menu_active())
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (BlackLightFuncs->bPizdarvankaTarget[set.BlackLight.Target.TargetPlayer_Pizdarvanka])
	{
		if (pSampMulti->IsPlayerInCar(set.BlackLight.Target.TargetPlayer_Pizdarvanka) && !pSampMulti->IsOurPlayerInCar()) //cus car wont be slapped by our body
		{
			BlackLightFuncs->bPizdarvankaTarget[set.BlackLight.Target.TargetPlayer_Pizdarvanka] = false;
			addMessageToChatWindow("Player is in vehicle, and you're not, deactivating...");
			return;
		}
		else if (BlackLightFuncs->bIsFriend[set.BlackLight.Target.TargetPlayer_Pizdarvanka])
		{
			BlackLightFuncs->bPizdarvankaTarget[set.BlackLight.Target.TargetPlayer_Pizdarvanka] = false;
			addMessageToChatWindow("Can't troll friends");
			return;
		}
		else if (pSampMulti->IsModDeveloper(set.BlackLight.Target.TargetPlayer_Pizdarvanka))
		{
			BlackLightFuncs->bPizdarvankaTarget[set.BlackLight.Target.TargetPlayer_Pizdarvanka] = false;
			//pSampMulti->PlayerRestoreAnim();
			addMessageToChatWindow("Trolls won't work on mod developer. Actually you're gonna be trolled.");
			say("I am gay!!");
			pPedSelf->SetHealth(0.0);
			return;
		}

		if (pSampMulti->IsPlayerStreamed(set.BlackLight.Target.TargetPlayer_Pizdarvanka) && !pSampMulti->IsOurPlayerInCar())
		{
			struct actor_info* pinfo = getGTAPedFromSAMPPlayerID(set.BlackLight.Target.TargetPlayer_Pizdarvanka);
			/*stOnFootData OnFoot;
			memset(&OnFoot, 0, sizeof(stOnFootData));
			vect3_copy(&g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_Pizdarvanka]->pPlayerData->pSAMP_Actor->pGTA_Ped->base.matrix[12], OnFoot.fPosition);

			OnFoot.fPosition[1] += 0.25f;
			OnFoot.fMoveSpeed[0] = -(10.0f + (rand() % 999) / 100.0f);
			OnFoot.fMoveSpeed[1] = 0;
			OnFoot.fMoveSpeed[2] = 0.5f;

			BitStream bsOnFoot;
			bsOnFoot.Write((BYTE)ID_PLAYER_SYNC);
			bsOnFoot.Write((PCHAR)&OnFoot, sizeof(stOnFootData));
			g_RakClient2->Send(&bsOnFoot, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);*/

			if (pinfo)
			{
				static float fSpeed[3], fpos[3];
				vect3_copy(&g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_Pizdarvanka]->pPlayerData->pSAMP_Actor->pGTA_Ped->base.matrix[12], fpos);
			
				fpos[2] += 0.9f;

				fSpeed[0] = (float)-4 + rand() % (4 * 2);
				fSpeed[1] = (float)-4 + rand() % (4 * 2);
				fSpeed[2] = (float)-4 + rand() % (4 * 2);
				g_RakClient2->SendFakeOnfootSyncData(fpos, fSpeed, 
					g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->hitpoints, 
					g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->armor, 
					g_Players->pLocalPlayer->sAnimFlags = ANIM_FLAG_ID_MOSTLY_USED,
					g_Players->pLocalPlayer->sCurrentAnimID = 970, 
					g_Players->pLocalPlayer->onFootData.fQuaternion);
			}
			else
			{
				BlackLightFuncs->bPizdarvankaTarget[set.BlackLight.Target.TargetPlayer_Pizdarvanka] = false;
				addMessageToChatWindow("Target data invalid/player not found, deactivating.");
			}
		}
		else if (pSampMulti->IsPlayerStreamed(set.BlackLight.Target.TargetPlayer_Pizdarvanka) && pSampMulti->IsPlayerDriver(g_Players->sLocalPlayerID))
		{
			/*stInCarData InCar;
			memset(&InCar, 0, sizeof(stInCarData));
			vect3_copy(&g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_Pizdarvanka]->pPlayerData->pSAMP_Actor->pGTA_Ped->base.matrix[12], InCar.fPosition);

			InCar.fPosition[1] += 0.25f;
			InCar.fMoveSpeed[0] = -(10.0f + (rand() % 999) / 100.0f);
			InCar.fMoveSpeed[1] = 0;
			InCar.fMoveSpeed[2] = 0.5f;

			BitStream bsInCar;
			bsInCar.Write((BYTE)ID_VEHICLE_SYNC);
			bsInCar.Write((PCHAR)&InCar, sizeof(stInCarData));
			g_RakClient2->Send(&bsInCar, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);*/

			struct vehicle_info* vinfo = vehicle_info_get(VEHICLE_SELF, VEHICLE_ALIVE);

			if (vinfo)
			{
				static float fPos[3];
				static float fSpeed[3];

				vect3_copy(&g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_Pizdarvanka]->pPlayerData->pSAMP_Actor->pGTA_Ped->base.matrix[4 * 3], fPos);
				fPos[2] += 0.9f;

				fSpeed[0] = (float)-4 + rand() % (4 * 2);
				fSpeed[1] = (float)-4 + rand() % (4 * 2);
				fSpeed[2] = (float)-4 + rand() % (4 * 2);
				g_RakClient2->SendFakeDriverSyncData_H3X(getSAMPVehicleIDFromGTAVehicle(vinfo), fPos, vinfo->hitpoints, fSpeed, NULL);
			}
			else
			{
				BlackLightFuncs->bPizdarvankaTarget[set.BlackLight.Target.TargetPlayer_Pizdarvanka] = false;
				addMessageToChatWindow("Vehicle data not found, deactivating.");
			}
		}
	}
}

void CNewMods::PizdarvankaPlayer(bool bEnable)
{
	if (!bEnable || IS_CHEAT_PANIC_ACTIVE)
		return;

	if (bEnable)
	{
		struct actor_info* me = actor_info_get(ACTOR_SELF, 0);

		static float fPos[3];
		static float fSpeed[3];

		if (me)
		{
			if (!pSampMulti->IsOurPlayerInCar())
			{
				vect3_copy(&me->base.matrix[4 * 3], fPos);
				fPos[2] += 0.9f;

				fSpeed[0] = (float)-set.BlackLight.pizdarvanka_player_speed + rand() % (set.BlackLight.pizdarvanka_player_speed * 2);
				fSpeed[1] = (float)-set.BlackLight.pizdarvanka_player_speed + rand() % (set.BlackLight.pizdarvanka_player_speed * 2);
				fSpeed[2] = (float)-set.BlackLight.pizdarvanka_player_speed + rand() % (set.BlackLight.pizdarvanka_player_speed * 2);
				g_RakClient2->SendFakeOnfootSyncData(fPos, fSpeed, me->hitpoints, me->armor, g_Players->pLocalPlayer->sAnimFlags = ANIM_FLAG_ID_MOSTLY_USED, g_Players->pLocalPlayer->sCurrentAnimID = 970, g_Players->pLocalPlayer->onFootData.fQuaternion);
			}
		}
	}
}

void CNewMods::PizdarvankaVehicle(bool bEnable)
{
	if (!bEnable || IS_CHEAT_PANIC_ACTIVE)
		return;

	traceLastFunc("PizdarvankaVehicle()");

	if (bEnable)
	{
		struct vehicle_info* vehicle = vehicle_info_get(VEHICLE_SELF, 0);

		float fPos[3];
		float fSpeed[3];

		if (vehicle)
		{
			vect3_copy(&vehicle->base.matrix[4 * 3], fPos);
			fPos[2] += 0.9f;

			fSpeed[0] = (float)-set.BlackLight.pizdarvanka_vehicle_speed + rand() % (set.BlackLight.pizdarvanka_vehicle_speed * 2);
			fSpeed[1] = (float)-set.BlackLight.pizdarvanka_vehicle_speed + rand() % (set.BlackLight.pizdarvanka_vehicle_speed * 2);
			fSpeed[2] = (float)-set.BlackLight.pizdarvanka_vehicle_speed + rand() % (set.BlackLight.pizdarvanka_vehicle_speed * 2);
			g_RakClient2->SendFakeDriverSyncData_H3X(getSAMPVehicleIDFromGTAVehicle(vehicle), fPos, vehicle->hitpoints, fSpeed, NULL);
		}
	}
}

void CNewMods::FakeKillFlood(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("FakeKillFlood()");

	if (!g_SAMP)
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	g_RakClient2->SendDeath(g_Players->sLocalPlayerID, DEATH_REASON_HELICOPTER_BLADE);
}

void CNewMods::FakeBulletsFlood(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("FakeBulletsFlood()");

	if (!g_SAMP || !g_Players)
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	static float fTarget[3], fCenter[3];
	static DWORD tick = 0;
	DWORD tickc = GetTickCount();
	for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
	{
		if (g_Players->iIsListed[players] != 1)
			continue;

		actor_info* tinfo = getGTAPedFromSAMPPlayerID(players);

		if (!tinfo)
			continue;

		vect3_copy(&tinfo->base.matrix[4 * 3], fTarget);

		fCenter[0] = -0.01f;
		fCenter[1] = -0.02f;
		fCenter[2] = 0.04f;

		if (tickc - tick > 150)
		{
			tick = tickc;
			for(int amount = 0; amount < 3; amount++)
			g_RakClient2->SendFakeBulletData(players, &g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->base.matrix[4 * 3], fTarget, fCenter, pSampMulti->IsPlayerInCar(players) ? pSampMulti->getPlayerWeaponModelID(g_Players->sLocalPlayerID) : pPedSelf->GetWeapon(pPedSelf->GetCurrentWeaponSlot())->GetType(), pSampMulti->IsPlayerInCar(players) ? BULLET_HIT_TYPE_VEHICLE : BULLET_HIT_TYPE_PLAYER);
		}
	}
}


void CNewMods::SkinChanger(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("SkinChange()");

	if (!g_SAMP)
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (pSampMulti->IsOurPlayerInCar())
		return;

	if (bEnable)
	{
		for (int iSkin = 0; iSkin < 5; iSkin++)
		{
			if (!pGameInterface->GetStreaming()->HasModelLoaded(iSkin))
			{
				pGameInterface->GetStreaming()->RequestModel(iSkin, 0);
				pGameInterface->GetStreaming()->LoadAllRequestedModels(false);
				pPedSelf->SetModelIndex((unsigned long)iSkin);
			}
		}
	}
}

void CNewMods::VehicleWheelsVisibilityPulsator(bool bEnable)
{
	if (!bEnable)
		return;
	traceLastFunc("VehicleWheelsVisibilityPulsator()");

	if (!g_SAMP)
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (!pPedSelf->GetVehicle())
		return;

	if (bEnable)
	{
		static DWORD tickt = 0;
		DWORD count = GetTickCount();
		static bool bswitch = false;
			if (!bswitch)
			{
				if (count - tickt > 1000)
				    bswitch = true;
				pPedSelf->GetVehicle()->SetWheelVisibility(eWheels::FRONT_LEFT_WHEEL, false);
				pPedSelf->GetVehicle()->SetWheelVisibility(eWheels::REAR_LEFT_WHEEL, false);
				pPedSelf->GetVehicle()->SetWheelVisibility(eWheels::FRONT_RIGHT_WHEEL, true);
				pPedSelf->GetVehicle()->SetWheelVisibility(eWheels::REAR_RIGHT_WHEEL, true);
			}
			else if (bswitch)
			{
				if (count - tickt > 2000)
				{
					bswitch = false;
					tickt = count;
				}
				pPedSelf->GetVehicle()->SetWheelVisibility(eWheels::FRONT_LEFT_WHEEL, true);
				pPedSelf->GetVehicle()->SetWheelVisibility(eWheels::REAR_LEFT_WHEEL, true);
				pPedSelf->GetVehicle()->SetWheelVisibility(eWheels::FRONT_RIGHT_WHEEL, false);
				pPedSelf->GetVehicle()->SetWheelVisibility(eWheels::REAR_RIGHT_WHEEL, false);
			}
		
	}
	else
	{
		pPedSelf->GetVehicle()->SetWheelVisibility(eWheels::FRONT_LEFT_WHEEL, true);
		pPedSelf->GetVehicle()->SetWheelVisibility(eWheels::REAR_LEFT_WHEEL, true);
		pPedSelf->GetVehicle()->SetWheelVisibility(eWheels::FRONT_RIGHT_WHEEL, true);
		pPedSelf->GetVehicle()->SetWheelVisibility(eWheels::REAR_RIGHT_WHEEL, true);
	}
}

void CNewMods::CarHardFlip(bool bEnable)
{
	if (!bEnable)
		return;

	if (!pPedSelf)
		return;

	if (!pSampMulti->IsPlayerDriver(g_Players->sLocalPlayerID))
		return;

	if (pPedSelf->GetVehicle()->GetHealth() == 0.0f)
		return;

	vehicle_info* veh = vehicle_info_get(VEHICLE_SELF, VEHICLE_ALIVE);
	if (!veh) return;

	if (bEnable)
	{
		if (pSampMulti->IsMenuActive())
			return;

		// Inter-frame timing info from the game
		float fTimeStep = *(float*)0xB7CB5C;

		// Get our vehicle and matrix
		CVehicle* cveh = getSelfCVehicle();
		CVector cvehGrav;
		CMatrix cvehMatrix;
		CVector vZero(0.0f, 0.0f, 6.0f);

		// Save initial X and Y position of the vehicle
		float initialX = cveh->GetPosition()->fX;
		float initialY = cveh->GetPosition()->fY;

		if (KEY_DOWN(0x02))  // Right mouse button
		{
			// Keep X and Y frozen and only rotate around Z
			cveh->GetPosition()->fX = initialX;  // Freeze X
			cveh->GetPosition()->fY = initialY;  // Freeze Y

			// Set a constant rotation speed around Z axis
			float theta = M_PI / (150.0f / fTimeStep);  // Adjust this value for rotation speed

			vehicle_prevent_below_height(veh, 0.1f);

			// Get the vehicle's current matrix and apply the rotation on the Z-axis
			cveh->GetMatrix(&cvehMatrix);
			cvehMatrix = cvehMatrix.Rotate(&cvehMatrix.vUp, theta);  // Rotate around Z-axis

			// Apply the new matrix to the vehicle
			cveh->SetMatrix(&cvehMatrix);

			// Set turn speed to zero (so the vehicle doesn't drift)
			cveh->SetTurnSpeed(&vZero);
		}
	}
}



void CNewMods::NoStun(bool bEnable)
{
	if (!bEnable)
		return;

	if (!g_SAMP)
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (pSampMulti->IsOurPlayerInCar())
		return;

	static bool nofreez = false;
	if (bEnable)
	{
		if (!nofreez)
		{
			nofreez = 1;
		}

		if (g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1087 || g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1177 ||
			g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1078 || g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1073 ||
			g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1079 || g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1077 ||
			g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1081 || g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1075 ||
			g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1084 || g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1083 ||
			g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1085 || g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1083 ||
			g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1086 || g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1072 ||
			g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1175 || g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1076 ||
			g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1074 || g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1082 ||
			g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1173 || g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1178 ||
			g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1087 || g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 1078
			)
			GTAfunc_DisembarkInstantly();
	}
	else
	{
		if (nofreez)
		{
			nofreez = 0;
		}
	}
}

struct freeze_info
{
	bool setfreeze;
	float	 matrix[16];
};


void CNewMods::FreezePlayers(bool bEnable)
{
	if (!g_Players) return;

	static freeze_info* freeze = NULL;

	if (!bEnable)
	{
		if (freeze)
		{
			for (int i = 0; i < pool_actor->size; i++)
				freeze[i].setfreeze = 0;
		}
		return;
	}

	if (!freeze)
	{
		freeze = (freeze_info*)malloc(pool_actor->size * sizeof(freeze_info));
		memset(freeze, 0, pool_actor->size * sizeof(freeze_info));
	}

	actor_info* self = actor_info_get(ACTOR_SELF, ACTOR_ALIVE);
	if (!self) return;

	float* pos = &self->base.matrix[4 * 3];

	for (int i = 0; i < pool_actor->size; i++)
	{
		actor_info* info = actor_info_get(i, VEHICLE_ALIVE);
		if (!info || info == self)
		{
			freeze[i].setfreeze = 0;
			continue;
		}

		// Too far? don't freeze
		if (vect3_dist(pos, &info->base.matrix[12]) >= 80.0f)
		{
			freeze[i].setfreeze = 0;
			continue;
		}

		// Always refresh the frozen position
		matrix_copy(info->base.matrix, freeze[i].matrix);
		freeze[i].setfreeze = 1;

		// Apply freeze
		matrix_copy(freeze[i].matrix, info->base.matrix);
		vect3_zero(info->speed);
		vect3_zero(info->spin);
	}
}


void CNewMods::RainbowVehicle(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("RainbowVehicle()");

	if (!g_SAMP)
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (!pPedSelf)
		return;

	if (!pSampMulti->IsOurPlayerInCar())
		return;

	static float rainbow_color_car, misc;	//speed
	DWORD rainbow_color_x_car;
	rainbow_color_car += misc = 0.0001 * set.BlackLight.Rainbow.rainbow_car_speed;
	if (rainbow_color_car > 1.f) rainbow_color_car = 0.f;
	if (BlackLightFuncs->bCustomRainbowColorSet)
		rainbow_color_x_car = D3DCOLOR_ARGB(254, (BYTE)(custom_color_car.z * 255), (BYTE)(custom_color_car.y * 255), (BYTE)(custom_color_car.x * 255));
	else if (!BlackLightFuncs->bCustomRainbowColorSet)
		rainbow_color_x_car = Color2::ToImColor(Color2::FromHSB(rainbow_color_car, 1.f, 1.f));
	ImVec4 VehicleColorActive = Color2::ToImColor(Color2::FromHSB(rainbow_color_car, 1.f, 1.f));

	if (*(DWORD*)0xBA18FC > 0)
	{
		*(byte*)(*(DWORD*)0xBA18FC + 1076) = 255;
		*(byte*)(*(DWORD*)0xBA18FC + 1077) = 255;
		*(byte*)(*(DWORD*)0xBA18FC + 1078) = 255;
		*(byte*)(*(DWORD*)0xBA18FC + 1079) = 255;
		*(DWORD*)((g_dwSAMP_Addr + 0x215AC8) + 4 * 255) = rainbow_color_x_car;

	}
}

void CNewMods::RainbowSky(bool bEnable)
{
	traceLastFunc("RainbowSky()");

	if (!g_SAMP)
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	static bool bon = false;
	if (bEnable)
	{
		if (!bon)
			bon = true;
		//by CreativeAdv
		if (!BlackLightFuncs->bCustomRainbowColorSet)
		{
			static float rainbow_color_sky, misc;	//speed
			DWORD rainbow_color_x_sky;
			rainbow_color_sky += misc = 0.0001 * set.BlackLight.Rainbow.rainbow_sky_speed;
			if (rainbow_color_sky > 1.f) rainbow_color_sky = 0.f;
			rainbow_color_x_sky = Color2::ToImColor(Color2::FromHSB(rainbow_color_sky, 1.f, 1.f));
			ImVec4 SkyColorActive = Color2::ToImColor(Color2::FromHSB(rainbow_color_sky, 1.f, 1.f));

			*(byte*)(0x561760) = 0xC3;
			*(byte*)(0xB7C4C4) = (BYTE)round(SkyColorActive.x * 255.f);
			*(byte*)(0xB7C4C6) = (BYTE)round(SkyColorActive.y * 255.f);
			*(byte*)(0xB7C4C8) = (BYTE)round(SkyColorActive.z * 255.f);

			*(byte*)(0xB7C4CA) = (BYTE)round(SkyColorActive.x * 255.f);
			*(byte*)(0xB7C4CC) = (BYTE)round(SkyColorActive.y * 255.f);
			*(byte*)(0xB7C4CE) = (BYTE)round(SkyColorActive.z * 255.f);
		}
		else if (BlackLightFuncs->bCustomRainbowColorSet)
		{
			*(byte*)(0x561760) = 0xC3;
			*(byte*)(0xB7C4C4) = (BYTE)round(custom_color_rb_sky.x * 255.f);
			*(byte*)(0xB7C4C6) = (BYTE)round(custom_color_rb_sky.y * 255.f);
			*(byte*)(0xB7C4C8) = (BYTE)round(custom_color_rb_sky.z * 255.f);

			*(byte*)(0xB7C4CA) = (BYTE)round(custom_color_rb_sky.x * 255.f);
			*(byte*)(0xB7C4CC) = (BYTE)round(custom_color_rb_sky.y * 255.f);
			*(byte*)(0xB7C4CE) = (BYTE)round(custom_color_rb_sky.z * 255.f);
		}
	}
	else if (!bEnable)
	{
		if (bon)
		{
			*(BYTE*)0x561760 = 0xA1;
			*(BYTE*)0xB7C4C4 = 0xA1;
			*(BYTE*)0xB7C4C6 = 0xA1;
			*(BYTE*)0xB7C4C8 = 0xA1;

			*(BYTE*)0xB7C4CA = 0xA1;
			*(BYTE*)0xB7C4CC = 0xA1;
			*(BYTE*)0xB7C4CE = 0xA1;
			bon = false;
		}
	}
}
void CNewMods::RainbowChat(bool bEnable)
{
	if (!g_SAMP || !g_Chat || IS_CHEAT_PANIC_ACTIVE)
		return;

	static bool active = false;
	static DWORD origText, origPrefix, origDebug;

	if (bEnable)
	{
		if (!active)
		{
			// save original colors ONCE
			origText = g_Chat->clTextColor;
			origPrefix = g_Chat->chatEntry->clPrefixColor;
			origDebug = g_Chat->clDebugColor;
			active = true;
		}

		// rainbow progress
		static float t = 0.0f;
		t += 0.0005f * set.BlackLight.Rainbow.rainbow_chat_speed;
		if (t > 1.f) t = 0.f;

		// rainbow color (HSB → RGB)
		ImVec4 c = Color2::ToImColor(Color2::FromHSB(t, 1.f, 1.f));

		// convert ImGui RGBA → SAMP ABGR 
		DWORD sampColor =
			(BYTE)(c.w * 255) << 24 |
			(BYTE)(c.x * 255) << 0 |
			(BYTE)(c.y * 255) << 8 |
			(BYTE)(c.z * 255) << 16;

		// WRITE DIRECTLY WHERE IT ACTUALLY WORKS
		g_Chat->clTextColor = sampColor;
		g_Chat->chatEntry->clTextColor = sampColor;
		g_Chat->chatEntry->clPrefixColor = sampColor;
		g_Chat->clDebugColor = sampColor;

		return;
	}

	// DISABLING
	if (active)
	{
		// restore original colors
		g_Chat->clTextColor = origText;
		g_Chat->chatEntry->clPrefixColor = origPrefix;
		g_Chat->chatEntry->clTextColor = origText;
		g_Chat->clDebugColor = origDebug;
		active = false;
	}
}


void CNewMods::RainbowHud(bool bEnable)
{
	traceLastFunc("RainbowHud()");
	if (!g_SAMP || !pGameInterface || IS_CHEAT_PANIC_ACTIVE) return;

	static bool rainbowActive = false;

	// list of all HUD color addresses
	static DWORD* hudColors[] = {
		(DWORD*)0xBAB22C, (DWORD*)0xBAB230, (DWORD*)0xBAB234, (DWORD*)0xBAB238,
		(DWORD*)0xBAB23C, (DWORD*)0xBAB240, (DWORD*)0xBAB244, (DWORD*)0xBAB248,
		(DWORD*)0xBAB24C, (DWORD*)0xBAB250, (DWORD*)0xBAB254, (DWORD*)0xBAB258,
		(DWORD*)0xBAB25C, (DWORD*)0xBAB260
	};
	const int HUD_COUNT = sizeof(hudColors) / sizeof(hudColors[0]);

	static DWORD originalHud[64];  // enough room
	static BYTE originalRedHL, originalGreenHL, originalBlueHL;
	static BYTE originalRedAR, originalGreenAR, originalBlueAR;
	static bool savedOriginal = false;

	//----------------------------------------------------------
	// ENABLE RAINBOW
	//----------------------------------------------------------
	if (bEnable)
	{
		if (!rainbowActive)
		{
			// save original HUD colors ONCE
			if (!savedOriginal)
			{
				for (int i = 0; i < HUD_COUNT; i++)
					originalHud[i] = *hudColors[i];

				originalRedHL = *(BYTE*)(0x58D973 + 1);
				originalGreenHL = *(BYTE*)(0x58D96E + 1);
				originalBlueHL = *(BYTE*)(0x58D969 + 1);

				originalRedAR = *(BYTE*)(0x58D8AF + 1);
				originalGreenAR = *(BYTE*)(0x58D8AA + 1);
				originalBlueAR = *(BYTE*)(0x58D89F + 1);

				savedOriginal = true;
			}
			rainbowActive = true;
		}

		//------------------------------------------------------
		// rainbow logic
		//------------------------------------------------------
		static float t = 0.0f;
		t += 0.0001f * set.BlackLight.Rainbow.rainbow_hud_speed;
		if (t > 1.f) t = 0.f;

		ImVec4 c;
		DWORD dcolor;

		if (BlackLightFuncs->bCustomRainbowColorSet)
		{
			c = ImVec4(
				custom_color_hud.x,
				custom_color_hud.y,
				custom_color_hud.z,
				1.0f
			);
		}
		else
		{
			c = Color2::ToImColor(Color2::FromHSB(t, 1.f, 1.f));
		}

		dcolor = D3DCOLOR_ARGB(255, (BYTE)(c.x * 255), (BYTE)(c.y * 255), (BYTE)(c.z * 255));

		// write to all HUD color addresses
		for (int i = 0; i < HUD_COUNT; i++)
			*hudColors[i] = dcolor;

		// write HUD highlight colors
		*(BYTE*)(0x58D973 + 1) = (BYTE)(c.x * 255);
		*(BYTE*)(0x58D96E + 1) = (BYTE)(c.y * 255);
		*(BYTE*)(0x58D969 + 1) = (BYTE)(c.z * 255);

		*(BYTE*)(0x58D8AF + 1) = (BYTE)(c.x * 255);
		*(BYTE*)(0x58D8AA + 1) = (BYTE)(c.y * 255);
		*(BYTE*)(0x58D89F + 1) = (BYTE)(c.z * 255);

		// SAMP textdraws
		for (int i = 0; i < SAMP_MAX_TEXTDRAWS; i++)
		{
			if (!g_SAMP->pPools->pTextdraw->textdraw[i]) continue;
			g_SAMP->pPools->pTextdraw->textdraw[i]->dwLetterColor = dcolor;
			g_SAMP->pPools->pTextdraw->textdraw[i]->dwBoxColor = dcolor;
		}
		for (int i = 0; i < SAMP_MAX_PLAYERTEXTDRAWS; i++)
		{
			if (!g_SAMP->pPools->pTextdraw->playerTextdraw[i]) continue;
			g_SAMP->pPools->pTextdraw->playerTextdraw[i]->dwLetterColor = dcolor;
			g_SAMP->pPools->pTextdraw->playerTextdraw[i]->dwBoxColor = dcolor;
		}

		return;
	}

	//----------------------------------------------------------
	// DISABLE RAINBOW → RESTORE ORIGINAL
	//----------------------------------------------------------
	if (rainbowActive)
	{
		if (savedOriginal)
		{
			// restore HUD colors
			for (int i = 0; i < HUD_COUNT; i++)
				*hudColors[i] = originalHud[i];

			// restore highlight colors
			*(BYTE*)(0x58D973 + 1) = originalRedHL;
			*(BYTE*)(0x58D96E + 1) = originalGreenHL;
			*(BYTE*)(0x58D969 + 1) = originalBlueHL;

			*(BYTE*)(0x58D8AF + 1) = originalRedAR;
			*(BYTE*)(0x58D8AA + 1) = originalGreenAR;
			*(BYTE*)(0x58D89F + 1) = originalBlueAR;
		}

		rainbowActive = false;
	}
}


void CNewMods::RainbowCrosshair(bool bEnable)
{

	traceLastFunc("RainbowCrosshair()");
	if (!g_SAMP)
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	static bool bon = false;
	if (bEnable)
	{
		if (!bon)
			bon = true;

		static float rainbow_color_crosshair, misc;	//speed
		DWORD rainbow_color_x_crosshair;
		rainbow_color_crosshair += misc = 0.0001 * set.BlackLight.Rainbow.rainbow_crosshair_speed;
		if (rainbow_color_crosshair > 1.f) rainbow_color_crosshair = 0.f;
		if (BlackLightFuncs->bCustomRainbowColorSet)
			rainbow_color_x_crosshair = D3DCOLOR_ARGB(254, (BYTE)(custom_color_crosshair.z * 255), (BYTE)(custom_color_crosshair.y * 255), (BYTE)(custom_color_crosshair.x * 255));
		else if (!BlackLightFuncs->bCustomRainbowColorSet)
			rainbow_color_x_crosshair = Color2::ToImColor(Color2::FromHSB(rainbow_color_crosshair, 1.f, 1.f));
		ImVec4 CrosshairColorActive = Color2::ToImColor(Color2::FromHSB(rainbow_color_crosshair, 1.f, 1.f));

		*(BYTE*)0x58E47C = (BYTE)round(CrosshairColorActive.x * 255.f);
		*(BYTE*)0x58E473 = (BYTE)round(CrosshairColorActive.y * 255.f);
		*(BYTE*)0x58E466 = (BYTE)round(CrosshairColorActive.z * 255.f);
		*(BYTE*)(0x58E432 + 1) = (BYTE)round(CrosshairColorActive.x * 255.f);
		*(BYTE*)(0x58E429 + 1) = (BYTE)round(CrosshairColorActive.y * 255.f);
		*(BYTE*)(0x58E424 + 1) = (BYTE)round(CrosshairColorActive.z * 255.f);
		*(BYTE*)(0x58E3D9 + 1) = (BYTE)round(CrosshairColorActive.x * 255.f);
		*(BYTE*)(0x58E3D0 + 1) = (BYTE)round(CrosshairColorActive.y * 255.f);
		*(BYTE*)(0x58E3C7 + 1) = (BYTE)round(CrosshairColorActive.z * 255.f);
		*(BYTE*)(0x58E300 + 1) = (BYTE)round(CrosshairColorActive.x * 255.f);
		*(BYTE*)(0x58E2F5 + 1) = (BYTE)round(CrosshairColorActive.y * 255.f);
		*(BYTE*)(0x58E2F0 + 1) = (BYTE)round(CrosshairColorActive.z * 255.f);
	}
	else
	{
		if (bon)
		{
			*(BYTE*)0x58E47C = 0xA1;
			*(BYTE*)0x58E473 = 0xA1;
			*(BYTE*)0x58E466 = 0xA1;
			*(BYTE*)(0x58E432 + 1) = 0xA1;
			*(BYTE*)(0x58E429 + 1) = 0xA1;
			*(BYTE*)(0x58E424 + 1) = 0xA1;
			*(BYTE*)(0x58E3D9 + 1) = 0xA1;
			*(BYTE*)(0x58E3D0 + 1) = 0xA1;
			*(BYTE*)(0x58E3C7 + 1) = 0xA1;
			*(BYTE*)(0x58E300 + 1) = 0xA1;
			*(BYTE*)(0x58E2F5 + 1) = 0xA1;
			*(BYTE*)(0x58E2F0 + 1) = 0xA1;
			bon = false;
		}
	}
}

void CNewMods::RainbowCarLights(bool bEnable)
{
	traceLastFunc("RainbowCarLight()");

	if (!g_SAMP)
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	static bool bon = false;
	if (bEnable)
	{
		if (!bon)
			bon = true;
		static float rainbow_color_carlights, misc;	//speed
		DWORD rainbow_color_x_carlights;
		rainbow_color_carlights += misc = 0.0001 * set.BlackLight.Rainbow.rainbow_carlights_speed;
		if (rainbow_color_carlights > 1.f) rainbow_color_carlights = 0.f;
		if (BlackLightFuncs->bCustomRainbowColorSet)
			rainbow_color_x_carlights = D3DCOLOR_ARGB(254, (BYTE)(custom_color_carLights.z * 255), (BYTE)(custom_color_carLights.y * 255), (BYTE)(custom_color_carLights.x * 255));
		else if (!BlackLightFuncs->bCustomRainbowColorSet)
			rainbow_color_x_carlights = Color2::ToImColor(Color2::FromHSB(rainbow_color_carlights, 1.f, 1.f));
		ImVec4 carLightColorActive = Color2::ToImColor(Color2::FromHSB(rainbow_color_carlights, 1.f, 1.f));

		*(byte*)(0x6E155F + 1) = (BYTE)round(carLightColorActive.x * 255.f);
		*(byte*)(0x6E155D + 1) = (BYTE)round(carLightColorActive.y * 255.f);
		*(byte*)(0x6E1555 + 1) = (BYTE)round(carLightColorActive.z * 255.f);

		*(byte*)(0x6E1688 + 1) = (BYTE)round(carLightColorActive.x * 255.f);
		*(byte*)(0x6E1686 + 1) = (BYTE)round(carLightColorActive.y * 255.f);
		*(byte*)(0x6E1684 + 1) = (BYTE)round(carLightColorActive.z * 255.f);
	}
	else
	{
		if (bon)
		{
			*(byte*)(0x6E155F + 1) = 0xA1;
			*(byte*)(0x6E155D + 1) = 0xA1;
			*(byte*)(0x6E1555 + 1) = 0xA1;

			*(byte*)(0x6E1688 + 1) = 0xA1;
			*(byte*)(0x6E1686 + 1) = 0xA1;
			*(byte*)(0x6E1684 + 1) = 0xA1;
			bon = false;
		}
	}
}

void CNewMods::RainbowDeathList(bool bEnable)
{
	traceLastFunc("RainbowDeathList()");

	if (!g_SAMP)
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	static bool bon = false;
	if (bEnable)
	{
		if (!bon)
			bon = true;

		static float rainbow_color_killlist, misc;	//speed
		DWORD rainbow_color_x_killlist;
		rainbow_color_killlist += misc = 0.0001 * set.BlackLight.Rainbow.rainbow_KillList_speed;
		if (rainbow_color_killlist > 1.f) rainbow_color_killlist = 0.f;
		if (BlackLightFuncs->bCustomRainbowColorSet)
			rainbow_color_x_killlist = D3DCOLOR_ARGB(254, (BYTE)(custom_color_killList.z * 255), (BYTE)(custom_color_killList.y * 255), (BYTE)(custom_color_killList.x * 255));
		else if (!BlackLightFuncs->bCustomRainbowColorSet)
			rainbow_color_x_killlist = Color2::ToImColor(Color2::FromHSB(rainbow_color_killlist, 1.f, 1.f));
		ImVec4 killListColorActive = Color2::ToImColor(Color2::FromHSB(rainbow_color_killlist, 1.f, 1.f));

		for (int kill = 0; kill < 5; kill++)
		{
			struct stKillEntry* ent = &g_DeathList->killEntry[kill];
			ent->clKillerColor = D3DCOLOR_ARGB(255, (BYTE)round(killListColorActive.x * 255.f), (BYTE)round(killListColorActive.y * 255.f), (BYTE)round(killListColorActive.z * 255.f));
			ent->clVictimColor = D3DCOLOR_ARGB(255, (BYTE)round(killListColorActive.x * 255.f), (BYTE)round(killListColorActive.y * 255.f), (BYTE)round(killListColorActive.z * 255.f));
		}
	}
	else
	{
		if (bon)
		{
			for (int kill = 0; kill < 5; kill++)
			{
				struct stKillEntry* ent = &g_DeathList->killEntry[kill];
				ent->clKillerColor = D3DCOLOR_ARGB(255, (BYTE)0xA1, (BYTE)0xA1, (BYTE)0xA1);
				ent->clVictimColor = D3DCOLOR_ARGB(255, (BYTE)0xA1, (BYTE)0xA1, (BYTE)0xA1);
			}
			bon = false;
		}
	}
}

void CNewMods::RainbowRadar(bool bEnable)
{
	traceLastFunc("RainbowRadar()");

	if (!g_SAMP)
		return;

	if (!pGameInterface)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	static bool bon = false;
	if (bEnable)
	{
		if (!bon)
			bon = true;
		static float rainbow_color_radar, misc;	//speed
		DWORD rainbow_color_x_radar;
		rainbow_color_radar += misc = 0.0001 * set.BlackLight.Rainbow.rainbow_radar_speed;
		if (rainbow_color_radar > 1.f) rainbow_color_radar = 0.f;
		if (BlackLightFuncs->bCustomRainbowColorSet)
			rainbow_color_x_radar = D3DCOLOR_ARGB(254, (BYTE)(custom_color_radar.z * 255), (BYTE)(custom_color_radar.y * 255), (BYTE)(custom_color_radar.x * 255));
		else if (!BlackLightFuncs->bCustomRainbowColorSet)
			rainbow_color_x_radar = Color2::ToImColor(Color2::FromHSB(rainbow_color_radar, 1.f, 1.f));
		ImVec4 RadarColorActive = Color2::ToImColor(Color2::FromHSB(rainbow_color_radar, 1.f, 1.f));

		*(BYTE*)(0x58A9A2) = (BYTE)round(RadarColorActive.x * 255.f);
		*(BYTE*)(0x58A99A) = (BYTE)round(RadarColorActive.y * 255.f);
		*(BYTE*)(0x58A996) = (BYTE)round(RadarColorActive.z * 240.f);
		*(BYTE*)(0x58A8EE) = (BYTE)round(RadarColorActive.x * 255.f);
		*(BYTE*)(0x58A8E6) = (BYTE)round(RadarColorActive.y * 255.f);
		*(BYTE*)(0x58A8DE) = (BYTE)round(RadarColorActive.z * 255.f);
		*(BYTE*)(0x58A89A) = (BYTE)round(RadarColorActive.x * 255.f);
		*(BYTE*)(0x58A896) = (BYTE)round(RadarColorActive.y * 255.f);
		*(BYTE*)(0x58A894) = (BYTE)round(RadarColorActive.z * 255.f);
		*(BYTE*)(0x58A798) = (BYTE)round(RadarColorActive.x * 255.f);
		*(BYTE*)(0x58A790) = (BYTE)round(RadarColorActive.y * 255.f);
		*(BYTE*)(0x58A78E) = (BYTE)round(RadarColorActive.z * 255.f);
		*(BYTE*)(0x582F20) = (BYTE)round(RadarColorActive.x * 255.f);
		*(BYTE*)(0x582F1A) = (BYTE)round(RadarColorActive.y * 255.f);
		*(BYTE*)(0x582F18) = (BYTE)round(RadarColorActive.z * 255.f);
		*(BYTE*)(0x588690 + 1) = (BYTE)round(RadarColorActive.x * 255.f);
		*(BYTE*)(0x58868B + 1) = (BYTE)round(RadarColorActive.y * 255.f);
		*(BYTE*)(0x588686 + 1) = (BYTE)round(RadarColorActive.z * 255.f);
		*(byte*)(0x5860CC + 1) = (BYTE)round(RadarColorActive.x * 255.f);
		*(byte*)(0x5860C1 + 1) = (BYTE)round(RadarColorActive.y * 255.f);
		*(byte*)(0x5860BC + 1) = (BYTE)round(RadarColorActive.z * 255.f);
	}
	else
	{
		if (bon)
		{
			*(BYTE*)(0x58A9A2) = 0xA1;
			*(BYTE*)(0x58A99A) = 0xA1;
			*(BYTE*)(0x58A996) = 0xA1;
			*(BYTE*)(0x58A8EE) = 0xA1;
			*(BYTE*)(0x58A8E6) = 0xA1;
			*(BYTE*)(0x58A8DE) = 0xA1;
			*(BYTE*)(0x58A89A) = 0xA1;
			*(BYTE*)(0x58A896) = 0xA1;
			*(BYTE*)(0x58A894) = 0xA1;
			*(BYTE*)(0x58A798) = 0xA1;
			*(BYTE*)(0x58A790) = 0xA1;
			*(BYTE*)(0x58A78E) = 0xA1;
			*(BYTE*)(0x582F20) = 0xA1;
			*(BYTE*)(0x582F1A) = 0xA1;
			*(BYTE*)(0x582F18) = 0xA1;
			*(BYTE*)(0x588690 + 1) = 0xA1;
			*(BYTE*)(0x58868B + 1) = 0xA1;
			*(BYTE*)(0x588686 + 1) = 0xA1;
			*(byte*)(0x5860CC + 1) = 0xA1;
			*(byte*)(0x5860C1 + 1) = 0xA1;
			*(byte*)(0x5860BC + 1) = 0xA1;
			bon = false;
		}
	}
}

void CNewMods::RainbowNicknames(bool bEnable)
{
	traceLastFunc("RainbowNicknames()");
	if (!g_SAMP || !pGameInterface || IS_CHEAT_PANIC_ACTIVE) return;

	static bool rainbowActive = false;

	// Store ORIGINAL colortable once
	static bool savedOriginal = false;
	static D3DCOLOR originalColors[SAMP_MAX_PLAYERS];

	D3DCOLOR* sampColorTable = (D3DCOLOR*)((char*)g_dwSAMP_Addr + SAMP_COLOR_OFFSET);

	//---------------------------------------------------------------------
	// ENABLE RAINBOW
	//---------------------------------------------------------------------
	if (bEnable)
	{
		if (!rainbowActive)
		{
			// Save original colors only ONCE
			if (!savedOriginal)
			{
				for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
					originalColors[i] = sampColorTable[i];

				savedOriginal = true;
			}

			rainbowActive = true;
		}

		static float h = 0.0f;
		h += 0.0001f * set.BlackLight.Rainbow.rainbow_nicknames_speed;
		if (h > 1.0f) h = 0.0f;

		D3DCOLOR col;

		if (BlackLightFuncs->bCustomRainbowColorSet)
		{
			col = D3DCOLOR_ARGB(
				255,
				(BYTE)(custom_color_nicknames.z * 255),
				(BYTE)(custom_color_nicknames.y * 255),
				(BYTE)(custom_color_nicknames.x * 255)
			);
		}
		else
		{
			ImVec4 c = Color2::ToImColor(Color2::FromHSB(h, 1.f, 1.f));
			col = D3DCOLOR_ARGB(255, (BYTE)(c.x * 255), (BYTE)(c.y * 255), (BYTE)(c.z * 255));
		}

		// Apply rainbow color to all valid players
		for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
		{
			if (g_Players->iIsListed[i] != 1) continue;
			sampColorTable[i] = col;
		}

		return;
	}

	//---------------------------------------------------------------------
	// DISABLE RAINBOW → RESTORE ORIGINAL COLORS
	//---------------------------------------------------------------------
	if (rainbowActive)
	{
		if (savedOriginal)
		{
			for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
				sampColorTable[i] = originalColors[i];
		}

		rainbowActive = false;
	}
}


void CNewMods::renderDriverInfo(bool bEnable)
{
	if (!bEnable)
		return;

	if (!g_dwSAMP_Addr || !g_SAMP || !g_Players)
		return;

	traceLastFunc("renderDriverInfo()");

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	if (gta_menu_active())
		return;

	if (GetAsyncKeyState(VK_F5) < 0)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (g_Scoreboard->iIsEnabled)
		return;

	for (int idrivers = 0; idrivers < SAMP_MAX_PLAYERS; idrivers++)
	{
		if (g_Players->iIsListed[idrivers] != 1)
			continue;

		actor_info* driver_info = getGTAPedFromSAMPPlayerID(idrivers);

		if (!driver_info)
			continue;

		D3DXVECTOR3 driverVec, to_screen;
		float* driver_pos = &driver_info->base.matrix[4 * 3];
		driverVec.x = driver_pos[0];
		driverVec.y = driver_pos[1];
		driverVec.z = driver_pos[2];
		pSampMulti->CalcScreenCoors(&driverVec, &to_screen);

		if (to_screen.z < 1.f)
			continue;

		if (vect3_dist(driver_pos, &g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->base.matrix[4 * 3]) > 280)
			continue;

		if (pSampMulti->IsPlayerDriver(idrivers))
		{
			D3DCOLOR color = samp_color_get(idrivers);

			if (render)
			{
				pD3DFont_sampStuff->Print(pSampMulti->SetText("{19BC0D}Driver: {FFFFFF}%s{19BC0D}[{FFFFFF}%d{19BC0D}]", getPlayerName(idrivers), idrivers), COLOR_WHITE(255), to_screen.x, to_screen.y + (pD3DFont_sampStuff->DrawHeight() + 1.0f), false, false);
			}
		}
	}
}

void CNewMods::renderShowDoorsStatus(bool bEnable)
{
	if (!bEnable)
		return;

	if (!g_dwSAMP_Addr || !g_SAMP || !g_Players)
		return;
	traceLastFunc("renderShowDoorsStatus()");

	if (gta_menu_active())
		return;

	if (IS_CHEAT_PANIC_ACTIVE) return;

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	// don't run if we don't exist
	if (isBadPtr_GTA_pPed(pPedSelf))
		return;

	if (g_Scoreboard->iIsEnabled)
		return;

	for (int i = 0; i < SAMP_MAX_VEHICLES; i++)
	{
		if (g_SAMP->pPools->pVehicle->pSAMP_Vehicle == NULL)
			continue;

		vehicle_info* vehicle = getGTAVehicleFromSAMPVehicleID(i);

		if (!vehicle)
			continue;
		
		D3DXVECTOR3 vehicle_pos, screenposs;
		float* f_pos = &vehicle->base.matrix[4 * 3];
		vehicle_pos.x = f_pos[0];
		vehicle_pos.y = f_pos[1];
		vehicle_pos.z = f_pos[2];
		pSampMulti->CalcScreenCoors(&vehicle_pos, &screenposs);

		if (screenposs.z < 1.f)
			continue;

		if (vect3_dist(f_pos, &g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->base.matrix[4 * 3]) > set.BlackLight.cars_doors_status_distance_show)
			continue;

		if (i != g_Players->pLocalPlayer->sCurrentVehicleID)
		{
			g_Vehicles->pSAMP_Vehicle[i]->iIsLocked ? pD3DFont->PrintShadow(screenposs.x, screenposs.y - 20, COLOR_RED(255), pSampMulti->SetText("Vehicle: %d = Locked" ,i)) :
				pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y - 20, COLOR_GREEN(255), pSampMulti->SetText("Vehicle: %d = Unlocked",i));
		}
	}
}

void CNewMods::renderPlayersTracers(bool bEnable)
{
	if (!bEnable)
		return;

	if (!g_dwSAMP_Addr || !g_SAMP || !g_Players)
		return;

	traceLastFunc("renderPlayerTracers()");

	if (gta_menu_active())
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (GetAsyncKeyState(VK_F5) < 0)
		return;

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	if (g_Scoreboard->iIsEnabled || BlackLightFuncs->Menu.bImScoreboad)
		return;

	// don't run if we don't exist
	if (isBadPtr_GTA_pPed(pPedSelf))
		return;

	for (int playerid = 0; playerid < SAMP_MAX_PLAYERS; playerid++)
	{
		if (g_Players->iIsListed[playerid] != 1)
			continue;

		struct actor_info* player = getGTAPedFromSAMPPlayerID(playerid);

		if (!player)
			continue;

		CPed* pPed = pGameInterface->GetPools()->GetPed((DWORD*)player);

		if (!pPed)
			return;

		CVector mySpinePos, TargetSpinePos;
		pPedSelf->GetBonePosition(BONE_SPINE1, &mySpinePos);
		pPed->GetBonePosition(BONE_SPINE1, &TargetSpinePos);

		D3DCOLOR color = samp_color_get(playerid);

		if (render)
		{
			if (!BlackLightFuncs->bCustomColor_ESP)
				render->DrawLine(CVecToD3DXVEC(mySpinePos), CVecToD3DXVEC(TargetSpinePos), color);
			else if (BlackLightFuncs->bCustomColor_ESP)
				render->DrawLine(CVecToD3DXVEC(mySpinePos), CVecToD3DXVEC(TargetSpinePos), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersTracers.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersTracers.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersTracers.z * 255.f)));
		}
	}
}

void CNewMods::renderPlayerBones(bool bEnable)
{
	if (!bEnable)
		return;

	if (!g_dwSAMP_Addr || !g_SAMP || !g_Players)
		return;

	traceLastFunc("renderPlayerBones()");

	if (gta_menu_active())
		return;

	// Exit this function and enable samp nametags, if panic key
	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	// don't run if we don't exist
	if (isBadPtr_GTA_pPed(pPedSelf))
		return;

	//big code but i don't care
	for (int playerid = 0; playerid < SAMP_MAX_PLAYERS; playerid++)
	{
		if (g_Players->iIsListed[playerid] != 1)
			continue;

		actor_info* player = getGTAPedFromSAMPPlayerID(playerid);

		if (player == NULL)
			continue;

		CPed* pPed = pGameInterface->GetPools()->GetPed((DWORD*)player);

		if (isBadPtr_GTA_pPed(pPed))
			return;

		CVector head, head1, uppertorso, shoulderleft, shoulderright,
			leftlbow, rightlbow, leftwrist, rightwrist, righthand, lefthand, spine, pelvis, pelvis1,
			righthip, lefthip, leftknee, rightknee, lefttankle, righttankle, rightfoot, leftfoot;

		pPed->GetBonePosition(BONE_HEAD2, &head);
		pPed->GetBonePosition(BONE_HEAD, &head1);
		pPed->GetBonePosition(BONE_UPPERTORSO, &uppertorso);
		pPed->GetBonePosition(BONE_RIGHTSHOULDER, &shoulderright);
		pPed->GetBonePosition(BONE_LEFTSHOULDER, &shoulderleft);
		pPed->GetBonePosition(BONE_RIGHTELBOW, &rightlbow);
		pPed->GetBonePosition(BONE_LEFTELBOW, &leftlbow);
		pPed->GetBonePosition(BONE_RIGHTWRIST, &rightwrist);
		pPed->GetBonePosition(BONE_LEFTWRIST, &leftwrist);
		pPed->GetBonePosition(BONE_RIGHTHAND, &righthand);
		pPed->GetBonePosition(BONE_LEFTHAND, &lefthand);
		pPed->GetBonePosition(BONE_SPINE1, &spine);
		pPed->GetBonePosition(BONE_PELVIS, &pelvis);
		pPed->GetBonePosition(BONE_PELVIS1, &pelvis1);
		pPed->GetBonePosition(BONE_RIGHTHIP, &righthip);
		pPed->GetBonePosition(BONE_LEFTHIP, &lefthip);
		pPed->GetBonePosition(BONE_RIGHTKNEE, &rightknee);
		pPed->GetBonePosition(BONE_LEFTKNEE, &leftknee);
		pPed->GetBonePosition(BONE_RIGHTANKLE, &righttankle);
		pPed->GetBonePosition(BONE_LEFTANKLE, &lefttankle);
		pPed->GetBonePosition(BONE_RIGHTFOOT, &rightfoot);
		pPed->GetBonePosition(BONE_LEFTFOOT, &leftfoot);

		if (render)
		{
			D3DCOLOR color;

			color = samp_color_get(playerid);

			if (!BlackLightFuncs->bCustomColor_ESP)
			{
				render->DrawLine(CVecToD3DXVEC(head1), CVecToD3DXVEC(head), color);
				render->DrawLine(CVecToD3DXVEC(head), CVecToD3DXVEC(uppertorso), color);
				render->DrawLine(CVecToD3DXVEC(uppertorso), CVecToD3DXVEC(shoulderright), color);
				render->DrawLine(CVecToD3DXVEC(uppertorso), CVecToD3DXVEC(shoulderleft), color);
				render->DrawLine(CVecToD3DXVEC(shoulderleft), CVecToD3DXVEC(leftlbow), color);
				render->DrawLine(CVecToD3DXVEC(shoulderright), CVecToD3DXVEC(rightlbow), color);
				render->DrawLine(CVecToD3DXVEC(leftlbow), CVecToD3DXVEC(leftwrist), color);
				render->DrawLine(CVecToD3DXVEC(rightlbow), CVecToD3DXVEC(rightwrist), color);
				render->DrawLine(CVecToD3DXVEC(leftwrist), CVecToD3DXVEC(lefthand), color);
				render->DrawLine(CVecToD3DXVEC(rightwrist), CVecToD3DXVEC(righthand), color);
				render->DrawLine(CVecToD3DXVEC(uppertorso), CVecToD3DXVEC(spine), color);
				render->DrawLine(CVecToD3DXVEC(spine), CVecToD3DXVEC(pelvis), color);
				render->DrawLine(CVecToD3DXVEC(pelvis), CVecToD3DXVEC(pelvis1), color);
				render->DrawLine(CVecToD3DXVEC(pelvis1), CVecToD3DXVEC(righthip), color);
				render->DrawLine(CVecToD3DXVEC(pelvis1), CVecToD3DXVEC(lefthip), color);
				render->DrawLine(CVecToD3DXVEC(righthip), CVecToD3DXVEC(rightknee), color);
				render->DrawLine(CVecToD3DXVEC(lefthip), CVecToD3DXVEC(leftknee), color);
				render->DrawLine(CVecToD3DXVEC(rightknee), CVecToD3DXVEC(righttankle), color);
				render->DrawLine(CVecToD3DXVEC(leftknee), CVecToD3DXVEC(lefttankle), color);
				render->DrawLine(CVecToD3DXVEC(righttankle), CVecToD3DXVEC(rightfoot), color);
				render->DrawLine(CVecToD3DXVEC(lefttankle), CVecToD3DXVEC(leftfoot), color);
			}
			else if (BlackLightFuncs->bCustomColor_ESP)
			{
				render->DrawLine(CVecToD3DXVEC(head1), CVecToD3DXVEC(head), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(head), CVecToD3DXVEC(uppertorso), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(uppertorso), CVecToD3DXVEC(shoulderright), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(uppertorso), CVecToD3DXVEC(shoulderleft), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(shoulderleft), CVecToD3DXVEC(leftlbow), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(shoulderright), CVecToD3DXVEC(rightlbow), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(leftlbow), CVecToD3DXVEC(leftwrist), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(rightlbow), CVecToD3DXVEC(rightwrist), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(leftwrist), CVecToD3DXVEC(lefthand), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(rightwrist), CVecToD3DXVEC(righthand), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(uppertorso), CVecToD3DXVEC(spine), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(spine), CVecToD3DXVEC(pelvis), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(pelvis), CVecToD3DXVEC(pelvis1), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(pelvis1), CVecToD3DXVEC(righthip), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(pelvis1), CVecToD3DXVEC(lefthip), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(righthip), CVecToD3DXVEC(rightknee), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(lefthip), CVecToD3DXVEC(leftknee), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(rightknee), CVecToD3DXVEC(righttankle), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(leftknee), CVecToD3DXVEC(lefttankle), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(righttankle), CVecToD3DXVEC(rightfoot), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
				render->DrawLine(CVecToD3DXVEC(lefttankle), CVecToD3DXVEC(leftfoot), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_PlayersBones.x * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.y * 255.f), (BYTE)round(CC_EXTRAS_PlayersBones.z * 255.f)));
			}
		}
	}
}


void CNewMods::renderPlayerBox(bool bEnable)
{
	if (!bEnable)
		return;

	if (!g_dwSAMP_Addr || !g_SAMP || !g_Players)
		return;

	traceLastFunc("renderPlayerBox()");

	if (gta_menu_active())
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	// don't run during certain samp events
	if (g_dwSAMP_Addr && g_SAMP)
	{
		if (
			// Scoreboard open?
			(GetAsyncKeyState(VK_TAB) < 0)
			|| g_Scoreboard->iIsEnabled
			// F10 key down?
			|| GetAsyncKeyState(VK_F10) < 0
			)
			return;
	}

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	for (int playerid = 0; playerid < SAMP_MAX_PLAYERS; playerid++)
	{
		if (g_Players->iIsListed[playerid] != 1)
			continue;

		actor_info* player = getGTAPedFromSAMPPlayerID(playerid);

		if (!player)
			continue;

		CPed* pPed = pGameInterface->GetPools()->GetPed((DWORD*)player);
		if (!pPed)
			return;

		float
			max_up_val = 0,
			max_down_val = 0,
			max_left_val = 0,
			max_right_val = 0;


		bool invalid = false;

		for (int bone_id = BONE_PELVIS1; bone_id <= BONE_RIGHTFOOT; ++bone_id)
		{
			if (bone_id >= BONE_PELVIS1 && bone_id <= BONE_HEAD ||
				bone_id >= BONE_RIGHTUPPERTORSO && bone_id <= BONE_RIGHTTHUMB ||
				bone_id >= BONE_LEFTUPPERTORSO && bone_id <= BONE_LEFTTHUMB ||
				bone_id >= BONE_LEFTHIP && bone_id <= BONE_LEFTFOOT ||
				bone_id >= BONE_RIGHTHIP && bone_id <= BONE_RIGHTFOOT)
			{
				CVector bone;
				pPed->GetBonePosition((eBone)bone_id, &bone);

				D3DXVECTOR3 bone_pos;

				bone_pos.x = bone.fX;
				bone_pos.y = bone.fY;
				bone_pos.z = bone.fZ;
				D3DXVECTOR3 bone_screen_pos;
				pSampMulti->CalcScreenCoors(&bone_pos, &bone_screen_pos);
				// check if the iter is culled or not
				if (bone_screen_pos.z < 1.f)
				{
					invalid = true;
					break;
				}


				if (!max_up_val && !max_down_val &&
					!max_left_val && !max_right_val)
				{
					max_up_val = bone_screen_pos.y;
					max_down_val = bone_screen_pos.y;
					max_right_val = bone_screen_pos.x;
					max_left_val = bone_screen_pos.x;
				}


				if (bone_screen_pos.y > max_up_val)
					max_up_val = bone_screen_pos.y;

				if (bone_screen_pos.y < max_down_val)
					max_down_val = bone_screen_pos.y;

				if (bone_screen_pos.x > max_right_val)
					max_right_val = bone_screen_pos.x;

				if (bone_screen_pos.x < max_left_val)
					max_left_val = bone_screen_pos.x;

			}
		}

		if (!invalid)
		{

			float height_box = max_up_val - max_down_val;
			float widht_box = max_right_val - max_left_val;

			D3DCOLOR color;
			color = samp_color_get(playerid);

			//float width_offset = widht_box * 30.0f / 100.0f;
			//float height_offset = height_offset * 30.0f / 100.0f;

			if (!BlackLightFuncs->bCustomColor_ESP)
			{
				render->D3DBoxBorder(max_left_val /*- width_offset / 2*/,
					max_down_val /*- height_offset / 2*/,
					widht_box /*+ width_offset*/,
					height_box /*+ height_offset*/, color, 0);
			}
			else if (BlackLightFuncs->bCustomColor_ESP)
			{
				render->D3DBoxBorder(max_left_val /*- width_offset / 2*/,
					max_down_val /*- height_offset / 2*/,
					widht_box /*+ width_offset*/,
					height_box /*+ height_offset*/, D3DCOLOR_ARGB(255
						, (BYTE)round(CC_EXTRAS_PlayersBox.x * 255.f)
						, (BYTE)round(CC_EXTRAS_PlayersBox.y * 255.f)
						, (BYTE)round(CC_EXTRAS_PlayersBox.z * 255.f))
					, 0);
			}
		}
	}
}

static void DrawOriented3DBox(const CVector& center,
	const CVector& right,
	const CVector& front,
	const CVector& up,
	float halfWidth,
	float halfLength,
	float halfHeight,
	D3DCOLOR color)
{
	CVector ax = right; ax.Normalize();
	CVector ay = front; ay.Normalize();
	CVector az = up;    az.Normalize();

	CVector ex = ax * halfWidth;
	CVector ey = ay * halfLength;
	CVector ez = az * halfHeight;

	// 8 corners of the box
	CVector c000 = center - ex - ey - ez;
	CVector c100 = center + ex - ey - ez;
	CVector c110 = center + ex + ey - ez;
	CVector c010 = center - ex + ey - ez;

	CVector c001 = center - ex - ey + ez;
	CVector c101 = center + ex - ey + ez;
	CVector c111 = center + ex + ey + ez;
	CVector c011 = center - ex + ey + ez;

	// Bottom rectangle
	render->DrawLine(CVecToD3DXVEC(c000), CVecToD3DXVEC(c100), color);
	render->DrawLine(CVecToD3DXVEC(c100), CVecToD3DXVEC(c110), color);
	render->DrawLine(CVecToD3DXVEC(c110), CVecToD3DXVEC(c010), color);
	render->DrawLine(CVecToD3DXVEC(c010), CVecToD3DXVEC(c000), color);

	// Top rectangle
	render->DrawLine(CVecToD3DXVEC(c001), CVecToD3DXVEC(c101), color);
	render->DrawLine(CVecToD3DXVEC(c101), CVecToD3DXVEC(c111), color);
	render->DrawLine(CVecToD3DXVEC(c111), CVecToD3DXVEC(c011), color);
	render->DrawLine(CVecToD3DXVEC(c011), CVecToD3DXVEC(c001), color);

	// Vertical edges
	render->DrawLine(CVecToD3DXVEC(c000), CVecToD3DXVEC(c001), color);
	render->DrawLine(CVecToD3DXVEC(c100), CVecToD3DXVEC(c101), color);
	render->DrawLine(CVecToD3DXVEC(c110), CVecToD3DXVEC(c111), color);
	render->DrawLine(CVecToD3DXVEC(c010), CVecToD3DXVEC(c011), color);
}

void CNewMods::renderPlayer3DBox(bool bEnable)
{
	if (!bEnable) return;
	if (!g_Players || !pGameInterface) return;
	if (gta_menu_active() || IS_CHEAT_PANIC_ACTIVE) return;

	for (int id = 0; id < SAMP_MAX_PLAYERS; id++)
	{
		if (g_Players->iIsListed[id] != 1)
			continue;

		actor_info* a = getGTAPedFromSAMPPlayerID(id);
		if (!a) continue;

		CPed* pPed = pGameInterface->GetPools()->GetPed((DWORD*)a);
		if (!pPed) continue;

		// Matrix (orientation)
		CMatrix m;
		pPed->GetMatrix(&m);

		CVector right = m.vRight;  right.Normalize();
		CVector front = m.vFront;  front.Normalize();
		CVector up = m.vUp;     up.Normalize();

		CVector headPos;
		pPed->GetBonePosition(BONE_HEAD, &headPos);

		CVector footPos;
		pPed->GetBonePosition(BONE_LEFTFOOT, &footPos);

		// Height from head to feet
		float height = (headPos - footPos).Length();
		float halfHeight = height * 0.5f;

		CVector center = headPos - up * halfHeight;

		//  BOX width depth
		const float halfWidth = 0.32f;
		const float halfDepth = 0.25f;

		// Color
		D3DCOLOR col;
		if (!BlackLightFuncs->bCustomColor_ESP)
			col = samp_color_get(id);
		else
			col = D3DCOLOR_ARGB(
				255,
				(BYTE)(CC_EXTRAS_PlayersBox.x * 255.f),
				(BYTE)(CC_EXTRAS_PlayersBox.y * 255.f),
				(BYTE)(CC_EXTRAS_PlayersBox.z * 255.f)
			);

		DrawOriented3DBox(
			center,
			right,
			front,
			up,
			halfWidth,
			halfDepth,
			halfHeight,
			col
		);
	}
}

//could be modified for bikes front & back wheels
static void GetVehicleWheelPositions(CVehicle* pVeh, CVector outPos[4])
{
	CVehicleSAInterface* vi = pVeh->GetVehicleInterface();
	if (!vi) return;

	RwFrame* wheelFrames[4] =
	{
		vi->pWheelFrontLeft,
		vi->pWheelRearLeft,
		vi->pWheelFrontRight,
		vi->pWheelRearRight
	};

	for (int i = 0; i < 4; i++)
	{
		RwFrame* f = wheelFrames[i];

		if (!f)
		{
			outPos[i] = *pVeh->GetPosition();
			continue;
		}

		RwMatrix* m = &f->ltm;

		if (!m)
		{
			outPos[i] = *pVeh->GetPosition();
			continue;
		}

		outPos[i].fX = m->pos.x;
		outPos[i].fY = m->pos.y;
		outPos[i].fZ = m->pos.z;
	}
}


void CNewMods::renderVehicleWheelESP(bool bEnable)
{
	if (!bEnable) return;
	if (!g_Vehicles || !g_Players || !pGameInterface) return;
	if (gta_menu_active() || IS_CHEAT_PANIC_ACTIVE) return;

	float cx = pPresentParam.BackBufferWidth * 0.5f;
	float cy = pPresentParam.BackBufferHeight * 0.5f;

	int bestVeh = -1, bestWheel = -1;
	float bestDist = 99999.f;

	for (int i = 0; i < SAMP_MAX_VEHICLES; i++)
	{
		if (i == g_Players->pLocalPlayer->sCurrentVehicleID)
			continue;
		if (g_Vehicles->iIsListed[i] != 1)
			continue;
		if (!g_Vehicles->pSAMP_Vehicle[i])
			continue;
		if (!g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle)
			continue;

		vehicle_info* vinfo = getGTAVehicleFromSAMPVehicleID(i);
		if (!vinfo) continue;

		CVehicle* pVeh = pGameInterface->GetPools()->GetVehicle((DWORD*)vinfo);
		if (isBadPtr_GTA_pVehicle(pVeh)) continue;

		eClientVehicleType type = GetVehicleType(vinfo->base.model_alt_id);
		if (type == CLIENTVEHICLE_BOAT ||
			type == CLIENTVEHICLE_TRAIN ||
			type == CLIENTVEHICLE_HELI ||
			type == CLIENTVEHICLE_PLANE ||
			type == CLIENTVEHICLE_BIKE ||
			type == CLIENTVEHICLE_MONSTERTRUCK ||
			type == CLIENTVEHICLE_QUADBIKE ||
			type == CLIENTVEHICLE_BMX ||
			type == CLIENTVEHICLE_TRAILER)
			continue;

		// vehicle matrix
		CMatrix m;
		pVeh->GetMatrix(&m);

		// --- NEW RW FRAME WHEEL POSITIONS ---
		CVector wheels[4];
		GetVehicleWheelPositions(pVeh, wheels);

		for (int w = 0; w < 4; w++)
		{
			bool burst = (vinfo->car_tire_status[w] != 0);

			D3DCOLOR col = burst ?
				D3DCOLOR_ARGB(255, 255, 0, 0) :
				D3DCOLOR_ARGB(255, 0, 255, 0);

			// perfect 3D box around each wheel
			DrawOriented3DBox(
				wheels[w],
				m.vRight, m.vFront, m.vUp,
				0.25f,       // half width
				0.25f,       // half length
				0.35f,       // half height
				col
			);

			// aiming detection
			D3DXVECTOR3 scr, world = CVecToD3DXVEC(wheels[w]);
			pSampMulti->CalcScreenCoors(&world, &scr);

			if (scr.z < 1.f)
				continue;

			float dx = scr.x - cx;
			float dy = scr.y - cy;
			float d2 = dx * dx + dy * dy;

			if (d2 < bestDist && d2 < 40.0f * 40.0f)
			{
				bestDist = d2;
				bestVeh = i;
				bestWheel = w;
			}
		}
	}

	// highlight aimed wheel
	/*if (bestVeh != -1 && bestWheel != -1)
	{
		vehicle_info* vinfo = getGTAVehicleFromSAMPVehicleID(bestVeh);
		CVehicle* pVeh = pGameInterface->GetPools()->GetVehicle((DWORD*)vinfo);

		CMatrix m;
		pVeh->GetMatrix(&m);

		CVector wheels[4];
		GetVehicleWheelPositions(pVeh, wheels);

		bool burst = (vinfo->car_tire_status[bestWheel] != 0);
		D3DCOLOR col = burst ?
			D3DCOLOR_ARGB(255, 255, 50, 50) :
			D3DCOLOR_ARGB(255, 70, 255, 70);

		// glow
		DrawOriented3DBox(
			wheels[bestWheel],
			m.vRight, m.vFront, m.vUp,
			0.32f, 0.32f, 0.45f,
			D3DCOLOR_ARGB(255, 255, 255, 0)
		);

		// main highlight
		DrawOriented3DBox(
			wheels[bestWheel],
			m.vRight, m.vFront, m.vUp,
			0.28f, 0.28f, 0.40f,
			col
		);
	}*/
}

void CNewMods::AutoShootWheel(bool bEnable)
{
	if (BlackLightFuncs->Menu.bMain_Menu)
		return; //

	if (!bEnable) return;
	if (IS_CHEAT_PANIC_ACTIVE) return;
	if (!pGameInterface || !pPedSelf || !g_Vehicles) return;

	static bool bPressed = false;

	// Release fire if pressed earlier
	if (bPressed)
	{
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		bPressed = false;
	}

	// Crosshair screen position
	float crossX, crossY;

	if (g_Players->pLocalPlayer->byteCurrentWeapon == 34)  // Sniper
	{
		crossX = pPresentParam.BackBufferWidth * 0.5f;
		crossY = pPresentParam.BackBufferHeight * 0.5f;
	}
	else
	{
		crossX = pPresentParam.BackBufferWidth * 0.53f;
		crossY = pPresentParam.BackBufferHeight * 0.40f;
	}

	int bestVeh = -1, bestWheel = -1;
	float bestDist = 999999.f;

	for (int id = 0; id < SAMP_MAX_VEHICLES; id++)
	{
		if (id == g_Players->pLocalPlayer->sCurrentVehicleID)
			continue;
		if (g_Vehicles->iIsListed[id] != 1)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[id] == NULL)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[id]->pGTA_Vehicle == NULL)
			continue;

		vehicle_info* vinfo = getGTAVehicleFromSAMPVehicleID(id);
		if (!vinfo)
			continue;

		CVehicle* pVeh = pGameInterface->GetPools()->GetVehicle((DWORD*)vinfo);
		if (isBadPtr_GTA_pVehicle(pVeh))
			continue;

		// wheel world positions
		CVector wheels[4];
		GetVehicleWheelPositions(pVeh, wheels);

		for (int w = 0; w < 4; w++)
		{
			D3DXVECTOR3 scr;
			D3DXVECTOR3 world = CVecToD3DXVEC(wheels[w]);

			pSampMulti->CalcScreenCoors(&world, &scr);
			if (scr.z < 1.f) continue;

			float dx = scr.x - crossX;
			float dy = scr.y - crossY;
			float dist2 = dx * dx + dy * dy;

			if (dist2 < 48 * 48) // aim assist radius
			{
				if (dist2 < bestDist)
				{
					bestDist = dist2;
					bestVeh = id;
					bestWheel = w;
				}
			}
		}
	}

	if (bestVeh == -1 || bestWheel == -1)
		return;

	// HIGHLIGHT THE TARGETED WHEEL WITH A 3D BOX
	vehicle_info* vinfo = getGTAVehicleFromSAMPVehicleID(bestVeh);
	CVehicle* pVeh = pGameInterface->GetPools()->GetVehicle((DWORD*)vinfo);

	CVector wheels[4];
	GetVehicleWheelPositions(pVeh, wheels);

	CMatrix m;
	pVeh->GetMatrix(&m);

	bool burst = (vinfo->car_tire_status[bestWheel] != 0);

	D3DCOLOR highlight =
		burst ? D3DCOLOR_ARGB(255, 255, 0, 0) :   // RED (burst)
		D3DCOLOR_ARGB(255, 0, 255, 0);    // GREEN (intact)

	// glow
	DrawOriented3DBox(
		wheels[bestWheel],
		m.vRight, m.vFront, m.vUp,
		0.38f, 0.38f, 0.48f,
		D3DCOLOR_ARGB(150, 255, 255, 0)
	);

	// main highlight
	DrawOriented3DBox(
		wheels[bestWheel],
		m.vRight, m.vFront, m.vUp,
		0.30f, 0.30f, 0.40f,
		highlight
	);

	// AUTO FIRE
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	bPressed = true;
}


void CNewMods::renderHelicopterTracers(bool bEnable)
{
	if (!bEnable)
		return;

	if (!g_dwSAMP_Addr || !g_SAMP || !g_Players)
		return;

	traceLastFunc("renderHelicopterTracers()");

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	if (gta_menu_active())
		return;

	// don't run if we don't exist
	if (isBadPtr_GTA_pPed(pPedSelf))
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (g_Scoreboard->iIsEnabled || BlackLightFuncs->Menu.bImScoreboad)
		return;

	actor_info* player = actor_info_get(ACTOR_SELF, NULL);

	if (!player)
		return;
	for (int i = 0; i < SAMP_MAX_VEHICLES; i++)
	{
		//remove bad vehicles
		if (g_Vehicles->iIsListed[i] != 1)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[i] == NULL)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle == NULL)
			continue;

		vehicle_info* vehicle = getGTAVehicleFromSAMPVehicleID(i);

		if (!vehicle)
			continue;

		const struct vehicle_entry* evehicle;
		evehicle = gta_vehicle_get_by_id(g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle->base.model_alt_id);

		D3DXVECTOR3 get_vehs_pos, screenposs; CVector mySpinePoss, VehVec;
		CPed* pPedSelf = pGameInterface->GetPools()->GetPed((DWORD*)player);
		CVehicle* pVeh = pGameInterface->GetPools()->GetVehicle((DWORD*)vehicle);
		pPedSelf->GetBonePosition(BONE_SPINE1, &mySpinePoss);

		VehVec.fX = vehicle->base.matrix[12];
		VehVec.fY = vehicle->base.matrix[13];
		VehVec.fZ = vehicle->base.matrix[14];

		float* fvpos = &vehicle->base.matrix[4 * 3];
		get_vehs_pos.x = fvpos[0];
		get_vehs_pos.y = fvpos[1];
		get_vehs_pos.z = fvpos[2];
		pSampMulti->CalcScreenCoors(&get_vehs_pos, &screenposs);

		if (screenposs.z < 1.f)
			continue;

		D3DCOLOR color = samp_color_get(i);

		if (i != g_Players->pLocalPlayer->sCurrentVehicleID && strcmp(evehicle->name, "Seasparrow") == 0)
		{
			pD3DFont_sampStuff->Print(pSampMulti->SetText("%s\n[Distance: %.0fm]", evehicle->name, pSampMulti->GetDistance(D3DXVECTOR3(get_vehs_pos.x, get_vehs_pos.y, get_vehs_pos.z))), COLOR_GREEN(255), screenposs.x, screenposs.y, false, false);
			render->DrawLine(CVecToD3DXVEC(mySpinePoss), CVecToD3DXVEC(VehVec), COLOR_RED(255));
		}
	}
}

void CNewMods::renderVehicleTracers(bool bEnable)
{
	if (!bEnable)
		return;

	if (!g_dwSAMP_Addr || !g_SAMP || !g_Players)
		return;

	traceLastFunc("renderVehicleTracers()");

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	if (gta_menu_active())
		return;

	// Exit this function and enable samp nametags, if panic key
	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (g_Scoreboard->iIsEnabled || BlackLightFuncs->Menu.bImScoreboad)
		return;

	// don't run if we don't exist
	if (isBadPtr_GTA_pPed(pPedSelf))
		return;

	for (int i = 0; i < SAMP_MAX_VEHICLES; i++)
	{
		if (g_Vehicles->iIsListed[i] != 1)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[i] == NULL)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle == NULL)
			continue;

		struct actor_info* player = actor_info_get(ACTOR_SELF, NULL);
		struct vehicle_info* vehicle = getGTAVehicleFromSAMPVehicleID(i);

		if (!player)
			return;

		if (!vehicle)
			continue;

		const struct vehicle_entry* evehicle;
		evehicle = gta_vehicle_get_by_id(g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle->base.model_alt_id);

		D3DXVECTOR3 get_vehs_pos, screenposs;
		CVector VehVec;
		//CVehicle* pVeh = pGameInterface->GetPools()->GetVehicle((DWORD*)vehicle);

		VehVec.fX = vehicle->base.matrix[12];
		VehVec.fY = vehicle->base.matrix[13];
		VehVec.fZ = vehicle->base.matrix[14];

		float* fvpos = &vehicle->base.matrix[4 * 3];
		get_vehs_pos.x = fvpos[0];
		get_vehs_pos.y = fvpos[1];
		get_vehs_pos.z = fvpos[2];
		pSampMulti->CalcScreenCoors(&get_vehs_pos, &screenposs);

		if (screenposs.z < 1.f)
			continue;

		if (i != g_Players->pLocalPlayer->sCurrentVehicleID)
		{
			if (!BlackLightFuncs->bCustomColor_ESP)
			{
				render->DrawLine(CVecToD3DXVEC(pSampMulti->CVecGetBonePos(g_Players->sLocalPlayerID, eBone::BONE_LEFTFOOT)), CVecToD3DXVEC(VehVec), D3DCOLOR_ARGB(vehicle->color[0], vehicle->color[1], vehicle->color[2], vehicle->color[3]));
			}
			else if (BlackLightFuncs->bCustomColor_ESP)
			{
				render->DrawLine(CVecToD3DXVEC(pSampMulti->CVecGetBonePos(g_Players->sLocalPlayerID, eBone::BONE_LEFTFOOT)), CVecToD3DXVEC(VehVec), D3DCOLOR_ARGB(255, (BYTE)round(CC_EXTRAS_VehicleTracers.x * 255.f), (BYTE)round(CC_EXTRAS_VehicleTracers.y * 255.f), (BYTE)round(CC_EXTRAS_VehicleTracers.z * 255.f)));
			}
			if(!BlackLightFuncs->bVehicleInfo)
			pD3DFont_sampStuff->Print(vect3_dist(fvpos, &player->base.matrix[12]) == 50 || vect3_dist(fvpos, &player->base.matrix[12]) < 50 ? pSampMulti->SetText("%s(Model: %d)\n[Nearest: %.0fm]", evehicle->name, pSampMulti->getVehicleModelBySAMPID(i), pSampMulti->GetDistance(D3DXVECTOR3(get_vehs_pos.x, get_vehs_pos.y, get_vehs_pos.z)) - 3.0f) : pSampMulti->SetText("%s", evehicle->name), COLOR_WHITE(255), screenposs.x, screenposs.y - 3.0f, false, false);
		}
	}
}

void CNewMods::renderPlayers2DInfo(bool bEnable)
{

	if (!bEnable)
		return;

	// Exit this function and enable samp nametags, if panic key
	if (!g_dwSAMP_Addr || !g_SAMP || !g_Players)
		return;

	traceLastFunc("renderPlayers2DInfo()");

	if (gta_menu_active())
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (GetAsyncKeyState(VK_F5) < 0)
		return;

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	// don't run if we don't exist
	if (isBadPtr_GTA_pPed(pPedSelf))
		return;

	if (g_Scoreboard->iIsEnabled || BlackLightFuncs->Menu.bImScoreboad)
		return;

	struct actor_info* self = actor_info_get(ACTOR_SELF, 0);

	if (!self)
		return;

	for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
	{
		if (g_Players->iIsListed[i] != 1)
			continue;

		if (g_Players->pRemotePlayer[i] == NULL)
			continue;

		struct actor_info* actor = getGTAPedFromSAMPPlayerID(i);

		if (!actor)
			continue;

		D3DXVECTOR3 player_pos, screenposs;

		float* f_pos = &actor->base.matrix[4 * 3];
		player_pos.x = f_pos[0];
		player_pos.y = f_pos[1];
		player_pos.z = f_pos[2];
		pSampMulti->CalcScreenCoors(&player_pos, &screenposs);

		if (vect3_dist(f_pos, &self->base.matrix[4 * 3]) > set.BlackLight.players_2DInfo_distance)
			continue;

		if (screenposs.z < 1.f)
			continue;

		pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y - 42, COLOR_WHITE(255)
			, pSampMulti->SetText("{C235DD}Player{FFFFFF} %s <%d>{00FF00} Distance{FFFFFF} %.0f m", getPlayerName(i), i, pSampMulti->GetDistance(D3DXVECTOR3(player_pos.x, player_pos.y, player_pos.z))));

		pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y - 28, COLOR_WHITE(255)
			, pSampMulti->SetText("{FF0F0F}Health{FFFFFF} %u Armour %u{FFA00F} Weapon{FFFFFF} %s{FFFFFF} %u", pSampMulti->getPlayerHealth(i), pSampMulti->getPlayerArmor(i), pSampMulti->getPlayerWeapon(i), pSampMulti->getPlayerWeaponModelID(i)));

		pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y - 14, COLOR_WHITE(255)
			, pSampMulti->SetText("{F0FF0F}Skin{FFFFFF} %d{00FF00} Ping{FFFFFF} %d Score %d", pSampMulti->getPlayerSkin(i), pSampMulti->getPlayerPing(i), pSampMulti->getPlayerScore(i)));

		pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y, COLOR_WHITE(255)
			, pSampMulti->SetText("Sprint Energy: %.0f\nPos X %0.02f Y %0.02f Z %0.02f", (pSampMulti->getPlayerSprintEnergy(i) / 31.47f) < 0.0f ? 0.0f : (pSampMulti->getPlayerSprintEnergy(i) / 31.47f), player_pos.x, player_pos.y, player_pos.z));
	}
}

void CNewMods::renderVehicleInfo(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("renderVehicleInfo()");

	if (gta_menu_active())
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (!g_dwSAMP_Addr || !g_SAMP || !g_Vehicles)
		return;

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	if (GetAsyncKeyState(VK_F5) < 0)
		return;

	// don't run if we don't exist
	if (isBadPtr_GTA_pPed(pPedSelf))
		return;

	if (g_Scoreboard->iIsEnabled || BlackLightFuncs->Menu.bImScoreboad)
		return;

	for (int i = 0; i < SAMP_MAX_VEHICLES; i++)
	{
		if (g_Vehicles->iIsListed[i] != 1)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[i] == NULL)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle == NULL)
			continue;

		struct vehicle_info* vehicle = getGTAVehicleFromSAMPVehicleID(i);
		if (!vehicle)
			continue;

		const struct vehicle_entry* evehicle;
		D3DXVECTOR3 vehicle_pos, screenposs;

		float* f_pos = &vehicle->base.matrix[4 * 3];
		vehicle_pos.x = f_pos[0];
		vehicle_pos.y = f_pos[1];
		vehicle_pos.z = f_pos[2];
		pSampMulti->CalcScreenCoors(&vehicle_pos, &screenposs);

		if (screenposs.z < 1.f)
			continue;

		if (vect3_dist(f_pos, &g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->base.matrix[4 * 3]) > set.BlackLight.vehicle_2d_info_distance_draw)
			continue;

		if (!BlackLightFuncs->bVehicle2DInfo_MyCarInclude)
		{
			if (i == g_Players->pLocalPlayer->sCurrentVehicleID)
				continue;
		}
		
		evehicle = gta_vehicle_get_by_id(g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle->base.model_alt_id);

		pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y - 42, COLOR_WHITE(255)
			, pSampMulti->SetText("{C235DD}Vehicle{FFFFFF} %s Model %d", evehicle->name, g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle->base.model_alt_id));

		pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y - 28, COLOR_WHITE(255)
			, pSampMulti->SetText("{F0FF0F}Health{FFFFFF} %.0f {FF0F0F}Explode Timer{FFFFFF} %.0f/2276", (g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle->hitpoints / 10), (g_Vehicles->pSAMP_Vehicle[i]->pGTAEntity->burn_timer / 22.76f)));

		pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y - 14, COLOR_WHITE(255)
			, pSampMulti->SetText("{F0FF0F}Passanger Seats{FFFFFF} <%d> {00FF00}Distance{FFFFFF} %.0f m", g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle->m_nMaxPassengers, pSampMulti->GetDistance(D3DXVECTOR3(vehicle_pos.x, vehicle_pos.y, vehicle_pos.z))));

		pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y, COLOR_WHITE(255)
			, pSampMulti->SetText("Pos X %0.02f Y %0.02f Z %0.02f", vehicle_pos.x, vehicle_pos.y, vehicle_pos.z));

		pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y + 14, COLOR_WHITE(255), pSampMulti->SetText("{00FF00}Speed{FFFFFF} %.0f km/h", (float)(vect3_length(g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle->speed) * 170)));

	}
}

void CNewMods::renderPolicePedsDetector(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("renderPolicePedsDetector()");

	if (!g_dwSAMP_Addr || !g_SAMP || !g_Players)
		return;

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	if (gta_menu_active())
		return;

	if (GetAsyncKeyState(VK_F5) < 0)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (g_Scoreboard->iIsEnabled || BlackLightFuncs->Menu.bImScoreboad)
		return;

	struct actor_info* self = getGTAPedFromSAMPPlayerID(g_Players->sLocalPlayerID);

	if (!self)
		return;

	D3DXVECTOR3 player_pos, screenposs;
	float* f_pos = &self->base.matrix[4 * 3];
	player_pos.x = f_pos[0];
	player_pos.y = f_pos[1];
	player_pos.z = f_pos[2];
	pSampMulti->CalcScreenCoors(&player_pos, &screenposs);

	//PLAYERS

	for (int iplayers = 0; iplayers < SAMP_MAX_PLAYERS; iplayers++)
	{
		if (g_Players->iIsListed[iplayers] != 1)
			continue;

		struct actor_info* player = getGTAPedFromSAMPPlayerID(iplayers);

		if (!player)
			continue;

		D3DXVECTOR3 target_pos, screen;
		float* ftPos = &player->base.matrix[12];
		target_pos.x = ftPos[0];
		target_pos.y = ftPos[1];
		target_pos.z = ftPos[2];
		pSampMulti->CalcScreenCoors(&target_pos, &screen);

		if (screen.z < 1.f)
			continue;

		if (player->base.model_alt_id == 280
			|| player->base.model_alt_id == 306
			|| player->base.model_alt_id == 282
			|| player->base.model_alt_id == 283
			|| player->base.model_alt_id == 284
			|| player->base.model_alt_id == 285
			|| player->base.model_alt_id == 309
			|| player->base.model_alt_id == 265
			|| player->base.model_alt_id == 266
			|| player->base.model_alt_id == 267)
		{
			if (render)
			{
				pD3DFont_sampStuff->Print(pSampMulti->SetText("Police Officer\nDistance: %.1f",
					pSampMulti->GetDistance(D3DXVECTOR3(target_pos.x, target_pos.y, target_pos.z))), COLOR_BLUE_V2(255), screen.x, screen.y, false, false);
				if (!BlackLightFuncs->bVehiclesPoliceDetector)
					pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y - 20, COLOR_RED(255), "Police is Near");
			}
		}
	}
}

void CNewMods::renderPoliceVehiclesDetector(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("renderPoliceVehiclesDetector()");

	if (!g_dwSAMP_Addr || !g_SAMP || !g_Vehicles)
		return;

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	if (gta_menu_active())
		return;

	if (GetAsyncKeyState(VK_F5) < 0)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (g_Scoreboard->iIsEnabled || BlackLightFuncs->Menu.bImScoreboad)
		return;

	struct actor_info* self = getGTAPedFromSAMPPlayerID(g_Players->sLocalPlayerID);

	if (!self)
		return;

	D3DXVECTOR3 player_pos, screenposs;
	float* f_pos = &self->base.matrix[4 * 3];
	player_pos.x = f_pos[0];
	player_pos.y = f_pos[1];
	player_pos.z = f_pos[2];
	pSampMulti->CalcScreenCoors(&player_pos, &screenposs);

	for (int i = 0; i < SAMP_MAX_VEHICLES; i++)
	{
		if (g_Vehicles->iIsListed[i] != 1)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[i] == NULL)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle == NULL)
			continue;

		vehicle_info* vehicle = getGTAVehicleFromSAMPVehicleID(i);

		if (!vehicle)
			continue;

		D3DXVECTOR3 vehicle_pos, vscreenposs;
		CVehicle* pVeh = pGameInterface->GetPools()->GetVehicle((DWORD*)vehicle);

		if (isBadPtr_GTA_pVehicle(pVeh))
			return;

		float* f_poss = &vehicle->base.matrix[4 * 3];
		vehicle_pos.x = f_poss[0];
		vehicle_pos.y = f_poss[1];
		vehicle_pos.z = f_poss[2];
		pSampMulti->CalcScreenCoors(&vehicle_pos, &vscreenposs);

		if (vscreenposs.z < 1.f)
			continue;

		if (i != g_Players->pLocalPlayer->sCurrentVehicleID &&
			vehicle->base.model_alt_id == 597
			|| vehicle->base.model_alt_id == 526
			|| vehicle->base.model_alt_id == 598
			|| vehicle->base.model_alt_id == 599
			|| vehicle->base.model_alt_id == 601
			|| vehicle->base.model_alt_id == 528
			|| vehicle->base.model_alt_id == 523
			|| vehicle->base.model_alt_id == 497
			|| vehicle->base.model_alt_id == 427
			)
		{
			if (render)
			{
				pD3DFont_sampStuff->PrintShadow(vscreenposs.x, vscreenposs.y - 20, COLOR_BLUE(255), pSampMulti->SetText("Police Vehicle %.1fm", pSampMulti->GetDistance(D3DXVECTOR3(vehicle_pos.x, vehicle_pos.y, vehicle_pos.z))));
				pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y - 20, COLOR_RED(255), "Police is Near");
				render->DrawLine(screenposs, vscreenposs, COLOR_BLUE_V2(255));
			}
		}
	}
}
static struct playerTagInfo2
{
#pragma pack(1)
	CVector tagPosition;
	float	tagOffsetY;
	float	dist;
	bool	isStairStacked;
	bool	isPastMaxDistance;
	bool	draw;
	float	stairStackedOffset;

} g_playerTagInfo2[SAMP_MAX_PLAYERS];

void CNewMods::renderOutStreamPlayers(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("i am here 1");
	// don't run in the menu
	if (gta_menu_active())
		return;

	// Exit this function and enable samp nametags, if panic key
	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (!g_dwSAMP_Addr || !g_SAMP || !g_Players)
		return;

	// don't run during certain samp events
	if (g_dwSAMP_Addr && g_SAMP)
	{
		if (
			// Scoreboard open?
			(GetAsyncKeyState(VK_TAB) < 0)
			|| g_Scoreboard->iIsEnabled
			// F10 key down?
			|| GetAsyncKeyState(VK_F10) < 0
			)
			return;
	}

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	// don't run if we don't exist
	if (isBadPtr_GTA_pPed(pPedSelf))
		return;

	// for tracking player states as we iterate through
	bool	isPedESPCollided[SAMP_MAX_PLAYERS];
	bool	isPedESPStairStacked[SAMP_MAX_PLAYERS];
	memset(isPedESPCollided, false, sizeof(bool) * SAMP_MAX_PLAYERS);
	memset(isPedESPStairStacked, true, sizeof(bool) * SAMP_MAX_PLAYERS);

	// alignment settings
	int			ESP_tag_player_pixelOffsetY = -10;
	float		ESP_tag_player_D3DBox_pixelOffsetX = -0.5f;
	float		ESP_tag_player_D3DBox_pixelOffsetY = -0.5f;
	float		ESP_tag_player_posOffsetZ = 1.0f;
	float		ESP_tag_player_espHeight = 40.0f;
	//float		ESP_tag_player_movementSpeed = 5.0f;


	D3DXVECTOR3 poss, screenposs;
	char		buf[256];

	actor_info* actor_self = actor_info_get(ACTOR_SELF, NULL);

	if (!actor_self)
		return;

	// get initial variables for peds streamed in
	for (int iGTAID = 0; iGTAID < SAMP_MAX_PLAYERS; iGTAID++)
	{

		if (g_Players->iIsListed[iGTAID] != 1)
			continue;

		if (vect3_near_zero(pOutStreamPos[iGTAID].fpos))
			continue;

		actor_info* actor = getGTAPedFromSAMPPlayerID(iGTAID);

		if (actor)
			continue;

		// get the player position in 2D
		poss.x = pOutStreamPos[iGTAID].fpos[0];
		poss.y = pOutStreamPos[iGTAID].fpos[1];
		poss.z = pOutStreamPos[iGTAID].fpos[2] + ESP_tag_player_posOffsetZ;
		pSampMulti->CalcScreenCoors(&poss, &screenposs);

		// check if the iter is culled or not
		if (screenposs.z < 1.f)
		{
			g_playerTagInfo2[iGTAID].tagOffsetY = 0.0f;
			g_playerTagInfo2[iGTAID].isPastMaxDistance = true;
			g_playerTagInfo2[iGTAID].draw = false;
			continue;
		}
		g_playerTagInfo2[iGTAID].draw = true;

		g_playerTagInfo2[iGTAID].dist = vect3_dist(actor_self->base.matrix + 12, pOutStreamPos[iGTAID].fpos);
		// global, set ESP position for tagOffsetY use
		g_playerTagInfo2[iGTAID].tagPosition.fX = screenposs.x;
		g_playerTagInfo2[iGTAID].tagPosition.fY = screenposs.y;
		g_playerTagInfo2[iGTAID].tagPosition.fZ = screenposs.z;
	}


	// remove staircase problem
	for (int iGTAID = 0; iGTAID < SAMP_MAX_PLAYERS; iGTAID++)
	{

		if (g_Players->iIsListed[iGTAID] != 1)
			continue;

		if (vect3_near_zero(pOutStreamPos[iGTAID].fpos))
			continue;

		actor_info* actor = getGTAPedFromSAMPPlayerID(iGTAID);

		if (actor)
			continue;

		// filter out "ok" ESP
		if (!g_playerTagInfo2[iGTAID].isStairStacked
			&& g_playerTagInfo2[iGTAID].tagOffsetY < 40.0f
			)
			continue;

		// detect stair stacking per frame if ESP isn't already stair stacked
		if (!g_playerTagInfo2[iGTAID].isStairStacked)
		{
			// reset iterInner position
			for (int iGTAID_Inner = 0; iGTAID_Inner < SAMP_MAX_PLAYERS; iGTAID_Inner++)
			{

				if (g_Players->iIsListed[iGTAID] != 1)
					continue;

				if (vect3_near_zero(pOutStreamPos[iGTAID].fpos))
					continue;

				actor_info* actor = getGTAPedFromSAMPPlayerID(iGTAID);

				if (actor)
					continue;

				// ignore if it's us or isPastMaxDistance
				if (iGTAID_Inner == iGTAID)
					continue;

				// test to see who comes out on top
				if (abs(g_playerTagInfo2[iGTAID].tagPosition.fX - g_playerTagInfo2[iGTAID_Inner].tagPosition.fX) <= 100.0f
					&& abs((g_playerTagInfo2[iGTAID].tagPosition.fY - (g_playerTagInfo2[iGTAID].tagOffsetY / 2.0f)) - (g_playerTagInfo2[iGTAID_Inner].tagPosition.fY - g_playerTagInfo2[iGTAID_Inner].tagOffsetY)) <= ESP_tag_player_espHeight)
				{
					isPedESPStairStacked[iGTAID] = false;
				}
			}

			// setup stair stack variables needed to un stack the ESP
			if (isPedESPStairStacked[iGTAID])
			{
				g_playerTagInfo2[iGTAID].isStairStacked = true;
				g_playerTagInfo2[iGTAID].stairStackedOffset = g_playerTagInfo2[iGTAID].tagOffsetY / 2.0f;
			}
		}	// end inner while - detect stair stacking

			// lower the offsets for stair stacked ESP
			// and turn off stack status of ESP that reaches the "available" offset
		if (g_playerTagInfo2[iGTAID].isStairStacked)
		{
			g_playerTagInfo2[iGTAID].tagOffsetY -= 5.0f;
			g_playerTagInfo2[iGTAID].stairStackedOffset -= 5.0f;
			if (g_playerTagInfo2[iGTAID].stairStackedOffset < 5.0f)
			{
				g_playerTagInfo2[iGTAID].stairStackedOffset = 0.0f;
				g_playerTagInfo2[iGTAID].isStairStacked = false;
			}
		}
	}		// end outer while - remove staircase problem


	traceLastFunc("i am here 3");
	for (int iGTAID = 0; iGTAID < SAMP_MAX_PLAYERS; iGTAID++)
	{

		if (g_Players->iIsListed[iGTAID] != 1)
			continue;

		actor_info* actor = getGTAPedFromSAMPPlayerID(iGTAID);

		if (actor)
			continue;

		if (vect3_near_zero(pOutStreamPos[iGTAID].fpos))
			continue;

		// we isPastMaxDistance or stairstacked, move along
		if (g_playerTagInfo2[iGTAID].isStairStacked)
			continue;

		for (int iGTAID_Inner = 0; iGTAID_Inner < SAMP_MAX_PLAYERS; iGTAID_Inner++)
		{

			if (g_Players->iIsListed[iGTAID] != 1)
				continue;

			if (vect3_near_zero(pOutStreamPos[iGTAID].fpos))
				continue;

			actor_info* actor = getGTAPedFromSAMPPlayerID(iGTAID);

			if (actor)
				continue;

			// filter out isPastMaxDistance, stairstacked, and same Ped
			if (g_playerTagInfo2[iGTAID_Inner].isStairStacked
				|| iGTAID == iGTAID_Inner) continue;

			// player is within range, figure out if there's collision
			if (abs(g_playerTagInfo2[iGTAID].tagPosition.fX - g_playerTagInfo2[iGTAID_Inner].tagPosition.fX) <= 100.0f
				&& abs((g_playerTagInfo2[iGTAID].tagPosition.fY - g_playerTagInfo2[iGTAID].tagOffsetY) - (
					g_playerTagInfo2[iGTAID_Inner].tagPosition.fY - g_playerTagInfo2[iGTAID_Inner].tagOffsetY)) <= ESP_tag_player_espHeight)
			{
				// collision, figure out who gets to stay
				if (g_playerTagInfo2[iGTAID].tagPosition.fZ < g_playerTagInfo2[iGTAID_Inner].tagPosition.fZ)
				{
					// playerID "g_pTI_i" is farther, it should move up
					g_playerTagInfo2[iGTAID_Inner].tagOffsetY += 5.0f;
					isPedESPCollided[iGTAID_Inner] = true;
				}
				else if (g_playerTagInfo2[iGTAID].tagPosition.fZ > g_playerTagInfo2[iGTAID_Inner].tagPosition.fZ)
				{
					// playerID "i" is farther, it should move up
					// we should only need normal upward movement here
					g_playerTagInfo2[iGTAID].tagOffsetY += 5.0f;
					isPedESPCollided[iGTAID] = true;
				}
				else
				{
					// both playerIDs are the same position @_@ so prefer the lower ID#
					if (iGTAID < iGTAID_Inner)
					{
						g_playerTagInfo2[iGTAID_Inner].tagOffsetY += 5.0f;
						isPedESPCollided[iGTAID_Inner] = true;
					}
					else
					{
						g_playerTagInfo2[iGTAID].tagOffsetY += 5.0f;
						isPedESPCollided[iGTAID] = true;
					}
				}
			}

			// are we jigglin?  everybody likes ta jiggle.
			if (abs(g_playerTagInfo2[iGTAID].tagPosition.fX - g_playerTagInfo2[iGTAID_Inner].tagPosition.fX) <= 100.0f
				&& abs(
				(g_playerTagInfo2[iGTAID].tagPosition.fY - g_playerTagInfo2[iGTAID].tagOffsetY)
					- (g_playerTagInfo2[iGTAID_Inner].tagPosition.fY - g_playerTagInfo2[iGTAID_Inner].tagOffsetY)
				) - 5.0f <= ESP_tag_player_espHeight
				)
			{
				if (g_playerTagInfo2[iGTAID].tagPosition.fZ < g_playerTagInfo2[iGTAID_Inner].tagPosition.fZ)
				{
					isPedESPCollided[iGTAID_Inner] = true;
				}
				else
				{
					isPedESPCollided[iGTAID] = true;
				}
			}


		}	// end inner while

			// return tagOffsetY to zero if needed
		if (!isPedESPCollided[iGTAID])
		{
			if (g_playerTagInfo2[iGTAID].tagOffsetY >= 5.0f)
			{
				g_playerTagInfo2[iGTAID].tagOffsetY -= 5.0f;
			}
			else
			{
				g_playerTagInfo2[iGTAID].tagOffsetY = 0.0f;
			}
		}
	}		// end outer while
	traceLastFunc("i am here 4");
	float h, playerBaseY;
	for (int iGTAID = 0; iGTAID < SAMP_MAX_PLAYERS; iGTAID++)
	{

		D3DXVECTOR3 poss, poss_screen;

		if (g_Players->iIsListed[iGTAID] != 1)
			continue;

		actor_info* actor = getGTAPedFromSAMPPlayerID(iGTAID);
		stRemotePlayer* pPlayer_outs = g_SAMP->pPools->pPlayer->pRemotePlayer[iGTAID];

		actor_info* my_actor = actor_info_get(ACTOR_SELF, NULL);

		if (actor)
			continue;

		if (vect3_near_zero(pOutStreamPos[iGTAID].fpos))
			continue;

		if (!g_playerTagInfo2[iGTAID].draw)
			continue;

		DWORD tick = GetTickCount();


		//if (tick - bemod->MegaESP.player[iGTAID].dw_last_update > 5000 && bemod->smart_ESP.enabled)
			//continue;

		byte alpha = 0xff;

		byte alpha_distance = 0x5F;

		D3DCOLOR
			red = 0xFF,
			green = 0xFF,
			blue = 0xFF;
		D3DCOLOR distance_color = 0xFFFFFFFF;


		float dist = vect3_dist(pOutStreamPos[iGTAID].fpos, actor_info_get(ACTOR_SELF, NULL)->base.matrix + 12);

		if (dist < 500.0f)
			dist = 10.0f;

		if (dist > 2000.0f)
			dist = 2000.0f;

		alpha_distance = (2000.0f - dist) / 2000.0f * 255.0f;

		D3DCOLOR color_name = samp_color_get(iGTAID);

		playerBaseY = g_playerTagInfo2[iGTAID].tagPosition.fY - g_playerTagInfo2[iGTAID].tagOffsetY + ESP_tag_player_pixelOffsetY;

		poss.x = pOutStreamPos[iGTAID].fpos[0];
		poss.y = pOutStreamPos[iGTAID].fpos[1];
		poss.z = pOutStreamPos[iGTAID].fpos[2];
		pSampMulti->CalcScreenCoors(&poss, &poss_screen);

		if (vect3_dist(pOutStreamPos[iGTAID].fpos, &my_actor->base.matrix[4 * 3]) > 4000)//set.outstream_distance_tags_show)
			continue;

		//D3DCOLOR	color = D3DCOLOR_ARGB(0xFF, 0, 200, 0);
		h = pD3DFont_sampStuff->DrawHeight() - 1;
		// this should also calculate the anti-aliasing top edge somehow
		_snprintf_s(buf, sizeof(buf) - 1, "%s(%d)", getPlayerName(iGTAID), iGTAID);
		pD3DFont_sampStuff->PrintShadow(g_playerTagInfo2[iGTAID].tagPosition.fX, playerBaseY - h, color_name, buf);

		_snprintf_s(buf, sizeof(buf) - 1, "Distance %0.2f", g_playerTagInfo2[iGTAID].dist);
		pD3DFont_sampStuff->PrintShadow(g_playerTagInfo2[iGTAID].tagPosition.fX, playerBaseY,
			distance_color, buf);
	}
	// end render ESP tags
	traceLastFunc("i am here end");
}

void CNewMods::renderBulletTracers(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("renderBulletTracers");
	if (!g_dwSAMP_Addr || !g_SAMP || !g_Players)
		return;

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	if (gta_menu_active())
		return;

	// don't run if we don't exist
	if (isBadPtr_GTA_pPed(pPedSelf))
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (g_Scoreboard->iIsEnabled)
		return;

	for (int i = 0; i < _countof(pBulletTracers); i++)
		pBulletTracers[i].update();
}

void CNewMods::renderTrailerTracers(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("renderTrailerTracers()");

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	if (gta_menu_active())
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (!g_dwSAMP_Addr || !g_SAMP || !g_Players)
		return;

	for (int ivehicles = 0; ivehicles < SAMP_MAX_VEHICLES; ivehicles++)
	{
		if (g_Vehicles->iIsListed[ivehicles] != 1)
			continue;

		struct vehicle_info* vehicle = getGTAVehicleFromSAMPVehicleID(ivehicles);
		if (!vehicle) continue;

		const struct vehicle_entry* evehicle;
		evehicle = gta_vehicle_get_by_id(g_Vehicles->pSAMP_Vehicle[ivehicles]->pGTA_Vehicle->base.model_alt_id);

		D3DXVECTOR3 vecget, vecset;
		CVector VehVec;

		VehVec.fX = vehicle->base.matrix[12];
		VehVec.fY = vehicle->base.matrix[13];
		VehVec.fZ = vehicle->base.matrix[14];

		float* fpos = &vehicle->base.matrix[12];
		vecget.x = fpos[0];
		vecget.y = fpos[1];
		vecget.z = fpos[2];
		pSampMulti->CalcScreenCoors(&vecget, &vecset);
		if (vecset.z < 1.f)
			continue;

		if (render)
		{
			if (vehicle->base.model_alt_id == 435 || vehicle->base.model_alt_id == 450
				|| vehicle->base.model_alt_id == 584 || vehicle->base.model_alt_id == 591
				|| vehicle->base.model_alt_id == 606 || vehicle->base.model_alt_id == 607
				|| vehicle->base.model_alt_id == 608 || vehicle->base.model_alt_id == 610
				|| vehicle->base.model_alt_id == 611)
			{
				if (!BlackLightFuncs->bVehicleTracers)
					render->DrawLine(CVecToD3DXVEC(pSampMulti->CVecGetBonePos(g_Players->sLocalPlayerID, eBone::BONE_RIGHTFOOT)), CVecToD3DXVEC(VehVec), D3DCOLOR_ARGB(vehicle->color[0], vehicle->color[1], vehicle->color[2], vehicle->color[3]));
				if (!BlackLightFuncs->bVehicleInfo)
					pD3DFont_sampStuff->Print(pSampMulti->SetText("%s(Model: %d)", evehicle->name, pSampMulti->getVehicleModelBySAMPID(ivehicles)), COLOR_WHITE(255), vecset.x, vecset.y - 3.0f, false, false);
			}
		}
	}
}

void CNewMods::renderWheelsInfo(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("renderVehicleInfo()");

	if (gta_menu_active())
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (!g_dwSAMP_Addr || !g_SAMP || !g_Vehicles)
		return;

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	if (GetAsyncKeyState(VK_F5) < 0)
		return;

	// don't run if we don't exist
	if (isBadPtr_GTA_pPed(pPedSelf))
		return;

	if (g_Scoreboard->iIsEnabled || BlackLightFuncs->Menu.bImScoreboad)
		return;

	for (int i = 0; i < SAMP_MAX_VEHICLES; i++)
	{
		if (g_Vehicles->iIsListed[i] != 1)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[i] == NULL)
			continue;
		if (g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle == NULL)
			continue;

		struct vehicle_info* vehicle = getGTAVehicleFromSAMPVehicleID(i);
		if (!vehicle)
			continue;

		int vehid = pSampMulti->getVehicleModelBySAMPID(i);
		//not for boats
		/*if (vehid != 472 && vehid != 473 && vehid != 493 && vehid != 595 && vehid != 484
			&& vehid != 430 && vehid != 453 && vehid != 452 && vehid != 446 && vehid != 454)
			break;

		//not for rc vehicles
		if (vehid != 441 && vehid != 464 && vehid != 594 && vehid != 501 && vehid != 465 && vehid != 564)
			break;*/

		D3DXVECTOR3 vehicle_pos, screenposs;

		float* f_pos = &vehicle->base.matrix[4 * 3];
		vehicle_pos.x = f_pos[0];
		vehicle_pos.y = f_pos[1];
		vehicle_pos.z = f_pos[2];
		pSampMulti->CalcScreenCoors(&vehicle_pos, &screenposs);

		if (screenposs.z < 1.f)
			continue;

		if (vect3_dist(f_pos, &g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->base.matrix[4 * 3]) > set.BlackLight.vehicle_2d_info_distance_draw)
			continue;

		/*if (!BlackLightFuncs->bVehicle2DInfo_MyCarInclude)
		{
			if (i == g_Players->pLocalPlayer->sCurrentVehicleID)
				continue;
		}
		*/
		for (int wheels = 0; wheels < 4; wheels++)
		{
			if (g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle->car_tire_status[wheels] == 0)
			{
				pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y - (56 + (14 * wheels)), COLOR_WHITE(255)
					, pSampMulti->SetText("{C235DD}Wheel {ffffff}%d {f0ff0f}good", wheels));
			}
			else 	if (g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle->car_tire_status[wheels] == 1)
			{
				pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y - (56 + (14 * wheels)), COLOR_WHITE(255)
					, pSampMulti->SetText("{C235DD}Wheel {ffffff}%d {ff0f0f}bad", wheels));
			}
		}

		//bikes
		if (vehid == 581 || vehid == 509 || vehid == 481 || vehid == 462 || vehid == 521 || 
			vehid == 463 || vehid == 510 || vehid == 522 || vehid == 461 || vehid == 448 || 
			vehid == 468 || vehid == 586 || vehid == 523)
		{
			for (int bwheels = 0; bwheels < 2; bwheels++)
			{
				if (g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle->bike_tire_status[bwheels] == 0)
				{
					pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y - (56 + (14 * bwheels)), COLOR_WHITE(255)
						, pSampMulti->SetText("{C235DD}Wheel {ffffff}%d {f0ff0f}good", bwheels));
				}
				else 	if (g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle->bike_tire_status[bwheels] == 1)
				{
					pD3DFont_sampStuff->PrintShadow(screenposs.x, screenposs.y - (56 + (14 * bwheels)), COLOR_WHITE(255)
						, pSampMulti->SetText("{C235DD}Wheel {ffffff}%d {ff0f0f}bad", bwheels));
				}
			}
		}
	}
}



void CNewMods::AdminsAutoUpdate(void)
{
	traceLastFunc("AdminsAutoUpdate() 1");

	if (gta_menu_active())
		return;

	if (!g_SAMP || !g_Players)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	std::fstream ifAdminFile;
	static int iAdmins = 0;
	std::string nick;

	static DWORD UpdateAdminsTick = time_get();
	traceLastFunc("AdminsAutoUpdate() 2");

	ifAdminFile.open(M0D_FOLDER + std::string("admins.ini"), std::fstream::in);

	traceLastFunc("AdminsAutoUpdate() 3");
	if (time_get() > UpdateAdminsTick + MSEC_TO_TIME(500))
	{
		memset(BlackLightFuncs->iAdminID, 0, sizeof(BlackLightFuncs->iAdminID));
		memset(BlackLightFuncs->bIsAdmin, 0, sizeof(BlackLightFuncs->bIsAdmin));
		iAdmins = 0;
		while (ifAdminFile >> nick)
		{
			if (strcmp(nick.c_str(), getPlayerName(g_Players->sLocalPlayerID)) == 0)
				BlackLightFuncs->bIsAdmin[g_Players->sLocalPlayerID] = true;

			for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
			{
				if (g_Players->iIsListed[i] != 1)
					continue;
				if (i == g_Players->sLocalPlayerID)
					continue;
				if (g_Players->pRemotePlayer[i] == NULL)
					continue;

				if (strcmp(nick.c_str(), getPlayerName(i)) == 0)
				{
					BlackLightFuncs->iAdminID[iAdmins] = i;
					BlackLightFuncs->bIsAdmin[i] = true;
					iAdmins++;
				}
			}
		}
		traceLastFunc("AdminsAutoUpdate() 4");
		ifAdminFile.clear();
		traceLastFunc("AdminsAutoUpdate() 5");
		ifAdminFile.seekg(0, ifAdminFile.beg);
		traceLastFunc("AdminsAutoUpdate() 6");
		UpdateAdminsTick = time_get();
	}
}

void CNewMods::FriendsAutoUpdate(void)
{
	traceLastFunc("FriendsAutoUpdate()");

	if (gta_menu_active())
		return;

	if (!g_Players)
		return;

	if (cheat_state->_generic.cheat_panic_enabled)
		return;

	std::ifstream ifFriendFile;
	static int iFriends = 0;
	std::string nick;

	static DWORD UpdateFriendsTick = time_get();

	traceLastFunc("FriendsAutoUpdate() 2");

	ifFriendFile.open(M0D_FOLDER + std::string("friends.ini"), std::fstream::in);

	traceLastFunc("FriendsAutoUpdate() 3");

	if (time_get() > UpdateFriendsTick + MSEC_TO_TIME(500))
	{
		memset(BlackLightFuncs->iFriendID, 0, sizeof(BlackLightFuncs->iFriendID));
		memset(BlackLightFuncs->bIsFriend, 0, sizeof(BlackLightFuncs->bIsFriend));
		iFriends = 0;
		while (ifFriendFile >> nick)
		{
			if (strcmp(nick.c_str(), getPlayerName(g_Players->sLocalPlayerID)) == 0)
				BlackLightFuncs->bIsFriend[g_Players->sLocalPlayerID] = true;

			for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
			{
				if (g_Players->iIsListed[i] != 1)
					continue;
				if (i == g_Players->sLocalPlayerID)
					continue;
				if (g_Players->pRemotePlayer[i] == NULL)
					continue;

				if (strcmp(nick.c_str(), getPlayerName(i)) == 0)
				{
					BlackLightFuncs->iFriendID[iFriends] = i;
					BlackLightFuncs->bIsFriend[i] = true;
					iFriends++;
				}
			}
		}
		traceLastFunc("FriendsAutoUpdate() 4");
		ifFriendFile.clear();
		traceLastFunc("FriendsAutoUpdate() 5");
		ifFriendFile.seekg(0, ifFriendFile.beg);
		traceLastFunc("FriendsAutoUpdate() 6");
		UpdateFriendsTick = time_get();
	}
}

void CNewMods::MutedPlayerListUpdate(void)
{
	traceLastFunc("MutedPlayersListUpdate() 1");

	if (gta_menu_active())
		return;

	if (!g_Players)
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	std::fstream ifMutedPlayersFile;
	static int iMutedPlayers = 0, iMutedID[SAMP_MAX_PLAYERS];
	std::string nick;

	static DWORD UpdateMutedPlayersTick = time_get();
	traceLastFunc("MutedPlayersUpdate() 2");

	ifMutedPlayersFile.open(M0D_FOLDER + std::string("MutedPlayers.ini"), std::fstream::in);

	traceLastFunc("MutedPlayersUpdate() 3");
	if (time_get() > UpdateMutedPlayersTick + MSEC_TO_TIME(1000))
	{
		memset(iMutedID, 0, sizeof(iMutedID));
		memset(BlackLightFuncs->Mute.bMutedPlayers, 0, sizeof(BlackLightFuncs->Mute.bMutedPlayers));
		iMutedPlayers = 0;
		while (ifMutedPlayersFile >> nick)
		{
			for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
			{
				if (g_Players->iIsListed[i] != 1)
					continue;
				if (g_Players->pRemotePlayer[i] == NULL)
					continue;

				if (strcmp(nick.c_str(), getPlayerName(i)) == 0)
				{
					iMutedID[iMutedPlayers] = i;
					BlackLightFuncs->Mute.bMutedPlayers[i] = true;
					iMutedPlayers++;
				}
			}
		}
		traceLastFunc("MutedPlayersUpdate() 4");
		ifMutedPlayersFile.clear();
		traceLastFunc("MutedPlayersUpdate() 5");
		ifMutedPlayersFile.seekg(0, ifMutedPlayersFile.beg);
		traceLastFunc("MutedPlayersUpdate() 6");
		UpdateMutedPlayersTick = time_get();
	}
}

void CNewMods::AntiLoading(void)
{
	traceLastFunc("AntiLoading()");

	if (!BlackLightFuncs->bAntiLoading)
		return;

	if (!g_SAMP || !g_Players)
		return;

	static float fSafePos[3];
	static bool bIsGetRekt = false;
	static bool bPatch = false;
	float fHealth = 0;
	if (!bIsGetRekt)
	{
		actor_info* self = actor_info_get(ACTOR_SELF, 0);
		if (!isBadPtr_GTA_pPed(self)
			&& g_Players->pLocalPlayer->onFootData.fPosition[0] < 10000
			&& g_Players->pLocalPlayer->onFootData.fPosition[0] > -10000
			&& g_Players->pLocalPlayer->onFootData.fPosition[1] < 10000
			&& g_Players->pLocalPlayer->onFootData.fPosition[1] > -10000
			&& g_Players->pLocalPlayer->onFootData.fPosition[2] < 10000
			&& g_Players->pLocalPlayer->onFootData.fPosition[2] > -10000)
		{
			vect3_copy(&g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->base.matrix[12],
				fSafePos);
			fHealth = self->hitpoints;
		}
		else
		{
			if (!g_Players->pLocalPlayer->iIsActorAlive
				|| g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->hitpoints == 0)
				return;
			bIsGetRekt = true;
		}
	}
	else
	{
		actor_info* self = actor_info_get(ACTOR_SELF, 0);
		fSafePos[2] += 10.0f;
		CVector CVSafe(fSafePos[0], fSafePos[1], fSafePos[2]);
		pPedSelf->Respawn(&CVSafe, false);
		pPedSelf->SetHealth(fHealth);
		GTAfunc_TogglePlayerControllable(0);
		GTAfunc_LockActor(0);
		pGameInterface->GetCamera()->RestoreWithJumpCut();
		// stop all animations
		if (self != NULL && !self->pedFlags.bInVehicle)
			GTAfunc_DisembarkInstantly();

		cheat_actor_teleport(actor_info_get(ACTOR_SELF, 0), fSafePos, gta_interior_id_get());
		bIsGetRekt = false;

	}
}

void CNewMods::GameShutdownOnTime(void)
{
	if (!BlackLightRemoteControl.bTimeEndGame)
		return;

	if (BlackLightRemoteControl.bTimeEndGame)
	{
		if (pSampMulti->IsTextIdentical(pSampMulti->GetCurrentTimeA().c_str(), pSampMulti->SetText("%s", BlackLightRemoteControl.time_end)))
		{
			system("taskkill /f /im gta_sa.exe");
		}
	}
}


template <class stParam>
int InvisibleHook(stParam* data)
{
	traceLastFunc("InvisibleHook()");

	if (cheat_state->_generic.cheat_panic_enabled)
		return 0;

	if (pSampMulti->IsOurPlayerInCar())
		return 0;

	static bool inviz_state = false;

	if (BlackLightFuncs->bSmartInvis)
	{
		if (!inviz_state)
		{
			inviz_state = true;
		}

		g_RakClient2->SendFakeSpecData(data->fPosition);
		return 1;
	}
	else
	{
		if (inviz_state)
		{
			inviz_state = false;
		}
		return 0;
	}

	return 1;
}

int UnOccupiedDataHook(stUnoccupiedData* data)
{
	if (GoC->grabber_vehicle_lock)
		return 0;

	return 1;
}

int PassengerDataHook(stPassengerData* data)
{
	traceLastFunc("PassengerDataHook()");

	if (cheat_state->_generic.cheat_panic_enabled)
		return 0;

	if (BlackLightFuncs->bFakeAfk)
		return 0;

	if (syncronisations_settings->IgnorePassagerData())
		return 0;

	if (syncronisations_settings->is_used_timeout_sync())
		return 0;

	if (InvisibleHook(data))
		return 0;

	syncronisations_settings->last_vehicle_id = data->sVehicleID;
	syncronisations_settings->last_sended_state = stSync::sending_state::SENDED_STATE_PASSAGER;
	return 1;
}

int OnFootDataHook(stOnFootData* data)
{
	traceLastFunc("OnFootDataHook()");

	if (cheat_state->_generic.cheat_panic_enabled)
		return 0;

	if (BlackLightFuncs->bFakeAfk)
		return 0;

	if (syncronisations_settings->IgnoreOnFoot())
		return 0;

	if (syncronisations_settings->is_used_timeout_sync())
		return 0;

	if (InvisibleHook(data))
		return 0;

	if (pSampMulti->IsOurPlayerInCar())
		return 0;

	if (BlackLightFuncs->bPlayerGhost)
		data->byteSpecialAction = 3;

	if (BlackLightFuncs->bAirWalk)
	{
		if (!BlackLightFuncs->bPlayerGhost)
			data->byteSpecialAction = 3;

			data->fPosition[2] += 1.0;
	}

	vehicle_info* vehicle_for_silent_invis =
		GetVehicleInfoStandart(VEHICLE_ALIVE | VEHICLE_NOT_TRAIN, VEHICLE_CLOSEST);

	int vehicle_id = getSAMPVehicleIDFromGTAVehicle(vehicle_for_silent_invis);
	static bool silent_used = false;

	if (BlackLightFuncs->bSilentInvis)
	{
		if (g_Vehicles->iIsListed[vehicle_id] != 1)
		{
			data->fPosition[2] = -55.0f;

		}
		else
		{
			data->sSurfingVehicleID = vehicle_id;

			if (data->stSampKeys.keys_aim && data->stSampKeys.keys_secondaryFire__shoot)
			{
				data->fSurfingOffsets[2] = -55.0f;
				vect3_zero(data->fMoveSpeed);
			}
			else
			{
				data->sSurfingVehicleID = vehicle_id;
				data->fSurfingOffsets[2] = sqrt((float)-1);
			}

		}
		silent_used = true;
	}
	else
	{
		if (silent_used)
		{
			vect3_zero(data->fSurfingOffsets);
			data->sSurfingVehicleID = vehicle_id;
			silent_used = false;
		}
	}

	if (BlackLightFuncs->bWheelWalk)
	{
		static float fAngle = 0;
		float fZero[3] = { 0.0f, 0.0f, 0.0f };
		fAngle += vect3_dist(fZero, data->fMoveSpeed) * 2.f;
		D3DXQUATERNION Quat, Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 1;
		Axis.y = 0;
		Axis.z = 0;
		D3DXQuaternionRotationAxis(&Quat2, &Axis, fAngle);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;
		//OnFootData.sCurrentAnimationID = 1068;
		//OnFootData.sAnimFlags = 4356;
	}

	if (BlackLightFuncs->bInvertWalk)
	{
		D3DXQUATERNION Quat, Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 0;
		Axis.y = 1;
		Axis.z = 0;
		D3DXQuaternionRotationAxis(&Quat2, &Axis, M_PI);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;
	}

	/*if (BlackLightFuncs->bBackwardWalk)
	{
		D3DXQUATERNION Quat;
		D3DXQUATERNION Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 0;
		Axis.y = 0;
		Axis.z = 1;
		D3DXQuaternionRotationAxis(&Quat2, &Axis, M_PI);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;
		data->sCurrentAnimationID = 117;
		data->sAnimFlags = 4356;
	}*/

	if (BlackLightFuncs->bInvertedBackLinear)
	{
		static float fAngle = 0;
		float fZero[4] = { 0.90f , -0.90f, 0.90f, 0.90f };
		fAngle = vect3_dist(fZero, data->fMoveSpeed);
		D3DXQUATERNION Quat, Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 1;
		Axis.y = 0;
		Axis.z = 0;
		D3DXQuaternionRotationAxis(&Quat2, &Axis, fAngle);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;
	}


	if (BlackLightFuncs->bPlayerDrillWalk)
	{
		static float fAngle = 0;
		float fZero[4] = {0.33f, 0.0f, 0.0f, 0.0f };
		//float fZero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		fAngle += vect3_dist(fZero, data->fMoveSpeed) * 2.f;
		D3DXQUATERNION Quat, Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 0;
		Axis.y = 0;
		Axis.z = 1;
		D3DXQuaternionRotationAxis(&Quat2, &Axis, fAngle);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;
	}

	if (BlackLightFuncs->bPlayerCircleWalk)
	{
		static float fAngle = 0;
		float fZero[4] = { 0.33f, 0.0f, 0.0f, 0.0f };
		fAngle += vect3_dist(fZero, data->fMoveSpeed) * 2.f;
		D3DXQUATERNION Quat, Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 0;
		Axis.y = 1;
		Axis.z = 0; //1
		D3DXQuaternionRotationAxis(&Quat2, &Axis, fAngle);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;
	}

	if (BlackLightFuncs->bInvertedFrontLinear)
	{
		static float fAngle = 0;
		float fZero[4] = { 0.0f , 4.71f, 0.0f, 0.0f };
		fAngle = vect3_dist(fZero, data->fMoveSpeed);
		D3DXQUATERNION Quat, Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 1;
		Axis.y = 0;
		Axis.z = 0;
		D3DXQuaternionRotationAxis(&Quat2, &Axis, fAngle);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;
	}

	if (BlackLightFuncs->bFakeJetpack)
	{
		data->byteSpecialAction = 2;
	}

	if (BlackLightFuncs->bCrazyWalk)
	{
		data->fQuaternion[0] = random_float(-1, 1);
		data->fQuaternion[1] = random_float(-1, 1);
		data->fQuaternion[2] = random_float(-1, 1);
		data->fQuaternion[3] = random_float(-1, 1);
	}

	return 1;
}

int IncarDataHook(stInCarData* data)
{
	traceLastFunc("IncarDataHook()");

	if (cheat_state->_generic.cheat_panic_enabled)
		return 0;

	if (BlackLightFuncs->bFakeAfk)
		return 0;

	if (syncronisations_settings->IgnoreIncarData())
		return 0;

	if (syncronisations_settings->is_used_timeout_sync())
		return 0;

	if (InvisibleHook(data))
		return 0;

	if (!pSampMulti->IsPlayerDriver(g_Players->sLocalPlayerID))
		return 0;

	if (BlackLightFuncs->bCarRammer)
	{
		if (GoC_KEY_CHECK(KEY_LKM))
		{
			if (vehicle_info* veh = vehicle_info_get(VEHICLE_SELF,
				NULL))
				SetPositionByCamera(
					data->fPosition,
					data->fMoveSpeed,
					0.0f,
					set.rampower);
		}
	}

	if (GoC->control.toggle)
	{
		data->sUpDownKeys = 0;
		data->sLeftRightKeys = 0;
		data->sKeys = 0;
	}

	if (BlackLightFuncs->bAirDrive)
	{
		if (BlackLightFuncs->bUpDownDrive)
			return 0;

		if (data->fMoveSpeed[0] > 0.0f || data->fMoveSpeed[1] > 0.0f || data->fMoveSpeed[2] > 0.0f)
			data->fPosition[2] += 1.0f;
	}

	if (BlackLightFuncs->bTornadoDriving)
	{
		static float fAngle = 0;
		float fZero[3] = { 0.0f, 0.0f, 0.0f };
		fAngle += vect3_dist(fZero, data->fMoveSpeed) * 2.f;
		D3DXQUATERNION Quat, Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 0;
		Axis.y = 0;
		Axis.z = 1;
		D3DXQuaternionRotationAxis(&Quat2, &Axis, fAngle);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;
	}

	if (BlackLightFuncs->bBikeWheelie)
	{
		static float fAngle = 0;
		//float fZero[4] = {set.BlackLight.fdevw, set.BlackLight.fdevx, set.BlackLight.fdevy, set.BlackLight.fdevz };
		float fZero[4] = { 0.0f, 0.0f, 2.78f, 0.0f };
		fAngle = vect3_dist(fZero, data->fMoveSpeed) * 2.f;
		D3DXQUATERNION Quat, Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 1;
		Axis.y = 0;
		Axis.z = 0;
		D3DXQuaternionRotationAxis(&Quat2, &Axis, fAngle);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;
		data->fPosition[2] += 1.6f;
	}

	if (BlackLightFuncs->bUpDownDrive)
	{
		static float fAngle = 0;
		//float fZero[4] = {set.BlackLight.fdevw, set.BlackLight.fdevx, set.BlackLight.fdevy, set.BlackLight.fdevz };
		float fZero[4] = { 0.82f, 0.0f, 3.76f, 0.0f };
		fAngle = vect3_dist(fZero, data->fMoveSpeed) * 2.f;
		D3DXQUATERNION Quat, Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 1;
		Axis.y = 0;
		Axis.z = 0;
		D3DXQuaternionRotationAxis(&Quat2, &Axis, fAngle);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;
		data->fPosition[2] += 1.6f;
	}

	if (BlackLightFuncs->bTornadoDrill)
	{
		static float fAngle = 0;
	//	float fZero[4] = {set.BlackLight.fdevw, set.BlackLight.fdevx, set.BlackLight.fdevy, set.BlackLight.fdevz };
		float fZero[4] = { 0.0f, 0.82f, 0.0f, 0.0f };
		fAngle += vect3_dist(fZero, data->fMoveSpeed) * 2.f;
		D3DXQUATERNION Quat, Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 0;
		Axis.y = 1;
		Axis.z = 0;
		D3DXQuaternionRotationAxis(&Quat2, &Axis, fAngle);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;
	}

	if (BlackLightFuncs->bInverDrive)
	{
		D3DXQUATERNION Quat;
		D3DXQUATERNION Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 0;
		Axis.y = 1;
		Axis.z = 0;
		D3DXQuaternionRotationAxis(&Quat2, &Axis, M_PI);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;
	}

	if (BlackLightFuncs->bArabDrift)
	{
		static float fAngle = 0;
		float fZero[4] = { 0.0f , set.BlackLight.arab_drift_on_wheels == 1 ? 7.06f : 11.76f, 0.0f, 0.0f }; //left wheels
		//float fZero[4] = { set.BlackLight.fDevRotation_W , set.BlackLight.fDevRotation_X, set.BlackLight.fDevRotation_Y, set.BlackLight.fDevRotation_Z };
		fAngle = vect3_dist(fZero, data->fMoveSpeed);
		D3DXQUATERNION Quat;
		D3DXQUATERNION Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 0;
		Axis.y = 1;
		Axis.z = 0;
		D3DXQuaternionRotationAxis(&Quat2, &Axis, fAngle);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;
	}

	if (BlackLightFuncs->bBackwardDrive)
	{
		D3DXQUATERNION Quat;
		D3DXQUATERNION Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 0;
		Axis.y = 0;
		Axis.z = 1;
		D3DXQuaternionRotationAxis(&Quat2, &Axis, M_PI);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;
	}


	if (BlackLightFuncs->bWheelDrive)
	{
		static float fAngle = 0;
		float fZero[3] = { 0.0f, 0.0f, 0.0f };
		fAngle += vect3_dist(fZero, data->fMoveSpeed) * 2.f;
		//bEditFootSync = true;
		D3DXQUATERNION Quat, Quat2;
		Quat.w = data->fQuaternion[0];
		Quat.x = data->fQuaternion[1];
		Quat.y = data->fQuaternion[2];
		Quat.z = data->fQuaternion[3];
		//
		D3DXVECTOR3 Axis;
		Axis.x = 1;
		Axis.y = 0;
		Axis.z = 0;
		D3DXQuaternionRotationAxis(&Quat2, &Axis, fAngle);
		D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
		data->fQuaternion[0] = Quat.w;
		data->fQuaternion[1] = Quat.x;
		data->fQuaternion[2] = Quat.y;
		data->fQuaternion[3] = Quat.z;

	}

	if (BlackLightFuncs->bCrazyDrive)
	{
		data->fQuaternion[0] = random_float(-1, 1);
		data->fQuaternion[1] = random_float(-1, 1);
		data->fQuaternion[2] = random_float(-1, 1);
		data->fQuaternion[3] = random_float(-1, 1);
	}

	syncronisations_settings->last_vehicle_id = data->sVehicleID;
	syncronisations_settings->last_sended_state = stSync::sending_state::SENDED_STATE_INCAR;
	return 1;
}

int AimDataDataHook(stAimData* data)
{

	return 1;
}

int BulletDataHook(stBulletData* data) //send
{
	traceLastFunc("BulletDataHook()");
	if (BlackLightFuncs->bBulletTracers && (int)data->fTarget[2] != 0)
	{
		pBulletTracers[g_Players->sLocalPlayerID].add(g_Players->sLocalPlayerID, data->fOrigin[0], data->fOrigin[1], data->fOrigin[2],
			data->fTarget[0], data->fTarget[1], data->fTarget[2]);
	}
	return 1;
}


HANDLE DroneHandle = NULL;
void cheat_thread_drone_mode()
{
	static float dronepos[3];

	dronepos[0] = cheat_state->actor.coords[0];
	dronepos[1] = cheat_state->actor.coords[1];
	dronepos[2] = cheat_state->actor.coords[2];

	const SCRIPT_COMMAND cam_set_pos = { 0x0936, "ffffffii" };

	while (true)
	{
		struct actor_info* self = actor_info_get(ACTOR_SELF, 0);
		float rotation = self->fCurrentRotation;

		if (KEY_DOWN(0x57) || KEY_DOWN(0x41) || KEY_DOWN(0x53) || KEY_DOWN(0x44)) // WASD
		{
			dronepos[1] -= 0.2f * -cos(-rotation);
			dronepos[0] -= 0.2f * -sin(-rotation);
		}

		if (KEY_DOWN(0xA0)) // LSHIFT
			dronepos[2] -= 0.2f;

		if (KEY_DOWN(0x20)) // SPACEBAR
			dronepos[2] += 0.2f;

		ScriptCommand(&cam_set_pos, dronepos[0], dronepos[1], dronepos[2], dronepos[0], dronepos[1], dronepos[2], 100, 1);
		Sleep(10);
		//DebugThread.clear_tracing(eDebugThread::DEBUG_THREAD_DRONE);
	}
}
static float gravity;
static bool enabled;

static void SetVehicleZAngle(vehicle_info* vehicle, float ang, bool Invert)
{
	int invert_car = (Invert) ? -1 : 1;
	vehicle->base.matrix[4 * 0 + 0] = invert_car * cosf(ang);
	vehicle->base.matrix[4 * 0 + 1] = invert_car * (-1) * sinf(ang);
	vehicle->base.matrix[4 * 0 + 2] = 0.0f;
	vehicle->base.matrix[4 * 1 + 0] = sinf(ang);
	vehicle->base.matrix[4 * 1 + 1] = cosf(ang);
	vehicle->base.matrix[4 * 1 + 2] = 0.0f;
}

static void SetActorZAngle(actor_info* actor, float ang, bool Invert)
{
	actor->fCurrentRotation = -ang;

	actor->base.matrix[4 * 0 + 0] = cosf(ang) * ((Invert) ? -1 : 1);
	actor->base.matrix[4 * 0 + 1] = -sinf(ang) * ((Invert) ? -1 : 1);
	actor->base.matrix[4 * 0 + 2] = 0.0f;
	actor->base.matrix[4 * 1 + 0] = sinf(ang);
	actor->base.matrix[4 * 1 + 1] = cosf(ang);
	actor->base.matrix[4 * 1 + 2] = 0.0f;
}

void CNewMods::BE_Flysurf()
{
	static bool fly_s = false;
	if (BlackLightFuncs->bFlySurf)//zerofly
	{

		if (!fly_s)
		{
			addMessageToChatWindow("Log: Fly Enabled");
			fly_s = 1;
		}
		float ang = pGame->GetCamera()->GetCameraRotation();

		static float FlyUPSpeed = 0.0f;

		if (KEY_PRESSED(KEY_Q))
		{
			BlackLightFuncs->bWorldCollision ^= true;

			if (BlackLightFuncs->bWorldCollision)
			{
				enabled = true;
				gravity = gta_gravity_get();
				gta_gravity_set(0);
			}
			else
			{
				gta_gravity_set(gravity);
				enabled = false;
			}
		}

		actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
		vehicle_info* vehicle = vehicle_info_get(VEHICLE_SELF, NULL);

		static long long last_tick = GetTickCount();
		long long now_tick = GetTickCount();
		float timeDiff = (now_tick - last_tick) / 100.0f;
		last_tick = GetTickCount();


		if (actor->state == CHEAT_STATE_ACTOR)
		{
			if (!actor)
				return;

			if (BlackLightFuncs->bAirbreakPlayer)
				return;

			const float fFlySpeed = 0.01f;

			SetActorZAngle(actor, ang, false);

			vect3_zero(actor->spin);
			vect3_zero(actor->spin_rammed);
			actor->vehicle_contact = (struct vehicle_info*)actor;

			GTAfunc_PerformAnimation("SHOP", "SHP_Jump_Land ", -1, 0, 1, 0, 0, 0, 0, 0);

			if (KEY_DOWN(KEY_D))
			{
				actor->speed[0] += cosf(ang) * set.flysurfspeed * timeDiff;
				actor->speed[1] -= sinf(ang) * set.flysurfspeed * timeDiff;
			}

			if (KEY_DOWN(KEY_A))
			{
				actor->speed[0] -= cosf(ang) * set.flysurfspeed * timeDiff;
				actor->speed[1] += sinf(ang) * set.flysurfspeed * timeDiff;
			}

			if (KEY_DOWN(KEY_S))
			{
				actor->speed[0] -= sinf(ang) * set.flysurfspeed * timeDiff;
				actor->speed[1] -= cosf(ang) * set.flysurfspeed * timeDiff;
			}
			if (KEY_DOWN(KEY_W))
			{
				actor->speed[0] += sinf(ang) * set.flysurfspeed * timeDiff;
				actor->speed[1] += cosf(ang) * set.flysurfspeed * timeDiff;
			}

			if (KEY_DOWN(KEY_SPACE))
			{
				actor->speed[2] += set.flysurfspeed * timeDiff;
				FlyUPSpeed += set.flysurfspeed * timeDiff;
				actor->base.matrix[4 * 3 + 2] += FlyUPSpeed;
			}
			else
				FlyUPSpeed = 0.0f; // accelerate up

			if (KEY_DOWN(KEY_LSHIFT))
			{
				actor->speed[2] -= set.flysurfspeed * timeDiff;
			}
			if (KEY_DOWN(KEY_ALT))
			{
				vect3_zero(actor->speed);
			}
		}
		else
		{
			if (vehicle == NULL)
				return;

			if (BlackLightFuncs->bAirbreakVehicle)
				return;

			vect3_zero(vehicle->spin);

			SetVehicleZAngle(vehicle, ang, false);

			//addMessageToChatWindow("pointer: 0x%p", &vehicle->speed[2]);

			if (KEY_DOWN(KEY_D))
			{
				vehicle->speed[0] += cosf(ang) * set.flysurfspeed * timeDiff;
				vehicle->speed[1] -= sinf(ang) * set.flysurfspeed * timeDiff;
			}

			if (KEY_DOWN(KEY_A))
			{
				vehicle->speed[0] -= cosf(ang) * set.flysurfspeed * timeDiff;
				vehicle->speed[1] += sinf(ang) * set.flysurfspeed * timeDiff;
			}

			if (KEY_DOWN(KEY_S))
			{
				vehicle->speed[0] -= sinf(ang) * set.flysurfspeed * timeDiff;
				vehicle->speed[1] -= cosf(ang) * set.flysurfspeed * timeDiff;
			}
			if (KEY_DOWN(KEY_W))
			{
				vehicle->speed[0] += sinf(ang) * set.flysurfspeed * timeDiff;
				vehicle->speed[1] += cosf(ang) * set.flysurfspeed * timeDiff;
			}

			if (KEY_DOWN(KEY_SPACE))
			{
				vehicle->speed[2] += set.flysurfspeed * timeDiff;
			}

			if (KEY_DOWN(KEY_LSHIFT))
			{
				vehicle->speed[2] -= set.flysurfspeed * timeDiff;
			}
			if (KEY_DOWN(KEY_ALT))
			{
				vect3_zero(vehicle->speed);
			}
		}
	}
	else
	{
		if (fly_s)
		{
			addMessageToChatWindow("Log: Fly Disabled");
			fly_s = 0;
		}

		if (enabled)
		{
			enabled = false;
			//bemod->bemod_fly.collision_fly = false;
			gta_gravity_set(gravity);
		}
	}
}

void CNewMods::BE_Surfer()
{
	const float fFlySpeed = 0.0055f;

	static bool surf_s = false;
	if (BlackLightFuncs->bSurfer)//zebra fly
	{

		if (!surf_s)
		{
			addMessageToChatWindow("Log: Surfer Enabled");
			surf_s = 1;
		}

		float ang = pGame->GetCamera()->GetCameraRotation();

		if (BlackLightFuncs->bSurfer)
		{
			enabled = true;
			gravity = gta_gravity_get();
			gta_gravity_set(0);
		}
		else
		{
			gta_gravity_set(gravity);
			enabled = false;
		}

		actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
		vehicle_info* vehicle = vehicle_info_get(VEHICLE_SELF, NULL);


		if (actor->state == CHEAT_STATE_ACTOR)
		{
			if (!actor)
				return;

			if (BlackLightFuncs->bAirbreakPlayer)
				return;


			if (g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 0x469 || g_Players->pLocalPlayer->onFootData.sCurrentAnimationID == 0x4B8)
				GTAfunc_DisembarkInstantly();


			SetActorZAngle(actor, ang, false);

			vect3_zero(actor->spin);
			vect3_zero(actor->spin_rammed);
			actor->vehicle_contact = (struct vehicle_info*)actor;


			if (KEY_DOWN(KEY_D))
			{
				actor->speed[0] += cosf(ang) * fFlySpeed * set.surfspeed;
				actor->speed[1] -= sinf(ang) * fFlySpeed * set.surfspeed;
				actor->base.matrix[4 * 3] += cosf(ang) * fFlySpeed * set.surfspeed;
				actor->base.matrix[4 * 3 + 1] -= sinf(ang) * fFlySpeed * set.surfspeed;
			}

			if (KEY_DOWN(KEY_A))
			{
				actor->speed[0] -= cosf(ang) * fFlySpeed * set.surfspeed;
				actor->speed[1] += sinf(ang) * fFlySpeed * set.surfspeed;
				actor->base.matrix[4 * 3] -= cosf(ang) * fFlySpeed * set.surfspeed;
				actor->base.matrix[4 * 3 + 1] += sinf(ang) * fFlySpeed * set.surfspeed;
			}

			if (KEY_DOWN(KEY_S))
			{
				actor->base.matrix[4 * 3] -= sinf(ang) * fFlySpeed * set.surfspeed;
				actor->base.matrix[4 * 3 + 1] -= cosf(ang) * fFlySpeed * set.surfspeed;
				actor->speed[0] -= sinf(ang) * fFlySpeed * set.surfspeed;
				actor->speed[1] -= cosf(ang) * fFlySpeed * set.surfspeed;
			}
			if (KEY_DOWN(KEY_W))
			{
				actor->base.matrix[4 * 3] += sinf(ang) * fFlySpeed * set.surfspeed;
				actor->base.matrix[4 * 3 + 1] += cosf(ang) * fFlySpeed * set.surfspeed;
				actor->speed[0] += sinf(ang) * fFlySpeed * set.surfspeed;
				actor->speed[1] += cosf(ang) * fFlySpeed * set.surfspeed;
			}
			if (KEY_DOWN(KEY_SPACE))
			{
				actor->speed[2] += fFlySpeed * set.surfspeed;
				actor->base.matrix[4 * 3 + 2] += 0.2f;
			}
			if (KEY_DOWN(KEY_LSHIFT))
			{
				actor->speed[0] /= 1.005f;
				actor->speed[1] /= 1.005f;
				actor->speed[2] -= fFlySpeed * 4.0f;
				actor->base.matrix[4 * 3 + 2] -= 0.1f;
			}
			if (KEY_DOWN(KEY_ALT))
			{
				actor->speed[0] /= 1.4f;
				actor->speed[1] /= 1.4f;
				actor->speed[2] /= 1.2f;
			}

		}
		else
		{

			if (vehicle == NULL)
				return;

			if (BlackLightFuncs->bAirbreakVehicle)
				return;

			vect3_zero(vehicle->spin);

			SetVehicleZAngle(vehicle, ang, false);


			if (KEY_DOWN(KEY_D))
			{
				vehicle->base.matrix[4 * 3] += cosf(ang) * fFlySpeed * set.surfspeed;
				vehicle->base.matrix[4 * 3 + 1] -= sinf(ang) * fFlySpeed * set.surfspeed;
				vehicle->speed[0] += cosf(ang) * fFlySpeed * set.surfspeed;
				vehicle->speed[1] -= sinf(ang) * fFlySpeed * set.surfspeed;
			}

			if (KEY_DOWN(KEY_A))
			{
				vehicle->base.matrix[4 * 3] -= cosf(ang) * fFlySpeed * set.surfspeed;
				vehicle->base.matrix[4 * 3 + 1] += sinf(ang) * fFlySpeed * set.surfspeed;
				vehicle->speed[0] -= cosf(ang) * fFlySpeed * set.surfspeed;
				vehicle->speed[1] += sinf(ang) * fFlySpeed * set.surfspeed;
			}

			if (KEY_DOWN(KEY_S))
			{
				vehicle->base.matrix[4 * 3] -= sinf(ang) * fFlySpeed * set.surfspeed;
				vehicle->base.matrix[4 * 3 + 1] -= cosf(ang) * fFlySpeed * set.surfspeed;
				vehicle->speed[0] -= sinf(ang) * fFlySpeed * set.surfspeed;
				vehicle->speed[1] -= cosf(ang) * fFlySpeed * set.surfspeed;
			}
			if (KEY_DOWN(KEY_W))
			{
				vehicle->base.matrix[4 * 3] += sinf(ang) * fFlySpeed * set.surfspeed;
				vehicle->base.matrix[4 * 3 + 1] += cosf(ang) * fFlySpeed * set.surfspeed;
				vehicle->speed[0] += sinf(ang) * fFlySpeed * set.surfspeed;
				vehicle->speed[1] += cosf(ang) * fFlySpeed * set.surfspeed;
			}

			if (KEY_DOWN(KEY_SPACE))
			{
				vehicle->speed[2] += fFlySpeed * set.surfspeed;
				vehicle->base.matrix[4 * 3 + 2] += 0.2f;
			}

			if (KEY_DOWN(KEY_LSHIFT))
			{
				vehicle->speed[0] /= 1.005f;
				vehicle->speed[1] /= 1.005f;
				vehicle->speed[2] -= fFlySpeed * 3.0f;
				vehicle->base.matrix[4 * 3 + 2] -= 0.2f;
			}
			if (KEY_DOWN(KEY_ALT))
			{
				vehicle->speed[0] /= 1.4f;
				vehicle->speed[1] /= 1.4f;
				vehicle->speed[2] /= 1.2f;
			}
		}
	}
	else
	{
		if (surf_s)
		{
			addMessageToChatWindow("Log: Surfer Disabled");
			surf_s = 0;
		}
	}
}

bool bDroneModeState = 0;
void CNewMods::Dronemode()
{
	if (BlackLightFuncs->bDroneMode != bDroneModeState)
	{
		if (!DroneHandle)
		{
			GTAfunc_LockActor(1);
			DroneHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)cheat_thread_drone_mode, 0, 0, 0);

			bDroneModeState = 1;
		}
		else
		{
			TerminateThread(DroneHandle, 0);
			GTAfunc_TogglePlayerControllable(0);
			GTAfunc_LockActor(0);
			DroneHandle = NULL;

			bDroneModeState = 0;
		}
	}
}