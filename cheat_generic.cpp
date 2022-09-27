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

int cheat_panic ( void )
{
	traceLastFunc( "cheat_panic()" );

	static int
		pstate_actor_hp_nSP = 0, // "Extra actor invincibility" patch (invulnerable single player enemies)
		pstate_actor_hp_SP = 0, // "Extra actor invincibility2" patch (no problems in SP)
		pstate_vehicle_hp = 0, // vehicle hp patch
		pstate_generic_menu = 0, // m0d's menu
		pstate_infnos = 0, // infinite NOS
		pstate_ini[INI_PATCHES_MAX],
		pstate_dummy_aligner; // this should all be a structure, static = DOING IT WRONG
	int			i;

	// sa-mp related
	static int	pstate_chat = 0, // chat
		pstate_deathlist = 0, // deathlist
		pstate_extragm = 0; // extra godmode

	if ( KEYCOMBO_PRESSED(set.key_panic) )
	{
		// toggle panic
		cheat_state->_generic.cheat_panic_enabled ^= 1;

		if ( cheat_state->_generic.cheat_panic_enabled )
		{
			struct actor_info	*actor_info = actor_info_get( ACTOR_SELF, ACTOR_ALIVE );

			if ( actor_info )
			{
				if (cheat_state->_generic.hp_cheat)
					actor_info->flags &= ~ACTOR_FLAGS_INVULNERABLE;
				actor_info->weapon_slot = 0;
			}

			// remove infinite NOS
			pstate_infnos = cheat_state->vehicle.infNOS_toggle_on;
			cheat_state->vehicle.infNOS_toggle_on = false;
			patcher_remove( &patch_vehicle_inf_NOS );

			if ( pPedSelf->GetVehicle() )
			{
				CVehicle	*pVehicleSelf = pPedSelf->GetVehicle();
				CVehicle	*pVehicleTemp = NULL;

				for ( pVehicleTemp = pVehicleSelf; pVehicleTemp != NULL; pVehicleTemp = pVehicleTemp->GetTowedVehicle() )
				{
					pVehicleTemp->SetGravity( &CVector(0.0, 0.0, -1.0) );
					pVehicleTemp->SetCanBeDamaged( true );

					if ( !BlackLightFuncs->bVehicleTrailerSupport)
						break;
				}

				// remove inf NOS
				if ( pstate_infnos )
				{
					pVehicleSelf->RemoveVehicleUpgrade( 1010 );
					//pVehicleSelf->AddVehicleUpgrade( 1010 );
				}

				// reset overrideLights, pstate not needed, will be reactivated on demand
				if ( set.enable_car_lights_at_day_time )
					pVehicleSelf->SetOverrideLights( 0 );
			}

			// m0d's menu
			pstate_generic_menu = cheat_state->_generic.menu;
			menu_toggle(false);

			// remove "Extra actor invincibility" patch
			pstate_actor_hp_nSP = patch_actor_hp_extraInv.installed;
			patcher_remove( &patch_actor_hp_extraInv );
			pstate_actor_hp_SP = patch_actor_hp.installed;
			patcher_remove( &patch_actor_hp );

			// remove vehicle hp patch
			pstate_vehicle_hp = patch_vehicle_hp.installed;
			patcher_remove( &patch_vehicle_hp );

			// just call with null vehicle info to disable
			cheat_handle_vehicle_fly( NULL, 0.0f );

			for ( i = 0; i < INI_PATCHES_MAX; i++ )
			{
				// added to not remove volatile patches
				if ( !set.patch[i].has_volatile )
				{
					pstate_ini[i] = set.patch[i].installed;
					patcher_remove( &set.patch[i] );
				}
			}

			// turn off kill & chat
			if ( g_DeathList != NULL )
			{
				pstate_deathlist = g_DeathList->iEnabled;
				g_DeathList->iEnabled = 1;
			}
			if ( g_Chat != NULL )
			{
				pstate_chat = g_Chat->iChatWindowMode;
				g_Chat->iChatWindowMode = 2;
			}
			pstate_extragm = set.enable_extra_godmode;
			set.enable_extra_godmode = 0;
		}
		else
		{
			// restore infinite NOS
			if ( pstate_infnos )
			{
				cheat_state->vehicle.infNOS_toggle_on = true;
				patcher_install( &patch_vehicle_inf_NOS );
			}

			// vehicle stuff
			CPed	*pPedSelf = pPools->GetPedFromRef( CPOOLS_PED_SELF_REF );
			if ( pPedSelf->GetVehicle() )
			{
				CVehicle	*pVehicleSelf = pPedSelf->GetVehicle();
				// restore inf NOS
				if ( pstate_infnos )
				{
					pVehicleSelf->AddVehicleUpgrade( 1010 );
				}
			}


			// restore "Extra actor invincibility" patch
			if ( pstate_actor_hp_nSP )
				patcher_install( &patch_actor_hp_extraInv );
			if ( pstate_actor_hp_SP )
				patcher_install( &patch_actor_hp );

			// restore vehicle hp patch
			if ( pstate_vehicle_hp )
				patcher_install( &patch_vehicle_hp );

			menu_toggle(pstate_generic_menu != 0);

			// restore patches
			for ( i = 0; i < INI_PATCHES_MAX; i++ )
			{
				if ( pstate_ini[i] )
					patcher_install( &set.patch[i] );
			}

			// restore kill & chat
			if ( g_DeathList != NULL )
			{
				g_DeathList->iEnabled = pstate_deathlist;
			}
			if ( g_Chat != NULL )
			{
				g_Chat->iChatWindowMode = pstate_chat;
			}

			// restore extra invincibility mode state
			set.enable_extra_godmode = pstate_extragm;

			// clear cheat state text
			cheat_state_text( NULL );
		}
	}

	return cheat_state->_generic.cheat_panic_enabled;
}

