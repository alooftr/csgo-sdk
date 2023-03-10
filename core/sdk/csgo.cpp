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

		capture_ptr( device, **reinterpret_cast< IDirect3DDevice9*** >( g_game_modules->get( shaderpidx9_dll ).find_pattern( "A1 ? ? ? ? 50 8B 08 FF 51 0C", 0x1 ) ) );



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