#pragma once

#define BOTS_MAX_BOTS_CONNECT 256
#define BOT_ID_NICK_LOCAL 1
#define BOT_ID_NICK_REALISTIC 2
#define BOT_ID_NICK_BINARY 3

extern class CBotFuncs* g_BotFuncs;

struct stBot
{
	RakClientInterface* pRakClient;
	char NickName[35];
	bool bConnected;
	bool bJoined;
	bool bSpawned;

	bool bIsInVehicle;
	uint16_t last_vehicleID;

	uint16_t playerID;
	unsigned int uiChallenge;

	float fLastOnFootPos[3];
	float fLastOnFootQuat[4];
	short sCurrentAnimationID;
	uint8_t byteSpecialAction;

	DWORD dwSendBulletTick;

	float fHealth;
	float fArmour;

	stBot()
	{
		fHealth = 100.0f;
	}
};

struct stBotSettings
{
	//int ClientCount;
	int ClientCount;
	DWORD UpdateSendRate;
	//int CurrentClientID;
	uint8_t CurrentClientID;
	uint8_t ClientJoined;
	uint8_t NicknameStyle;

	bool bReceiveRPC;

	bool FakePing;
	int incrasePing;
	bool UseFakeBot;
	int ClassID;

	bool bFollow;
	float fDistanceFollow;
	float fDistanceFollowBetweenBots;

	bool bShooter;
	bool bBotTarget[SAMP_MAX_PLAYERS];

	short sTargetID;
	bool bBotAttacker;
	uint8_t uiAttackWeaponID;

	bool bBotElevator;
	bool bBotJetpackElevator;
	bool bBotPick;
	bool bBotSlapper;
	bool bBotFuck;
	bool bBotFollowAnim; //target player
	bool bBotSyncAnim; //our player
	short sBotFollowAnimID;
	bool bUIF_DM_Farmer;

	bool bBotSun;

	//advanced
	bool bShowBotInfoID;
	bool bCopyHealth;
	bool bCopyArmor;
	bool bCopyWeapon;
	bool bCopyChat;
	bool bCopyChat_NOFOLLOW_FromTarget;
	bool bCopyChat_NOFOLLOW_FromAll;
	bool bCopyRPC;

	//temp 
	bool bBotSilent;
	float fMultiplierSilent;
	float fSelfPos[3];
	float fSelfSpeed[3];
	bool bStorm;
	bool bHealthPulsator, bArmorPulsator;
};

class CBotFuncs
{
public:

	CBotFuncs();

	~CBotFuncs();
	uint16_t Client_in_Queue;
	uint16_t N_Client_to_Add_In_Queue;
	uint16_t uiIsBot[SAMP_MAX_PLAYERS][2];

	stBotSettings BotSettings;
	stBot BotClient[BOTS_MAX_BOTS_CONNECT];
	void Add_Bot_Queue();
	bool Add_Bot();
	bool Add_Bot_Nick(char* nick);
	void Delete_Bot();
	void Spawn_Bot(uint8_t ClientID);
	void Random_Spawn_Bot(uint8_t ClientID);
	void Bot_Die();
	void Bot_Process();
	//RPC
	bool Bot_SendRPC(uint8_t ClientID, int iRPCID, BitStream Params, PacketPriority Priority, PacketReliability Reliability, char OrderingChannel, bool ShiftTimestamp);
	void Bot_ReceiveRPC(RakClientInterface** pRakClient);

private:
	//RPC
	void Packet_ConnectionAccepted(Packet* packet, uint8_t ClientID);
	void Packet_Auth_Key(Packet* packet, uint8_t ClientID);
	void Packet_AimSync(Packet* packet, uint8_t ClientID);
	void Packet_BulletSync(Packet* packet, uint8_t ClientID);
	void Packet_PlayerSync(Packet* packet, uint8_t ClientID);

	void Bot_Follow();
	void Bot_Follow_OnFoot(bool bFake);
	void Bot_Follow_Passenger(bool bFake);
	void Bot_Silent();
	void Bot_Shooter();
	void Bot_Attacker();
	void Bot_Elevator();
	void Bot_Slapper();
	void Bot_Fuck();
	void Bot_AnimsSync();
	void Bot_Pick();
	void Bot_Follow_Anim();
	void Bot_Sun();
	void Bot_HealthPulse();
	void Bot_ArmorPulse();

	void Bot_Door_Storm();
	void Bot_Storm();

	void UIF_DM_Farmer();

	void Bot_Send_OnFoot(const float* fPos, const float* fSpeed, bool bFake, uint16_t sSurfingVehicleID = 0, float* fSurfPos = nullptr);
	void Bot_Send_Passenger(bool bFake);
};

void Bot_SetHealth(RPCParameters* params);
void Bot_SetArmour(RPCParameters* params);