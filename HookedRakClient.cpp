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

bool HookedRakClientInterface::RPC(int* uniqueID, BitStream *parameters, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp)
{
	traceLastFunc("HookedRakClientInterface::RPC(BitStream)");
	if (uniqueID != nullptr)
	{
		if (!OnSendRPC(*uniqueID, parameters, priority, reliability, orderingChannel, shiftTimestamp))
			return false;
	}
	return g_RakClient2->GetInterface()->RPC(uniqueID, parameters, priority, reliability, orderingChannel, shiftTimestamp);
}

bool HookedRakClientInterface::Send(BitStream * bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel)
{
	traceLastFunc("HookedRakClientInterface::Send(BitStream)");
	
	BYTE packetId;
	bitStream->Read(packetId);

	if (packetId == PacketEnumeration::ID_PLAYER_SYNC)
		if (!OnFootDataHook((stOnFootData*)(bitStream->GetData() + 1)))
			return 0;

	if (packetId == PacketEnumeration::ID_VEHICLE_SYNC)
		if (!IncarDataHook((stInCarData*)(bitStream->GetData() + 1)))
			return 0;

	if (packetId == PacketEnumeration::ID_UNOCCUPIED_SYNC)
		if (!UnOccupiedDataHook((stUnoccupiedData*)(bitStream->GetData() + 1)))
			return 0;

	if (packetId == PacketEnumeration::ID_PASSENGER_SYNC)
		if (!PassengerDataHook((stPassengerData*)(bitStream->GetData() + 1)))
			return 0;

	if (packetId == PacketEnumeration::ID_BULLET_SYNC)
		if (!BulletDataHook((stBulletData*)(bitStream->GetData() + 1)))
			return 0;
	
	if (bitStream != nullptr)
	{
		if (!OnSendPacket(bitStream, priority, reliability, orderingChannel))
			return false;
	}
	return g_RakClient2->GetInterface()->Send(bitStream, priority, reliability, orderingChannel);
}


BYTE GetPacketID(Packet* p)
{
	if (p == 0) return 255;
	return (unsigned char)p->data[0];
}

