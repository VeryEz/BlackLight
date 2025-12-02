#include "main.h"
//_=Gigant=_ was here 

struct stSync syncInstance;
struct stSync* syncronisations_settings = &syncInstance; //goc by masterzero

//MasterZero syncs
bool stSync::IsReadyToSync()
{
	return !vehicle_deque.size();
}

bool stSync::IgnoreIncarData()
{

	switch (sync)
	{
	case stSync::SYNC_TYPE_DEFAULT:
	case stSync::SYNC_TYPE_ECTOPIC:
	case stSync::SYNC_TYPE_UNOCCUPIED:
		return false;
	}

	if (car_enter.timeout)
	{
		if (!car_enter.IsFree())
			return true; // ignore incar packets
	}


	if (vehicle_deque.size())
		return true;

	return false;
}

bool stSync::IgnoreOnFoot()
{
	switch (sync)
	{
	case stSync::SYNC_TYPE_DEFAULT:
	case stSync::SYNC_TYPE_ECTOPIC:
	case stSync::SYNC_TYPE_UNOCCUPIED:
		return false;
	}

	if (vehicle_deque.size())
		return true;

	return false;
}


bool stSync::IgnorePassagerData()
{

	switch (sync)
	{
	case stSync::SYNC_TYPE_DEFAULT:
	case stSync::SYNC_TYPE_ECTOPIC:
	case stSync::SYNC_TYPE_UNOCCUPIED:
		return false;
	}


	if (car_enter.timeout)
	{
		if (!car_enter.IsFree())
			return true; // ignore incar packets
	}

	if (vehicle_deque.size())
		return true;


	return false;
}

bool stCarEnter::IsFree()
{
	long now_tick = GetTickCount();
	if (now_tick - dw_last_tick < timeout)
		return false; // ignore incar packets
	else
		return true;
}

void stCarEnter::Send(WORD vehicleid, int passid)
{
	if (!timeout)
		return;

	if ((vehicleid == syncronisations_settings->last_vehicle_id) &&
		(syncronisations_settings->last_sended_state == stSync::sending_state::SENDED_STATE_INCAR ||
			syncronisations_settings->last_sended_state == stSync::sending_state::SENDED_STATE_PASSAGER))
		return;

	dw_last_tick = GetTickCount();

	g_RakClient2->SyncFakeEnterVehicle(vehicleid, passid);
}

