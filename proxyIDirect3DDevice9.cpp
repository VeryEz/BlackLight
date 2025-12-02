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

// externals
unsigned long	ulFullScreenRefreshRate;
extern D3DC9	orig_Direct3DCreate9 = NULL;;

// externals
#pragma data_seg( ".d3d9_shared" )
proxyIDirect3DDevice9	*pDirect3DDevice9 = NULL;
HRESULT_VOID			orig_Direct3DShaderValidatorCreate9 = NULL;
IDirect3DDevice9		*origIDirect3DDevice9;
IDirect3DDevice9		*pRwD3DDevice = (IDirect3DDevice9 *) * (DWORD *)__RwD3DDevice;
IDirect3DTexture9		*pSpriteTexture;
D3DPRESENT_PARAMETERS	pPresentParam;
D3DMATRIX				m_mViewMatrix, m_mProjMatrix, m_mWorldMatrix;
CDirect3DData			*pD3DData =	new CDirect3DData();
#pragma data_seg()
bool					bD3DRenderInit;
bool					bD3DWindowModeSet;
bool					g_isRequestingWindowModeToggle;
bool					g_isRequesting_RwD3D9ChangeVideoMode;

IDirect3DPixelShader9	*chams_green;
IDirect3DPixelShader9	*chams_blue;
IDirect3DPixelShader9	*chams_red;

D3DXVECTOR3				speedoPos;
D3DXVECTOR2				needlePos;

IDirect3DTexture9		*tSpeedoPNG;
ID3DXSprite				*sSpeedoPNG;
IDirect3DTexture9		*tNeedlePNG;
ID3DXSprite				*sNeedlePNG;

// create a render object
CD3DRender				*render = new CD3DRender( 128 );

// create font objects

// also HUD somehow, the HUD comment below isn't totally right
CD3DFont				*pD3DFont = new CD3DFont( "Small Fonts", 10, FCR_BOLD );

//pd3dFont_sampStuff = player info list, player score list, player ESP
CD3DFont				*pD3DFont_sampStuff = new CD3DFont( "Small Fonts", 9, FCR_BOLD);

//pD3DFontFixed = cheat_state_msg, HUD
CD3DFont				*pD3DFontFixed = new CD3DFont( "Small Fonts", 8, FCR_BOLD);

//pD3DFontFixedSmall = health under bars (cars, players), vehicle ESP
CD3DFont				*pD3DFontFixedSmall = new CD3DFont( "Small Fonts", 8, FCR_BOLD);

//pD3DFontChat = chat, kill list
//CD3DFont				*pD3DFontChat = new CD3DFont( "Tahoma", 10, FCR_NONE );
CD3DFont				*pD3DFontChat = new CD3DFont( "Tahoma", 11, FCR_BOLD );

struct gui				*hud_bar = &set.guiset[0];
struct gui				*menu_titlebar_background = &set.guiset[2];
struct gui				*menu_background = &set.guiset[3];
struct gui				*menu_selected_item_bar = &set.guiset[4];
struct gui				*menu_selected_item_text = &set.guiset[5];

struct gui				*gta_hp_bar = &set.guiset[6];
struct gui				*gta_money_hud = &set.guiset[7];

extern int				iClickWarpEnabled;

ImFont* pFontArial, *pFontArialV2,*pFontSmall, * pFontSpeedmeter;

///////////////////////////////////////////////////////////////////////////////
// Common D3D functions.
///////////////////////////////////////////////////////////////////////////////

static WCHAR *ToWChar ( char *str )
{
	traceLastFunc( "ToWChar()" );

	static WCHAR	buffer[1024];
	_wcsset( buffer, 0 );
	MultiByteToWideChar( CP_ACP, 0, str, strlen(str), buffer, 1024 );
	return buffer;
}

// by s0beit, GHOSTER, Azorbix
HRESULT GenerateShader ( IDirect3DDevice9 *Device, IDirect3DPixelShader9 **pShader, float alpha, float red, float green,
						 float blue )
{
	traceLastFunc( "GenerateShader()" );

	char		szShader[256];
	ID3DXBuffer *pShaderBuffer = NULL;
	sprintf_s( szShader, sizeof(szShader), "ps.1.1\ndef c0, %f, %f, %f, %f\nmov r0,c0", red, green, blue, alpha );
	if ( FAILED(D3DXAssembleShader(szShader, sizeof(szShader), NULL, NULL, 0, &pShaderBuffer, NULL)) )
	{
		// Log( "Shader fail." ); - yeah it does sometimes
		return E_FAIL;
	}

	if ( FAILED(Device->CreatePixelShader((const DWORD *)pShaderBuffer->GetBufferPointer(), pShader)) )
	{
		return E_FAIL;
	}

	return S_OK;
}

void LoadSpriteTexture ( void )
{
	traceLastFunc( "LoadSpriteTexture()" );

	char			filename[MAX_PATH];
	D3DLOCKED_RECT	d3dlr;
	FILE			*fd = NULL;
	int				x, y;
	uint8_t			*surface;

	SAFE_RELEASE( pSpriteTexture );
	pSpriteTexture = NULL;

	/* XXX use a different texture format */
	if ( FAILED(origIDirect3DDevice9->CreateTexture(256, 256, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pSpriteTexture,
				 NULL)) )
	{
		Log( "Failed to create sprite texture!" );
		goto out;
	}

	snprintf( filename, sizeof(filename), "%s\\" M0D_FOLDER "%s", g_szWorkingDirectory, RAW_TEX_FILE );
	if ( (fd = fopen(filename, "rb")) == NULL )
	{
		Log( "Failed to open %s!", filename );
		goto out;
	}

	if ( FAILED(pSpriteTexture->LockRect(0, &d3dlr, 0, 0)) )
	{
		Log( "Failed to lock texture!" );
		goto out;
	}

	surface = (uint8_t *)d3dlr.pBits;

	for ( y = 0; y < 120; y++ )
	{
		uint8_t data[240];
		fread( data, 240, 1, fd );

		for ( x = 0; x < 240; x++ )
		{
			uint8_t alpha = ( data[x] & 7 ) << 5;
			uint8_t value = data[x] &~7;

			surface[x * 4 + 0] = value;
			surface[x * 4 + 1] = value;
			surface[x * 4 + 2] = value;
			surface[x * 4 + 3] = alpha;
		}

		surface += d3dlr.Pitch;
	}

	pSpriteTexture->UnlockRect( 0 );

	fclose( fd );

	//Log("Sprite texture loaded.");
	return;

out: ;
	SAFE_RELEASE( pSpriteTexture ) if ( fd != NULL )
		fclose( fd );
	return;
}

void RenderMenu ( void )
{
	traceLastFunc( "RenderMenu()" );

	static const int	ROW_HEIGHT = (int)ceilf( pD3DFont->DrawHeight() );
	static const int	MENU_HEIGHT = (int)ceilf( pD3DFont->DrawHeight() * (float)MENU_ROWS ) + 2;

	char				title[256] = NAME;
	struct menu_item	*item;
	struct menu			*menu;
	int					left, top;
	float				x, y;
	int					pos, top_pos, i;
	float				tdlen;

	if ( menu_active == NULL )
		return;

	/* find root menu */
	for ( menu = menu_active; menu->parent != NULL; menu = menu->parent );

	while ( menu != NULL )
	{
		if ( menu->pos < 0 || menu->pos >= menu->count )
			break;
		item = &menu->item[menu->pos];
		if ( item->submenu == NULL || menu == menu_active )
			break;
		snprintf( title + strlen(title), sizeof(title) - strlen(title), " > %s", item->name );
		menu = item->submenu;
	}

	/* draw titlebar */
	left = pPresentParam.BackBufferWidth / 2 - MENU_WIDTH / 2;
	top = pPresentParam.BackBufferHeight - MENU_HEIGHT - ROW_HEIGHT - 2 - 20 - 2;

	tdlen = pD3DFont->DrawLength(title);
	tdlen -= MENU_WIDTH;
	if (tdlen < 0.0f)
		tdlen = 0.0f;
	int cp = pD3DFont->GetCharPos(title, tdlen); // child p... char pos
	if (cp > 0) ++cp;
	render->D3DBoxBorderi( left, top, MENU_WIDTH, ROW_HEIGHT + 2,
						   D3DCOLOR_ARGB(223, 91, 91, 91), D3DCOLOR_ARGB(menu_titlebar_background->alpha,
						   menu_titlebar_background->red, menu_titlebar_background->green,
						   menu_titlebar_background->blue) );
	pD3DFont->PrintShadow( (float)(left + 1), (float)(top + 1), D3DCOLOR_XRGB(223, 223, 223), &title[cp] );

	/* draw window */
	left = pPresentParam.BackBufferWidth / 2 - MENU_WIDTH / 2;
	top = pPresentParam.BackBufferHeight - MENU_HEIGHT - 1 - 20;
	render->D3DBoxBorderi( left, top, MENU_WIDTH, MENU_HEIGHT, D3DCOLOR_ARGB(223, 63, 63, 63),
						   D3DCOLOR_ARGB(menu_background->alpha, menu_background->red, menu_background->green, menu_background->blue) );

	/* calculate positions */
	pos = menu_active->pos;
	top_pos = menu_active->top_pos;

	if ( pos - MENU_ROWS >= top_pos )
		top_pos = pos - MENU_ROWS + 1;
	if ( pos < top_pos )
		top_pos = pos;

	if ( top_pos < 0 )
		top_pos = 0;
	else if ( top_pos >= menu_active->count )
		top_pos = menu_active->count - 1;

	menu_active->top_pos = top_pos;

	/* draw menu items */
	y = (float)top;
	x = (float)( left + 2 );

	for ( i = top_pos; i < top_pos + MENU_ROWS; i++, y += pD3DFont->DrawHeight() )
	{
		int enabled;

		if ( i < 0 || i >= menu_active->count )
			continue;

		item = &menu_active->item[i];
		enabled = menu_active->callback( MENU_OP_ENABLED, item );

		if ( i == pos )
		{
			render->D3DBoxi( left + 1, (int)floorf(y) + 1, MENU_WIDTH - 1, ROW_HEIGHT + 1, D3DCOLOR_ARGB(
								 menu_selected_item_bar->alpha, menu_selected_item_bar->red,
							 menu_selected_item_bar->green, menu_selected_item_bar->blue), NULL );
		}

		if ( i == menu_mouseover )
		{
			render->D3DBoxi( left + 1, (int)floorf(y) + 1, MENU_WIDTH - 1, ROW_HEIGHT + 1, D3DCOLOR_ARGB(
								 menu_selected_item_bar->alpha / 2, menu_selected_item_bar->red,
							 menu_selected_item_bar->green, menu_selected_item_bar->blue), NULL );
		}

		if ( item->name[0] == '\t' )
		{
			pD3DFont->PrintShadow( x, y, item->color, item->name + 1 );
			render->D3DBoxi( left + 2, (int)floorf(y) + ROW_HEIGHT - 1, (int)ceilf(pD3DFont->DrawLength(item->name + 1)),
							 1, D3DCOLOR_ARGB(191, 127, 127, 127), NULL );
		}
		else
		{
			/* XXX ... */
			pD3DFont->PrintShadow( x, y, enabled ? D3DCOLOR_XRGB(menu_selected_item_text->red,
								   menu_selected_item_text->green, menu_selected_item_text->blue) : item->color,
								   item->name );
		}

		if ( item->submenu != NULL )
		{
			pD3DFont->PrintShadow( x + (float)MENU_WIDTH - pD3DFont->DrawLength(">") - 4, y,
								   D3DCOLOR_ARGB(191, 127, 127, 127), ">" );
		}
	}
}

static void RenderMapDot ( const float self_pos[3], const float pos[16], DWORD color, const char *name )
{
	traceLastFunc( "RenderMapDot()" );

	static int	init;
	float		vect[3], rvect[2];
	float		a, x, y;

	vect3_vect3_sub( pos, self_pos, vect );

	if ( vect3_length(vect) > 1000.0f )
		return;

	color = ( color & 0x00FFFFFF ) | 0xDF000000;

	a = -atan2f( cam_matrix[4 * 0 + 0], cam_matrix[4 * 0 + 1] ) - M_PI / 2.0f;

	rvect[0] = vect[0] * cosf( a ) - -vect[1] * sinf( a );
	rvect[1] = vect[0] * sinf( a ) + -vect[1] * cosf( a );

	rvect[1] /= pPresentParam.BackBufferWidth / pPresentParam.BackBufferHeight;

	x = (float)pPresentParam.BackBufferWidth / 2 + roundf( rvect[0] );
	y = (float)pPresentParam.BackBufferHeight / 2 + roundf( rvect[1] );
	if (x < 0.0f || x > pPresentParam.BackBufferWidth || y < 0.0f || y > pPresentParam.BackBufferHeight)
		return;
	if (set.map_draw_lines && vect3_length(vect) > 5.0f)
		render->D3DLine(x, y, pPresentParam.BackBufferWidth / 2, pPresentParam.BackBufferHeight / 2, color);

	/* the "^ v o" icons are 9x9 large, and located at 168x98 in the texture */
	if ( pSpriteTexture != NULL )
	{
		float	u, v, ts;

		if ( name != NULL )
			pD3DFontFixed->PrintShadow( floor(x - pD3DFontFixed->DrawLength(name) / 2.0f), floor(y - 14.0f), color, name );

		u = 168.0f;
		v = 98.0f;

		if ( vect[2] < -4.0f )
			u += 9.0f;
		else if ( vect[2] <= 4.0f )
			u += 18.0f;

		u /= 256.0f;
		v /= 256.0f;
		ts = 9.0f / 256.0f;

		render->D3DBindTexture( pSpriteTexture );

		render->D3DTexQuad( x - 4.5f, y - 4.5f, x + 4.5f, y + 4.5f, u, v, u + ts, v + ts );

		render->D3DBindTexture( NULL );
	}
	else
	{
		if ( name != NULL )
			pD3DFont->PrintShadow( floor(x - pD3DFontFixed->DrawLength(name) / 2.0f), floor(y - 14.0f), color, name );

		if ( vect[2] < -4.0f )
		{
			pD3DFont->PrintShadow( floor(x - 4.5f), floor(y - 4.5f), color, "v" );
			return;
		}
		else if ( vect[2] <= 4.0f )
		{
			pD3DFont->PrintShadow( floor(x - 4.5f), floor(y - 4.5f), color, "o" );
			return;
		}
		else if ( vect[2] > 4.0f )
		{
			pD3DFont->PrintShadow( floor(x - 4.5f), floor(y - 4.5f), color, "^" );
			return;
		}
	}
}

