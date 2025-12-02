//_=Gigant=_ was here 

struct boolean_function // структура дл€ функций, имеющих только включение/вылючение и гор€чие клавиши
{
	bool enabled;
	keycombo keys;
};


struct stGoC
{
	enum main_funcs
	{
		GOC_MAIN_NONE,
		GOC_MAIN_SUPERGUN,
		GOC_MAIN_LUGGER_SUN,
		GOC_MAIN_TORNADO_MAGNET,
		GOC_MAIN_GRABBER,
		GOC_MAIN_SNAKE_UNVRAVITY,
		GOC_MAIN_SPANWER_CHAOS,
	} main;


	keycombo main_key[GOC_MAIN_SPANWER_CHAOS];



	enum faker_funcs
	{
		GOC_FAKER_NONE,
		GOC_FAKER_SKIN_ATTACKER,
		GOC_FAKER_KICKUP,
		GOC_FAKER_DESTRUCTOR,
		GOC_FAKER_LOADING,
		GOC_FAKER_STICK,
		GOC_FAKER_SLEP,
		GOC_FAKER_KILL,
		GOC_FAKER_REMOVE_KEBAB,
		GOC_FAKER_TELE_UP,
		GOC_FAKER_MAP_TELE,
		GOC_FAKER_SLAP,
		GOC_FAKER_MAGNIT,
		GOC_FAKER_CARDEFORMER,
		GOC_FACKER_PIZDARVANKA,
		GOC_FAKER_CONTROL,
		GOC_FAKER_UP_SHOOTER,
		GOC_FAKER_CAR_SHOOTER,


	} faker;

	keycombo faker_key[GOC_FAKER_CAR_SHOOTER];

	int faker_target;


	bool grabber_vehicle_lock;


	struct stControll
	{
		WORD playerid;
		WORD my_vehicleid;
		bool toggle;
		float save_pos[3];

	} control;


	struct stSettings
	{
		boolean_function key_down;
		boolean_function incar_only; // to ini  and dump ....
		boolean_function onfoot_only;
		boolean_function ignore_my_passagers;
		boolean_function ignore_afk;
		boolean_function ignore_attach; // ^^^^
		boolean_function ignore_locked;
		boolean_function ignore_driver;

		/*enum vehicle_health_maker
		{
			V_HELTH_MAKER_NONE,
			V_HELTH_MAKER_REPAIR,
			V_HELTH_MAKER_BIRN
		}  health_type;*/
	} set;

};
extern struct stGoC* GoC;

void SetPositionByCamera(float* camPos, float* camVect, float fPos, float fSpeed);
vehicle_info* GetVehicleInfoStandart(DWORD using_config_flags, DWORD force_add_flags);
void main_GoC_send_sync(vehicle_info* boomveh, float* pos, float* speed);
bool is_base_GoC_check(actor_info*& actor, vehicle_info*& myveh, vehicle_info*& boomveh);


bool GoC_KEY_CHECK(int key);

void God_of_Cars_hook();
bool Windows_Procedure_GoC_Hook(HWND wnd, UINT umsg, WPARAM wparam, LPARAM lparam);
int GetActorFlag(bool alive, bool incar, bool onfoot, bool ignore_myself, bool not_in_my_car, bool not_afk, bool no_protect, bool no_attach, bool not_friend);

void main_super_gun();