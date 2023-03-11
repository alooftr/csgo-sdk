#pragma once

// @credits: https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/shared/igamemovement.h
class c_move_data
{
public:
	bool			first_run_of_functions : 1;
	bool			game_code_moved_player : 1;
	bool			no_air_control : 1;
	std::uintptr_t	player_handle;		// edict index on server, client entity handle on client=
	int				impulse_command;	// impulse command issued.
	vec3_t			view_angles;		// command view angles (local space)
	vec3_t			abs_view_angles;	// command view angles (world space)
	int				buttons;			// attack buttons.
	int				old_buttons;		// from host_client->oldbuttons;
	float			forward_move;
	float			side_move;
	float			up_move;
	float			max_speed;
	float			client_max_speed;
	vec3_t			velocity;		// edict::velocity	// current movement direction.
	vec3_t			trailing_velocity;
	float			trailing_velocity_time;
	vec3_t			angles;			// edict::angles
	vec3_t			old_angles;
	float			out_step_height;	// how much you climbed this move
	vec3_t			out_wish_vel;		// this is where you tried 
	vec3_t			out_jump_vel;		// this is your jump velocity
	vec3_t			constraint_center;
	float			constraint_radius;
	float			constraint_width;
	float			constraint_speed_factor;
	bool			constraint_past_radius;
	vec3_t			abs_origin;
};

// @credits: https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/shared/imovehelper.h
class c_base_entity;
class i_physics_surface_props;
class c_game_trace;
enum e_sound_level;
class i_move_helper
{
public:
	virtual	const char* get_name( void* ent ) const = 0;
	virtual void				set_host( c_base_entity* host ) = 0;
	virtual void				reset_touch_list( ) = 0;
	virtual bool				add_to_touched( const c_game_trace& trace, const vec3_t& impact_velocity ) = 0;
	virtual void				process_impacts( ) = 0;
	virtual void				con_n_printf( int index, char const* fmt, ... ) = 0;
	virtual void				start_sound( const vec3_t& origin, int channel, char const* sample, float volume, e_sound_level sound_level, int flags, int pitch ) = 0;
	virtual void				start_sound( const vec3_t& origin, const char* soundname ) = 0;
	virtual void				playback_event_full( int flags, int client_index, unsigned short event_index, float delay, vec3_t& origin, vec3_t& angles, float param1, float param2, int param3, int param4, int param5, int param6 ) = 0;
	virtual bool				player_falling_damage( ) = 0;
	virtual void				player_set_animation( int player_animation ) = 0;
	virtual i_physics_surface_props* get_surface_props( ) = 0;
	virtual bool				is_world_entity( const unsigned long& ent ) = 0;
};

class i_game_movement
{
public:
	virtual						~i_game_movement( )
	{ }
	virtual void				process_movement( c_base_entity* ent, c_move_data* move ) = 0;
	virtual void				reset( ) = 0;
	virtual void				start_track_prediction_errors( c_base_entity* ent ) = 0;
	virtual void				finish_track_prediction_errors( c_base_entity* ent ) = 0;
	virtual void				diff_print( char const* fmt, ... ) = 0;
	virtual vec3_t const& get_player_mins( bool ducked ) const = 0;
	virtual vec3_t const& get_player_maxs( bool ducked ) const = 0;
	virtual vec3_t const& get_player_view_offset( bool ducked ) const = 0;
	virtual bool				is_moving_player_stuck( ) const = 0;
	virtual c_base_entity* get_moving_player( ) const = 0;
	virtual void				unblock_pusher( c_base_entity* ent, c_base_entity* pusher ) = 0;
	virtual void				setup_movement_bounds( c_move_data* move ) = 0;
};

class i_prediction
{
public:
	std::byte		pad0[ 0x4 ];						// 0x0000
	std::uintptr_t	last_ground;					// 0x0004
	bool			in_prediction;					// 0x0008
	bool			old_cl_predict_value;				// 0x000B
	bool			is_first_time_predicted;			// 0x0009
	bool			engine_paused;					// 0x000A
	int				previous_start_frame;			// 0x000C
	int				incoming_packet_number;			// 0x0010
	float			last_server_world_time_stamp;		// 0x0014

	struct split_t
	{
		bool		is_first_time_predicted;			// 0x0018
		std::byte	pad0[ 0x3 ];						// 0x0019
		int			commands_predicted;				// 0x001C
		int			server_commands_acknowledged;	// 0x0020
		int			previous_ack_had_errors;			// 0x0024
		float		ideal_pitch;					// 0x0028
		int			last_command_acknowledged;		// 0x002C
		bool		previous_ack_error_triggers_full_latch_reset; // 0x0030
		c_utl_vector<c_base_handle> entities_with_prediction_errors_in_last_ack; // 0x0031
		bool		performed_tick_shift;			// 0x0045
	};

	split_t			split[ 1 ];						// 0x0018
	// SavedGlobals 0x4C

public:
	void update( int start_frame, bool valid_frame, int incoming_acknowledged, int outgoing_command )
	{
		c_memory::call_vfunc<void>( this, 3, start_frame, valid_frame, incoming_acknowledged, outgoing_command );
	}

	void get_local_view_angles( vec3_t& view )
	{
		c_memory::call_vfunc<void>( this, 12, std::ref( view ) );
	}

	void set_local_view_angles( vec3_t& view )
	{
		c_memory::call_vfunc<void>( this, 13, std::ref( view ) );
	}

	void check_moving_ground( c_base_entity* ent, double frametime )
	{
		c_memory::call_vfunc<void>( this, 18, ent, frametime );
	}

	void setup_move( c_base_entity* ent, c_user_cmd* cmd, i_move_helper* helper, c_move_data* move_data )
	{
		c_memory::call_vfunc<void>( this, 20, ent, cmd, helper, move_data );
	}

	void finish_move( c_base_entity* ent, c_user_cmd* cmd, c_move_data* move_data )
	{
		c_memory::call_vfunc<void>( this, 21, ent, cmd, move_data );
	}
};