void stSync::Hook()
{
	//DebugThread.push_trace_func(eDebugThread::DEBUG_THREAD_RENDER, "void stSync::Hook()");
	if (!vehicle_deque.size())
		return;

	WORD vehicleid = vehicle_deque.begin()->vehicleid;
	vehicle_info* my_vehicle = getGTAVehicleFromSAMPVehicleID(last_vehicle_id);/*getGTAVehicleFromSAMPVehicleID(g_Players->pLocalPlayer->inCarData.sVehicleID)*/;
	actor_info* actor = actor_info_get(ACTOR_SELF, NULL);

	if (!actor)
		return;

	if (my_vehicle && vehicleid == last_vehicle_id && last_sended_state == sending_state::SENDED_STATE_INCAR)
	{
		g_RakClient2->SendFakeDriverSyncData(vehicleid,
			vehicle_deque.begin()->pos,
			vehicle_deque.begin()->health,
			vehicle_deque.begin()->speed, 0);
		vehicle_deque.pop_front();
		return;
	}

	//DebugThread.push_trace_func(eDebugThread::DEBUG_THREAD_RENDER, "void stSync::Hook 1()");

	long now_tick = GetTickCount();
	if (vehicle_deque.begin()->force)
	{
		if (state == state_sync::SYNC_STATE_READY)
		{
			switch (vehicle_deque.begin()->force_type)
			{
			case stSync::SYNC_TYPE_INCAR:

				if (state == state_sync::SYNC_STATE_READY)
				{
					if (car_enter.timeout)
					{
						if (car_enter.IsFree())
						{
							car_enter.Send(vehicleid, 0);
							state = state_sync::SYNC_STATE_ENTER;
						}
						return;
					}
					else
						state = state_sync::SYNC_STATE_ENTER;

				} // if (state == state_sync::SYNC_STATE_READY)

				//DebugThread.push_trace_func(eDebugThread::DEBUG_THREAD_RENDER, "void stSync::Hook() 2");
				if (state == state_sync::SYNC_STATE_ENTER)
				{
					if (!car_enter.timeout || car_enter.IsFree())
					{
						g_RakClient2->SendFakeDriverSyncData(
							vehicleid,
							vehicle_deque.begin()->pos,
							vehicle_deque.begin()->health,
							vehicle_deque.begin()->speed, 0);
						//	vehicle_deque.begin()->rotation);
						dw_base_last_tick = now_tick;
						state = state_sync::SYNC_STATE_TIMEOUT;
					}
					else
						return;
				}
				break;

			case sync_type::SYNC_TYPE_PASSANGER:
				g_RakClient2->SendFakePassengerSyncData(vehicleid,
					vehicle_deque.begin()->pos,
					vehicle_deque.begin()->health,
					g_Players->pLocalPlayer->byteCurrentWeapon,
					1, 0);
				break;

			case sync_type::SYNC_TYPE_UNOCCUPIED:
				g_RakClient2->SendUnoccupiedSyncData(vehicleid,
					vehicle_deque.begin()->pos,
					vehicle_deque.begin()->health,
					vehicle_deque.begin()->speed, 0);
				break;
			}

			dw_base_last_tick = now_tick;
			state = state_sync::SYNC_STATE_TIMEOUT;
		}
	}
	else
	{
		if (!getGTAVehicleFromSAMPVehicleID(vehicleid))
		{
			vehicle_deque.pop_front();
			return;
		}

		switch (sync)
		{

		case stSync::SYNC_TYPE_ECTOPIC:
			if (state == state_sync::SYNC_STATE_READY)
			{
				dw_base_last_tick = now_tick;
				state = state_sync::SYNC_STATE_TIMEOUT;
				break;
			}

		case stSync::SYNC_TYPE_DEFAULT:
			if (state == state_sync::SYNC_STATE_READY)
			{
				dw_base_last_tick = now_tick;
				state = state_sync::SYNC_STATE_TIMEOUT;
				break;
			}
		case stSync::SYNC_TYPE_INCAR:
			if (state == state_sync::SYNC_STATE_READY)
			{
				if (car_enter.timeout)
				{

					if (car_enter.IsFree())
					{
						car_enter.Send(vehicleid, 0);
						state = state_sync::SYNC_STATE_ENTER;
					}
					return;
				}
				else
					state = state_sync::SYNC_STATE_ENTER;

			} // if (state == state_sync::SYNC_STATE_READY)

			//DebugThread.push_trace_func(eDebugThread::DEBUG_THREAD_RENDER, "void stSync::Hook() 3");
			if (state == state_sync::SYNC_STATE_ENTER)
			{
				if (!car_enter.timeout || car_enter.IsFree())
				{
					g_RakClient2->SendFakeDriverSyncData(
						vehicleid,
						vehicle_deque.begin()->pos,
						vehicle_deque.begin()->health,
						vehicle_deque.begin()->speed, 0);
					//	vehicle_deque.begin()->rotation);
					dw_base_last_tick = now_tick;
					state = state_sync::SYNC_STATE_TIMEOUT;
				}
				else
					return;
			}
			break;

		case stSync::SYNC_TYPE_TRAILER:
			if (state == state_sync::SYNC_STATE_READY)
			{
				if (my_vehicle)
				{
					g_RakClient2->SendFakeDriverSyncData(getSAMPVehicleIDFromGTAVehicle(my_vehicle),
						g_Players->pLocalPlayer->inCarData.fPosition,
						g_Players->pLocalPlayer->inCarData.fVehicleHealth,
						g_Players->pLocalPlayer->inCarData.fMoveSpeed,
						g_Players->pLocalPlayer->inCarData.sKeys,
						vehicleid);

					g_RakClient2->SendFakeTrailerData(vehicleid,
						vehicle_deque.begin()->pos,
						vehicle_deque.begin()->health,
						vehicle_deque.begin()->speed);
				}
				dw_base_last_tick = now_tick;
				state = state_sync::SYNC_STATE_TIMEOUT;

			}
			break;

		case stSync::SYNC_TYPE_UNOCCUPIED:
			if (state == state_sync::SYNC_STATE_READY)
			{
				g_RakClient2->SendUnoccupiedSyncData(vehicleid,
					vehicle_deque.begin()->pos,
					vehicle_deque.begin()->health,
					vehicle_deque.begin()->speed,
					1);



				dw_base_last_tick = now_tick;
				state = state_sync::SYNC_STATE_TIMEOUT;

			}
			break;

		case stSync::SYNC_TYPE_TIMEOUT_SYNC:

			switch (timeout_settings.state)
			{
			case stTimeout_sync::syncer_state::TIME_SYNC_STATE_READY:
				if (!last_sended_state == sending_state::SENDED_STATE_ONFOOT)
				{
					g_RakClient2->SendFakeOnfootSyncData(vehicle_deque.begin()->pos, actor->speed, actor->hitpoints,
						actor->armor, g_Players->pLocalPlayer->onFootData.sAnimFlags,
						g_Players->pLocalPlayer->onFootData.sCurrentAnimationID,
						g_Players->pLocalPlayer->onFootData.fQuaternion);
					timeout_settings.last_tick_self_out = now_tick;
				}
				timeout_settings.state = stTimeout_sync::syncer_state::TIME_SYNC_STATE_SELF_OUT;

				return;
			case stTimeout_sync::syncer_state::TIME_SYNC_STATE_SELF_OUT:
				if (now_tick - timeout_settings.last_tick_self_out > timeout_settings.timeout_self_out)
				{
					g_RakClient2->SyncFakeEnterVehicle(vehicleid, NULL);
					timeout_settings.last_tick_enter = now_tick;
					timeout_settings.state = stTimeout_sync::syncer_state::TIME_SYNC_STATE_ENTER;
				}
				return;
			case stTimeout_sync::syncer_state::TIME_SYNC_STATE_ENTER:
				if (now_tick - timeout_settings.last_tick_enter > timeout_settings.timeout_enter)
				{
					g_RakClient2->SendFakeDriverSyncData(
						vehicleid,
						vehicle_deque.begin()->pos,
						vehicle_deque.begin()->health,
						vehicle_deque.begin()->speed, 0);
					timeout_settings.last_tick_inside = now_tick;
					timeout_settings.state = stTimeout_sync::syncer_state::TIME_SYNC_STATE_INSIDE;
				}
				return;
			case stTimeout_sync::syncer_state::TIME_SYNC_STATE_INSIDE:
				if (now_tick - timeout_settings.last_tick_inside > timeout_settings.timeout_inside)
				{
					g_RakClient2->SendFakeOnfootSyncData(vehicle_deque.begin()->pos, actor->speed, actor->hitpoints, actor->armor,
						g_Players->pLocalPlayer->onFootData.sAnimFlags,
						g_Players->pLocalPlayer->onFootData.sCurrentAnimationID,
						g_Players->pLocalPlayer->onFootData.fQuaternion);
					timeout_settings.last_tick_out = now_tick;
					timeout_settings.state = stTimeout_sync::syncer_state::TIME_SYNC_STATE_OUT;
				}
				return;
			case stTimeout_sync::syncer_state::TIME_SYNC_STATE_OUT:
				if (now_tick - timeout_settings.last_tick_out > timeout_settings.timeout_out)
				{
					if (!last_sended_state == sending_state::SENDED_STATE_ONFOOT)
						timeout_settings.last_tick_goback = now_tick;
					timeout_settings.state = stTimeout_sync::syncer_state::TIME_SYNC_STATE_GO_BACK;
				}
				return;
			case stTimeout_sync::syncer_state::TIME_SYNC_STATE_GO_BACK:
				if (!my_vehicle)
					timeout_settings.state = stTimeout_sync::syncer_state::SYNC_STATE_SELF_VEHICLE;
				if (now_tick - timeout_settings.last_tick_goback > timeout_settings.timeout_goback)
				{
					if (my_vehicle)
						g_RakClient2->SendFakeDriverSyncData(
							getSAMPVehicleIDFromGTAVehicle(my_vehicle),
							&my_vehicle->base.matrix[12],
							my_vehicle->hitpoints,
							my_vehicle->speed, 0);
					timeout_settings.state = stTimeout_sync::syncer_state::SYNC_STATE_SELF_VEHICLE;
				}

				return;
			case stTimeout_sync::syncer_state::SYNC_STATE_SELF_VEHICLE:
				if (now_tick - timeout_settings.last_tick_self_vehicle > timeout_settings.timeout_self_vehicle)
				{
					vehicle_deque.pop_front();
					timeout_settings.state = stTimeout_sync::syncer_state::TIME_SYNC_STATE_READY;
				}
				return;
			}

			break;
		case stSync::SYNC_TYPE_PASSANGER:
			if (state == state_sync::SYNC_STATE_READY)
			{
				g_RakClient2->SendFakePassengerSyncData(vehicleid,
					vehicle_deque.begin()->pos,
					vehicle_deque.begin()->health,
					g_Players->pLocalPlayer->byteCurrentWeapon,
					1, 0);

				dw_base_last_tick = now_tick;
				state = state_sync::SYNC_STATE_TIMEOUT;
			}
			break;
		default:
			break;
		}
	}

	if (state == state_sync::SYNC_STATE_TIMEOUT)
	{
		if (now_tick - dw_base_last_tick > timeout_sync)
		{
			vehicle_deque.pop_front();
			state = state_sync::SYNC_STATE_READY;
		}
	}

	//DebugThread.push_trace_func(eDebugThread::DEBUG_THREAD_RENDER, "void stSync::Hook() end");
}