static void RenderMap ( void )
{
	traceLastFunc( "renderMap()" );

	// don't run in the menu
	if ( gta_menu_active() )
		return;

	if ( g_SAMP != NULL )
	{
		// showing scorelist?
		if ( (GetAsyncKeyState(VK_TAB) < 0)
		 ||	 g_Scoreboard->iIsEnabled ) 
			return;

		if ( GetAsyncKeyState(VK_F1) < 0 )
			return;
		if ( GetAsyncKeyState(VK_F5) < 0 )
			return;
		if ( GetAsyncKeyState(VK_F10) < 0 )
			return;
	}

	if (BlackLightFuncs->Menu.bImScoreboad || pSampMulti->IsMenuActive())
		return;

	struct actor_info	*self = actor_info_get( ACTOR_SELF, ACTOR_ALIVE );
	if ( self == NULL )
		return;

	const struct vehicle_entry	*vehicle;
	float						pos[3];
	char						buf[256];
	int							i;

	if ( pool_actor != NULL )
	{
		if ( g_SAMP != NULL )
		{
			for ( i = 0; i < SAMP_MAX_PLAYERS; i++ )
			{
				if ( !getPlayerPos(i, pos) )
					continue;
				if ( g_Players->pRemotePlayer[i] == NULL )
					continue;
				_snprintf_s( buf, sizeof(buf)-1, "%s(%d)", getPlayerName(i), i );
				RenderMapDot( &self->base.matrix[4 * 3], pos, samp_color_get(i), buf );
			}
		}
		else
		{
			CPoolsSA::pedPool_t::mapType::iterator it = ((CPoolsSA *)pGameInterface->GetPools())->m_pedPool.map.begin();
			for (; it.pos < it.end; it++)
			{
				CPedSA *ped = it.pos->second;
				if (ped == nullptr)
					continue;
				if (ped == pPedSelf)
					continue;
				_snprintf_s(buf, sizeof(buf)-1, "%d", ped->GetArrayID());
				RenderMapDot(&self->base.matrix[4 * 3], &ped->GetPosition()->fX, D3DCOLOR_XRGB(255, 255, 255), buf);
			}
		}
	}

	if (!BlackLightFuncs->bMapIncludeVehicles)
	{
		if ( g_SAMP != NULL )
		{
			if ( g_Vehicles != NULL )
			{
				for ( i = 0; i < SAMP_MAX_VEHICLES; i++ )
				{
					if ( g_Vehicles->iIsListed[i] != 1 )
						continue;
					if ( g_Vehicles->pSAMP_Vehicle[i] == NULL )
						continue;
					if ( g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle != NULL )
					{
						vehicle = gta_vehicle_get_by_id( g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle->base.model_alt_id );
						if ( g_Players->pLocalPlayer->sCurrentVehicleID == i )
							continue;

						RwColor color = getVehicleColorRGB(vehicle_getColor0(g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle));
						_snprintf_s( buf, sizeof(buf)-1, "%s(%d)", vehicle->name, i );
						RenderMapDot( &self->base.matrix[4 * 3],
									  &g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle->base.matrix[4 * 3],
									  D3DCOLOR_XRGB(color.r, color.g, color.b), buf );
					}
				}
			}
		}
		else
		{
			for ( i = 0; i < pool_vehicle->size; i++ )
			{
				struct vehicle_info *vehs = vehicle_info_get( i, VEHICLE_ALIVE );
				if ( vehs == NULL )
					continue;
				RwColor color = getVehicleColorRGB(vehicle_getColor0(vehs));
				vehicle = gta_vehicle_get_by_id( vehs->base.model_alt_id );
				_snprintf_s( buf, sizeof(buf)-1, "%s (%d)", vehicle->name, i );
				RenderMapDot( &self->base.matrix[4 * 3], &vehs->base.matrix[4 * 3], D3DCOLOR_XRGB(color.r, color.g, color.b), buf );
			}
		}
	}

	if (BlackLightFuncs->bTeleportTextSet == true)
	{
		for ( i = 0; i < TELEPORT_MAX; i++ )
		{
			vect3_copy( &cheat_state->teleport[i].matrix[4 * 3], pos );
			if ( vect3_near_zero(pos) )
				continue;
			_snprintf_s( buf, sizeof(buf)-1, "Teleport %d (%0.1f)", i, vect3_dist(&self->base.matrix[4 * 3], pos) );
			RenderMapDot( &self->base.matrix[4 * 3], pos, D3DCOLOR_XRGB(0, 200, 200), buf );
		}
	}
	
	// self
	RenderMapDot( &self->base.matrix[4 * 3], &self->base.matrix[4 * 3], D3DCOLOR_XRGB(255, 255, 255), NULL );
}

static void ShowSpeedoMeter(void)
{
	if (BlackLightFuncs->Menu.bImMenuNewSpeedometer)
		return;

	traceLastFunc("ShowSpeedoMeter()");

	struct actor_info* pinfo = actor_info_get(ACTOR_SELF, 0);
	struct vehicle_info* vinfo = vehicle_info_get(VEHICLE_SELF, 0);

	if (vinfo == NULL)
		return;
	if (pinfo == NULL)
		return;

	if (cheat_state->_generic.cheat_panic_enabled || g_Scoreboard->iIsEnabled)
		return;

	float				speed;

	// acceleration/distance speed
	static float	speed_last;
	static float	speed_lastVect[3];
	static float	speed_nowVect[3];
	static float	speed_dist;
	static float	speed_secondsLastCheck = 0.0f;
	static float	speed_now;
	static float	speed_acceleration;

	// update our data about position, speed, acceleration
	if ((TIME_TO_DOUBLE(time_get()) - speed_secondsLastCheck) > 0.10f)
	{
		////////////////////
		// distance speed //
		////////////////////
		vect3_copy(speed_nowVect, speed_lastVect);
		vect3_copy(&vinfo->base.matrix[4 * 3], speed_nowVect);
		speed_dist = vect3_dist(speed_lastVect, speed_nowVect) / (TIME_TO_DOUBLE(time_get()) - (float)speed_secondsLastCheck);

		//////////
		// m/ss //
		//////////
		speed_last = speed_now;
		speed_now = (vect3_length(vinfo->speed) * 170.0f) / 3.6f;
		speed_acceleration = (speed_now - speed_last) / (TIME_TO_DOUBLE(time_get()) - speed_secondsLastCheck);
		speed_secondsLastCheck = TIME_TO_DOUBLE(time_get());
	}

	// distance speed while air braking
	if (BlackLightFuncs->bAirbreakVehicle)
		speed = speed_dist * (3.43f / 170.0f);
	else
		speed = vect3_length(vinfo->speed);

	if (speed > 260.0f)
		speed = 260.0f;

	static float	mult = set.speedometer_multiplier;

	float			rotationNeedle = 0.0f;
	D3DXMATRIX		mat;

	rotationNeedle = DEGTORAD((90.0f / 100.0f) * ((speed * mult) * 1.55f));
	rotationNeedle /= 2;
	if (rotationNeedle > 3.29f)
		rotationNeedle = 3.29f;

	D3DXVECTOR2 axisSpeedo = D3DXVECTOR2(speedoPos.x, speedoPos.y);
	D3DXVECTOR2 axisNeedle = D3DXVECTOR2((130.00f * needlePos.x), (152.00f * needlePos.y));

	if (!gta_menu_active() && !KEY_DOWN(VK_TAB))
	{
		if ((sSpeedoPNG) && (tSpeedoPNG) && (sNeedlePNG) && (tNeedlePNG))
		{
			D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &needlePos, &axisNeedle, 0.0f, &axisSpeedo);
			sSpeedoPNG->Begin(D3DXSPRITE_ALPHABLEND);
			sSpeedoPNG->SetTransform(&mat);
			sSpeedoPNG->Draw(tSpeedoPNG, NULL, NULL, NULL, 0xCCFFFFFF);
			sSpeedoPNG->End();

			D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &needlePos, &axisNeedle, rotationNeedle, &axisSpeedo);
			sNeedlePNG->Begin(D3DXSPRITE_ALPHABLEND);
			sNeedlePNG->SetTransform(&mat);
			sNeedlePNG->Draw(tNeedlePNG, NULL, NULL, NULL, 0xCCFFFFFF);
			sNeedlePNG->End();
		}
	}
}

// for renderPlayerTags()
struct playerTagInfo
{
#pragma pack( 1 )
	CVector tagPosition;
	float	tagOffsetY;
	bool	isStairStacked;
	float	stairStackedOffset;
	bool	isPastMaxDistance;
} g_playerTagInfo[SAMP_MAX_PLAYERS];

