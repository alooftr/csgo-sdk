#include "modules.h"

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
