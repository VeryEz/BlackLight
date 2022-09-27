#include "main.h"

ImVec4
ImColorRed = ImVec4(0.882f, 0.035f, 0.066f, 1.000f),
ImColorGreen = ImVec4(0.105f, 0.733f, 0.027f, 1.000f),
ImColorOrange = ImVec4(1.000f, 0.807f, 0.483f, 1.000f), //menu text
ImColorBanana = ImVec4(1.000f, 0.898f, 0.726f, 1.000f), //menus
ImColorTransparentBlack = ImVec4(0.000f, 0.000f, 0.000f, 0.741f), //window background
ImColorDeepOrange = ImVec4(1.000f, 0.537f, 0.000f, 1.000f),
ImColorWhite = ImVec4(1.000f, 1.000f, 1.000f, 1.000f),
ImColorNone = ImVec4(0.000f, 0.000f, 0.000f, 0.000f),
ImColorSlider = ImVec4(0.348f, 0.348f, 0.348f, 0.557f),
ImColorSliderHovered = ImVec4(0.428f, 0.428f, 0.428f, 0.577f),
ImColorSliderActive  = ImVec4(0.552f, 0.552f, 0.552f, 0.761f),
ImColorSliderGrab = ImVec4(0.033f, 0.597f, 0.488f, 1.000f),
ImColorSliderGrabActive = ImVec4(0.013f, 0.652f, 0.662f, 1.000f),
ImColorAqua = ImVec4(0.010f, 0.550f, 0.620f, 1.000f),
ImColorBlue = ImVec4(0.010f, 0.390f, 0.550f, 1.000f);


CBlackLightMenu* pMenu;
int menus = 0, inner_menus = 0;
static HSTREAM sample;

std::fstream radio_file; //radio file read
std::fstream file_friends; //friends file read
std::fstream file_admins; //admins file read

static float		fpsDisplay, fpsBuf1, fpsBuf2, fpsBuf3, fpsBuf4;
uint32_t	fps_time = GetTickCount();
static int			fpsFrameCounter;

static float getFPS()
{
	// increment frame counter
	fpsFrameCounter++;

	// get fps
	if ((GetTickCount() - 240) > fps_time)
	{
		// for averaging
		fpsBuf4 = fpsBuf3;
		fpsBuf3 = fpsBuf2;
		fpsBuf2 = fpsBuf1;

		// set new FPS
		fpsBuf1 = (fpsFrameCounter * 4);
		fpsDisplay = (fpsBuf1 + fpsBuf2 + fpsBuf3 + fpsBuf4) / 4.0;

		// reset counter
		fpsFrameCounter = 0;

		// reset tick
		fps_time = GetTickCount();
	}

	return pSampMulti->IsMenuActive() && inner_menus == MENU_INFO_GAME ? fpsDisplay / 2 : fpsDisplay;
}

void CBlackLightMenu::Initialize_ImGui_Menus(void)
{ 
    this->ImBlackLightMenu();
	this->ImBlackLightHud(BlackLightFuncs->Menu.bImGuiHudMenu);
	this->ImScoreboard(BlackLightFuncs->Menu.bImScoreboad);
	this->ImInitBlackLight_AudioStreamPlayer(BlackLightFuncs->Menu.bImSAMPAudioStream);
	this->ImMenuInit_RouteRecording(BlackLightFuncs->bVehicleRecordingEnable);

	this->ImFriendsAndAdminsMenu(BlackLightFuncs->Menu.bImFriendAndAdminsMenu);
	this->ImInitBlackLightPlayersTags(BlackLightFuncs->Menu.ESPMenus.bImBlackLightPlayersTags);
	this->ImInitBlackLightPlayersInfo(BlackLightFuncs->Menu.ESPMenus.bImBlackLightPlayersInfo);
	this->ImInitBlakcLightRadioVolume(BlackLightFuncs->Menu.bRadioVolume);
	this->ImInitBlackLight_NewSpeedometer(BlackLightFuncs->Menu.bImMenuNewSpeedometer);
	this->ImInitBlackLight_DamagerMenu();
	this->ImInitBlackLight_BotInfoIDMenu(g_BotFuncs->BotSettings.bShowBotInfoID);
}

