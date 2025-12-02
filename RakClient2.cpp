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

RakClient2 *g_RakClient2 = NULL;

RakClient2::RakClient2( void *pRakClientInterface )
{
	pRakClient = (RakClientInterface2 *)pRakClientInterface;
}

bool RakClient2::RPC( int rpcId, BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp )
{
	if ( !pRakClient )
		return false;

	return pRakClient->RPC( &rpcId, bitStream, priority, reliability, orderingChannel, shiftTimestamp );
}

bool RakClient2::Send( BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel )
{
	if ( !pRakClient )
		return false;

	return pRakClient->Send( bitStream, priority, reliability, orderingChannel );
}

// misc functions
void RakClient2::SendDeath( uint16_t killerId, uint8_t reason )
{
	BitStream bsDeath;

	bsDeath.Write( reason );
	bsDeath.Write( killerId );
	this->RPC( RPC_Death, &bsDeath );
}

void RakClient2::SendPickUp( int pickupId )
{
	BitStream bsPickup;

	bsPickup.Write( pickupId );
	
	this->RPC( RPC_PickedUpPickup, &bsPickup );
}

void RakClient2::SyncFakeEnterVehicle(WORD vehicleid, int  passid)
{
	RakNet::BitStream bs;


	bs.Write(vehicleid);
	bs.Write(passid);
	this->RPC(RPC_EnterVehicle, &bs);
}

