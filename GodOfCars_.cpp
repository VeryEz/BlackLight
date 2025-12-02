#include "main.h"

//yes i copied this from blue eclipse since that mod was abandoned by original developers so why not use this good script they made and I modified to work
//with imgui so yes enjoy 
//_=Gigant=_ was here 

struct stGoC GoCInstance;         // Static instance (allocated at compile-time)
struct stGoC* GoC = &GoCInstance; // GoC points to GoCInstance

char szMainFunc[32];
char szFakerFunc[32];
char szSync[32];

vehicle_info* GetVehicleInfoForceFlags(DWORD flags)
{
	traceLastFunc("GetVehicleInfo()");

	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);

	if (!actor)
		return NULL;


	vehicle_info* ret = NULL;

	bool nullid = (flags & VEHICLE_NEXT) != 0;


	static WORD NextVehID;
search_againt:

	for (int id = (nullid) ? NextVehID : NULL; id < SAMP_MAX_VEHICLES; id++)
	{
		vehicle_info* vehicle = getGTAVehicleFromSAMPVehicleID(id);

		if (vehicle == NULL)
			continue;

		if ((flags & VEHICLE_ALIVE) && (vehicle->hitpoints < 100.0f))
			continue;

		if ((flags & VEHICLE_NOT_MY) && (vehicle_info_get(VEHICLE_SELF, NULL) == vehicle))
			continue;

		if ((flags & VEHICLE_NOT_OCCUPIED) && vehicle->passengers && *vehicle->passengers)
			continue;


		/*if (flags & VEHICLE_NO_BE_PROTECT)
		{
			actor_info* driver = vehicle->passengers[0];
			if (driver && driver->state == ACTOR_STATE_DRIVING)
			{
				int id = getSAMPPlayerIDFromGTAPed(driver);
				if (id != INVALID && bemod->bemod_protection.bemod_player[id].have_bemod)
					continue;
			}
		}*/


		if (flags & VEHICLE_FOR_FLY)
		{
			const vehicle_entry* model = gta_vehicle_get_by_id(vehicle->base.model_alt_id);
			if (!model)
				continue;

			switch (model->class_id)
			{
			case VEHICLE_CLASS_HELI:
			case VEHICLE_CLASS_AIRPLANE:
			case VEHICLE_CLASS_BOAT:
			case VEHICLE_CLASS_MINI:
			case VEHICLE_CLASS_TRAILER:
				continue;
			default:
				break;
			}
		}


		if (flags & VEHICLE_NOT_BOATS)
		{
			const vehicle_entry* model = gta_vehicle_get_by_id(vehicle->base.model_alt_id);
			if (!model)
				continue;

			if (model->class_id == VEHICLE_CLASS_BOAT)
				continue;
		}


		if (flags & VEHICLE_NOT_TRAILER)//Флаг не будет брать трейлер как машину для синхры, ибо с трейлером будет кидать в инвиз
		{
			const vehicle_entry* model = gta_vehicle_get_by_id(vehicle->base.model_alt_id);
			if (!model)
				continue;

			if (model->class_id == VEHICLE_CLASS_TRAILER)
				continue;
		}


		if (flags & VEHICLE_NOT_PLANE)
		{
			const vehicle_entry* model = gta_vehicle_get_by_id(vehicle->base.model_alt_id);
			if (!model)
				continue;

			if (model->class_id == VEHICLE_CLASS_AIRPLANE)
				continue;
		}


		if (flags & VEHICLE_NOT_TRAIN)
		{
			switch (vehicle->base.model_alt_id)
			{
			case 449:
			case 537:
			case 538:
			case 569:
			case 570:
			case 590:
				continue;
			}
		}

		if (flags & VEHICLE_NOT_FRIEND)
		{
			actor_info* driver = vehicle->passengers[0];
			if (driver && driver->state == ACTOR_STATE_DRIVING)
			{
				int id = getSAMPPlayerIDFromGTAPed(driver);
				if (id != INVALID && BlackLightFuncs->bIsFriend[id])
					continue;
			}
		}


		if ((flags & VEHICLE_NOT_CLOSED) && GoC->set.ignore_locked.enabled)
		{
			if (g_Vehicles->pSAMP_Vehicle[id]->iIsLocked)
				continue;
		}

		if (flags & VEHICLE_CARS_ONLY)
		{
			eClientVehicleType vehicleType = GetVehicleType(vehicle->base.model_alt_id);
			if (vehicleType != CLIENTVEHICLE_CAR)
				continue;
		}



		if (flags & VEHICLE_PLANES_ONLY)
		{
			if (vehicle->base.model_alt_id != 513)
				continue;
		}


		if (flags & VEHICLE_CLOSEST)
		{
			if (ret == NULL)
			{
				ret = vehicle;
				continue;
			}
			else if (vect3_dist(&actor->base.matrix[12], &vehicle->base.matrix[12]) < vect3_dist(&actor->base.matrix[12], &ret->base.matrix[12]))
				ret = vehicle;
		}
		else
		{
			if (flags & VEHICLE_NEXT)
				NextVehID = id + 1;


			traceLastFunc("GetVehicleInfo() found");
			return vehicle;
		}
	}


	if (nullid)
	{
		nullid = false;
		goto search_againt;
	}


	traceLastFunc("GetVehicleInfo() not found");
	return ret;
}

vehicle_info* GetVehicleInfoStandart(DWORD using_config_flags, DWORD force_add_flags)
{

	DWORD result_flags = force_add_flags;

	if (using_config_flags & VEHICLE_NOT_OCCUPIED)
	{
		switch (syncronisations_settings->sync)
		{
		case stSync::sync_type::SYNC_TYPE_UNOCCUPIED:
		case stSync::sync_type::SYNC_TYPE_DEFAULT:
		case stSync::sync_type::SYNC_TYPE_TRAILER:
		case stSync::sync_type::SYNC_TYPE_ECTOPIC:
			result_flags |= VEHICLE_NOT_OCCUPIED;
			break;

		default:
			if (GoC->set.ignore_driver.enabled)
				result_flags |= VEHICLE_NOT_OCCUPIED;
			break;
		}
	}

//if (using_config_flags & VEHICLE_NOT_FRIEND)
		//if (bemod->GoC.set.ignore_friends.enabled)
		//	result_flags |= VEHICLE_NOT_FRIEND;

	if (using_config_flags & VEHICLE_NOT_CLOSED)
		if (GoC->set.ignore_locked.enabled)
			result_flags |= VEHICLE_NOT_CLOSED;

	return GetVehicleInfoForceFlags(result_flags);
}