void CBlackLightMenu::ImBlackLightMenu(void)
{
	if (BlackLightFuncs->Menu.bMain_Menu == true)
	{
		static bool bCheckInfoChildWindowFocus = false;
		static float fMainMenuW = 320.0f, fMainMenuH = 360.0f;

		ImGui::SetNextWindowSize(ImVec2(fMainMenuW, fMainMenuH));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColorTransparentBlack);
		ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.040f, 0.040f, 0.040f, 1.000f));
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.040f, 0.040f, 0.040f, 1.000f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImColorDeepOrange);
		ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
		//ImGui::SetNextWindowPos(ImVec2(((float)pPresentParam.BackBufferWidth / 2) - (fMainMenuW / 2), ((float)pPresentParam.BackBufferHeight / 2) - (fMainMenuH / 2)));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 2.0f));
		ImGui::Begin(pSampMulti->SetText("%s %s##BlackLingMainMenu", M0D_NAME, M0D_VERSION), &BlackLightFuncs->Menu.bMain_Menu,
			ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoResize);

		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(5);

		if (ImGui::BeginChild("##ChildMainMenu1", ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - 58.0f), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings))
		{
			if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
			{
				if (!BlackLightFuncs->Menu.bIsMainMenuFocused)
					BlackLightFuncs->Menu.bIsMainMenuFocused = true;
			}
			else
			{
				if (BlackLightFuncs->Menu.bIsMainMenuFocused)
					BlackLightFuncs->Menu.bIsMainMenuFocused = false;
			}

			switch (menus)
			{

			case MENU_MAIN:
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				pMenu->AddMenu("Addon ", MENU_ADDONS);
				pMenu->AddMenu("Music ", MENU_MP3);
				pMenu->AddMenu("Radio", MENU_RADIO);
				pMenu->AddMenu("ESP", MENU_ESP);
				pMenu->AddMenu("Hud", MENU_HUD);
				pMenu->AddMenu("Player Functions", MENU_PLAYER);
				pMenu->AddMenu("Vehicle Functions", MENU_VEHICLE);
				pMenu->AddMenu("Teleports", MENU_TELEPORTS);
				pMenu->AddMenu("Commands", MENU_COMMANDS);
				pMenu->AddMenu("Settings", MENU_SETTINGS);
				ImGui::PopStyleColor(3);
				break;
			}

			case MENU_ADDONS:
			{
				pMenu->AddMenuText("Additional Menus");

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				pMenu->AddMenu("Rainbow Funcs", MENU_ADDON_RAINBOW);
				pMenu->AddMenu("GTA/SA:MP Patches", MENU_ADDON_PATCHES);
				pMenu->AddMenu("NetGame", MENU_ADDON_RPCs);
				pMenu->AddMenu("Server Joiner", MENU_ADDON_SERVERS);
				pMenu->AddMenu("Spectate Players", MENU_ADDON_SPECTATE_PLAYERS);
				pMenu->AddMenu("Targer Info", MENU_ADDON_TARGET_INFO);
				pMenu->AddMenu("Spammer", MENU_ADDON_SPAMMER);
				ImGui::PopStyleColor(3);

				pMenu->AddMenuText("Other Functions");
				ImGui::PushFont(pFontArialV2);
				pMenu->AddButton(&BlackLightFuncs->Menu.bImGuiDemoWindow, "ImGui Demo##AddonFunc1");
				pMenu->AddItemDescription("ImGui window");
				pMenu->AddButton(&BlackLightFuncs->Menu.bImSAMPAudioStream, "AudioStream Player");
				pMenu->AddItemDescription("Allows you to play radio stream links in game using\nsamp network system.", ImColorGreen);
				pMenu->AddButton(&BlackLightFuncs->bClickWarp, "Click Warp##AddonFunc1a");
				pMenu->AddItemDescription("press Middle Mouse Button to warp");
				pMenu->AddButton(&BlackLightFuncs->bQuickWarp, "Quick Warp##AddonFunc1b");
				pMenu->AddItemDescription("press and hold [RMB] then press [R] to warp");
				pMenu->AddButton(&BlackLightFuncs->bSpeedometer, "Speedometer##AddonFunc2");
				pMenu->AddButton(&BlackLightFuncs->Menu.bImMenuNewSpeedometer, "New Speedometer##AddonFunc2a");
				pMenu->AddButton(&BlackLightFuncs->bDisableWaterWaves, "Disable Water Waves##AddonFunc3");
				pMenu->AddButton(&BlackLightFuncs->bSampFastConnect, "Samp Fast Join##AddonFunc4");
				pMenu->AddItemDescription("Speeds up join time while you're\non a connecting to server screen");
				pMenu->AddButton(&BlackLightFuncs->UIF.Islands.bCustomObjectKeyControls, "UIF Islands##AddonFunc5");
				pMenu->AddItemDescription("Q - copy object\nO - open objects menu\nR - remove object\nE - move object");
				pMenu->AddButton(&BlackLightFuncs->bWorldCollision, "World Collision##AddonFunc5a");

				pMenu->AddMenuText("Custom Game Time");
				pMenu->AddButton(&BlackLightFuncs->bCustomTime, BlackLightFuncs->bCustomTime ? pSampMulti->SetText("Custom Time %d##AddonFunc6", set.BlackLight.custom_time) : "Custom Time##AddonFunc6", BlackLightFuncs->bCustomTime ? 70.0f : 0.0f);
				if (BlackLightFuncs->bCustomTime == true)
					pMenu->AddIncDecButtons(set.BlackLight.custom_time, 1, 23, 0, true, "inctime1", "dectime1");

				pMenu->AddMenuText("Custom Game Weather");
				pMenu->AddButton(&BlackLightFuncs->bCustomWeather, BlackLightFuncs->bCustomWeather ? pSampMulti->SetText("Weather: %s##AddonFunc7", pSampMulti->getWeatherName(set.BlackLight.custom_weather)) : "Custom Weather##AddonFunc7", BlackLightFuncs->bCustomWeather ? 70.0f : 0.0f);
				if (BlackLightFuncs->bCustomWeather == true)
					pMenu->AddIncDecButtons(set.BlackLight.custom_weather, 1, 45, 0, true, "incweather1", "decweather1");

				pMenu->AddMenuText("Custom Game Speed");
				pMenu->AddButton(&BlackLightFuncs->bCustomGameSpeed, "Game Speed##AddonFunc7a", BlackLightFuncs->bCustomGameSpeed ? 70.0f : 0.0f);
				if (BlackLightFuncs->bCustomGameSpeed == true)
					pMenu->AddIncDecButtons(set.game_speedc, 0.05f, 200.00f, 0.00f, true, "incgamespeed1", "decgamespeed1");

				pMenu->AddMenuText("Friends & Admins");
				pMenu->AddButton(&BlackLightFuncs->Menu.bImFriendAndAdminsMenu, "Friends&Admins Menu##AddonFunc8");
				pMenu->AddMenuText("Server");
				if (pMenu->AddStaticButton("Player Joiner##AddonFunc9"))
				{
					if (!BlackLightFuncs->bPlayerJoiner)
					{
						BlackLightFuncs->bPlayerJoiner = true;

						if (!BlackLightFuncs->Menu.bImFriendAndAdminsMenu)
							BlackLightFuncs->Menu.bImFriendAndAdminsMenu = true;
						set.BlackLight.menu_admins_and_friends = MOD_JOINER_LOGGER;
						cheat_state_text("Player Joiner: {00FF00}ON");
					}
					else
					{
						BlackLightFuncs->bPlayerJoiner = false;
						cheat_state_text("Player Joiner: {FF0000}OFF");
					}
				}
				pMenu->AddItemDescription(BlackLightFuncs->bPlayerJoiner ? "Now click on 'Logger' button\nin the hud" : "a.k.a Raknet Logger");
				pMenu->AddButton(&BlackLightFuncs->bAutoReconnect, "Auto Reconnect##AddonFunc10");
				pMenu->AddButton(&BlackLightFuncs->bDeathLogChat, "Kill List Chat##AddonFunc11");
				pMenu->AddItemDescription("Show kill list inside chat");
				//pMenu->AddButton(&BlackLightFuncs->bChangeGPCI, "GPCI Changer##AddonFunc12");

				ImGui::PopFont();

				break;
			}
			case MENU_ADDON_RAINBOW:
			{
				ImGui::PushFont(pFontArialV2);
				//rainbow vehicle
				pMenu->AddMenuText("Vehicle", true);
				pMenu->AddButton(&BlackLightFuncs->bRainbow_Vehicle, "Rainbow Vehicle##RainbowFunc1", BlackLightFuncs->bCustomRainbowColorSet ? 30.0f : 0.0f);
				if (BlackLightFuncs->bCustomRainbowColorSet)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##RainbowVehicleCustomColor1", (float*)&custom_color_car, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
				}
				pMenu->AddSlider("##SetRainbowVehicleSpeed", set.BlackLight.Rainbow.rainbow_car_speed, 0, 100);

				//rainbow sky
				pMenu->AddMenuText("Sky", true);
				pMenu->AddButton(&BlackLightFuncs->bRainbow_Sky, "Rainbow Sky##RainbowFunc2", BlackLightFuncs->bCustomRainbowColorSet ? 30.0f : 0.0f);
				if (BlackLightFuncs->bCustomRainbowColorSet)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##RainbowSkyCustomColor1", (float*)&custom_color_rb_sky, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
				}
				pMenu->AddSlider("##SetRainbowSkySpeed", set.BlackLight.Rainbow.rainbow_sky_speed, 0, 100);


				//rainbow radar
				pMenu->AddMenuText("Radar", true);
				pMenu->AddButton(&BlackLightFuncs->bRainbow_Radar, "Rainbow Radar##RainbowFunc3", BlackLightFuncs->bCustomRainbowColorSet ? 30.0f : 0.0f);
				if (BlackLightFuncs->bCustomRainbowColorSet)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##RainbowRadarCustomColor1", (float*)&custom_color_radar, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
				}
				pMenu->AddSlider("##SetRainbowRadarSpeed", set.BlackLight.Rainbow.rainbow_radar_speed, 0, 100);

				//rainbow hud
				pMenu->AddMenuText("Hud", true);
				pMenu->AddButton(&BlackLightFuncs->bRainbow_Hud, "Rainbow Hud##RainbowFunc4", BlackLightFuncs->bCustomRainbowColorSet ? 30.0f : 0.0f);
				if (BlackLightFuncs->bCustomRainbowColorSet)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##RainbowHudCustomColor1", (float*)&custom_color_hud, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
				}
				pMenu->AddSlider("##SetRainbowHudSpeed", set.BlackLight.Rainbow.rainbow_hud_speed, 0, 100);

				//rainbow chat
				pMenu->AddMenuText("Chat", true);
				pMenu->AddButton(&BlackLightFuncs->bRainbow_Chat, "Rainbow Chat##RainbowFunc5", BlackLightFuncs->bCustomRainbowColorSet ? 30.0f : 0.0f);
				if (BlackLightFuncs->bCustomRainbowColorSet)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##RainbowChatCustomColor1", (float*)&custom_color_chat, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
				}
				pMenu->AddSlider("##SetRainbowChatSpeed", set.BlackLight.Rainbow.rainbow_chat_speed, 0, 100);

				//rainbow nicknames
				pMenu->AddMenuText("Nicknames", true);
				pMenu->AddButton(&BlackLightFuncs->bRainbow_Nicknames, "Rainbow Nicknames##RainbowFunc6", BlackLightFuncs->bCustomRainbowColorSet ? 30.0f : 0.0f);
				if (BlackLightFuncs->bCustomRainbowColorSet)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##RainbowNamesCustomColor1", (float*)&custom_color_nicknames, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
				}
				pMenu->AddSlider("##SetRainbowNamesSpeed", set.BlackLight.Rainbow.rainbow_nicknames_speed, 0, 100);

				//rainbow kill_list
				pMenu->AddMenuText("Kill List", true);
				pMenu->AddButton(&BlackLightFuncs->bRainbow_KillList, "Rainbow Kill List##RainbowFunc7", BlackLightFuncs->bCustomRainbowColorSet ? 30.0f : 0.0f);
				if (BlackLightFuncs->bCustomRainbowColorSet)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##RainbowDeathListCustomColor1", (float*)&custom_color_killList, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
				}
				pMenu->AddSlider("##SetRainbowDeathlistSpeed", set.BlackLight.Rainbow.rainbow_KillList_speed, 0, 100);

				//rainbow car light
				pMenu->AddMenuText("Vehicle Lights", true);
				pMenu->AddButton(&BlackLightFuncs->bRainbow_CarLights, "Rainbow Car Lights##RainbowFunc8", BlackLightFuncs->bCustomRainbowColorSet ? 30.0f : 0.0f);
				if (BlackLightFuncs->bCustomRainbowColorSet)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##RainbowCarLightsCustomColor1", (float*)&custom_color_carLights, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
				}
				pMenu->AddSlider("##SetRainbowCarLightsSpeed", set.BlackLight.Rainbow.rainbow_carlights_speed, 0, 100);

				//crosshair
				pMenu->AddMenuText("Crosshair", true);
				pMenu->AddButton(&BlackLightFuncs->bRainbow_Crosshair, "Rainbow Crosshair##RainbowFunc9", BlackLightFuncs->bCustomRainbowColorSet ? 30.0f : 0.0f);
				if (BlackLightFuncs->bCustomRainbowColorSet)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##RainbowCrosshairCustomColor1", (float*)&custom_color_crosshair, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
				}
				pMenu->AddSlider("##SetRainbowCrosshairSpeed", set.BlackLight.Rainbow.rainbow_crosshair_speed, 0, 100);

				ImGui::PopFont();
				break;
			}

			case MENU_ADDON_PATCHES:
			{
				pMenu->AddMenuText("GTA Patches");
				ImGui::PushFont(pFontArialV2);
				for (int i = 0; i < INI_PATCHES_MAX; i++)
				{
					struct patch_set* p = &set.patch[i];

					if (p->name == NULL)
						continue;

					if (p->failed)
						continue;

					ImGui::PushStyleColor(ImGuiCol_Button, !p->installed ? ImColorRed : ImColorGreen);
					ImGui::PushID(std::to_string(i).c_str());
					if (ImGui::Button("Patch##GTAP"))
					{
						if (p->installed || p->failed)
						{
							patcher_remove(p);
							addMessageToChatWindow("Patch %s OFF", p->name);
						}
						else
						{
							patcher_install(p);
							addMessageToChatWindow("Patch %s ON", p->name);

						}
					}
					ImGui::PopID();
					ImGui::PopStyleColor(1);
					ImGui::SameLine();
					ImGui::Text(p->name);
					pMenu->AddItemDescription(p->name);
				}
				pMenu->AddMenuText("SA:MP Patches");
				for (int isamp = 0; isamp < INI_SAMPPATCHES_MAX; isamp++)
				{
					struct patch_set* patch = &set.sampPatch[isamp];

					if (patch->name == NULL)
						continue;

					ImGui::PushStyleColor(ImGuiCol_Button, !patch->installed ? ImColorRed : ImColorGreen);
					ImGui::PushID(isamp);
					if (ImGui::Button("Patch##SAMPps"))
					{
						if (patch->installed || patch->failed)
						{
							patcher_remove(patch);
							addMessageToChatWindow("Samp Patch %s OFF", patch->name);
						}
						else
						{
							if (!patch->installed)
								patch->installed = 1;
							patcher_install(patch);
							addMessageToChatWindow("Samp Patch %s ON", patch->name);
						}
					}
					ImGui::PopID();
					ImGui::PopStyleColor(1);

					ImGui::SameLine();
					ImGui::Text(patch->name);
					pMenu->AddItemDescription(patch->name);
				}
				ImGui::PopFont();
				break;
			}

			case MENU_ADDON_RPCs:
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				pMenu->AddMenu("Incoming RPC's/Packets", MENU_ADDON_RPC_INCOMING);
				pMenu->AddMenu("Outcoming RPC's/Packets", MENU_ADDON_RPC_OUTCOMING);
				pMenu->AddMenu("Outcoming Packets", MENU_ADDON_PACKET_OUTCOMING);
				pMenu->AddMenu("Incoming Packets", MENU_ADDON_PACKET_INCOMING);
				pMenu->AddMenuText("Other RPC Functions");
				pMenu->AddButton(&BlackLightFuncs->bPTPAntiEndRoundFreeze, "PTP No Freeze");
				pMenu->AddItemDescription("For UIF Server\nno freeze after ptp round ends");
				pMenu->AddButton(&BlackLightFuncs->bRPCInterior, "RPC Disable Interior");

				ImGui::PopStyleColor(3);
				break;
			}

			case MENU_ADDON_RPC_INCOMING:
			{
				ImGui::PushFont(pFontArialV2);
				for (int i = 0; i < iNetPatchesCount; i++)
				{
					stNetPatch& patch_in = set.netPatch[i];

					if (patch_in.type == INCOMING_RPC)
					{
						ImGui::PushID(i);
						pMenu->AddButton(&patch_in.enabled, patch_in.name);
						ImGui::PopID();
					}
				}
				ImGui::PopFont();
				break;
			}

			case MENU_ADDON_RPC_OUTCOMING:
			{
				ImGui::PushFont(pFontArialV2);
				for (int i = 0; i < iNetPatchesCount; i++)
				{
					stNetPatch& patch_out = set.netPatch[i];

					if (patch_out.type == OUTCOMING_RPC)
					{
						ImGui::PushID(i);
						pMenu->AddButton(&patch_out.enabled, patch_out.name);
						ImGui::PopID();
					}
				}
				ImGui::PopFont();
				break;
			}

			case MENU_ADDON_PACKET_INCOMING:
			{
				ImGui::PushFont(pFontArialV2);
				for (int i = 0; i < iNetPatchesCount; i++)
				{
					stNetPatch& patch_pin = set.netPatch[i];

					if (patch_pin.type == INCOMING_PACKET)
					{
						ImGui::PushID(i);
						pMenu->AddButton(&patch_pin.enabled, patch_pin.name);
						ImGui::PopID();
					}
				}
				ImGui::PopFont();
				break;
			}

			case MENU_ADDON_PACKET_OUTCOMING:
			{
				ImGui::PushFont(pFontArialV2);
				for (int i = 0; i < iNetPatchesCount; i++)
				{
					stNetPatch& patch_pout = set.netPatch[i];

					if (patch_pout.type == OUTCOMING_PACKET)
					{
						ImGui::PushID(i);
						pMenu->AddButton(&patch_pout.enabled, patch_pout.name);
						ImGui::PopID();
					}
				}
				ImGui::PopFont();
				break;
			}


			case MENU_ADDON_SERVERS:
			{
				ImGui::PushFont(pFontArialV2);
				pMenu->AddMenuText("Servers List (?)");
				pMenu->AddItemDescription("Add any server into\nBlackLight.ini config file");
				for (int i = 0; i < INI_SERVERS_MAX; i++)
				{
					if (set.server[i].server_name == NULL || set.server[i].ip == NULL
						|| strlen(set.server[i].ip) < 7 || set.server[i].port == 0)
						continue;

					ImGui::Text("(%d). %s", i, set.server[i].server_name);
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("Click to Connect to: %s\nAddress: %s:%d", set.server[i].server_name, set.server[i].ip, set.server[i].port);
						ImGui::Text(!strlen(set.server[i].password) ? "Password: None" : "Password: %s", set.server[i].password);
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) //connect
					{
						if (strcmp(pSampMulti->SetText("%s:%d", set.server[i].ip, set.server[i].port), pSampMulti->getServerIp()) == 0)
							addMessageToChatWindow("You are already on this server.");
						else
						{
							if (set.use_current_name == 0) setLocalPlayerName(set.server[i].nickname);
							changeServer(set.server[i].ip, set.server[i].port, set.server[i].password);
							addMessageToChatWindow("Connecting to: %s:%d", set.server[i].ip, set.server[i].port);
						}
					}
					ImGui::Separator();
				}
				ImGui::PopFont();
				break;
			}

			case MENU_ADDON_SPECTATE_PLAYERS:
			{
				ImGui::PushFont(pFontArialV2);
				static bool bSpec;
				if (pSampMulti->GetPlayersInStream() == 0) ImGui::TextColored(ImColorRed, "No Players In Stream.");
				else 	if (pSampMulti->GetPlayersInStream() > 0)
				{
					for (int i = 0; i < SAMP_MAX_PLAYERS; i++)
					{
						if (g_Players->iIsListed[i] != 1) continue;

						struct actor_info* target = getGTAPedFromSAMPPlayerID(i); if (!target) continue;
						struct vehicle_info* vinfo = getGTAVehicleFromSAMPVehicleID(g_Players->pRemotePlayer[i]->pPlayerData->sVehicleID);

						ImGui::PushID(i);
						if (ImGui::Button("Spec##SpecPlayerI"))
						{
							pGameInterface->GetCamera()->RestoreWithJumpCut(); //fix bad cam pos
							if (!pSampMulti->IsPlayerInCar(i))
								GTAfunc_CameraOnActor(target);
							else
							{
								if (vinfo)
									GTAfunc_CameraOnVehicle(vinfo);
							}
							set.BlackLight.Target.TargetPlayer_Spec = i;
							if (!bSpec) bSpec = true;
						}
						if (bSpec && set.BlackLight.Target.TargetPlayer_Spec == i)
						{
							ImGui::SameLine();
							if (ImGui::Button("OFF"))
							{
								bSpec = false;
								set.BlackLight.Target.TargetPlayer_Spec = -1;
								pGameInterface->GetCamera()->RestoreWithJumpCut();
							}
						}
						ImGui::PopID();
						ImGui::SameLine();
						ImGui::Text("%s(%d)", getPlayerName(i), i);
					}
				}
				ImGui::PopFont();
				break;
			}

			case MENU_ADDON_TARGET_INFO:
			{
				pMenu->AddMenuText("Choose Target");
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				pMenu->AddMenu(pSampMulti->SetText("Targets Available: %d", pSampMulti->GetPlayersInStream()), MENU_ADDON_TARGET_INFO_LIST_OFF_PLAYERS);
				ImGui::PopStyleColor(3);

				pMenu->AddButton(&BlackLightFuncs->bTargetInfo[set.BlackLight.Target.TargetPlayer_MenuInfo], "View Target Info");
				if (BlackLightFuncs->bTargetInfo[set.BlackLight.Target.TargetPlayer_MenuInfo])
				{
					if (pSampMulti->IsPlayerStreamed(set.BlackLight.Target.TargetPlayer_MenuInfo))
					{
						pMenu->AddMenuText("Target Info:");
						pMenu->AddMenuText(pSampMulti->SetText("%s(%d)", getPlayerName(set.BlackLight.Target.TargetPlayer_MenuInfo), set.BlackLight.Target.TargetPlayer_MenuInfo), true, ImColorBanana);
						pMenu->AddMenuText(pSampMulti->SetText("Weapon: %s(%d)", pSampMulti->getPlayerWeapon(set.BlackLight.Target.TargetPlayer_MenuInfo), pSampMulti->getPlayerWeaponModelID(set.BlackLight.Target.TargetPlayer_MenuInfo)), true, ImColorBanana);
						pMenu->AddMenuText(pSampMulti->SetText("Sprint Energy: %.1f", pSampMulti->getPlayerSprintEnergy(set.BlackLight.Target.TargetPlayer_MenuInfo)), true, ImColorBanana);
						pMenu->AddMenuText(pSampMulti->SetText("Armor: %u", pSampMulti->getPlayerArmor(set.BlackLight.Target.TargetPlayer_MenuInfo)), true, ImColorBanana);
						pMenu->AddMenuText(pSampMulti->SetText("Health: %u", pSampMulti->getPlayerHealth(set.BlackLight.Target.TargetPlayer_MenuInfo)), true, ImColorBanana);
						pMenu->AddMenuText(pSampMulti->SetText("Ping: %d | Score: %d", pSampMulti->getPlayerPing(set.BlackLight.Target.TargetPlayer_MenuInfo), pSampMulti->getPlayerScore(set.BlackLight.Target.TargetPlayer_MenuInfo)), true, ImColorBanana);
						pMenu->AddMenuText(pSampMulti->SetText("Anim Index: %d", g_Players->pRemotePlayer[set.BlackLight.Target.TargetPlayer_MenuInfo]->pPlayerData->onFootData.sCurrentAnimationID), true, ImColorBanana);
						pMenu->AddMenuText("Target Vehicle:");
						if (pSampMulti->IsPlayerInCar(set.BlackLight.Target.TargetPlayer_MenuInfo))
						{
							pMenu->AddMenuText(pSampMulti->SetText("Vehicle: %s[%d]", pSampMulti->getPlayerVehicleName(set.BlackLight.Target.TargetPlayer_MenuInfo), pSampMulti->getPlayerVehicleModelID_V2(set.BlackLight.Target.TargetPlayer_MenuInfo), true, ImColorBanana));
							pMenu->AddMenuText(pSampMulti->SetText("Vehicle Health: %.0f", pSampMulti->getPlayerVehicleHealth(set.BlackLight.Target.TargetPlayer_MenuInfo)), true, ImColorBanana);
						}
						else pMenu->AddMenuText("Not in vehicle", true);
					}
					else pMenu->AddMenuText("Not streamed", true);
				}
				else
				{
					pMenu->AddMenuText("Your Info:");
					pMenu->AddMenuText(pSampMulti->SetText( "%s(%d)", getPlayerName(g_Players->sLocalPlayerID), g_Players->sLocalPlayerID), true, ImColorBanana);
					pMenu->AddItemDescription(pSampMulti->SetText("Your Samp Name&ID\nSize: (%d/%d)", strlen(getPlayerName(g_Players->sLocalPlayerID)), SAMP_ALLOWED_PLAYER_NAME_LENGTH));
					pMenu->AddMenuText(pSampMulti->SetText("Weapon: %s(%d)", pSampMulti->getPlayerWeapon(g_Players->sLocalPlayerID), pSampMulti->getPlayerWeaponModelID(g_Players->sLocalPlayerID)), true, ImColorBanana);
					pMenu->AddMenuText(pSampMulti->SetText("Sprint Energy: %.1f", pSampMulti->getPlayerSprintEnergy(g_Players->sLocalPlayerID)), true, ImColorBanana);
					pMenu->AddMenuText(pSampMulti->SetText("Armor: %hhu", pSampMulti->getPlayerArmor(g_Players->sLocalPlayerID)), true, ImColorBanana);
					pMenu->AddMenuText(pSampMulti->SetText("Health: %hhu", pSampMulti->getPlayerHealth(g_Players->sLocalPlayerID)), true, ImColorBanana);
					pMenu->AddMenuText(pSampMulti->SetText("Ping: %d | Score: %d", pSampMulti->getPlayerPing(g_Players->sLocalPlayerID), pSampMulti->getPlayerScore(g_Players->sLocalPlayerID)), true, ImColorBanana);
					pMenu->AddMenuText(pSampMulti->SetText("Anim Index: %d | Flag: %hu", g_Players->pLocalPlayer->onFootData.sCurrentAnimationID, g_Players->pLocalPlayer->onFootData.sAnimFlags), true, ImColorBanana);
					if (pSampMulti->IsOurPlayerInCar())
					{
						pMenu->AddMenuText(pSampMulti->SetText("Vehicle: %s[%d]", pSampMulti->getPlayerVehicleName(g_Players->sLocalPlayerID), pSampMulti->getPlayerVehicleModelID_V2(g_Players->sLocalPlayerID), true, ImColorBanana));
						pMenu->AddMenuText(pSampMulti->SetText("Vehicle Health: %.0f", pSampMulti->getPlayerVehicleHealth(g_Players->sLocalPlayerID)), true, ImColorBanana);
					}
					else pMenu->AddMenuText("Not in vehicle", true);
				}
				break;
			}

			case MENU_ADDON_TARGET_INFO_LIST_OFF_PLAYERS:
			{
				for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
				{
					if (g_Players->iIsListed[players] != 1)
						continue;

					if (pSampMulti->IsModDeveloper(players) != pSampMulti->IsModDeveloper(players))
						continue;

					if (BlackLightFuncs->bIsFriend[players] != BlackLightFuncs->bIsFriend[players])
						continue;

					actor_info* info = getGTAPedFromSAMPPlayerID(players);
					if (!info) continue;

					if (set.BlackLight.Target.TargetPlayer_MenuInfo == players)
					{
						ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
						ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 1.000f));
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
					}
					else
					{
						ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
						ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 0.000f));
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
					}
					if (ImGui::Button(pSampMulti->SetText("%s(%d)", getPlayerName(players), players), pMenu->MenuItemMatchMenuWidth()))
					{
						set.BlackLight.Target.TargetPlayer_MenuInfo = players;
						if (BlackLightFuncs->bTargetInfo[players] == true)
						{
							BlackLightFuncs->bTargetInfo[players] = false;
						}
						else BlackLightFuncs->bTargetInfo[players] = true;
						menus = MENU_ADDON_TARGET_INFO;
					}
					ImGui::PopStyleColor(4);
					ImGui::PopStyleVar(1);
					pMenu->AddItemDescription("set as target\nfor info view");
				}

				break;
			}

			case MENU_ADDON_SPAMMER:
			{
				break;
			}

			case MENU_MP3:
			{
				static bool bSelectedSong = false;
				pMenu->AddMenuText(pSampMulti->SetText("Songs: %d", pSampMulti->GetSongsCount()));
				ImGui::PushFont(pFontArialV2);

				for (const auto& mp3_files : std::experimental::filesystem::directory_iterator(MOD_FOLDER_MP3))
				{
					std::string szGetSong = mp3_files.path().string().c_str();
					szGetSong.replace(szGetSong.find(MOD_FOLDER_MP3), strlen(MOD_FOLDER_MP3), ""); //remove folder location name and just keep name of the mp3 file

					if (bSelectedSong && strcmp(szGetSong.c_str(), __MP3.current_playing_song.c_str()) == 0)
					{
						ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 1.000f));
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
						ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
					}
					else
					{
						ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
						ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
					}
					if (ImGui::Button(szGetSong.c_str(), pMenu->MenuItemMatchMenuWidth()))
					{
						if (!bSelectedSong) 
							bSelectedSong = true;

						if (BASS_ChannelIsActive(radio_channel))
						{
							BASS_StreamFree(radio_channel);
							BASS_ChannelStop(radio_channel);
						}
						if (BASS_ChannelIsActive(mp3_channel))
						{
							BASS_StreamFree(sample);
							BASS_ChannelStop(mp3_channel);
						}

						__MP3.current_playing_song = szGetSong.c_str();

						BASS_Init(-1, 44100, BASS_DEVICE_DSOUND, GetActiveWindow(), NULL);
						mp3_channel = BASS_StreamCreateFile(FALSE, mp3_files.path().string().c_str(), 0,0, /*__MP3.bMP3LoopSong ? BASS_SAMPLE_LOOP | BASS_UNICODE : */ 
							BASS_STREAM_PRESCAN | BASS_STREAM_AUTOFREE);
						BASS_ChannelPlay(mp3_channel, TRUE);

						if (inner_menus != MENU_INFO_MUSIC) inner_menus = MENU_INFO_MUSIC;

						QWORD len = BASS_ChannelGetLength(mp3_channel, BASS_POS_BYTE); // the length in bytes
						DWORD time = BASS_ChannelBytes2Seconds(mp3_channel, len);// * 1000; // get time

					   snprintf(mp3_duration, sizeof(mp3_duration) -1, "%u:%02u", time / 60, time % 60);
						
						if (BlackLightFuncs->Menu.bImGuiHudMenu)
						{
							if (set.BlackLight.menu_hud_set != MOD_HUD_MENU_MUSIC)
								set.BlackLight.menu_hud_set = MOD_HUD_MENU_MUSIC;
						}
						else addMessageToChatWindow("Listening: %s", __MP3.current_playing_song.c_str());
						pSampMulti->WriteToFile("BlackLight\\MusicLogs.txt", pSampMulti->SetText("[%s]Played: %s", pSampMulti->GetCurrentTimeA().c_str(), __MP3.current_playing_song.c_str()));
						addMessageToChatWindow("Song Length: %s", mp3_duration);
					}
					ImGui::PopStyleColor(4);
					ImGui::PopStyleVar(2);
					if (bSelectedSong && strcmp(szGetSong.c_str(), __MP3.current_playing_song.c_str()) == 0)
					{
						pMenu->AddItemDescription(pSampMulti->SetText("%s\nDuration: %s", szGetSong.c_str(), mp3_duration));
					}
					else
					{
						pMenu->AddItemDescription(szGetSong.c_str());
					}
				}
				ImGui::PopFont();
				if (BASS_ChannelIsActive(mp3_channel) == BASS_ACTIVE_PLAYING) BASS_SetVolume(__MP3.mp3_volume);
				break;
			}

			case MENU_RADIO:
			{	
				pMenu->AddMenuText("Radio Stations");
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				pMenu->AddMenu("Radio Stations", MENU_RADIO_STATIONS);
				ImGui::PopStyleColor(3);
				pMenu->AddMenuText("Station Urls:");

				radio_file.open(__Radio.radio_station_path.c_str(), std::fstream::in);
				if (radio_file.is_open())
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
					for (std::string radio_urls; std::getline(radio_file, radio_urls);)
					{
						if (!radio_urls.empty())
						{
							if (ImGui::Button(radio_urls.c_str(), pMenu->MenuItemMatchMenuWidth()))
							{
								addMessageToChatWindow("Please wait, loading radio url....");

								if (BASS_ChannelIsActive(mp3_channel)) //stop mp3 channel
								{
									if (strlen(mp3_duration))
										ZeroMemory(mp3_duration, sizeof(mp3_duration));

									__MP3.current_playing_song.clear();
									BASS_ChannelStop(mp3_channel);
								}
								BASS_SampleFree(sample);

								if (BASS_ChannelIsActive(radio_channel))
									BASS_ChannelStop(radio_channel); //stop radio channel
								BASS_StreamFree(radio_channel);

								BASS_Init(-1, 44100, BASS_DEVICE_CPSPEAKERS, GetActiveWindow(), NULL);
								radio_channel = BASS_StreamCreateURL(radio_urls.c_str(), 0, BASS_STREAM_BLOCK | BASS_STREAM_STATUS | BASS_STREAM_AUTOFREE, NULL, 0); // open URL
								BASS_ChannelPlay(radio_channel, FALSE);

								if (inner_menus != MENU_INFO_RADIO) inner_menus = MENU_INFO_RADIO;

								if (BlackLightFuncs->Menu.bImGuiHudMenu)
								{
									if (set.BlackLight.menu_hud_set != MOD_HUD_MENU_RADIO)
										set.BlackLight.menu_hud_set = MOD_HUD_MENU_RADIO;
								}
								else
								{
									addMessageToChatWindow("Listening: %s", pSampMulti->RadioUpdateSongTitle(radio_channel));
								}
							}
							pMenu->AddItemDescription(radio_urls.c_str());
							if (ImGui::IsItemClicked(ImGuiMouseButton_Middle)) //mmb mouse to delete url
								pSampMulti->DeleteFileText(MOD_FOLDER_RADIO, __Radio.radio_st_name_no_path.c_str(), "", radio_urls.c_str(), true, true);
						}
					}
					ImGui::PopStyleColor(3);
					radio_file.close();
				}
				else  pMenu->AddMenuText("No Station selected", true);
				break;
			}

			case MENU_RADIO_STATIONS:
			{
				pMenu->AddMenuText(pSampMulti->SetText("Radio Stations: %d", pSampMulti->GetRadioStationsCount()));
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				for (const auto & RadioStations : std::experimental::filesystem::directory_iterator(MOD_FOLDER_RADIO))
				{
					std::string r_stations = RadioStations.path().string().c_str();
					r_stations.replace(r_stations.find(MOD_FOLDER_RADIO), strlen(MOD_FOLDER_RADIO), "");
					r_stations.replace(r_stations.find(".txt"), strlen(".txt"), "");

					if (ImGui::Button(r_stations.c_str(), pMenu->MenuItemMatchMenuWidth()))
					{
						__Radio.radio_st_name_original = r_stations.c_str();
						__Radio.radio_station_path = RadioStations.path().string().c_str();
						__Radio.radio_st_name_no_path = RadioStations.path().string().replace(RadioStations.path().string().find(MOD_FOLDER_RADIO), strlen(MOD_FOLDER_RADIO), "").c_str();

						if (menus != MENU_RADIO) menus = MENU_RADIO; // go bacc
					}
					pMenu->AddItemDescription(pSampMulti->SetText("Radio Station:\n%s\nclick to view radio addresses\nmmb click to delete", r_stations.c_str()));
					if (ImGui::IsItemClicked(ImGuiMouseButton_Middle))
					{
						if (strcmp(r_stations.c_str(), __Radio.radio_st_name_original.c_str()) == 0)
						{
							remove(RadioStations.path().string().c_str());
							__Radio.radio_st_name_original.clear();
							__Radio.radio_station_path.clear();
							__Radio.radio_st_name_no_path.clear();
						}
						else addMessageToChatWindow("Select station %s to remove it", r_stations.c_str());
					}
				}
				ImGui::PopStyleColor(3);

				break;
			}

			case MENU_ESP:
			{
				ImGui::PushFont(pFontArialV2);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				pMenu->AddMenu("Teleport To Objects", MENU_ESP_TP_TO_OBJECTS);
				pMenu->AddMenu("Teleport To Pickups", MENU_ESP_TP_TO_PICKUPS);
				ImGui::PopStyleColor(3);
				pMenu->AddMenuText("Nametags");
				pMenu->AddButton(&BlackLightFuncs->bDefaultModNametags, "Players Nametags##ESPFunc0");
				pMenu->AddItemDescription("lshift + g to activate via keys");
				pMenu->AddSlider("##NametagsDrawDistance1", set.player_tags_dist, 0.0f, MAX_DISTANCE_ESP_DRAW, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bDefaultModVehicleTags, "Vehicle Nametags##ESPFunc0a");
				pMenu->AddItemDescription("lshift + h to activate via keys");
				pMenu->AddSlider("##VehsNametagsDrawDistance1", set.vehicle_tags_dist, 0.0f, MAX_DISTANCE_ESP_DRAW, "%.1f");

				pMenu->AddMenuText("ESP");
				pMenu->AddButton(&BlackLightFuncs->bWheelsInfo, "Wheels Status##ESPFunc1");
				if (BlackLightFuncs->bWheelsInfo)
				{
					pMenu->AddSlider("##WheelsStatusDistShow", set.BlackLight.wheels_info_dist, 0.0f, MAX_DISTANCE_ESP_DRAW);
				}
				pMenu->AddButton(&BlackLightFuncs->bSeeDrivers, "See Drivers##ESPFunc1a");

				pMenu->AddButton(&BlackLightFuncs->bCarsDoorsStatus, "Cars Doors Status##ESPFunc2");
				pMenu->AddSlider("##CarsDoorsStatusDistance", set.BlackLight.cars_doors_status_distance_show, 0.0f, MAX_DISTANCE_ESP_DRAW, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bPlayersTracers, "Players Tracers##ESPFunc3", BlackLightFuncs->bCustomColor_ESP ? 30.0f : 0.0f);
				if (BlackLightFuncs->bCustomColor_ESP)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##PlayersTracersCustomColor1", (float*)&CC_EXTRAS_PlayersTracers, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
				}
				pMenu->AddButton(&BlackLightFuncs->bPlayersBones, "Players Bones##ESPFunc4", BlackLightFuncs->bCustomColor_ESP ? 30.0f : 0.0f);
				if (BlackLightFuncs->bCustomColor_ESP)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##PlayersBonesCustomColor1", (float*)&CC_EXTRAS_PlayersBones, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
				}
				pMenu->AddButton(&BlackLightFuncs->bPlayerBox, "Players Box##ESPFunc5", BlackLightFuncs->bCustomColor_ESP ? 30.0f : 0.0f);
				if (BlackLightFuncs->bCustomColor_ESP)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##PlayersBoxCustomColor1", (float*)&CC_EXTRAS_PlayersBox, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
				}
				pMenu->AddButton(&BlackLightFuncs->bVehicleInfo, "Vehicle Info##ESPFunc6");
				pMenu->AddButton(&BlackLightFuncs->bVehicle2DInfo_MyCarInclude, "Vehicle Info My Car##ESPFunc6a");
				pMenu->AddSlider("##VehicleInfoDrawDistance", set.BlackLight.vehicle_2d_info_distance_draw, 0.0f, MAX_DISTANCE_ESP_DRAW, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bVehicleTracers, "Vehicle Tracers##ESPFunc7", BlackLightFuncs->bCustomColor_ESP ? 30.0f : 0.0f);
				if (BlackLightFuncs->bCustomColor_ESP)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##VehicleTracersCustomColor1", (float*)&CC_EXTRAS_VehicleTracers, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
				}
				pMenu->AddButton(&BlackLightFuncs->bHelicopterTracers, "Helicopter Tracers##ESPFunc8");
				pMenu->AddButton(&BlackLightFuncs->bPlayersInfo, "Players 2D Info##ESPFuncs9");
				pMenu->AddSlider("##PlayersInfoDrawDistance", set.BlackLight.players_2DInfo_distance, 0.0f, MAX_DISTANCE_ESP_DRAW, "%.1f");
				pMenu->AddButton(&BlackLightFuncs->bPlayersMap, "Players Map##ESPFuncs9a");
				pMenu->AddButton(&BlackLightFuncs->bTeleportTextSet, "Teleport Text Tag##ESPFuncs9b");
				pMenu->AddItemDescription("press * to set teleport\npress / to teleport");
				pMenu->AddButton(&BlackLightFuncs->bTrailerTracers, "Trailer Tracers##ESPFuncs9c");

				pMenu->AddMenuText("Police Detection");
				pMenu->AddButton(&BlackLightFuncs->bPoliceDetectorPeds, "Police Players Detector##ESPFuncs10");
				pMenu->AddButton(&BlackLightFuncs->bVehiclesPoliceDetector, "Police Vehicles Detector##ESPFuncs11");
				pMenu->AddMenuText("Outstream");
				pMenu->AddButton(&BlackLightFuncs->bOutStreamTags, "OutStream##ESPFunc12");

				pMenu->AddMenuText("Special ESP");
				pMenu->AddItemDescription("Improved ESP interface");
				pMenu->AddButton(&BlackLightFuncs->Menu.ESPMenus.bImBlackLightPlayersTags, "Players Tags##SpecialESPFunc1");
				pMenu->AddSlider("##SpecialESPPlayerTagsDistanceSet", set.BlackLight.SpecialESP.fDistancePlayersTags, 0.0f, MAX_DISTANCE_ESP_DRAW, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->Menu.ESPMenus.bImBlackLightPlayersInfo, "Players Info##SpecialESPFunc2");
				pMenu->AddSlider("##SpecialESPPlayersInfoDistanceSet", set.BlackLight.SpecialESP.fDistancePlayersInfo, 0.0f, MAX_DISTANCE_ESP_DRAW, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bBulletTracers, "BulletTracers##SpecialEspFunc3");

				pMenu->AddButton(&BlackLightFuncs->bPickupESP, "Pickups Finder##EspFunc12");
				pMenu->AddSlider("##ESPPickupInfoDistanceSet", set.pickup_tags_dist, 0.0f, MAX_DISTANCE_ESP_DRAW, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bObjectsESP, "Objects ESP##EspFunc13");
				pMenu->AddSlider("##ESPObjectsInfoDistanceSet", set.object_tags_dist, 0.0f, MAX_DISTANCE_ESP_DRAW, "%.1f");

				pMenu->AddMenuText("Chams");
				pMenu->AddButton(&BlackLightFuncs->bDefaultChams, "Chams##ESPFunc14");
				pMenu->AddButton(&BlackLightFuncs->bPlayersThroughWalls, "See Players Through Walls##ESPFunc15");
				pMenu->AddSlider("##ChamsStyleSetSlider1", set.BlackLight.chams_style, 1, 3);
				pMenu->AddItemDescription("Set Chams style");
				ImGui::PopStyleVar(2);
				ImGui::PopFont();

				break;
			}

			case MENU_ESP_TP_TO_OBJECTS:
			{
				static float	poso[3];
				for (int i = 0; i < SAMP_MAX_OBJECTS; i++)
				{
					if (g_SAMP->pPools->pObject->iIsListed[i] != 1)
						continue;
					if (g_SAMP->pPools->pObject->object[i] == NULL)
						continue;
					if (g_SAMP->pPools->pObject->object[i]->pGTAEntity == NULL)
						continue;

					ImGui::PushID(i);
					if (pMenu->AddStaticButton(pSampMulti->SetText("Objects Model: (ID: %d)%d", i, g_SAMP->pPools->pObject->object[i]->iModel)))
					{
						vect3_copy(&g_SAMP->pPools->pObject->object[i]->pGTAEntity->base.matrix[4 * 3], poso);
						if (vect3_near_zero(poso))
							continue;

						poso[2] += 2.0f;
						cheat_teleport(poso, 0);
					}
					ImGui::PopID();
				}
				break;
			}

			case MENU_ESP_TP_TO_PICKUPS:
			{
				static float posp[3];
				for (int i = 0; i < SAMP_MAX_PICKUPS; i++)
				{
					if (g_SAMP->pPools->pPickup->pickup[i].iModelID == 0)
						continue;
					if (g_SAMP->pPools->pPickup->pickup[i].iType == 0)
						continue;

					ImGui::PushID(i);
					if (pMenu->AddStaticButton(pSampMulti->SetText("Pickup Model: (ID: %d)%d", i,g_SAMP->pPools->pPickup->pickup[i].iModelID)))
					{
						vect3_copy(g_SAMP->pPools->pPickup->pickup[i].fPosition, posp);
						if (vect3_near_zero(posp))
							continue;

						posp[2] += 2.0f;
						cheat_teleport(posp, 0);
					}
					ImGui::PopID();
				}
				break;
			}

			case MENU_HUD:
			{
				ImGui::PushFont(pFontArialV2);
				pMenu->AddButton(&BlackLightFuncs->GtaHUD.bDisabelAll, "Disable All##HudDisableAll");
				pMenu->AddButton(&BlackLightFuncs->GtaHUD.bDisableMoney, "Money##HudMoney");
				pMenu->AddButton(&BlackLightFuncs->GtaHUD.bDisableArmorBar, "Armor##HudArmorBar");
				pMenu->AddButton(&BlackLightFuncs->GtaHUD.bDisableHealthBar, "Health##HudHPBar");
				pMenu->AddButton(&BlackLightFuncs->GtaHUD.bDisableWeaponIcon, "Wapon##HudWeapon");
     			pMenu->AddButton(&BlackLightFuncs->GtaHUD.bDisableBreathBar, "Breath##HudBreath");
				pMenu->AddButton(&BlackLightFuncs->GtaHUD.bDisableClock, "Clock##HudClock");
				pMenu->AddButton(&BlackLightFuncs->GtaHUD.bDisableRadar, "Radar##HudRadar");
				pMenu->AddButton(&BlackLightFuncs->GtaHUD.bDisableAmmo, "Ammo##HudAmmo");
				pMenu->AddButton(&BlackLightFuncs->GtaHUD.bDisableVitalStats, "Vital Stats##HudVitalStats");
				
				ImGui::PopFont();
				break;
			}
			case MENU_PLAYER:
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				pMenu->AddMenu("Money", MENU_PLAYER_MONEY);
				pMenu->AddMenu("Special Actions", MENU_PLAYER_ANIMS);
				pMenu->AddMenu("Deathmatch", MENU_PLAYER_DEATHMATCH);
				pMenu->AddMenu("Weapons", MENU_PLAYER_WEAPONS);
				pMenu->AddMenu("Fast Anims", MENU_PLAYER_FAST_ANIMS);
				pMenu->AddMenu("Target Troll", MENU_PLAYER_TARGET_FUNCS);
				pMenu->AddMenu("Muted Players", MENU_PLAYER_MUTED_PLAYERS);
				pMenu->AddMenu("Teleport To Vehicles", MENU_PLAYER_TELEPORT_TO_VEHICLE);
				pMenu->AddMenu("Warp Vehicles", MENU_PLAYER_WARP_VEHICLE_TO_ME);
				pMenu->AddItemDescription("Teleport nearby vehicles to you.");
				pMenu->AddMenu("Go To Players", MENU_PLAYER_WARP);
				pMenu->AddItemDescription("Works on outstreamed players too.\n(if same world)");
				pMenu->AddMenu("Bot Functions", MENU_PLAYER_BOTS);
				ImGui::PopStyleColor(3);
				pMenu->AddMenuText("Other Functions");

				ImGui::PushFont(pFontArialV2);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				if (ImGui::Button("Self Kill##SelfKillFunc", pMenu->MenuItemMatchMenuWidth()))
				{
					if (pPedSelf->GetHealth() == 0.0f)
						break;

					if (pSampMulti->IsOurPlayerInCar())
						addMessageToChatWindow("You must be on foot!");
					else pPedSelf->SetHealth(0.0f);
				}
				if (ImGui::Button("Restore Health##BtnResHP1", pMenu->MenuItemMatchMenuWidth()))
				{
					if (pPedSelf->GetHealth() == 0.0f)
						break;

					if (pSampMulti->IsOurPlayerInCar())
						pPedSelf->GetVehicle()->SetHealth(1000.0f);
					else pPedSelf->SetHealth(95.0f);
				}
				if (ImGui::Button("Restore Armour##BtnResArmor1", pMenu->MenuItemMatchMenuWidth()))
				{
					if (pPedSelf->GetHealth() == 0.0f)
						break;

					if (pSampMulti->IsOurPlayerInCar())
						addMessageToChatWindow("You must be on foot!");
					else pPedSelf->SetArmor(95.0f);
				}

				if (ImGui::Button("Give Jetpack##JetpackSet1", pMenu->MenuItemMatchMenuWidth()))
				{
					if (pSampMulti->IsOurPlayerInCar())
						addMessageToChatWindow("You must be on foot!");
					else if (pPedSelf->GetHealth() == 0.0f)
					{
						addMessageToChatWindow("Won't work while you are dead");
					}
					else gta_jetpack_give();
				}

				if (ImGui::Button("Extinguish Fire##FireParticleRemove", pMenu->MenuItemMatchMenuWidth()))
				{
					if (pSampMulti->IsPlayerDriver(g_Players->sLocalPlayerID))
					{
						if (pPedSelf->GetVehicle()->GetHealth() == 0.0f)
						{
							addMessageToChatWindow("Wont work on destroyed car");
							break;
						}
						pPedSelf->GetVehicle()->SetAlpha(0.5f); //,ExtinguishCarFire();
					}
					else
					{
						if (pPedSelf->GetHealth() == 0.0f)
						{
						   addMessageToChatWindow("Won't work when you are dead");
						   break;
						}

						pPedSelf->SetOnFire(false);
					}
				}
				pMenu->AddItemDescription("For player & vehicle");
				if (ImGui::Button("Warp to empty vehicle##WarpToNearestCar1", pMenu->MenuItemMatchMenuWidth()))
				{
					cheat_teleport_nearest_car();
				}
				ImGui::PopStyleColor(3);

				pMenu->AddButton(&BlackLightFuncs->bGodModePlayer, "God Mod Player##PlayerFunc0");
				pMenu->AddButton(&BlackLightFuncs->bGodModeHeliBlade, "God Mod Heli Blade##PlayerFunc0a");
				pMenu->AddButton(&BlackLightFuncs->bGodModeVehicle, "God Mod Vehicle##PlayerFunc0b");
				pMenu->AddButton(&BlackLightFuncs->bActorNoFall, "No Fall##PlayerFunc0c");
				pMenu->AddButton(&BlackLightFuncs->bPlayerGhost, "Player Ghost##PlayerFunc1");
				pMenu->AddButton(&BlackLightFuncs->bVehicleNoCollision, "Vehicle No Collision##PlayerFunc1a");
				pMenu->AddButton(&BlackLightFuncs->bObjectsNoCollision, "Objects No Collision##PlayerFunc1b");

				pMenu->AddButton(&BlackLightFuncs->bSmartInvis, "Invisible##PlayerFunc1c");

				pMenu->AddButton(&BlackLightFuncs->bCameraOverview, "Camera Overview##PlayerFunc2");
				pMenu->AddSlider("##fSetCameraOverview", set.BlackLight.fCamOverview, 1.0f, 108.7f, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bCustomSprintEnergy, "Sprint Energy##PlayerFunc3");
				if(BlackLightFuncs->bCustomSprintEnergy)
				pMenu->AddSlider("##fSetSprintEnergy", set.BlackLight.fSprintEng, 1.0f, 100.0f, "%.1f");
				pMenu->AddButton(&BlackLightFuncs->bSkinChanger, "Skin Changer##PlayerFunc23");

				pMenu->AddButton(&BlackLightFuncs->bCrazyRoter, "Crazy Roter##PlayerFunc4");
				pMenu->AddButton(&BlackLightFuncs->bNoSpread, "No Spread##PlayerFunc5");
				pMenu->AddButton(&BlackLightFuncs->bSlowmotion, "Slowmotion##PlayerFunc6");
				pMenu->AddButton(&BlackLightFuncs->bSlowmotionViaKey, "Slowmotion On Key##PlayerFunc6a");
				pMenu->AddSlider("##fSlowmotionDetection", set.BlackLight.slowmo_detection, 0.1f, 0.8f, "%.1f");
				pMenu->AddButton(&BlackLightFuncs->bFakeJetpack, "Fake Jetpack##PlayerFunc6b");
				pMenu->AddItemDescription("Others see you having jetpack, \ndoesn't work on all servers");
				pMenu->AddButton(&BlackLightFuncs->bPlayerDrillWalk, "Player Drill Walk##PlayerFunc6c");
				pMenu->AddItemDescription("Others see you spinning like drill while walking");
				pMenu->AddButton(&BlackLightFuncs->bPlayerCircleWalk, "Player Circle Walk##PlayerFunc6d");
				pMenu->AddItemDescription("Others see you rotating in circle while walking");
				pMenu->AddButton(&BlackLightFuncs->bInvertedBackLinear, "Inverted Back##PlayerFunc6e");
				pMenu->AddItemDescription("Others see you walking rotated where you\nare rotated face down to floor");
				pMenu->AddButton(&BlackLightFuncs->bInvertedFrontLinear, "Inverted Front##PlayerFunc6d");
				pMenu->AddItemDescription("Others see you walking rotated where you\nare rotated face up to sky");
				pMenu->AddButton(&BlackLightFuncs->bInvertWalk, "Invert Walk##PlayerFunc7");
				pMenu->AddItemDescription("Others see you walking on head");
				pMenu->AddButton(&BlackLightFuncs->bWheelWalk, "Wheel Walk##PlayerFunc8");
				pMenu->AddItemDescription("Others see you walking like wheel (rotating)");
				pMenu->AddButton(&BlackLightFuncs->bCrazyWalk, "Crazy Walk##PlayerFunc9");
				pMenu->AddItemDescription("Others see you rotating in all directions");
				pMenu->AddButton(&BlackLightFuncs->bAirWalk, "Air Walk##PlayerFunc10");
				pMenu->AddItemDescription("Others see you walking on air, 2m above ground");
				pMenu->AddButton(&BlackLightFuncs->bBackwardWalk, "Moon Walk##PlayerFunc10a");
				pMenu->AddButton(&BlackLightFuncs->bFakeAfk, "Fake AFK##PlayerFunc10b");
				pMenu->AddItemDescription("other players see you AFK.\nNote: some stuff won't be synced.");
				pMenu->AddButton(&BlackLightFuncs->bFreezePlayers, "Freeze Players##PlayerFunc10c");
				pMenu->AddItemDescription("a.k.a 'Moveless Players' ");
				pMenu->AddButton(&BlackLightFuncs->bFastRotation, "Fast Rotation##PlayerFunc11");
				pMenu->AddSlider("##fSetFastRotationSpeed", set.BlackLight.fFastRotation, 1.0f, 40.0f, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bAirbreakPlayer, "Airbrake##PlayerFunc12");
				pMenu->AddSlider("##fSetAirbrakePlayerSpeed", set.air_brake_speed, 1.0f, 300.0f, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bPlayerFly, "Player Fly##PlayerFunc13");
				pMenu->AddSlider("##fSetPlayerFlySpeed", set.fly_player_speed, 1.0f, 10.0f, "%.1f");
				pMenu->AddSlider("##iPlayerFlyStyle", set.BlackLight.player_fly_style, 1, 3);
				pMenu->AddItemDescription("set player fly style");

				pMenu->AddButton(&BlackLightFuncs->bPlayerSurf, "Player Surf##PlayerFunc14");
				pMenu->AddButton(&BlackLightFuncs->bDroneMode, "Drone View##PlayerFunc14a");
				pMenu->AddMenuText("Pulsators");
				pMenu->AddButton(&BlackLightFuncs->bHealthPulsator, "Health Pulsator##PlayerFunc15");
				pMenu->AddButton(&BlackLightFuncs->bArmorPulsator, "Armor Pulsator##PlayerFunc16");
				pMenu->AddButton(&BlackLightFuncs->bWeaponsPulsator, "Weapons Pulsator##PlayerFunc17");
				pMenu->AddSlider("##fSetWeaponsPulseSpeed", set.BlackLight.weapons_pulse_speed, 1, 100);

				pMenu->AddButton(&BlackLightFuncs->bPizdarvankaPlayer, "Player Fugga##PlayerFunc18");
				pMenu->AddSlider("##PlayerFuggaSpeed1", set.BlackLight.pizdarvanka_player_speed, 1, 10);
				pMenu->AddButton(&BlackLightFuncs->bFakeBulletsFlood, "Fake Bullets Flood##PlayerFunc19");
				pMenu->AddButton(&BlackLightFuncs->bFakeKillFlood, "Fake Kill Flood##PlayerFunc20");

				ImGui::PopStyleVar(2);
				ImGui::PopFont();
				break;
			}

			case MENU_PLAYER_DEATHMATCH:
			{
				pMenu->AddButton(&BlackLightFuncs->bRapidFire, "Rapid Fire##DMFunc1", BlackLightFuncs->bRapidFire ? 70.0f : 0.0f);
				if (BlackLightFuncs->bRapidFire)
					pMenu->AddIncDecButtons(set.rapid_speed, 1.0f, 10, 0, true, "IncRapidFirePower", "DecRapidFirePower");
				pMenu->AddButton(&BlackLightFuncs->bAutoCBug, "Auto C-Bug##DMFunc2");
				pMenu->AddButton(&BlackLightFuncs->bAutoScroll, "Auto Scroll##DMFunc3");
				pMenu->AddButton(&BlackLightFuncs->bNoStun, "No Stun V1##DMFunc4");
				pMenu->AddButton(&BlackLightFuncs->bNoStunV2, "No Stun V2##DMFunc4a");
				pMenu->AddButton(&BlackLightFuncs->bNoSpread, "No Spread##DMFunc4");
				pMenu->AddButton(&BlackLightFuncs->bNoReload, "No Reload##DMFunc5");
				pMenu->AddButton(&BlackLightFuncs->bFastCrosshair, "Fast Crosshair##DMFunc6");
				pMenu->AddButton(&BlackLightFuncs->bAutoShoot, "Auto Shoot##DMFunc7");

				break;
			}

			case MENU_PLAYER_ANIMS:
			{
				//pMenu->AddButton(&BlackLightFuncs->bSpecialActionAnims, "Enable Special Actions");
				//pMenu->AddItemDescription("Enable this, then select some anims from below.");
				pMenu->AddMenuText("Anims");

				if (pMenu->AddStaticButton(ICON_FA_STOP " STOP ANY ANIM " ICON_FA_STOP))
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_NONE;
				if (pMenu->AddStaticButton(ICON_FA_CAT " CARRY " ICON_FA_CAT))
				{
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_NONE;
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_CARRY;
				}
				if (pMenu->AddStaticButton(ICON_FA_LOCK " CUFFED " ICON_FA_LOCK))
				{
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_NONE;
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_CUFFED;
				}
				if (pMenu->AddStaticButton(ICON_FA_FROG " URINATE " ICON_FA_FROG))
				{
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_NONE;
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_DRINK_SPRUNK;
				}
				if (pMenu->AddStaticButton(ICON_FA_MUSIC " DANCE 1 " ICON_FA_MUSIC))
				{
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_NONE;
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_DANCE1;
				}
				if (pMenu->AddStaticButton(ICON_FA_MUSIC " DANCE 2 " ICON_FA_MUSIC))
				{
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_NONE;
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_DANCE2;
				}
					if (pMenu->AddStaticButton(ICON_FA_MUSIC " DANCE 3 " ICON_FA_MUSIC))
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_DANCE3;
				if (pMenu->AddStaticButton(ICON_FA_MUSIC " DANCE 4 " ICON_FA_MUSIC))
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_DANCE4;
				if (pMenu->AddStaticButton(ICON_FA_HANDS " HANDSUP " ICON_FA_HANDS))
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_HANDSUP;
				if (pMenu->AddStaticButton(ICON_FA_PHONE " PHONE CALL " ICON_FA_PHONE))
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_USECELLPHONE;
				if (pMenu->AddStaticButton(ICON_FA_PHONE " STOP PHONE CALL " ICON_FA_PHONE))
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_STOPUSECELLPHONE;
				if (pMenu->AddStaticButton(ICON_FA_BEER " DRINK BEER " ICON_FA_BEER))
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_DRINK_BEER;
				if (pMenu->AddStaticButton(ICON_FA_SKULL_CROSSBONES " SMOKE CIGGY " ICON_FA_SKULL_CROSSBONES))
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_SMOKE_CIGGY;
				if (pMenu->AddStaticButton(ICON_FA_HANDS " DRINK WINE " ICON_FA_HANDS))
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_DRINK_WINE;
				if (pMenu->AddStaticButton(ICON_FA_HANDS " DRINK SPRUNK " ICON_FA_HANDS))
					set.special_action_anim = ID_MENU_SPECIAL_ACTION_DRINK_SPRUNK;

				break;
			}

			case MENU_PLAYER_MONEY:
			{
				static uint32_t	money;

				pMenu->AddButton(&BlackLightFuncs->bEnableMoneyCheat, "Enable Money Cheat");

				if (BlackLightFuncs->bEnableMoneyCheat)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
					if (ImGui::Button("$10,000##MoneyFunc1", pMenu->MenuItemMatchMenuWidth()))
					{
						money += 10000;
						gta_money_set(money);
					}
					if (ImGui::Button("$50,000##MoneyFunc2", pMenu->MenuItemMatchMenuWidth()))
					{
						money += 50000;
						gta_money_set(money);
					}
					if (ImGui::Button("$150,000##MoneyFunc3", pMenu->MenuItemMatchMenuWidth()))
					{
						money += 150000;
						gta_money_set(money);
					}
					if (ImGui::Button("$300,000##MoneyFunc4", pMenu->MenuItemMatchMenuWidth()))
					{
						money += 150000*2;
						gta_money_set(money);
					}
					if (ImGui::Button("$1000,000##MoneyFunc5", pMenu->MenuItemMatchMenuWidth()))
					{
						money += 1000000;
						gta_money_set(money);
					}
					if (ImGui::Button("Money Max##MoneyFuncFinal", pMenu->MenuItemMatchMenuWidth()))
					{
						money += 999999999;
						gta_money_set(money);
					}

					ImGui::PopStyleColor(3);
				}
				break;
			}

			case MENU_PLAYER_TELEPORT_TO_VEHICLE:
			{
				for (int cars = 0; cars < SAMP_MAX_VEHICLES; cars++)
				{
					if (g_Vehicles->iIsListed[cars] != 1)
						continue;
					if (g_Vehicles->pSAMP_Vehicle[cars] == NULL)
						continue;
					if (g_Vehicles->pSAMP_Vehicle[cars]->pGTA_Vehicle == NULL)
						continue;

					struct vehicle_info* cari = getGTAVehicleFromSAMPVehicleID(cars);

					if (!cari)
						continue;

					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
					ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
					ImGui::PushID(cars);
					if (ImGui::Button(pSampMulti->SetText("Name: %s(Model: %d)",pSampMulti->getVehicleNameBySAMPID(cars), pSampMulti->getVehicleModelBySAMPID(cars)), pMenu->MenuItemMatchMenuWidth()))
					{
						if (vehicle_contains_actor(cari, actor_info_get(ACTOR_SELF, ACTOR_ALIVE)))
						{
							addMessageToChatWindow("You are in this vehicle");
							break;
						}

						static float fpos[3];
						vect3_copy(&cari->base.matrix[12], fpos);
						fpos[2] += 5.0f;
						cheat_teleport(fpos,0);
					}
					ImGui::PopID();
					ImGui::PopStyleColor(4);
					ImGui::PopStyleVar(1);
					pMenu->AddItemDescription("Click to teleport");
				}

				break;
			}

			case MENU_PLAYER_WARP_VEHICLE_TO_ME:
			{
				for (int cars = 0; cars < SAMP_MAX_VEHICLES; cars++)
				{
					if (g_Vehicles->iIsListed[cars] != 1)
						continue;
					if (g_Vehicles->pSAMP_Vehicle[cars] == NULL)
						continue;
					if (g_Vehicles->pSAMP_Vehicle[cars]->pGTA_Vehicle == NULL)
						continue;

					struct vehicle_info* vinfo = getGTAVehicleFromSAMPVehicleID(cars);

					if (!vinfo)
						continue;

					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
					ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
					ImGui::PushID(cars);
					if (ImGui::Button(pSampMulti->SetText("Name: %s(Model: %d)", pSampMulti->getVehicleNameBySAMPID(cars), pSampMulti->getVehicleModelBySAMPID(cars)), pMenu->MenuItemMatchMenuWidth()))
					{
						if (vehicle_contains_actor(vinfo, actor_info_get(ACTOR_SELF, ACTOR_ALIVE)))
						{
							addMessageToChatWindow("You are in this vehicle");
							break;
						}

						if (!g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->pedFlags.bInVehicle)
						{
							BitStream bsData4OnFoot;
							bsData4OnFoot.Write((BYTE)ID_PLAYER_SYNC);
							bsData4OnFoot.Write((PCHAR)&g_Players->pLocalPlayer->onFootData, sizeof(stOnFootData));
							g_RakClient2->Send(&bsData4OnFoot, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
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
						UnoccupiedData.sVehicleID = cars;
						UnoccupiedData.fHealth = vinfo->hitpoints;
						UnoccupiedData.byteSeatID = 1;
						vect3_copy(fPos, UnoccupiedData.fPosition);
						vect3_copy(fSpeed, UnoccupiedData.fMoveSpeed);
						vect3_copy(vinfo->spin, UnoccupiedData.fTurnSpeed);
						vect3_copy(&vinfo->base.matrixStruct->right.X, UnoccupiedData.fRoll);
						vect3_copy(&vinfo->base.matrixStruct->up.X, UnoccupiedData.fDirection);

						BitStream bsData;
						bsData.Write((BYTE)ID_UNOCCUPIED_SYNC);
						bsData.Write((PCHAR)&UnoccupiedData, sizeof(stUnoccupiedData));
						g_RakClient2->Send(&bsData, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

						cheat_vehicle_teleport(vinfo, fPos, 0);
						vect3_copy(fSpeed, vinfo->speed);
					}
					ImGui::PopID();
					ImGui::PopStyleColor(4);
					ImGui::PopStyleVar(1);
					pMenu->AddItemDescription("Click to teleport\nthis vehicle to you");
				}
				break;
			}

			case MENU_PLAYER_WARP:
			{
				for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
				{
					if (g_Players->iIsListed[players] != 1)
						continue;

					if (BlackLightFuncs->bIsFriend[players])
						break;
					if (pSampMulti->IsModDeveloper(players))
						break;
					if (g_BotFuncs->uiIsBot[players][0] != 0)
						continue;

					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
					ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
					ImGui::PushID(players);
					if (ImGui::Button(pSampMulti->SetText("%s(%d)", getPlayerName(players), players), pMenu->MenuItemMatchMenuWidth()))
					{
						actor_info* sinfo = getGTAPedFromSAMPPlayerID(players);
						if (sinfo != NULL && ACTOR_IS_DEAD(sinfo))
						{
							addMessageToChatWindow("Warp Error: Player is dead");
							return;
						}

						pOutStreamPos[players].fpos[1] += 1.0f;
						cheat_teleport(pOutStreamPos[players].fpos, NULL);
					}
					ImGui::PopID();
					ImGui::PopStyleColor(4);
					ImGui::PopStyleVar(1);
					pMenu->AddItemDescription("Click to teleport");
				}

				break;
			}


			case MENU_PLAYER_BOTS:
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				pMenu->AddMenu("Bot Trolls", MENU_PLAYER_BOT_TROLLS);
				pMenu->AddMenuText(pSampMulti->SetText("Bot %s Functions:", M0D_VERSION));
				ImGui::PopStyleColor(3);

				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 0.000f));
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				if (ImGui::Button(pSampMulti->SetText(g_BotFuncs->N_Client_to_Add_In_Queue > 1 ? "Bots Connect: %d" : "Bot Connect: %d", g_BotFuncs->N_Client_to_Add_In_Queue), pMenu->MenuItemMatchMenuWidth(70.0f)))
				{
					g_BotFuncs->Add_Bot_Queue();
				}
				ImGui::PopStyleColor(4);
				pMenu->AddIncDecButtons(g_BotFuncs->N_Client_to_Add_In_Queue, 1, BOTS_MAX_BOTS_CONNECT, 1, true, "botIncAdd", "botDecAddLess");

				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 0.000f));
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				if (ImGui::Button("Delete Bot", pMenu->MenuItemMatchMenuWidth()))
				{
						g_BotFuncs->Delete_Bot();

				}
				ImGui::PopStyleColor(4);
				pMenu->AddItemDescription("Delete last bot");

				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 0.000f));
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				if (ImGui::Button("Delete All Bots##BotFunc3", pMenu->MenuItemMatchMenuWidth()))
				{
					for (int i = g_BotFuncs->BotSettings.ClientCount; i > 0; i--)
						g_BotFuncs->Delete_Bot();
				}
				ImGui::PopStyleColor(4);
				
				pMenu->AddMenuText("Bot Options");
				pMenu->AddButton(&g_BotFuncs->BotSettings.bFollow, "Bot Follow##BotFunc1");
				pMenu->AddButton(&g_BotFuncs->BotSettings.bBotPick, "Bot Pick##BotFunc2");
				pMenu->AddButton(&g_BotFuncs->BotSettings.bShooter, "Bot Shooter##BotFunc3");
				pMenu->AddButton(&g_BotFuncs->BotSettings.bHealthPulsator, "Bot Health Pulsator##BotFunc3a");
				pMenu->AddButton(&g_BotFuncs->BotSettings.bArmorPulsator, "Bot Armor Pulsator##BotFunc3b");
				pMenu->AddButton(&g_BotFuncs->BotSettings.bStorm, "Bot Storm##BotFunc4");
				pMenu->AddButton(&g_BotFuncs->BotSettings.bBotSilent, "Bot Silent##BotFunc5");
				pMenu->AddItemDescription("Silent aim for bots");
				pMenu->AddButton(&g_BotFuncs->BotSettings.bShowBotInfoID, "Bot Info Menu##BotFunc6");

				pMenu->AddMenuText("Bot Class:");
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 0.000f));
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				if (ImGui::Button(pSampMulti->SetText("Bot Class Set: %d", g_BotFuncs->BotSettings.ClassID), pMenu->MenuItemMatchMenuWidth(70.0f)))
				{
					for (int j = 0; j < 2; j++)
						for (int i = g_BotFuncs->BotSettings.ClientCount - 1; i >= 0; i--)
							g_BotFuncs->Spawn_Bot(i);
				}
				pMenu->AddItemDescription("click to spawn\nfrom this class");
				pMenu->AddIncDecButtons(g_BotFuncs->BotSettings.ClassID, 1, 299, 0, true, "IncClassIDBot", "DecClassIDBot");
				if (ImGui::Button("Bot Class Copy My", pMenu->MenuItemMatchMenuWidth()))
				{
					g_BotFuncs->BotSettings.ClassID = pSampMulti->getPlayerSkin(g_Players->sLocalPlayerID);
				}
				ImGui::PopStyleColor(4);
				pMenu->AddItemDescription("set bot skin id like yours");

				pMenu->AddMenuText("Bot Position");
				pMenu->AddSlider("##BotsFollowDistanceSet", g_BotFuncs->BotSettings.fDistanceFollow, -15.0f, 15.0f);
				pMenu->AddSlider("##BetweenBotsDistanceSet", g_BotFuncs->BotSettings.fDistanceFollowBetweenBots, -15.0f, 15.0f);

				pMenu->AddMenuText(g_BotFuncs->BotSettings.NicknameStyle == BOT_ID_NICK_LOCAL ? "Bot Name Type: Local" : 
					g_BotFuncs->BotSettings.NicknameStyle == BOT_ID_NICK_REALISTIC ? "Bot Name Type: Realistic" : 
				g_BotFuncs->BotSettings.NicknameStyle == BOT_ID_NICK_BINARY ? "Bot Name Type: Binary" : "Bot Name Settings");
				pMenu->AddSlider("##SetBotNameTypes", *(int*)&g_BotFuncs->BotSettings.NicknameStyle, 1, 3);

				pMenu->AddMenuText("Bot Settings");
				pMenu->AddButton(&g_BotFuncs->BotSettings.UseFakeBot, "Bot Set As Fake##BotFuncSettings0");
				pMenu->AddButton(&g_BotFuncs->BotSettings.bCopyWeapon, "Bot Copy Weapon##BotFuncSettings1");
				pMenu->AddItemDescription("Allows bots to copy your weapon.");
				pMenu->AddButton(&g_BotFuncs->BotSettings.bCopyArmor, "Bot Copy Armor##BotFuncSettings2");
				pMenu->AddItemDescription("Allows bots to copy your armor.");
				pMenu->AddButton(&g_BotFuncs->BotSettings.bCopyHealth, "Bot Copy Health##BotFuncSettings3");
				pMenu->AddItemDescription("Allows bots to copy your health.\nEnable if players are able to damage your bots");
				pMenu->AddButton(&g_BotFuncs->BotSettings.bCopyChat, "Bot Copy Chat##BotFuncSettings4");
				pMenu->AddItemDescription("Allows bots to copy your chat.");
				pMenu->AddButton(&g_BotFuncs->BotSettings.bCopyRPC, "Bot Copy RPC##BotFuncSettings5");
				pMenu->AddItemDescription("Allows bots to follow you inside interiors & different worlds.");
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 0.000f));
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				if (ImGui::Button("Bot UIF - Report spam", pMenu->MenuItemMatchMenuWidth()))
				{
					for (int i = 0; i < g_BotFuncs->BotSettings.ClientCount; i++)
					{
						for (int is = g_BotFuncs->BotSettings.ClientCount; is > 0; is--)
						      say("/.bl.botsay %d /report %d uif best, pls no ban :( me good kiddo I love my mom", i, is); //report another bot lol
					}
				}
				ImGui::PopStyleColor(4);
				break;
			}

			case MENU_PLAYER_BOT_TROLLS:
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				pMenu->AddMenu("Set Target Troll", MENU_BOT_TROLL_SET_TARGET);
				ImGui::PopStyleColor(3);

				if (g_BotFuncs->BotSettings.ClientCount >= 0)
				{
					if (pSampMulti->IsPlayerStreamed(g_BotFuncs->BotSettings.sTargetID))
					{
						pMenu->AddButton(&g_BotFuncs->BotSettings.bBotFuck, "Bot Fuck##BotTrollFunc1a");
						pMenu->AddButton(&g_BotFuncs->BotSettings.bBotFollowAnim, "Bot Follow Anim##BotTrollFunc1b");
						pMenu->AddButton(&g_BotFuncs->BotSettings.bBotAttacker, "Bot Attacker##BotTrollFunc2");
						pMenu->AddButton(&g_BotFuncs->BotSettings.bBotElevator, "Bot Elevator##BotTrollFunc3");
						pMenu->AddButton(&g_BotFuncs->BotSettings.bBotJetpackElevator, "Bot Jetpack Elevator##BotTrollFunc4");
					}
					else pMenu->AddMenuText("Target Invalid", true, ImColorWhite);
				}
				else 	if (g_BotFuncs->BotSettings.ClientCount < 0)
					pMenu->AddMenuText("No Bots Connected", true, ImColorWhite);
				break;
			}

			case MENU_BOT_TROLL_SET_TARGET:
			{
				if (pSampMulti->GetPlayersInStream() > 0)
				{
					for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
					{
						if (g_Players->iIsListed[players] != 1)
							continue;

						actor_info* sinfo = getGTAPedFromSAMPPlayerID(players);
						if (!sinfo) continue;

						if (BlackLightFuncs->bIsFriend[players])
							break;
						if (pSampMulti->IsModDeveloper(players))
							break;
						if (g_BotFuncs->uiIsBot[players][0] != 0)
							continue;

						if (g_BotFuncs->BotSettings.bBotTarget[players] == true)
						{
							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 1.000f));
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
						}
						else {
							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
							ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
						}
						ImGui::PushID(players);
						if (ImGui::Button(pSampMulti->SetText("%s(%d)", getPlayerName(players), players), pMenu->MenuItemMatchMenuWidth()))
						{
							if (g_BotFuncs->BotSettings.bBotTarget[players] == true)
							{
								g_BotFuncs->BotSettings.bBotTarget[players] = false;
							}
							else
							{
								set.BlackLight.Target.TargetPlayer_ForBots = players;
								g_BotFuncs->BotSettings.bBotTarget[players] = true;
							}
						}
						ImGui::PopID();
						ImGui::PopStyleColor(4);
						ImGui::PopStyleVar(1);
						pMenu->AddItemDescription("set as target\nto use bots on");
					}
				}
				else ImGui::Text("No players in stream.");
				break;
			}

			case MENU_PLAYER_WEAPONS:
			{
				pMenu->AddButton(&BlackLightFuncs->bWeaponsEnable, "Enable Weapon Cheat");
				pMenu->AddButton(&BlackLightFuncs->brestore_weapons_after_death, "Restore Weapons After Death");
				pMenu->AddButton(&BlackLightFuncs->bWeaponsAmmoRandom, "Randomize ammo values");

				for (int slot = 0; slot < 13; slot++)
				{
					const struct weapon_entry* weapon = weapon_list;

					pMenu->AddMenuText(pSampMulti->SetText("Slot %d", slot), false, ImColorBanana, true);

					while (weapon->name != NULL)
					{
						if (weapon->slot == slot)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
							if (ImGui::Button(pSampMulti->SetText("%s", weapon->name), pMenu->MenuItemMatchMenuWidth()))
							{
								struct actor_info* me = actor_info_get(ACTOR_SELF, ACTOR_ALIVE);

								if (me != NULL)
								{
									for (int i = 0; i < 13; i++)
									{
										if (set.weapon[i] == NULL)
											continue;

										if (BlackLightFuncs->bWeaponsAmmoRandom == true)
										{
											if (me->weapon[set.weapon[i]->slot].id != set.weapon[i]->id
												|| (
													me->weapon[set.weapon[i]->slot].ammo == 0
													&& me->weapon[set.weapon[i]->slot].ammo_clip == 0
													))
											{
												int randammo = (rand() % set.ammo) + 1;
												int randammoclip = (rand() % set.ammo_clip) + 1;
												gta_weapon_set(me, set.weapon[i]->slot, set.weapon[i]->id, randammo, randammoclip);
											}
										}
										else
										{
											gta_weapon_set(me, set.weapon[i]->slot, set.weapon[i]->id, set.ammo, set.ammo_clip);
										}
									}
								}
							}
							ImGui::PopStyleColor(3);
						}
						weapon++;
					}
				}
				break;
			}

			case MENU_PLAYER_FAST_ANIMS:
			{
				pMenu->AddMenuText("Speed up anims");
				pMenu->AddButton(&BlackLightFuncs->bFastDeagle, "Fast Deagle##FastAnimsFunc1");
				pMenu->AddSlider("##SliderFastDeagleSpeedSet", set.BlackLight.fast_deagle, 1.0f, 40.0f, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bFasterRunArmed, "Fast Run Armed##FastAnimsFunc2");
				pMenu->AddSlider("##SliderFastRunArmedSpeedSet", set.BlackLight.fast_run_armed_speed, 1.0f, 40.0f, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bFastReload, "Fast Reload##FastAnimsFunc3");
				pMenu->AddSlider("##SliderFastReloadSpeedSet", set.BlackLight.fast_reload_speed, 1.0f, 40.0f, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bFastRun, "Fast Run##FastAnimsFunc4");
				pMenu->AddSlider("##SliderFastRunSpeedSet", set.BlackLight.fast_run_speed, 1.0f, 40.0f, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bFastSprint, "Fast Sprint##FastAnimsFunc5");
				pMenu->AddSlider("##SliderFastSprintSpeedSet", set.BlackLight.fast_sprint_speed, 1.0f, 40.0f, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bFastSwim, "Fast Swim##FastAnimsFunc6");
				pMenu->AddSlider("##SliderFastSwimSpeedSet", set.BlackLight.fast_swim_speed, 1.0f, 40.0f, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bFastWalk, "Fast Walk##FastAnimsFunc7");
				pMenu->AddSlider("##SliderFastWalkSpeedSet", set.BlackLight.fast_walk_speed, 1.0f, 40.0f, "%.1f");

				break;
			}

			case MENU_PLAYER_TARGET_FUNCS:
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				pMenu->AddMenu("Fuck Troll", MENU_PLAYER_FUCKTROLL);
				pMenu->AddMenu("Copy Chat", MENU_PLAYER_COPYCHAT);
				pMenu->AddMenu("Follow Troll", MENU_PLAYER_FOLLOWTROLL);
				pMenu->AddMenu("Slap Troll", MENU_PLAYER_SLAPTROLL);
				pMenu->AddMenu("Send Bullets", MENU_PLAYER_BULLETS_SEND);
				pMenu->AddMenu("Pizdarvanka Slapper", MENU_PLAYER_PIZDARVANKA_TARGET);

				ImGui::PopStyleColor(3);

				break;
			}

			case MENU_PLAYER_PIZDARVANKA_TARGET:
			{
				if (pSampMulti->GetPlayersInStream() > 0)
				{
					for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
					{
						if (g_Players->iIsListed[players] != 1)
							continue;

						actor_info* sinfo = getGTAPedFromSAMPPlayerID(players);
						if (!sinfo) continue;

						if (pSampMulti->IsModDeveloper(players))
							break;

						if (BlackLightFuncs->bIsFriend[players])
							break;

						if (g_BotFuncs->uiIsBot[players][0] != 0)
							continue;

						if (BlackLightFuncs->bPizdarvankaTarget[players])
						{
							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 1.000f));
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
						}
						else {
							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
							ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
						}
						ImGui::PushID(players);
						if (ImGui::Button(pSampMulti->SetText("%s(%d)##pizdec", getPlayerName(players), players), pMenu->MenuItemMatchMenuWidth()))
						{
							if (BlackLightFuncs->bPizdarvankaTarget[players] == true)
							{
								BlackLightFuncs->bPizdarvankaTarget[players] = false;
							}
							else {
								set.BlackLight.Target.TargetPlayer_Pizdarvanka = players;
								BlackLightFuncs->bPizdarvankaTarget[players] = true;
							}
						}
						ImGui::PopID();
						ImGui::PopStyleColor(4);
						ImGui::PopStyleVar(1);
						pMenu->AddItemDescription("set as target\nto use pizdarvanka on");
					}
				}
				else ImGui::Text("No players in stream.");
				break;
			}

			case MENU_PLAYER_FOLLOWTROLL:
			{
				if (pSampMulti->GetPlayersInStream() > 0)
				{
					for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
					{
						if (g_Players->iIsListed[players] != 1)
							continue;

						actor_info* sinfo = getGTAPedFromSAMPPlayerID(players);
						if (!sinfo) continue;

						if (pSampMulti->IsModDeveloper(players))
							break;

						if (BlackLightFuncs->bIsFriend[players])
							break;

						if (g_BotFuncs->uiIsBot[players][0] != 0)
							continue;

						if (BlackLightFuncs->bFollowTroll[players])
						{
							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 1.000f));
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
						}
						else {
							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
							ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
						}
						ImGui::PushID(players);
						if (ImGui::Button(pSampMulti->SetText("%s(%d)##stcp", getPlayerName(players), players), pMenu->MenuItemMatchMenuWidth()))
						{
							if (BlackLightFuncs->bFollowTroll[players] == true)
							{
								BlackLightFuncs->bFollowTroll[players] = false;
							}
							else {
								set.BlackLight.Target.TargetPlayer_Follow = players;
								BlackLightFuncs->bFollowTroll[players] = true;
							}
						}
						ImGui::PopID();
						ImGui::PopStyleColor(4);
						ImGui::PopStyleVar(1);
						pMenu->AddItemDescription("set as target\nto follow");
					}
				}
				else ImGui::Text("No players in stream.");
				break;
			}

			case MENU_PLAYER_BULLETS_SEND:
			{
				if (pSampMulti->GetPlayersInStream() > 0)
				{
					for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
					{
						if (g_Players->iIsListed[players] != 1)
							continue;

						actor_info* info = getGTAPedFromSAMPPlayerID(players);
						if (!info) continue;

						if (pSampMulti->IsModDeveloper(players))
							break;

						if (BlackLightFuncs->bIsFriend[players])
							break;

						if (g_BotFuncs->uiIsBot[players][0] != 0)
							continue;

						if (BlackLightFuncs->bFakeBullets[players])
						{
							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 1.000f));
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
						}
						else {
							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
							ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
						}
						ImGui::PushID(players);
						if (ImGui::Button(pSampMulti->SetText("%s(%d)##fb", getPlayerName(players), players), pMenu->MenuItemMatchMenuWidth()))
						{
							if (BlackLightFuncs->bIsFriend[players])
							{
								addMessageToChatWindow("You can't use this on Friends");
								break;
							}
							if (pSampMulti->IsModDeveloper(players))
							{
								addMessageToChatWindow("You cant use this on Mod Developer");
								break;
							}

							if (BlackLightFuncs->bFakeBullets[players] == true)
							{
								BlackLightFuncs->bFakeBullets[players] = false;
							}
							else {
								set.BlackLight.Target.TargetPlayer_SendBullets = players;

								BlackLightFuncs->bFakeBullets[players] = true;
							}
						}
						ImGui::PopID();
						ImGui::PopStyleColor(4);
						ImGui::PopStyleVar(1);
						pMenu->AddItemDescription("set as target\nto send bullets too");
					}
				}
				else ImGui::Text("No players in stream.");
				break;
			}

			case MENU_PLAYER_COPYCHAT:
			{
				for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
				{
					if (g_Players->iIsListed[players] != 1)
						continue;

					if (pSampMulti->IsModDeveloper(players))
						break;

					if (BlackLightFuncs->bIsFriend[players])
						break;

					if (g_BotFuncs->uiIsBot[players][0] != 0)
						continue;

					if (BlackLightFuncs->_CopyChat.bCopyChat[players] == true)
					{
						ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
						ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 1.000f));
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
					}
					else {
						ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
						ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
					}
					ImGui::PushID(players);
					if (ImGui::Button(pSampMulti->SetText("%s(%d)##cpc", getPlayerName(players), players), pMenu->MenuItemMatchMenuWidth()))
					{

						if (BlackLightFuncs->_CopyChat.bCopyChat[players] == true)
						{
							BlackLightFuncs->_CopyChat.bCopyChat[players] = false;
						}
						else {
							set.BlackLight.Target.TargetPlayer_CopyChat = players;
							BlackLightFuncs->_CopyChat.bCopyChat[players] = true;
						}
					}

					ImGui::PopID();
					ImGui::PopStyleColor(4);
					ImGui::PopStyleVar(1);
					pMenu->AddItemDescription("set as target\nto copy chat from");
				}
				break;
			}

			case MENU_PLAYER_FUCKTROLL:
			{
				if (pSampMulti->GetPlayersInStream() > 0)
				{
					int shitid = 0;
					for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
					{
						if (g_Players->iIsListed[players] != 1)
							continue;

						actor_info* info = getGTAPedFromSAMPPlayerID(players);
						if (!info) continue;

						if (pSampMulti->IsModDeveloper(players))
							break;

						if (BlackLightFuncs->bIsFriend[players])
							break;

						if (g_BotFuncs->uiIsBot[players][0] != 0)
							continue;

						if (BlackLightFuncs->bFuckTroll[players] == true)
						{
							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 1.000f));
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
						}
						else
						{
							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
							ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
						}
						ImGui::PushID(players);
						if (ImGui::Button(pSampMulti->SetText("%s(%d)##fktroll", getPlayerName(players), players), pMenu->MenuItemMatchMenuWidth()))
						{

							if (BlackLightFuncs->bFuckTroll[players] == true)
							{
								BlackLightFuncs->bFuckTroll[players] = false;
							}
							else
							{
								set.BlackLight.Target.TargetPlayer_FuckTroll = players;
								BlackLightFuncs->bFuckTroll[players] = true;
							}
						}
						ImGui::PopID();
						ImGui::PopStyleColor(4);
						ImGui::PopStyleVar(1);
						pMenu->AddItemDescription("set as target\nfor fuck troll");
					}
				}
				else ImGui::Text("No players in stream.");
				break;
			}

			case MENU_PLAYER_SLAPTROLL:
			{
				if (pSampMulti->GetPlayersInStream() > 0)
				{
					for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
					{
						if (g_Players->iIsListed[players] != 1)
							continue;

						actor_info* pinfo = getGTAPedFromSAMPPlayerID(players);
						if (!pinfo) continue;

						if (pSampMulti->IsModDeveloper(players))
							break;

						if (BlackLightFuncs->bIsFriend[players])
							break;

						if (g_BotFuncs->uiIsBot[players][0] != 0)
							continue;

						if (BlackLightFuncs->bSlapTroll[players])
						{
							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 1.000f));
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
						}
						else {
							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
							ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
						}
						ImGui::PushID(players);
						if (ImGui::Button(pSampMulti->SetText("%s(%d)##pslap", getPlayerName(players), players), pMenu->MenuItemMatchMenuWidth()))
						{
							if (BlackLightFuncs->bSlapTroll[players] == true)
							{
								BlackLightFuncs->bSlapTroll[players] = false;
							}
							else {
								set.BlackLight.Target.TargetPlayer_SlapTroll = players;
								BlackLightFuncs->bSlapTroll[players] = true;
							}
						}
						ImGui::PopID();
						ImGui::PopStyleColor(4);
						ImGui::PopStyleVar(1);
						pMenu->AddItemDescription("set as target\nfor slap");
					}
				}
				else ImGui::Text("No players in stream.");
				break;
			}

			case MENU_PLAYER_MUTED_PLAYERS:
			{
				pMenu->AddMenuText(pSampMulti->SetText("Muted Players: %d", pSampMulti->GetMutedPlayersCount()));
				for (int imutedp = 0; imutedp < SAMP_MAX_PLAYERS; imutedp++)
				{
					if (g_Players->iIsListed[imutedp] != 1)
						continue;

					if (BlackLightFuncs->Mute.bMutedPlayers[imutedp] == true)
					{
						ImGui::Text("%s(%d)", getPlayerName(imutedp), imutedp);
						pMenu->AddItemDescription(pSampMulti->SetText("%s(%d)\nLeft click to unmute", getPlayerName(imutedp), imutedp));
						if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
						{
							pSampMulti->DeleteFileText(M0D_FOLDER, "MutedPlayers", ".ini", getPlayerName(imutedp), true);
						}
					}
				}

				break;
			}

			case MENU_VEHICLE:
			{
				pMenu->AddMenuText("Vehicle");

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				pMenu->AddMenu("Route Recording", MENU_VEH_ROUTE_RECORDING);
				pMenu->AddMenu("Custom Handling", MENU_VEH_HANDLING);
				pMenu->AddMenu("Teleport Vehicles", MENU_VEH_WARP_VEHICLES);
				pMenu->AddMenu("Vehicle Tuning", MENU_VEH_VEHICLE_TUNING);
				ImGui::PopStyleColor(3);

				pMenu->AddMenuText("Other functions");
				ImGui::PushFont(pFontArialV2);
				pMenu->AddButton(&BlackLightFuncs->bMouseDrive, "Mouse Drive##VehicleFunc1");
				pMenu->AddButton(&BlackLightFuncs->bVehicleQuickWarp, "Quick Warp##VehicleFunc1a");
				pMenu->AddItemDescription("press and hold [LShift] then press [R] to warp");
				pMenu->AddButton(&BlackLightFuncs->bVehicleDisableFrame, "Vehicle No Frame##VehicleFunc2");
				pMenu->AddButton(&BlackLightFuncs->bVehicleDisableWheels, "Vehicle No Wheels##VehicleFunc3");
				if (pMenu->AddStaticButton("Fix Vehicle##VehicleFixFunc"))
				{
					if (g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->state == ACTOR_STATE_DRIVING)
						pPedSelf->GetVehicle()->Fix();
					else addMessageToChatWindow("You are not driving any vehicle!");
				}
				if (pMenu->AddStaticButton("Recolor Vehicles"))
				{
					pBlackLightMods->RecolorVehicles();
				}
				pMenu->AddButton(&BlackLightFuncs->bVehicleQuickTurn, "Quick Turns##VehicleFunc3a");
				pMenu->AddButton(&BlackLightFuncs->bArabDrift, "Arab Drift##VehicleFunc4_");
				pMenu->AddItemDescription("Others see you driving on 2 wheels");
				pMenu->AddSlider("##ArabDriftWheelStyleSet1", set.BlackLight.arab_drift_on_wheels, 1, 2);
				pMenu->AddItemDescription("1 = left wheels, 2 = right wheels");
				pMenu->AddButton(&BlackLightFuncs->bTornadoDriving, "Tornado Driving##VehicleFunc4A");
				pMenu->AddItemDescription("others see your car spinning like tornado");
				pMenu->AddButton(&BlackLightFuncs->bTornadoDrill, "Drill Driving##VehicleFunc4B");
				pMenu->AddItemDescription("others see your car spinning like drill");

				/*pMenu->AddSlider("##fDevW", set.BlackLight.fdevw, 0.0f, 50.0f);
				pMenu->AddSlider("##fDevX", set.BlackLight.fdevx, 0.0f, 50.0f);
				pMenu->AddSlider("##fDevY", set.BlackLight.fdevy, 0.0f, 50.0f);
				pMenu->AddSlider("##fDevZ", set.BlackLight.fdevz, 0.0f, 50.0f);
				pMenu->AddSlider("##fDevAxis", set.BlackLight.fdev_axis, 1, 3);*/
				pMenu->AddButton(&BlackLightFuncs->bUpDownDrive, "UpDown Drive##VehicleFunc4C");
				pMenu->AddItemDescription("Other players see your car hood in ground, back lifted in sky");

				pMenu->AddButton(&BlackLightFuncs->bBikeWheelie, "Bike Wheelie##VehicleFunc4D");
				pMenu->AddItemDescription("Other players see you driving bike & motors on back wheel (front wheel is up)");

				pMenu->AddButton(&BlackLightFuncs->bInverDrive, "Invert Drive##VehicleFunc4E");
				pMenu->AddItemDescription("Other players see you driving on roof\nyou see normal.");
				pMenu->AddButton(&BlackLightFuncs->bWheelDrive, "Wheel Drive##VehicleFunc5");
				pMenu->AddItemDescription("Other players see you moving with car like wheels rotates.");
				pMenu->AddButton(&BlackLightFuncs->bCrazyDrive, "Crazy Drive##VehicleFunc6");
				pMenu->AddItemDescription("Others see your car rotating crazy in all directions.");
				pMenu->AddButton(&BlackLightFuncs->bAirDrive, "Air Drive##VehicleFunc7");
				pMenu->AddItemDescription("Others see you driving your car in air,\n2m above street");
				pMenu->AddButton(&BlackLightFuncs->bBackwardDrive, "Moon Drive##VehicleFunc10");
				pMenu->AddItemDescription("Others see you driving your car backwards");
				pMenu->AddButton(&BlackLightFuncs->bAirbreakVehicle, "Airbrake##VehicleFunc8");
				pMenu->AddItemDescription("Vehicle Airbrake\npress key 'right shift' to toggle on/off");
				pMenu->AddSlider("##fSetAirbrakeVehicleSpeed", set.air_brake_vehicle_speed, 1.0f, 300.0f, "%.1f");
				pMenu->AddButton(&BlackLightFuncs->bVehicleJump, "Vehicle Jump##VehicleFunc9");
				pMenu->AddItemDescription("If ON press B to jump with car");
				pMenu->AddSlider("##fSetHpLevelVehicle", set.vehicle_hop_speed, 0.0f, 10.0f, "%.1f");

				pMenu->AddButton(&BlackLightFuncs->bVehicleBreakDance, "Vehicle BreakDance##VehicleFunc10");
				pMenu->AddItemDescription("P - on/off\nO/U - rotate left and right\nI/K - forward,backward\nJ/L - 360* rotation");
				pMenu->AddButton(&BlackLightFuncs->bVehicleFly, "Vehicle Fly##VehicleFunc11");
				pMenu->AddButton(&BlackLightFuncs->bFlyHeliMode, "Heli Mode Vehicle Fly##VehicleFunc11a");
				pMenu->AddButton(&BlackLightFuncs->bVehicleFastExit, "Fast Exit##VehicleFunc12");
				pMenu->AddItemDescription("Press N to eject yourself from vehicle");
				pMenu->AddButton(&BlackLightFuncs->bVehicleBoost, "Vehicle Boost##VehicleFunc13");
				pMenu->AddItemDescription("Press LALT to boost your vehicle");
				pMenu->AddButton(&BlackLightFuncs->bVehicleInstantBrake, "Vehicle Instant Brake##VehicleFunc14");
				pMenu->AddButton(&BlackLightFuncs->bVehicleSpider, "Vehicle Spider##VehicleFunc15");
				pMenu->AddButton(&BlackLightFuncs->bVehicleWheelsVisibilityPulsator, "Wheels Visibility Pulse##VehicleFunc15a");
				pMenu->AddButton(&cheat_state->vehicle.freezerot, "Freeze Vehicles Rotation##VehicleFunc16");
				pMenu->AddItemDescription("when in air");
				pMenu->AddButton(&BlackLightFuncs->bVehicleFreeze, "Freeze Vehicles##VehicleFunc17");
				pMenu->AddButton(&BlackLightFuncs->bWheelsPulsator, "Wheels Pulsator##VehicleFunc17a");
				pMenu->AddButton(&BlackLightFuncs->bBlinkColors, "Blink Colors##VehicleFunc17b");
				pMenu->AddButton(&BlackLightFuncs->bVehicleFakeFire, "Fake Fire##VehicleFunc17c");
				pMenu->AddItemDescription("others see");
				pMenu->AddButton(&BlackLightFuncs->bCarHardFlip, "Car Hard Flip##VehicleFunc17d");
				pMenu->AddItemDescription("Press RMB to flip");

				pMenu->AddMenuText("God Of Cars:", false, ImColorGreen);
				pMenu->AddButton(&BlackLightFuncs->bPickVehicle, "Pick Vehicles##VehicleFunc18");
				pMenu->AddItemDescription("Aim at vehicle then press and hold right mouse button");
				pMenu->AddButton(&BlackLightFuncs->bComponentFlood, "Flood Car Parts##VehicleFunc19");
				pMenu->AddItemDescription("Won't work for boat & bike");
				pMenu->AddSlider("##CompFloodDelaySlider1", set.BlackLight.component_flood_delay, 0.0f, 100.0f);
				pMenu->AddButton(&BlackLightFuncs->bWheelsStatusPulse, "Wheels Status Pulse##VehicleFunc20");
				pMenu->AddSlider("##WheelStatusPulseDelaySlider1", set.BlackLight.wheel_status_pulse_delay, 0.0f, 100.0f);
				pMenu->AddButton(&BlackLightFuncs->bPizdarvankaVehicle, "Vehicle Fugga##VehicleFunc21");
				pMenu->AddSlider("##VehicleFuggaSpeed1", set.BlackLight.pizdarvanka_vehicle_speed, 1, 10);
				pMenu->AddButton(&BlackLightFuncs->bVehiclesFugga, "Vehicle Fugga All##VehicleFunc22");
				pMenu->AddItemDescription("Slap all nearby players");

				ImGui::PopFont();

				break;
			}

			case MENU_VEH_HANDLING:
			{
				break;
			}

			case MENU_VEH_ROUTE_RECORDING: 
			{
				pMenu->AddMenuText("Route Recording");
				pMenu->AddButton(&BlackLightFuncs->bVehicleRecordingEnable, "Enable Recording");
				pMenu->AddMenuText("Routes History");
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				pMenu->AddMenu("Saved Recordings", MENU_VEHROUTE_SAVED_RECORDINGS);
				ImGui::PopStyleColor(3);
				pMenu->AddMenuText("Recording Settings");

				break;
			}

			case MENU_VEHROUTE_SAVED_RECORDINGS:
			{

				break;
			}

			case MENU_VEH_VEHICLE_TUNING:
			{
				break;
			}

			case MENU_VEH_WARP_VEHICLES:
			{
				break;
			}

			case MENU_TELEPORTS:
			{
				pMenu->AddMenuText("Interiors " ICON_FA_HOME);
				for (int i = 0; i < 146; i++) //interiors
				{
					ImGui::PushID(i);
					if (pMenu->AddStaticButton(pSampMulti->SetText(interiors_list[i].interior_name), 40.0f))
					{
						if (g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->state != ACTOR_STATE_DEAD)
						{
							cheat_teleport(interiors_list[i].pos, interiors_list[i].interior_id);
							addMessageToChatWindow("Teleported to interior: %s(ID: %d)", interiors_list[i].interior_name, interiors_list[i].interior_id);
						}
						else addMessageToChatWindow("Can't teleport because you're dead.");
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("Click to teleport.\nRMB to copy teleport position.");
						ImGui::Text("Pos: |X %0.2f |Y %0.2f |Z %0.2f", interiors_list[i].pos[0], interiors_list[i].pos[1], interiors_list[i].pos[2]);
						ImGui::EndTooltip();
					}
					ImGui::SameLine();
					if (pMenu->AddStaticButton(ICON_FA_COPY, 10.0f))
					{
						ImGui::SetClipboardText(pSampMulti->SetText("%0.2f, %0.2f, %0.2f", interiors_list[i].pos[0], interiors_list[i].pos[1], interiors_list[i].pos[2]));
						cheat_state_text(ImGui::GetClipboardText());
					}
					pMenu->AddItemDescription("Click to copy position.");
					ImGui::PopID();
					/*ImGui::SameLine();

					ImGui::Text("%s", interiors_list[i].interior_name);
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("Pos: |X %0.2f |Y %0.2f |Z %0.2f", interiors_list[i].pos[0], interiors_list[i].pos[1], interiors_list[i].pos[2]);
						ImGui::Text("%s, ID: %d", interiors_list[i].interior_name, interiors_list[i].interior_id);
						ImGui::EndTooltip();
					}*/
				}
				ImGui::Separator();
				pMenu->AddMenuText("Static Teleports " ICON_FA_PLANE);
				ImGui::Separator();
				for (int i = 0; i < STATIC_TELEPORT_MAX; i++)
				{
					if (strlen(set.static_teleport_name[i]) == 0)
						continue;

					if (vect3_near_zero(set.static_teleport[i].pos))
						continue;

					ImGui::PushID(i);
					if (pMenu->AddStaticButton(pSampMulti->SetText("%s", set.static_teleport_name[i]), 40.0f))
					{
						if (g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->state != ACTOR_STATE_DEAD) {
							cheat_teleport(set.static_teleport[i].pos, set.static_teleport[i].interior_id);
							addMessageToChatWindow("Teleported to: %s(ID: %d)", set.static_teleport_name[i], set.static_teleport[i].interior_id);
						}
						else addMessageToChatWindow("Can't teleport because you're dead.");
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("Click to teleport.");
						ImGui::Text("Pos: |X %0.2f |Y %0.2f |Z %0.2f", set.static_teleport[i].pos[0], set.static_teleport[i].pos[1], set.static_teleport[i].pos[2]);
						ImGui::EndTooltip();
					}
					ImGui::SameLine();		
					if (pMenu->AddStaticButton(ICON_FA_COPY, 10.0f))
					{
						ImGui::SetClipboardText(pSampMulti->SetText("%0.2f, %0.2f, %0.2f", set.static_teleport[i].pos[0], set.static_teleport[i].pos[1], set.static_teleport[i].pos[2]));
						cheat_state_text(ImGui::GetClipboardText());
					}
					pMenu->AddItemDescription("Click to copy position.");
					ImGui::PopID();
					/*ImGui::SameLine();

					ImGui::Text("%s", set.static_teleport_name[i]);
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("Pos: |X %0.2f |Y %0.2f |Z %0.2f", set.static_teleport[i].pos[0], set.static_teleport[i].pos[1], set.static_teleport[i].pos[2]);
						ImGui::Text("%s, ID: %d", set.static_teleport_name[i], set.static_teleport[i].interior_id);
						ImGui::EndTooltip();
					}*/
				}
				break;
			}

			case MENU_COMMANDS:
			{
				for (int cmd_num = 0; cmd_num < g_m0dCmdNum; cmd_num++)
				{
					ImGui::Text("(%d)  /%s", cmd_num, g_m0dCmdlist[cmd_num]);
					pMenu->AddItemDescription(pSampMulti->SetText("(%d)  /%s", cmd_num, g_m0dCmdlist[cmd_num]));
				}
				break;
			}

			case MENU_SETTINGS:
			{
				ImGui::PushFont(pFontArialV2);
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				pMenu->AddMenu("Hud Settings", MENU_HUD_SETTINGS);
				pMenu->AddMenu("Game State", MENU_SETTINGS_GAMESTATE);
				ImGui::PopStyleColor(3);
				pMenu->AddButton(&BlackLightFuncs->bPulseButtonsBorders, "Buttons Borders Opacity Pulsator");

				if (BlackLightRemoteControl.bTimeEndGame)
				{
					pMenu->AddButton(&BlackLightRemoteControl.bTimeEndGame, "Game Auto Shutdown");
					pMenu->AddItemDescription(pSampMulti->SetText("Game will automatically shutdown at: %s", BlackLightRemoteControl.time_end));
				}
				if (pMenu->AddStaticButton("Copy Server Address##BtnCopyAddrSrv1"))
				{
					if (!pSampMulti->IsTextIdentical(ImGui::GetClipboardText(), pSampMulti->getServerIp()))
					{
						ImGui::SetClipboardText(pSampMulti->getServerIp());
						addMessageToChatWindow("Server address has been copied to clipboard");
					}
					else addMessageToChatWindow("Clipboard already contains this text.");
				}
				pMenu->AddMenuText("MP3 Settings");
				pMenu->AddButton(&__MP3.bMP3LoopSong, "Loop Song##SettingsFunc0");
				pMenu->AddItemDescription("This also improves sound quality.");

				pMenu->AddMenuText("Other Settings");
				pMenu->AddSlider("##SliderVSyncFPS1", set.fps_limit, 1, 60);
				pMenu->AddItemDescription("Use ONLY if you have enabled\nVSYNC mode in game settings");
				pMenu->AddButton(&BlackLightFuncs->bVehicleTrailerSupport, "Trailer Support##FuncsSettings0a");
				pMenu->AddItemDescription("If you have trailer attached to your vehicle\nsome functions will be synced with it like fly car.");
				pMenu->AddButton(&BlackLightFuncs->bCustomColor_ESP, "Custom Color ESP##FuncsSettings1");
				pMenu->AddButton(&BlackLightFuncs->bCustomRainbowColorSet, "Custom Color Rainbow##FuncsSettings2");
				pMenu->AddButton(&BlackLightFuncs->bChatboxLog, "Log Chat##FuncsSettings3");
				pMenu->AddButton(&BlackLightFuncs->bMutedPlayerChatting, "Muted Player Warning##FuncsSettings3a");
				pMenu->AddItemDescription("Shows notification message in chat\nif muted player is saying something");
				pMenu->AddMenuText("Player Tags");
				pMenu->AddButton(&BlackLightFuncs->bFriendTags, "Friend Tags##FuncsSettings4");
				pMenu->AddItemDescription("Shows friend tag on player name\nif player is in friend list");
				pMenu->AddButton(&BlackLightFuncs->bAdminsTags, "Admin Tags##FuncsSettings5");
				pMenu->AddItemDescription("Shows admin tag on player name\nif player is in admin list");
				pMenu->AddButton(&BlackLightFuncs->Menu.bRadioVolume, "Radio Volume Slider##FuncsSettings6");
				pMenu->AddMenuText("Damager Settings");
				pMenu->AddButton(&__Damager.bEnableBackground, "Damager Background##FuncsSettings7");
				pMenu->AddItemDescription("Enable background for damager info menu.");
				pMenu->AddButton(&__Damager.bShowInChat, "DamageInfo Chat##FuncsSettings8");
				pMenu->AddItemDescription("Informs your about damage you've dealt\nto other players via chat message.");
				pMenu->AddButton(&__Damager.bShowOnTarget, "DamageInfo Draw2##FuncsSettings9");
				pMenu->AddItemDescription("Draw damage info menu on player\nwhich you've damaged.");

				pMenu->AddMenuText("SA:MP Scoreboard");
				pMenu->AddButton(&BlackLightFuncs->bUseNewScoreboard, "New Scoreboard##FuncsSettings10");
				pMenu->AddMenuText("GTA Window Mode");

				if (BlackLightFuncs->bWindowedMode)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
				}
				else
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
				}
				if (BlackLightFuncs->bWindowedMode)
				{
					ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 1.000f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.400f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.600f));
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 0.000f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				}
				if (pMenu->AddStaticButton("GTA:SA Windowed##FuncsSettings8"))
				{
					toggleWindowedMode();
				}
				ImGui::PopStyleColor(4);
				ImGui::PopStyleVar(2);

				pMenu->AddButton(&BlackLightFuncs->bShowKillCounterInsideHud, "Kill Counter Hud##FuncsSettings11");
				pMenu->AddButton(&BlackLightFuncs->bMapIncludeVehicles, "Map Include Vehicles##FuncSettings12");
				//pMenu->AddButton(&BlackLightFuncs->bLoadModCommands, "Load BlackLight Commands##FuncsSettings10");
				pMenu->AddMenuText("Protection Settings");
				pMenu->AddButton(&BlackLightFuncs->bAntiWeaponCrasher, "Anti Weapon Crasher##ProtectionFunc1");
				pMenu->AddButton(&BlackLightFuncs->bAntiCarJack, "Anti Car Jacking##ProtectionFunc2");
				pMenu->AddButton(&BlackLightFuncs->bAntiLoading, "Anti Loading##ProtectionFunc3");

				ImGui::PopFont();
				break;
			}

			case MENU_HUD_SETTINGS:
			{
				pMenu->AddMenuText("Mod Hud Options");
				pMenu->AddButton(&BlackLightFuncs->Menu.bImGuiHudMenu, "Hud Enable##ShowModHud1");
				break;
			}

			case MENU_SETTINGS_GAMESTATE:
			{
				pMenu->AddMenuText("Game State Set");
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
				if (ImGui::Button("Connect##GameStateFunc1", pMenu->MenuItemMatchMenuWidth()))
					pSampMulti->SAMPSetGamestate(GAMESTATE_CONNECTED);

				if (ImGui::Button("Connecting##GameStateFunc2", pMenu->MenuItemMatchMenuWidth()))
					pSampMulti->SAMPSetGamestate(GAMESTATE_CONNECTING);

				if (ImGui::Button("Restarting##GameStateFunc3", pMenu->MenuItemMatchMenuWidth()))
					pSampMulti->SAMPSetGamestate(GAMESTATE_RESTARTING);

				if (ImGui::Button("Await Join##GameStateFunc4", pMenu->MenuItemMatchMenuWidth()))
					pSampMulti->SAMPSetGamestate(GAMESTATE_AWAIT_JOIN);

				if (ImGui::Button("Wait Connect##GameStateFunc5", pMenu->MenuItemMatchMenuWidth()))
					pSampMulti->SAMPSetGamestate(GAMESTATE_WAIT_CONNECT);
				ImGui::PopStyleColor(3);
				break;
			}

			case MENU_CREDITS:
			{
				pMenu->AddMenuText("BlackLight Owner & Coder:", false, ImColorDeepOrange);
				pMenu->AddMenuText("_=Gigant=_", false, ImColorBanana);
				pMenu->AddMenuText("Thanks to", false, ImColorDeepOrange);
				pMenu->AddMenuText("MasterZero", false, ImColorBanana);
				pMenu->AddMenuText("Rck", false, ImColorBanana);
				pMenu->AddMenuText("Krakazabra", false, ImColorBanana);
				pMenu->AddMenuText("Slonoboyko", false, ImColorBanana);
				pMenu->AddMenuText("CleanLegend", false, ImColorBanana);
				pMenu->AddMenuText("Used softwares:", false, ImColorDeepOrange);
				pMenu->AddMenuText("ImGui by Ocornut", false, ImColorBanana);
				pMenu->AddMenuText("bass.lib - UnseenDevelopments", false, ImColorBanana);
				pMenu->AddMenuText("original mod_sa source from FYP", false, ImColorBanana);

				break;
			}

			default:
				menus = MENU_MAIN;
				break;

			}
			ImGui::EndChild();
		}


		//down info menu
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.000f, 0.000f, 0.000f, 1.000f));
		if (ImGui::BeginChild("##AddonModInfo", ImVec2(ImGui::GetWindowSize().x, 31.0f), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::PopStyleColor(1);
			static float fButtonW = 0.0f;
			ImGui::PushFont(pFontArialV2);

			if (ImGui::BeginPopupContextWindow("##PopUpContextWin1", ImGuiPopupFlags_MouseButtonRight))
			{
				if (fButtonW != 100.0f) fButtonW += 5.0f;
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.095f, 0.095f, 0.095f, 0.821f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.376f, 0.841f, 1.000f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.060f, 0.530f, 0.980f, 1.000f));

				for (int rows = 0; rows < 2; rows++)
				{
					if (rows == 0)
					{
						if (ImGui::Button("Weapon Info", ImVec2(fButtonW, 25.0f)))
						{
							pMenu->SetMenu(MENU_INFO_WEAPONS, true);
						}
						ImGui::SameLine();
						if (ImGui::Button("Player Info", ImVec2(fButtonW, 25.0f)))
						{
							pMenu->SetMenu(MENU_INFO_PLAYER, true);
						}
						if (ImGui::Button("Vehicle Info", ImVec2(fButtonW, 25.0f)))
						{
							pMenu->SetMenu(MENU_INFO_VEHICLE, true);
						}
						ImGui::SameLine();
						if (ImGui::Button("Music Options", ImVec2(fButtonW, 25.0f)))
						{
							pMenu->SetMenu(MENU_INFO_MUSIC, true);
						}
					}
					if (rows == 1)
					{
						if (ImGui::Button("Radio Info", ImVec2(fButtonW, 25.0f)))
						{
							pMenu->SetMenu(MENU_INFO_RADIO, true);
						}
						ImGui::SameLine();
						if (ImGui::Button("Game Info", ImVec2(fButtonW, 25.0f)))
						{
							pMenu->SetMenu(MENU_INFO_GAME, true);
						}
						if (ImGui::Button("Time & Date", ImVec2(fButtonW, 25.0f)))
						{
							pMenu->SetMenu(MENU_INFO_SYSTEM, true);
						}
						ImGui::SameLine();
						if (ImGui::Button("Credits", ImVec2(fButtonW, 25.0f)))
						{
							pMenu->SetMenu(MENU_CREDITS, false);
						}
					}
				}
				ImGui::PopStyleColor(3);
				ImGui::EndPopup();
			}
			else
			{
				if (fButtonW != 0.0f)
					fButtonW = 0.0f;
			}

			ImGui::PushStyleColor(ImGuiCol_Text, ImColorDeepOrange);
			switch (inner_menus)
			{
			case MENU_INFO_WEAPONS:
			{
				pMenu->AddMenuText(pSampMulti->SetText("Weapon: %s[%d]", pSampMulti->getPlayerWeapon(g_Players->sLocalPlayerID), pSampMulti->getPlayerWeaponModelID(g_Players->sLocalPlayerID)), false, ImColorBanana);
				break;
			}

			case MENU_INFO_VEHICLE:
			{
				if (pSampMulti->IsOurPlayerInCar())
					pMenu->AddMenuText(pSampMulti->SetText("Vehicle/Model: %s[%d]", pSampMulti->getPlayerVehicleName(g_Players->sLocalPlayerID), pSampMulti->getPlayerVehicleModelID_V2(g_Players->sLocalPlayerID)), false, ImColorBanana);
				else 	pMenu->AddMenuText(pSampMulti->SetText("You're not in vehicle"), false, ImColorBanana);
				break;
			}

			case MENU_INFO_PLAYER: //local player
			{
				pMenu->AddMenuText(pSampMulti->SetText("Health: %d | Armor: %d", pSampMulti->getPlayerHealth(g_Players->sLocalPlayerID), pSampMulti->getPlayerArmor(g_Players->sLocalPlayerID)), false, ImColorBanana);
				break;
			}
			
			case MENU_INFO_GAME:
			{
				pMenu->AddMenuText(pSampMulti->SetText("FPS: %.0f", pGame->GetFPS()), false, ImColorBanana);
				break;
			}

			case MENU_INFO_SYSTEM:
			{
				pMenu->AddMenuText(pSampMulti->SetText("%s | %s", pSampMulti->GetCurrentTimeA().c_str(), pSampMulti->GetCurrentDateA().c_str()), false, ImColorBanana);
				break;
			}

			case MENU_INFO_RADIO:
			{
				if (BASS_ChannelIsActive(radio_channel))
				{
					ImGui::PushFont(pFontArial);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
					ImGui::Text("");
					ImGui::SameLine((ImGui::GetWindowSize().x / 2) - 50.0f);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.000f));
					if (ImGui::Button(ICON_FA_PAUSE_CIRCLE "##RadioPause1", ImVec2(25.0f, 25.0f)))
					{
						if (BASS_ChannelIsActive(radio_channel) == BASS_ACTIVE_PLAYING)
							BASS_ChannelPause(radio_channel);
					}
					ImGui::SameLine();
					if (ImGui::Button(ICON_FA_PLAY_CIRCLE "##RadioPlay/Continue1", ImVec2(25.0f, 25.0f)))
					{
						if (BASS_ChannelIsActive(radio_channel) == BASS_ACTIVE_PAUSED)
							BASS_ChannelPlay(radio_channel, 0);
					}
					ImGui::SameLine();
					if (ImGui::Button(ICON_FA_STOP_CIRCLE "##RadioStop1", ImVec2(25.0f, 25.0f)))
					{
						if (BASS_ChannelIsActive(radio_channel) == BASS_ACTIVE_PAUSED || BASS_ChannelIsActive(radio_channel) == BASS_ACTIVE_PLAYING)
							BASS_ChannelStop(radio_channel);
					}

					ImGui::PopStyleColor(3);
					ImGui::PopStyleVar(2);
					ImGui::PopFont();
				}
				else pMenu->AddMenuText("Radio isn't playing", false, ImColorBanana);
				break;
			}

			case MENU_INFO_MUSIC:
			{
				if (BASS_ChannelIsActive(mp3_channel))
				{
					pSampMulti->ImAllingItem((ImGui::GetWindowSize().x / 2) - 85.0f);
					ImGui::PushFont(pFontArial);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.000f));
					if (ImGui::Button(ICON_FA_PLUS_SQUARE "##MP3AddStartSong", ImVec2(25.0f, 25.0f)))
					{
						if (strlen(__MP3.current_playing_song.c_str()))
							pSampMulti->AddSongOnStart(__MP3.current_playing_song.c_str());
						else addMessageToChatWindow("Error: No song from BlackLight folder is playing");
					}
					pMenu->AddItemDescription("Add as starting song.");
					ImGui::SameLine();
					if (ImGui::Button(ICON_FA_PAUSE_CIRCLE "##MP3Pause1", ImVec2(25.0f, 25.0f)))
					{
						if (BASS_ChannelIsActive(mp3_channel) == BASS_ACTIVE_PLAYING)
							BASS_ChannelPause(mp3_channel);
					}
					pMenu->AddItemDescription("Pause");
					ImGui::SameLine();
					if (ImGui::Button(ICON_FA_PLAY_CIRCLE "##MP3Play/Continue1", ImVec2(25.0f, 25.0f)))
					{
						if (BASS_ChannelIsActive(mp3_channel) == BASS_ACTIVE_PAUSED)
							BASS_ChannelPlay(mp3_channel, 0);
					}
					pMenu->AddItemDescription("Play/Continue");
					ImGui::SameLine();
					if (ImGui::Button(ICON_FA_STOP_CIRCLE "##MP3Stop1", ImVec2(25.0f, 25.0f)))
					{
						if (BASS_ChannelIsActive(mp3_channel) == BASS_ACTIVE_PAUSED || BASS_ChannelIsActive(mp3_channel) == BASS_ACTIVE_PLAYING)
							BASS_ChannelStop(mp3_channel);
					}
					pMenu->AddItemDescription("Stop");
					ImGui::SameLine();
					if (ImGui::Button(ICON_FA_WINDOW_MINIMIZE "##MP3CopyName1", ImVec2(25.0f, 25.0f)))
					{
						ImGui::SetClipboardText(__MP3.current_playing_song.c_str());
						addMessageToChatWindow("Song name: %s ,has been copied.", __MP3.current_playing_song.c_str());
					}
					pMenu->AddItemDescription("Copy Name");

					ImGui::PopStyleColor(3);
					ImGui::PopStyleVar(2);
					ImGui::PopFont();
				}
				else pMenu->AddMenuText("Music isn't playing", false, ImColorBanana);

				break;
			}

			default:
				inner_menus = MENU_INFO_SYSTEM;
				break;
			}
			ImGui::PopStyleColor(1);
			ImGui::PopFont();
			ImGui::EndChild();

			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Right click\nfor more options");
				ImGui::EndTooltip();

				if (!bCheckInfoChildWindowFocus)
					bCheckInfoChildWindowFocus = true;
			}
			else
			{
				if (bCheckInfoChildWindowFocus)
					bCheckInfoChildWindowFocus = false;
			}
		} //begin child 
		ImGui::End();

		//menu control system
		if (BlackLightFuncs->Menu.bIsMainMenuFocused && !bCheckInfoChildWindowFocus)
		{
			if (pMenu->GetLastMenu() != pMenu->GetCurrentMenu())
			{
				if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Backspace)) || ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				{
					pMenu->SetMenu(pMenu->GetLastMenu(), false);
				}
			}
			else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Backspace)) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)
				&& menus == MENU_MAIN) BlackLightFuncs->Menu.bMain_Menu = false;
		}
	}
}

