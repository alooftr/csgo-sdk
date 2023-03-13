#include "csgo.h"

#define capture_interface( ptr, type, module_name, interface_name ) ptr = g_game_modules->get( module_name ).get_interface<type>( interface_name ); if ( !ptr ) return false;
#define capture_ptr( ptr, target ) ptr = target; if ( !ptr ) return false;

bool c_game_interfaces::setup( )
{

	capture_interface( this->client, i_base_client, client_dll, "VClient" );
	capture_interface( this->surface, i_surface, vgui_dll, "VGUI_Surface" );
	capture_interface( this->panel, i_panel, vgui2_dll, "VGUI_Panel" );
	capture_interface( this->engine, i_engine_client, engine_dll, "VEngineClient" );
	capture_interface( this->entity_list, i_client_entity_list, client_dll, "VClientEntityList" );
	capture_interface( this->game_types, i_game_types, matchmaking_dll, "VENGINE_GAMETYPES_VERSION" );
	capture_interface( this->model_info, i_model_info, engine_dll, "VModelInfoClient" );
	capture_interface( this->engine_trace, i_engine_trace, engine_dll, "EngineTraceClient" );
	capture_interface( this->mdl_cache, i_mdl_cache, datacache_dll, "MDLCache" );
	capture_interface( this->convar, i_convar, vstdlib_dll, "VEngineCvar" );
	capture_interface( this->localize, i_localize, localize_dll, "Localize_" );
	capture_interface( this->prediction, i_prediction, client_dll, "VClientPrediction" );
	capture_interface( this->game_movement, i_game_movement, client_dll, "GameMovement" );
	capture_interface( this->file_system, i_file_system, filesystem_stdio_dll, "VBaseFileSystem011" );
	capture_interface( this->material_system, i_material_system, materialsystem_dll, "VMaterialSystem" );
	capture_interface( this->model_render, i_model_render, engine_dll, "VEngineModel" );
	capture_interface( this->studio_render, i_studio_render, studiorender_dll, "VStudioRender" );
	capture_interface( this->debug_overlay, i_debug_overlay, engine_dll, "VDebugOverlay" );
	capture_interface( this->physics_surface_props, i_physics_surface_props, physics_dll, "VPhysicsSurfaceProps" );

	capture_ptr( this->device, **reinterpret_cast< IDirect3DDevice9*** >( g_game_modules->get( shaderpidx9_dll ).find_pattern( "A1 ? ? ? ? 50 8B 08 FF 51 0C", 0x1 ) ) );
	capture_ptr( this->key_values_system, reinterpret_cast< key_values_system_fn >( g_game_modules->get( vstdlib_dll ).get_proc_address( "KeyValuesSystem" ) )( ) );
	capture_ptr( this->client_mode, **reinterpret_cast< i_client_mode_shared*** >( c_memory::get_vfunc<std::uintptr_t>( client, 10 ) + 0x5 ) );
	capture_ptr( this->client_state, **reinterpret_cast< i_client_state*** >( c_memory::get_vfunc<std::uintptr_t>( engine, 12 ) + 0x10 ) );
	capture_ptr( this->weapon_system, *reinterpret_cast< i_weapon_system** >( g_game_modules->get( client_dll ).find_pattern( "8B 35 ? ? ? ? FF 10 0F B7 C0", 0x2 ) ) );
	capture_ptr( this->global_vars, **reinterpret_cast< i_global_vars_base*** >( c_memory::get_vfunc<std::uintptr_t>( client, 11 ) + 0xA ) );
	capture_ptr( this->input, *reinterpret_cast< i_input** >( c_memory::get_vfunc<std::uintptr_t>( client, 16 ) + 0x1 ) );
	capture_ptr( this->glow_manager, *reinterpret_cast< i_glow_object_manager** >( g_game_modules->get( client_dll ).find_pattern( "0F 11 05 ? ? ? ? 83 C8 01", 0x3 ) ) );

	return true;
}

// remove the macros
#ifdef capture_interface
#undef capture_interface
#endif // capture_interface
#ifdef capture_ptr
#undef capture_ptr
#endif // capture_ptr