int GetClosestPlayerid(actor_info* player, int flugs)
{
	//traceLastFunc("int GetClosestPlayerid(actor_info *player, int flugs)");
	int minID = INVALID;
	double minDist = 0.0f;

	/*
	#define ACTOR_POSCLOS			128
	#define ACTOR_ANGCLOS			256
	*/

	for (int id = NULL; id < SAMP_MAX_PLAYERS; id++)
	{
		actor_info* actor = getGTAPedFromSAMPPlayerID(id);
		if (actor == NULL)
			continue;

		if ((flugs & ACTOR_ALIVE) && actor->hitpoints <= 0.0f)
			continue;


		if ((flugs & ACTOR_NOT_ME) && actor == player)
			continue;

		if ((flugs & ACTOR_INVEHICLE) &&
			//actor->state != ACTOR_STATE_DRIVING
			g_Players->pRemotePlayer[id]->pPlayerData->bytePlayerState != PLAYER_STATE_DRIVER
			&& g_Players->pRemotePlayer[id]->pPlayerData->bytePlayerState != PLAYER_STATE_PASSENGER)
			continue;

		if ((flugs & ACTOR_ONFOOT) &&
			g_Players->pRemotePlayer[id]->pPlayerData->bytePlayerState != PLAYER_STATE_ONFOOT)
			continue;


		if ((flugs & ACTOR_NOT_AFK) && g_Players->pRemotePlayer[id]->pPlayerData->iAFKState == 2)
			continue;


		if (flugs & ACTOR_NOT_ATTACH)
		{
			vehicle_info* myveh = vehicle_info_get(VEHICLE_SELF, NULL);
			//g_Players->pRemotePlayer[id]->pPlayerData->bytePlayerState == PLAYER_STATE_ONFOOT // control fix
			if (myveh && g_Players->pRemotePlayer[id]->pPlayerData->bytePlayerState == PLAYER_STATE_ONFOOT &&
				getSAMPVehicleIDFromGTAVehicle(myveh) == g_Players->pRemotePlayer[id]->pPlayerData->onFootData.sSurfingVehicleID)
				continue;
		}

		if ((flugs & ACTOR_NOT_FRIEND) && !BlackLightFuncs->bIsFriend[id])
		{
			int id = getSAMPPlayerIDFromGTAPed(actor);
			if ((id != INVALID))
				continue;
		}


		if (flugs & ACTOR_NOT_IN_MY_CAR)
		{
			vehicle_info* myveh = vehicle_info_get(VEHICLE_SELF, NULL);
			if (player->state == ACTOR_STATE_DRIVING &&
				(g_Players->pRemotePlayer[id]->pPlayerData->bytePlayerState == PLAYER_STATE_DRIVER ||
					g_Players->pRemotePlayer[id]->pPlayerData->bytePlayerState == PLAYER_STATE_PASSENGER) && myveh && myveh == actor->vehicle)
				continue;
		}

		//add mod dev
		/*
		if ((flugs & ACTOR_NO_PROTECT) && !BlackLightFuncs->bemod_protection.ignore_bemod_players)
		{

			if (bemod->bemod_protection.bemod_player[id].have_bemod)
				continue;


			if (g_Players->pRemotePlayer[id]->pPlayerData->bytePlayerState == PLAYER_STATE_DRIVER
				|| g_Players->pRemotePlayer[id]->pPlayerData->bytePlayerState == PLAYER_STATE_PASSENGER)
			{
				vehicle_info* vehicle = actor->vehicle;

				if (vehicle)
				{
					bool car_with_goc = false;

					for (int i = 0; i < 9; i++)
					{
						actor_info* passager = vehicle->passengers[i];
						if (!passager)
							continue;
						int driver_actor_id = getSAMPPlayerIDFromGTAPed(passager);

						if (bemod->bemod_protection.bemod_player[driver_actor_id].have_bemod)
						{
							car_with_goc = true;
							break;
						}
					}
					if (car_with_goc)
						continue;
				}
			}

		}
		*/
		double dist = 0;

		dist = vect3_dist(&actor->base.matrix[4 * 3], &player->base.matrix[4 * 3]);

		//if (!dist) return INVALID;

		if (minID == INVALID)
		{
			minID = id;
			minDist = dist;
		}
		else
		{
			if (dist < minDist)
			{
				minID = id;
				minDist = dist;
			}
		}

	}
	return minID;
}


bool GoC_KEY_CHECK(int key)
{
	return (GoC->set.key_down.enabled) ?
		KEY_DOWN(key) :
		KEY_PRESSED(key);
}

void SyncTargetActor(actor_info* deadman, float* pos, float* Speed)
{
	if (!syncronisations_settings->IsReadyToSync())
		return;

	vehicle_info* sync_veh = NULL;
	vehicle_info* my_vehicle = vehicle_info_get(VEHICLE_SELF, NULL);

	if (deadman->state == ACTOR_STATE_DRIVING)
		sync_veh = deadman->vehicle;
	else
	{
		if (g_Players->pRemotePlayer[GoC->faker_target]->pPlayerData->onFootData.sSurfingVehicleID == 65535)
		{

			if (my_vehicle)
				sync_veh = my_vehicle;
			else
				sync_veh = GetVehicleInfoStandart(
					VEHICLE_NO_BE_PROTECT |
					VEHICLE_NOT_OCCUPIED |
					VEHICLE_NOT_FRIEND |
					VEHICLE_NOT_CLOSED,
					VEHICLE_NEXT |
					VEHICLE_NOT_MY |
					VEHICLE_NOT_TRAIN);


			vect3_copy(&deadman->base.matrix[4 * 3], pos);
			for (int i = 0; i < 2; i++)
				pos[i] += deadman->speed[i] * 10.0f;
			pos[2] -= 2.0f;


			vect3_zero(Speed);

		}
		else
		{
			sync_veh = getGTAVehicleFromSAMPVehicleID(g_Players->pRemotePlayer[GoC->faker_target]->pPlayerData->onFootData.sSurfingVehicleID);

			if ((sync_veh == my_vehicle) && my_vehicle)
			{
				float mypos[3];

				vect3_copy(&my_vehicle->base.matrix[12], mypos);
				mypos[2] += 5;

				GTAfunc_RemoveActorFromCarAndPutAt(mypos);
			}
		}
	}

	if (sync_veh)
		syncronisations_settings->SyncVehicle(getSAMPVehicleIDFromGTAVehicle(sync_veh),
			pos, sync_veh->hitpoints, NULL, Speed);
}

int GetActorFlag(bool alive, bool incar, bool onfoot, bool ignore_myself, bool not_in_my_car, bool not_afk, bool no_protect, bool no_attach, bool not_friend)
{
	int flag = 0;


	if (alive)
		flag |= ACTOR_ALIVE;

	if (incar)
		flag |= ACTOR_INVEHICLE;

	if (onfoot)
		flag |= ACTOR_ONFOOT;

	if (ignore_myself)
		flag |= ACTOR_NOT_ME;

	if (not_in_my_car)
		flag |= ACTOR_NOT_IN_MY_CAR;

	if (not_afk)
		flag |= ACTOR_NOT_AFK;

	if (no_protect)
		flag |= ACTOR_NO_PROTECT;

	if (no_attach)
		flag |= ACTOR_NOT_ATTACH;

	if (not_friend)
		flag |= ACTOR_NOT_FRIEND;

	return flag;
}

bool is_base_GoC_check(actor_info*& actor, vehicle_info*& myveh, vehicle_info*& boomveh)
{

	actor = actor_info_get(ACTOR_SELF, NULL);
	if (!actor)
		return false;

	if (g_Players->pLocalPlayer->iIsInSpawnScreen)
		return false;

	myveh = vehicle_info_get(VEHICLE_SELF, NULL);

	if (actor->state == ACTOR_STATE_DRIVING && !myveh)
		return false;


	if (!GoC_KEY_CHECK(KEY_LKM) && !GoC_KEY_CHECK(KEY_RKM))
		return false;


	if (!syncronisations_settings->IsReadyToSync())
		return false;


	boomveh = GetVehicleInfoStandart(
		VEHICLE_NO_BE_PROTECT |
		VEHICLE_NOT_OCCUPIED |
		VEHICLE_NOT_FRIEND |
		VEHICLE_NOT_CLOSED,
		VEHICLE_NEXT |
		VEHICLE_NOT_MY |
		VEHICLE_NOT_TRAIN);


	if (!boomveh)
		return false;

	return true;
}