void CBlackLightMenu::ImBlackLightHud(bool bEnable)
{
	traceLastFunc("ImBlackLightHud()");

	if (IS_CHEAT_PANIC_ACTIVE || gta_menu_active())
		return;

	if (bEnable)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f,8.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColorNone);
		ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
		ImGui::SetNextWindowSize(ImVec2((float)pPresentParam.BackBufferWidth, 59.0f));
		ImGui::SetNextWindowPos(ImVec2(0.0f, (float)pPresentParam.BackBufferHeight - 59.0f));
		if (ImGui::Begin("##ImBlackLightHud", &bEnable, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
		{
			ImGui::PopStyleColor(2);
			ImGui::PopStyleVar(3);

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 0.0f));

			ImGui::Text("");
			ImGui::SameLine(ImGui::GetWindowSize().x - ((ImGui::GetWindowSize().x / 2) - 200.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColorNone);
			if (ImGui::BeginChild("##InnerChild0", ImVec2((ImGui::GetWindowSize().x / 2) - 200.0f, 23.0f), false, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
			{
				ImGui::PopStyleColor(1);
				ImGui::PopStyleVar(1);

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 0.0f));
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));

				if (BlackLightFuncs->bShowKillCounterInsideHud)
				{
					static bool badd = false;
					static int kill_counter_recover = 0;
					ImGui::Text("");
					ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize("Kills: 100").x + ImGui::CalcTextSize("Friends 999").x + ImGui::CalcTextSize(" Admins 999").x + ImGui::CalcTextSize(" Settings ").x + ImGui::CalcTextSize(" Logger ").x + 90.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.045f, 0.182f, 0.567f, 0.756f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.045f, 0.182f, 0.567f, 0.756f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.045f, 0.182f, 0.567f, 0.756f));
					if (ImGui::Button(pSampMulti->SetText("Kills: %d##KilLCounterButton", set.BlackLight.kill_counter)))
					{
						if (!pSampMulti->IsMenuActive())
						{
							addMessageToChatWindow("%s Menu must be active", M0D_NAME);
						}
						else
						{

							if (!badd)
							{
								if (set.BlackLight.kill_counter != 0)
								{
									kill_counter_recover = set.BlackLight.kill_counter;
									set.BlackLight.kill_counter = 0;
									addMessageToChatWindow("Kill counter reseted, click again to restore to previous value.");
									badd = true;
								}
								else addMessageToChatWindow("Kill counter cannot be reseted because it's already on 0");
							}
							else if (badd)
							{
								set.BlackLight.kill_counter = kill_counter_recover;
								addMessageToChatWindow("Kill counter value has been restored to previous value.");
								badd = false;
							}
						}
					}
					ImGui::PopStyleColor(3);
					pMenu->AddItemDescription(pSampMulti->IsMenuActive() ? "Click to reset counter.\nClick again to restore to previous value." : "Blacklight Menu must be active!");
					ImGui::SameLine();
				}
				else
				{
					ImGui::Text("");
					ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize("Friends 999").x + ImGui::CalcTextSize(" Admins 999").x + ImGui::CalcTextSize(" Settings ").x + ImGui::CalcTextSize(" Logger ").x + 90.0f));
				}
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.045f, 0.182f, 0.567f, 0.756f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.183f, 0.326f, 0.781f, 0.647f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.045f, 0.182f, 0.567f, 0.756f));
				if (ImGui::Button(pSampMulti->SetText(ICON_FA_USER_FRIENDS " Friends %d##BtnFriends1", pSampMulti->GetFriendsInStream())))
				{
					if (!BlackLightFuncs->Menu.bImFriendAndAdminsMenu)
					{
						if (set.BlackLight.menu_admins_and_friends != MOD_FRIENDS_MENU)
							set.BlackLight.menu_admins_and_friends = MOD_FRIENDS_MENU;
						BlackLightFuncs->Menu.bImFriendAndAdminsMenu = true;
					}
					else
					{
						if (set.BlackLight.menu_admins_and_friends != MOD_FRIENDS_MENU)
							set.BlackLight.menu_admins_and_friends = MOD_FRIENDS_MENU;
					}
				}
				pMenu->AddItemDescription(pSampMulti->SetText(BlackLightFuncs->bViewAllFriends ? "Streamed: %d\nAll Friends: %d" : "Streamed: %d\nOnline Friends: %d", pSampMulti->GetFriendsInStream(), pSampMulti->GetFriendsCount()));

				ImGui::SameLine();
				if (ImGui::Button(pSampMulti->SetText(ICON_FA_USER_SHIELD " Admins %d##BtnAdmins1", pSampMulti->GetAdminsInStream())))
				{
					if (!BlackLightFuncs->Menu.bImFriendAndAdminsMenu)
					{
						if (set.BlackLight.menu_admins_and_friends != MOD_ADMINS_MENU)
							set.BlackLight.menu_admins_and_friends = MOD_ADMINS_MENU;
						BlackLightFuncs->Menu.bImFriendAndAdminsMenu = true;
					}
					else
					{
						if (set.BlackLight.menu_admins_and_friends != MOD_ADMINS_MENU)
							set.BlackLight.menu_admins_and_friends = MOD_ADMINS_MENU;
					}
				}
				pMenu->AddItemDescription(pSampMulti->SetText(BlackLightFuncs->bViewAllAdmins ? "Streamed: %d\nAll Admins: %d" : "Streamed: %d\nOnline: %d", pSampMulti->GetAdminsInStream(), pSampMulti->GetAdminsCount()));
				ImGui::SameLine();

				if (ImGui::Button(ICON_FA_INFO " Logger##BtnPlayerJoiner1"))
				{
					if (!BlackLightFuncs->bPlayerJoiner)
					{
						if (set.BlackLight.menu_admins_and_friends != MOD_JOINER_LOGGER)
							set.BlackLight.menu_admins_and_friends = MOD_JOINER_LOGGER;
						BlackLightFuncs->bPlayerJoiner = true;
					}
					else
					{
						if (set.BlackLight.menu_admins_and_friends != MOD_JOINER_LOGGER)
							set.BlackLight.menu_admins_and_friends = MOD_JOINER_LOGGER;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button(ICON_FA_WRENCH " Settings##BtnSettings1"))
				{
					if (!BlackLightFuncs->Menu.bImFriendAndAdminsMenu)
					{
						if (set.BlackLight.menu_admins_and_friends != FRIENDS_ADMINS_SETTINGS_MENU)
							set.BlackLight.menu_admins_and_friends = FRIENDS_ADMINS_SETTINGS_MENU;
						BlackLightFuncs->Menu.bImFriendAndAdminsMenu = true;
					}
					else
					{
						if (set.BlackLight.menu_admins_and_friends != FRIENDS_ADMINS_SETTINGS_MENU)
							set.BlackLight.menu_admins_and_friends = FRIENDS_ADMINS_SETTINGS_MENU;
					}
				}
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.000f, 0.000f, 0.000f, 0.697f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.677f, 0.000f, 0.000f, 0.831f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.000f, 0.000f, 0.000f, 0.697f));
				if (ImGui::Button("X##CloseHud1"))
				{
					if (pSampMulti->IsMenuActive())
					{
						BlackLightFuncs->Menu.bImGuiHudMenu = false;
						if (BlackLightFuncs->Menu.bImFriendAndAdminsMenu == true)
							BlackLightFuncs->Menu.bImFriendAndAdminsMenu = false;
					}
				}
				ImGui::PopStyleColor(3);

				ImGui::PopStyleVar(2);
				ImGui::EndChild();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 6.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColorTransparentBlack);
			ImGui::PushStyleColor(ImGuiCol_Border, ImColorBanana);
			if (ImGui::BeginChild("##innerChild1", ImVec2(ImGui::GetWindowSize().x - 1.0f, 30.0f), true, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
			{
				ImGui::PopStyleColor(2);
				ImGui::PopStyleVar(3);

				ImGui::PushFont(pFontArial);
				ImGui::TextColored(ImColorGreen, "FPS: %.0f", getFPS());
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 0.0f));
				switch (set.BlackLight.menu_hud_set)
				{
				case MOD_HUD_MENU_BASIC_INFO:
				{
					ImGui::SameLine((ImGui::GetWindowSize().x - ImGui::GetWindowSize().x) + ImGui::CalcTextSize("FPS: 999").x + 20.0f);

					ImGui::TextColored(ImColorBanana, "In Vehicle: %d", pSampMulti->GetPlayersInVehicleCount());
					ImGui::SameLine();
					ImGui::TextColored(ImColorBanana, "Players: %d", pSampMulti->getPlayersOnServerCount());
					ImGui::SameLine();
					ImGui::TextColored(ImColorBanana, "Players In Stream: %d", pSampMulti->GetPlayersInStream());
					ImGui::SameLine();
					ImGui::TextColored(ImColorBanana, "AFK Players: %d", pSampMulti->GetAFKPlayersCount());
					pMenu->AddItemDescription("streamed");
					ImGui::SameLine();
					ImGui::TextColored(ImColorBanana, "Vehicles In Stream: %d", pSampMulti->GetVehiclesInStreamCount());
					ImGui::SameLine();
					pSampMulti->ImAllingItem(ImGui::GetWindowWidth() - (ImGui::CalcTextSize("Time Playing: 00:00:00").x + 10.0f));
					ImGui::TextColored(ImColorOrange, "Time Playing: %s", pSampMulti->getTimeCounter());
					break;
				}

				case MOD_HUD_MENU_FUNCTIONS: //display hud functions
				{
					ImGui::SameLine((ImGui::GetWindowSize().x - ImGui::GetWindowSize().x) + ImGui::CalcTextSize("FPS: 999").x + 20.0f);
					ImGui::Text("");
					pMenu->AddHudItem(&BlackLightFuncs->bPlayerGhost, "Ghost##HudItemGhost");
					pMenu->AddHudItem(&BlackLightFuncs->bVehicleNoCollision, "Vehicle Collision##HudItemVehCol");
					pMenu->AddHudItem(&BlackLightFuncs->bSmartInvis, "Invisible##HudItemInvis");
					pMenu->AddHudItem(&BlackLightFuncs->bActorNoFall, "No Fall##HudItemNoFall");
					pMenu->AddHudItem(&BlackLightFuncs->bFakeAfk, "Fake AFK##HudItemFakeAfk");
					pMenu->AddHudItem(&BlackLightFuncs->bPizdarvankaPlayer, "Slapper##HudItemSlapper");
					pMenu->AddHudItem(&BlackLightFuncs->bAirbreakPlayer, "Airbreak##HudItemAirbreak");
					pMenu->AddHudItem(&BlackLightFuncs->bInvertWalk, "Invert Walk##HudItemInvWalk");
					pMenu->AddHudItem(&BlackLightFuncs->bBulletTracers, "Bullet Tracers##HudItemBulletTracers");
					pMenu->AddHudItem(&BlackLightFuncs->bNoSpread, "No Spread##HudItemNoSpread");
					pMenu->AddHudItem(&BlackLightFuncs->bFastRotation, "Fast Rotation##HudItemFastRotation");
					pMenu->AddHudItem(&BlackLightFuncs->bVehicleJump, "Vehicle Hop##HudItemVehicleHop");
					pMenu->AddHudItem(&BlackLightFuncs->bVehicleBoost, "Vehicle Boost##HudItemVehicleBoost");

					ImGui::SameLine();
					pSampMulti->ImAllingItem(ImGui::GetWindowWidth() - (ImGui::CalcTextSize("Time Playing: 00:00:00").x + 10.0f));
					ImGui::TextColored(ImColorOrange, "Time Playing: %s", pSampMulti->getTimeCounter());
					break;
				}

				case MOD_HUD_MENU_MUSIC:
				{
					if (BASS_ChannelIsActive(mp3_channel))
					{
						ImGui::SameLine((ImGui::GetWindowSize().x - ImGui::GetWindowSize().x) + ImGui::CalcTextSize("FPS: 999").x + 20.0f);
						ImGui::Text(ICON_FA_MUSIC "  %s  ", __MP3.current_playing_song.c_str());
						ImGui::SameLine();
						ImGui::Text(ICON_FA_CLOCK "  Length: %s", mp3_duration);
						ImGui::SameLine();
						pSampMulti->ImAllingItem(ImGui::GetWindowWidth() - (ImGui::CalcTextSize("Time Playing: 00:00:00").x + 10.0f));
						ImGui::TextColored(ImColorOrange, "Time Playing: %s", pSampMulti->getTimeCounter());
					}
					else
					{
						ImGui::SameLine((ImGui::GetWindowSize().x - ImGui::GetWindowSize().x) + ImGui::CalcTextSize("FPS: 999").x + 20.0f);
						ImGui::TextColored(ImColorBanana, "Music isn't playing.");
						ImGui::SameLine();
						pSampMulti->ImAllingItem(ImGui::GetWindowWidth() - (ImGui::CalcTextSize("Time Playing: 00:00:00").x + 10.0f));
						ImGui::TextColored(ImColorOrange, "Time Playing: %s", pSampMulti->getTimeCounter());
					}

					break;
				}


				case MOD_HUD_MENU_RADIO:
				{
					if (BASS_ChannelIsActive(radio_channel))
					{
						ImGui::SameLine((ImGui::GetWindowSize().x - ImGui::GetWindowSize().x) + ImGui::CalcTextSize("FPS: 999").x + 20.0f);
						ImGui::TextColored(ImColorWhite, " Playing: %s", pSampMulti->RadioUpdateSongTitle(radio_channel));
						if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
						{
							ImGui::SetClipboardText(pSampMulti->RadioUpdateSongTitle(radio_channel));
							addMessageToChatWindow("Radio song name has been copied.");
						}
						ImGui::SameLine();
						pSampMulti->ImAllingItem(ImGui::GetWindowWidth() - (ImGui::CalcTextSize("Time Playing: 00:00:00").x + 10.0f));
						ImGui::TextColored(ImColorOrange, "Time Playing: %s", pSampMulti->getTimeCounter());
					}
					else
					{
						ImGui::SameLine((ImGui::GetWindowSize().x - ImGui::GetWindowSize().x) + ImGui::CalcTextSize("FPS: 999").x + 20.0f);
						ImGui::TextColored(ImColorBanana, "Radio isn't playing.");
						ImGui::SameLine();
						pSampMulti->ImAllingItem(ImGui::GetWindowWidth() - (ImGui::CalcTextSize("Time Playing: 00:00:00").x + 10.0f));
						ImGui::TextColored(ImColorOrange, "Time Playing: %s", pSampMulti->getTimeCounter());
					}
					break;
				}

				default:
					set.BlackLight.menu_hud_set = MOD_HUD_MENU_BASIC_INFO;
					break;
				}
				ImGui::PopStyleVar(1); //ITEM SPACING
				if (pSampMulti->IsMenuActive() && ImGui::BeginPopupContextWindow("##NewHudMenuSwitch1", ImGuiPopupFlags_MouseButtonRight))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
					ImGui::PushStyleColor(ImGuiCol_Button, ImColorBlue);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColorTransparentBlack);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.858f, 0.377f, 0.377f, 0.603f));
					if (ImGui::Button("Basic Info##HudMenuBasicInfo", ImVec2(70.0f, 25.0f)))
					{
						if (set.BlackLight.menu_hud_set != MOD_HUD_MENU_BASIC_INFO)
							set.BlackLight.menu_hud_set = MOD_HUD_MENU_BASIC_INFO;
					}
					ImGui::Spacing();
					if (ImGui::Button("Functions##HudMenuFunctions", ImVec2(70.0f, 25.0f)))
					{
						if (set.BlackLight.menu_hud_set != MOD_HUD_MENU_FUNCTIONS)
							set.BlackLight.menu_hud_set = MOD_HUD_MENU_FUNCTIONS;
					}
					ImGui::Spacing();
					if (ImGui::Button("Music##HudMenuMusicInfo", ImVec2(70.0f, 25.0f)))
					{
						if (set.BlackLight.menu_hud_set != MOD_HUD_MENU_MUSIC)
							set.BlackLight.menu_hud_set = MOD_HUD_MENU_MUSIC;
					}
					ImGui::Spacing();
					if (ImGui::Button("Radio##HudMenuRadioInfo", ImVec2(70.0f, 25.0f)))
					{
						if (set.BlackLight.menu_hud_set != MOD_HUD_MENU_RADIO)
							set.BlackLight.menu_hud_set = MOD_HUD_MENU_RADIO;
					}
					ImGui::PopStyleVar(1);
					ImGui::PopStyleColor(3); //buttons
					ImGui::EndPopup();
				}
				ImGui::PopFont();


				ImGui::EndChild();
			}
			ImGui::PopStyleVar(1);

			ImGui::End();
		}
	}
}