BYTE GetPacketID2(Packet2* p)
{
	if (p == 0) return 255;
	return (unsigned char)p->data[0];
}
Packet2 *HookedRakClientInterface::Receive(void)
{
	traceLastFunc("HookedRakClientInterface::Receive");
	Packet2 *p = g_RakClient2->GetInterface()->Receive();

	BYTE ReceivedPacketID = GetPacketID2(p);

	if (BlackLightFuncs->bAutoReconnect)
	{
		if (ReceivedPacketID == ID_CONNECTION_BANNED)
		{
			disconnect(set.BlackLight.auto_reconnect_delay);
			g_SAMP->iGameState = GAMESTATE_RESTARTING;
			g_SAMP->iGameState = GAMESTATE_AWAIT_JOIN;
		}
		else if (ReceivedPacketID == ID_CONNECTION_LOST)
		{
			disconnect(set.BlackLight.auto_reconnect_delay);
			g_SAMP->iGameState = GAMESTATE_RESTARTING;
			g_SAMP->iGameState = GAMESTATE_AWAIT_JOIN;

		}
	}

	if (ReceivedPacketID == ID_MARKERS_SYNC)
	{
		BitStream	bsMarkerSync(p->data, p->length, false);
		int			iNumberOfPlayers = 0;
		USHORT	playerID = USHORT(-1);
		short		sPosX, sPosY, sPosZ;
		bool		bIsPlayerActive = false;

		bsMarkerSync.IgnoreBits(8);
		bsMarkerSync.Read(iNumberOfPlayers);
		if (iNumberOfPlayers < 0 || iNumberOfPlayers > SAMP_MAX_PLAYERS)
			return p;
		//if (g_Players->pLocalPlayer->iIsActorAlive)
		//{
		for (int i = 0; i < iNumberOfPlayers; i++)
		{
			bsMarkerSync.Read(playerID);
			bsMarkerSync.ReadCompressed(bIsPlayerActive);
			if (!bIsPlayerActive)
				continue;
			bsMarkerSync.Read(sPosX);
			bsMarkerSync.Read(sPosY);
			bsMarkerSync.Read(sPosZ);
			if (isBadSAMPPlayerID(playerID))
				continue;
			g_stStreamedOutInfo.iPlayerID[playerID] = playerID;
			g_stStreamedOutInfo.fPlayerPos[playerID][0] = (float)sPosX;
			g_stStreamedOutInfo.fPlayerPos[playerID][1] = (float)sPosY;
			g_stStreamedOutInfo.fPlayerPos[playerID][2] = (float)sPosZ;

			pOutStreamPos[playerID].fpos[0] = (float)sPosX;
			pOutStreamPos[playerID].fpos[1] = (float)sPosY;
			pOutStreamPos[playerID].fpos[2] = (float)sPosZ;

		}
		//}
	}

	if (ReceivedPacketID == ID_AIM_SYNC && BlackLightFuncs->bAntiWeaponCrasher)
	{
		BitStream bsAimSync((unsigned char*)p->data, p->length, false);

		stAimData AimData;
		unsigned short PlayerID;

		bsAimSync.IgnoreBits(8);
		bsAimSync.Read(PlayerID);
		bsAimSync.Read((PCHAR)&AimData, sizeof(stAimData));

		float fPos[3] = { 0, 0, 0 };

		bool bInvalidData = false;
		int bGotPos = getPlayerPos(PlayerID, fPos);

		if (bGotPos && vect3_dist(fPos, AimData.vecAimPos) > 500.0f)
		{
			vect3_mult(AimData.vecAimPos, 0.0f, AimData.vecAimPos);
			bInvalidData = true;
		}

		for (int i = 0; i < 3; i++)
		{
			if (AimData.vecAimf1[i] < -9.9f || AimData.vecAimf1[i] > 9.9f)
			{
				AimData.vecAimf1[i] = 0.0f;
				bInvalidData = true;
			}
		}

		if (bInvalidData)
		{
			BitStream bsFix;
			bsFix.Write((BYTE)ID_AIM_SYNC);
			bsFix.Write(PlayerID);
			bsFix.Write((char*)&AimData, sizeof(stAimData));
			memcpy(p->data, bsFix.GetData(), p->length);
			cheat_state_text("Anti-Crasher: %s tried to send you invalid aim data.", getPlayerName(PlayerID));
		}
	}

	/*if (ReceivedPacketID == ID_BULLET_SYNC)
	{
		BitStream bsBulletSync((unsigned char*)p->data, p->length, false);
		stBulletData BulletData;
		unsigned short PlayerID;
		bsBulletSync.IgnoreBits(8);
		bsBulletSync.Read(PlayerID);
		bsBulletSync.Read((PCHAR)&BulletData, sizeof(stBulletData));

		if (NewFunctions->bBulletTracers)
		{
			pBulletTracers[PlayerID].add(PlayerID, BulletData.fOrigin[0], BulletData.fOrigin[1], BulletData.fOrigin[2],
				BulletData.fTarget[0], BulletData.fTarget[1], BulletData.fTarget[2]);
		}
	}*/
	return p;
}

bool HookedRakClientInterface::Connect(const char* host, unsigned short serverPort, unsigned short clientPort, unsigned int depreciated, int threadSleepTimer)
{
	traceLastFunc("HookedRakClientInterface::Connect");

	return g_RakClient2->GetInterface()->Connect(host, serverPort, clientPort, depreciated, threadSleepTimer);
}

void HookedRakClientInterface::Disconnect(unsigned int blockDuration, unsigned char orderingChannel)
{
	traceLastFunc("HookedRakClientInterface::Disconnect");

	g_RakClient2->GetInterface()->Disconnect(blockDuration, orderingChannel);
}

void HookedRakClientInterface::InitializeSecurity(const char *privKeyP, const char *privKeyQ)
{
	traceLastFunc("HookedRakClientInterface::InitializeSecurity");

	g_RakClient2->GetInterface()->InitializeSecurity(privKeyP, privKeyQ);
}