void RakClient2::SendFakeDriverSyncData(WORD vehicleid, float fPos[3], float HealthCar, float Speed[3], WORD key, WORD trailer)
{
	struct actor_info* self = actor_info_get(ACTOR_SELF, ACTOR_ALIVE);
	stInCarData InCarData;
	ZeroMemory(&InCarData, sizeof(stInCarData));
	RakNet::BitStream bsVehicleSync;
	InCarData.sVehicleID = vehicleid;

	InCarData.fPosition[0] = fPos[0];
	InCarData.fPosition[1] = fPos[1];
	InCarData.fPosition[2] = fPos[2];

	InCarData.fVehicleHealth = HealthCar;

	InCarData.fMoveSpeed[0] = Speed[0];
	InCarData.fMoveSpeed[1] = Speed[1];
	InCarData.fMoveSpeed[2] = Speed[2];

	InCarData.sKeys = key;
	InCarData.bytePlayerHealth = self->hitpoints;
	InCarData.byteArmor = self->armor;

	InCarData.sTrailerID = trailer;

	bsVehicleSync.Write((BYTE)ID_VEHICLE_SYNC);
	bsVehicleSync.Write((PCHAR)&InCarData, sizeof(stInCarData));
	g_RakClient2->Send(&bsVehicleSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
}

void RakClient2::SendFakePassengerSyncData(WORD vehicleid, float fPos[3], float HealthCar, uint8_t WeaponID, int SeatID, WORD key)
{
	struct actor_info* self = actor_info_get(ACTOR_SELF, ACTOR_ALIVE);
	stPassengerData PassData;
	ZeroMemory(&PassData, sizeof(stPassengerData));
	RakNet::BitStream bsVehicleSync;
	PassData.sVehicleID = vehicleid;

	PassData.fPosition[0] = fPos[0];
	PassData.fPosition[1] = fPos[1];
	PassData.fPosition[2] = fPos[2];
	PassData.byteCurrentWeapon = WeaponID;
	PassData.sKeys = key;
	PassData.byteSeatID = SeatID;
	PassData.byteHealth = self->hitpoints;
	PassData.byteArmor = self->armor;
	bsVehicleSync.Write((BYTE)ID_PASSENGER_SYNC);
	bsVehicleSync.Write((PCHAR)&PassData, sizeof(stPassengerData));
	g_RakClient2->Send(&bsVehicleSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
}
void RakClient2::SendUnoccupiedSyncData(WORD vehicleid, float fPos[3], float HealthCar, float Speed[3], int SeatID)
{
	stUnoccupiedData unSync;
	ZeroMemory(&unSync, sizeof(stUnoccupiedData));
	unSync.sVehicleID = vehicleid;

	unSync.fPosition[0] = fPos[0];
	unSync.fPosition[1] = fPos[1];
	unSync.fPosition[2] = fPos[2];

	unSync.fHealth = HealthCar;
	unSync.fMoveSpeed[0] = Speed[0];
	unSync.fMoveSpeed[1] = Speed[1];
	unSync.fMoveSpeed[2] = Speed[2];

	unSync.fRoll[0] = 1;
	unSync.fRoll[1] = 0;
	unSync.fRoll[2] = 0;

	unSync.fDirection[0] = 0;
	unSync.fDirection[1] = 1;
	unSync.fDirection[2] = 0;

	unSync.fTurnSpeed[0] = 0;
	unSync.fTurnSpeed[1] = 0;
	unSync.fTurnSpeed[2] = 0;

	unSync.byteSeatID = SeatID;
	RakNet::BitStream bsUnoccupiedSync;
	bsUnoccupiedSync.Write((BYTE)ID_UNOCCUPIED_SYNC);
	bsUnoccupiedSync.Write((PCHAR)&unSync, sizeof(stUnoccupiedData));
	g_RakClient2->Send(&bsUnoccupiedSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
}
void RakClient2::SendFakeTrailerData(WORD vehicleid, float fpos[3], float HealthCar, float Speed[3])
{

	stTrailerData trailer_sync;

	ZeroMemory(&trailer_sync, sizeof(stTrailerData));

	vect3_copy(fpos, trailer_sync.fPosition);
	vect3_copy(Speed, trailer_sync.fSpeed);

	trailer_sync.sTrailerID = vehicleid;

	RakNet::BitStream bsPackettrailer;
	bsPackettrailer.Write((BYTE)ID_TRAILER_SYNC);
	bsPackettrailer.Write((PCHAR)(&trailer_sync), sizeof(trailer_sync));
	g_RakClient2->Send(&bsPackettrailer, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
}

void RakClient2::RequestClass( int classId )
{
	BitStream bsClass;

	bsClass.Write( classId );
	this->RPC( RPC_RequestClass, &bsClass );
}

void RakClient2::SendSCMEvent( int vehicleID, int eventId, int param1, int param2 )
{
	BitStream bsScmEvent;

	bsScmEvent.Write ( vehicleID );
	bsScmEvent.Write ( param1 );
	bsScmEvent.Write ( param2 );
	bsScmEvent.Write ( eventId );

	this->RPC( RPC_SCMEvent, &bsScmEvent );
}

void RakClient2::SendSpawn( void )
{
	BitStream bsSpawn;

	this->RPC( RPC_RequestSpawn, &bsSpawn );
	this->RPC( RPC_Spawn, &bsSpawn );
}

void RakClient2::SendFakeSpecData(float fPos[3])
{
	traceLastFunc("SendFakeSpecData()");

	stSpectatorData specData;

	ZeroMemory(&specData, sizeof(stSpectatorData));
	specData.fPosition[0] = fPos[0];
	specData.fPosition[1] = fPos[1];
	specData.fPosition[2] = fPos[2];
	BitStream bsSpec;
	bsSpec.Write((BYTE)ID_SPECTATOR_SYNC);
	bsSpec.Write((PCHAR)&specData, sizeof(stSpectatorData));
	g_RakClient2->Send(&bsSpec, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
}

void RakClient2::SendFakeBulletData(uint16_t PlayerID, float Origin[3], float Target[3], float  Center[3], uint8_t WeaponID, BYTE type)
{
	traceLastFunc("SendFakeBulletData()");

	stBulletData bulletSync;
	ZeroMemory(&bulletSync, sizeof(stBulletData));

	bulletSync.sTargetID = PlayerID;

	bulletSync.fOrigin[0] = Origin[0];
	bulletSync.fOrigin[1] = Origin[1];
	bulletSync.fOrigin[2] = Origin[2];

	bulletSync.fTarget[0] = Target[0];
	bulletSync.fTarget[1] = Target[1];
	bulletSync.fTarget[2] = Target[2];

	bulletSync.fCenter[0] = Center[0];
	bulletSync.fCenter[1] = Center[1];
	bulletSync.fCenter[2] = Center[2];

	bulletSync.byteWeaponID = WeaponID;

	bulletSync.byteType = type;

	BitStream bsSync;
	bsSync.Write((BYTE)PacketEnumeration::ID_BULLET_SYNC);
	bsSync.Write((PCHAR)(&bulletSync), sizeof(stBulletData));
	g_RakClient2->Send(&bsSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
}

void RakClient2::SendFakeOnfootSyncData(float fPos[3], float fSpeed[3], float Health, float Armor, int AnimFlags, int AnimationID, float Quaternion[4])
{
	stOnFootData onSync;
	ZeroMemory(&onSync, sizeof(stOnFootData));

	onSync.byteHealth = Health;
	onSync.byteArmor = Armor;

	onSync.fPosition[0] = fPos[0];
	onSync.fPosition[1] = fPos[1];
	onSync.fPosition[2] = fPos[2];

	onSync.fMoveSpeed[0] = fSpeed[0];
	onSync.fMoveSpeed[1] = fSpeed[1];
	onSync.fMoveSpeed[2] = fSpeed[2];

	onSync.fQuaternion[0] = Quaternion[0];
	onSync.fQuaternion[1] = Quaternion[1];
	onSync.fQuaternion[2] = Quaternion[2];
	onSync.fQuaternion[3] = Quaternion[3];

	onSync.sAnimFlags = AnimFlags;
	onSync.sCurrentAnimationID = AnimationID;

	BitStream bsData;
	bsData.Write((BYTE)ID_PLAYER_SYNC);
	bsData.Write((PCHAR)&onSync, sizeof(stOnFootData));
	g_RakClient2->Send(&bsData, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
}

void RakClient2::SendFakeDriverSyncData_(int iSAMPVehicleID, float fVehiclePos[3], float fVehicleHealth, float fMoveSpeed[3])
{
	stInCarData InCarData;
	BitStream bsInCarData;

	struct actor_info* pSelfActor = actor_info_get(ACTOR_SELF, 0);

	InCarData.sVehicleID = iSAMPVehicleID;
	InCarData.fMoveSpeed[0] = fMoveSpeed[0];
	InCarData.fMoveSpeed[1] = fMoveSpeed[1];
	InCarData.fMoveSpeed[2] = fMoveSpeed[2];
	InCarData.fPosition[0] = fVehiclePos[0];
	InCarData.fPosition[1] = fVehiclePos[1];
	InCarData.fPosition[2] = fVehiclePos[2];
	InCarData.fVehicleHealth = fVehicleHealth;
	InCarData.bytePlayerHealth = (BYTE)pSelfActor->hitpoints;
	InCarData.byteArmor = (BYTE)pSelfActor->armor;

	if (cheat_state->state == CHEAT_STATE_VEHICLE)
		InCarData.byteCurrentWeapon = g_Players->pLocalPlayer->inCarData.byteCurrentWeapon;

	else if (cheat_state->state == CHEAT_STATE_ACTOR)
		InCarData.byteCurrentWeapon = g_Players->pLocalPlayer->onFootData.byteCurrentWeapon;

	else InCarData.byteCurrentWeapon = 0;

	bsInCarData.Write((BYTE)ID_VEHICLE_SYNC);
	bsInCarData.Write((PCHAR)&InCarData, sizeof(stInCarData));

	g_RakClient2->Send(&bsInCarData, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0);
}

void RakClient2::SendFakeDriverSyncData_H3X(USHORT VehicleID, float fPos[3], float HealthCar, float Speed[3], WORD key)
{
	stInCarData picSync;
	memset(&picSync, 0, sizeof(stInCarData));
	BitStream bsVehicleSync;
	picSync.sVehicleID = VehicleID;
	picSync.fPosition[0] = fPos[0];
	picSync.fPosition[1] = fPos[1];
	picSync.fPosition[2] = fPos[2];
	picSync.fVehicleHealth = HealthCar;
	picSync.fMoveSpeed[0] = Speed[0];
	picSync.fMoveSpeed[1] = Speed[1];
	picSync.fMoveSpeed[2] = Speed[2];
	picSync.sKeys = key;
	picSync.bytePlayerHealth = g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->hitpoints;
	picSync.byteArmor = g_Players->pLocalPlayer->pSAMP_Actor->pGTA_Ped->armor;
	bsVehicleSync.Write((BYTE)ID_VEHICLE_SYNC);
	bsVehicleSync.Write((PCHAR)&picSync, sizeof(stInCarData));
	g_RakClient2->Send(&bsVehicleSync);
}

void RakClient2::SendFakeUnnocopiedSyncData(float fPos[3], float fSpeed[3], float fTurnSpeed[3], float fHealth, int16_t sVehicleID)
{
	stUnoccupiedData onSync;
	ZeroMemory(&onSync, sizeof(stUnoccupiedData));

	onSync.fPosition[0] = fPos[0];
	onSync.fPosition[1] = fPos[1];
	onSync.fPosition[2] = fPos[2];
	onSync.fTurnSpeed[0] = fTurnSpeed[0];
	onSync.fTurnSpeed[1] = fTurnSpeed[1];
	onSync.fTurnSpeed[2] = fTurnSpeed[2];
	onSync.fHealth = fHealth;
	onSync.sVehicleID = sVehicleID;
	onSync.fMoveSpeed[0] = fSpeed[0];
	onSync.fMoveSpeed[1] = fSpeed[1];
	onSync.fMoveSpeed[2] = fSpeed[2];

	BitStream bsData;
	bsData.Write((BYTE)ID_UNOCCUPIED_SYNC);
	bsData.Write((PCHAR)&onSync, sizeof(stUnoccupiedData));
	g_RakClient2->Send(&bsData, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
}

void RakClient2::SendDamageVehicle(WORD vehicleID, DWORD panel, DWORD door, BYTE lights, BYTE tires)
{
	BitStream bsDamageVehicle;
	bsDamageVehicle.Write(vehicleID);
	bsDamageVehicle.Write(panel);
	bsDamageVehicle.Write(door);
	bsDamageVehicle.Write(lights);
	bsDamageVehicle.Write(tires);
	g_RakClient2->RPC(RPC_DamageVehicle, &bsDamageVehicle);
}

void RakClient2::SendVehicleDamageComponents(int vehicleID)
{
	BitStream bStream;

	uint32_t vehPanelStatus = encode_panels(3, 3, 3, 3, 3, 3, 3);
	uint32_t vehDoorStatus = encode_doors(4, 4, 4, 4);
	uint8_t vehLightStatus = 0;
	uint8_t vehTireStatus = 0;

	bStream.Write(vehicleID);
	bStream.Write((uint32_t)vehPanelStatus);
	bStream.Write((uint32_t)vehDoorStatus);
	bStream.Write((uint8_t)vehLightStatus);
	bStream.Write((uint8_t)vehTireStatus);

	g_RakClient2->RPC(RPC_DamageVehicle, &bStream);
}