void CBlackLightMenu::ImFriendsAndAdminsMenu(bool bEnable)
{
	traceLastFunc("ImFriendsAndAdminsMenu");

	if (IS_CHEAT_PANIC_ACTIVE || gta_menu_active())
		return;

	if (bEnable)
	{
		float fMainMenuW = set.BlackLight.menu_admins_and_friends == MOD_JOINER_LOGGER ? 400.0f : 300.0f, fMainMenuH = 250.0f, fHudHeight = 60.0f;
		ImGui::SetNextWindowSize(ImVec2(fMainMenuW, fMainMenuH));
		ImGui::SetNextWindowPos(ImVec2((float)pPresentParam.BackBufferWidth - fMainMenuW, BlackLightFuncs->Menu.bImGuiHudMenu ? (float)pPresentParam.BackBufferHeight  - (fMainMenuH + fHudHeight) : (float)pPresentParam.BackBufferHeight - fMainMenuH));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 8.0f));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.000f,0.000f, 0.000f,0.000f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
		ImGui::Begin("##ImFriends&AdminsMenu", &bEnable, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(3);

		if (BlackLightFuncs->Menu.bImGuiHudMenu == false)
		{
			if (pSampMulti->IsMenuActive() && ImGui::BeginPopupContextWindow())
			{
				//ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
				if (ImGui::Button("Friends##DisplayFriends1"))
				{
					if (set.BlackLight.menu_admins_and_friends != MOD_FRIENDS_MENU)
						set.BlackLight.menu_admins_and_friends = MOD_FRIENDS_MENU;
				}
				if (ImGui::Button("Admins##DisplayAdmins1"))
				{
					if (set.BlackLight.menu_admins_and_friends != MOD_ADMINS_MENU)
						set.BlackLight.menu_admins_and_friends = MOD_ADMINS_MENU;
				}
				if (ImGui::Button("Player Joiner##PlayerJoinerBtn"))
				{
					if (set.BlackLight.menu_admins_and_friends != MOD_JOINER_LOGGER)
						set.BlackLight.menu_admins_and_friends = MOD_JOINER_LOGGER;
				}
				if (ImGui::Button("Settings##DisplaySettings1"))
				{
					if (set.BlackLight.menu_admins_and_friends != FRIENDS_ADMINS_SETTINGS_MENU)
						set.BlackLight.menu_admins_and_friends = FRIENDS_ADMINS_SETTINGS_MENU;
				}
			//	ImGui::PopStyleColor(1);
				ImGui::EndPopup();
			}
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f,0.0f));
		switch (set.BlackLight.menu_admins_and_friends)
		{

		case MOD_FRIENDS_MENU:
		{
			if (ImGui::IsWindowHovered())
			{
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					BlackLightFuncs->bViewAllFriends ^= 1;
			}

			if (BlackLightFuncs->bViewAllFriends)
			{
				file_friends.open("BlackLight\\friends.ini", std::fstream::in);
				if (file_friends.is_open())
				{
					ImGui::Text("");
					ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize("From File:").x + 1.0f));
					ImGui::Text("From File:");

					for (std::string szfriends; std::getline(file_friends, szfriends);)
					{
						if (!szfriends.empty())
						{
							ImGui::Text("");
							ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize(szfriends.c_str()).x + 8.0f));
							ImGui::PushStyleColor(ImGuiCol_Button, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorTransparentBlack);
							ImGui::Button(pSampMulti->SetText("%s", szfriends.c_str()));
							ImGui::PopStyleColor(3);
							if (pSampMulti->IsMenuActive() && ImGui::IsItemClicked(ImGuiMouseButton_Right)) //delete friend
							{
								pSampMulti->DeleteFileText(M0D_FOLDER, "friends", ".ini", szfriends.c_str());
							}
						}
					}
					file_friends.close();
				}
			}
			else
			{
				ImGui::Text("");
				ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize(pSampMulti->SetText("Friends: %d" , pSampMulti->GetFriendsCount())).x + 3.0f));
				ImGui::TextColored(ImColorGreen ,"Friends: %d", pSampMulti->GetFriendsCount());
				for (int iplayers = 0; iplayers < SAMP_MAX_PLAYERS; iplayers++)
				{
					if (BlackLightFuncs->bIsFriend[iplayers])
					{
						if (BlackLightFuncs->bUsePlayersColor)
						{
							ImGui::Text("");
							ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize(pSampMulti->SetText(pSampMulti->IsPlayerStreamed(iplayers) ? "%s(%d) [Streamed]" : "%s(%d)", getPlayerName(iplayers), iplayers)).x + 8.0f));
							ImGui::PushStyleColor(ImGuiCol_Button, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ImU32(samp_color_get(iplayers))));
							ImGui::Button(pSampMulti->SetText(pSampMulti->IsPlayerStreamed(iplayers) ? "%s(%d) [Streamed]" : "%s(%d)", getPlayerName(iplayers), iplayers));
							ImGui::PopStyleColor(4);
						}
						else
						{
							ImGui::Text("");
							ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize(pSampMulti->SetText(pSampMulti->IsPlayerStreamed(iplayers) ? "%s(%d) [Streamed]" : "%s(%d)", getPlayerName(iplayers), iplayers)).x + 8.0f));
							ImGui::PushStyleColor(ImGuiCol_Button, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_Text, ImColorWhite);
							ImGui::Button(pSampMulti->SetText(pSampMulti->IsPlayerStreamed(iplayers) ? "%s(%d) [Streamed]" : "%s(%d)", getPlayerName(iplayers), iplayers));
							ImGui::PopStyleColor(4);
						}
						pMenu->AddItemDescription(pSampMulti->SetText(pSampMulti->IsPlayerStreamed(iplayers) ? "%s(%d) [Streamed]\nRight click to delete" : "%s(%d)\nRight click to delete", getPlayerName(iplayers), iplayers));
						if (pSampMulti->IsMenuActive() && ImGui::IsItemClicked(ImGuiMouseButton_Right))
						{
							if (!pSampMulti->IsModDeveloper(iplayers))
								pSampMulti->DeleteFriend(iplayers);
						}
					}
				}
			}

			break;
		}

		case MOD_ADMINS_MENU:
		{
			if (ImGui::IsWindowHovered())
			{
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					BlackLightFuncs->bViewAllAdmins ^= 1;
			}

			if (BlackLightFuncs->bViewAllAdmins)
			{
				file_admins.open("BlackLight\\admins.ini", std::fstream::in);
				if (file_admins.is_open())
				{
					ImGui::Text("");
					ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize("From File:").x + 1.0f));
					ImGui::Text("From File:");

					for (std::string szadmins; std::getline(file_admins, szadmins);)
					{
						if (!szadmins.empty())
						{
							ImGui::Text("");
							ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize(szadmins.c_str()).x + 8.0f));
							ImGui::PushStyleColor(ImGuiCol_Button, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorTransparentBlack);
							ImGui::Button(pSampMulti->SetText("%s", szadmins.c_str()));
							ImGui::PopStyleColor(3);
							if (pSampMulti->IsMenuActive() && ImGui::IsItemClicked(ImGuiMouseButton_Right)) //delete admin
							{
								pSampMulti->DeleteFileText(M0D_FOLDER, "admins", ".ini", szadmins.c_str());
							}
						}
					}
					file_admins.close();
				}
			}
			else
			{
				ImGui::Text("");
				ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize(pSampMulti->SetText("Admins: %d", pSampMulti->GetAdminsCount())).x + 3.0f));
				ImGui::TextColored(ImColorRed, "Admins: %d", pSampMulti->GetAdminsCount());
				for (int iplayers = 0; iplayers < SAMP_MAX_PLAYERS; iplayers++)
				{
					if (BlackLightFuncs->bIsAdmin[iplayers])
					{
						if (BlackLightFuncs->bUsePlayersColor)
						{
							ImGui::Text("");
							ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize(pSampMulti->SetText(pSampMulti->IsPlayerStreamed(iplayers) ? "%s(%d) [Streamed]" : "%s(%d)", getPlayerName(iplayers), iplayers)).x + 8.0f));
							ImGui::PushStyleColor(ImGuiCol_Button, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ImU32(samp_color_get(iplayers))));
							ImGui::Button(pSampMulti->SetText(pSampMulti->IsPlayerStreamed(iplayers) ? "%s(%d) [Streamed]" : "%s(%d)", getPlayerName(iplayers), iplayers));
							ImGui::PopStyleColor(4);
						}
						else
						{
							ImGui::Text("");
							ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize(pSampMulti->SetText(pSampMulti->IsPlayerStreamed(iplayers) ? "%s(%d) [Streamed]" : "%s(%d)", getPlayerName(iplayers), iplayers)).x + 8.0f));
							ImGui::PushStyleColor(ImGuiCol_Button, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_Text, ImColorWhite);
							ImGui::Button(pSampMulti->SetText(pSampMulti->IsPlayerStreamed(iplayers) ? "%s(%d) [Streamed]" : "%s(%d)", getPlayerName(iplayers), iplayers));
							ImGui::PopStyleColor(4);
						}
						pMenu->AddItemDescription(pSampMulti->SetText(pSampMulti->IsPlayerStreamed(iplayers) ? "%s(%d) [Streamed]\nRight click to delete" : "%s(%d)\nRight click to delete", getPlayerName(iplayers), iplayers));
						if (pSampMulti->IsMenuActive() && ImGui::IsItemClicked(ImGuiMouseButton_Right))
						{
							if (!pSampMulti->IsModDeveloper(iplayers))
								pSampMulti->DeleteAdmin(iplayers);
						}
					}
				}
			}
			break;
		}

		case MOD_JOINER_LOGGER: //raknet logger
		{
			std::list<std::string>::iterator itBegin = gRaknetLogger.begin();
			std::list<std::string>::iterator itEnd = gRaknetLogger.end();
			ImGui::Text("");
			ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize("Logger:").x + 1.0f));
			ImGui::TextColored(ImColorBanana,"Logger:");
			for (set.raknet_lines_limit = 0; itBegin != itEnd && set.raknet_lines_limit < set.BlackLight.player_joiner_lines_size; itBegin++, set.raknet_lines_limit++)
			{
				ImGui::Text("");
				ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize(itBegin->c_str()).x + 4.0f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
				ImGui::PushStyleColor(ImGuiCol_Button, ImColorTransparentBlack);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorTransparentBlack);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.000f, 0.500f));
				ImGui::Button(itBegin->c_str(), ImVec2(ImGui::CalcTextSize(itBegin->c_str()).x + 4.5f, 21.0f));
				ImGui::PopStyleColor(4);
				pMenu->AddItemDescription(itBegin->c_str());
			}

			break;
		}

		case FRIENDS_ADMINS_SETTINGS_MENU:
		{
			ImGui::Text("");
			ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize("Settings:").x + 3.0f));
			ImGui::Text("Settings:");
			pMenu->AddButton(&BlackLightFuncs->bUsePlayersColor,"Use Players Colors##SettingsFunc1");

			ImGui::Text("");
			ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize("Player Joiner:").x + 3.0f));
			ImGui::Text("Player Joiner:");
			pMenu->AddButton(&BlackLightFuncs->bPlayerJoiner, "Logger##SettingsFunc2", 70.0f);
			pMenu->AddItemDescription("a.k.a raknet logger\nsee players who join & leave server with reason.");
			pMenu->AddIncDecButtons(set.BlackLight.player_joiner_lines_size, 1, 10, 0, true, "##playerJoiner++", "##playerJoiner--", pSampMulti->SetText("Max lines 10\nCurrent lines: %d", set.BlackLight.player_joiner_lines_size), pSampMulti->SetText("Minimum lines 0\nCurrent lines: %d", set.BlackLight.player_joiner_lines_size));

			break;
		}

		default:
			set.BlackLight.menu_admins_and_friends = MOD_FRIENDS_MENU;
			break;
		}
		ImGui::PopStyleVar(1);

		ImGui::End();
	}
}