// new "Air Ride" player ESP by nuckfuts
// this is optimized to all hell, so please don't
// mess with it unless you completely understand it
static void renderPlayerTags ( void )
{
	traceLastFunc( "renderPlayerTags()" );

	// don't run in the menu
	if ( gta_menu_active() )
		return;

	// Exit this function and enable samp nametags, if panic key
	if ( cheat_state->_generic.cheat_panic_enabled || !BlackLightFuncs->bDefaultModNametags)
	{
		if ( g_dwSAMP_Addr && g_SAMP )
		{
			sampPatchDisableNameTags( 0 );
		}
		return;
	}

	// don't run during certain samp events
	if ( g_dwSAMP_Addr && g_SAMP )
	{
		if (
			// Scoreboard open?
			( GetAsyncKeyState(VK_TAB) < 0)
			|| g_Scoreboard->iIsEnabled
			// F10 key down?
			|| GetAsyncKeyState(VK_F10) < 0
		)
		{
			return;
		}

		// Disable samp Nametags
		sampPatchDisableNameTags( 1 );
	}

	if (BlackLightFuncs->Menu.ESPMenus.bImBlackLightPlayersTags) //new special esp player tags
		return;

	// don't run if the CGameSA doesn't exist
	if ( !pGameInterface )
		return;

	// don't run if we don't exist
	if (isBadPtr_GTA_pPed(pPedSelf))
		return;

	// for tracking player states as we iterate through
	bool	isPedESPCollided[SAMP_MAX_PLAYERS];
	bool	isPedESPStairStacked[SAMP_MAX_PLAYERS];
	memset( isPedESPCollided, false, sizeof(bool) * SAMP_MAX_PLAYERS );
	memset( isPedESPStairStacked, true, sizeof(bool) * SAMP_MAX_PLAYERS );

	// alignment settings
	int			ESP_tag_player_pixelOffsetY = -10;
	float		ESP_tag_player_D3DBox_pixelOffsetX = -0.5f;
	float		ESP_tag_player_D3DBox_pixelOffsetY = 1.0f;
	float		ESP_tag_player_posOffsetZ = 1.0f;
	float		ESP_tag_player_espHeight = 20.0f;
	//float		ESP_tag_player_movementSpeed = 5.0f;

	// trash values to use during iterations
	float		vh, va;
	int			iGTAID, iGTAID_Inner, selfGTAID;
	CVector		iterPosition, ourPosMinusIter, ourPosition;
	D3DXVECTOR3 poss, screenposs;
	char		buf[256];

	// get our info
	if ( pPedSelf->GetVehicle() )
	{
		// RC Vehicle Fix (Not showing names of recently streamed in players
		// while being in a RC Vehicle)
		ourPosition = *(pPedSelf->GetVehicle()->GetPosition());
	}
	else
	{
		ourPosition = pPedSelfSA->Placeable.matrix->vPos;
	}

	// get our scripting ID so we can exclude ourself
	selfGTAID = (int)pPedSelf->GetArrayID();

	// setup iterator
	CPedSA		*iterPed = NULL;
	CPoolsSA	*pPools = reinterpret_cast < CPoolsSA * > ( pGameInterface->GetPools() );
	CPoolsSA::pedPool_t::mapType::iterator iter = pPools->m_pedPool.map.begin();

	// get initial variables for peds streamed in
	while ( iter.pos < iter.end )
	{
		// map iterator pointer to our pointer
		iterPed = iter.pos->second;

		// advance to next item for next pass
		iter.pos++;
		if ( !iterPed )
			continue;

		// get player id
		iGTAID = (int)iterPed->GetArrayID();

		// ignore if it's us
		if ( iGTAID == selfGTAID )
			continue;

		// RC Vehicle fix (not showing names of players in RC vehicles)
		if ( iterPed->GetVehicle() != NULL )
		{
			if ( gta_vehicle_get_by_id(iterPed->GetVehicle()->GetModelIndex())->class_id == VEHICLE_CLASS_MINI
				&& iterPed->GetVehicle()->GetDriver() == iterPed )
			{
				iterPosition = *iterPed->GetVehicle()->GetPosition();
			}
			else
			{
				iterPosition = iterPed->GetInterface()->Placeable.matrix->vPos;
			}
		}
		else
		{
			iterPosition = iterPed->GetInterface()->Placeable.matrix->vPos;
		}

		// check if it's farther than set.player_tags_dist
		ourPosMinusIter = ourPosition - iterPosition;
		if ( ourPosMinusIter.Length() > set.player_tags_dist )
		{
			g_playerTagInfo[iGTAID].isPastMaxDistance = true;
			continue;
		}
		else
			g_playerTagInfo[iGTAID].isPastMaxDistance = false;

		// get the player position in 2D
		poss.x = iterPosition.fX;
		poss.y = iterPosition.fY;
		poss.z = iterPosition.fZ + ESP_tag_player_posOffsetZ;
		pSampMulti->CalcScreenCoors( &poss, &screenposs );

		// check if the iter is culled or not
		if ( screenposs.z < 1.f )
		{
			g_playerTagInfo[iGTAID].tagOffsetY = 0.0f;
			g_playerTagInfo[iGTAID].isPastMaxDistance = true;
			continue;
		}

		// global, set ESP position for tagOffsetY use
		g_playerTagInfo[iGTAID].tagPosition.fX = screenposs.x;
		g_playerTagInfo[iGTAID].tagPosition.fY = screenposs.y;
		g_playerTagInfo[iGTAID].tagPosition.fZ = screenposs.z;
	}

	// reset iter position & setup iterInner
	iter = pPools->m_pedPool.map.begin();

	CPedSA	*iterInnerPed = NULL;
	CPoolsSA::pedPool_t::mapType::iterator iterInner;

	// remove staircase problem
	while ( iter.pos < iter.end )
	{
		// map iterator pointer to our pointer
		iterPed = iter.pos->second;

		// advance to next item for next pass
		iter.pos++;
		if ( !iterPed )
			continue;

		// get player id
		iGTAID = (int)iterPed->GetArrayID();

		// ignore if it's us
		if ( iGTAID == selfGTAID )
			continue;

		// filter out "ok" ESP
		if ( g_playerTagInfo[iGTAID].isPastMaxDistance
			||	!g_playerTagInfo[iGTAID].isStairStacked
				&& g_playerTagInfo[iGTAID].tagOffsetY < 40.0f
			)
			continue;

		// detect stair stacking per frame if ESP isn't already stair stacked
		if ( !g_playerTagInfo[iGTAID].isStairStacked )
		{
			// reset iterInner position
			iterInner = pPools->m_pedPool.map.begin();
			while ( iterInner.pos < iterInner.end )
			{
				// map iterator pointer to our pointer
				iterInnerPed = iterInner.pos->second;

				// advance to next item for next pass
				iterInner.pos++;
				if ( !iterInnerPed )
					continue;

				// get player id
				iGTAID_Inner = (int)iterInnerPed->GetArrayID();

				// ignore if it's us or isPastMaxDistance
				if ( g_playerTagInfo[iGTAID_Inner].isPastMaxDistance || iGTAID_Inner == iGTAID )
					continue;

				// test to see who comes out on top
				if ( abs(g_playerTagInfo[iGTAID].tagPosition.fX - g_playerTagInfo[iGTAID_Inner].tagPosition.fX) <= 100.0f
				 &&	 abs((g_playerTagInfo[iGTAID].tagPosition.fY - (g_playerTagInfo[iGTAID].tagOffsetY / 2.0f)) - (g_playerTagInfo[iGTAID_Inner].tagPosition.fY - g_playerTagInfo[iGTAID_Inner].tagOffsetY)) <= ESP_tag_player_espHeight )
				{
					isPedESPStairStacked[iGTAID] = false;
				}
			}

			// setup stair stack variables needed to un stack the ESP
			if ( isPedESPStairStacked[iGTAID] )
			{
				g_playerTagInfo[iGTAID].isStairStacked = true;
				g_playerTagInfo[iGTAID].stairStackedOffset = g_playerTagInfo[iGTAID].tagOffsetY / 2.0f;
			}
		}	// end inner while - detect stair stacking

		// lower the offsets for stair stacked ESP
		// and turn off stack status of ESP that reaches the "available" offset
		if ( g_playerTagInfo[iGTAID].isStairStacked )
		{
			g_playerTagInfo[iGTAID].tagOffsetY -= 5.0f;
			g_playerTagInfo[iGTAID].stairStackedOffset -= 5.0f;
			if ( g_playerTagInfo[iGTAID].stairStackedOffset < 5.0f )
			{
				g_playerTagInfo[iGTAID].stairStackedOffset = 0.0f;
				g_playerTagInfo[iGTAID].isStairStacked = false;
			}
		}
	}		// end outer while - remove staircase problem

	// reset iter position & setup iterInner
	iter = pPools->m_pedPool.map.begin();

	// detect & adjust for ESP collisions
	while ( iter.pos < iter.end )
	{
		// map iterator pointer to our pointer
		iterPed = iter.pos->second;

		// advance to next item for next pass
		iter.pos++;
		if ( !iterPed )
			continue;

		// get player id
		iGTAID = (int)iterPed->GetArrayID();

		// we isPastMaxDistance or stairstacked, move along
		if ( g_playerTagInfo[iGTAID].isPastMaxDistance || g_playerTagInfo[iGTAID].isStairStacked )
			continue;

		// reset iterInner position
		iterInner = pPools->m_pedPool.map.begin();
		while ( iterInner.pos < iterInner.end )
		{
			// map iterator pointer to our pointer
			iterInnerPed = iterInner.pos->second;

			// advance to next item for next pass
			iterInner.pos++;
			if ( !iterInnerPed )
				continue;

			// get player id
			iGTAID_Inner = (int)iterInnerPed->GetArrayID();

			// filter out isPastMaxDistance, stairstacked, and same Ped
			if ( g_playerTagInfo[iGTAID].isPastMaxDistance
			 ||	 g_playerTagInfo[iGTAID_Inner].isStairStacked
			 ||	 iGTAID == iGTAID_Inner ) continue;

			// player is within range, figure out if there's collision
			if ( abs(g_playerTagInfo[iGTAID].tagPosition.fX - g_playerTagInfo[iGTAID_Inner].tagPosition.fX) <= 100.0f
			 &&	 abs((g_playerTagInfo[iGTAID].tagPosition.fY - g_playerTagInfo[iGTAID].tagOffsetY) - (
						  g_playerTagInfo[iGTAID_Inner].tagPosition.fY - g_playerTagInfo[iGTAID_Inner].tagOffsetY)) <= ESP_tag_player_espHeight )
			{
				// collision, figure out who gets to stay
				if ( g_playerTagInfo[iGTAID].tagPosition.fZ < g_playerTagInfo[iGTAID_Inner].tagPosition.fZ )
				{
					// playerID "g_pTI_i" is farther, it should move up
					g_playerTagInfo[iGTAID_Inner].tagOffsetY += 5.0f;
					isPedESPCollided[iGTAID_Inner] = true;
				}
				else if ( g_playerTagInfo[iGTAID].tagPosition.fZ > g_playerTagInfo[iGTAID_Inner].tagPosition.fZ )
				{
					// playerID "i" is farther, it should move up
					// we should only need normal upward movement here
					g_playerTagInfo[iGTAID].tagOffsetY += 5.0f;
					isPedESPCollided[iGTAID] = true;
				}
				else
				{
					// both playerIDs are the same position @_@ so prefer the lower ID#
					if ( iGTAID < iGTAID_Inner )
					{
						g_playerTagInfo[iGTAID_Inner].tagOffsetY += 5.0f;
						isPedESPCollided[iGTAID_Inner] = true;
					}
					else
					{
						g_playerTagInfo[iGTAID].tagOffsetY += 5.0f;
						isPedESPCollided[iGTAID] = true;
					}
				}
			}

			// are we jigglin?  everybody likes to jiggle.
			if (	abs(g_playerTagInfo[iGTAID].tagPosition.fX - g_playerTagInfo[iGTAID_Inner].tagPosition.fX) <= 100.0f
					&& abs(
						(g_playerTagInfo[iGTAID].tagPosition.fY - g_playerTagInfo[iGTAID].tagOffsetY)
						- (g_playerTagInfo[iGTAID_Inner].tagPosition.fY - g_playerTagInfo[iGTAID_Inner].tagOffsetY)
					) - 5.0f <= ESP_tag_player_espHeight
				)
			{
				if ( g_playerTagInfo[iGTAID].tagPosition.fZ < g_playerTagInfo[iGTAID_Inner].tagPosition.fZ )
				{
					isPedESPCollided[iGTAID_Inner] = true;
				}
				else
				{
					isPedESPCollided[iGTAID] = true;
				}
			}
		}	// end inner while

		// return tagOffsetY to zero if needed
		if ( !isPedESPCollided[iGTAID] )
		{
			if ( g_playerTagInfo[iGTAID].tagOffsetY >= 5.0f )
			{
				g_playerTagInfo[iGTAID].tagOffsetY -= 5.0f;
			}
			else
			{
				g_playerTagInfo[iGTAID].tagOffsetY = 0.0f;
			}
		}
	}		// end outer while

	// reset iter position & setup iterInner
	iter = pPools->m_pedPool.map.begin();

	// start render ESP tags
	float h, playerBaseY;
	while ( iter.pos < iter.end )
	{
		// map iterator pointer to our pointer
		iterPed = iter.pos->second;

		// advance to next item for next pass
		iter.pos++;
		if ( !iterPed )
			continue;

		// get player id
		iGTAID = (int)iterPed->GetArrayID();

		// ignore if isPastMaxDistance or if it's us
		if ( g_playerTagInfo[iGTAID].isPastMaxDistance || iGTAID == selfGTAID )
			continue;

		playerBaseY = g_playerTagInfo[iGTAID].tagPosition.fY -
			g_playerTagInfo[iGTAID].tagOffsetY +
			ESP_tag_player_pixelOffsetY;

		int iSAMPID;
		if ( g_Players )
			iSAMPID = translateGTASAMP_pedPool.iSAMPID[getPedGTAIDFromInterface( (DWORD *)iterPed->GetPedInterface() )];

		// get Ped health
		// works in single player, but SAMP maintains its own player health
		//vh = iterPed->GetHealth();
		// get samp health
		if ( g_Players )
		{
			if ( g_Players->pRemotePlayer[iSAMPID] != NULL
				&&	 g_Players->pRemotePlayer[iSAMPID]->pPlayerData != NULL
				&&  g_Players->pRemotePlayer[iSAMPID]->pPlayerData->pSAMP_Actor != NULL
				&&  (DWORD)g_Players->pRemotePlayer[iSAMPID]->pPlayerData->pSAMP_Actor->pGTA_Ped == (DWORD)iterPed->GetPedInterface() )
			{
				vh = g_Players->pRemotePlayer[iSAMPID]->pPlayerData->fActorHealth;
				va = g_Players->pRemotePlayer[iSAMPID]->pPlayerData->fActorArmor;
			}
			else
			{
				// SA-MP running, but was not a remote player
				continue;
			}
		}
		else
		{
			// SA-MP not running or failed to initialize g_Players
			vh = iterPed->GetHealth();
			va = iterPed->GetArmor();
		}


		float hpbar = vh;
		if ( hpbar > 100.0f )
			hpbar = 100.0f;
		hpbar *= 70.0f / 100.0f;

		float offY = ESP_tag_player_D3DBox_pixelOffsetY;
		if ( va > 0.0f )
		{
			float armbar = va;
			if ( armbar > 100.0f )
				armbar = 100.0f;
			armbar *= 70.0f / 100.0f;
			//render->D3DBoxBorder( g_playerTagInfo[iGTAID].tagPosition.fX + ESP_tag_player_D3DBox_pixelOffsetX,
				//			playerBaseY + offY, 70.0f, 10.0f, D3DCOLOR_ARGB(255, 0, 0, 0), COLOR_WHITE(240));
			render->D3DBox( g_playerTagInfo[iGTAID].tagPosition.fX + 1.0f + ESP_tag_player_D3DBox_pixelOffsetX,
							playerBaseY + 1.0f + offY, armbar - 2.0f, 8.0f,
							COLOR_WHITE(240));
			offY += 9.0f;
		}

		//render->D3DBoxBorder( g_playerTagInfo[iGTAID].tagPosition.fX + ESP_tag_player_D3DBox_pixelOffsetX,
				//		playerBaseY + offY, 70.0f, 10.0f, D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(128, 80, 0, 0) );
		render->D3DBox( g_playerTagInfo[iGTAID].tagPosition.fX + 1.0f + ESP_tag_player_D3DBox_pixelOffsetX,
						playerBaseY + 1.0f + offY, hpbar - 2.0f, 8.0f, COLOR_RED(240) );

		if ((GoC->faker != stGoC::GOC_FAKER_NONE) && (iSAMPID == GoC->faker_target) 
			|| (GoC->faker != stGoC::GOC_FAKER_NONE) && (iSAMPID == set.fPlayerID))
		{
			// Define a static variable to keep track of the color phase
			static int rainbowPhase = 0;

			// Increment the rainbow phase and loop back to 0 after a full cycle
			rainbowPhase = (rainbowPhase + 1) % 360;

			// Convert the rainbow phase to a color using an HSV to RGB conversion for a smooth transition
			D3DCOLOR blicker_color = D3DCOLOR_XRGB(
				int(127.5f * (1 + sin(rainbowPhase * 3.14159f / 180.0f))),  // Red
				int(127.5f * (1 + sin((rainbowPhase + 120) * 3.14159f / 180.0f))),  // Green
				int(127.5f * (1 + sin((rainbowPhase + 240) * 3.14159f / 180.0f)))  // Blue
			);

			// Draw the D3DBox
			render->D3DBoxBorder(g_playerTagInfo[iGTAID].tagPosition.fX + 1.0f + ESP_tag_player_D3DBox_pixelOffsetX + 1.0f, playerBaseY - 40.0f + offY, 98.0f, 20.0f, D3DCOLOR_ARGB(255, 0, 0, 0), blicker_color);

			// Draw the text inside the D3DBox
			pD3DFont_sampStuff->PrintShadow(g_playerTagInfo[iGTAID].tagPosition.fX + ESP_tag_player_D3DBox_pixelOffsetX + 5.0f, // Adjusted X position to fit within the box
				playerBaseY - 40.0f + offY, // Adjusted Y position to fit within the box
				COLOR_WHITE(255), "GoC Target");
		}


		if (BlackLightFuncs->bFriendTags)
		{
			if (BlackLightFuncs->bIsFriend[iSAMPID])
			{
				pD3DFont_sampStuff->PrintShadow(g_playerTagInfo[iGTAID].tagPosition.fX + ESP_tag_player_D3DBox_pixelOffsetX + 55.0f,
					playerBaseY - 25.0f + offY, COLOR_GREEN(255), "Friend"); //40.0f

			}
		}
		if (BlackLightFuncs->bAdminsTags)
		{
			if (BlackLightFuncs->bIsAdmin[iSAMPID])
			{
				pD3DFont_sampStuff->PrintShadow(g_playerTagInfo[iGTAID].tagPosition.fX + 1.0f + ESP_tag_player_D3DBox_pixelOffsetX + 1.0f,
					playerBaseY - 25.0f + offY, COLOR_RED(255), "Admin"); // - 40.0f
			}
		}

		if(!g_BotFuncs->BotSettings.bShowBotInfoID || !BlackLightFuncs->Menu.ESPMenus.bImBlackLightPlayersInfo)
		if (g_BotFuncs->uiIsBot[iSAMPID][0] == 1)
		{
			pD3DFontFixedSmall->PrintShadow(g_playerTagInfo[iGTAID].tagPosition.fX - 10.0f + ESP_tag_player_D3DBox_pixelOffsetX, // + 55.0f
				playerBaseY + 10.0f + offY, COLOR_GREEN(255), pSampMulti->SetText("BOT ID: %d", g_BotFuncs->uiIsBot[iSAMPID][1])); // - 40.0f
		}

		// this should also calculate the anti-aliasing top edge somehow
		h = pD3DFont_sampStuff->DrawHeight() + 1;

		if (va > 0.0f)
			sprintf_s(buf, "%d {E00000}%d", (int)va, (int)vh);
		else
			sprintf_s(buf, "{E00000}%d", (int)vh);
		float w = pD3DFont_sampStuff->DrawLength(buf);
		pD3DFont_sampStuff->PrintShadow(g_playerTagInfo[iGTAID].tagPosition.fX + 70.0f - w - 1.0f, playerBaseY + 10.0f + offY,
										D3DCOLOR_ARGB(0xFF, 0xF0, 0xF0, 0xF0), buf);
		if ( g_Players )
		{
			// render the main nametag last so it's on top
			// this should calculate the anti-aliasing top edge somehow
			h = pD3DFont_sampStuff->DrawHeight() - 1;
			sprintf_s( buf, "%s (%d)", getPlayerName(iSAMPID), iSAMPID );
			pD3DFont_sampStuff->PrintShadow( g_playerTagInfo[iGTAID].tagPosition.fX, playerBaseY - h,
			samp_color_get( iSAMPID, 0xDD000000 ), buf);

			if (g_Players->pRemotePlayer[iSAMPID]->pPlayerData->iAFKState == 2)
			{
				pD3DFont_sampStuff->PrintShadow(g_playerTagInfo[iGTAID].tagPosition.fX + 1.0f, playerBaseY + 10.0f + offY,
												D3DCOLOR_ARGB(0xFF, 0xF0, 0xF0, 0xF0), "AFK");
			}
		}
	}
	// end render ESP tags
}

// render vehicle ESP
static void renderVehicleTags ( void )
{
	traceLastFunc( "renderVehicleTags()" );

	// don't run if the menu is active
	if ( gta_menu_active() )
		return;

	// no tags during panic attacks
	if ( cheat_state->_generic.cheat_panic_enabled )
		return;

	// don't run if the pools don't exist
	if ( !pGameInterface || !pGameInterface->GetPools() )
		return;

	// don't run if we don't exist
	if ( !pPedSelf )
		return;

	// don't display tags during certain key press & game events
	// g_dwSAMP_Addr will be set before sa-mp was completely initialized
	if	( g_SAMP && g_dwSAMP_Addr &&
			(
				(GetAsyncKeyState(VK_TAB) < 0)
				|| g_Scoreboard->iIsEnabled
				|| GetAsyncKeyState(VK_F10) < 0
			)
		)
		return;

	const vehicle_entry *vehicle;
	char				buf[256];
	int					ESP_tag_vehicle_pixelOffsetY = 0;
	float				ESP_tag_vehicle_D3DBox_pixelOffsetX = -0.5;
	float				ESP_tag_vehicle_D3DBox_pixelOffsetY = -0.5;
	float				ESP_tag_vehicle_posOffsetZ = -1.0f;
	int					v;
	float				w, h, vh;

	D3DXVECTOR3			poss, screenposs;
	D3DCOLOR			vcolor;
	CVector2D			screenPosition;
	CVector				ourPosition, iterPosition, ourPosMinusIter;

	// get our position
	if ( !isBadPtr_GTA_pVehicle(pPedSelf->GetVehicle()) )
	{
		// RC Vehicle Fix (Not showing names of recently streamed in players
		// while being in a RC Vehicle)
		ourPosition = *(pPedSelf->GetVehicle()->GetPosition());
	}
	else
	{
		ourPosition = pPedSelf->GetInterface()->Placeable.matrix->vPos;
	}

	// setup iterator
	CVehicleSA	*iterVehicle = NULL;
	CPoolsSA	*pPools = reinterpret_cast < CPoolsSA * > ( pGameInterface->GetPools() );
	CPoolsSA::vehiclePool_t::mapType::iterator iter = pPools->m_vehiclePool.map.begin();

	// iterate
	while ( iter.pos < iter.end )
	{
		// map iterator CVehicleSA pointer to our CVehicleSA pointer
		iterVehicle = iter.pos->second;

		// advance to next CVehicleSA for next pass
		iter.pos++;

		// move past null pointers
		if ( isBadPtr_GTA_pVehicle(iterVehicle) )
			continue;
		//if ( isBadPtr_GTA_pVehicle(iterVehicle->GetVehicleInterface()) )
		//	continue;

		// check if it's farther than set.vehicle_tags_dist
		iterPosition = iterVehicle->GetInterface()->Placeable.matrix->vPos;
		ourPosMinusIter = ourPosition - iterPosition;
		if ( ourPosMinusIter.Length() > set.vehicle_tags_dist )
			continue;

		// check if it's our vehicle
		if ( iterVehicle == pPedSelf->GetVehicle() )
			continue;

		// check if it's near zero
		if ( iterVehicle->GetPosition()->IsNearZero() )
			continue;

		// CVector to D3DXVECTOR3, function to be converted to CVector later
		poss.x = iterPosition.fX;
		poss.y = iterPosition.fY;
		poss.z = iterPosition.fZ + ESP_tag_vehicle_posOffsetZ;

		// yup
		pSampMulti->CalcScreenCoors( &poss, &screenposs );

		// check if the vehicle is culled or not
		if ( screenposs.z < 1.f )
			continue;

		// D3DXVECTOR3 to CVector2D
		screenPosition.fX = screenposs.x;
		screenPosition.fY = screenposs.y;

		// get the vehicle model's name
		vehicle = gta_vehicle_get_by_id( iterVehicle->GetModelIndex() );

		// get SAMP's vehicle id
		if ( g_Vehicles != NULL )
			v = translateGTASAMP_vehiclePool.iSAMPID[getVehicleGTAIDFromInterface( (DWORD *)iterVehicle->GetVehicleInterface() )];
		else
			v = (int)iterVehicle->GetArrayID();

		/////////////////
		// render time //
		h = pD3DFontFixed->DrawHeight();
		_snprintf_s( buf, sizeof(buf)-1, "%s (%d)", vehicle->name, v );
		w = pD3DFontFixed->DrawLength( buf );

		// get color by primary vehicle color from colors palette
		byte color[4];
		iterVehicle->GetColor(&color[0], &color[1], &color[2], &color[3]);
		RwColor rwcolor = getVehicleColorRGB(color[0]);

		// render vehicle name
		pD3DFontFixed->PrintShadow( screenPosition.fX, screenPosition.fY - h + ESP_tag_vehicle_pixelOffsetY,
										 COLOR_WHITE(200), buf );

		// health bar
		vh = iterVehicle->GetVehicleInterface()->m_nHealth;
		if ( vh > 1000.0f )
			vh = 1000.0f;
		vh /= 10.0f;
		if ( vh > 50.0f )
			vcolor = D3DCOLOR_ARGB( 64, 0, 150, 0 );
		else if ( vh > 25.0f )
			vcolor = D3DCOLOR_ARGB( 64, 150, 150, 0 );
		else
			vcolor = D3DCOLOR_ARGB( 64, 200, 0, 0 );

		// render health bar
		render->D3DBox( screenPosition.fX + ESP_tag_vehicle_D3DBox_pixelOffsetX,
						screenPosition.fY + ESP_tag_vehicle_pixelOffsetY + ESP_tag_vehicle_D3DBox_pixelOffsetY, 100.0f,
						11.0f, D3DCOLOR_ARGB(64, 0, 0, 0) );
		render->D3DBox( screenPosition.fX + 1.0f + ESP_tag_vehicle_D3DBox_pixelOffsetX,
						screenPosition.fY + 1.0f + ESP_tag_vehicle_pixelOffsetY + ESP_tag_vehicle_D3DBox_pixelOffsetY,
						vh - 2.0f, 9.0f, vcolor );

		h = pD3DFontFixedSmall->DrawHeight();
		_snprintf_s( buf, sizeof(buf)-1, "Health: %d", (int)vh );
		pD3DFontFixedSmall->PrintShadow( screenPosition.fX + 4.0f,
										 screenPosition.fY - h + 10.0f + ESP_tag_vehicle_pixelOffsetY,
										 D3DCOLOR_ARGB(200, 0, 255, 0), buf );


// trailer debugging visualizations
	/*	CVector vecTowBarPos, vecTowHitchPos;
		iterVehicle->GetTowBarPos( &vecTowBarPos );
		iterVehicle->GetTowHitchPos( &vecTowHitchPos );
		// plot vehicle position box
		render->D3DBox( screenPosition.fX,
						screenPosition.fY,
						5.0f,
						5.0f,
						D3DCOLOR_ARGB(255, 0, 0, 255) );
		// plot Tow Bar box
		poss.x = vecTowBarPos.fX;
		poss.y = vecTowBarPos.fY;
		poss.z = vecTowBarPos.fZ;
	pSampMulti->CalcScreenCoors( &poss, &screenposs );
		screenPosition.fX = screenposs.x;
		screenPosition.fY = screenposs.y;
		render->D3DBox( screenPosition.fX,
						screenPosition.fY,
						10.0f,
						10.0f,
						D3DCOLOR_ARGB(255, 0, 255, 0) );
		// plot Tow Hitch box
		poss.x = vecTowHitchPos.fX;
		poss.y = vecTowHitchPos.fY;
		poss.z = vecTowHitchPos.fZ;
		pSampMulti->CalcScreenCoors( &poss, &screenposs );
		screenPosition.fX = screenposs.x;
		screenPosition.fY = screenposs.y;
		render->D3DBox( screenPosition.fX,
						screenPosition.fY,
						10.0f,
						10.0f,
						D3DCOLOR_ARGB(255, 255, 0, 0) );
						*/
	}
}

