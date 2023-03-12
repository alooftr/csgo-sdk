#include "modules.h"

bool c_game_modules::setup( )
{
	this->modules[ fnv1a::hash( engine_dll ) ] = c_memory::get_module_handle( engine_dll );
	this->modules[ fnv1a::hash( client_dll ) ] = c_memory::get_module_handle( client_dll );
	this->modules[ fnv1a::hash( localize_dll ) ] = c_memory::get_module_handle( localize_dll );
	this->modules[ fnv1a::hash( materialsystem_dll ) ] = c_memory::get_module_handle( materialsystem_dll );
	this->modules[ fnv1a::hash( vgui_dll ) ] = c_memory::get_module_handle( vgui_dll );
	this->modules[ fnv1a::hash( vgui2_dll ) ] = c_memory::get_module_handle( vgui2_dll );
	this->modules[ fnv1a::hash( shaderpidx9_dll ) ] = c_memory::get_module_handle( shaderpidx9_dll );
	this->modules[ fnv1a::hash( gameoverlayrenderer_dll ) ] = c_memory::get_module_handle( gameoverlayrenderer_dll );
	this->modules[ fnv1a::hash( physics_dll ) ] = c_memory::get_module_handle( physics_dll );
	this->modules[ fnv1a::hash( vstdlib_dll ) ] = c_memory::get_module_handle( vstdlib_dll );
	this->modules[ fnv1a::hash( tier0_dll ) ] = c_memory::get_module_handle( tier0_dll );
	this->modules[ fnv1a::hash( inputsystem_dll ) ] = c_memory::get_module_handle( inputsystem_dll );
	this->modules[ fnv1a::hash( studiorender_dll ) ] = c_memory::get_module_handle( studiorender_dll );
	this->modules[ fnv1a::hash( datacache_dll ) ] = c_memory::get_module_handle( datacache_dll );
	this->modules[ fnv1a::hash( steam_api_dll ) ] = c_memory::get_module_handle( steam_api_dll );
	this->modules[ fnv1a::hash( matchmaking_dll ) ] = c_memory::get_module_handle( matchmaking_dll );
	this->modules[ fnv1a::hash( server_dll ) ] = c_memory::get_module_handle( server_dll );
	this->modules[ fnv1a::hash( serverbrowser_dll ) ] = c_memory::get_module_handle( serverbrowser_dll );
	this->modules[ fnv1a::hash( filesystem_stdio_dll ) ] = c_memory::get_module_handle( filesystem_stdio_dll );

	return !this->modules.empty( );
}