#define IM_SCOREBOARD_WIDTH 770.0f + 10.0f
#define IM_SCOREBOARD_HEIGHT 600.0f + 60.0f
#define IM_SCOREBOARD_ID_POS_X 3.0f
#define IM_SCOREBOARD_NAMES_POS_X 45.0f
#define IM_SCOREBOARD_STATUS_POS_X 290.0f
#define IM_SCOREBOARD_SCORE_POS_X 460.0f
#define IM_SCOREBOARD_PING_POS_X 670.0f

void CBlackLightMenu::ImScoreboard(bool bEnable)
{
	if (!BlackLightFuncs->bUseNewScoreboard)
		return;

	if (!g_SAMP || !g_Players || !pGameInterface)
		return;

	traceLastFunc("ImScoreboardNew()");

	if (g_Input->iInputEnabled)
		return;

	if (bEnable)
	{
		ImGui::SetNextWindowSize(ImVec2(IM_SCOREBOARD_WIDTH, IM_SCOREBOARD_HEIGHT)); //default
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::Begin("##ImScoreboardMenu", &bEnable, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings);
		ImGui::PopStyleVar(1);
		ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1.000f, 1.000f, 1.000f, 0.100f));

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.000f, 0.000f, 0.000f, 0.800f));
		ImGui::BeginChild("##ImSCRBChild1", ImVec2(IM_SCOREBOARD_WIDTH - 10.0f, 40.0f));
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(1);
		ImGui::PushFont(pFontArialV2);

		pSampMulti->ImAllingItem(IM_SCOREBOARD_ID_POS_X - 0.5f); ImGui::Text("ID");
		ImGui::SameLine(IM_SCOREBOARD_NAMES_POS_X - 3.0f);  ImGui::Text("Name");
		ImGui::SameLine(IM_SCOREBOARD_STATUS_POS_X - 5.0f);  ImGui::Text("Status");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Show if player is\nAdmin(red*)\nFriend(green*)\nGroup Member(blue*)\nMod Dev(yellow*)");

			ImGui::EndTooltip();
		}
		ImGui::SameLine(IM_SCOREBOARD_SCORE_POS_X - 3.0f);  ImGui::Text("Score");
		ImGui::SameLine(IM_SCOREBOARD_PING_POS_X - 3.0f); ImGui::Text("Ping");

		ImGui::Separator();

		pSampMulti->ImAllingItem(IM_SCOREBOARD_ID_POS_X);
		ImGui::Text("%d", g_Players->sLocalPlayerID);
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Your id");
			ImGui::EndTooltip();
		}
		ImGui::SameLine(IM_SCOREBOARD_NAMES_POS_X);
		ImGui::Text("%s", getPlayerName(g_Players->sLocalPlayerID));
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("You.");
			ImGui::EndTooltip();
		}
		////////////////////////STATUS////////////////////////
		if (BlackLightFuncs->bIsAdmin[g_Players->sLocalPlayerID])
		{
			ImGui::SameLine(IM_SCOREBOARD_STATUS_POS_X);
			ImGui::TextColored(ImColorRed, "*");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("You are in Admin list");
				ImGui::EndTooltip();
			}
		}
		if (BlackLightFuncs->bIsFriend[g_Players->sLocalPlayerID])
		{
			ImGui::SameLine(BlackLightFuncs->bIsAdmin[g_Players->sLocalPlayerID] ? 0.0f : IM_SCOREBOARD_STATUS_POS_X);
			ImGui::TextColored(ImColorGreen, "*");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("You are in Friend list");
				ImGui::EndTooltip();
			}
		}
		if (pSampMulti->IsModDeveloper(g_Players->sLocalPlayerID))
		{
			ImGui::SameLine(BlackLightFuncs->bIsAdmin[g_Players->sLocalPlayerID] || BlackLightFuncs->bIsFriend[g_Players->sLocalPlayerID]  ? 0.0f : IM_SCOREBOARD_STATUS_POS_X);
			ImGui::TextColored(ImColorBanana, "*");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Mod Developer");
				ImGui::EndTooltip();
			}
		}
		/////////////////////////////////////////////////////////
		ImGui::SameLine(IM_SCOREBOARD_SCORE_POS_X);
		ImGui::Text("%d", pSampMulti->getPlayerScore(g_Players->sLocalPlayerID));
		ImGui::SameLine(IM_SCOREBOARD_PING_POS_X);
		ImGui::Text("%d", pSampMulti->getPlayerPing(g_Players->sLocalPlayerID));

		ImGui::PopFont();
		ImGui::EndChild();

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 12.0f);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.000f, 0.000f, 0.000f, 0.800f));
		ImGui::BeginChild("##ImSCRBChild2", ImVec2(IM_SCOREBOARD_WIDTH - 10.0f, IM_SCOREBOARD_HEIGHT - 110.0f));
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);
		ImGui::PushFont(pFontArial);

		for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
		{
			if (g_Players->iIsListed[players] != 1)
				continue;

			if (g_Players->pRemotePlayer[players] == NULL)
				continue;

			D3DCOLOR color = samp_color_get(players);
			ImGui::Separator();
			if (players != g_Players->sLocalPlayerID)
			{
				//we are not using pSampMulti->getPlayerScore(id) etc.. 'cus it's for streamed
				pSampMulti->ImAllingItem(IM_SCOREBOARD_ID_POS_X);
				ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color), "%d", players);
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("%s ID is %d", getPlayerName(players), players);
					ImGui::EndTooltip();
				}
				ImGui::SameLine(IM_SCOREBOARD_NAMES_POS_X);
				ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color), "%s", getPlayerName(players));
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("%s", getPlayerName(players));
					ImGui::EndTooltip();
				}
				////////////////////////STATUS////////////////////////
				if (BlackLightFuncs->bIsAdmin[players])
				{
					ImGui::SameLine(IM_SCOREBOARD_STATUS_POS_X);
					ImGui::TextColored(ImColorRed, "*");
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("%s is in Admin list", getPlayerName(players));
						ImGui::EndTooltip();
					}
				}
				if (BlackLightFuncs->bIsFriend[players])
				{
					ImGui::SameLine(BlackLightFuncs->bIsAdmin[players] ? 0.0f : IM_SCOREBOARD_STATUS_POS_X);
					ImGui::TextColored(ImColorGreen, "*");
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("%s is in Friend list", getPlayerName(players));
						ImGui::EndTooltip();
					}
				}
				if (pSampMulti->IsModDeveloper(players))
				{
					ImGui::SameLine(BlackLightFuncs->bIsAdmin[players] || BlackLightFuncs->bIsFriend[players] ? 0.0f : IM_SCOREBOARD_STATUS_POS_X);
					ImGui::TextColored(ImColorBanana, "*");
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("%s is Mod Developer", getPlayerName(players));
						ImGui::EndTooltip();
					}
				}
				/////////////////////////////////////////////////////////

				ImGui::SameLine(IM_SCOREBOARD_SCORE_POS_X);
				ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color), "%d", g_Players->pRemotePlayer[players]->iScore);
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("%s score is %d", getPlayerName(players), g_Players->pRemotePlayer[players]->iScore);
					ImGui::EndTooltip();
				}
				ImGui::SameLine(IM_SCOREBOARD_PING_POS_X);
				ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color), "%d", g_Players->pRemotePlayer[players]->iPing);
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("%s ping is %d", getPlayerName(players), g_Players->pRemotePlayer[players]->iPing);
					ImGui::EndTooltip();
				}
			}
		}

		ImGui::PopFont();
		ImGui::EndChild();

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 12.0f);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.000f, 0.000f, 0.000f, 0.800f));
		ImGui::BeginChild("##ImSCRBChild3", ImVec2(IM_SCOREBOARD_WIDTH - 10.0f, 40));
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);
		ImGui::PushFont(pFontArial);

		pSampMulti->ImSpacing(2); pSampMulti->ImAllingItem(3.0f);
		if (pSampMulti->IsCurrentServer()) ImGui::Text("UIF - United Islands Freeroam | IP: 51.254.85.134:7776");
		else ImGui::Text("%s | IP: %s", g_SAMP->szHostname, pSampMulti->getServerIp());
		ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize("Players: 999").x + 6.0f));
		ImGui::Text("Players: %d", pSampMulti->getPlayersOnServerCount());

		ImGui::PopFont();
		ImGui::EndChild();

		ImGui::PopStyleColor(1); //separator
		ImGui::End();
	}
}

