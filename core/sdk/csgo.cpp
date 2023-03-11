#include "csgo.h"

#define capture_interface( ptr, type, module_name, interface_name ) ptr = g_game_modules->get( module_name ).get_interface<type>( interface_name ); if ( !ptr ) return false;
#define capture_ptr( ptr, target ) ptr = target; if ( !ptr ) return false;

bool c_game_interfaces::setup( )
{
	try
	{
		capture_interface( this->client, i_base_client, client_dll, "VClient" );
		capture_interface( this->surface, i_surface, vgui_dll, "VGUI_Surface" );
		capture_interface( this->panel, i_panel, vgui2_dll, "VGUI_Panel" );
		capture_interface( this->engine, i_engine_client, engine_dll, "VEngineClient" );
		capture_interface( this->entity_list, i_client_entity_list, client_dll, "VClientEntityList" );
		capture_interface( this->game_types, i_game_types, matchmaking_dll, "VENGINE_GAMETYPES_VERSION" );
		capture_interface( this->model_info, iv_model_info, engine_dll, "VModelInfoClient" );
		capture_interface( this->engine_trace, i_engine_trace, engine_dll, "EngineTraceClient" );
		capture_interface( this->mdl_cache, i_mdl_cache, datacache_dll, "MDLCache" );
		capture_interface( this->convar, i_convar, vstdlib_dll, "VEngineCvar" );
		capture_interface( this->localize, i_localize, localize_dll, "Localize_" );
		capture_interface( this->prediction, i_prediction, client_dll, "VClientPrediction" );
		capture_interface( this->game_movement, i_game_movement, client_dll, "GameMovement" );

		capture_ptr( device, **reinterpret_cast< IDirect3DDevice9*** >( g_game_modules->get( shaderpidx9_dll ).find_pattern( "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) + 0x1 ) );
		capture_ptr( key_values_system, reinterpret_cast< key_values_system_fn >( g_game_modules->get( vstdlib_dll ).get_proc_address( "KeyValuesSystem" ) )( ) );
		capture_ptr( client_mode, **reinterpret_cast< i_client_mode_shared*** >( c_memory::get_vfunc<std::uintptr_t>( client, 10 ) + 0x5 ) );
		capture_ptr( client_state, **reinterpret_cast< i_client_state*** >( c_memory::get_vfunc<std::uintptr_t>( engine, 12 ) + 0x10 ) );
		capture_ptr( weapon_system, *reinterpret_cast< i_weapon_system** >( c_memory::find_pattern( "client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0" ) + 0x2 ) );
		capture_ptr( global_vars, **reinterpret_cast< i_global_vars_base*** >( c_memory::get_vfunc<std::uintptr_t>( client, 11 ) + 0xA ) );
	}
	catch ( const std::exception& ex )
	{
		error_log( "[error] {}", ex.what( ) );

#ifdef _DEBUG
		// show error message window (or replace to your exception handler)
		_RPT0( _CRT_ERROR, ex.what( ) );
#endif
	}

	return true;
}

bool c_game_modules::setup( )
{
	/* modules list */
	modules[ engine_dll ] = c_memory::get_module_handle( engine_dll );
	modules[ client_dll ] = c_memory::get_module_handle( client_dll );
	modules[ localize_dll ] = c_memory::get_module_handle( localize_dll );
	modules[ materialsystem_dll ] = c_memory::get_module_handle( materialsystem_dll );
	modules[ vgui_dll ] = c_memory::get_module_handle( vgui_dll );
	modules[ vgui2_dll ] = c_memory::get_module_handle( vgui2_dll );
	modules[ shaderpidx9_dll ] = c_memory::get_module_handle( shaderpidx9_dll );
	modules[ gameoverlayrenderer_dll ] = c_memory::get_module_handle( gameoverlayrenderer_dll );
	modules[ physics_dll ] = c_memory::get_module_handle( physics_dll );
	modules[ vstdlib_dll ] = c_memory::get_module_handle( vstdlib_dll );
	modules[ tier0_dll ] = c_memory::get_module_handle( tier0_dll );
	modules[ inputsystem_dll ] = c_memory::get_module_handle( inputsystem_dll );
	modules[ studiorender_dll ] = c_memory::get_module_handle( studiorender_dll );
	modules[ datacache_dll ] = c_memory::get_module_handle( datacache_dll );
	modules[ steam_api_dll ] = c_memory::get_module_handle( steam_api_dll );
	modules[ matchmaking_dll ] = c_memory::get_module_handle( matchmaking_dll );
	modules[ server_dll ] = c_memory::get_module_handle( server_dll );
	modules[ serverbrowser_dll ] = c_memory::get_module_handle( serverbrowser_dll );

	return !modules.empty( );
}

// remove the macros
#ifdef capture_interface
#undef capture_interface
#endif // capture_interface
#ifdef capture_ptr
#undef capture_ptr
#endif // capture_ptr