void main_GoC_send_sync(vehicle_info* boomveh, float* pos, float* speed)
{
	if (set.health_type == 2)
		GTAfunc_RepairVehicle(boomveh);

	if (set.health_type == 3)
		boomveh->hitpoints = 0.0f;

	vect3_copy(pos, &boomveh->base.matrix[12]);
	vect3_copy(speed, boomveh->speed);
	vect3_zero(boomveh->spin);

	syncronisations_settings->SyncVehicle(getSAMPVehicleIDFromGTAVehicle(boomveh), pos, boomveh->hitpoints, NULL, speed);
}

static void GetPosFromPlaceID(float* res, int id)
{
	const float MaxLen = 3000.0f;
	const int Tochnost = 8;
	res[0] = -MaxLen;
	res[1] = -MaxLen;
	res[2] = 1000.0f;

	int row = id / Tochnost;
	int col = id % Tochnost;


	res[0] += col * (MaxLen / (float)Tochnost);
	res[1] += row * (MaxLen / (float)Tochnost);
}
static void GetPosFromPosWithOffset(float* result, float input[3], float offset[3], float ang)
{
	result[2] = input[2] + offset[2]; // z

	result[0] = input[0] - sin(ang) * offset[1];
	result[1] = input[1] + cos(ang) * offset[1];

	result[0] = result[0] + cos(ang) * offset[0];
	result[1] = result[1] + sin(ang) * offset[0];
}

static void GetMapMarkerPos(float* pos)
{
	if (!pGameInterface || !(*(int*)0xBA6774))
		return;

	for (int i = 0; i < (0xAF * 0x28); i += 0x28)
	{
		if (*(short*)(0xBA873D + i) == 4611)
		{
			float* ppos = (float*)(0xBA86F8 + 0x28 + i);
			gta_interior_id_set(/*gta_interior_id_get()*/NULL);
			pos[0] = *ppos;
			pos[1] = *(ppos + 1);
			pos[2] = pGameInterface->GetWorld()->FindGroundZForPosition(pos[0], pos[1]);
		}
	}
}

void SetPositionByCamera(float* camPos, float* camVect, float fPos, float fSpeed)
{
	CMatrix matCamera;
	pGame->GetCamera()->GetMatrix(&matCamera);
	matCamera.vFront.Normalize();

	camPos[0] = matCamera.vPos.fX;
	camPos[1] = matCamera.vPos.fY;
	camPos[2] = matCamera.vPos.fZ;

	camVect[0] = matCamera.vFront.fX;
	camVect[1] = matCamera.vFront.fY;
	camVect[2] = matCamera.vFront.fZ;

	for (int i = 0; i < 3; i++)
		camPos[i] += camVect[i] * fPos;

	for (int i = 0; i < 3; i++)
		camVect[i] *= fSpeed;
}

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

void main_super_gun()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
	vehicle_info* myveh = vehicle_info_get(VEHICLE_SELF, NULL);
	vehicle_info* boomveh;

	if (actor->state == ACTOR_STATE_DRIVING)myveh->base.interior_id = (KEY_DOWN(KEY_RKM) || GoC_KEY_CHECK(KEY_LKM)) ? 1 : 0;

	if (!is_base_GoC_check(actor, myveh, boomveh))
		return;

	if (!GoC_KEY_CHECK(KEY_LKM))
		return;

	float campos[3],
		camVect[3];

	SetPositionByCamera(
		campos,
		camVect,
		6.5f,
		7.5f);

	for (int i = 0; i <= 6; i++)
		boomveh->base.matrix[i] = actor->base.matrix[i];

	main_GoC_send_sync(boomveh, campos, camVect);
}

void main_lugger_sun()
{
	actor_info* actor;
	vehicle_info* myveh;
	vehicle_info* boomveh;
	float pos[3] = {};
	float speed[3] = {};

	if (!is_base_GoC_check(actor, myveh, boomveh))
		return;

	static float Angle = 0;
	float offset[3] = {};

	if (GoC_KEY_CHECK(KEY_LKM))
	{
		offset[1] = 10.0f;
		offset[2] = 10.0f;
		GetPosFromPosWithOffset(pos, &actor->base.matrix[12], offset, actor->fCurrentRotation);
	}
	else
	{
		offset[2] = 1.0f;
		offset[1] = 10.0f;
		GetPosFromPosWithOffset(pos, &actor->base.matrix[12], offset, Angle);
		SetVehicleZAngle(boomveh, Angle, false);

		const float AbsSpeed = 10.0f;

		speed[0] = AbsSpeed * cos(Angle);
		speed[1] = AbsSpeed * sin(Angle);

		Angle += DEGTORAD(115);
	}
	main_GoC_send_sync(boomveh, pos, speed);
}


void main_tornado_magnet()
{
	actor_info* actor;
	vehicle_info* myveh;
	vehicle_info* boomveh;
	float pos[3] = {};
	float speed[3] = {};


	if (!is_base_GoC_check(actor, myveh, boomveh))
		return;

	static float Angle = 0;
	float offset[3] = {};

	if (GoC_KEY_CHECK(KEY_LKM))
	{
		//offset[2] = 10.0f;
		offset[1] = 20.0f;

		GetPosFromPosWithOffset(pos, &actor->base.matrix[12], offset, Angle);
		SetVehicleZAngle(boomveh, Angle, false);

		const float AbsSpeed = 1.0f;
		speed[2] = AbsSpeed;
		speed[0] = AbsSpeed * cos(Angle);
		speed[1] = AbsSpeed * sin(Angle);

		Angle += DEGTORAD(20);

	}
	else
	{
		GetPosFromPosWithOffset(pos, &boomveh->base.matrix[12], offset, Angle);

		float force = 2.0f;

		for (int i = 0; i < 3; i++)
			speed[i] = (pos[i] < actor->base.matrix[12 + i]) ? force : -force;
	}

	main_GoC_send_sync(boomveh, pos, speed);
}

void main_snake_ungravity()
{
	actor_info* actor;
	vehicle_info* myveh;
	vehicle_info* boomveh;
	float pos[3] = {};
	float speed[3] = {};

	if (!is_base_GoC_check(actor, myveh, boomveh))
		return;

	static float Angle = 0;
	float offset[3] = {};

	if (GoC_KEY_CHECK(KEY_LKM))
	{
		static int count = 0;
		//offset[1] = 20.0f;

		if (count > 50)
			count = 0;


		CMatrix matCamera;
		pGame->GetCamera()->GetMatrix(&matCamera);
		//Нормализация камеры
		matCamera.vFront.Normalize();

		float campos[3];
		campos[0] = matCamera.vPos.fX;
		campos[1] = matCamera.vPos.fY;
		campos[2] = matCamera.vPos.fZ;
		float camVect[3];
		camVect[0] = matCamera.vFront.fX;
		camVect[1] = matCamera.vFront.fY;
		camVect[2] = matCamera.vFront.fZ;


		for (int i = 0; i < 3; i++)
			campos[i] += camVect[i] * ((5 + count++) * 3.0f);

		vect3_copy(campos, pos);
	}
	else
	{
		GetPosFromPosWithOffset(pos, &boomveh->base.matrix[12], offset, 0);

		if (pos[2] < actor->base.matrix[12 + 2] + 50.0f)
			speed[2] = 0.5f;
		//SetVehicleZAngle(boomveh, Angle, false);
	}



	main_GoC_send_sync(boomveh, pos, speed);
}

