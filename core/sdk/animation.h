#pragma once

#include "data_types/utl_vector.h"

/* max animation layers */
#define MAXOVERLAYS 15

enum e_sequence_activity : int
{
	act_csgo_null = 957,
	act_csgo_defuse,
	act_csgo_defuse_with_kit,
	act_csgo_flashbang_reaction,
	act_csgo_fire_primary,
	act_csgo_fire_primary_opt_1,
	act_csgo_fire_primary_opt_2,
	act_csgo_fire_secondary,
	act_csgo_fire_secondary_opt_1,
	act_csgo_fire_secondary_opt_2,
	act_csgo_reload,
	act_csgo_reload_start,
	act_csgo_reload_loop,
	act_csgo_reload_end,
	act_csgo_operate,
	act_csgo_deploy,
	act_csgo_catch,
	act_csgo_silencer_detach,
	act_csgo_silencer_attach,
	act_csgo_twitch,
	act_csgo_twitch_buyzone,
	act_csgo_plant_bomb,
	act_csgo_idle_turn_balanceadjust,
	act_csgo_idle_adjust_stoppedmoving,
	act_csgo_alive_loop,
	act_csgo_flinch,
	act_csgo_flinch_head,
	act_csgo_flinch_molotov,
	act_csgo_jump,
	act_csgo_fall,
	act_csgo_climb_ladder,
	act_csgo_land_light,
	act_csgo_land_heavy,
	act_csgo_exit_ladder_top,
	act_csgo_exit_ladder_bottom
};

enum e_anim_layer
{
	animation_layer_aimmatrix,
	animation_layer_weapon_action,
	animation_layer_weapon_action_recrouch,
	animation_layer_adjust,
	animation_layer_movement_jump_or_fall,
	animation_layer_movement_land_or_climb,
	animation_layer_movement_move,
	animation_layer_movement_strafechange,
	animation_layer_whole_body,
	animation_layer_flashed,
	animation_layer_flinch,
	animation_layer_aliveloop,
	animation_layer_lean,
	animation_layer_count
};

class matrix3x4a_t;
class c_base_animating;
class c_bone_accessor
{
public:
	const c_base_animating* animating;		//0x00
	matrix3x4a_t* mat_bones;		//0x04
	int						readable_bones;	//0x08
	int						writable_bones;	//0x0C
}; // Size: 0x10

class c_animation_layer
{
public:
	float			animation_time;		//0x00
	float			fade_out;				//0x04
	void* studio_hdr;				//0x08
	int				dispatched_src;			//0x0C
	int				dispatched_dst;			//0x10
	int				order;					//0x14
	std::uintptr_t  sequence;				//0x18
	float			prev_cycle;			//0x1C
	float			weight;				//0x20
	float			weight_delta_rate;		//0x24
	float			playback_rate;			//0x28
	float			cycle;				//0x2C
	void* owner;					//0x30
	int				invalidate_physics_bits;	//0x34

	void reset( )
	{
		this->sequence = 0;
		this->weight = 0;
		this->weight_delta_rate = 0;
		this->playback_rate = 0;
		this->prev_cycle = 0;
		this->cycle = 0;
	}
}; // Size: 0x38

class c_base_entity;
struct animstate_pose_param_cache_t
{
public:
	bool initialized; //0
private:
	std::byte pad[ 0x3 ];
public:
	int index; //4
	const char* name; //8

	bool init( c_base_entity* player, const char* name );
	void set_value( c_base_entity* player, float value );
}; //Size: 0xC(12)

struct procedural_foot_t
{
	vec3_t pos_anim;
	vec3_t pos_anim_last;
	vec3_t pos_plant;
	vec3_t plant_vel;
	float lock_amount;
	float last_plant_time;
};//Size: 0x38(56)

class c_base_entity;
class c_base_combat_weapon;
class c_csgo_player_anim_state
{
public:
	void create( c_base_entity* entity )
	{
		using create_animation_state_fn = void( __thiscall* )( void*, c_base_entity* );
		static auto o_create_animation_state = reinterpret_cast< create_animation_state_fn >( c_memory::find_pattern( "client.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46" ) ); // @xref: "ggprogressive_player_levelup"

		if ( o_create_animation_state == nullptr )
			return;

		o_create_animation_state( this, entity );
	}

	void update( vec3_t view_angles )
	{
		using update_animation_state_fn = void( __vectorcall* )( void*, void*, float, float, float, void* );
		static auto o_update_animation_state = reinterpret_cast< update_animation_state_fn >( c_memory::find_pattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" ) ); // @xref: "%s_aim"

		if ( o_update_animation_state == nullptr )
			return;

		o_update_animation_state( this, nullptr, 0.0f, view_angles.y, view_angles.x, nullptr );
	}

	void reset( )
	{
		using reset_animation_state_fn = void( __thiscall* )( void* );
		static auto o_reset_animation_state = reinterpret_cast< reset_animation_state_fn >( c_memory::find_pattern( "client.dll", "56 6A 01 68 ? ? ? ? 8B F1" ) ); // @xref: "player_spawn"

		if ( o_reset_animation_state == nullptr )
			return;

		o_reset_animation_state( this );
	}

	char pad0[ 0x4 ]; // 0
	bool first_run_since_init; // 4
	char pad1[ 0x5b ]; // 5
	c_base_entity* player; // 96
	c_base_combat_weapon* weapon; // 100
	c_base_combat_weapon* weapon_last; // 104
	float last_update_time; // 108
	int last_update_frame; // 112
	float last_update_increment; // 116
	float eye_yaw; // 120
	float eye_pitch; // 124
	float foot_yaw; // 128
	float foot_yaw_last; // 132
	float move_yaw; // 136
	float move_yaw_ideal; // 140
	float move_yaw_current_to_ideal; // 144
	float time_to_align_lower_body; // 148
	float primary_cycle; // 152
	float move_weight; // 156
	float move_weight_smoothed; // 160
	float duck_amount; // 164
	float duck_additional; // 168
	float recrouch_weight; // 172
	vec3_t position_current; // 176
	vec3_t position_last; // 188
	vec3_t velocity; // 200
	vec3_t velocity_normalized; // 212
	vec3_t velocity_normalized_non_zero; // 224
	float velocity_length_xy; // 236
	float velocity_length_z; // 240
	float speed_as_portion_of_run_top_speed; // 244
	float speed_as_portion_of_walk_top_speed; // 248
	float speed_as_portion_of_crouch_top_speed; // 252
	float duration_moving; // 256
	float duration_still; // 260
	bool on_ground; // 264
	bool landing; // 265
	char pad2[ 0x12 ]; // 266
	float walk_to_run_transition; // 284
	char pad3[ 0x4 ]; // 288
	float in_air_smooth_value; // 292
	bool on_ladder; // 296
	char pad4[ 0x3 ]; // 297
	float ladder_weight; // 300
	char pad5[ 0x2c ]; // 304
	vec3_t velocity_last; // 348
	char pad6[ 0x45 ]; // 360
	bool player_is_accelerating; // 429
	char pad7[ 0x2 ]; // 430
	animstate_pose_param_cache_t pose_param_mappings[ 20 ]; // 432
	float duration_move_weight_is_too_high; // 672
	float static_approach_speed; // 676
	int previous_move_state; // 680
	float stutter_step; // 684
	char pad8[ 0x80 ]; // 688
	float yaw_min; // 816
	float yaw_max; // 820
	float pitch_min; // 824
	float pitch_max; // 828
	int animstate_version; // 832

	// custom members
	float lower_body_realign_timer;
};