/* XXX move to cheat_funcs.cpp? */
extern bool GTAfunc_RemoveActorFromCarAndPutAt_requestingTeleport;
void cheat_teleport ( const float pos[3], int interior_id )
{
	if ( cheat_state->state == CHEAT_STATE_ACTOR
		|| GTAfunc_RemoveActorFromCarAndPutAt_requestingTeleport )
	{
		GTAfunc_RemoveActorFromCarAndPutAt_requestingTeleport = false;
		cheat_actor_teleport( actor_info_get(ACTOR_SELF, 0), pos, interior_id );
	}
	else if ( cheat_state->state == CHEAT_STATE_VEHICLE ) //&& !set.teleport_slow )
		cheat_vehicle_teleport( vehicle_info_get(VEHICLE_SELF, 0), pos, interior_id );
	/*else if ( cheat_state->state == CHEAT_STATE_VEHICLE && set.teleport_slow )
	{
		vect3_copy(pos, cheat_state->vehicle.tele_coords);
		cheat_state->vehicle.tele_on = 1;
	}*/
}

void cheat_teleport_nearest_car ( void )
{
	int					id = vehicle_find_nearest( VEHICLE_ALIVE | VEHICLE_EMPTY );
	struct vehicle_info *info = vehicle_info_get( id, VEHICLE_ALIVE | VEHICLE_EMPTY );

	if ( id == -1 || info == NULL )
		return;

	cheat_teleport( &info->base.matrix[4 * 3], info->base.interior_id );
}

