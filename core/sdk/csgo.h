#pragma once

// used:std::unordered_map
#include <unordered_map>
// used: c_memory and c_game_modules
#include "../utilities/modules.h"

// including all interfaces class
#include "interfaces/ibaseclient.h"
#include "interfaces/isurface.h"
#include "interfaces/ipanel.h"
#include "interfaces/iengine.h"
#include "interfaces/ikeyvaluesys.h"
#include "interfaces/ientitylist.h"
#include "interfaces/imodelinfo.h"
#include "interfaces/igametypes.h"
#include "interfaces/imdlcache.h"
#include "interfaces/iconvar.h"
#include "interfaces/iglobalvars.h"
#include "interfaces/ilocalize.h"
#include "interfaces/iprediction.h"
#include "interfaces/ifilesystem.h"
#include "interfaces/imaterialsystem.h"
#include "interfaces/imodelrender.h"
#include "interfaces/idebugoverlay.h"
#include "interfaces/iinput.h"
#include "interfaces/iphysicsurfaceprop.h"
#include "interfaces/iglowmanager.h"

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
	i_key_values_system* key_values_system = nullptr;
	i_client_entity_list* entity_list = nullptr;
	i_game_types* game_types = nullptr;
	i_model_info* model_info = nullptr;
	i_engine_trace* engine_trace = nullptr;
	i_mdl_cache* mdl_cache = nullptr;
	i_client_mode_shared* client_mode = nullptr;
	i_client_state* client_state = nullptr;
	i_convar* convar = nullptr;
	i_weapon_system* weapon_system = nullptr;
	i_global_vars_base* global_vars = nullptr;
	i_localize* localize = nullptr;
	// we will grab move_helper in run_command hook later
	i_move_helper* move_helper = nullptr;
	i_prediction* prediction = nullptr;
	i_game_movement* game_movement = nullptr;
	i_file_system* file_system = nullptr;
	i_material_system* material_system = nullptr;
	i_model_render* model_render = nullptr;
	i_studio_render* studio_render = nullptr;
	i_debug_overlay* debug_overlay = nullptr;
	i_input* input = nullptr;
	i_physics_surface_props* physics_surface_props = nullptr;
	i_glow_object_manager* glow_manager = nullptr;
};

// const pointer
inline const std::unique_ptr<c_game_interfaces> g_game_interfaces{ new c_game_interfaces( ) };