static void RenderTeleportTexts ( void )
{
	if (cheat_state->_generic.cheat_panic_enabled)
		return;

	traceLastFunc( "RenderTeleportTexts()" );

	int					i;
	struct actor_info	*self = actor_info_get( ACTOR_SELF, 0 );
	char				buf[32];

	if ( self != NULL )
	{
		for ( i = 0; i < TELEPORT_MAX; i++ )
		{
			float		pos[3], screenpos[3];
			D3DXVECTOR3 poss, screenposs;
			vect3_copy( &cheat_state->teleport[i].matrix[4 * 3], pos );
			if ( vect3_near_zero(pos) )
				continue;
			if ( vect3_dist(pos, &self->base.matrix[4 * 3]) > set.BlackLight.teleport_tag_dist)
				continue;

			poss.x = pos[0];
			poss.y = pos[1];
			poss.z = pos[2];
			pSampMulti->CalcScreenCoors( &poss, &screenposs );
			screenpos[0] = screenposs.x;
			screenpos[1] = screenposs.y;
			screenpos[2] = screenposs.z;
			if ( screenpos[2] < 1.f )
				continue;

			sprintf( buf, "Teleport: %d", i );
			pD3DFontFixed->PrintShadow( screenpos[0], screenpos[1] - 5.0f, D3DCOLOR_XRGB(0, 200, 200), buf );
		}
	}
}

static void RenderPickupTexts ( void )
{
	traceLastFunc( "renderPickupTexts()" );

	if ( cheat_state->_generic.cheat_panic_enabled )
		return;
	if ( g_SAMP->pPools->pPickup == NULL )
		return;

	if ( (GetAsyncKeyState(VK_TAB) < 0)
	 ||	 g_Scoreboard->iIsEnabled ) 
		return;

	struct actor_info* self = actor_info_get(ACTOR_SELF, 0);
	if ( self != NULL )
	{
		for (int i = 0; i < SAMP_MAX_PICKUPS; i++ )
		{
			if ( g_SAMP->pPools->pPickup->pickup[i].iModelID == 0 )
				continue;
			if ( g_SAMP->pPools->pPickup->pickup[i].iType == 0 )
				continue;

			float		pos[3], screenpos[3];
			D3DXVECTOR3 poss, screenposs;
			vect3_copy( g_SAMP->pPools->pPickup->pickup[i].fPosition, pos );
			if ( vect3_near_zero(pos) )
				continue;
			if ( vect3_dist( pos, &self->base.matrix[4 * 3] ) > set.pickup_tags_dist )
				continue;

			poss.x = pos[0];
			poss.y = pos[1];
			poss.z = pos[2];
			pSampMulti->CalcScreenCoors( &poss, &screenposs );
			screenpos[0] = screenposs.x;
			screenpos[1] = screenposs.y;
			screenpos[2] = screenposs.z;
			if ( screenpos[2] < 1.f )
				continue;

			pD3DFontFixed->PrintShadow( screenpos[0], screenpos[1] - 5.0f, D3DCOLOR_XRGB(0, 200, 0), pSampMulti->SetText("Pickup: %d, ModelID: %d", i, g_SAMP->pPools->pPickup->pickup[i].iModelID));
		}
	}
}

static void RenderObjectTexts ( void )
{
	traceLastFunc( "renderObjectTexts()" );

	struct actor_info	*self = actor_info_get( ACTOR_SELF, 0 );
	if ( cheat_state->_generic.cheat_panic_enabled || self == NULL )
		return;
	if ( g_SAMP->pPools->pObject == NULL )
		return;

	if ( (GetAsyncKeyState(VK_TAB) < 0)
	 ||	g_Scoreboard->iIsEnabled ) 
		return;

	for (int i = 0; i < SAMP_MAX_OBJECTS; i++ )
	{
		if ( g_SAMP->pPools->pObject->iIsListed[i] != 1 )
			continue;
		if ( g_SAMP->pPools->pObject->object[i] == NULL )
			continue;
		if ( g_SAMP->pPools->pObject->object[i]->pGTAEntity == NULL )
			continue;

		float		pos[3], screenpos[3];
		D3DXVECTOR3 poss, screenposs;
		vect3_copy( &g_SAMP->pPools->pObject->object[i]->pGTAEntity->base.matrix[4 * 3], pos );
		if ( vect3_near_zero(pos) )
			continue;
		if ( vect3_dist(pos, &self->base.matrix[4 * 3]) > set.object_tags_dist )
			continue;

		poss.x = pos[0];
		poss.y = pos[1];
		poss.z = pos[2];
		pSampMulti->CalcScreenCoors( &poss, &screenposs );
		screenpos[0] = screenposs.x;
		screenpos[1] = screenposs.y;
		screenpos[2] = screenposs.z;
		if ( screenpos[2] < 1.f )
			continue;

		pD3DFontFixed->PrintShadow( screenpos[0], screenpos[1] - 5.0f, D3DCOLOR_XRGB(200, 200, 0), pSampMulti->SetText("Object: %d, ModelID: %d", i,
			g_SAMP->pPools->pObject->object[i]->pGTAEntity->base.model_alt_id));
	}
}

static void renderTextLabels (void)
{
	traceLastFunc( "renderTextLabels()" );

	// orig-Textlabels
	memcpy_safe( (void *)(g_dwSAMP_Addr + SAMP_DRAWTEXTLABELS), "\x74", 1 );

	if ( gta_menu_active() || !set.d3dtext_labels )
		return;

	if ( cheat_state->_generic.cheat_panic_enabled )
		return;

	if ( g_SAMP->pPools->pText3D == NULL || g_Vehicles == NULL || g_Players == NULL )
		return;

	struct actor_info	*self = actor_info_get( ACTOR_SELF, 0 );
	if ( self == NULL )
		return;

	// Deactivate orig-Textlabels
	memcpy_safe( (void *)(g_dwSAMP_Addr + SAMP_DRAWTEXTLABELS), "\xEB", 1 );

	if ( (GetAsyncKeyState(VK_TAB) < 0)
		|| g_Scoreboard->iIsEnabled ) 
		return;

	if ( GetAsyncKeyState(VK_F10) < 0 )
		return;

	for ( int i = 0; i < SAMP_MAX_3DTEXTS; i++ )
	{
		if ( !g_SAMP->pPools->pText3D->iIsListed[i] )
			continue;
		if ( g_SAMP->pPools->pText3D->textLabel[i].pText != NULL )
		{
			float		pos[3], screenpos[3];
			D3DXVECTOR3 poss, screenposs;

			if ( g_SAMP->pPools->pText3D->textLabel[i].sAttachedToPlayerID == ((uint16_t) - 1)
			 &&	 g_SAMP->pPools->pText3D->textLabel[i].sAttachedToVehicleID == ((uint16_t) - 1) )
			{
				vect3_copy( g_SAMP->pPools->pText3D->textLabel[i].fPosition, pos );
			}
			else if ( g_SAMP->pPools->pText3D->textLabel[i].sAttachedToPlayerID != ((uint16_t) - 1) )
			{
				int id = g_SAMP->pPools->pText3D->textLabel[i].sAttachedToPlayerID;
				// check if player is valid
				if ( g_Players->pRemotePlayer[id] == NULL || g_Players->pRemotePlayer[id]->pPlayerData == NULL 
					|| g_Players->pRemotePlayer[id]->pPlayerData->pSAMP_Actor == NULL
					|| g_Players->pRemotePlayer[id]->pPlayerData->pSAMP_Actor->pGTA_Ped == NULL )
					continue;
				vect3_copy( &g_Players->pRemotePlayer[id]->pPlayerData->pSAMP_Actor->pGTA_Ped->base.matrix[4 * 3],
					pos );
				if ( !near_zero(pos[2]) )
					pos[2] += 0.5f;
			}
			else if ( g_SAMP->pPools->pText3D->textLabel[i].sAttachedToVehicleID != ((uint16_t) - 1) )
			{
				int id = g_SAMP->pPools->pText3D->textLabel[i].sAttachedToVehicleID;
				// check if vehicle is valid
				if ( g_Vehicles->pGTA_Vehicle[id] == NULL )
					continue;
				else
					vect3_copy( &g_Vehicles->pGTA_Vehicle[id]->base.matrix[4 * 3], pos );
			}

			if ( vect3_near_zero(pos) )
				continue;
			if ( vect3_dist(pos, &self->base.matrix[4 * 3]) > set.d3dtext_labels_maxDist )
				continue;

			poss.x = pos[0];
			poss.y = pos[1];
			poss.z = pos[2];
			pSampMulti->CalcScreenCoors( &poss, &screenposs );
			screenpos[0] = screenposs.x;
			screenpos[1] = screenposs.y;
			screenpos[2] = screenposs.z;

			if ( screenpos[2] < 1.f )
				continue;

			pD3DFontFixed->PrintShadow( screenpos[0], screenpos[1], g_SAMP->pPools->pText3D->textLabel[i].color,
										pSampMulti->SetText("%s", g_SAMP->pPools->pText3D->textLabel[i].pText) );
		}
	}
}