static int random(int min, int max)
{
	return min + (rand() % (max - min));
}

void main_spawner_chaos()
{

	actor_info* actor;
	vehicle_info* myveh;
	vehicle_info* boomveh;
	float pos[3] = {};
	float speed[3] = {};
	float offset[3] = {};

	if (!is_base_GoC_check(actor, myveh, boomveh))
		return;


	static float Angle = 0.0f;


	if (GoC_KEY_CHECK(KEY_LKM))
	{
		offset[2] = 10.0f;
		//offset[1] = 20.0f;

		GetPosFromPosWithOffset(pos, &actor->base.matrix[12], offset, NULL);


		for (int i = 0; i < 2; i++)
			pos[i] += random(-20, 20);

		vect3_copy((cheat_state->state == CHEAT_STATE_ACTOR) ? actor->speed : myveh->speed, speed);
		speed[2] += 0.2f;


		main_GoC_send_sync(boomveh, pos, speed);
	}
	else
	{

		offset[2] = 20.0f;
		offset[1] = 10.0f;
		GetPosFromPosWithOffset(pos, &actor->base.matrix[12], offset, Angle);
		SetVehicleZAngle(boomveh, Angle, false);

		const float AbsSpeed = 10.0f;

		speed[0] = AbsSpeed * cos(Angle);
		speed[1] = AbsSpeed * sin(Angle);
		speed[2] = -AbsSpeed;

		Angle += DEGTORAD(157);


		main_GoC_send_sync(boomveh, pos, speed);







		//float _vect3zero[3] = {};
		//float _vect3offset[3] = { 0.0, 10.0f, 0.0f };

		//static int type = 0;


		//GetPosFromPosWithOffset(speed, _vect3zero, _vect3offset, actor->fCurrentRotation);

		//switch (type)
		//{
		//case 0:
		//	offset[0] = -10.0f;
		//	break;
		//case 1:
		//	offset[0] = 0.0f;
		//	break;
		//case 2:
		//	offset[0] = 10.0f;
		//	break;
		//default:
		//	break;
		//}
		//offset[1] = 10.0f;
		//offset[2] = 5.0f;



		//if (++type > 2)
		//	type = 0;
		//
		////blue_eclipse_text("%3.0f %3.0f %3.0f ", offset[0], offset[1], offset[2]);

		//GetPosFromPosWithOffset(pos, &actor->base.matrix[12], offset, actor->fCurrentRotation);

		//SetVehicleZAngle(boomveh, actor->fCurrentRotation,false);
		//// temp
		////vect3_zero(speed);

		//main_GoC_send_sync(boomveh, pos, speed);
	}
}

static bool IsPassager(actor_info* actor)
{
	if (!actor)
		return false;

	if (actor->state != ACTOR_STATE_DRIVING)
		return false;

	if (!actor->vehicle)
		return false;

	if (actor->vehicle->passengers[0] == actor)
		return false;

	for (int i = 1; i < 9; i++)
		if (actor->vehicle->passengers[i] == actor)
			return true;

	return false;
}


static bool ActorIsVehicleDriver(actor_info* actor, vehicle_info* vehicle)
{
	if (!actor || !vehicle)
		return 0;

	if (actor->state != ACTOR_STATE_DRIVING)
		return 0;

	if (!actor->vehicle)
		return 0;

	if (actor->vehicle != vehicle)
		return 0;

	if (vehicle->passengers[0] != actor)
		return 0;

	return true;
}

static bool isVehicleHavePassagers(WORD vehicleid)
{
	return true;
}
static bool isVehicleHavePassagers(vehicle_info* vehicle)
{
	traceLastFunc("isVehicleHavePassagers");
	if (!vehicle)
		return false;

	if (*vehicle->passengers)
		return true;

	const int	seat_count = gta_vehicle_get_by_id(vehicle->base.model_alt_id)->passengers;
	if (seat_count > 0)
	{
		for (int seat = 1; seat <= seat_count; seat++)
		{
			if (vehicle->passengers[seat])
				return true;
		}
	}
	return false;
}

