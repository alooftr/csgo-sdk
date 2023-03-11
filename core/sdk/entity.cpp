#include "entity.h"
// used: g_game_interfaces
#include "csgo.h"
// used: math library
#include "../utilities/math.h"
// used: g_utils
#include "../utilities/utilities.h"

c_base_entity* c_base_entity::get_local_player( )
{
	return g_game_interfaces->entity_list->get<c_base_entity>( g_game_interfaces->engine->get_local_player( ) );
}

int c_base_entity::get_sequence_activity( int sequence )
{
	studiohdr_t* studio_hdr = g_game_interfaces->model_info->get_studio_model( this->get_model( ) );

	if ( studio_hdr == nullptr )
		return -1;

	using get_sequence_activity_fn = int( __fastcall* )( void*, void*, int );
	static auto o_get_sequence_activity = reinterpret_cast< get_sequence_activity_fn >( c_memory::find_pattern( "client.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83" ) ); // @xref: "Need to handle the activity %d\n"
	assert( o_get_sequence_activity != nullptr );

	return o_get_sequence_activity( this, studio_hdr, sequence );
}

int c_base_entity::get_max_health( )
{
	//check is dangerzone
	if ( g_game_interfaces->game_types->get_current_game_type( ) == gametype_freeforall )
		return 120;

	return 100;
}


void c_base_entity::modify_eye_position( const c_csgo_player_anim_state* anim_state, vec3_t* position ) const
{ 
	// @ida modifyeyeposition: client.dll @ 55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 38

	if ( g_game_interfaces->engine->is_hltv( ) || g_game_interfaces->engine->is_playing_demo( ) )
		return;

	c_base_entity* base_entity = anim_state->player;

	if ( base_entity == nullptr )
		return;

	i_client_entity* ground_entity = g_game_interfaces->entity_list->get_client_entity_from_handle( base_entity->get_ground_entity_handle( ) );

	if ( !anim_state->landing && anim_state->duck_amount == 0.f && ground_entity != nullptr )
		return;

	if ( const auto head_position = base_entity->get_bone_position( base_entity->get_bone_by_hash( fnv1a::hash_const( "head_0" ) ) ); head_position.has_value( ) )
	{
		vec3_t head = head_position.value( );
		head.z += 1.7f;

		if ( head.z < position->z )
		{
			float factor = 0.f;
			const float delta = std::fabsf( position->z - head.z );
			const float offset = ( delta - 4.0f ) / 6.0f;

			if ( offset >= 0.f )
				factor = std::min( offset, 1.0f );

			const float factor_squared = ( factor * factor );
			position->z += ( ( head.z - position->z ) * ( ( factor_squared * 3.0f ) - ( ( factor_squared * 2.0f ) * factor ) ) );
		}
	}
}

std::optional<vec3_t> c_base_entity::get_bone_position( int bone_index )
{
	assert( bone_index > bone_invalid && bone_index < MAXSTUDIOBONES ); // given invalid bone index for getboneposition

	std::array<matrix3x4_t, MAXSTUDIOBONES> bones_to_world = { };

	if ( this->setup_bones( bones_to_world.data( ), bones_to_world.size( ), BONE_USED_BY_ANYTHING, 0.f ) )
		return bones_to_world.at( bone_index ).at( 3 );

	return std::nullopt;
}

mstudiobbox_t* c_base_entity::get_hitbox( int hb_id )
{
	assert( hb_id > hitbox_invalid && hb_id < hitbox_max ); // given invalid hitbox index for gethitboxposition

	std::array<matrix3x4_t, MAXSTUDIOBONES> bones_to_world = { };

	if ( const auto model = this->get_model( ); model != nullptr )
	{
		if ( const auto studio_hdr = g_game_interfaces->model_info->get_studio_model( model ); studio_hdr != nullptr )
		{
			return studio_hdr->get_hitbox( hb_id, 0 );
		}
	}

	return nullptr;
}

int c_base_entity::get_bone_by_hash( const fnv1a_t bone_hash ) const
{
	if ( const auto model = this->get_model( ); model != nullptr )
	{
		if ( const auto studio_hdr = g_game_interfaces->model_info->get_studio_model( model ); studio_hdr != nullptr )
		{
			for ( int i = 0; i < studio_hdr->bones; i++ )
			{
				if ( const auto bone = studio_hdr->get_bone( i ); bone != nullptr && fnv1a::hash( bone->get_name( ) ) == bone_hash )
					return i;
			}
		}
	}

	return bone_invalid;
}

std::optional<vec3_t> c_base_entity::get_hitbox_position( const int iHitbox )
{
	assert( iHitbox > hitbox_invalid && iHitbox < hitbox_max ); // given invalid hitbox index for gethitboxposition

	std::array<matrix3x4_t, MAXSTUDIOBONES> bones_to_world = { };

	if ( const auto hitbox = this->get_hitbox( iHitbox ); hitbox != nullptr )
	{
		if ( this->setup_bones( bones_to_world.data( ), bones_to_world.size( ), BONE_USED_BY_HITBOX, 0.f ) )
		{
			// get mins/maxs by bone
			vec3_t min, max;
			g_math::transform_vector( hitbox->bb_min, bones_to_world.at( hitbox->bone ), min );
			g_math::transform_vector( hitbox->bb_max, bones_to_world.at( hitbox->bone ), max );

			// get center
			return ( min + max ) * 0.5f;
		}
	}

	return std::nullopt;
}