void CBlackLightMenu::ImInitBlackLightPlayersTags(bool bEnable)
{
	traceLastFunc("ImInitBlackLight3DInfoPlayer()");

	if (!g_SAMP)
		return;

	if (!pGameInterface)
		return;

	if (gta_menu_active())
		return;
	
	if (g_Scoreboard->iIsEnabled || BlackLightFuncs->Menu.bImScoreboad)
		return;

	static int samp_names = 2;
	if (bEnable)
	{
		float fButtonsWidht = 100.0f;
		samp_names = 0;
		sampPatchDisableNameTags(1);
		static bool bOff[SAMP_MAX_PLAYERS]; //deactivate hp, armor bar for specific player
		for (int iplayers = 0; iplayers < SAMP_MAX_PLAYERS; iplayers++)
		{
			if (g_Players->iIsListed[iplayers] != 1)
				continue;

			actor_info* info = getGTAPedFromSAMPPlayerID(iplayers);

			if (!info)
				continue;

			CVector vecHead;
			vecHead = pSampMulti->CVecGetBonePos(iplayers, BONE_HEAD);
			float* pPos = &info->base.matrix[12];

			if (vect3_dist(pPos, &g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->base.matrix[4 * 3]) > set.BlackLight.SpecialESP.fDistancePlayersTags)
				continue;

			D3DXVECTOR3 getvec, toscreen;
			getvec.x = vecHead.fX;
			getvec.y = vecHead.fY;
			getvec.z = vecHead.fZ +0.5;
			pSampMulti->CalcScreenCoors(&getvec, &toscreen);

			if (toscreen.z < 1.f)
				continue;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::SetNextWindowSize(ImVec2(180.0f, 60.0f));
			ImGui::SetNextWindowPos(ImVec2(toscreen.x, toscreen.y));
			if (ImGui::Begin(pSampMulti->SetText("##MenuID%d",iplayers), &bEnable, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
			{
				ImGui::PopStyleVar(1);

				if (pSampMulti->IsMenuActive() && ImGui::BeginPopupContextWindow())
				{
					if (BlackLightFuncs->bIsAdmin[iplayers])
					{
						if (ImGui::Button("Delete Admin##42342", ImVec2(fButtonsWidht, 25.0f)))
						{
							pSampMulti->DeleteAdmin(iplayers);
						}
					}
					else
					{
						if (ImGui::Button("Add Admin##23424", ImVec2(fButtonsWidht, 25.0f)))
						{
							pSampMulti->AddAdmin(iplayers);
						}
					}

					if (BlackLightFuncs->bIsFriend[iplayers])
					{
						if (ImGui::Button("Delete Friend##DelFriend", ImVec2(fButtonsWidht, 25.0f)))
						{
							addMessageToChatWindow(">>>>>>> Deleted Friend %s(%d)", getPlayerName(iplayers), iplayers);
							pSampMulti->DeleteFriend(iplayers);
						}
					}
					else
					{
						if (ImGui::Button("Add Friend##AddFriend", ImVec2(fButtonsWidht, 25.0f)))
						{
							pSampMulti->AddFriend(iplayers);
						}
					}

					if (BlackLightFuncs->Mute.bMutedPlayers[iplayers])
					{
						if (ImGui::Button("Unmute##UnmutePlayer", ImVec2(fButtonsWidht, 25.0f)))
						{
							pSampMulti->DeleteFileText(M0D_FOLDER, "MutedPlayers", ".ini", getPlayerName(iplayers));
						}
					}
					else
					{
						if (ImGui::Button("Mute##MutePlayer", ImVec2(fButtonsWidht, 25.0f)))
						{
							if (BlackLightFuncs->bIsFriend[iplayers])
								return addMessageToChatWindow("You can't mute friends.");

							if (pSampMulti->IsModDeveloper(iplayers))
								return addMessageToChatWindow("You can't mute mod developer");

							pSampMulti->WriteToFile("BlackLight\\MutedPlayers.ini", getPlayerName(iplayers));
							addMessageToChatWindow(">>>>>>> Muted %s(%d)", getPlayerName(iplayers), iplayers);
						}
					}

					ImGui::Separator();
					if (ImGui::Button("Disable bars", ImVec2(fButtonsWidht, 25.0f)))
					{
						bOff[iplayers] ^= 1;
					}
					pMenu->AddItemDescription("No HP/Armor bar");

					ImGui::EndPopup();
				}

				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 0.0f));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 1.0f));

				ImGui::PushFont(pFontSmall);
				if (BlackLightFuncs->bIsFriend[iplayers]) ImGui::TextColored(ImColorGreen, "Friend");
				if (BlackLightFuncs->bIsAdmin[iplayers]) { ImGui::SameLine(); ImGui::TextColored(ImColorRed, "Admin"); }
				ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(samp_color_get(iplayers)), pSampMulti->IsModDeveloper(iplayers) ? 
					g_Players->pRemotePlayer[iplayers]->pPlayerData->iAFKState == 2 ? "[AFK] [ModDev]%s(%d)" : "[ModDev]%s(%d)" : 
					g_Players->pRemotePlayer[iplayers]->pPlayerData->iAFKState == 2 ? "[AFK]%s(%d)" : "%s(%d)", getPlayerName(iplayers), iplayers);

				if (!bOff[iplayers])
				{
					if (pSampMulti->getPlayerHealth(iplayers) > 0)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImColorRed);
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorRed);
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColorRed);
						ImGui::Button("##Health", ImVec2((float)pSampMulti->getPlayerHealth(iplayers), 10.0f));
						ImGui::PopStyleColor(3);
						ImGui::SameLine((float)pSampMulti->getPlayerHealth(iplayers) - (ImGui::CalcTextSize(pSampMulti->SetText("%d", pSampMulti->getPlayerHealth(iplayers))).x));
						ImGui::TextColored(ImColorWhite, "%d", pSampMulti->getPlayerHealth(iplayers));
					}
					if (pSampMulti->getPlayerArmor(iplayers) > 0)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImColorWhite);
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorWhite);
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColorWhite);
						ImGui::Button("##Armor", ImVec2((float)pSampMulti->getPlayerArmor(iplayers), 10.0f));
						ImGui::PopStyleColor(3);

						ImGui::SameLine((float)pSampMulti->getPlayerArmor(iplayers) - (ImGui::CalcTextSize(pSampMulti->SetText("%d", pSampMulti->getPlayerArmor(iplayers))).x));
						ImGui::TextColored(ImColorTransparentBlack, "%d", pSampMulti->getPlayerArmor(iplayers));
					}
				}
				ImGui::PopFont();

				ImGui::PopStyleVar(3);
				ImGui::End();
			}
		}
	}
	else
	{
		if (samp_names == 0)
		{
			samp_names++;
			sampPatchDisableNameTags(0);
		}
	}
}