static void clickWarp(void)
{
	traceLastFunc("clickWarp()");

	if (!g_iCursorEnabled) return;
	if (gta_menu_active()) return;
	if (pSampMulti->IsMenuActive()) return;

	POINT cursor_pos;
	if (GetCursorPos(&cursor_pos) && ScreenToClient(pPresentParam.hDeviceWindow, &cursor_pos))
	{
		D3DXVECTOR3 poss, screenposs;
		float pos[3];
		char buf[256];

		CVehicle *pCVehicleTeleport = nullptr;
		screenposs.x = (float)cursor_pos.x;
		screenposs.y = (float)cursor_pos.y;
		screenposs.z = 700.0f;

		pSampMulti->CalcWorldCoors(&screenposs, &poss);

		CVector vecTarget(poss.x, poss.y, poss.z);

		// setup variables
		CVector				vecOrigin, vecGroundPos;
		CColPoint			*pCollision = nullptr;
		CEntitySAInterface	*pCollisionEntity = nullptr;

		// origin = our camera
		vecOrigin = *pGame->GetCamera()->GetCam(pGame->GetCamera()->GetActiveCam())->GetSource();

		// check for collision
		bool bCollision = GTAfunc_ProcessLineOfSight( &vecOrigin, &vecTarget, &pCollision, &pCollisionEntity,
			1, 1, 0, 1, 1, 0, 0, 0 );

		if (bCollision && pCollision)
		{
			// calculate position to check for ground
			vecGroundPos = *pCollision->GetPosition();
			if ( cheat_state->state == CHEAT_STATE_VEHICLE )
			{
				vecGroundPos = vecGroundPos - (*pCollision->GetNormal() * 2.0f);
			}
			else
			{
				vecGroundPos = vecGroundPos - (*pCollision->GetNormal() * 0.5f);
			}

			// get ground position from collision position
			if (pPedSelf->GetAreaCode() == 0)
			{
				vecGroundPos.fZ = pGameInterface->GetWorld()->FindGroundZForPosition(vecGroundPos.fX, vecGroundPos.fY);
			}
			else
			{
				CVector vecGroundPosSlightlyAbove = vecGroundPos;
				vecGroundPosSlightlyAbove.fZ += 1.0f;
				vecGroundPos.fZ = pGameInterface->GetWorld()->FindGroundZFor3DPosition(&vecGroundPosSlightlyAbove);
			}

			// setup some stuff for vehicle jumper
			if (pCollisionEntity && pCollisionEntity->nType == ENTITY_TYPE_VEHICLE)
			{
				pCVehicleTeleport = pGameInterface->GetPools()->GetVehicle((DWORD *)pCollisionEntity);
				if (pCVehicleTeleport)
				{
					const struct vehicle_entry *vehicleEntry = gta_vehicle_get_by_id(pCVehicleTeleport->GetModelIndex());
					if (vehicleEntry != nullptr)
					{
						sprintf(buf, "Warp to %s", vehicleEntry->name);
					}
				}
				else
				{
					sprintf(buf, "Distance %0.2f", vect3_dist(&vecOrigin.fX, &vecGroundPos.fX));
				}
			}
			// setup some stuff for normal warp
			else
			{
				sprintf(buf, "Distance %0.2f", vect3_dist(&vecOrigin.fX, &vecGroundPos.fX));
			}
		}
		else
		{
			iClickWarpEnabled = 0; // force disable, prevents clicks
			//toggleSAMPCursor(0);
			return;
		}
		if (pCollision != nullptr)
		{
			// destroy the collision object
			pCollision->Destroy();
		}
		if (iClickWarpEnabled)
		{
			if (pCVehicleTeleport != nullptr)
			{
				// ClickWarp to vehicle
				int iVehicleID = getVehicleGTAIDFromInterface( (DWORD*) pCVehicleTeleport->GetInterface() );
				if ( !vehicleJumper(iVehicleID) && cheat_state->state != CHEAT_STATE_VEHICLE )
				{
					// failed to jump into vehicle, warp to it instead if we're not in a vehicle

					// check for collision
					CMatrix matVehicle;
					pCVehicleTeleport->GetMatrix(&matVehicle);
					CVector vecVehicleAbove = (matVehicle.vUp * 5.0f) + *pCVehicleTeleport->GetPosition(); // up multiplier should be enough to get above most vehicles, but not enough to jump above things over it
					bool bCollision = GTAfunc_ProcessLineOfSight( &vecVehicleAbove, pCVehicleTeleport->GetPosition(), &pCollision, &pCollisionEntity,
						1, 1, 0, 1, 1, 0, 0, 0 );
					if (bCollision && pCollision)
					{
						// set pos floats for actual teleporting
						pos[0] = pCollision->GetPosition()->fX;
						pos[1] = pCollision->GetPosition()->fY;
						pos[2] = pCollision->GetPosition()->fZ + 0.5f; // should be enough so we're surfing properly
					}
					else
					{
						// set pos floats for actual teleporting
						pos[0] = vecGroundPos.fX;
						pos[1] = vecGroundPos.fY;
						pos[2] = vecGroundPos.fZ + 0.5f;
					}
					if (pCollision != nullptr)
					{
						// destroy the collision object
						pCollision->Destroy();
					}
					// teleport
					cheat_teleport(pos, gta_interior_id_get());
					GTAfunc_TogglePlayerControllable(0);
					GTAfunc_LockActor(0);
				}
			} // end ClickWarp to vehicle
			else
			{
				// ClickWarp to location
				if ( cheat_state->state == CHEAT_STATE_VEHICLE )
				{
					// check for collision
					CVehicle *vehSelf = pPedSelf->GetVehicle();
					if (vehSelf)
					{
						// check for collision
						CVector vecVehicleGravity;
						vehSelf->GetGravity(&vecVehicleGravity);
						CVector vecVehicleAbove = (-vecVehicleGravity * 5.0f) + vecGroundPos;
						CVector vecVehicleBelow = (vecVehicleGravity * 5.0f) + vecGroundPos;
						bool bCollision = GTAfunc_ProcessLineOfSight( &vecVehicleAbove, &vecVehicleBelow, &pCollision, &pCollisionEntity,
							1, 0, 0, 1, 1, 0, 0, 0 ); // not checking for vehicle collisions
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
							pos[0] = pCollision->GetPosition()->fX;
							pos[1] = pCollision->GetPosition()->fY;
							pos[2] = pCollision->GetPosition()->fZ + 1.0f; // should be enough to stay above the ground properly
						}
						else
						{
							// set pos floats for actual teleporting
							pos[0] = vecGroundPos.fX;
							pos[1] = vecGroundPos.fY;
							pos[2] = vecGroundPos.fZ + 0.5f;
						}
						if (pCollision != nullptr)
						{
							// destroy the collision object
							pCollision->Destroy();
						}
					}
					else
					{
						// set pos floats for actual teleporting
						pos[0] = vecGroundPos.fX;
						pos[1] = vecGroundPos.fY;
						pos[2] = vecGroundPos.fZ + 0.5f;
					}
				}
				else
				{
					// set pos floats for actual teleporting
					pos[0] = vecGroundPos.fX;
					pos[1] = vecGroundPos.fY;
					pos[2] = vecGroundPos.fZ + 0.5f;
				}

				// teleport
				cheat_teleport(pos, gta_interior_id_get());
				GTAfunc_TogglePlayerControllable(0);
				GTAfunc_LockActor(0);
			} // end ClickWarp to location

			iClickWarpEnabled = 0;
			toggleSAMPCursor(0);
			g_iCursorEnabled = 0;
		}

		if (pCVehicleTeleport != nullptr)
		{
			D3DXVECTOR3 vehPoss, vehScreenposs;
			vehPoss.x = pCVehicleTeleport->GetPosition()->fX;
			vehPoss.y = pCVehicleTeleport->GetPosition()->fY;
			vehPoss.z = pCVehicleTeleport->GetPosition()->fZ + -1.0f;
			pSampMulti->CalcScreenCoors( &vehPoss, &vehScreenposs );
			// print vehicle warp target name below vehicle & vehicle esp
			pD3DFont->PrintShadow(vehScreenposs.x, vehScreenposs.y + 10.0f, -1, buf);
		}
		else if (bCollision && pCollision)
		{
			D3DXVECTOR3 groundPoss, groundScreenposs;
			groundPoss.x = vecGroundPos.fX;
			groundPoss.y = vecGroundPos.fY;
			groundPoss.z = vecGroundPos.fZ;
			pSampMulti->CalcScreenCoors( &groundPoss, &groundScreenposs );
			// print warp distance
			pD3DFont->PrintShadow(
				groundScreenposs.x - (pD3DFont->DrawLength(buf) / 2.0f) + 3.5f,
				groundScreenposs.y - 20.0f,
				-1, buf);
		}
	}
}

