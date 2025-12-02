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
#define INI_STRLEN_MAX		64	/* including null */
#define INI_PATCHES_MAX		96
#define INI_SAMPPATCHES_MAX 128 /*64*/
#define INI_NETPATCHES_MAX	1024
#define INI_SERVERS_MAX		64
#define INI_CHATMSGS_MAX	64
#define INI_GUI_MAX			8
#define INI_KEYCOMBO_MAX	8

struct settings_coord
{
	float	pos[3];
	int		interior_id;
	int		iMenuID;
};

struct fav_server
{
	const char	*nickname;
	const char	*server_name;
	const char	*ip;
	uint16_t	port;
	char		*password;
};

struct chat_msg
{
	int			key;
	const char	*msg;
	int			spam;
};

struct gui
{
	const char	*element;
	int			alpha;
	int			red;
	int			green;
	int			blue;
};

struct keycombo
{
	unsigned int count;
	uint8_t key[INI_KEYCOMBO_MAX];
};

// set structure
// DO NOT use bool types in here, it screws up the alignment even with pragma pack directive
struct settings
{
	int						run_mode;
	int						wine_compatibility;
	int						basic_mode;
	int						fps_limit;
	float                    rapid_speed;

	//BlackLight
	struct stBlackLight
	{
		struct stKeys
		{
			struct stMenu
			{
				keycombo key_main_menu;
				keycombo key_new_hud;

			} MenuKeys;
		} KeyPress;

		struct stTargeting
		{
			int TargetPlayer_Spec; //used for MENU_ADDON_SPECTATE_PLAYERS
			int TargetPlayer_MenuInfo; //for in menu info
			int TargetPlayer_CopyChat;
			int TargetPlayer_FuckTroll;
			int TargetPlayer_SlapTroll;
			int TargetPlayer_SendBullets;
			int TargetPlayer_Pizdarvanka;
			int TargetPlayer_ForBots;
			int TargetPlayer_Follow;
			int PlayerStick;

		} Target;

		struct stRainbow
		{
			int rainbow_radar_speed;
			int rainbow_hud_speed;
			int rainbow_crosshair_speed;
			int rainbow_carlights_speed;
			int rainbow_car_speed;
			int rainbow_sky_speed;
			int rainbow_KillList_speed;
			int rainbow_chat_speed;
			int rainbow_nicknames_speed;

		} Rainbow;

		struct stSpecialESP
		{
			float fDistancePlayersTags,
			fDistancePlayersInfo;
		} SpecialESP;

		int menu_hud_set, menu_admins_and_friends;
		int custom_time, custom_weather; //custom time -1 - 24 (-1 allow server time set)
		int kill_counter;
		int chams_style;
		float slowmo_detection;
		keycombo key_slowmo_on_key;
		keycombo key_world_collision;
		keycombo key_player_ghost;
		keycombo key_vehicle_hologram;
		keycombo key_moveless_players;
		float fCamOverview;
		float fSprintEng;
		float fFastRotation;
		float fast_rotation_speed;
		float fast_swim_speed;
		float fast_walk_speed;
		float fast_run_speed;
		float fast_sprint_speed;
		float fast_run_armed_speed;
		float fast_reload_speed;
		float fast_deagle;


		float wheels_info_dist;
		float cars_doors_status_distance_show;
		float vehicle_2d_info_distance_draw;
		float players_2DInfo_distance;
		int player_joiner_lines_size;
		float teleport_tag_dist;
		int auto_reconnect_delay;
		int component_flood_delay;
		int wheel_status_pulse_delay;
		int weapons_pulse_speed;
		int blink_color_deley;
		int blinkcolor1, blinkcolor2;
		int pulse_disk_deley;
		int pizdarvanka_player_speed;
		int pizdarvanka_vehicle_speed;
		int player_fly_style;
		int fake_kill_reason;
		int arab_drift_on_wheels;
		int get_damaged_player_for_imgui;

		//float fdevx, fdevy, fdevz, fdevw;
		//int fdev_axis;

	} BlackLight;

	keycombo key_superstunt;

	keycombo				key_hp_cheat;
	float					hp_minimum;
	float					hp_regen;
	float					hp_regen_onfoot;
	int						hp_actor_explosion_inv;
	int						hp_actor_fall_inv;
	int						hp_actor_fire_inv;
	int						hp_keep_vehicleHPsane;
	int						hp_disable_inv_sp_enemies;
	int						hp_tire_support;