void CBlackLightMenu::ImInitBlackLightPlayersInfo(bool bEnable)
{ 
	if (!bEnable)
		return;

	traceLastFunc("ImInitBlackLightPlayersInfo()");

	if (!g_SAMP)
		return;

	if (!pGameInterface || gta_menu_active())
		return;

	if (g_Scoreboard->iIsEnabled || BlackLightFuncs->Menu.bImScoreboad)
		return;

	if (bEnable)
	{
		for (int iplayers = 0; iplayers < SAMP_MAX_PLAYERS; iplayers++)
		{
			if (g_Players->iIsListed[iplayers] != 1)
				continue;

			actor_info* info = getGTAPedFromSAMPPlayerID(iplayers);

			if (!info)
				continue;

			if (pSampMulti->IsModDeveloper(iplayers))
				break;
			if (BlackLightFuncs->bIsFriend[iplayers])
				break;

			D3DXVECTOR3 draw_getvec, info_toscreen;
			CVector vecBone;
			vecBone = pSampMulti->CVecGetBonePos(iplayers, BONE_SPINE1);
			float* pPos = &info->base.matrix[12];

			if (vect3_dist(pPos, &g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->base.matrix[4 * 3]) > set.BlackLight.SpecialESP.fDistancePlayersInfo)
				continue;

			draw_getvec.x = vecBone.fX;
			draw_getvec.y = vecBone.fY;
			draw_getvec.z = vecBone.fZ;
			pSampMulti->CalcScreenCoors(&draw_getvec, &info_toscreen);

			if (info_toscreen.z < 1.f)
				continue;

			if (set.BlackLight.get_damaged_player_for_imgui == iplayers)
				break;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColorTransparentBlack);
			ImGui::SetNextWindowSize(ImVec2(230.0f, 130.0f));
			ImGui::SetNextWindowPos(ImVec2(info_toscreen.x, info_toscreen.y));
			if (ImGui::Begin(pSampMulti->SetText("##PlayersInfoMenuID%d", iplayers), &bEnable, /*ImGuiWindowFlags_NoBackground |*/ ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
			{
				ImGui::PopStyleColor(1);
				ImGui::PopStyleVar(1);

				ImGui::PushFont(pFontSmall);
				if (g_BotFuncs->uiIsBot[iplayers][0] == 1)
				{
					ImGui::Text("BOT Name/ID: %s(%d) (BOT ID: %d)", getPlayerName(iplayers), iplayers, g_BotFuncs->uiIsBot[iplayers][1]);
				}
				else 	ImGui::Text("Name/ID: %s(%d)", getPlayerName(iplayers), iplayers);
				ImGui::Text("Health: %d, Armor: %d", pSampMulti->getPlayerHealth(iplayers), pSampMulti->getPlayerArmor(iplayers));
				ImGui::Text("Weapon/Model: %s(%d)", pSampMulti->getPlayerWeapon(iplayers), pSampMulti->getPlayerWeaponModelID(iplayers));
				if (pSampMulti->IsPlayerInCar(iplayers))
					ImGui::Text("Vehicle/Model: %s(%d)", pSampMulti->getPlayerVehicleName(iplayers), pSampMulti->getPlayerVehicleModelID_V2(iplayers));
				else ImGui::Text("Vehicle: Not in vehicle.");
				ImGui::Text("Sprint Energy: %.1f",pSampMulti->getPlayerSprintEnergy(iplayers));
				ImGui::Text("Position: X %0.2f, Y %0.2f, Z %0.2f", pPos[0], pPos[1], pPos[2]);

				ImGui::PushStyleColor(ImGuiCol_Button, ImColorTransparentBlack);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, pSampMulti->IsMenuActive() ? ImColorDeepOrange : ImColorTransparentBlack);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorTransparentBlack);
				if (ImGui::Button("Say##SayInfo", ImVec2(ImGui::GetWindowSize().x - 15.0f,20.0f)))
				{
					if(pSampMulti->IsMenuActive())
					pSampMulti->SayPlayerInfo(iplayers);
				}
				ImGui::PopStyleColor(3);
				ImGui::PopFont();
				ImGui::End();
			}
		}
	}
}

void CBlackLightMenu::ImInitBlakcLightRadioVolume(bool bEnable)
{
	traceLastFunc("ImInitBlakcLightRadioVolume()");

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (gta_menu_active())
		return;

	if(bEnable && pSampMulti->IsMenuActive())
	if (BASS_ChannelIsActive(radio_channel) == BASS_ACTIVE_PLAYING)
		BASS_SetVolume(__Radio.radio_volume);
	else if(BASS_ChannelIsActive(mp3_channel) == BASS_ACTIVE_PLAYING)
		BASS_SetVolume(__MP3.mp3_volume);

	static float fHudHeight = 60.0f, fMenuWidth = 190.0f;

	if (bEnable && pSampMulti->IsMenuActive() && BASS_ChannelIsActive(radio_channel) == BASS_ACTIVE_PLAYING)
	{
		ImGui::SetNextWindowPos(ImVec2(0.0f,(float)pPresentParam.BackBufferHeight - (fMenuWidth + fHudHeight)));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColorTransparentBlack);
		if (ImGui::Begin("##RadioVolume", &bEnable, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::PopStyleColor(1);
			ImGui::PopStyleVar(2);
			ImGui::VSliderFloat("##RadioVolumeSlider", ImVec2(30.0f, 200.0f), &__Radio.radio_volume, 0.00f, 1.00, "%.2f");

			ImGui::End();
		}
	}
	else 	if (bEnable && pSampMulti->IsMenuActive() && BASS_ChannelIsActive(mp3_channel) == BASS_ACTIVE_PLAYING)
	{
		ImGui::SetNextWindowPos(ImVec2(0.0f, (float)pPresentParam.BackBufferHeight - (fMenuWidth + fHudHeight)));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColorTransparentBlack);
		if (ImGui::Begin("##MP3Volume", &bEnable, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::PopStyleColor(1);
			ImGui::PopStyleVar(2);
			ImGui::VSliderFloat("##MP3VolumeSlider", ImVec2(30.0f, 200.0f), &__MP3.mp3_volume, 0.00f, 1.00, "%.2f");

			ImGui::End();
		}
	}
}

char szPlayURL[1024] = "Input your audio URL";
void CBlackLightMenu::ImInitBlackLight_AudioStreamPlayer(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("ImInitBlackLight_AudioStreamPlayer()");

	if (g_Scoreboard->iIsEnabled)
		return;

	if (g_Input->iInputEnabled) //samp chat input is active
		return;

	if (IS_CHEAT_PANIC_ACTIVE || gta_menu_active())
		return;

	if (bEnable)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColorTransparentBlack);
		ImGui::SetNextWindowSize(ImVec2(460.0f, 45.0f));
		ImGui::SetNextWindowPos(ImVec2(pPresentParam.BackBufferWidth / 2 - 480.0f / 2, 0.0f));
		if (ImGui::Begin("##AudioStreamPlayer", &bEnable, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse))
		{
			ImGui::PopStyleColor(1);
			ImGui::PopStyleVar(2);

			ImGui::SetNextItemWidth(400.0f);
			if (ImGui::InputText("##InputURL", szPlayURL, IM_ARRAYSIZE(szPlayURL), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_AutoSelectAll))
			{
				g_RakClient2->PlayAudioStream(szPlayURL);
			}
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				pSampMulti->AddText_to_Text(szPlayURL, *(char*)ImGui::GetClipboardText());
			ImGui::SameLine();
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 0.000f));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.000f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.000f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.000f));
			if (ImGui::Button(ICON_FA_PLAY, {40.0f, 18.0f}))
			{
				g_RakClient2->PlayAudioStream(szPlayURL);
			}			
			pMenu->AddItemDescription("Play");
			ImGui::PopStyleColor(4);
			ImGui::PopStyleVar(2);
			
			ImGui::End();
		}
	}
}