void main_grabber()
{
	actor_info* actor;
	vehicle_info* myveh;
	vehicle_info* boomveh;
	float pos[3] = {};
	float speed[3] = {};
	float offset[3] = {};

	static int vehicle_id = 0;

	static bool VehicleInArea[SAMP_MAX_VEHICLES];
	static DWORD last_founded_veh_tick = 0;
	//static DWORD last_sended_new_vehicle_tick = 0;

	actor = actor_info_get(ACTOR_SELF, NULL);
	if (!actor)
		return;

	if (g_Players->pLocalPlayer->iIsInSpawnScreen)
		return;

	myveh = vehicle_info_get(VEHICLE_SELF, NULL);

	if (actor->state == ACTOR_STATE_DRIVING && !myveh)
		return;

	if (!GoC_KEY_CHECK(KEY_LKM) && !GoC_KEY_CHECK(KEY_RKM))
	{
		last_founded_veh_tick = GetTickCount();
		vehicle_id = 0;
		GoC->grabber_vehicle_lock = false;
		return;
	}


	if (!syncronisations_settings->IsReadyToSync())
		return;

	if (GoC_KEY_CHECK(KEY_LKM))
	{

		boomveh =
			GetVehicleInfoStandart(
				VEHICLE_NO_BE_PROTECT |
				VEHICLE_NOT_OCCUPIED |
				VEHICLE_NOT_FRIEND |
				VEHICLE_NOT_CLOSED,
				VEHICLE_NEXT |
				VEHICLE_NOT_MY |
				VEHICLE_NOT_TRAIN);

		if (!boomveh)
			return;

		GetMapMarkerPos(pos);
		if (*pos)
		{
			syncronisations_settings->ForceSyncVehicleUnoccupied(getSAMPVehicleIDFromGTAVehicle(boomveh), pos, 1000.0f, 0, speed);
			//LocalClient->SendUnoccupiedSyncData(area_id, pos, 1000.0f, speed, 0);
			vect3_copy(pos, boomveh->base.matrix + 12);
			vect3_zero(boomveh->speed);
		}
	}
	else
	{

		//vect3_copy(pos, &boomveh->base.matrix[12]);
		//vect3_copy(speed, boomveh->speed);
		//vect3_zero(boomveh->spin);

		GetMapMarkerPos(pos);
		if (*pos)
		{

			if (!vehicle_id)
			{
				ZeroMemory(VehicleInArea, sizeof(bool) * SAMP_MAX_VEHICLES);
			}

			GoC->grabber_vehicle_lock = true;

			static int area_id = 0;
			vehicle_info* vinfo = 0;

			for (; area_id < SAMP_MAX_VEHICLES; area_id++)
			{
				vehicle_info* localinfo = getGTAVehicleFromSAMPVehicleID(area_id);

				if (!localinfo)
					continue;


				VehicleInArea[area_id] = true;

				if (vect3_dist(pos, localinfo->base.matrix + 12) < 50.0f)
					continue;


				if (isVehicleHavePassagers(localinfo))
					continue;


				// not train
				switch (localinfo->base.model_alt_id)
				{
				case 449:
				case 537:
				case 538:
				case 569:
				case 570:
				case 590:
					continue;
				}

				vinfo = localinfo;
				break;

			}

			cheat_state_text("vehicle_id: %i | Scaning...", vehicle_id);


			if (!vinfo)
			{
				DWORD now_tick = GetTickCount();

				if (now_tick - last_founded_veh_tick > 200)
				{
					VehicleInArea[vehicle_id++] = true;
					last_founded_veh_tick = now_tick;

					for (; vehicle_id < SAMP_MAX_VEHICLES; vehicle_id++)
					{
						if (VehicleInArea[vehicle_id])
							continue;
						else
							break;
					}

					if (vehicle_id >= SAMP_MAX_VEHICLES)
					{
						vehicle_id = 0;
						return;
					}
				}
				syncronisations_settings->ForceSyncVehiclePassanger(vehicle_id, actor->base.matrix + 12, 1000.0f);
			}
			else
			{

				syncronisations_settings->ForceSyncVehicleUnoccupied(area_id, pos, 1000.0f, 0, speed);
				//LocalClient->SendUnoccupiedSyncData(area_id, pos, 1000.0f, speed, 0);
				vect3_copy(pos, vinfo->base.matrix + 12);
				vect3_zero(vinfo->speed);
				VehicleInArea[area_id] = true;
				last_founded_veh_tick = GetTickCount();
			}

			if (area_id++ >= SAMP_MAX_VEHICLES)
				area_id = 0;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////------------------///////////////////////////////////////////////////////////
void faker_skin_attacker()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
 
	if (!actor)
		return;

	if (!syncronisations_settings->IsReadyToSync())
		return;

	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));

	if (BlackLightFuncs->bGoCfaker)
	{
		if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
		{
			GoC->faker_target = -1;
			return;
		}
		else
			GoC->faker_target = set.fPlayerID;
	}


	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;

	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	if (!deadman)
		return;


	float pos[3];
	vect3_copy(&deadman->base.matrix[4 * 3], pos);

	if ((deadman->state == ACTOR_STATE_DRIVING) && !deadman->vehicle)
		return;

	for (int i = 0; i < 2; i++)
		pos[i] += (deadman->state != ACTOR_STATE_DRIVING) ? deadman->speed[i] : (deadman->vehicle->speed[i]) * 15;

	pos[2] += 5.0f;
	float Speed[3] = { 0.0f, 0.0f, -10.0f };


	if (actor->state != ACTOR_STATE_DRIVING)
	{

		vehicle_info* boomveh =
			GetVehicleInfoStandart(
				VEHICLE_NO_BE_PROTECT |
				VEHICLE_NOT_OCCUPIED |
				VEHICLE_NOT_FRIEND |
				VEHICLE_NOT_CLOSED,
				VEHICLE_NEXT |
				VEHICLE_NOT_MY |
				VEHICLE_NOT_TRAIN);

		if (!boomveh)
			return;//if vehicle not found == crash :\

		if (boomveh)
			syncronisations_settings->SyncVehicle(getSAMPVehicleIDFromGTAVehicle(boomveh), pos, boomveh->hitpoints, NULL, Speed);
		else
			g_RakClient2->SendFakeOnfootSyncData(pos, Speed, g_Players->pLocalPlayer->onFootData.byteHealth, g_Players->pLocalPlayer->onFootData.byteArmor, g_Players->pLocalPlayer->sAnimFlags, g_Players->pLocalPlayer->sCurrentAnimID, NULL);
	}
	if (cheat_state->state == CHEAT_STATE_VEHICLE)
	{
		vehicle_info* vehicle = vehicle_info_get(VEHICLE_SELF, NULL);
		syncronisations_settings->SyncVehicle(getSAMPVehicleIDFromGTAVehicle(vehicle), pos, vehicle->hitpoints, NULL, Speed);
	}
}
void faker_kick_up()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
 


	if (!actor)
		return;

	if (!syncronisations_settings->IsReadyToSync())
		return;

	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));

	if (BlackLightFuncs->bGoCfaker)
	{
		if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
		{
			GoC->faker_target = -1;
			return;
		}
		else
			GoC->faker_target = set.fPlayerID;
	}

	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;

	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	if (!deadman)
		return;


	float pos[3];
	vect3_copy(&deadman->base.matrix[4 * 3], pos);

	if ((deadman->state == ACTOR_STATE_DRIVING) && !deadman->vehicle)
		return;

	for (int i = 0; i < 2; i++)
		pos[i] += (deadman->state != ACTOR_STATE_DRIVING) ? deadman->speed[i] : (deadman->vehicle->speed[i]) * 10;

	float Speed[3] = { 0.0f, 0.0f, 10.0f };


	if (actor->state != ACTOR_STATE_DRIVING)
	{

		vehicle_info* boomveh =
			GetVehicleInfoStandart(
				VEHICLE_NO_BE_PROTECT |
				VEHICLE_NOT_OCCUPIED |
				VEHICLE_NOT_FRIEND |
				VEHICLE_NOT_CLOSED,
				VEHICLE_NEXT |
				VEHICLE_NOT_MY |
				VEHICLE_NOT_TRAIN);

		if (!boomveh)
			return;


		if (boomveh)
			syncronisations_settings->SyncVehicle(getSAMPVehicleIDFromGTAVehicle(boomveh), pos, boomveh->hitpoints, NULL, Speed);
		else
			g_RakClient2->SendFakeOnfootSyncData(pos, Speed, actor->hitpoints, actor->armor, g_Players->pLocalPlayer->sAnimFlags, g_Players->pLocalPlayer->sCurrentAnimID, NULL);
	}
	if (cheat_state->state == CHEAT_STATE_VEHICLE)
	{
		vehicle_info* vehicle = vehicle_info_get(VEHICLE_SELF, NULL);
		syncronisations_settings->SyncVehicle(getSAMPVehicleIDFromGTAVehicle(vehicle), pos, vehicle->hitpoints, NULL, Speed);
	}
}
void faker_destructor()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
 

	if (!actor)
		return;

	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));

	if (BlackLightFuncs->bGoCfaker)
	{
		if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
		{
			GoC->faker_target = -1;
			return;
		}
		else
			GoC->faker_target = set.fPlayerID;
	}

	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;

	float fOrigin[3], fTarget[3], fCenter[3];

	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	actor_info* me = actor_info_get(ACTOR_SELF, NULL);

	vect3_copy(&me->base.matrix[4 * 3], fOrigin);
	vect3_copy(&deadman->base.matrix[4 * 3], fTarget);

	fCenter[0] = -0.01f;
	fCenter[1] = -0.02f;
	fCenter[2] = 0.04f;

	g_RakClient2->SendFakeBulletData(GoC->faker_target, fOrigin, fTarget, fCenter, g_Players->pLocalPlayer->byteCurrentWeapon, BULLET_HIT_TYPE_PLAYER);
}
void faker_loading()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
 


	if (!actor)
		return;

	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));

	if (BlackLightFuncs->bGoCfaker)
	{
		if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
		{
			GoC->faker_target = -1;
			return;
		}
		else
			GoC->faker_target = set.fPlayerID;
	}

	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;

	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	if (!deadman)
		return;


	float pos[3];
	float Speed[3] = {};

	for (int i = NULL; i < 3; i++)
		pos[i] = 2E10f;

	for (int i = NULL; i < 3; i++)
		Speed[i] = 75.0f;

	SyncTargetActor(deadman, pos, Speed);

}
void faker_stick()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
 
	if (!actor)
		return;

	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));

	if (BlackLightFuncs->bGoCfaker)
	{
		if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
		{
			GoC->faker_target = -1;
			return;
		}
		else
			GoC->faker_target = set.fPlayerID;
	}

	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;

	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	if (!deadman)
		return;


	float pos[3];
	float Speed[3] = {};
	float Offset[3] = { 0.0f, 10.0f, 5.0f };

	GetPosFromPosWithOffset(pos, &actor->base.matrix[12], Offset, actor->fCurrentRotation);


	SyncTargetActor(deadman, pos, Speed);
}