void cheat_handle_quickwarp(struct vehicle_info *vehicle_info, struct actor_info *actor_info)
{
	if (!BlackLightFuncs->bQuickWarp)
		return;

	if (KEYCOMBO_PRESSED(set.key_quickwarp))
	{
		CCam *pCam = pGame->GetCamera()->GetCam(pGame->GetCamera()->GetActiveCam());
		CVector camsrc(*pCam->GetSource()), src, target, tppos;
		CColPoint *pCollision = nullptr;
		CEntitySAInterface *pEntity = nullptr;

		if (pCam->GetMode() == MODE_AIMWEAPON || pCam->GetMode() == MODE_AIMWEAPON_ATTACHED || pCam->GetMode() == MODE_AIMWEAPON_FROMCAR)
		{
			// calculate target position by aim vector
			pGame->GetCamera()->Find3rdPersonCamTargetVector(700.0f, &camsrc, &src, &target);
		}
		else
		{
			// else by camera vector
			target = camsrc + *pCam->GetFront() * 700.0f;
		}

		// ignore self vehicle
		if (vehicle_info != nullptr)
		{
			*(::vehicle_info **)VAR_IgnoredEntity = vehicle_info;
		}

		if (GTAfunc_ProcessLineOfSight(&camsrc, &target, &pCollision, &pEntity, true, true, false, true, true, false, false, false))
		{
			tppos = *pCollision->GetPosition();
			tppos -= (*pCollision->GetNormal()) * 0.5f;
			
			if (pCollision->GetNormal()->fZ >= 0.5f || pPedSelf->GetAreaCode() != 0)
			{
				tppos.fZ += 1.0f;
				tppos.fZ = pGameInterface->GetWorld()->FindGroundZFor3DPosition(&tppos);
			}
			else
			{
				tppos.fZ = pGameInterface->GetWorld()->FindGroundZForPosition(tppos.fX, tppos.fY);
			}

			if (vehicle_info != nullptr)
			{
				// check for collision
				CVehicle *vehSelf = pPedSelf->GetVehicle();
				if (vehSelf)
				{
					pCollision->Destroy();
					// check for collision
					CVector vecVehicleGravity;
					vehSelf->GetGravity(&vecVehicleGravity);
					CVector vecVehicleAbove = (-vecVehicleGravity * 5.0f) + tppos;
					CVector vecVehicleBelow = (vecVehicleGravity * 5.0f) + tppos;
					bool bCollision = GTAfunc_ProcessLineOfSight(&vecVehicleAbove, &vecVehicleBelow, &pCollision, &pEntity,
																 true, false, false, true, true, false, false, false); // not checking for vehicle collisions
					if (bCollision && pCollision)
					{
						// set vehicle to same Up position has surface normal
						CMatrix matVehicleSelf;
						vehSelf->GetMatrix(&matVehicleSelf);
						CVector vecCollisionNormal = *pCollision->GetNormal();

						// get "down" from vehicle model
						CVector rotationAxis = matVehicleSelf.vUp;

						// normalize our vectors
						vecCollisionNormal.Normalize();
						rotationAxis.Normalize();

						// axis and rotation for gravity
						float	theta = acos(rotationAxis.DotProduct(&vecCollisionNormal));
						if (!near_zero(theta))
						{
							rotationAxis.CrossProduct(&vecCollisionNormal);
							rotationAxis.Normalize();
							rotationAxis.ZeroNearZero();
							matVehicleSelf = matVehicleSelf.Rotate(&rotationAxis, -theta);
						}

						// set the new matrix
						vehSelf->SetMatrix(&matVehicleSelf);

						// set pos floats for actual teleporting
						tppos.fX = pCollision->GetPosition()->fX;
						tppos.fY = pCollision->GetPosition()->fY;
						tppos.fZ = pCollision->GetPosition()->fZ + 1.0f; // should be enough to stay above the ground properly
					}
					else
					{
						tppos.fZ += 0.5f;
					}
					cheat_vehicle_teleport(vehicle_info, &tppos.fX, gta_interior_id_get(), true);
				}
			}
			else if (actor_info != nullptr)
			{
				tppos.fZ += 1.0f;
				cheat_actor_teleport(actor_info, &tppos.fX, gta_interior_id_get());
			}
			GTAfunc_TogglePlayerControllable(0);
			GTAfunc_LockActor(0);
		}
		if (pCollision != nullptr)
		{
			pCollision->Destroy();
		}
	}
}

void cheat_handle_misc ( void )
{
	if ( KEYCOMBO_PRESSED(set.key_map) )
	{
		BlackLightFuncs->bPlayersMap ^= 1; /* toggle minimap */
	}

	if ( KEYCOMBO_PRESSED(set.key_map_show_vehicles) )
	{
		BlackLightFuncs->bMapIncludeVehicles ^= 1;
	}

	if ( KEYCOMBO_PRESSED(set.key_render_player_tags) )
		BlackLightFuncs->bDefaultModNametags ^= 1;

	if ( KEYCOMBO_PRESSED(set.key_render_vehicle_tags) )
		BlackLightFuncs->bDefaultModVehicleTags ^= 1;

	/* time */
	if ( set.force_hour >= 0 )
		gta_time_hour_set( set.force_hour );

	/* weather */
	if ( set.force_weather >= 0 )
		gta_weather_state_set( set.force_weather );

	SetCloudsEnabled( set.enable_clouds );
}