std::optional<vec3_t> c_base_entity::get_hit_group_position( const int hit_group )
{
	assert( hit_group >= hitgroup_generic && hit_group <= hitgroup_gear ); // given invalid hitbox index for gethitgroupposition

	std::array<matrix3x4_t, MAXSTUDIOBONES> bones_to_world = { };

	if ( const auto model = this->get_model( ); model != nullptr )
	{
		if ( const auto studio_hdr = g_game_interfaces->model_info->get_studio_model( model ); studio_hdr != nullptr )
		{
			if ( const auto hitboxSet = studio_hdr->get_hitbox_set( this->get_hitbox_set( ) ); hitboxSet != nullptr )
			{
				if ( this->setup_bones( bones_to_world.data( ), bones_to_world.size( ), BONE_USED_BY_HITBOX, 0.f ) )
				{
					mstudiobbox_t* hitbox = nullptr;
					for ( int i = 0; i < hitboxSet->hitboxes; i++ )
					{
						hitbox = hitboxSet->get_hitbox( i );

						// check is reached needed group
						if ( hitbox->group == hit_group )
							break;
					}

					if ( hitbox != nullptr )
					{
						// get mins/maxs by bone
						vec3_t min, max;
						g_math::transform_vector( hitbox->bb_min, bones_to_world.at( hitbox->bone ), min );
						g_math::transform_vector( hitbox->bb_max, bones_to_world.at( hitbox->bone ), max );

						// get center
						return ( min + max ) * 0.5f;
					}
				}
			}
		}
	}

	return std::nullopt;
}

void c_base_entity::post_think( )
{
	// @ida postthink: client.dll 56 8B 35 ? ? ? ? 57 8B F9 8B CE 8B 06 FF 90 ? ? ? ? 8B 07

	using post_think_v_physics_fn = bool( __thiscall* )( c_base_entity* );
	static auto o_post_think_v_physics = reinterpret_cast< post_think_v_physics_fn >( c_memory::find_pattern( "client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB" ) );
	assert( o_post_think_v_physics != nullptr );

	using simulate_player_simulated_entities_fn = void( __thiscall* )( c_base_entity* );
	static auto o_simulate_player_simulated_entities = reinterpret_cast< simulate_player_simulated_entities_fn >( c_memory::find_pattern( "client.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74" ) );
	assert( o_simulate_player_simulated_entities != nullptr );

	g_game_interfaces->mdl_cache->begin_lock( );

	if ( this->is_alive( ) )
	{
		this->update_collision_bounds( );

		if ( this->get_flags( ) & fl_onground )
			*this->get_fall_velocity( ) = 0.f;

		if ( this->get_sequence( ) == -1 )
			this->set_sequence( 0 );

		this->studio_frame_advance( );
		o_post_think_v_physics( this );
	}

	o_simulate_player_simulated_entities( this );

	g_game_interfaces->mdl_cache->end_lock( );
}

bool c_base_entity::is_enemy( c_base_entity* entity )
{
	//check is dangerzone
	if ( g_game_interfaces->game_types->get_current_game_type( ) == gametype_freeforall )
		// check is not teammate
		return ( this->get_survival_team( ) != entity->get_survival_team( ) );

	static c_convar* mp_teammates_are_enemies = g_game_interfaces->convar->find_var( "mp_teammates_are_enemies" );

	// check is deathmatch
	if ( mp_teammates_are_enemies != nullptr && mp_teammates_are_enemies->get_bool( ) && this->get_team( ) == entity->get_team( ) && this != entity )
		return true;

	if ( this->get_team( ) != entity->get_team( ) )
		return true;

	return false;
}

bool c_base_entity::is_targeting_local( c_base_entity* local )
{
	vec3_t forward = { };
	const vec3_t view = this->get_eye_angles( );
	g_math::angle_vectors( view, &forward );
	forward *= MAX_DISTANCE;

	const vec3_t start = this->get_eye_position( );
	const vec3_t end_pos = start + forward;

	const ray_t ray( start, end_pos );
	c_trace_filter filter( this );

	trace_t trace = { };
	g_game_interfaces->engine_trace->trace_ray( ray, MASK_SHOT, &filter, &trace );

	if ( trace.hit_entity == local )
		return true;

	return false;
}