void faker_slep()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
 
	if (!actor)
		return;

	if (!syncronisations_settings->IsReadyToSync())
		return;

	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));

	if (BlackLightFuncs->bGoCfaker)
	{
		if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
		{
			GoC->faker_target = -1;
			return;
		}
		else
			GoC->faker_target = set.fPlayerID;
	}

	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;


	if (!syncronisations_settings->IsReadyToSync())
		return;


	vehicle_info* boomveh =
		GetVehicleInfoStandart(
			VEHICLE_NO_BE_PROTECT |
			VEHICLE_NOT_OCCUPIED |
			VEHICLE_NOT_FRIEND |
			VEHICLE_NOT_CLOSED,
			VEHICLE_NEXT |
			VEHICLE_NOT_MY |
			VEHICLE_NOT_TRAIN);

	if (!boomveh)
		return;

	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	if (!deadman)
		return;

	float Speed[3] = {};


	syncronisations_settings->SyncVehicle(getSAMPVehicleIDFromGTAVehicle(boomveh), &deadman->base.matrix[12], boomveh->hitpoints, NULL, Speed);
	vect3_copy(&deadman->base.matrix[12], &boomveh->base.matrix[12]);

	if (set.health_type == 2)
		GTAfunc_RepairVehicle(boomveh);

	if (set.health_type == 3)
		boomveh->hitpoints = 0.0f;

	vect3_zero(boomveh->spin);
}
void faker_kill()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);

	if (!actor)
		return;

	if (!syncronisations_settings->IsReadyToSync())
		return;

	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));

	if (BlackLightFuncs->bGoCfaker)
	{
		if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
		{
			GoC->faker_target = -1;
			return;
		}
		else
			GoC->faker_target = set.fPlayerID;
	}

	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;

	vehicle_info* boomveh =
		GetVehicleInfoStandart(
			VEHICLE_NO_BE_PROTECT |
			VEHICLE_NOT_OCCUPIED |
			VEHICLE_NOT_FRIEND |
			VEHICLE_NOT_CLOSED,
			VEHICLE_NEXT |
			VEHICLE_NOT_MY |
			VEHICLE_NOT_TRAIN);


	if (!boomveh)
		return;

	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	if (!deadman)
		return;

	float Speed[3] = { 0.0f, 0.0f, -set.upPower };
	float pos[3];


	vect3_copy(deadman->base.matrix + 12, pos);

	pos[2] += 5.0f;

	syncronisations_settings->SyncVehicle(getSAMPVehicleIDFromGTAVehicle(boomveh), pos, boomveh->hitpoints, NULL, Speed);
	vect3_copy(pos, &boomveh->base.matrix[12]);


	if (vect3_length(Speed) < 10.0f)
		vect3_copy(Speed, boomveh->speed);


	if (set.health_type == 2)
		GTAfunc_RepairVehicle(boomveh);

	if (set.health_type == 3)
		boomveh->hitpoints = 0.0f;

	vect3_zero(boomveh->spin);
}
void faker_remove_kebab()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
 
	if (!actor)
		return;

	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));

	if (BlackLightFuncs->bGoCfaker)
	{
		if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
		{
			GoC->faker_target = -1;
			return;
		}
		else
			GoC->faker_target = set.fPlayerID;
	}

	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;

	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	if (!deadman)
		return;


	float pos[3] = {};
	float Speed[3] = {};

	for (int i = 0; i < 2; i++)
		pos[i] = random(-2000, 2000);
	pos[2] = random(-100, 1000);

	SyncTargetActor(deadman, pos, Speed);
}

void faker_teleport_up()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
 
	if (!actor)
		return;

	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));

	if (BlackLightFuncs->bGoCfaker)
	{
		if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
		{
			GoC->faker_target = -1;
			return;
		}
		else
			GoC->faker_target = set.fPlayerID;
	}

	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;

	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	if (!deadman)
		return;


	float pos[3] = {};
	float Speed[3] = {};
	float Offset[3] = { 0.0f, 0.0f, 500.0f };
	GetPosFromPosWithOffset(pos, &deadman->base.matrix[12], Offset, deadman->fCurrentRotation);

	SyncTargetActor(deadman, pos, Speed);
}

void faker_teleport_map()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);

	if (!actor)
		return;

	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));

	if (BlackLightFuncs->bGoCfaker)
	{
		if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
		{
			GoC->faker_target = -1;
			return;
		}
		else
			GoC->faker_target = set.fPlayerID;
	}

	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;

	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	if (!deadman)
		return;

	float pos[3] = {};
	float Speed[3] = {};

	GetMapMarkerPos(pos);

	if (!*pos)
		vect3_copy(&deadman->base.matrix[4 * 3], pos);

	pos[2] += 50;

	SyncTargetActor(deadman, pos, Speed);
}

void faker_super_slap()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
 
	if (!actor)
		return;
	
	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));


	if (BlackLightFuncs->bGoCfaker)
	{
		if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
		{
			GoC->faker_target = -1;
			return;
		}
		else
			GoC->faker_target = set.fPlayerID;
	}

	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;

	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	if (!deadman)
		return;


	float pos[3];
	float Speed[3] = {};
	float _vect3zero[3] = {};
	float Offset[3] = { 0.0f, 0.0f, 100.0f };
	float _vect3offset[3] = { 0.0, 10.0f, 10.0f };

	GetPosFromPosWithOffset(pos, &deadman->base.matrix[12], Offset, deadman->fCurrentRotation);
	GetPosFromPosWithOffset(Speed, _vect3zero, _vect3offset, deadman->fCurrentRotation);

	SyncTargetActor(deadman, pos, Speed);
}
void faker_cardeformer()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
 
	if (!actor)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;

	vehicle_info* vehicle =
		GetVehicleInfoStandart(
			VEHICLE_NO_BE_PROTECT |
			VEHICLE_NOT_FRIEND |
			VEHICLE_NOT_CLOSED,
			VEHICLE_NEXT |
			VEHICLE_NOT_OCCUPIED |
			VEHICLE_NOT_MY |
			VEHICLE_ALIVE |
			VEHICLE_NOT_TRAIN);


	if (!vehicle)
		return;

	float fPos[3];
	vect3_copy(&actor->base.matrix[4 * 3], fPos);


	fPos[0] += sinf(-actor->fCurrentRotation) * 15.0f;
	fPos[1] += cosf(-actor->fCurrentRotation) * 15.0f;

	float fGroundZHeight = pGame->GetWorld()->FindGroundZForPosition(fPos[0], fPos[1]);

	fPos[2] = fGroundZHeight;

	stUnoccupiedData unSync;
	RakNet::BitStream bsUnoccupiedSync;
	ZeroMemory(&unSync, sizeof(stUnoccupiedData));

	unSync.sVehicleID = getSAMPVehicleIDFromGTAVehicle(vehicle);

	vect3_copy(fPos, unSync.fPosition);

	unSync.fRoll[0] = 100; // size x   100
	unSync.fRoll[1] = 100; // size y	100
	unSync.fRoll[2] = 100; // size z	100

	vect3_zero(unSync.fMoveSpeed);
	vect3_zero(unSync.fTurnSpeed);

	vect3_copy(unSync.fRoll, &vehicle->base.matrixStruct->right.X);
	unSync.fDirection[0] = 100; // 100
	unSync.fDirection[1] = 100; // 100b2
	unSync.fDirection[2] = -2; //-2.0f

	vect3_copy(unSync.fDirection, &vehicle->base.matrixStruct->up.X);
	unSync.fHealth = 1000.0f;

	bsUnoccupiedSync.Write((BYTE)ID_UNOCCUPIED_SYNC);
	bsUnoccupiedSync.Write((PCHAR)&unSync, sizeof(stUnoccupiedData));
	g_RakClient2->Send(&bsUnoccupiedSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

	cheat_vehicle_teleport(vehicle, fPos, gta_interior_id_get());

	vehicle->hitpoints = 1000.0f;

}


