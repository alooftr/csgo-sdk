#pragma once

// used:std::unordered_map
#include <unordered_map>
// used: c_memory
#include "../utilities/memory.h"


// including all interfaces class
#include "interfaces/ibaseclient.h"
#include "interfaces/isurface.h"
#include "interfaces/ipanel.h"
#include "interfaces/iengine.h"

/* holding all the game's loaded module */
class c_game_modules
{
public:
	bool setup( );

	auto get( std::string name )
	{
		return this->modules[ name ];
	}
private:
	std::unordered_map<std::string, c_module> modules = { };
};

/* holding game's interfaces pointer */
class c_game_interfaces
{
public:
	bool setup( );

	i_base_client* client = nullptr;
	i_surface* surface = nullptr;
	IDirect3DDevice9* device = nullptr;
	HWND window_handle = nullptr;
	i_panel* panel = nullptr;
	i_engine_client* engine = nullptr;
};

// const pointer
inline const std::unique_ptr<c_game_modules> g_game_modules{ new c_game_modules( ) };
inline const std::unique_ptr<c_game_interfaces> g_game_interfaces{ new c_game_interfaces( ) };