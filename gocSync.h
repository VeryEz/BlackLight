#pragma once

//#define ACTOR_ALIVE			0x01
#define ACTOR_INVEHICLE			4
#define ACTOR_ONFOOT			8
#define ACTOR_NOT_ME			16
#define ACTOR_NOT_IN_MY_CAR		32
#define ACTOR_NOT_AFK			64
#define ACTOR_NO_PROTECT		128
#define ACTOR_NOT_ATTACH		256
#define ACTOR_NOT_FRIEND		512
#define ACTOR_NO_SELF_COLOR		1024
#define ACTOR_NO_SELF_SKIN		2048
#define ACTOR_NO_SELF_TEAM		4096


//#define VEHICLE_ALIVE			1
#define VEHICLE_NOT_MY			2
#define VEHICLE_CLOSEST			4
#define VEHICLE_NOT_OCCUPIED	8
#define VEHICLE_NO_BE_PROTECT	16
#define VEHICLE_NEXT			32
#define VEHICLE_FOR_FLY			64
#define VEHICLE_NOT_TRAIN		128
#define VEHICLE_NOT_BOATS		256
//#define VEHICLE_NOT_INCAR_ONLY	512
#define VEHICLE_NOT_FRIEND		1024
#define VEHICLE_NOT_CLOSED		2048
#define VEHICLE_CARS_ONLY		4096
#define VEHICLE_PLANES_ONLY		8192
#define VEHICLE_WITH_DRIVER		16384
#define VEHICLE_NOT_PLANE		32768
#define VEHICLE_NOT_TRAILER		65536 

extern char szMainFunc[32];
extern char szFakerFunc[32];
extern char szSync[32];

struct stCarEnter
{
	keycombo keys;
	DWORD timeout;
	DWORD dw_last_tick;
	bool IsFree(); // если синхра свободна и может использоваться
	void Send(WORD vehicleid, int passid);
};

struct stSync
{

	int timeout_sync; // -> TWbar 0 - 5000
	DWORD dw_base_last_tick;

	enum sending_state
	{
		SENDED_STATE_ONFOOT,
		SENDED_STATE_INCAR,
		SENDED_STATE_PASSAGER,
	} last_sended_state;

	WORD	last_vehicle_id;

	// кроме следующих синхронизаций:
	enum sync_type
	{
		SYNC_TYPE_DEFAULT,
		SYNC_TYPE_INCAR,
		SYNC_TYPE_ECTOPIC,
		SYNC_TYPE_TRAILER,
		SYNC_TYPE_UNOCCUPIED,
		SYNC_TYPE_TIMEOUT_SYNC,
		SYNC_TYPE_PASSANGER,
	} sync;


	enum state_sync
	{
		SYNC_STATE_READY,
		SYNC_STATE_ENTER,
		SYNC_STATE_TIMEOUT,
	} state;


	struct stTimeout_sync// эт моя ебота для улучшенной сихнронизации
	{
		enum syncer_state
		{
			TIME_SYNC_STATE_READY,
			TIME_SYNC_STATE_SELF_OUT,
			TIME_SYNC_STATE_ENTER,
			TIME_SYNC_STATE_INSIDE,
			TIME_SYNC_STATE_OUT,
			TIME_SYNC_STATE_GO_BACK,
			SYNC_STATE_SELF_VEHICLE,
		} state;

		DWORD timeout_self_out;// -> TWbar 0 - 5000
		DWORD timeout_goback;// -> TWbar 0 - 5000
		DWORD timeout_enter;
		DWORD timeout_inside;
		DWORD timeout_out;
		DWORD timeout_self_vehicle;

		DWORD last_tick_self_out;
		DWORD last_tick_enter;
		DWORD last_tick_inside;
		DWORD last_tick_out;
		DWORD last_tick_goback;
		DWORD last_tick_self_vehicle;
	} timeout_settings;



	struct stVehicleSyncValues
	{
		sync_type		force_type;
		bool			force;
		WORD			vehicleid;
		float			pos[3];
		float			rotation[4];
		float			health;
		float			speed[3];
	};

	typedef std::deque<stVehicleSyncValues> deq_veh;
	deq_veh	vehicle_deque;

	stCarEnter car_enter; // timeout -> TWbar   0 - 5000

	bool is_used_timeout_sync();
	bool IsReadyToSync(); // готов к синхронизации следующего транспорта
	bool IgnoreIncarData(); // для HookedRakClient. может ли самп отправлять Incar дату в данный момент
	bool IgnorePassagerData();
	bool IgnoreOnFoot();
	void Hook();
	void SyncVehicle(WORD vehicleid, float* pos, float health, float* rotation, float* speed);
	void ForceSyncVehiclePassanger(WORD vehicleid, float* pos, float health);
	void ForceSyncVehicleUnoccupied(WORD vehicleid, float* pos, float health, float* rotation, float* speed);
	void ForceSyncVehicleIncar(WORD vehicleid, float* pos, float health, float* rotation, float* speed);
};
extern 	stSync* syncronisations_settings; //goc by masterzero