bool stSync::is_used_timeout_sync()
{
	if (vehicle_deque.size() &&
		(sync == sync_type::SYNC_TYPE_TIMEOUT_SYNC))
		return true;
	else
		return false;
}

void stSync::ForceSyncVehicleUnoccupied(WORD vehicleid, float* pos, float health, float* rotation, float* speed)
{
	stVehicleSyncValues item;

	item.vehicleid = vehicleid;
	item.health = health;

	vect3_copy(pos, item.pos);
	vect3_copy(speed, item.speed);
	item.force = true;
	item.force_type = sync_type::SYNC_TYPE_UNOCCUPIED;

	if (rotation)
		memcpy(item.rotation, rotation, sizeof(float) * 4);

	vehicle_deque.push_back(item);

	if (vehicle_deque.size() == 1)
		Hook();

}


void stSync::ForceSyncVehicleIncar(WORD vehicleid, float* pos, float health, float* rotation, float* speed)
{
	stVehicleSyncValues item;

	item.vehicleid = vehicleid;
	item.health = health;

	vect3_copy(pos, item.pos);
	vect3_copy(speed, item.speed);
	item.force = true;
	item.force_type = sync_type::SYNC_TYPE_INCAR;

	if (rotation)
		memcpy(item.rotation, rotation, sizeof(float) * 4);


	vehicle_deque.push_back(item);


	if (vehicle_deque.size() == 1)
		Hook();

}


void stSync::ForceSyncVehiclePassanger(WORD vehicleid, float* pos, float health)
{
	stVehicleSyncValues item;

	item.vehicleid = vehicleid;
	item.health = health;

	vect3_copy(pos, item.pos);

	item.force = true;
	item.force_type = sync_type::SYNC_TYPE_PASSANGER;

	vehicle_deque.push_back(item);


	if (vehicle_deque.size() == 1)
		Hook();

}



void stSync::SyncVehicle(WORD vehicleid, float* pos, float health, float* rotation, float* speed)
{
	stVehicleSyncValues item;


	item.vehicleid = vehicleid;
	item.health = health;

	vect3_copy(pos, item.pos);
	vect3_copy(speed, item.speed);
	item.force = false;

	if (rotation)
		memcpy(item.rotation, rotation, sizeof(float) * 4);


	vehicle_deque.push_back(item);


	if (vehicle_deque.size() == 1)
		Hook();

}