void faker_pizdarvanka()
{
	stOnFootData onSync;
	memset(&onSync, 0, sizeof(stOnFootData));
	RakNet::BitStream bsOnfootSync;

	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);

	if (!actor)
		return;

	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));

	if (BlackLightFuncs->bGoCfaker)
	{
		if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
		{
			GoC->faker_target = -1;
			return;
		}
		else
			GoC->faker_target = set.fPlayerID;
	}


	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;

	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	if (!deadman)
		return;

	vect3_copy(&deadman->base.matrix[4 * 3], onSync.fPosition);

	if ((deadman->state == ACTOR_STATE_DRIVING) && !deadman->vehicle)
		return;

	onSync.byteHealth = actor->hitpoints;

	onSync.fPosition[2] -= 0.3f;
	onSync.sCurrentAnimationID = 959;
	for (int i = 0; i < 4; i++)
		onSync.fQuaternion[i] = random_float(0, 2);
	onSync.fMoveSpeed[2] -= (float)-set.speedkick_speed + rand() % (set.speedkick_speed * 2);

	bsOnfootSync.Write((BYTE)ID_PLAYER_SYNC);
	bsOnfootSync.Write((PCHAR)&onSync, sizeof(stOnFootData));
	g_RakClient2->Send(&bsOnfootSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
}


void faker_teleport_magnet()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
 
	if (!actor)
		return;

	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));

		if (BlackLightFuncs->bGoCfaker)
		{
			if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
			{
				GoC->faker_target = -1;
				return;
			}
			else
				GoC->faker_target = set.fPlayerID;
		}
	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	if (!deadman)
		return;


	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;


	float pos[3] = { -1569.4526f, 98.3293f, 20.0894f };
	float Speed[3] = {};

	SyncTargetActor(deadman, pos, Speed);
}




void faker_car_shooter()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);

	if (!actor)
		return;

	if (!syncronisations_settings->IsReadyToSync())
		return;

	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));

	if (BlackLightFuncs->bGoCfaker)
	{
		if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
		{
			GoC->faker_target = -1;
			return;
		}
		else
			GoC->faker_target = set.fPlayerID;
	}


	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;

	vehicle_info* boomveh =
		GetVehicleInfoStandart(
			VEHICLE_NO_BE_PROTECT |
			VEHICLE_NOT_OCCUPIED |
			VEHICLE_NOT_FRIEND |
			VEHICLE_NOT_CLOSED,
			VEHICLE_NEXT |
			VEHICLE_NOT_MY |
			VEHICLE_NOT_TRAIN);


	if (!boomveh)
		return;

	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	if (!deadman)
		return;

	float Speed[3] = { 0.0f, 10.0f, 0.0f };
	float offset[3] = { 0.0f, 10.0f, 0.0f };
	float pos[3];
	float f_vect_zero[3] = {};


	//vect3_copy(deadman->base.matrix + 12, pos);
	//pos[2] += 5.0f;

	GetPosFromPosWithOffset(pos, deadman->base.matrix + 12, offset, deadman->fCurrentRotation);
	GetPosFromPosWithOffset(Speed, f_vect_zero, Speed, deadman->fCurrentRotation);

	syncronisations_settings->SyncVehicle(getSAMPVehicleIDFromGTAVehicle(boomveh), pos, boomveh->hitpoints, NULL, Speed);
	vect3_copy(pos, boomveh->base.matrix + 12);
	vect3_copy(Speed, boomveh->speed);

	if (set.health_type == 2)
		GTAfunc_RepairVehicle(boomveh);

	if (set.health_type == 3)
		boomveh->hitpoints = 0.0f;

	vect3_zero(boomveh->spin);
}


void faker_up_shooter()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
 
	if (!actor)
		return;

	if (!syncronisations_settings->IsReadyToSync())
		return;
	
	GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
		GoC->set.incar_only.enabled,
		GoC->set.onfoot_only.enabled, true,
		GoC->set.ignore_my_passagers.enabled,
		GoC->set.ignore_afk.enabled,
		!pSampMulti->IsModDeveloper(GoC->faker_target),
		GoC->set.ignore_attach.enabled,
		BlackLightFuncs->bIsFriend[GoC->faker_target]));

		if (BlackLightFuncs->bGoCfaker)
		{
			if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
			{
				GoC->faker_target = -1;
				return;
			}
			else
				GoC->faker_target = set.fPlayerID;
		}


	if (GoC->faker_target == INVALID)
		return;

	if (!GoC_KEY_CHECK(KEY_R))
		return;

	vehicle_info* boomveh =
		GetVehicleInfoStandart(
			VEHICLE_NO_BE_PROTECT |
			VEHICLE_NOT_OCCUPIED |
			VEHICLE_NOT_FRIEND |
			VEHICLE_NOT_CLOSED,
			VEHICLE_NEXT |
			VEHICLE_NOT_MY |
			VEHICLE_NOT_TRAIN);


	if (!boomveh)
		return;

	actor_info* deadman = getGTAPedFromSAMPPlayerID(GoC->faker_target);
	if (!deadman)
		return;

	float Speed[3] = { 0.0f, 0.0f, set.upPower };
	float offset[3] = { 0.0f, 0.0f, 0.0f };
	float pos[3];
	float f_vect_zero[3] = {};

	//vect3_copy(deadman->base.matrix + 12, pos);
	//pos[2] += 5.0f;

	GetPosFromPosWithOffset(pos, deadman->base.matrix + 12, offset, deadman->fCurrentRotation);
	GetPosFromPosWithOffset(Speed, f_vect_zero, Speed, deadman->fCurrentRotation);

	if ((deadman->state == ACTOR_STATE_DRIVING) && !deadman->vehicle)
		return;

	for (int i = 0; i < 2; i++)
		pos[i] += (deadman->state != ACTOR_STATE_DRIVING) ? deadman->speed[i] : (deadman->vehicle->speed[i]) * 10;

	syncronisations_settings->SyncVehicle(getSAMPVehicleIDFromGTAVehicle(boomveh), pos, boomveh->hitpoints, NULL, Speed);
	vect3_copy(pos, boomveh->base.matrix + 12);

	if (vect3_length(Speed) < 10.0f)
		vect3_copy(Speed, boomveh->speed);

	if (set.health_type == 2)
		GTAfunc_RepairVehicle(boomveh);

	if (set.health_type == 3)
		boomveh->hitpoints = 0.0f;

	vect3_zero(boomveh->spin);
}