void cheat_handle_spoof_weapon ( void )
{
	struct actor_info	*info = actor_info_get( ACTOR_SELF, ACTOR_ALIVE );

	/*struct samp_info *spl = samp_info_get();*/
	if ( info != NULL )
	{
		if ( cheat_state->_generic.spoof_weapon != -1 )
			info->weapon_hit_type = ( uint8_t ) cheat_state->_generic.spoof_weapon;
	}
}

void cheat_handle_weapon_disable ( void )
{
	struct actor_info	*self = actor_info_get( ACTOR_SELF, 0 );
	struct actor_info	*info;
	int					bad_weapon_ids[] =
	{
		16, /* Grenade */
		17, /* Teargas */
		18, /* Molotov Cocktail */
		35, /* Rocket Launcher */
		36, /* Heat Seeking RPG */
		39, /* Remote Explosives */
		40, /* Detonator */
		-1
	};
	int					i, j;

	if ( !cheat_state->_generic.weapon_disable )
		return;

	for ( i = 0; i < pool_actor->size; i++ )
	{
		if ( (info = actor_info_get(i, ACTOR_ALIVE)) == NULL )
			continue;

		if ( info == self )
			continue;

		if ( info->weapon_slot > 12 )
			continue;

		if ( (int)info->weapon[info->weapon_slot].id > 46 ) /* invalid weapon ID */
			goto bad_weapon;

		/* filter disallowed weapons */
		for ( j = 0; bad_weapon_ids[j] != -1; j++ )
		{
			if ( (int)info->weapon[info->weapon_slot].id == bad_weapon_ids[j] )
				goto bad_weapon;
		}

		continue;
bad_weapon: ;
		info->weapon_slot = 0;
	}
}

struct freeze_info
{
	int		set;
	float	matrix[16];
};

void cheat_handle_freeze_vehicles ( struct vehicle_info *vehicle_info, struct actor_info *actor_info )
{
	traceLastFunc( "cheat_handle_freeze_vehicles()" );

	static struct freeze_info	*freeze;
	struct vehicle_info			*info;
	float						*pos;
	int							i;

	if ( KEYCOMBO_PRESSED(set.key_vehicles_freeze) )
	{
		BlackLightFuncs->bVehicleFreeze ^= 1;
		if ( !BlackLightFuncs->bVehicleFreeze && freeze != NULL )
			memset( freeze, 0, pool_vehicle->size * sizeof(struct freeze_info) );
	}

	if ( vehicle_info != NULL )
		pos = &vehicle_info->base.matrix[4 * 3];
	else if ( actor_info != NULL )
		pos = &actor_info->base.matrix[4 * 3];
	else
		return;

	if ( freeze == NULL )	/* XXX free on exit */
	{
		freeze = (struct freeze_info *)malloc( pool_vehicle->size * sizeof(struct freeze_info) );
		if ( freeze == NULL )
			return;
		memset( freeze, 0, pool_vehicle->size * sizeof(struct freeze_info) );
	}

	if (BlackLightFuncs->bVehicleFreeze)
	{
		for ( i = 0; i < pool_vehicle->size; i++ )
		{
			if ( (info = vehicle_info_get(i, VEHICLE_ALIVE)) == NULL || info == vehicle_info )
			{
				if ( info == vehicle_info && freeze[i].set )
					cheat_vehicle_air_brake_set( 1 );	/* entered a frozen vehicle */
				freeze[i].set = 0;
				continue;
			}

			if ( freeze[i].set )
			{
				matrix_copy( freeze[i].matrix, info->base.matrix );
				vect3_mult( info->speed, 0.0f, info->speed );
				vect3_mult( info->spin, 0.0f, info->spin );
			}

			if ( vehicle_contains_trailer(vehicle_info, info) )
			{
				freeze[i].set = 0;
				continue;
			}

			if ( vect3_dist(pos, &info->base.matrix[4 * 3]) >= 80.0f )
			{
				freeze[i].set = 0;
				continue;
			}

			if ( freeze[i].set )
			{
				continue;
			}

			matrix_copy( info->base.matrix, freeze[i].matrix );
			freeze[i].set = 1;
		}
	}
}