static void renderImGui(void)
{
	traceLastFunc("renderImGui()");

	// don't run if the CGameSA doesn't exist
	if (!pGameInterface)
		return;

	if (!g_SAMP)
		return;
	if (GetAsyncKeyState(VK_F5) < 0)
		return;

	if (gta_menu_active())
		return;

	if (cheat_state->_generic.cheat_panic_enabled)
		return;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (BlackLightFuncs->Menu.bImGuiDemoWindow)
		ImGui::ShowDemoWindow();

	pMenu->Initialize_ImGui_Menus();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void renderSAMP ( void )
{
	traceLastFunc( "renderSAMP()" );
	
	// get samp structures
	if ( !g_renderSAMP_initSAMPstructs )
	{
		g_SAMP = stGetSampInfo();
		if ( isBadPtr_writeAny(g_SAMP, sizeof(stSAMP)) )
			return;
		
		if ( isBadPtr_writeAny(g_SAMP->pPools, sizeof(stSAMPPools)) )
			return;
		
		g_Players = g_SAMP->pPools->pPlayer;
		if ( isBadPtr_writeAny(g_Players, sizeof(stPlayerPool)) )
			return;

		g_Vehicles = g_SAMP->pPools->pVehicle;
		if ( isBadPtr_writeAny(g_Vehicles, sizeof(stVehiclePool)) )
			return;
		
		g_Chat = stGetSampChatInfo();
		if ( isBadPtr_writeAny(g_Chat, sizeof(stChatInfo)) )
			return;

		g_Input = stGetInputInfo();
		if ( isBadPtr_writeAny(g_Input, sizeof(stInputInfo)) )
			return;

		g_DeathList = stGetKillInfo();
		if ( isBadPtr_writeAny(g_DeathList, sizeof(stKillInfo)) )
			return;

		g_Scoreboard = stGetScoreboardInfo();
		if ( isBadPtr_writeAny(g_Scoreboard, sizeof(stScoreboardInfo)) )
			return;
			
		if ( g_SAMP->pRakClientInterface == NULL )
			return;
		
		// initialize raknet
		g_RakClient2 = new RakClient2( g_SAMP->pRakClientInterface );
		g_SAMP->pRakClientInterface = new HookedRakClientInterface();
		
		initChatCmds();

		memcpy_safe((void *)0x004B35A0, (uint8_t *)"\x83\xEC\x0C\x56\x8B\xF1", 6 ); // godmode patch
		
		// 0x: Set's the Frame Sleeping to 0 so you get more performance (sa:mp init is so far a good place ;d) .
		*(BYTE*)0xBAB318 = 0;
		memset_safe((void *)0x53E94C, 0, 1);

		g_renderSAMP_initSAMPstructs = 1;
	}

	if ( g_SAMP != NULL )
	{
		setSAMPCustomSendRates( set.onfoot_sendrate, set.incar_sendrate, set.aim_sendrate, set.headsync_sendrate );

		if ( !g_SAMP->pSettings->byteShowNameTags && set.nametags_show)
			g_SAMP->pSettings->byteShowNameTags = 1;
		if ( g_SAMP->pSettings->byteNoNametagsBehindWalls && set.disable_line_of_sight)
			g_SAMP->pSettings->byteNoNametagsBehindWalls = 0;
		if ( !g_SAMP->pSettings->iPlayerMarkersMode && set.markers_show )
			g_SAMP->pSettings->iPlayerMarkersMode = PLAYER_MARKERS_MODE_GLOBAL;

		g_SAMP->pSettings->fNameTagsDistance = set.line_of_sight_dist;
		if (BlackLightFuncs->bPickupESP )
			RenderPickupTexts();
		if (BlackLightFuncs->bObjectsESP)
			RenderObjectTexts();

		updateScoreboardData();
		renderTextLabels();
		clickWarp();
		pBlackLightMods->BlackLight_InitESPFuncs();
		pBlackLightMods->BlackLight_InitRainbowFuncs();

		static bool a;
		if (!a)
		{
			if ( set.player_tags_dist > set.line_of_sight_dist )
				memcpy_safe( (void *)(g_dwSAMP_Addr + SAMP_DRAWING_DISTANCE), &set.player_tags_dist, sizeof(float) );
			else
				memcpy_safe( (void *)(g_dwSAMP_Addr + SAMP_DRAWING_DISTANCE), &set.line_of_sight_dist, sizeof(float) );

			a = true;
		}
	}
}

static void mapMenuTeleport ( void )
{
	traceLastFunc( "mapMenuTeleport()" );

	if ( set.map_must_be_open && *(byte *)0xBA68A5 != 5 )
		return;

	if ( (*(int *)0xBA6774 != 0) && GetAsyncKeyState(set.key_map_teleport) & ( 1 << 16 ) && pGameInterface != NULL )
	{
		// ty to Racer_S for this
		float	mapPos[3];
		for ( int i = 0; i < (0xAF * 0x28); i += 0x28 )
		{
			if ( *(short *)(0xBA873D + i) == 4611 )
			{
				float	*pos = (float *)( 0xBA86F8 + 0x28 + i );
				gta_interior_id_set( 0 );
				mapPos[0] = *pos;
				mapPos[1] = *( pos + 1 );
				mapPos[2] = pGameInterface->GetWorld()->FindGroundZForPosition( mapPos[0], mapPos[1] ) + 2.0f;
				cheat_teleport( mapPos, 0 );
			}
		}
	}
}

void texturesInitResources ( IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters )
{
	traceLastFunc( "texturesInitResources()" );

	if ((
			 fopen(set.speedometer_speedo_png_filename, "rb") == NULL
	 ||	 fopen(set.speedometer_needle_png_filename, "rb") == NULL
	 ) )
	{
		Log( "Could not find the speedometer files, disabling it." );
		BlackLightFuncs->bSpeedometer = false;
	}
		// init speedo
		tSpeedoPNG = NULL;
		sSpeedoPNG = NULL;
		tNeedlePNG = NULL;
		sNeedlePNG = NULL;
		if ( !tSpeedoPNG )
			D3DXCreateTextureFromFile( pDevice, set.speedometer_speedo_png_filename, &tSpeedoPNG );
		if ( !sSpeedoPNG )
			D3DXCreateSprite( pDevice, &sSpeedoPNG );
		if ( !tNeedlePNG )
			D3DXCreateTextureFromFile( pDevice, set.speedometer_needle_png_filename, &tNeedlePNG );
		if ( !sNeedlePNG )
			D3DXCreateSprite( pDevice, &sNeedlePNG );
		needlePos.x = ( pPresentationParameters->BackBufferWidth / 1024.0f );
		needlePos.y = ( pPresentationParameters->BackBufferHeight / 768.0f );
		speedoPos.x = ( 750.0f * needlePos.x );
		speedoPos.y = pPresentationParameters->BackBufferHeight - ( 292.0f * needlePos.y );
	
	// ret
}

void proxyID3DDevice9_Reset ( void )
{
	traceLastFunc( "proxyID3DDevice9_Reset()" );

	// fonts
	pD3DFont->Invalidate();
	pD3DFontFixed->Invalidate();
	pD3DFontFixedSmall->Invalidate();
	pD3DFontChat->Invalidate();
	pD3DFont_sampStuff->Invalidate();

	SAFE_RELEASE( chams_green );
	SAFE_RELEASE( chams_blue );
	SAFE_RELEASE( chams_red );


		SAFE_RELEASE( sSpeedoPNG );
		SAFE_RELEASE( tSpeedoPNG );
		SAFE_RELEASE( sNeedlePNG );
		SAFE_RELEASE( tNeedlePNG );
	
	// death texture
	SAFE_RELEASE( pSpriteTexture );

	// the main render class
	render->Invalidate();
	ImGui_ImplDX9_InvalidateDeviceObjects();

	// supposedly this worked so set init state
	// this should probably actually check eventually
	static int	proxyIDirect3DDevice9_init = 0;
	bD3DRenderInit = false;
}

void proxyID3DDevice9_Present ( D3DPRESENT_PARAMETERS *pPresentationParameters )
{
	traceLastFunc( "proxyID3DDevice9_InitOurShit()" );

	// orig device has been reset, let's re-init
	render->Initialize( origIDirect3DDevice9 );

	// load fonts
	pD3DFont->Initialize( origIDirect3DDevice9 );
	pD3DFontFixed->Initialize( origIDirect3DDevice9 );
	pD3DFontFixedSmall->Initialize( origIDirect3DDevice9 );
	pD3DFontChat->Initialize( origIDirect3DDevice9 );
	pD3DFont_sampStuff->Initialize( origIDirect3DDevice9 );

	// load shaders
	GenerateShader( origIDirect3DDevice9, &chams_green, 0.8f, 0, 1.0f, 0 );
	GenerateShader( origIDirect3DDevice9, &chams_blue, 0.8f, 0, 0, 1.0f );
	GenerateShader( origIDirect3DDevice9, &chams_red, 0.8f, 1.0f, 0, 0 );

	// load GUI textures/sprits
	texturesInitResources( origIDirect3DDevice9, pPresentationParameters );

	// load death texture
	LoadSpriteTexture();
	
	// supposedly this worked so set init state
	// this should probably actually check eventually
	bD3DRenderInit = true;
}

// window/fullscreen init/update
bool	g_InitWindowMode_ForceUpdate_Active = false;
void proxyID3DDevice9_InitWindowMode ( D3DPRESENT_PARAMETERS *pPresentationParameters )
{
	traceLastFunc( "proxyID3DDevice9_InitWindowMode()" );

	if (*(byte*)0x746225 == 0x90) {
		g_isRequestingWindowModeToggle = false;
		g_isRequesting_RwD3D9ChangeVideoMode = false;
		return;
	}

	// window mode toggle, flips set.window_mode bit
	if ( g_isRequestingWindowModeToggle )
	{
		g_isRequestingWindowModeToggle = false;
		set.window_mode ^= 1;
	}

	// m_ForceChange settings
	bool	m_ForceUpdate;

	// set to force the desired video mode if needed
	if ( set.window_mode != pPresentationParameters->Windowed || set.window_mode != g_pGTAPresent->Windowed )
	{
		// we need to force an update
		m_ForceUpdate = true;
	}

	// we do not want basic presentation parameters anymore
	/*g_pGTAPresent->Flags = 0;
	g_pGTAPresent->PresentationInterval = 0;
	pPresentationParameters->Flags = 0;
	pPresentationParameters->PresentationInterval = 0;*/

	// get the current video mode & the correct back buffer size
	int			m_dwVideoMode = g_pCSettingsSAInterface->dwVideoMode;
	VideoMode	m_currentVideoMode;
	GTAfunc_getVideoModeInfo( &m_currentVideoMode, m_dwVideoMode );

	int presentWidth = m_currentVideoMode.width;
	int presentHeight = m_currentVideoMode.height;

	// set windowed or fullscreen
	if ( g_isRequesting_RwD3D9ChangeVideoMode && m_ForceUpdate && !g_InitWindowMode_ForceUpdate_Active )
	{
		g_isRequesting_RwD3D9ChangeVideoMode = false;
		g_InitWindowMode_ForceUpdate_Active = true;
		if ( set.window_mode )
		{
			// set to windowed
			if ( GTAfunc_RwD3D9ChangeVideoMode(0) != 1 )
			{
				Log( "proxyID3DDevice9_InitWindowMode() failed to GTAfunc_RwD3D9ChangeVideoMode(0) #1" );
				g_InitWindowMode_ForceUpdate_Active = false;
				return;
			}
			else
			{
				*(int *)0xC920CC = 1;	// _?windowed
				g_RsGlobal->ps->fullscreen = 0;
				if ( isBadPtr_writeAny(g_pGTAPresent, sizeof(D3DPRESENT_PARAMETERS)) )
				{
					Log( "proxyID3DDevice9_InitWindowMode() g_pGTAPresent pointer bad. Continuing anyways." );
				}
				else
				{
					g_pGTAPresent->Windowed = 1;
					g_pGTAPresent->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
					g_pGTAPresent->BackBufferWidth = presentWidth;
					g_pGTAPresent->BackBufferHeight = presentHeight;
				}

				if ( isBadPtr_writeAny(pPresentationParameters, sizeof(D3DPRESENT_PARAMETERS)) )
				{
					Log( "proxyID3DDevice9_InitWindowMode() pPresentationParameters pointer bad. Continuing anyways." );
				}
				else
				{
					pPresentationParameters->Windowed = 1;
					pPresentationParameters->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
					pPresentationParameters->BackBufferWidth = presentWidth;
					pPresentationParameters->BackBufferHeight = presentHeight;
				}

				if ( GTAfunc_RwD3D9ChangeVideoMode(0) != 1 )
				{
					Log( "proxyID3DDevice9_InitWindowMode() failed to GTAfunc_RwD3D9ChangeVideoMode(0) #2" );
					g_InitWindowMode_ForceUpdate_Active = false;
					return;
				}

				//GTAfunc_setCurrentVideoMode(0);
			}
		}
		else if ( !set.window_mode )
		{
			// set to fullscreen
			if ( GTAfunc_RwD3D9ChangeVideoMode(0) != 1 )
			{
				Log( "proxyID3DDevice9_InitWindowMode() failed to GTAfunc_RwD3D9ChangeVideoMode(0) #3" );
				g_InitWindowMode_ForceUpdate_Active = false;
				return;
			}
			else
			{
				*(int *)0xC920CC = 0;	// _?windowed
				g_RsGlobal->ps->fullscreen = 1;
				if ( isBadPtr_writeAny(g_pGTAPresent, sizeof(D3DPRESENT_PARAMETERS)) )
				{
					Log( "proxyID3DDevice9_InitWindowMode() g_pGTAPresent pointer bad. Continuing anyways." );
				}
				else
				{
					g_pGTAPresent->Windowed = 0;
					g_pGTAPresent->FullScreen_RefreshRateInHz = ulFullScreenRefreshRate;
					g_pGTAPresent->BackBufferWidth = presentWidth;
					g_pGTAPresent->BackBufferHeight = presentHeight;
				}

				if ( isBadPtr_writeAny(pPresentationParameters, sizeof(D3DPRESENT_PARAMETERS)) )
				{
					Log( "proxyID3DDevice9_InitWindowMode() pPresentationParameters pointer bad. Continuing anyways." );
				}
				else
				{
					pPresentationParameters->Windowed = 0;
					pPresentationParameters->FullScreen_RefreshRateInHz = ulFullScreenRefreshRate;
					pPresentationParameters->BackBufferWidth = presentWidth;
					pPresentationParameters->BackBufferHeight = presentHeight;
				}

				if ( GTAfunc_RwD3D9ChangeVideoMode(m_dwVideoMode) != 1 )
				{
					Log( "proxyID3DDevice9_InitWindowMode() failed to GTAfunc_RwD3D9ChangeVideoMode(m_dwVideoMode) #1, m_dwVideoMode=%d",
					 m_dwVideoMode );
					g_InitWindowMode_ForceUpdate_Active = false;
					return;
				}

				//GTAfunc_setCurrentVideoMode(m_dwVideoMode);
			}
		}

		g_InitWindowMode_ForceUpdate_Active = false;
	}
	else if ( m_ForceUpdate && set.window_mode && !g_InitWindowMode_ForceUpdate_Active )
	{
		g_InitWindowMode_ForceUpdate_Active = true;
		*(int *)0xC920CC = 1;			// _?windowed
		g_RsGlobal->ps->fullscreen = 0;
		if ( isBadPtr_writeAny(g_pGTAPresent, sizeof(D3DPRESENT_PARAMETERS)) )
		{
			Log( "proxyID3DDevice9_InitWindowMode() g_pGTAPresent pointer bad. Continuing anyways." );
		}
		else
		{
			g_pGTAPresent->Windowed = 1;
			g_pGTAPresent->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			g_pGTAPresent->BackBufferWidth = presentWidth;
			g_pGTAPresent->BackBufferHeight = presentHeight;
		}

		if ( isBadPtr_writeAny(pPresentationParameters, sizeof(D3DPRESENT_PARAMETERS)) )
		{
			Log( "proxyID3DDevice9_InitWindowMode() pPresentationParameters pointer bad. Continuing anyways." );
		}
		else
		{
			pPresentationParameters->Windowed = 1;
			pPresentationParameters->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			pPresentationParameters->BackBufferWidth = presentWidth;
			pPresentationParameters->BackBufferHeight = presentHeight;
		}

		g_InitWindowMode_ForceUpdate_Active = false;
	}

	// window mode stuff
	if ( set.window_mode
	 &&	 pPresentationParameters->BackBufferWidth != 0
	 &&	 pPresentationParameters->BackBufferHeight != 0 )
	{
		// title bar or not?
		if ( set.window_mode_titlebar )
		{
			RECT	um;		// damn near killed um

			// add caption bar, etc
			SetWindowLong( pPresentationParameters->hDeviceWindow, GWL_STYLE,
						   WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE );

			// update caption bar, etc
			SetWindowPos( pPresentationParameters->hDeviceWindow, HWND_NOTOPMOST, 0, 0, 0, 0,
						  SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );

			// the client area of the window must be the same size as the back buffer
			GetClientRect( pPresentationParameters->hDeviceWindow, &um );
			if ( um.right == pPresentationParameters->BackBufferWidth
			 &&	 um.bottom == pPresentationParameters->BackBufferHeight ) goto proxyID3DDevice9_InitWindowMode_end;
			SetWindowPos( pPresentationParameters->hDeviceWindow, HWND_NOTOPMOST, 0, 0,
						  pPresentationParameters->BackBufferWidth + (pPresentationParameters->BackBufferWidth - um.right),
							  pPresentationParameters->BackBufferHeight + (pPresentationParameters->BackBufferHeight - um.bottom),
								  SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE );
			if ( pool_actor != NULL )
			{
				int x, y;
				x = GetSystemMetrics( SM_CXSCREEN );
				y = GetSystemMetrics( SM_CYSCREEN );
				SetWindowPos( pPresentationParameters->hDeviceWindow, HWND_NOTOPMOST,
							  (x / 2) - (pPresentationParameters->BackBufferWidth / 2),
							  (y / 2) - (pPresentationParameters->BackBufferHeight / 2),
							  pPresentationParameters->BackBufferWidth + (pPresentationParameters->BackBufferWidth - um.right),
						  pPresentationParameters->BackBufferHeight +
								  (pPresentationParameters->BackBufferHeight - um.bottom), SWP_SHOWWINDOW );
			}
		}
		else
		{
			// center the window
			SetWindowPos( pPresentationParameters->hDeviceWindow, HWND_NOTOPMOST, 0, 0, 0, 0,
						  SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
		}
	}

proxyID3DDevice9_InitWindowMode_end: ;
	// always make sure our window_mode is synced with the game's
	set.window_mode = ( g_RsGlobal->ps->fullscreen == 0 );
}

void renderHandler()
{
	traceLastFunc("renderHandler()");

	// set FPS limit for vsync mode
	*(int*)0xC1704C = set.fps_limit;

	// we should already be initialized, but what the hell why not
	while (!bD3DRenderInit)
		proxyID3DDevice9_Present(&pPresentParam);

	// init death type textures and HUD colors
	static int	proxyIDirect3DDevice9_init = 0;
	if (!proxyIDirect3DDevice9_init)
	{
		LoadSpriteTexture();

		*(uint8_t*)0xBAB22C = gta_hp_bar->red;
		*(uint8_t*)0xBAB22D = gta_hp_bar->green;
		*(uint8_t*)0xBAB22E = gta_hp_bar->blue;
		*(uint8_t*)0xBAB22F = gta_hp_bar->alpha;

		*(uint8_t*)0xBAB230 = gta_money_hud->red;
		*(uint8_t*)0xBAB231 = gta_money_hud->green;
		*(uint8_t*)0xBAB232 = gta_money_hud->blue;
		*(uint8_t*)0xBAB233 = gta_money_hud->alpha;

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		// Load Fonts
		pFontArial = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\arialbd.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
		IM_ASSERT(pFontArial != NULL);
		// merge in icons from Font Awesome
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF(pSampMulti->SetText("%s%s", M0D_FOLDER, FONT_ICON_FILE_NAME_FAS), 16.0f, &icons_config, icons_ranges);

		pFontArialV2 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\arialbd.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
		IM_ASSERT(pFontArialV2 != NULL);

		pFontSmall = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\arialbd.ttf", 11.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
		IM_ASSERT(pFontSmall != NULL);

		pFontSpeedmeter = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\arialbd.ttf", 25.0f, NULL);
		IM_ASSERT(pFontSpeedmeter != NULL);

		ImGui_ImplWin32_Init(pPresentParam.hDeviceWindow);
		ImGui_ImplDX9_Init(origIDirect3DDevice9);
		proxyIDirect3DDevice9_init = 1;
	}

	// setup render class now for doing stuff within cheat_hook
	bool	isBeginRenderWIN = SUCCEEDED(render->BeginRender());

	// run all dem hacks
	cheat_hook(pPresentParam.hDeviceWindow);

	static char	buf[256];
	if (isBeginRenderWIN)
	{
		static int	game_inited;

		if (cheat_panic() || cheat_state->state == CHEAT_STATE_NONE)
		{
			if (set.flickering_problem)
				goto no_render;

			if (!set.flickering_problem)
			{
				if (iIsSAMPSupported)
				{
					pD3DFont->PrintShadow(1.0f, (float)(pPresentParam.BackBufferHeight) - pD3DFont->DrawHeight() - 3, D3DCOLOR_ARGB(127, 255, 255, 255), \
						pSampMulti->SetText("%s for %s", NAME, g_szSAMPVer));
				}
				else
				{
					sNeedlePNG->Begin(D3DXSPRITE_ALPHABLEND);
					sNeedlePNG->Draw(tNeedlePNG, NULL, NULL, NULL, 0xCCFFFFFF);
					sNeedlePNG->End();
				}
			}
			// startup logo was here, but damn it works better without it
			// we should figure out why that is some time
		}
		else
		{

			if (!game_inited)
			{
				game_inited = 1;
				goto no_render;
			}

			if (set.flickering_problem)
				if (gta_menu_active())
					goto no_render;
		}

		if (cheat_state->state == CHEAT_STATE_VEHICLE)
		{
			if (BlackLightFuncs->bSpeedometer == true) ShowSpeedoMeter();
		}
		else if (cheat_state->state == CHEAT_STATE_ACTOR)
		{

		}

		if (cheat_state->state != CHEAT_STATE_NONE)
		{
		}

		if (cheat_state->text_time > 0 && time_get() - cheat_state->text_time < MSEC_TO_TIME(3000))
		{
			uint32_t	color, alpha = 255;

			if (time_get() - cheat_state->text_time > MSEC_TO_TIME(2000))
				alpha -= (time_get() - cheat_state->text_time - MSEC_TO_TIME(2000)) * 255 / MSEC_TO_TIME(1000);

			color = D3DCOLOR_ARGB(alpha, 255, 255, 255);

			_snprintf_s(buf, sizeof(buf) - 1, "%s <-", cheat_state->text);
			render->D3DBox(pPresentParam.BackBufferWidth - pD3DFont->DrawLength(buf) - 3.0f, 1, pD3DFont->DrawLength(buf), pD3DFont->DrawHeight(), D3DCOLOR_ARGB(alpha, 0, 0, 0));
			pD3DFont->PrintShadow(pPresentParam.BackBufferWidth - pD3DFont->DrawLength(buf) - 3.0f, 1,
				D3DCOLOR_ARGB(alpha, 255, 255, 255), buf);
		}


		renderSAMP();
		if (g_BotFuncs) g_BotFuncs->Bot_Process();

		renderPlayerTags();

		if (BlackLightFuncs->bTeleportTextSet)
			RenderTeleportTexts();
		if (BlackLightFuncs->bDefaultModVehicleTags)
			renderVehicleTags();
		if (BlackLightFuncs->bPlayersMap)
			RenderMap();

		renderImGui();

		// render menu
		if (cheat_state->_generic.menu)
			RenderMenu();

	no_render:;
		render->EndRender();
	}

	mapMenuTeleport();

	traceLastFunc("it_wasnt_us()");
}

///////////////////////////////////////////////////////////////////////////////
// End of common D3D functions.
///////////////////////////////////////////////////////////////////////////////

proxyIDirect3DDevice9::proxyIDirect3DDevice9 ( IDirect3DDevice9 *pOriginal )
{
	traceLastFunc( "proxyIDirect3DDevice9::proxyIDirect3DDevice9()" );

	//Log("proxyIDirect3DDevice9 constructor called. Original IDirect3DDevice9 interface address is 0x%p", pOriginal);
	origIDirect3DDevice9 = pOriginal;
	bD3DRenderInit = false;
}

proxyIDirect3DDevice9::~proxyIDirect3DDevice9 ( void )
{
	Log( "proxyIDirect3DDevice9 destructor called." );
}

HRESULT proxyIDirect3DDevice9::QueryInterface ( REFIID riid, void **ppvObj )
{
	traceLastFunc( "proxyIDirect3DDevice9::QueryInterface()" );

	HRESULT hRes;

	*ppvObj = NULL;
	hRes = origIDirect3DDevice9->QueryInterface( riid, ppvObj );
	if ( hRes == NOERROR )
		*ppvObj = this;

	return hRes;
}

ULONG proxyIDirect3DDevice9::AddRef ( void )
{
	return origIDirect3DDevice9->AddRef();
}

ULONG proxyIDirect3DDevice9::Release ( void )
{
	traceLastFunc( "proxyIDirect3DDevice9::Release()" );

	ULONG	count = origIDirect3DDevice9->Release();

	if ( count == 0 )
	{
		delete( this );
	}

	return count;
}

HRESULT proxyIDirect3DDevice9::TestCooperativeLevel ( void )
{
	return origIDirect3DDevice9->TestCooperativeLevel();
}

UINT proxyIDirect3DDevice9::GetAvailableTextureMem ( void )
{
	return origIDirect3DDevice9->GetAvailableTextureMem();
}

HRESULT proxyIDirect3DDevice9::EvictManagedResources ( void )
{
	return origIDirect3DDevice9->EvictManagedResources();
}

HRESULT proxyIDirect3DDevice9::GetDirect3D ( IDirect3D9 **ppD3D9 )
{
	return origIDirect3DDevice9->GetDirect3D( ppD3D9 );
}

HRESULT proxyIDirect3DDevice9::GetDeviceCaps ( D3DCAPS9 *pCaps )
{
	return origIDirect3DDevice9->GetDeviceCaps( pCaps );
}

HRESULT proxyIDirect3DDevice9::GetDisplayMode ( UINT iSwapChain, D3DDISPLAYMODE *pMode )
{
	return origIDirect3DDevice9->GetDisplayMode( iSwapChain, pMode );
}

HRESULT proxyIDirect3DDevice9::GetCreationParameters ( D3DDEVICE_CREATION_PARAMETERS *pParameters )
{
	return origIDirect3DDevice9->GetCreationParameters( pParameters );
}

HRESULT proxyIDirect3DDevice9::SetCursorProperties ( UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 *pCursorBitmap )
{
	return origIDirect3DDevice9->SetCursorProperties( XHotSpot, YHotSpot, pCursorBitmap );
}

void proxyIDirect3DDevice9::SetCursorPosition ( int X, int Y, DWORD Flags )
{
	origIDirect3DDevice9->SetCursorPosition( X, Y, Flags );
}

BOOL proxyIDirect3DDevice9::ShowCursor ( BOOL bShow )
{
	return origIDirect3DDevice9->ShowCursor( bShow );
}

HRESULT proxyIDirect3DDevice9::CreateAdditionalSwapChain ( D3DPRESENT_PARAMETERS *pPresentationParameters,
														   IDirect3DSwapChain9 **pSwapChain )
{
	return origIDirect3DDevice9->CreateAdditionalSwapChain( pPresentationParameters, pSwapChain );
}

HRESULT proxyIDirect3DDevice9::GetSwapChain ( UINT iSwapChain, IDirect3DSwapChain9 **pSwapChain )
{
	return origIDirect3DDevice9->GetSwapChain( iSwapChain, pSwapChain );
}

UINT proxyIDirect3DDevice9::GetNumberOfSwapChains ( void )
{
	return origIDirect3DDevice9->GetNumberOfSwapChains();
}

HRESULT proxyIDirect3DDevice9::Reset ( D3DPRESENT_PARAMETERS *pPresentationParameters )
{
	traceLastFunc( "proxyIDirect3DDevice9::Reset()" );

	// new PROPER reset code
	//
	// return for the reset
	HRESULT hRes_orig_Reset;

	// this should always be done if a reset is requested
	// and we have assets initialized on the proxy device
	while ( bD3DRenderInit )
		proxyID3DDevice9_Reset();

	// always attempt to reset orig device if we are asked to
	// since GTA is getting the state of the original device
	hRes_orig_Reset = origIDirect3DDevice9->Reset( pPresentationParameters );

	// handle the return from original Reset()
	if ( hRes_orig_Reset == D3D_OK )
	{
		// variable for checking the pointers
		short	badPointerCount_g_pGTAPresent = 0;
		bool	badPointerBreak_g_pGTAPresent = false;
		short	badPointerCount_pPresentationParameters = 0;
		bool	badPointerBreak_pPresentationParameters = false;
		while ( isBadPtr_writeAny(g_pGTAPresent, sizeof(D3DPRESENT_PARAMETERS)) && !badPointerBreak_g_pGTAPresent )
		{
			badPointerCount_g_pGTAPresent++;
			if ( badPointerCount_g_pGTAPresent < 50 )
			{
				Sleep( 100 );
			}
			else
			{
				Log( "During D3D9 Reset(), g_pGTAPresent was bad for over 5 seconds. Continuing anyways." );
				badPointerBreak_g_pGTAPresent = true;
			}
		}

		while
		(
			isBadPtr_writeAny(pPresentationParameters, sizeof(D3DPRESENT_PARAMETERS))
			&& !badPointerBreak_pPresentationParameters
		)
		{
			badPointerCount_pPresentationParameters++;
			if ( badPointerCount_pPresentationParameters < 50 )
			{
				Sleep( 100 );
			}
			else
			{
				Log( "During D3D9 Reset(), pPresentationParameters was bad for over 5 seconds. Continuing anyways." );
				badPointerBreak_pPresentationParameters = true;
			}
		}

		// init our shit
		while ( !bD3DRenderInit )
		{
			proxyID3DDevice9_Present( pPresentationParameters );
			if ( !bD3DRenderInit )
				Sleep( 100 );
		}

		// init our window mode
		proxyID3DDevice9_InitWindowMode( pPresentationParameters );

		// update the global Present Param struct AFTER original reset, only if it's ok
		pPresentParam = *pPresentationParameters;

		// Update our data.
		pD3DData->StoreViewport( 0, 0, pPresentationParameters->BackBufferWidth,
								 pPresentationParameters->BackBufferHeight );
	}

	// reporting problems is about all we can do here.
	// we would normally just end the application gracefully right
	// now, but GTA should be handling this on its own, so whatever.
	else if ( hRes_orig_Reset == D3DERR_DEVICELOST )
	{
		Log( "origIDirect3DDevice9::Reset() returned D3DERR_DEVICELOST" );
	}
	else if ( hRes_orig_Reset == D3DERR_DRIVERINTERNALERROR )
	{
		Log( "WTF: origIDirect3DDevice9::Reset() returned D3DERR_DRIVERINTERNALERROR" );
	}
	else if ( hRes_orig_Reset == D3DERR_OUTOFVIDEOMEMORY )
	{
		Log( "WTF: origIDirect3DDevice9::Reset() returned D3DERR_OUTOFVIDEOMEMORY" );
	}
	else
	{
		Log( "WTF: origIDirect3DDevice9::Reset() returned value %d", hRes_orig_Reset );
	}

	// bai
	return hRes_orig_Reset;
}

HRESULT proxyIDirect3DDevice9::Present ( CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride,
										 CONST RGNDATA *pDirtyRegion )
{
	traceLastFunc( "proxyIDirect3DDevice9::Present()" );

	renderHandler();
	// A fog flicker fix for some ATI cards
	// this is screwing up right not for some reason
	// check out the other pD3DData references for an idea of what this does
	//
	//D3DMATRIX projMatrix;
	//pD3DData->GetTransform ( D3DTS_PROJECTION, &projMatrix );
	//origIDirect3DDevice9->SetTransform ( D3DTS_PROJECTION, &projMatrix );

	// get original function to return
	return origIDirect3DDevice9->Present( pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion );
}

HRESULT proxyIDirect3DDevice9::GetBackBuffer ( UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type,
											   IDirect3DSurface9 **ppBackBuffer )
{
	return origIDirect3DDevice9->GetBackBuffer( iSwapChain, iBackBuffer, Type, ppBackBuffer );
}

HRESULT proxyIDirect3DDevice9::GetRasterStatus ( UINT iSwapChain, D3DRASTER_STATUS *pRasterStatus )
{
	return origIDirect3DDevice9->GetRasterStatus( iSwapChain, pRasterStatus );
}

HRESULT proxyIDirect3DDevice9::SetDialogBoxMode ( BOOL bEnableDialogs )
{
	return origIDirect3DDevice9->SetDialogBoxMode( bEnableDialogs );
}

void proxyIDirect3DDevice9::SetGammaRamp ( UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP *pRamp )
{
	origIDirect3DDevice9->SetGammaRamp( iSwapChain, Flags, pRamp );
}

void proxyIDirect3DDevice9::GetGammaRamp ( UINT iSwapChain, D3DGAMMARAMP *pRamp )
{
	origIDirect3DDevice9->GetGammaRamp( iSwapChain, pRamp );
}

HRESULT proxyIDirect3DDevice9::CreateTexture ( UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format,
											   D3DPOOL Pool, IDirect3DTexture9 **ppTexture, HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateTexture( Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::CreateVolumeTexture ( UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage,
													 D3DFORMAT Format, D3DPOOL Pool,
													 IDirect3DVolumeTexture9 **ppVolumeTexture, HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateVolumeTexture( Width, Height, Depth, Levels, Usage, Format, Pool,
													  ppVolumeTexture, pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::CreateCubeTexture ( UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format,
												   D3DPOOL Pool, IDirect3DCubeTexture9 **ppCubeTexture,
												   HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateCubeTexture( EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture,
													pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::CreateVertexBuffer ( UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool,
													IDirect3DVertexBuffer9 **ppVertexBuffer, HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateVertexBuffer( Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::CreateIndexBuffer ( UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
												   IDirect3DIndexBuffer9 **ppIndexBuffer, HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateIndexBuffer( Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::CreateRenderTarget ( UINT Width, UINT Height, D3DFORMAT Format,
													D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality,
													BOOL Lockable, IDirect3DSurface9 **ppSurface, HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateRenderTarget( Width, Height, Format, MultiSample, MultisampleQuality, Lockable,
													 ppSurface, pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::CreateDepthStencilSurface ( UINT Width, UINT Height, D3DFORMAT Format,
														   D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality,
														   BOOL Discard, IDirect3DSurface9 **ppSurface,
														   HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateDepthStencilSurface( Width, Height, Format, MultiSample, MultisampleQuality,
															Discard, ppSurface, pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::UpdateSurface ( IDirect3DSurface9 *pSourceSurface, CONST RECT *pSourceRect,
											   IDirect3DSurface9 *pDestinationSurface, CONST POINT *pDestPoint )
{
	return origIDirect3DDevice9->UpdateSurface( pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint );
}

HRESULT proxyIDirect3DDevice9::UpdateTexture ( IDirect3DBaseTexture9 *pSourceTexture,
											   IDirect3DBaseTexture9 *pDestinationTexture )
{
	return origIDirect3DDevice9->UpdateTexture( pSourceTexture, pDestinationTexture );
}

HRESULT proxyIDirect3DDevice9::GetRenderTargetData ( IDirect3DSurface9 *pRenderTarget, IDirect3DSurface9 *pDestSurface )
{
	return origIDirect3DDevice9->GetRenderTargetData( pRenderTarget, pDestSurface );
}

HRESULT proxyIDirect3DDevice9::GetFrontBufferData ( UINT iSwapChain, IDirect3DSurface9 *pDestSurface )
{
	return origIDirect3DDevice9->GetFrontBufferData( iSwapChain, pDestSurface );
}

HRESULT proxyIDirect3DDevice9::StretchRect ( IDirect3DSurface9 *pSourceSurface, CONST RECT *pSourceRect,
											 IDirect3DSurface9 *pDestSurface, CONST RECT *pDestRect,
											 D3DTEXTUREFILTERTYPE Filter )
{
	return origIDirect3DDevice9->StretchRect( pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter );
}

HRESULT proxyIDirect3DDevice9::ColorFill ( IDirect3DSurface9 *pSurface, CONST RECT *pRect, D3DCOLOR color )
{
	return origIDirect3DDevice9->ColorFill( pSurface, pRect, color );
}

HRESULT proxyIDirect3DDevice9::CreateOffscreenPlainSurface ( UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool,
															 IDirect3DSurface9 **ppSurface, HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateOffscreenPlainSurface( Width, Height, Format, Pool, ppSurface, pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::SetRenderTarget ( DWORD RenderTargetIndex, IDirect3DSurface9 *pRenderTarget )
{
	return origIDirect3DDevice9->SetRenderTarget( RenderTargetIndex, pRenderTarget );
}

HRESULT proxyIDirect3DDevice9::GetRenderTarget ( DWORD RenderTargetIndex, IDirect3DSurface9 **ppRenderTarget )
{
	return origIDirect3DDevice9->GetRenderTarget( RenderTargetIndex, ppRenderTarget );
}

HRESULT proxyIDirect3DDevice9::SetDepthStencilSurface ( IDirect3DSurface9 *pNewZStencil )
{
	return origIDirect3DDevice9->SetDepthStencilSurface( pNewZStencil );
}

HRESULT proxyIDirect3DDevice9::GetDepthStencilSurface ( IDirect3DSurface9 **ppZStencilSurface )
{
	return origIDirect3DDevice9->GetDepthStencilSurface( ppZStencilSurface );
}

HRESULT proxyIDirect3DDevice9::BeginScene ( void )
{
	traceLastFunc( "proxyIDirect3DDevice9::BeginScene()" );

	// return original function
	HRESULT ret = origIDirect3DDevice9->BeginScene();
	traceLastFunc( "end of proxyIDirect3DDevice9::BeginScene()" );
	return ret;
}

HRESULT proxyIDirect3DDevice9::EndScene ( void )
{
	return origIDirect3DDevice9->EndScene();
}

HRESULT proxyIDirect3DDevice9::Clear ( DWORD Count, CONST D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z,
									   DWORD Stencil )
{
	return origIDirect3DDevice9->Clear( Count, pRects, Flags, Color, Z, Stencil );
}

HRESULT proxyIDirect3DDevice9::SetTransform ( D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix )
{
	// Store the matrix
	pD3DData->StoreTransform ( State, pMatrix );

	return origIDirect3DDevice9->SetTransform( State, pMatrix );
}

HRESULT proxyIDirect3DDevice9::GetTransform ( D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix )
{
	return origIDirect3DDevice9->GetTransform( State, pMatrix );
}

HRESULT proxyIDirect3DDevice9::MultiplyTransform ( D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix )
{
	return origIDirect3DDevice9->MultiplyTransform( State, pMatrix );
}

HRESULT proxyIDirect3DDevice9::SetViewport ( CONST D3DVIEWPORT9 *pViewport )
{
	// Store matrix
	pD3DData->StoreViewport ( pViewport->X, pViewport->Y, pViewport->Width, pViewport->Height );

	return origIDirect3DDevice9->SetViewport( pViewport );
}

HRESULT proxyIDirect3DDevice9::GetViewport ( D3DVIEWPORT9 *pViewport )
{
	return origIDirect3DDevice9->GetViewport( pViewport );
}

HRESULT proxyIDirect3DDevice9::SetMaterial ( CONST D3DMATERIAL9 *pMaterial )
{
	return origIDirect3DDevice9->SetMaterial( pMaterial );
}

HRESULT proxyIDirect3DDevice9::GetMaterial ( D3DMATERIAL9 *pMaterial )
{
	return origIDirect3DDevice9->GetMaterial( pMaterial );
}

HRESULT proxyIDirect3DDevice9::SetLight ( DWORD Index, CONST D3DLIGHT9 *pLight )
{
	return origIDirect3DDevice9->SetLight( Index, pLight );
}

HRESULT proxyIDirect3DDevice9::GetLight ( DWORD Index, D3DLIGHT9 *pLight )
{
	return origIDirect3DDevice9->GetLight( Index, pLight );
}

HRESULT proxyIDirect3DDevice9::LightEnable ( DWORD Index, BOOL Enable )
{
	return origIDirect3DDevice9->LightEnable( Index, Enable );
}

HRESULT proxyIDirect3DDevice9::GetLightEnable ( DWORD Index, BOOL *pEnable )
{
	return origIDirect3DDevice9->GetLightEnable( Index, pEnable );
}

HRESULT proxyIDirect3DDevice9::SetClipPlane ( DWORD Index, CONST float *pPlane )
{
	return origIDirect3DDevice9->SetClipPlane( Index, pPlane );
}

HRESULT proxyIDirect3DDevice9::GetClipPlane ( DWORD Index, float *pPlane )
{
	return origIDirect3DDevice9->GetClipPlane( Index, pPlane );
}

HRESULT proxyIDirect3DDevice9::SetRenderState ( D3DRENDERSTATETYPE State, DWORD Value )
{
	return origIDirect3DDevice9->SetRenderState( State, Value );
}

HRESULT proxyIDirect3DDevice9::GetRenderState ( D3DRENDERSTATETYPE State, DWORD *pValue )
{
	return origIDirect3DDevice9->GetRenderState( State, pValue );
}

HRESULT proxyIDirect3DDevice9::CreateStateBlock ( D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9 **ppSB )
{
	return origIDirect3DDevice9->CreateStateBlock( Type, ppSB );
}

HRESULT proxyIDirect3DDevice9::BeginStateBlock ( void )
{
	return origIDirect3DDevice9->BeginStateBlock();
}

HRESULT proxyIDirect3DDevice9::EndStateBlock ( IDirect3DStateBlock9 **ppSB )
{
	return origIDirect3DDevice9->EndStateBlock( ppSB );
}

HRESULT proxyIDirect3DDevice9::SetClipStatus ( CONST D3DCLIPSTATUS9 *pClipStatus )
{
	return origIDirect3DDevice9->SetClipStatus( pClipStatus );
}

HRESULT proxyIDirect3DDevice9::GetClipStatus ( D3DCLIPSTATUS9 *pClipStatus )
{
	return origIDirect3DDevice9->GetClipStatus( pClipStatus );
}

HRESULT proxyIDirect3DDevice9::GetTexture ( DWORD Stage, IDirect3DBaseTexture9 **ppTexture )
{
	return origIDirect3DDevice9->GetTexture( Stage, ppTexture );
}

HRESULT proxyIDirect3DDevice9::SetTexture ( DWORD Stage, IDirect3DBaseTexture9 *pTexture )
{
	return origIDirect3DDevice9->SetTexture( Stage, pTexture );
}

HRESULT proxyIDirect3DDevice9::GetTextureStageState ( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue )
{
	return origIDirect3DDevice9->GetTextureStageState( Stage, Type, pValue );
}

HRESULT proxyIDirect3DDevice9::SetTextureStageState ( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value )
{
	return origIDirect3DDevice9->SetTextureStageState( Stage, Type, Value );
}

HRESULT proxyIDirect3DDevice9::GetSamplerState ( DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD *pValue )
{
	return origIDirect3DDevice9->GetSamplerState( Sampler, Type, pValue );
}

HRESULT proxyIDirect3DDevice9::SetSamplerState ( DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value )
{
	return origIDirect3DDevice9->SetSamplerState( Sampler, Type, Value );
}

HRESULT proxyIDirect3DDevice9::ValidateDevice ( DWORD *pNumPasses )
{
	return origIDirect3DDevice9->ValidateDevice( pNumPasses );
}

HRESULT proxyIDirect3DDevice9::SetPaletteEntries ( UINT PaletteNumber, CONST PALETTEENTRY *pEntries )
{
	return origIDirect3DDevice9->SetPaletteEntries( PaletteNumber, pEntries );
}

HRESULT proxyIDirect3DDevice9::GetPaletteEntries ( UINT PaletteNumber, PALETTEENTRY *pEntries )
{
	return origIDirect3DDevice9->GetPaletteEntries( PaletteNumber, pEntries );
}

HRESULT proxyIDirect3DDevice9::SetCurrentTexturePalette ( UINT PaletteNumber )
{
	return origIDirect3DDevice9->SetCurrentTexturePalette( PaletteNumber );
}

HRESULT proxyIDirect3DDevice9::GetCurrentTexturePalette ( UINT *PaletteNumber )
{
	return origIDirect3DDevice9->GetCurrentTexturePalette( PaletteNumber );
}

HRESULT proxyIDirect3DDevice9::SetScissorRect ( CONST RECT *pRect )
{
	return origIDirect3DDevice9->SetScissorRect( pRect );
}

HRESULT proxyIDirect3DDevice9::GetScissorRect ( RECT *pRect )
{
	return origIDirect3DDevice9->GetScissorRect( pRect );
}

HRESULT proxyIDirect3DDevice9::SetSoftwareVertexProcessing ( BOOL bSoftware )
{
	return origIDirect3DDevice9->SetSoftwareVertexProcessing( bSoftware );
}

BOOL proxyIDirect3DDevice9::GetSoftwareVertexProcessing ( void )
{
	return origIDirect3DDevice9->GetSoftwareVertexProcessing();
}

HRESULT proxyIDirect3DDevice9::SetNPatchMode ( float nSegments )
{
	return origIDirect3DDevice9->SetNPatchMode( nSegments );
}

float proxyIDirect3DDevice9::GetNPatchMode ( void )
{
	return origIDirect3DDevice9->GetNPatchMode();
}

HRESULT proxyIDirect3DDevice9::DrawPrimitive ( D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount )
{
	return origIDirect3DDevice9->DrawPrimitive( PrimitiveType, StartVertex, PrimitiveCount );
}

HRESULT proxyIDirect3DDevice9::DrawIndexedPrimitive ( D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex,
													  UINT MinVertexIndex, UINT NumVertices, UINT startIndex,
													  UINT primCount )
{
	traceLastFunc( "proxyIDirect3DDevice9::DrawIndexedPrimitive()" );

	DWORD	dwRet_addr = ( DWORD ) _ReturnAddress();

	// chams probably works better with texture instead of shaders

	if (BlackLightFuncs->bPlayersThroughWalls && !cheat_state->_generic.cheat_panic_enabled)
	{
		// actors
		if (dwRet_addr == 0x761142)
		{
			//origIDirect3DDevice9->SetRenderState( D3DRS_LIGHTING, true );
			//origIDirect3DDevice9->SetRenderState( D3DRS_SPECULARENABLE, true );
			//origIDirect3DDevice9->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
			//origIDirect3DDevice9->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
			//origIDirect3DDevice9->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );

			if (set.BlackLight.chams_style == 1)
			{
				origIDirect3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
			else if (set.BlackLight.chams_style == 2)
			{
				origIDirect3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
			else if (set.BlackLight.chams_style == 3)
			{
				origIDirect3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
			}

			// actor behind wall
			origIDirect3DDevice9->SetRenderState(D3DRS_ZENABLE, false);
			//origIDirect3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			origIDirect3DDevice9->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices,
				startIndex, primCount);

			if (set.BlackLight.chams_style == 1)
			{
				origIDirect3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
			else if (set.BlackLight.chams_style == 2)
			{
				origIDirect3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
			else if (set.BlackLight.chams_style == 3)
			{
				origIDirect3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
			}

			// actor infront wall
			origIDirect3DDevice9->SetRenderState(D3DRS_ZENABLE, true);
			origIDirect3DDevice9->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices,
				startIndex, primCount);
			//origIDirect3DDevice9->SetPixelShader(NULL);
			return origIDirect3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}


	if (BlackLightFuncs->bDefaultChams && !cheat_state->_generic.cheat_panic_enabled )
	{
		// actors
		if ( dwRet_addr == 0x761142 )
		{
			origIDirect3DDevice9->SetRenderState( D3DRS_ZENABLE, false );
			//origIDirect3DDevice9->SetRenderState( D3DRS_LIGHTING, true );
			//origIDirect3DDevice9->SetRenderState( D3DRS_SPECULARENABLE, true );
			//origIDirect3DDevice9->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
			//origIDirect3DDevice9->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
			//origIDirect3DDevice9->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );

			if (set.BlackLight.chams_style == 2)
			{
				origIDirect3DDevice9->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
			}
			else if (set.BlackLight.chams_style == 1)
			{
				origIDirect3DDevice9->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
			}
			else if (set.BlackLight.chams_style == 3)
			{
				origIDirect3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
			}
			origIDirect3DDevice9->SetRenderState( D3DRS_PATCHEDGESTYLE, D3DPATCHEDGE_CONTINUOUS );


			// actor behind wall
			origIDirect3DDevice9->SetPixelShader( chams_blue );
			origIDirect3DDevice9->DrawIndexedPrimitive( PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices,
														startIndex, primCount );

			// actor infront wall
			origIDirect3DDevice9->SetPixelShader( chams_green );
			origIDirect3DDevice9->SetRenderState( D3DRS_ZENABLE, true );
			origIDirect3DDevice9->DrawIndexedPrimitive( PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices,
														startIndex, primCount );
			origIDirect3DDevice9->SetPixelShader( NULL );
			return origIDirect3DDevice9->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		}

		// some environment parts are also found..
		// weapons
		/*if ( dwRet_addr == 0x75731B )
		{
			LPDIRECT3DVERTEXBUFFER9 Stream_Data;
			D3DVERTEXBUFFER_DESC desc;
			unsigned int desc_size = 0;
			UINT Stride = 0;
			if ( origIDirect3DDevice9->GetStreamSource(0, &Stream_Data, &NumVertices,&Stride) == D3D_OK )
			{
				if ( Stream_Data != NULL )
				{
					Stream_Data->GetDesc( &desc );
					Stream_Data->Release();
					desc_size = desc.Size;
				}
			}

			if ( desc_size == 0x20000 )
			{
				origIDirect3DDevice9->SetRenderState( D3DRS_ZENABLE, false );
				origIDirect3DDevice9->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

				// weapon behind wall
				origIDirect3DDevice9->SetPixelShader( chams_red );
				origIDirect3DDevice9->DrawIndexedPrimitive( PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices,
															startIndex, primCount );

				// weapon infront wall
				origIDirect3DDevice9->SetPixelShader( chams_blue );
				origIDirect3DDevice9->SetRenderState( D3DRS_ZENABLE, true );
				origIDirect3DDevice9->DrawIndexedPrimitive( PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices,
															startIndex, primCount );
				origIDirect3DDevice9->SetPixelShader( NULL );
				return origIDirect3DDevice9->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
			}
		}*/
	}

	return origIDirect3DDevice9->DrawIndexedPrimitive( PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices,
													   startIndex, primCount );
}

HRESULT proxyIDirect3DDevice9::DrawPrimitiveUP ( D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount,
												 CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride )
{
	return origIDirect3DDevice9->DrawPrimitiveUP( PrimitiveType, PrimitiveCount, pVertexStreamZeroData,
												  VertexStreamZeroStride );
}

HRESULT proxyIDirect3DDevice9::DrawIndexedPrimitiveUP ( D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex,
														UINT NumVertices, UINT PrimitiveCount, CONST void *pIndexData,
														D3DFORMAT IndexDataFormat, CONST void *pVertexStreamZeroData,
														UINT VertexStreamZeroStride )
{
	return origIDirect3DDevice9->DrawIndexedPrimitiveUP( PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount,
														 pIndexData, IndexDataFormat, pVertexStreamZeroData,
														 VertexStreamZeroStride );
}

HRESULT proxyIDirect3DDevice9::ProcessVertices ( UINT SrcStartIndex, UINT DestIndex, UINT VertexCount,
												 IDirect3DVertexBuffer9 *pDestBuffer,
												 IDirect3DVertexDeclaration9 *pVertexDecl, DWORD Flags )
{
	return origIDirect3DDevice9->ProcessVertices( SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags );
}

HRESULT proxyIDirect3DDevice9::CreateVertexDeclaration ( CONST D3DVERTEXELEMENT9 *pVertexElements,
														 IDirect3DVertexDeclaration9 **ppDecl )
{
	return origIDirect3DDevice9->CreateVertexDeclaration( pVertexElements, ppDecl );
}

HRESULT proxyIDirect3DDevice9::SetVertexDeclaration ( IDirect3DVertexDeclaration9 *pDecl )
{
	return origIDirect3DDevice9->SetVertexDeclaration( pDecl );
}

HRESULT proxyIDirect3DDevice9::GetVertexDeclaration ( IDirect3DVertexDeclaration9 **ppDecl )
{
	return origIDirect3DDevice9->GetVertexDeclaration( ppDecl );
}

HRESULT proxyIDirect3DDevice9::SetFVF ( DWORD FVF )
{
	return origIDirect3DDevice9->SetFVF( FVF );
}

HRESULT proxyIDirect3DDevice9::GetFVF ( DWORD *pFVF )
{
	return origIDirect3DDevice9->GetFVF( pFVF );
}

HRESULT proxyIDirect3DDevice9::CreateVertexShader ( CONST DWORD *pFunction, IDirect3DVertexShader9 **ppShader )
{
	return origIDirect3DDevice9->CreateVertexShader( pFunction, ppShader );
}

HRESULT proxyIDirect3DDevice9::SetVertexShader ( IDirect3DVertexShader9 *pShader )
{
	return origIDirect3DDevice9->SetVertexShader( pShader );
}

HRESULT proxyIDirect3DDevice9::GetVertexShader ( IDirect3DVertexShader9 **ppShader )
{
	return origIDirect3DDevice9->GetVertexShader( ppShader );
}

HRESULT proxyIDirect3DDevice9::SetVertexShaderConstantF ( UINT StartRegister, CONST float *pConstantData,
														  UINT Vector4fCount )
{
	return origIDirect3DDevice9->SetVertexShaderConstantF( StartRegister, pConstantData, Vector4fCount );
}

HRESULT proxyIDirect3DDevice9::GetVertexShaderConstantF ( UINT StartRegister, float *pConstantData, UINT Vector4fCount )
{
	return origIDirect3DDevice9->GetVertexShaderConstantF( StartRegister, pConstantData, Vector4fCount );
}

HRESULT proxyIDirect3DDevice9::SetVertexShaderConstantI ( UINT StartRegister, CONST int *pConstantData,
														  UINT Vector4iCount )
{
	return origIDirect3DDevice9->SetVertexShaderConstantI( StartRegister, pConstantData, Vector4iCount );
}

HRESULT proxyIDirect3DDevice9::GetVertexShaderConstantI ( UINT StartRegister, int *pConstantData, UINT Vector4iCount )
{
	return origIDirect3DDevice9->GetVertexShaderConstantI( StartRegister, pConstantData, Vector4iCount );
}

HRESULT proxyIDirect3DDevice9::SetVertexShaderConstantB ( UINT StartRegister, CONST BOOL *pConstantData, UINT BoolCount )
{
	return origIDirect3DDevice9->SetVertexShaderConstantB( StartRegister, pConstantData, BoolCount );
}

HRESULT proxyIDirect3DDevice9::GetVertexShaderConstantB ( UINT StartRegister, BOOL *pConstantData, UINT BoolCount )
{
	return origIDirect3DDevice9->GetVertexShaderConstantB( StartRegister, pConstantData, BoolCount );
}

HRESULT proxyIDirect3DDevice9::SetStreamSource ( UINT StreamNumber, IDirect3DVertexBuffer9 *pStreamData,
												 UINT OffsetInBytes, UINT Stride )
{
	return origIDirect3DDevice9->SetStreamSource( StreamNumber, pStreamData, OffsetInBytes, Stride );
}

HRESULT proxyIDirect3DDevice9::GetStreamSource ( UINT StreamNumber, IDirect3DVertexBuffer9 **ppStreamData,
												 UINT *OffsetInBytes, UINT *pStride )
{
	return origIDirect3DDevice9->GetStreamSource( StreamNumber, ppStreamData, OffsetInBytes, pStride );
}

HRESULT proxyIDirect3DDevice9::SetStreamSourceFreq ( UINT StreamNumber, UINT Divider )
{
	return origIDirect3DDevice9->SetStreamSourceFreq( StreamNumber, Divider );
}

HRESULT proxyIDirect3DDevice9::GetStreamSourceFreq ( UINT StreamNumber, UINT *Divider )
{
	return origIDirect3DDevice9->GetStreamSourceFreq( StreamNumber, Divider );
}

HRESULT proxyIDirect3DDevice9::SetIndices ( IDirect3DIndexBuffer9 *pIndexData )
{
	return origIDirect3DDevice9->SetIndices( pIndexData );
}

HRESULT proxyIDirect3DDevice9::GetIndices ( IDirect3DIndexBuffer9 **ppIndexData )
{
	return origIDirect3DDevice9->GetIndices( ppIndexData );
}

HRESULT proxyIDirect3DDevice9::CreatePixelShader ( CONST DWORD *pFunction, IDirect3DPixelShader9 **ppShader )
{
	return origIDirect3DDevice9->CreatePixelShader( pFunction, ppShader );
}

HRESULT proxyIDirect3DDevice9::SetPixelShader ( IDirect3DPixelShader9 *pShader )
{
	return origIDirect3DDevice9->SetPixelShader( pShader );
}

HRESULT proxyIDirect3DDevice9::GetPixelShader ( IDirect3DPixelShader9 **ppShader )
{
	return origIDirect3DDevice9->GetPixelShader( ppShader );
}

HRESULT proxyIDirect3DDevice9::SetPixelShaderConstantF ( UINT StartRegister, CONST float *pConstantData,
														 UINT Vector4fCount )
{
	return origIDirect3DDevice9->SetPixelShaderConstantF( StartRegister, pConstantData, Vector4fCount );
}

HRESULT proxyIDirect3DDevice9::GetPixelShaderConstantF ( UINT StartRegister, float *pConstantData, UINT Vector4fCount )
{
	return origIDirect3DDevice9->GetPixelShaderConstantF( StartRegister, pConstantData, Vector4fCount );
}

HRESULT proxyIDirect3DDevice9::SetPixelShaderConstantI ( UINT StartRegister, CONST int *pConstantData, UINT Vector4iCount )
{
	return origIDirect3DDevice9->SetPixelShaderConstantI( StartRegister, pConstantData, Vector4iCount );
}

HRESULT proxyIDirect3DDevice9::GetPixelShaderConstantI ( UINT StartRegister, int *pConstantData, UINT Vector4iCount )
{
	return origIDirect3DDevice9->GetPixelShaderConstantI( StartRegister, pConstantData, Vector4iCount );
}

HRESULT proxyIDirect3DDevice9::SetPixelShaderConstantB ( UINT StartRegister, CONST BOOL *pConstantData, UINT BoolCount )
{
	return origIDirect3DDevice9->SetPixelShaderConstantB( StartRegister, pConstantData, BoolCount );
}

HRESULT proxyIDirect3DDevice9::GetPixelShaderConstantB ( UINT StartRegister, BOOL *pConstantData, UINT BoolCount )
{
	return origIDirect3DDevice9->GetPixelShaderConstantB( StartRegister, pConstantData, BoolCount );
}

HRESULT proxyIDirect3DDevice9::DrawRectPatch ( UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo )
{
	return origIDirect3DDevice9->DrawRectPatch( Handle, pNumSegs, pRectPatchInfo );
}

HRESULT proxyIDirect3DDevice9::DrawTriPatch ( UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo )
{
	return origIDirect3DDevice9->DrawTriPatch( Handle, pNumSegs, pTriPatchInfo );
}

HRESULT proxyIDirect3DDevice9::DeletePatch ( UINT Handle )
{
	return origIDirect3DDevice9->DeletePatch( Handle );
}

HRESULT proxyIDirect3DDevice9::CreateQuery ( D3DQUERYTYPE Type, IDirect3DQuery9 **ppQuery )
{
	return origIDirect3DDevice9->CreateQuery( Type, ppQuery );
}

void toggleWindowedMode ( void )
{
	traceLastFunc( "toggleWindowedMode()" );

	g_isRequestingWindowModeToggle = true;
	g_isRequesting_RwD3D9ChangeVideoMode = true;
	proxyID3DDevice9_InitWindowMode( &pPresentParam );

	if (!BlackLightFuncs->bWindowedMode)
		BlackLightFuncs->bWindowedMode = true;
	else BlackLightFuncs->bWindowedMode = false;
}