inline void disable_control()
{
	vehicle_info* now_vehicle = vehicle_info_get(VEHICLE_SELF, NULL);
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);

	stGoC::stControll* control = &GoC->control;

	vehicle_info* my_vehicle = getGTAVehicleFromSAMPVehicleID(control->my_vehicleid);

	if (my_vehicle)
		GTAfunc_PutActorInCar(my_vehicle);
	else
		if (now_vehicle)
			GTAfunc_RemoveActorFromCarAndPutAt(control->save_pos);
		else
			vect3_copy(control->save_pos, actor->base.matrix + 12);


	pGameInterface->GetCamera()->RestoreWithJumpCut();
	control->toggle = false;
}

void faker_controller()
{
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);
	vehicle_info* now_vehicle = vehicle_info_get(VEHICLE_SELF, NULL);
 
	if (!actor)
		return;

	stGoC::stControll* control = &GoC->control;

	if (!control->toggle)
	{

		if (GoC->faker != stGoC::faker_funcs::GOC_FAKER_CONTROL)
			return;

		if (!now_vehicle)
		{
			GoC->faker_target = INVALID;
			return;
		}
		
		GoC->faker_target = GetClosestPlayerid(actor, GetActorFlag(true,
			GoC->set.incar_only.enabled,
			GoC->set.onfoot_only.enabled, true,
			GoC->set.ignore_my_passagers.enabled,
			GoC->set.ignore_afk.enabled,
			!pSampMulti->IsModDeveloper(GoC->faker_target),
			GoC->set.ignore_attach.enabled,
			BlackLightFuncs->bIsFriend[GoC->faker_target]));//GoC->set.ignore_friends.enabled));

		if (BlackLightFuncs->bGoCfaker)
		{
			if (BlackLightFuncs->bIsFriend[set.fPlayerID] || pSampMulti->IsModDeveloper(set.fPlayerID))
			{
				GoC->faker_target = -1;
				return;
			}
			else
				GoC->faker_target = set.fPlayerID;
		}
		
		if (GoC->faker_target == INVALID)
			return;

		if (KEY_PRESSED(KEY_R))
		{
			vect3_copy(now_vehicle->base.matrix + 12, control->save_pos);
			control->my_vehicleid = getSAMPVehicleIDFromGTAVehicle(now_vehicle);
			control->playerid = GoC->faker_target;

			actor_info* target = getGTAPedFromSAMPPlayerID(GoC->faker_target);
			GTAfunc_PutActorInCar(target->vehicle);
			pGameInterface->GetCamera()->RestoreWithJumpCut();
			control->toggle = true;

		}
	}
	else
	{
		if (((KEY_PRESSED(KEY_R) || !now_vehicle) && (GoC->faker == stGoC::faker_funcs::GOC_FAKER_CONTROL))
			||
			((GoC->faker != stGoC::faker_funcs::GOC_FAKER_CONTROL) && control->toggle)
			||
			!getGTAPedFromSAMPPlayerID(control->playerid))
		{
			disable_control();
		}
	}
}

void main_hook()
{
	switch (GoC->main)
	{
	case stGoC::main_funcs::GOC_MAIN_NONE:
		break;
	case stGoC::main_funcs::GOC_MAIN_SUPERGUN:
		main_super_gun();
		break;
	case stGoC::main_funcs::GOC_MAIN_LUGGER_SUN:
		main_lugger_sun();
		break;
	case stGoC::main_funcs::GOC_MAIN_TORNADO_MAGNET:
		main_tornado_magnet();
		break;
	case stGoC::main_funcs::GOC_MAIN_SPANWER_CHAOS:
		main_spawner_chaos();
		break;
	case stGoC::main_funcs::GOC_MAIN_SNAKE_UNVRAVITY:
		main_snake_ungravity();
		break;
	case stGoC::main_funcs::GOC_MAIN_GRABBER:
		main_grabber();
		break;

	default:
		GoC->grabber_vehicle_lock = false;
		break;
	}
}

void faker_hook()
{
	faker_controller();

	switch (GoC->faker)
	{
	case stGoC::faker_funcs::GOC_FAKER_SKIN_ATTACKER:
		faker_skin_attacker();
		break;
	case stGoC::faker_funcs::GOC_FAKER_KICKUP:
		faker_kick_up();
		break;
	case stGoC::faker_funcs::GOC_FAKER_DESTRUCTOR:
		faker_destructor();
		break;
	case stGoC::faker_funcs::GOC_FAKER_LOADING:
		faker_loading();
		break;
	case stGoC::faker_funcs::GOC_FAKER_STICK:
		faker_stick();
		break;
	case stGoC::faker_funcs::GOC_FAKER_SLEP:
		faker_slep();
		break;
	case stGoC::faker_funcs::GOC_FAKER_KILL:
		faker_kill();
		break;
	case stGoC::faker_funcs::GOC_FAKER_REMOVE_KEBAB:
		faker_remove_kebab();
		break;
	case stGoC::faker_funcs::GOC_FAKER_TELE_UP:
		faker_teleport_up();
		break;
	case stGoC::faker_funcs::GOC_FAKER_MAP_TELE:
		faker_teleport_map();
		break;
	case stGoC::faker_funcs::GOC_FAKER_SLAP:
		faker_super_slap();
		break;
	case stGoC::faker_funcs::GOC_FAKER_CARDEFORMER:
		faker_cardeformer();
		break;
	case stGoC::faker_funcs::GOC_FACKER_PIZDARVANKA:
		faker_pizdarvanka();
		break;
	case stGoC::faker_funcs::GOC_FAKER_MAGNIT:
		faker_teleport_magnet();
		break;

	case stGoC::faker_funcs::GOC_FAKER_UP_SHOOTER:
		faker_up_shooter();
		break;

	case stGoC::faker_funcs::GOC_FAKER_CAR_SHOOTER:
		faker_car_shooter();
		break;

	}
}

void God_of_Cars_hook()
{
	traceLastFunc("God_of_Cars_hook");

	for (int i = 0; i < stGoC::main_funcs::GOC_MAIN_SPANWER_CHAOS; i++)
		if (KEYCOMBO_PRESSED(GoC->main_key[i]))
		{
			if ((i + 1) == GoC->main)
				GoC->main = stGoC::main_funcs::GOC_MAIN_NONE;
			else
				GoC->main = (stGoC::main_funcs)(i + 1);
		}

	for (int i = 0; i < stGoC::faker_funcs::GOC_FAKER_CAR_SHOOTER; i++)
		if (KEYCOMBO_PRESSED(GoC->faker_key[i]))
		{
			if ((i + 1) == GoC->faker)
				GoC->faker = stGoC::faker_funcs::GOC_FAKER_NONE;
			else
				GoC->faker = (stGoC::faker_funcs)(i + 1);
		}

	main_hook();
	faker_hook();
}

bool Windows_Procedure_GoC_Hook(HWND wnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	if (gta_menu_active())
		return false;

	if (cheat_state->_generic.menu || BlackLightFuncs->Menu.bImScoreboad || BlackLightFuncs->Menu.bMain_Menu)
	{
		if (!g_Players->pLocalPlayer->iIsInSpawnScreen)
		{
			switch (umsg)
			{
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYDOWN:
			case WM_KEYUP:
				if ((int)wparam == KEY_R && GoC->faker != stGoC::GOC_FAKER_NONE)
					return true; // игнорировать сообщения, когда мы юзаем функции гока
				break;
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
				if (GoC->main != stGoC::GOC_MAIN_NONE)
					return true;
				break;
			}
		}
	}
	return false; // is all ok
}