void cheat_handle_hp ( struct vehicle_info *vehicle_info, struct actor_info *actor_info, float time_diff )
{
	traceLastFunc( "cheat_handle_hp()" );

	static float	prev_pos[3];

	if (KEYCOMBO_PRESSED(set.key_hp_cheat))
	{
		cheat_state->_generic.hp_cheat ^= 1;	/* toggle hp cheat */
		if (!cheat_state->_generic.hp_cheat)
		{
			struct actor_info *self = actor_info_get(ACTOR_SELF, 0);
			self->flags &= ~ACTOR_FLAGS_INVULNERABLE;
		}
	}

	// this will make SP enemies invulnerable
	// now checking for a valid SAMP game
	if (g_SAMP)
	{
		if ( cheat_state->_generic.hp_cheat && cheat_state->actor.invulnerable && !set.hp_disable_inv_sp_enemies )
			patcher_install( &patch_actor_hp_extraInv );
		else
			patcher_remove( &patch_actor_hp_extraInv );
	}
	
	if ( cheat_state->_generic.hp_cheat && cheat_state->actor.invulnerable )
		patcher_install( &patch_actor_hp );
	else
		patcher_remove( &patch_actor_hp );

	if ( cheat_state->_generic.hp_cheat && cheat_state->vehicle.invulnerable )
		patcher_install( &patch_vehicle_hp );
	else
		patcher_remove( &patch_vehicle_hp );

	if ( vehicle_info != NULL )
	{
		struct vehicle_info *info = vehicle_info;
		struct vehicle_info *temp;

		// actor_info is NULL, if vehicle_info is not ( we need the actor though )
		actor_info = actor_info_get( ACTOR_SELF, 0 );

		if ( cheat_state->_generic.hp_cheat )
		{

			for ( temp = info; temp != NULL; temp = temp->trailer )
			{
				if(temp == NULL) return;

				/* minimum hp */
				if ( cheat_state->vehicle.hp_minimum_on && temp->hitpoints < set.hp_minimum )
					temp->hitpoints = set.hp_minimum;

				if ( set.hp_keep_vehicleHPsane )
				{
					// actor_info was NULL duh
					if ( temp->passengers[0] == actor_info && temp->hitpoints > 1000.0f )
					{
						if ( set.hp_minimum <= 1000.0f )
							temp->hitpoints = 1000.0f;
						else if ( temp->hitpoints > set.hp_minimum )
							temp->hitpoints = set.hp_minimum;
					}
				}

				/* regen */
				if ( temp->hitpoints < 1000.0f )
					temp->hitpoints += time_diff * set.hp_regen;

				if ( cheat_state->vehicle.invulnerable )
				{
					temp->m_nVehicleFlags.bCanBeDamaged = false;
					//temp->flags |= VEHICLE_FLAGS_INVULNERABLE;	/* invulnerable to most things */
					temp->flags &= ~2; /* make sure we're not frozen :p */
					//if ( temp->pulling_truck != NULL )
					//	temp->hitpoints = 1000.0f;
				}
				else
				{
					temp->m_nVehicleFlags.bCanBeDamaged = true;
					//temp->flags &= ~VEHICLE_FLAGS_INVULNERABLE;
				}

				if ( cheat_state->vehicle.hp_tire_support )
					cheat_vehicle_tires_set( temp, 0 );

				/*
				if(cheat_state->vehicle.is_engine_on == 1)
					temp->m_nVehicleFlags.bEngineOn = 1;
				else if((cheat_state->vehicle.is_engine_on == 0))
					temp->m_nVehicleFlags.bEngineOn = 0;
				*/
				// temp->engine_state |= 16; // protect against engine stalls
				if ( !BlackLightFuncs->bVehicleTrailerSupport)
					break;
			}
		}
		else
		{
			for ( temp = info; temp != NULL; temp = temp->trailer )
			{
				if(temp == NULL) return;

				// HP cheat disabled - keep HP value sane
				if ( set.hp_keep_vehicleHPsane )
				{
					if ( temp->hitpoints > 1000.0f )
						temp->hitpoints = 1000.0f;
				}

				temp->m_nVehicleFlags.bCanBeDamaged = true;
				//temp->flags &= ~VEHICLE_FLAGS_INVULNERABLE;

				if ( !BlackLightFuncs->bVehicleTrailerSupport)
					break;
			}
		}
	}

	if ( actor_info != NULL )
	{
		struct actor_info	*info = actor_info;

		if ( cheat_state->_generic.hp_cheat )
		{
			if ( info->hitpoints < 100.0f )
				info->hitpoints += time_diff * set.hp_regen_onfoot;
		}

		if ( cheat_state->_generic.hp_cheat && cheat_state->actor.invulnerable )
		{
			// invulnerability is on - at least be
			//  invulnerable against non explosive projectiles
			info->flags |= 4;
			if ( set.hp_actor_explosion_inv )
				info->flags |= 8;
			if ( set.hp_actor_fall_inv )
				info->flags |= 64;
			if ( set.hp_actor_fire_inv )
				info->flags |= 128;
			//info->flags |= ACTOR_FLAGS_INVULNERABLE;
		}
	}
}