void CBlackLightMenu::ImMenuInit_RouteRecording(bool bEnable)
{
	if (!bEnable)
		return;

	if (!pGameInterface || IS_CHEAT_PANIC_ACTIVE || gta_menu_active())
		return;

	traceLastFunc("ImMenuInit_RouteRecording()");

	if (bEnable && pSampMulti->IsOurPlayerInCar())
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColorTransparentBlack);
		ImGui::SetNextWindowSize(ImVec2(set.recording_status == RECORDING_RECORD ? 430.0f : 350.0f, 40.0f));
	//	ImGui::SetNextWindowPos(ImVec2(0.0f, (float)pPresentParam.BackBufferHeight - 200.0f));
		if (ImGui::Begin("##ImRouterRecorder", &bEnable,   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse))
		{
			ImGui::PopStyleColor(1);
			ImGui::PopStyleVar(2);

			ImGui::PushStyleColor(ImGuiCol_Text, ImColorWhite);

			ImGui::PushStyleColor(ImGuiCol_Button, set.recording_status == RECORDING_RECORD ? pSampMulti->ColorOpacityPulse(ImColorGreen) : ImColorGreen);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColorTransparentBlack);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorGreen);
			if (ImGui::Button("RECORD " ICON_FA_PLAY))
			{
				set.recording_status = RECORDING_RECORD;
			}
			ImGui::PopStyleColor(3);
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, set.recording_status == RECORDING_RECORD ? pSampMulti->ColorOpacityPulse(ImColorRed) : ImColorRed);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColorTransparentBlack);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorGreen);
			if (ImGui::Button("STOP " ICON_FA_STOP_CIRCLE))
			{
				set.recording_status = RECORDING_OFF;
			}
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, set.recording_status == RECORDING_RECORD ? pSampMulti->ColorOpacityPulse(ImColorBlue) : ImColorBlue);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColorTransparentBlack);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorGreen);
			if (ImGui::Button("PLAY " ICON_FA_PLAY_CIRCLE))
			{
				set.recording_status = RECORDING_PLAY;
			}
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, rec_continueAfterFin ? pSampMulti->ColorOpacityPulse(ImColorAqua) : ImColorAqua);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColorTransparentBlack);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorAqua);
			if (ImGui::Button("LOOP "))
			{
				rec_continueAfterFin ^= true;
			}
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::TextColored(ImColorBanana, "Status: ");
			ImGui::SameLine();

			switch (set.recording_status)
			{
			case RECORDING_OFF:
			{
				ImGui::TextColored(ImColorRed, "OFF ");
				break;
			}

			case RECORDING_PLAY:
			{
				ImGui::TextColored(ImColorBanana, "Playing route " ICON_FA_ROUTE);
				break;
			}

			case RECORDING_RECORD:
			{
				ImGui::TextColored(ImColorGreen, "REC " ICON_FA_CAMERA);
				break;
			}

			default:
				set.recording_status = RECORDING_OFF;
				break;

			}

			ImGui::PopStyleColor(1); //text color white
			ImGui::End();
		}
	}
}



void CBlackLightMenu::ImInitBlackLight_NewSpeedometer(bool bEnable)
{
	if (!bEnable)
		return;

	traceLastFunc("ImInitBlackLight_NewSpeedometer()");

	if (g_Scoreboard->iIsEnabled)
		return;

	if (bEnable && pSampMulti->IsOurPlayerInCar())
	{
		static bool bSwitchMultipler = false;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColorNone);
		ImGui::SetNextWindowSize(ImVec2(90.0f, 60.0f));
		if (ImGui::Begin("##NewSpeedometer", &bEnable, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse))
		{
			ImGui::PopStyleColor(1);
			ImGui::PopStyleVar(2);

			ImGui::PushFont(pFontSpeedmeter);

			ImGui::PushStyleColor(ImGuiCol_Border, ImColorWhite);
			ImGui::PushStyleColor(ImGuiCol_Text, ImColorWhite);
			ImGui::PushStyleColor(ImGuiCol_Button, ImColorTransparentBlack);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColorTransparentBlack);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorTransparentBlack);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
			ImGui::Button(pSampMulti->SetText("%.1f", pSampMulti->getVehicleSpeed(g_Players->sLocalPlayerID, bSwitchMultipler ? 170.0f : 274.0f)), ImVec2(70.0f, 35.0f));
			ImGui::PopStyleColor(5);
			if (ImGui::IsItemHovered() && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_C)))
			{
				if (bSwitchMultipler == false)
				{
					addMessageToChatWindow("Switched to KM-H");
					bSwitchMultipler = true;
				}
				else
				{
					addMessageToChatWindow("Switched to MP-H");
					bSwitchMultipler = false;
				}
			}

			ImGui::PopStyleVar(2);
			ImGui::PopFont();

			ImGui::End();
		}
	}
}

void CBlackLightMenu::ImInitBlackLight_DamagerMenu(void)
{
	if (!g_SAMP)
		return;

	if (!pGameInterface)
		return;

	if (!__Damager.bShowOnTarget)
		return;

	if (gta_menu_active())
		return;

	if (g_Scoreboard->iIsEnabled || BlackLightFuncs->Menu.bImScoreboad)
		return;

	if (!pSampMulti->IsPlayerStreamed(set.BlackLight.get_damaged_player_for_imgui))
	{
		__Damager.bTarget[set.BlackLight.get_damaged_player_for_imgui] = false;
		return;
	}

	if (pSampMulti->IsModDeveloper(set.BlackLight.get_damaged_player_for_imgui))
		return;

	if (__Damager.bTarget[set.BlackLight.get_damaged_player_for_imgui] == true)
	{
		static DWORD tick;
		DWORD tickcc = GetTickCount();

		if (tickcc - tick >= 7000)
		{
			tick = tickcc;
			__Damager.bTarget[set.BlackLight.get_damaged_player_for_imgui] = false;
		}
	}

	D3DXVECTOR3 vecget, vecset;
	float* fpos = &g_Players->pRemotePlayer[set.BlackLight.get_damaged_player_for_imgui]->pPlayerData->pSAMP_Actor->pGTA_Ped->base.matrix[12];
	vecget.x = fpos[0];
	vecget.y = fpos[1];
	vecget.z = fpos[2];
	pSampMulti->CalcScreenCoors(&vecget, &vecset);

	if (vecset.z <= 1.f)
	{
		//__Damager.bTarget[set.BlackLight.get_damaged_player_for_imgui] = false;
		return;
	}

	ImGui::SetNextWindowPos(ImVec2(vecset.x, vecset.y));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	if (!__Damager.bEnableBackground)
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColorNone);
		ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
	}
	if (ImGui::Begin("##MenuPlayerDamager", &__Damager.bTarget[set.BlackLight.get_damaged_player_for_imgui], ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse))
	{
		if (!__Damager.bEnableBackground)
			ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(1);
		ImGui::PushFont(pFontArial);

		ImGui::Text("Damaged %s\nDamage: %s\nWeapon %s\nDistance %s", __Damager.dmg_info_player.c_str(), __Damager.dmg_info_delivered.c_str(), __Damager.dmg_info_weapon.c_str(), __Damager.dmg_info_distance.c_str());
		ImGui::PopFont();
		ImGui::End();
	}
}

void CBlackLightMenu::ImInitBlackLight_BotInfoIDMenu(bool bEnable)
{
	if (!bEnable)
		return;

	if (!g_SAMP)
		return;

	if (!pGameInterface)
		return;

	if (!g_Players)
		return;

	if (gta_menu_active())
		return;

	if (IS_CHEAT_PANIC_ACTIVE)
		return;

	if (BlackLightFuncs->Menu.ESPMenus.bImBlackLightPlayersInfo)
		return;

	for (int players = 0; players < SAMP_MAX_PLAYERS; players++)
	{
		if (g_Players->iIsListed[players] != 1)
			continue;

		if (g_BotFuncs->uiIsBot[players][0] == 1)
		{
			for (int j = 0; j < 2; j++)
				for (int i = g_BotFuncs->BotSettings.ClientCount - 1; i >= 0; i--)
				{
					actor_info* botsinfo = getGTAPedFromSAMPPlayerID(players);

					if (!botsinfo)
						continue;

					float* fQuat = &botsinfo->base.matrix[12];
					D3DXVECTOR3 getvec, rendervec;
					getvec.x = fQuat[0];
					getvec.y = fQuat[1];
					getvec.z = fQuat[2];
					pSampMulti->CalcScreenCoors(&getvec, &rendervec);

					if (rendervec.z < 1.f)
						continue;

					if (bEnable)
					{
						ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColorTransparentBlack);
						ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
						ImGui::SetNextWindowPos(ImVec2(rendervec.x, rendervec.y));
						ImGui::SetNextWindowSize(ImVec2(150.0f, 60.0f));
						if (ImGui::Begin(pSampMulti->SetText("##MenuBotsInfoID%d", i), &bEnable,
							ImGuiWindowFlags_NoBringToFrontOnFocus |
							ImGuiWindowFlags_NoFocusOnAppearing |
							ImGuiWindowFlags_NoCollapse |
							ImGuiWindowFlags_NoSavedSettings |
							ImGuiWindowFlags_NoResize |
							ImGuiWindowFlags_NoScrollbar |
							ImGuiWindowFlags_NoMove |
							ImGuiWindowFlags_NoTitleBar))
						{
							ImGui::PopStyleVar(1);
							ImGui::PopStyleColor(1);

							ImGui::PushFont(pFontSmall);
							ImGui::Text("%s\nBOT ID: %d", getPlayerName(players), g_BotFuncs->uiIsBot[players][1]);
							ImGui::PushStyleColor(ImGuiCol_Button, ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, pSampMulti->IsMenuActive() ? ImColorDeepOrange : ImColorTransparentBlack);
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColorTransparentBlack);
							ImGui::PushID(i);
							if (ImGui::Button("Bot Say Info##BotInfoSayOfBot", ImVec2(-FLT_MIN, 22.0f)))
							{
								if (!pSampMulti->IsMenuActive())
									return;

								pSampMulti->SayPlayerInfo(players);
							}
							ImGui::Separator();
							ImGui::PopID();
							ImGui::PopStyleColor(3);
							ImGui::PopFont();
							ImGui::End();
						}
					}
				}
		}
	}
}


//class menu definitions
int CBlackLightMenu::GetLastMenu(void)
{
	if (menus != MENU_MAIN && menus <= MENU_CREDITS /*last menu*/)
		return MENU_MAIN;
	else if (menus == MENU_ADDON_SERVERS ||
		menus == MENU_ADDON_PATCHES ||
		menus == MENU_ADDON_RAINBOW ||
		menus == MENU_ADDON_RPCs ||
		menus == MENU_ADDON_SPECTATE_PLAYERS ||
		menus == MENU_ADDON_TARGET_INFO ||
		menus == MENU_ADDON_SPAMMER)
		return MENU_ADDONS;
	else if (menus == MENU_ADDON_TARGET_INFO_LIST_OFF_PLAYERS)
		return MENU_ADDON_TARGET_INFO;
	else if (menus == MENU_ADDON_RPC_INCOMING ||
		menus == MENU_ADDON_RPC_OUTCOMING ||
		menus == MENU_ADDON_PACKET_INCOMING ||
		menus == MENU_ADDON_PACKET_OUTCOMING)
		return MENU_ADDON_RPCs;
	else if (menus == MENU_PLAYER_FAST_ANIMS ||
		menus == MENU_PLAYER_WEAPONS ||
		menus == MENU_PLAYER_TARGET_FUNCS ||
		menus == MENU_PLAYER_MUTED_PLAYERS ||
		menus == MENU_PLAYER_BOTS ||
		menus == MENU_PLAYER_MONEY ||
		menus == MENU_PLAYER_WARP ||
		menus == MENU_PLAYER_TELEPORT_TO_VEHICLE ||
		menus == MENU_PLAYER_WARP_VEHICLE_TO_ME ||
		menus == MENU_PLAYER_DEATHMATCH ||
		menus == MENU_PLAYER_ANIMS)
		return MENU_PLAYER;
	else if (menus == MENU_ESP_TP_TO_OBJECTS ||
		menus == MENU_ESP_TP_TO_PICKUPS)
		return MENU_ESP;
	else if (menus == MENU_PLAYER_COPYCHAT ||
		menus == MENU_PLAYER_FOLLOWTROLL ||
		menus == MENU_PLAYER_FUCKTROLL ||
		menus == MENU_PLAYER_SLAPTROLL ||
		menus == MENU_PLAYER_BULLETS_SEND ||
		menus == MENU_PLAYER_PIZDARVANKA_TARGET)
		return MENU_PLAYER_TARGET_FUNCS;
	else if (menus == MENU_PLAYER_BOT_TROLLS)
		return MENU_PLAYER_BOTS;
	else if (menus == MENU_BOT_TROLL_SET_TARGET)
		return MENU_PLAYER_BOT_TROLLS;
	else if (menus == MENU_HUD_SETTINGS ||
		menus == MENU_SETTINGS_GAMESTATE)
		return MENU_SETTINGS;
	else if (menus == MENU_RADIO_STATIONS)
		return MENU_RADIO;
	else if (menus == MENU_VEHROUTE_SAVED_RECORDINGS)
		return MENU_VEH_ROUTE_RECORDING;
	else if (menus == MENU_VEH_HANDLING ||
		menus == MENU_VEH_ROUTE_RECORDING ||
		menus == MENU_VEH_VEHICLE_TUNING ||
		menus == MENU_VEH_WARP_VEHICLES)
		return MENU_VEHICLE;

	return MENU_MAIN;
}

int CBlackLightMenu::GetCurrentMenu(void)
{
	return menus;
}

void CBlackLightMenu::SetMenu(int MenuID, bool bInnerMenu)
{
	if (!bInnerMenu)
	{
		if (menus != MenuID)
			menus = MenuID;
	}
	else if(bInnerMenu == true)
	{
		if (inner_menus != MenuID)
			inner_menus = MenuID;
	}
}


void CBlackLightMenu::AddMenu(const char* szMenuName, int menuid, ImVec4 ImTextColor)
{
	ImGui::PushFont(pFontArial);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
	ImGui::PushID(menuid);
	if (ImGui::Button(pSampMulti->SetText("##%s",szMenuName), this->MenuItemMatchMenuWidth()))
	{
		if (menus != menuid)
			menus = menuid;
	}
	ImGui::PopID();
	ImGui::PopStyleVar(2);
	ImGui::SameLine(2.0f);
	ImGui::PushStyleColor(ImGuiCol_Text, ImTextColor);
	ImGui::Text(szMenuName);
	ImGui::PopStyleColor(1);
	ImGui::SameLine(ImGui::GetWindowSize().x - (ImGui::CalcTextSize(ICON_FA_ARROW_ALT_CIRCLE_RIGHT).x +10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImColorDeepOrange);
	ImGui::Text(ICON_FA_ARROW_ALT_CIRCLE_RIGHT);
	ImGui::PopStyleColor(1);
	ImGui::PopFont();
}

void CBlackLightMenu::AddButton(bool *bEnable, const char* szButtonName, float fWidthDecrease)
{
	if (*bEnable)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
	}
	else
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
	}

	if (*bEnable)
	{
		ImGui::PushStyleColor(ImGuiCol_Border, BlackLightFuncs->bPulseButtonsBorders ? pSampMulti->ColorOpacityPulse(ImVec4(0.000f, 0.766f, 0.091f, 1.000f), 0.050f, 1.000f, 0.100f, 60) : ImVec4(0.000f, 0.766f, 0.091f, 1.000f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.400f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.600f));
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
	}
	if (ImGui::Button(szButtonName, pMenu->MenuItemMatchMenuWidth(fWidthDecrease)))
	{
		*bEnable = !(*bEnable);
	}
	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar(2);
}

bool CBlackLightMenu::AddStaticButton(const char* szBtnName, float fWidthDecrease)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.000f, 0.766f, 0.091f, 0.000f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.600f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.516f, 0.896f, 0.169f, 0.478f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.139f, 0.139f, 0.139f, 0.493f));
	if (ImGui::Button(szBtnName,pMenu->MenuItemMatchMenuWidth(fWidthDecrease == 0.0f ? 0.0f : fWidthDecrease)))
	{
		 return true;
	}
	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar(2);

	return false;
}

void CBlackLightMenu::AddSlider(const char* szName, int& vSettings, int vMinValue, int vMaxValue, const char* szFormat)
{
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColorSlider);
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColorSliderHovered);
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColorSliderActive);
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImColorSliderGrab);
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImColorSliderGrabActive);
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::SliderInt(szName, &vSettings, vMinValue, vMaxValue, szFormat);
	ImGui::PopStyleColor(5);

}

void CBlackLightMenu::AddSlider(const char* szName, float& vSettings, float vMinValue, float vMaxValue, const char* szFormat)
{
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColorSlider);
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColorSliderHovered);
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColorSliderActive);
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImColorSliderGrab);
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImColorSliderGrabActive);
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::SliderFloat(szName, &vSettings, vMinValue, vMaxValue, szFormat);
	ImGui::PopStyleColor(5);
}

void CBlackLightMenu::AddMenuText(const char* szText, bool bStickAllign, ImVec4 ImColor, bool bSeparators)
{
	if (bSeparators)
	{
		ImGui::PushStyleColor(ImGuiCol_Separator, ImColorBanana);
		ImGui::Separator();
		ImGui::PopStyleColor(1);
	}
	ImGui::PushFont(pFontArial);
	ImGui::Text("");
	ImGui::PushStyleColor(ImGuiCol_Text, ImColor);
	ImGui::SameLine(bStickAllign ? 1.0f : (ImGui::GetWindowSize().x / 2) - ImGui::CalcTextSize(szText).x / 2); ImGui::Text(szText);
	ImGui::PopStyleColor(1);
	ImGui::PopFont();
	if (bSeparators)
	{
		ImGui::PushStyleColor(ImGuiCol_Separator, ImColorBanana);
		ImGui::Separator();
		ImGui::PopStyleColor(1);
	}
}

void CBlackLightMenu::AddIncDecButtons(int& settings, int increase_power, int max_value, int min_value, bool bSameLine, const char* str_id_inc,
	const char* str_id_dec, const char* szIncDescription, const char* szDecDescription)
{
	if (bSameLine) ImGui::SameLine();
	ImGui::PushFont(pFontArial);
	if (ImGui::Button(pSampMulti->SetText(ICON_FA_PLUS_CIRCLE "##%s", str_id_inc), ImVec2(25.0f, 25.0f)))
	{
		if (settings != max_value)
			settings += increase_power;
		else
		{
			settings = max_value;
		}
	}
	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(VK_OEM_PLUS)))
		{
			if (settings != max_value)
				settings += increase_power;
			else
			{
				settings = max_value;
			}
		}

		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColorTransparentBlack);
		ImGui::PushStyleColor(ImGuiCol_Border, ImColorBanana);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.0f);
		ImGui::BeginTooltip();

		ImGui::PushFont(pFontArialV2);
		ImGui::Text(pSampMulti->SetText("%s: %d", szIncDescription, settings));
		ImGui::PopFont();

		ImGui::EndTooltip();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);
	}

	ImGui::SameLine();
	if (ImGui::Button(pSampMulti->SetText(ICON_FA_MINUS_CIRCLE "##%s", str_id_dec), ImVec2(25.0f, 25.0f)))
	{
		if (settings != min_value)
			settings -= increase_power;
		else
		{
			settings = min_value;
		}
	}
	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex( VK_OEM_MINUS)))
		{
			if (settings != min_value)
				settings -= increase_power;
			else
			{
				settings = min_value;
			}
		}

		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColorTransparentBlack);
		ImGui::PushStyleColor(ImGuiCol_Border, ImColorBanana);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.0f);
		ImGui::BeginTooltip();

		ImGui::PushFont(pFontArialV2);
		ImGui::Text(pSampMulti->SetText("%s: %d", szDecDescription, settings));
		ImGui::PopFont();

		ImGui::EndTooltip();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);
	}
	ImGui::PopFont();
}

void CBlackLightMenu::AddIncDecButtons(float& settings, float increase_size, float max_value, float min_value, bool bSameLine, const char* str_id_inc,
	const char* str_id_dec, const char* szIncDescription, const char* szDecDescription)
{
	if (bSameLine) ImGui::SameLine();
	ImGui::PushFont(pFontArial);
	if (ImGui::Button(pSampMulti->SetText(ICON_FA_PLUS_CIRCLE "##%s", str_id_inc), ImVec2(25.0f, 25.0f)))
	{
		if (settings != max_value)
			settings += increase_size;
		else
		{
			settings = max_value;
		}
	}
	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_NumPlus)))
		{
			if (settings != max_value)
				settings += increase_size;
			else
			{
				settings = max_value;
			}
		}

		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColorTransparentBlack);
		ImGui::PushStyleColor(ImGuiCol_Border, ImColorBanana);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.0f);
		ImGui::BeginTooltip();

		ImGui::PushFont(pFontArialV2);
		ImGui::Text(pSampMulti->SetText("%s: %d", szIncDescription, settings));
		ImGui::PopFont();

		ImGui::EndTooltip();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);
	}

	ImGui::SameLine();
	if (ImGui::Button(pSampMulti->SetText(ICON_FA_MINUS_CIRCLE "##%s", str_id_dec), ImVec2(25.0f, 25.0f)))
	{
		if (settings != min_value)
			settings -= increase_size;
		else
		{
			settings = min_value;
		}
	}
	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_NumMinus)))
		{
			if (settings != min_value)
				settings -= increase_size;
			else
			{
				settings = min_value;
			}
		}

		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColorTransparentBlack);
		ImGui::PushStyleColor(ImGuiCol_Border, ImColorBanana);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.0f);
		ImGui::BeginTooltip();

		ImGui::PushFont(pFontArialV2);
		ImGui::Text(pSampMulti->SetText("%s: %d", szDecDescription, settings));
		ImGui::PopFont();

		ImGui::EndTooltip();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);
	}
	ImGui::PopFont();
}


void CBlackLightMenu::AddIncDecButtons(uint16_t& settings, int increase_power, int max_value, int min_value, bool bSameLine, const char* str_id_inc,
	const char* str_id_dec, const char* szIncDescription, const char* szDecDescription)
{
	if (bSameLine) ImGui::SameLine();
	ImGui::PushFont(pFontArial);
	if (ImGui::Button(pSampMulti->SetText(ICON_FA_PLUS_CIRCLE "##%s", str_id_inc), ImVec2(25.0f, 25.0f)))
	{
		if (settings != max_value)
			settings += increase_power;
		else
		{
			settings = max_value;
		}
	}
	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_NumPlus)))
		{
			if (settings != max_value)
				settings += increase_power;
			else
			{
				settings = max_value;
			}
		}

		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColorTransparentBlack);
		ImGui::PushStyleColor(ImGuiCol_Border, ImColorBanana);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.0f);
		ImGui::BeginTooltip();

		ImGui::PushFont(pFontArialV2);
		ImGui::Text(pSampMulti->SetText("%s: %d", szIncDescription, settings));
		ImGui::PopFont();

		ImGui::EndTooltip();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);
	}
	ImGui::SameLine();
	if (ImGui::Button(pSampMulti->SetText(ICON_FA_MINUS_CIRCLE "##%s", str_id_dec), ImVec2(25.0f, 25.0f)))
	{
		if (settings != min_value)
			settings -= increase_power;
		else
		{
			settings = min_value;
		}
	}
	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(VK_OEM_MINUS)))
		{
			if (settings != min_value)
				settings -= increase_power;
			else
			{
				settings = min_value;
			}
		}

		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColorTransparentBlack);
		ImGui::PushStyleColor(ImGuiCol_Border, ImColorBanana);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.0f);
		ImGui::BeginTooltip();

		ImGui::PushFont(pFontArialV2);
		ImGui::Text(pSampMulti->SetText("%s: %d", szDecDescription, settings));
		ImGui::PopFont();

		ImGui::EndTooltip();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);
	}
	ImGui::PopFont();
}

void CBlackLightMenu::AddTargetIncDecButtons(int& settings,bool bSameLine, const char* str_id_inc, const char* str_id_dec)
{
	if(bSameLine) ImGui::SameLine();
	ImGui::PushFont(pFontArial);
	if (ImGui::Button(pSampMulti->SetText(ICON_FA_PLUS_CIRCLE "##%s", str_id_inc), ImVec2(25.0f, 25.0f)))
	{
		settings = pSampMulti->GetNextPlayerAsTarget(settings, +1);
	}
	ImGui::SameLine();
	if (ImGui::Button(pSampMulti->SetText(ICON_FA_MINUS_CIRCLE "##%s", str_id_dec), ImVec2(25.0f, 25.0f)))
	{
		settings = pSampMulti->GetNextPlayerAsTarget(settings, -1);
	}
	ImGui::PopFont();
}

void CBlackLightMenu::AddItemDescription(const char* szText, ImVec4 textColor)
{
	if (ImGui::IsItemHovered())
	{
		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColorTransparentBlack);
		ImGui::PushStyleColor(ImGuiCol_Border, ImColorBanana);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.0f);
		ImGui::BeginTooltip();
		ImGui::PushFont(pFontArialV2);
		ImGui::TextColored(textColor, szText);
		ImGui::PopFont();
		ImGui::EndTooltip();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);
	}
}

void CBlackLightMenu::AddHudItem(bool* bEnabled, const char* szItemName, bool bItemDescription, const char* szDescription)
{
	if (*bEnabled == true)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImColorWhite);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.105f, 0.733f, 0.027f,0.600f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.348f, 0.348f, 0.348f, 0.557f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.348f, 0.348f, 0.348f, 0.650f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImColorWhite);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.882f, 0.035f, 0.066f, 0.600f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.348f, 0.348f, 0.348f, 0.557f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.348f, 0.348f, 0.348f, 0.650f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImColorNone);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
	}
	ImGui::SameLine();
	if (ImGui::Button(szItemName, ImVec2(0.0f, 18.5f)))
	{
		*bEnabled ^= true;
	}
	ImGui::PopStyleColor(5);
	ImGui::PopStyleVar(2);
	if(bItemDescription)
	pMenu->AddItemDescription(szDescription == nullptr ? "Click to toggle\nOn/Off" : szDescription);
}

//buttons
ImVec2 CBlackLightMenu::MenuItemMatchMenuWidth(void)
{
	return ImVec2(-FLT_MIN, 25.0f);
}

ImVec2 CBlackLightMenu::MenuItemMatchMenuWidth(float fWidthCustomSet)
{
	return ImVec2(-FLT_MIN - fWidthCustomSet, 25.0f);
}