	keycombo				key_anti_freeze;

	keycombo				key_autoaim_toggle;
	int						use_gta_autoaim;

	keycombo				key_map;
	keycombo				key_map_show_vehicles;

	keycombo				key_air_brake_mod;
	keycombo				key_air_brake_foot_mod;
	keycombo				key_air_brake_mod2;
	keycombo				key_air_brake_forward;
	keycombo				key_air_brake_backward;
	keycombo				key_air_brake_left;
	keycombo				key_air_brake_right;
	keycombo				key_air_brake_up;
	keycombo				key_air_brake_down;
	keycombo				key_air_brake_rot_yaw1;
	keycombo				key_air_brake_rot_yaw2;
	keycombo				key_air_brake_rot_roll1;
	keycombo				key_air_brake_rot_roll2;
	keycombo				key_air_brake_rot_pitch1;
	keycombo				key_air_brake_rot_pitch2;
	int						air_brake_toggle;
	int						air_brake_behaviour;
	float					air_brake_speed; //player speed
	float                air_brake_vehicle_speed;
	float					air_brake_rot_speed;
	float					air_brake_accel_time;

	keycombo				key_warp_mod;
	float					warp_speed;
	int						warp_use_speed;

	keycombo				key_blinking_car_lights;
	int						enable_car_lights_at_day_time;

	keycombo				key_keep_trailer;
	keycombo				key_disable_Wall_Collisions;
	int						wall_collisions_disableObjects;
	int						wall_collisions_disableRender;

	keycombo				key_repair_car;

	keycombo				key_nitro;

#ifdef __CHEAT_VEHRECORDING_H__
	int						recording_activated;
	float					recording_maxDistToEntryPoint;
	float					recording_play_customSpeed;
	int						key_recording_record;
	int						key_recording_continueAfterFinish;
	int						key_recording_play;
	int						key_recording_customSpeed;
	int						key_recording_rev;
	int						key_recording_rev_customSpeed;
#endif

	//int					teleport_slow;
	//int					key_slowTeleport_stop;
	keycombo				key_teleport_hist;
	keycombo				key_teleport[TELEPORT_MAX];
	keycombo				key_teleport_set[TELEPORT_MAX];
	struct settings_coord	teleport[TELEPORT_MAX];

	char					static_teleport_name[STATIC_TELEPORT_MAX][INI_STRLEN_MAX];
	struct settings_coord	static_teleport[STATIC_TELEPORT_MAX];

	keycombo				key_unflip;
	keycombo				key_quick_turn_180;
	keycombo				key_quick_turn_left;
	keycombo				key_quick_turn_right;

	keycombo				key_stick;
	keycombo				key_stick_prev;
	keycombo				key_stick_next;
	keycombo				key_stick_nearest;
	keycombo				key_stick_forward;
	keycombo				key_stick_backward;
	keycombo				key_stick_left;
	keycombo				key_stick_right;
	keycombo				key_stick_up;
	keycombo				key_stick_down;
	keycombo				key_stick_in;
	keycombo				key_stick_out;
	float					stick_min_height;
	float					stick_vect[4];	/* must be 4d! */
	float					stick_vect_dist;
	float					stick_accel_time;

	keycombo				key_checkpoint_1;
	keycombo				key_checkpoint_2;
	float					checkpoint_min_height;

	keycombo				key_nitro_mod;
	float					nitro_high;
	float					nitro_low;
	float					nitro_accel_time;
	float					nitro_decel_time;

	keycombo				key_brake_mod;
	float					brake_mult;

	keycombo				key_protection;
	float					protection_spin_cap;
	float					protection_speed_cap;
	float					protection_min_height;

	keycombo				key_self_destruct;
	keycombo				key_engine;
	keycombo				key_vehicle_jumper;
	keycombo				key_vehicle_occupied_jumper;

	keycombo				key_weapon;

	int						ammo;
	int						ammo_clip;

	struct weapon_entry		*weapon[13];

	keycombo				key_money;
	keycombo				key_menu;
	int						key_menu_up;
	int						key_menu_right;
	int						key_menu_down;
	int						key_menu_left;
	int						key_menu_select;
	int						key_menu_dec;
	int						key_menu_inc;
	keycombo				key_menu_mousecontrol;

	int						render_text_shadows;
	struct gui				guiset[INI_GUI_MAX];

	int						force_hour;
	int						force_weather;