void cheat_handle_weapon ( void )
{
	struct actor_info	*actor_info = actor_info_get( ACTOR_SELF, ACTOR_ALIVE );
	int					i;

	if ( KEYCOMBO_PRESSED(set.key_weapon) )
		BlackLightFuncs->bWeaponsEnable ^= 1;	/* toggle weapon cheat */

	if (BlackLightFuncs->bWeaponsEnable)
	{
		if ( actor_info != NULL )
		{
			for ( i = 0; i < 13; i++ )
			{
				if ( set.weapon[i] == NULL )
					continue;

				if (BlackLightFuncs->bWeaponsAmmoRandom)
				{
					if ( actor_info->weapon[set.weapon[i]->slot].id != set.weapon[i]->id
					 ||	 (
							 actor_info->weapon[set.weapon[i]->slot].ammo == 0
					 &&	 actor_info->weapon[set.weapon[i]->slot].ammo_clip == 0
					 ) )
					{
						int randammo = ( rand() % set.ammo ) + 1;
						int randammoclip = ( rand() % set.ammo_clip ) + 1;
						gta_weapon_set( actor_info, set.weapon[i]->slot, set.weapon[i]->id, randammo, randammoclip );
					}
				}
				else
				{
					gta_weapon_set( actor_info, set.weapon[i]->slot, set.weapon[i]->id, set.ammo, set.ammo_clip );
				}
			}
		}
		else if ( !BlackLightFuncs->brestore_weapons_after_death)
		{
			BlackLightFuncs->bWeaponsEnable = 0;
		}
	}
}

void cheat_handle_teleport ( struct vehicle_info *vehicle_info, struct actor_info *actor_info, float time_diff )
{
	traceLastFunc( "cheat_handle_teleport()" );

	struct object_base	*base = ( vehicle_info != NULL ) ? &vehicle_info->base : &actor_info->base;
	int					i;

	// Set teleport coordinates
	for ( i = 0; i < TELEPORT_MAX; i++ )
	{
		if ( set.key_teleport_set[i].count == 0 )
			continue;
		if ( KEYCOMBO_PRESSED(set.key_teleport_set[i]) )
		{
			cheat_state->teleport[i].set = 1;
			matrix_copy( base->matrix, cheat_state->teleport[i].matrix );
			cheat_state->teleport[i].interior_id = gta_interior_id_get();
			cheat_state_text( "Teleport coordinates set." );
		}
	}

	// Teleport to stored coordinates
	for ( i = 0; i < TELEPORT_MAX; i++ )
	{
		if ( set.key_teleport[i].count == 0 )
			continue;
		if ( KEYCOMBO_PRESSED(set.key_teleport[i]) )
		{
			if ( cheat_state->teleport[i].set )
			{
				cheat_teleport_history_save();
				cheat_teleport( &cheat_state->teleport[i].matrix[4 * 3], cheat_state->teleport[i].interior_id );

				// when teleports are stored in-game, we have a copy of the matrix to preserve rotation, etc..
				matrix_copy( cheat_state->teleport[i].matrix, base->matrix );
			}
			else if ( !vect3_near_zero(set.teleport[i].pos) )
			{
				cheat_teleport_history_save();
				cheat_teleport( set.teleport[i].pos, set.teleport[i].interior_id );
			}
			else
			{
				cheat_state_text( "Teleport coordinates not set." );
				break;
			}
			break;
		}
	}

	if ( KEYCOMBO_PRESSED(set.key_teleport_hist) )
	{
		struct cheat_state_teleport *teleport = NULL;

		cheat_state->teleport_hist_count--;
		if ( cheat_state->teleport_hist_count < 0 )
			cheat_state->teleport_hist_count = 0;
		else
			teleport = &cheat_state->teleport_hist[cheat_state->teleport_hist_count];

		if ( teleport != NULL && teleport->set )
		{
			cheat_teleport( &teleport->matrix[4 * 3], teleport->interior_id );
			matrix_copy( teleport->matrix, base->matrix );
			teleport->set = 0;
		}
		else
		{
			cheat_state_text( "Teleport history list is empty." );
		}
	}
}

