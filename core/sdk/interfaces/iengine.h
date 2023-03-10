#pragma once

#pragma once
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/cdll_int.h

// used: viewmatrix_t, matrix3x4_t
#include "../data_types/matrix.h"
// used: crc32 hashing
#include "../../utilities/crc32.h"

#pragma region engineclient_enumerations
enum e_client_frame_stage : int
{
	frame_undefined = -1,
	frame_start,
	// a network packet is being recieved
	frame_net_update_start,
	// data has been received and we are going to start calling postdataupdate
	frame_net_update_postdataupdate_start,
	// data has been received and called postdataupdate on all data recipients
	frame_net_update_postdataupdate_end,
	// received all packets, we can now do interpolation, prediction, etc
	frame_net_update_end,
	// start rendering the scene
	frame_render_start,
	// finished rendering the scene
	frame_render_end,
	frame_net_full_frame_update_on_remove
};

enum e_render_view_info : int
{
	renderview_unspecified = 0,
	renderview_drawviewmodel = ( 1 << 0 ),
	renderview_drawhud = ( 1 << 1 ),
	renderview_suppressmonitorrendering = ( 1 << 2 )
};
#pragma endregion

struct player_info_t
{
	std::uint64_t	version = 0ULL;
	union
	{
		std::uint64_t xuid;
		struct
		{
			std::uint32_t xuid_low;
			std::uint32_t xuid_high;
		};
	};

	char			name[ 128 ];
	int				user_id;
	char			steam_id[ 33 ];
	std::uint32_t	friends_id;
	char			friends_name[ 128 ];
	bool			fake_player;
	bool			is_hltv;
	crc32_t			custom_files[ 4 ];
	std::uint8_t	files_downloaded;
};

struct audio_state_t
{
	vec3_t			origin;
	vec3_t			angles;
	bool			is_underwater;
};

struct steam_api_context_t;
class i_net_channel_info;

class i_engine_client
{
public:
	void get_screen_size( int& width, int& height )
	{
		c_memory::call_vfunc<void>( this, 5, std::ref( width ), std::ref( height ) );
	}

	bool get_player_info( int entity_index, player_info_t* info )
	{
		return c_memory::call_vfunc<bool>( this, 8, entity_index, info );
	}

	int get_player_for_user_id( int user_id )
	{
		return c_memory::call_vfunc<int>( this, 9, user_id );
	}

	bool is_console_visible( )
	{
		return c_memory::call_vfunc<bool>( this, 11 );
	}

	int get_local_player( )
	{
		return c_memory::call_vfunc<int>( this, 12 );
	}

	float get_last_time_stamp( )
	{
		return c_memory::call_vfunc<float>( this, 14 );
	}

	vec3_t get_view_angles( )
	{
		vec3_t temp{};
		c_memory::call_vfunc<void>( this, 18, std::ref( temp ) );

		return temp;
	}

	void set_view_angles( vec3_t& view )
	{
		c_memory::call_vfunc<void>( this, 19, std::ref( view ) );
	}

	int get_max_clients( )
	{
		return c_memory::call_vfunc<int>( this, 20 );
	}

	// returns true if the player is fully connected and active in game (i.e, not still loading) and for check doesnt need isconnected
	bool is_in_game( )
	{
		return c_memory::call_vfunc<bool>( this, 26 );
	}

	// returns true if the player is connected, but not necessarily active in game (could still be loading)
	bool is_connected( )
	{
		return c_memory::call_vfunc<bool>( this, 27 );
	}

	bool is_drawing_loading_image( )
	{
		return c_memory::call_vfunc<bool>( this, 28 );
	}

	const view_matrix_t& world_to_screen_matrix( )
	{
		return c_memory::call_vfunc<const view_matrix_t&>( this, 37 );
	}

	void* get_bsp_tree_query( )
	{
		return c_memory::call_vfunc<void*>( this, 43 );
	}

	const char* get_level_name( )
	{
		return c_memory::call_vfunc<const char*>( this, 52 );
	}

	const char* get_level_name_short( )
	{
		return c_memory::call_vfunc<const char*>( this, 53 );
	}

	void fire_event( )
	{
		return c_memory::call_vfunc<void>( this, 59 );
	}

	i_net_channel_info* get_net_channel_info( )
	{
		return c_memory::call_vfunc<i_net_channel_info*>( this, 78 );
	}

	bool is_playing_demo( )
	{
		return c_memory::call_vfunc<bool>( this, 82 );
	}

	bool is_recording_demo( )
	{
		return c_memory::call_vfunc<bool>( this, 83 );
	}

	bool is_playing_time_demo( )
	{
		return c_memory::call_vfunc<bool>( this, 84 );
	}

	bool is_taking_screenshot( )
	{
		return c_memory::call_vfunc<bool>( this, 92 );
	}

	bool is_hltv( )
	{
		return c_memory::call_vfunc<bool>( this, 93 );
	}

	unsigned int get_engine_build_number( )
	{
		return c_memory::call_vfunc<unsigned int>( this, 104 );
	}

	const char* get_product_version_string( )
	{
		return c_memory::call_vfunc<const char*>( this, 105 );
	}

	void execute_client_cmd( const char* cmd_string )
	{
		c_memory::call_vfunc<void>( this, 108, cmd_string );
	}

	void client_cmd_unrestricted( const char* cmd_string, bool from_console_or_keybind = false )
	{
		c_memory::call_vfunc<void>( this, 114, cmd_string, from_console_or_keybind );
	}

	steam_api_context_t* get_steam_api_context( )
	{
		return c_memory::call_vfunc<steam_api_context_t*>( this, 185 );
	}

	bool is_voice_recording( )
	{
		return c_memory::call_vfunc<bool>( this, 224 );
	}
};