void HookedRakClientInterface::SetPassword(const char *_password)
{
	traceLastFunc("HookedRakClientInterface::SetPassword");

	g_RakClient2->GetInterface()->SetPassword(_password);
}

bool HookedRakClientInterface::HasPassword(void) const
{
	traceLastFunc("HookedRakClientInterface::HasPassword");

	return g_RakClient2->GetInterface()->HasPassword();
}

bool HookedRakClientInterface::Send(const char *data, const int length, PacketPriority priority, PacketReliability reliability, char orderingChannel)
{
	traceLastFunc("HookedRakClientInterface::Send");

	return g_RakClient2->GetInterface()->Send(data, length, priority, reliability, orderingChannel);
}

void HookedRakClientInterface::DeallocatePacket(Packet2 *packet)
{
	traceLastFunc("HookedRakClientInterface::DeallocatePacket");

	g_RakClient2->GetInterface()->DeallocatePacket(packet);
}

void HookedRakClientInterface::PingServer(void)
{
	traceLastFunc("HookedRakClientInterface::PingServer");

	g_RakClient2->GetInterface()->PingServer();
}

void HookedRakClientInterface::PingServer(const char* host, unsigned short serverPort, unsigned short clientPort, bool onlyReplyOnAcceptingConnections)
{
	traceLastFunc("HookedRakClientInterface::PingServer");

	g_RakClient2->GetInterface()->PingServer(host, serverPort, clientPort, onlyReplyOnAcceptingConnections);
}

int HookedRakClientInterface::GetAveragePing(void)
{
	traceLastFunc("HookedRakClientInterface::GetAveragePing");

	return g_RakClient2->GetInterface()->GetAveragePing();
}

int HookedRakClientInterface::GetLastPing(void) const
{
	traceLastFunc("HookedRakClientInterface::GetLastPing");

	return g_RakClient2->GetInterface()->GetLastPing();
}

int HookedRakClientInterface::GetLowestPing(void) const
{
	traceLastFunc("HookedRakClientInterface::GetLowestPing");

	return g_RakClient2->GetInterface()->GetLowestPing();
}

int HookedRakClientInterface::GetPlayerPing(const PlayerID playerId)
{
	traceLastFunc("HookedRakClientInterface::GetPlayerPing");

	return g_RakClient2->GetInterface()->GetPlayerPing(playerId);
}

void HookedRakClientInterface::StartOccasionalPing(void)
{
	traceLastFunc("HookedRakClientInterface::StartOccasionalPing");

	g_RakClient2->GetInterface()->StartOccasionalPing();
}

void HookedRakClientInterface::StopOccasionalPing(void)
{
	traceLastFunc("HookedRakClientInterface::StopOccasionalPing");

	g_RakClient2->GetInterface()->StopOccasionalPing();
}

bool HookedRakClientInterface::IsConnected(void) const
{
	traceLastFunc("HookedRakClientInterface::IsConnected");

	return g_RakClient2->GetInterface()->IsConnected();
}

unsigned int HookedRakClientInterface::GetSynchronizedRandomInteger(void) const
{
	traceLastFunc("HookedRakClientInterface::GetSynchronizedRandomInteger");

	return g_RakClient2->GetInterface()->GetSynchronizedRandomInteger();
}

bool HookedRakClientInterface::GenerateCompressionLayer(unsigned int inputFrequencyTable[256], bool inputLayer)
{
	traceLastFunc("HookedRakClientInterface::GenerateCompressionLayer");

	return g_RakClient2->GetInterface()->GenerateCompressionLayer(inputFrequencyTable, inputLayer);
}

bool HookedRakClientInterface::DeleteCompressionLayer(bool inputLayer)
{
	traceLastFunc("HookedRakClientInterface::DeleteCompressionLayer");

	return g_RakClient2->GetInterface()->DeleteCompressionLayer(inputLayer);
}

void HookedRakClientInterface::RegisterAsRemoteProcedureCall(int* uniqueID, void(*functionPointer) (RPCParameters *rpcParms))
{
	traceLastFunc("HookedRakClientInterface::RegisterAsRemoteProcedureCall");

	g_RakClient2->GetInterface()->RegisterAsRemoteProcedureCall(uniqueID, functionPointer);
}