void cheat_handle_checkpoint ( void )
{
	if ( KEYCOMBO_PRESSED(set.key_checkpoint_1) || KEYCOMBO_PRESSED(set.key_checkpoint_2) )
	{
		int					n = KEYCOMBO_PRESSED( set.key_checkpoint_1 ) ? 0 : 1;
		struct checkpoint	*cp = gta_checkpoint_info_get( n );
		float				pos[3];

		if ( cp != NULL )
		{
			struct vehicle_info *vehicle_info = vehicle_info_get( VEHICLE_SELF, 0 );

			vect3_copy( cp->position, pos );
			pos[2] += 1.0f;
			cheat_teleport( pos, 0 );		/* XXX interior id? */

			if ( vehicle_info != NULL )
				vehicle_prevent_below_height( vehicle_info, set.checkpoint_min_height );
		}
		else
		{
			cheat_state_text( "Checkpoint does not exist." );
		}
	}
}

void cheat_handle_unfreeze ( struct vehicle_info *vehicle_info, struct actor_info *actor_info, float time_diff )
{
	traceLastFunc( "cheat_handle_unfreeze()" );

	if ( KEYCOMBO_PRESSED(set.key_anti_freeze) )
	{
		GTAfunc_TogglePlayerControllable(0);
		GTAfunc_LockActor(0);
		pGameInterface->GetCamera()->RestoreWithJumpCut();
		
		// stop all animations
		if ( actor_info != NULL && !actor_info->pedFlags.bInVehicle )
			GTAfunc_DisembarkInstantly();
	}
}

void cheat_handle_emo ( struct vehicle_info *vehicle_info, struct actor_info *actor_info, float time_diff )
{
	traceLastFunc( "cheat_handle_emo()" );

	struct vehicle_info *vtemp;

	if ( !isBadPtr_GTA_pPed(actor_info) )
	{
		if ( KEYCOMBO_PRESSED(set.key_self_destruct) )
			actor_info->hitpoints = 0.0f;
	}
	else if ( !isBadPtr_GTA_pVehicle(vehicle_info) )
	{
		actor_info = actor_info_get(ACTOR_SELF, 0);
		if ( actor_info->state == ACTOR_STATE_DRIVING && actor_info->vehicle->passengers[0] == actor_info )
		{
			if ( KEYCOMBO_PRESSED(set.key_self_destruct) )
			{
				for ( vtemp = vehicle_info; vtemp != NULL; vtemp = vtemp->trailer )
				{
					if(vtemp == NULL) return;

					vtemp->hitpoints = 1.0f;
					cheat_vehicle_tires_set( vtemp, 1 );
					if ( !BlackLightFuncs->bVehicleTrailerSupport)
						break;
				}
			}
		}
	}
}

void cheat_handle_exit_vehicle ( struct vehicle_info *vehicle_info, struct actor_info *actor_info )
{
	if (!isBadPtr_GTA_pVehicle(vehicle_info))
	{
		if (vehicle_info != cheat_state->_generic.pVehicleExit_Last)
		{
			if (!isBadPtr_GTA_pVehicle(cheat_state->_generic.pVehicleExit_Last))
			{
				cheat_state->_generic.pVehicleExit_Last->m_nVehicleFlags.bCanBeDamaged = true;
			}
			cheat_state->_generic.pVehicleExit_Last = vehicle_info;
		}
	}
	if (!isBadPtr_GTA_pPed(actor_info))
	{
		if (!isBadPtr_GTA_pVehicle(cheat_state->_generic.pVehicleExit_Last))
		{
			cheat_state->_generic.pVehicleExit_Last->m_nVehicleFlags.bCanBeDamaged = true;
			cheat_state->_generic.pVehicleExit_Last = NULL;
		}
	}
}