bool c_base_entity::is_visible( c_base_entity* entity, const vec3_t& end_pos, bool smoke_check )
{
	const vec3_t vecStart = this->get_eye_position( false );

	const ray_t ray( vecStart, end_pos );
	c_trace_filter filter( this );

	trace_t trace = { };
	g_game_interfaces->engine_trace->trace_ray( ray, MASK_SHOT, &filter, &trace );

	// trace check
	if ( ( trace.is_visible( ) || trace.hit_entity == entity ) &&
		// smoke check
		!( smoke_check && g_utils::line_goes_through_smoke( vecStart, end_pos ) ) )
		return true;

	return false;
}

bool c_base_entity::is_reloading( )
{
	auto animLayer = this->get_animation_layer( 1 );

	auto sequence = animLayer->sequence;
	if ( sequence )
	{
		auto activity = this->get_sequence_activity( sequence );

		return activity == act_csgo_reload && animLayer->weight;
	}

	return false;
}

c_weapon_cs_base* c_base_entity::get_active_weapon( )
{
	return g_game_interfaces->entity_list->get<c_weapon_cs_base>( this->get_active_weapon_handle( ) );
}

bool c_base_entity::has_c4( )
{
	using has_c4_fn = bool( __thiscall* )( void* );
	static auto o_has_c4 = reinterpret_cast< has_c4_fn >( c_memory::find_pattern( "client.dll", "56 8B F1 85 F6 74 31" ) );

	assert( o_has_c4 != nullptr );

	return o_has_c4( this );
}

vec3_t c_base_entity::get_shoot_pos( bool interpolated )
{
	if ( !this )
		return vec3_t( );

	if ( !interpolated )
		return this->get_origin( ) + this->get_view_offset( );
	else
	{
		vec3_t temp{};
		c_memory::call_vfunc<void>( this, 169, &temp );
		return temp;
	}
}

bool c_base_entity::is_breakable( )
{
	// @ida isbreakableentity: client.dll @ 55 8B EC 51 56 8B F1 85 F6 74 68

	const int health = this->get_health( );

	// first check to see if it's already broken
	if ( health < 0 && this->is_max_health( ) > 0 )
		return true;

	if ( this->get_take_damage( ) != damage_yes )
	{
		const e_class_index class_index = this->get_client_class( )->class_id;

		// force pass cfuncbrush
		if ( class_index != e_class_index::cfuncbrush )
			return false;
	}

	if ( const int collision_group = this->get_collision_group( ); collision_group != collision_group_pushaway && collision_group != collision_group_breakable_glass && collision_group != collision_group_none )
		return false;

	if ( health > 200 )
		return false;

	if ( i_multiplayer_physics* physics_interface = dynamic_cast< i_multiplayer_physics* >( this ); physics_interface != nullptr )
	{
		if ( physics_interface->get_multiplayer_physics_mode( ) != physics_multiplayer_solid )
			return false;
	}
	else
	{
		if ( const char* class_name = this->get_classname( ); !strcmp( class_name, "func_breakable" ) || !strcmp( class_name, "func_breakable_surf" ) )
		{
			if ( !strcmp( class_name, "func_breakable_surf" ) )
			{
				c_breakable_surface* pSurface = static_cast< c_breakable_surface* >( this );

				// don't try to break it if it has already been broken
				if ( pSurface->is_broken( ) )
					return false;
			}
		}
		else if ( this->physics_solid_mask_for_entity( ) & CONTENTS_PLAYERCLIP )
		{
			// hostages and players use CONTENTS_PLAYERCLIP, so we can use it to ignore them
			return false;
		}
	}

	if ( i_breakable_with_prop_data* breakable_interface = dynamic_cast< i_breakable_with_prop_data* >( this ); breakable_interface != nullptr )
	{
		// bullets don't damage it - ignore
		if ( breakable_interface->get_dmg_mod_bullet( ) <= 0.0f )
			return false;
	}

	return true;
}

float c_inferno::get_max_time( )
{
	static auto inferno_flame_lifetime = g_game_interfaces->convar->find_var( "inferno_flame_lifetime" );
	if ( !inferno_flame_lifetime )
		return 7.0f;

	return inferno_flame_lifetime->get_float( );
}

c_weapon_data* c_base_combat_weapon::get_weapon_data( )
{
	return g_game_interfaces->weapon_system->get_weapon_data( this->get_item_definition_index( ) );
}

bool c_weapon_cs_base::can_shoot( )
{
	const float server_time = TICKS_TO_TIME( g_game_interfaces->client_state->clock_drift_mgr.server_tick );

	// check is have ammo
	if ( this->get_ammo( ) <= 0 )
		return false;

	// is player ready to shoot
	if ( this->get_next_attack( ) > server_time )
		return false;

	const short definition_index = this->get_item_definition_index( );

	// check is weapon with burst mode
	if ( ( definition_index == weapon_famas || definition_index == weapon_glock ) &&
		// check is burst mode
		this->is_burst_mode( ) && this->get_burst_shots_remaining( ) > 0 )
		return true;

	// is weapon ready to shoot
	if ( this->get_next_primary_attack( ) > server_time )
		return false;

	// check for revolver cocking ready
	if ( definition_index == weapon_revolver && this->get_fire_ready_time( ) > server_time )
		return false;

	return true;
}