void HookedRakClientInterface::RegisterClassMemberRPC(int* uniqueID, void *functionPointer)
{
	traceLastFunc("HookedRakClientInterface::RegisterClassMemberRPC");

	g_RakClient2->GetInterface()->RegisterClassMemberRPC(uniqueID, functionPointer);
}

void HookedRakClientInterface::UnregisterAsRemoteProcedureCall(int* uniqueID)
{
	traceLastFunc("HookedRakClientInterface::UnregisterAsRemoteProcedureCall");

	g_RakClient2->GetInterface()->UnregisterAsRemoteProcedureCall(uniqueID);
}

bool HookedRakClientInterface::RPC(int* uniqueID, const char *data, unsigned int bitLength, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp)
{
	traceLastFunc("HookedRakClientInterface::RPC");

	return g_RakClient2->GetInterface()->RPC(uniqueID, data, bitLength, priority, reliability, orderingChannel, shiftTimestamp);
}

void HookedRakClientInterface::SetTrackFrequencyTable(bool b)
{
	traceLastFunc("HookedRakClientInterface::SetTrackFrequencyTable");

	g_RakClient2->GetInterface()->SetTrackFrequencyTable(b);
}

bool HookedRakClientInterface::GetSendFrequencyTable(unsigned int outputFrequencyTable[256])
{
	traceLastFunc("HookedRakClientInterface::GetSendFrequencyTable");

	return g_RakClient2->GetInterface()->GetSendFrequencyTable(outputFrequencyTable);
}

float HookedRakClientInterface::GetCompressionRatio(void) const
{
	traceLastFunc("HookedRakClientInterface::GetCompressionRatio");

	return g_RakClient2->GetInterface()->GetCompressionRatio();
}

float HookedRakClientInterface::GetDecompressionRatio(void) const
{
	traceLastFunc("HookedRakClientInterface::GetDecompressionRatio");

	return g_RakClient2->GetInterface()->GetDecompressionRatio();
}

void HookedRakClientInterface::AttachPlugin(void *messageHandler)
{
	traceLastFunc("HookedRakClientInterface::AttachPlugin");

	g_RakClient2->GetInterface()->AttachPlugin(messageHandler);
}

void HookedRakClientInterface::DetachPlugin(void *messageHandler)
{
	traceLastFunc("HookedRakClientInterface::DetachPlugin");

	g_RakClient2->GetInterface()->DetachPlugin(messageHandler);
}

BitStream * HookedRakClientInterface::GetStaticServerData(void)
{
	traceLastFunc("HookedRakClientInterface::GetStaticServerData");

	return g_RakClient2->GetInterface()->GetStaticServerData();
}

void HookedRakClientInterface::SetStaticServerData(const char *data, const int length)
{
	traceLastFunc("HookedRakClientInterface::SetStaticServerData");

	g_RakClient2->GetInterface()->SetStaticServerData(data, length);
}

BitStream * HookedRakClientInterface::GetStaticClientData(const PlayerID playerId)
{
	traceLastFunc("HookedRakClientInterface::GetStaticClientData");

	return g_RakClient2->GetInterface()->GetStaticClientData(playerId);
}

void HookedRakClientInterface::SetStaticClientData(const PlayerID playerId, const char *data, const int length)
{
	traceLastFunc("HookedRakClientInterface::SetStaticClientData");

	g_RakClient2->GetInterface()->SetStaticClientData(playerId, data, length);
}

void HookedRakClientInterface::SendStaticClientDataToServer(void)
{
	traceLastFunc("HookedRakClientInterface::SendStaticClientDataToServer");

	g_RakClient2->GetInterface()->SendStaticClientDataToServer();
}

PlayerID HookedRakClientInterface::GetServerID(void) const
{
	traceLastFunc("HookedRakClientInterface::GetServerID");

	return g_RakClient2->GetInterface()->GetServerID();
}