	float					speedometer_multiplier;
	char					speedometer_speedo_png_filename[32];
	char					speedometer_needle_png_filename[32];

	int						chams_wireframe;

	int						d3dtext_labels;
	float					d3dtext_labels_maxDist;

	int						nametags_show;
	int						disable_line_of_sight;
	float					line_of_sight_dist;
	int						markers_show;

	int						samp_custom_sendrates_enable;
	int						onfoot_sendrate;
	int						incar_sendrate;
	int						aim_sendrate;
	int						headsync_sendrate;
	int raknet_lines_limit;
	keycombo				key_clickwarp_enable;
	keycombo				key_clickwarp_click;

	keycombo				key_fast_exit;

	keycombo key_front_flip;
	keycombo key_backflip;
	keycombo key_playerhop;
	float player_hop_force;

	// ignored because of the function being removed
	int						window_mode;
	int						window_mode_titlebar;

	int						flickering_problem;
	int                        kill_counter;
	int                        auto_reconnect;
	int                        kill_list_chat;
	int new_scoreboard;
	int no_fall;
	int no_stun;
	int buttons_opacity_pulse;
	int loop_mp3_song;
	int player_joiner;
	int uif_islands_keys;
	int custom_sprint_energy;
	int muted_player_notify_chat;
	int slowmotion_enable;
	int slowmotion_via_key_enable;
	int camera_overview_enable;
	int disable_car_wheels;
	int mouse_drive_enable;
	int disable_car_frame;

	float mp3_volume, radio_volume;

	int						walk_under_water;
	int						vehicles_unlock;
	int						vehicles_warp_invert;
	keycombo				key_vehicles_freeze;

	keycombo				key_respawn;

	keycombo				key_render_player_tags;
	float					player_tags_dist;
	keycombo				key_render_vehicle_tags;
	float					vehicle_tags_dist;
	keycombo				key_vehicle_hop;
	float					vehicle_hop_speed;

	keycombo key_car_flip;
	keycombo				key_spiderwheels;
	/*
	int						key_spiderfeet;
	int						key_ninjaflipfront;
	int						key_ninjaflipback;
	*/

	keycombo				key_fly_player;
	int						key_fly_player_accelerate;
	int						key_fly_player_decelerate;
	int						key_fly_player_strafeLeft;
	int						key_fly_player_strafeRight;
	int						key_fly_player_strafeUp;
	float					fly_player_speed;
	float					fly_player_accel_multiplier;
	float					fly_player_decel_multiplier;

	float					handling_multiplier;

	int						use_current_name;
	struct fav_server		server[INI_SERVERS_MAX];

	keycombo				chat_secondary_key;
	struct chat_msg			chat[INI_CHATMSGS_MAX];

	struct patch_set		patch[INI_PATCHES_MAX];
	struct patch_set		sampPatch[INI_SAMPPATCHES_MAX];

	keycombo				key_panic;

	int						custom_runanimation_id;

	keycombo				key_brkd_toggle;
	keycombo				key_brkd_forward;
	keycombo				key_brkd_left;
	keycombo				key_brkd_backward;
	keycombo				key_brkd_right;
	keycombo				key_brkd_rightward;
	keycombo				key_brkd_leftward;
	float					brkdance_velocity;
	float                game_speedc;
	keycombo				key_fly_vehicle;
	keycombo				key_fly_vehicle_modeChange;

	keycombo				key_rejoin;
	int						rejoin_delay;
	float                        surfspeed; //BE_Surfer
	float flysurfspeed;
	int health_type; //goc
	int speedkick_speed; //goc
	float upPower;
	int rampower;
	int fPlayerID;
	// new ESP ish
	float					pickup_tags_dist;
	float					object_tags_dist;
	int						enable_clouds;

	int						enable_extra_godmode;
	int						key_map_teleport;
	int						map_must_be_open;
	int						map_draw_lines;
	
	keycombo				key_quickwarp;
	keycombo				key_surf;
	keycombo				key_freezerot;

	stNetPatch				netPatch[INI_NETPATCHES_MAX];
	stNetPatch				*netPatchAssoc[RAKNET_MAX_PACKET][4];
};


///////////////////////////////////////////////
extern struct settings	set;
extern int	iGTAPatchesCount, iSAMPPatchesCount, iNetPatchesCount, iServersCount;

void					ini_load ( void );
void					ini_reload ( void );
void					ini_free ( void );
void					ini_load_setSettings( void );