PlayerID HookedRakClientInterface::GetPlayerID(void) const
{
	traceLastFunc("HookedRakClientInterface::GetPlayerID");

	return g_RakClient2->GetInterface()->GetPlayerID();
}

PlayerID HookedRakClientInterface::GetInternalID(void) const
{
	traceLastFunc("HookedRakClientInterface::GetInternalID");

	return g_RakClient2->GetInterface()->GetInternalID();
}

const char* HookedRakClientInterface::PlayerIDToDottedIP(const PlayerID playerId) const
{
	traceLastFunc("HookedRakClientInterface::PlayerIDToDottedIP");

	return g_RakClient2->GetInterface()->PlayerIDToDottedIP(playerId);
}

void HookedRakClientInterface::PushBackPacket(Packet2 *packet, bool pushAtHead)
{
	traceLastFunc("HookedRakClientInterface::PushBackPacket");

	g_RakClient2->GetInterface()->PushBackPacket(packet, pushAtHead);
}

void HookedRakClientInterface::SetRouterInterface(void *routerInterface)
{
	traceLastFunc("HookedRakClientInterface::SetRouterInterface");

	g_RakClient2->GetInterface()->SetRouterInterface(routerInterface);
}
void HookedRakClientInterface::RemoveRouterInterface(void *routerInterface)
{
	traceLastFunc("HookedRakClientInterface::RemoveRouterInterface");

	g_RakClient2->GetInterface()->RemoveRouterInterface(routerInterface);
}

void HookedRakClientInterface::SetTimeoutTime(RakNetTime timeMS)
{
	traceLastFunc("HookedRakClientInterface::SetTimeoutTime");

	g_RakClient2->GetInterface()->SetTimeoutTime(timeMS);
}

bool HookedRakClientInterface::SetMTUSize(int size)
{
	traceLastFunc("HookedRakClientInterface::SetMTUSize");

	return g_RakClient2->GetInterface()->SetMTUSize(size);
}

int HookedRakClientInterface::GetMTUSize(void) const
{
	traceLastFunc("HookedRakClientInterface::GetMTUSize");

	return g_RakClient2->GetInterface()->GetMTUSize();
}

void HookedRakClientInterface::AllowConnectionResponseIPMigration(bool allow)
{
	traceLastFunc("HookedRakClientInterface::AllowConnectionResponseIPMigration");

	g_RakClient2->GetInterface()->AllowConnectionResponseIPMigration(allow);
}

void HookedRakClientInterface::AdvertiseSystem(const char *host, unsigned short remotePort, const char *data, int dataLength)
{
	traceLastFunc("HookedRakClientInterface::AdvertiseSystem");

	g_RakClient2->GetInterface()->AdvertiseSystem(host, remotePort, data, dataLength);
}

RakNetStatisticsStruct* const HookedRakClientInterface::GetStatistics(void)
{
	traceLastFunc("HookedRakClientInterface::GetStatistics");

	return g_RakClient2->GetInterface()->GetStatistics();
}

void HookedRakClientInterface::ApplyNetworkSimulator(double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance)
{
	traceLastFunc("HookedRakClientInterface::ApplyNetworkSimulator");

	g_RakClient2->GetInterface()->ApplyNetworkSimulator(maxSendBPS, minExtraPing, extraPingVariance);
}

bool HookedRakClientInterface::IsNetworkSimulatorActive(void)
{
	traceLastFunc("HookedRakClientInterface::IsNetworkSimulatorActive");

	return g_RakClient2->GetInterface()->IsNetworkSimulatorActive();
}

PlayerIndex HookedRakClientInterface::GetPlayerIndex(void)
{
	traceLastFunc("HookedRakClientInterface::GetPlayerIndex");

	return g_RakClient2->GetInterface()->GetPlayerIndex();
}

bool HookedRakClientInterface::RPC_(int* uniqueID, BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp, NetworkID networkID)
{
	traceLastFunc("HookedRakClientInterface::RPC_");

	return g_RakClient2->GetInterface()->RPC_(uniqueID, bitStream, priority, reliability, orderingChannel, shiftTimestamp, networkID);
}