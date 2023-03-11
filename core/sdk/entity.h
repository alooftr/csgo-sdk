#pragma once

// used: fnv1a hash
#include "../utilities/fnv1a.h"
// used: vector struct
#include "data_types/vector.h"
// used: c_memory
#include "../utilities/modules.h"
// used: data_map_t
#include "data_types/map.h"
// used: c_bf_read, c_bf_write
#include "data_types/bitbuf.h"
// used: c_user_cmd
#include "data_types/user_cmd.h"
// used: netvar manager class
#include "netvar/netvar.h"
// used: game's hitbox studio
#include "data_types/studio.h"
// used: commonly used enums/macros in game and prop physics class
#include "interfaces/ipropphysics.h"
// used: irefcount interface
#include "interfaces/irefcount.h"
// used: game's animations ( anim_state, anim_layer )
#include "animation.h"

using c_base_handle = std::uintptr_t;

#pragma region entities_definitions
#define invalid_ehandle_index		0xFFFFFFFF
#define num_ent_entry_bits			(11 + 2)
#define num_ent_entries				(1 << num_ent_entry_bits)
#define num_serial_num_bits			16
#define num_serial_num_shift_bits	(32 - num_serial_num_bits)
#define ent_entry_mask				((1 << num_serial_num_bits) - 1)
#pragma endregion

#pragma region entities_enumerations
enum e_data_update_type : int
{
	data_update_created = 0,
	data_update_datatable_changed,
};

enum e_team_id : int
{
	team_unassigned = 0,
	team_spectator,
	team_tt,
	team_ct
};

enum e_think_methods : int
{
	think_fire_all_functions = 0,
	think_fire_base_only,
	think_fire_all_but_base,
};

enum e_item_definition_index : short
{
	weapon_none = 0,
	weapon_deagle = 1,
	weapon_elite = 2,
	weapon_fiveseven = 3,
	weapon_glock = 4,
	weapon_ak47 = 7,
	weapon_aug = 8,
	weapon_awp = 9,
	weapon_famas = 10,
	weapon_g3sg1 = 11,
	weapon_galilar = 13,
	weapon_m249 = 14,
	weapon_m4a1 = 16,
	weapon_mac10 = 17,
	weapon_p90 = 19,
	weapon_zone_repulsor = 20,
	weapon_mp5sd = 23,
	weapon_ump45 = 24,
	weapon_xm1014 = 25,
	weapon_bizon = 26,
	weapon_mag7 = 27,
	weapon_negev = 28,
	weapon_sawedoff = 29,
	weapon_tec9 = 30,
	weapon_taser = 31,
	weapon_hkp2000 = 32,
	weapon_mp7 = 33,
	weapon_mp9 = 34,
	weapon_nova = 35,
	weapon_p250 = 36,
	weapon_shield = 37,
	weapon_scar20 = 38,
	weapon_sg556 = 39,
	weapon_ssg08 = 40,
	weapon_knife_gg = 41,
	weapon_knife = 42,
	weapon_flashbang = 43,
	weapon_hegrenade = 44,
	weapon_smokegrenade = 45,
	weapon_molotov = 46,
	weapon_decoy = 47,
	weapon_incgrenade = 48,
	weapon_c4 = 49,
	weapon_healthshot = 57,
	weapon_knife_t = 59,
	weapon_m4a1_silencer = 60,
	weapon_usp_silencer = 61,
	weapon_cz75a = 63,
	weapon_revolver = 64,
	weapon_tagrenade = 68,
	weapon_fists = 69,
	weapon_breachcharge = 70,
	weapon_tablet = 72,
	weapon_melee = 74,
	weapon_axe = 75,
	weapon_hammer = 76,
	weapon_spanner = 78,
	weapon_knife_ghost = 80,
	weapon_firebomb = 81,
	weapon_diversion = 82,
	weapon_frag_grenade = 83,
	weapon_snowball = 84,
	weapon_bumpmine = 85,
	weapon_knife_bayonet = 500,
	weapon_knife_css = 503,
	weapon_knife_flip = 505,
	weapon_knife_gut = 506,
	weapon_knife_karambit = 507,
	weapon_knife_m9_bayonet = 508,
	weapon_knife_tactical = 509,
	weapon_knife_falchion = 512,
	weapon_knife_survival_bowie = 514,
	weapon_knife_butterfly = 515,
	weapon_knife_push = 516,
	weapon_knife_cord = 517,
	weapon_knife_canis = 518,
	weapon_knife_ursus = 519,
	weapon_knife_gypsy_jackknife = 520,
	weapon_knife_outdoor = 521,
	weapon_knife_stiletto = 522,
	weapon_knife_widowmaker = 523,
	weapon_knife_skeleton = 525,
	glove_studded_brokenfang = 4725,
	glove_studded_bloodhound = 5027,
	glove_t = 5028,
	glove_ct = 5029,
	glove_sporty = 5030,
	glove_slick = 5031,
	glove_leather_handwraps = 5032,
	glove_motorcycle = 5033,
	glove_specialist = 5034,
	glove_studded_hydra = 5035,
	special_agent_blueberries_buckshot = 4619,
	special_agent_two_times_mccoy_tacp = 4680,
	special_agent_commandor_mae_jamison = 4711,
	special_agent_1st_lieutenant_farlow,
	special_agent_john_kask,
	special_agent_bio_haz_specialist,
	special_agent_sergeant_bombson,
	special_agent_chem_haz_specialist,
	special_agent_rezan_the_redshirt = 4718,
	special_agent_sir_bloody_miami_darryl = 4726,
	special_agent_safecracker_voltzmann,
	special_agent_little_kev,
	special_agent_getaway_sally = 4730,
	special_agent_number_k = 4732,
	special_agent_sir_bloody_silent_darryl = 4733,
	special_agent_sir_bloody_skullhead_darryl,
	special_agent_sir_bloody_darryl_royale,
	special_agent_sir_bloody_loudmouth_darryl,
	special_agent_t = 5036,
	special_agent_ct = 5037,
	special_agent_ground_rebel = 5105,
	special_agent_osiris,
	special_agent_shahmat,
	special_agent_muhlik,
	special_agent_soldier = 5205,
	special_agent_enforcer,
	special_agent_slingshot,
	special_agent_street_soldier,
	special_agent_operator = 5305,
	special_agent_markus_delrow,
	special_agent_michael_syfers,
	special_agent_ava,
	special_agent_3rd_commando_company = 5400,
	special_agent_seal_team_6_soldier,
	special_agent_buckshot,
	special_agent_two_times_mccoy_usaf,
	special_agent_ricksaw,
	special_agent_dragomir = 5500,
	special_agent_maximus,
	special_agent_rezan_the_ready,
	special_agent_blackwolf = 5503,
	special_agent_the_doctor,
	special_agent_dragomir_footsoldiers,
	special_agent_b_squadron_officer = 5601
};

enum e_weapon_type : int
{
	weapontype_knife = 0,
	weapontype_pistol = 1,
	weapontype_submachinegun = 2,
	weapontype_rifle = 3,
	weapontype_shotgun = 4,
	weapontype_sniper = 5,
	weapontype_machinegun = 6,
	weapontype_c4 = 7,
	weapontype_placeholder = 8,
	weapontype_grenade = 9,
	weapontype_healthshot = 11,
	weapontype_fists = 12,
	weapontype_breachcharge = 13,
	weapontype_bumpmine = 14,
	weapontype_tablet = 15,
	weapontype_melee = 16
};
#pragma endregion

class i_handle_entity
{
public:
	virtual							~i_handle_entity( )
	{ }
	virtual void					set_ref_e_handle( const c_base_handle& hRef ) = 0;
	virtual const c_base_handle& get_ref_e_handle( ) const = 0;
};

class i_client_unknown;
class i_collideable
{
public:
	virtual i_handle_entity* get_entity_handle( ) = 0;
	virtual vec3_t& obb_mins( ) const = 0;
	virtual vec3_t& obb_maxs( ) const = 0;
};

class i_client_alpha_property
{
public:
	virtual i_client_unknown* get_i_client_unknown( ) = 0;
	virtual void set_alpha_modulation( std::uint8_t uAlpha ) = 0;
	virtual void set_render_fx( int nRenderFx, int nRenderMode, float flStartTime = FLT_MAX, float flDuration = 0.0f ) = 0;
	virtual void set_fade( float flGlobalFadeScale, float flDistFadeStart, float flDistFadeEnd ) = 0;
	virtual void set_desync_offset( int nOffset ) = 0;
	virtual void enable_alpha_modulation_override( bool bEnable ) = 0;
	virtual void enable_shadow_alpha_modulation_override( bool bEnable ) = 0;
	virtual void set_distance_fade_mode( int nFadeMode ) = 0;
};

class i_client_networkable;
class i_client_renderable;
class i_client_entity;
class c_base_entity;
class i_client_thinkable;
class i_client_unknown : public i_handle_entity
{
public:
	virtual i_collideable* get_collideable( ) = 0;
	virtual i_client_networkable* get_client_networkable( ) = 0;
	virtual i_client_renderable* get_client_renderable( ) = 0;
	virtual i_client_entity* get_i_client_entity( ) = 0;
	virtual c_base_entity* get_base_entity( ) = 0;
	virtual i_client_thinkable* get_client_thinkable( ) = 0;
	virtual i_client_alpha_property* get_client_alpha_property( ) = 0;
};

struct renderable_instance_t
{
	std::uint8_t uAlpha;
};

struct model_t;
using client_shadow_handle_t = std::uint16_t;
using client_render_handle_t = std::uint16_t;
using model_instance_handle_t = std::uint16_t;
class i_client_renderable
{
public:
	virtual i_client_unknown* get_i_client_unknown( ) = 0;
	virtual vec3_t& get_render_origin( ) = 0;
	virtual vec3_t& get_render_angles( ) = 0;
	virtual bool					should_draw( ) = 0;
	virtual int						get_render_flags( ) = 0;
	virtual bool					is_transparent( ) = 0;
	virtual client_shadow_handle_t	get_shadow_handle( ) const = 0;
	virtual client_render_handle_t& render_handle( ) = 0;
	virtual const model_t* get_model( ) const = 0;
	virtual int						draw_model( int nFlags, const renderable_instance_t& uInstance ) = 0;
	virtual int						get_body( ) = 0;
	virtual void					get_color_modulation( float* pColor ) = 0;
	virtual bool					lod_test( ) = 0;
	virtual bool					setup_bones( matrix3x4_t* matBoneToWorldOut, int nMaxBones, int fBoneMask, float flCurrentTime ) = 0;
	virtual void					setup_weights( const matrix3x4_t* matBoneToWorld, int nFlexWeightCount, float* flFlexWeights, float* flFlexDelayedWeights ) = 0;
	virtual void					do_animation_events( ) = 0;
	virtual void* get_pvs_notify_interface( ) = 0;
	virtual void					get_render_bounds( vec3_t& vecMins, vec3_t& vecMaxs ) = 0;
	virtual void					get_render_bounds_worldspace( vec3_t& vecMins, vec3_t& vecMaxs ) = 0;
	virtual void					get_shadow_render_bounds( vec3_t& vecMins, vec3_t& vecMaxs, int iShadowType ) = 0;
	virtual bool					should_receive_projected_textures( int nFlags ) = 0;
	virtual bool					get_shadow_cast_distance( float* pDistance, int iShadowType ) const = 0;
	virtual bool					get_shadow_cast_direction( vec3_t* vecDirection, int iShadowType ) const = 0;
	virtual bool					is_shadow_dirty( ) = 0;
	virtual void					mark_shadow_dirty( bool bDirty ) = 0;
	virtual i_client_renderable* get_shadow_parent( ) = 0;
	virtual i_client_renderable* first_shadow_child( ) = 0;
	virtual i_client_renderable* next_shadow_peer( ) = 0;
	virtual int						shadow_cast_type( ) = 0;
	virtual void					unused2( )
	{ }
	virtual void					create_model_instance( ) = 0;
	virtual model_instance_handle_t	get_model_instance( ) = 0;
	virtual const matrix3x4_t& renderable_to_world_transform( ) = 0;
	virtual int						lookup_attachment( const char* pAttachmentName ) = 0;
	virtual bool					get_attachment( int index, vec3_t& vecOrigin, vec3_t& angView ) = 0;
	virtual bool					get_attachment( int index, matrix3x4_t& matAttachment ) = 0;
	virtual bool					compute_lighting_origin( int nAttachmentIndex, vec3_t vecModelLightingCenter, const matrix3x4_t& matrix, vec3_t& vecTransformedLightingCenter ) = 0;
	virtual float* get_render_clip_plane( ) = 0;
	virtual int						get_skin( ) = 0;
	virtual void					on_threaded_draw_setup( ) = 0;
	virtual bool					uses_flex_delayed_weights( ) = 0;
	virtual void					record_tool_message( ) = 0;
	virtual bool					should_draw_for_split_screen_user( int nSlot ) = 0;
	virtual std::uint8_t			override_alpha_modulation( std::uint8_t uAlpha ) = 0;
	virtual std::uint8_t			override_shadow_alpha_modulation( std::uint8_t uAlpha ) = 0;
	virtual void* get_client_model_renderable( ) = 0;
};

class c_base_client;
class i_client_networkable
{
public:
	virtual i_client_unknown* get_i_client_unknown( ) = 0;
	virtual void					release( ) = 0;
	virtual c_base_client* get_client_class( ) = 0;
	virtual void					notify_should_transmit( int iState ) = 0;
	virtual void					on_pre_data_changed( e_data_update_type updateType ) = 0;
	virtual void					on_data_changed( e_data_update_type updateType ) = 0;
	virtual void					pre_data_update( e_data_update_type updateType ) = 0;
	virtual void					post_data_update( e_data_update_type updateType ) = 0;
	virtual void					on_data_unchanged_in_pvs( ) = 0;
	virtual bool					is_dormant( ) const = 0;
	virtual int						get_index( ) const = 0;
	virtual void					receive_message( e_class_index classIndex, c_bf_read& msg ) = 0;
	virtual void* get_data_table_base_ptr( ) = 0;
	virtual void					set_destroyed_on_recreate_entities( ) = 0;
};

class c_client_think_handle;
using client_think_handle_t = c_client_think_handle*;
class i_client_thinkable
{
public:
	virtual i_client_unknown* get_i_client_unknown( ) = 0;
	virtual void					client_think( ) = 0;
	virtual client_think_handle_t		get_think_handle( ) = 0;
	virtual void					set_think_handle( client_think_handle_t hThink ) = 0;
	virtual void					release( ) = 0;
};

class i_client_entity : public i_client_unknown, public i_client_renderable, public i_client_networkable, public i_client_thinkable
{
public:
	virtual void					release( ) override = 0;
	virtual const vec3_t& get_abs_origin( ) const = 0;
	virtual const vec3_t& get_abs_angles( ) const = 0;
	virtual void* get_mouth( ) = 0;
	virtual bool					get_sound_spatialization( struct spatialization_info_t& info ) = 0;
	virtual bool					is_blurred( ) = 0;

	void set_abs_origin( const vec3_t& origin )
	{
		using set_abs_origin_fn = void( __thiscall* )( void*, const vec3_t& );
		static auto o_set_abs_origin = reinterpret_cast< set_abs_origin_fn >( g_game_modules->get( client_dll ).find_pattern( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8" ) );
		o_set_abs_origin( this, origin );
	}

	void set_abs_angles( const vec3_t& view )
	{
		using set_abs_angle_fn = void( __thiscall* )( void*, const vec3_t& );
		static auto o_set_abs_angles = reinterpret_cast< set_abs_angle_fn >( g_game_modules->get( client_dll ).find_pattern( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" ) );
		o_set_abs_angles( this, view );
	}

	data_map_t* get_data_desc_map( )
	{
		return c_memory::call_vfunc<data_map_t*>( this, 15 );
	}

	data_map_t* get_prediction_desc_map( )
	{
		return c_memory::call_vfunc<data_map_t*>( this, 17 );
	}
};

class c_weapon_cs_base;
class c_base_entity : public i_client_entity
{
public:
#pragma region DT_BasePlayer
	n_add_pvariable( float, get_fall_velocity, "CBasePlayer->m_flFallVelocity" );
	n_add_variable( vec3_t, get_view_punch, "CBasePlayer->m_viewPunchAngle" );
	n_add_variable( vec3_t, get_punch, "CBasePlayer->m_aimPunchAngle" );
	n_add_variable( vec3_t, get_view_offset, "CBasePlayer->m_vecViewOffset[0]" );
	n_add_variable( float, get_friction, "CBasePlayer->m_flFriction" );
	n_add_variable( int, get_tick_base, "CBasePlayer->m_nTickBase" );
	n_add_pvariable( int, get_next_think_tick, "CBasePlayer->m_nNextThinkTick" );
	n_add_variable( vec3_t, get_velocity, "CBasePlayer->m_vecVelocity[0]" );

	n_add_pvariable_offset( vec3_t, get_view_angles, "CBasePlayer->deadflag", 0x4 );
	n_add_variable( c_base_handle, get_ground_entity_handle, "CBasePlayer->m_hGroundEntity" );
	n_add_variable( int, get_health, "CBasePlayer->m_iHealth" );
	n_add_variable( int, get_life_state, "CBasePlayer->m_lifeState" );
	n_add_variable( float, get_max_speed, "CBasePlayer->m_flMaxspeed" );
	n_add_variable( int, get_flags, "CBasePlayer->m_fFlags" );
	n_add_pvariable( int, get_observer_mode, "CBasePlayer->m_iObserverMode" );
	n_add_variable( c_base_handle, get_observer_target_handle, "CBasePlayer->m_hObserverTarget" );
	n_add_variable( c_base_handle, get_view_model_handle, "CBasePlayer->m_hViewModel[0]" );
	n_add_pvariable( const char, get_last_place, "CBasePlayer->m_szLastPlaceName" );
	n_add_variable_offset( int, get_button_disabled, "CBasePlayer->m_hViewEntity", -0xC );
	n_add_variable_offset( int, get_button_forced, "CBasePlayer->m_hViewEntity", -0x8 );
	n_add_pvariable_offset( c_user_cmd*, get_current_command, "CBasePlayer->m_hViewEntity", -0x4 ); // @ida: client.dll @ [89 BE ? ? ? ? E8 ? ? ? ? 85 FF + 0x2]

	n_add_datafield( int, get_eflags, this->get_prediction_desc_map( ), "m_iEFlags" );
	n_add_pdatafield( int, get_buttons, this->get_prediction_desc_map( ), "m_nButtons" );
	n_add_datafield( int, get_button_last, this->get_prediction_desc_map( ), "m_afButtonLast" );
	n_add_datafield( int, get_button_pressed, this->get_prediction_desc_map( ), "m_afButtonPressed" );
	n_add_datafield( int, get_button_released, this->get_prediction_desc_map( ), "m_afButtonReleased" );
	n_add_pdatafield( int, get_impulse, this->get_prediction_desc_map( ), "m_nImpulse" );
	n_add_datafield( float, get_surface_friction, this->get_prediction_desc_map( ), "m_surfaceFriction" );
	n_add_datafield( vec3_t, get_abs_velocity, this->get_prediction_desc_map( ), "m_vecAbsVelocity" );
	n_add_variable( int, get_hide_hud, "CBasePlayer->m_iHideHUD" );
	n_add_variable( float, get_duck_speed, "CBasePlayer->m_flDuckSpeed" );

	inline bool is_alive( )
	{
		// @note: https://github.com/rollraw/qo0-base/issues/135
		return ( this && this->get_health( ) > 0 );
	}

	int& get_take_damage( )
	{
		static const std::uintptr_t take_damage_offset = *reinterpret_cast< std::uintptr_t* >( g_game_modules->get( client_dll ).find_pattern( "80 BE ? ? ? ? ? 75 46 8B 86" ) + 0x2 );
		return *reinterpret_cast< int* >( reinterpret_cast< std::uintptr_t >( this ) + take_damage_offset );
	}

	c_user_cmd& get_last_command( )
	{
		static const std::uintptr_t last_command_offset = *reinterpret_cast< std::uintptr_t* >( g_game_modules->get( client_dll ).find_pattern( "8D 8E ? ? ? ? 89 5C 24 3C" ) + 0x2 );
		return *reinterpret_cast< c_user_cmd* >( reinterpret_cast< std::uintptr_t >( this ) + last_command_offset );
	}
#pragma endregion

#pragma region DT_CSPlayer
	n_add_variable( int, get_shots_fired, "CCSPlayer->m_iShotsFired" );
	n_add_variable_offset( float, get_spawn_time, "CCSPlayer->m_iAddonBits", -0x4 ); // @ida: client.dll @ [89 86 ? ? ? ? E8 ? ? ? ? 80 BE ? ? ? ? ? + 0x2]
	n_add_variable( int, get_money, "CCSPlayer->m_iAccount" );
	n_add_variable( int, get_total_hits, "CCSPlayer->m_totalHitsOnServer" );
	n_add_variable( int, get_armor, "CCSPlayer->m_ArmorValue" );
	n_add_variable( vec3_t, get_eye_angles, "CCSPlayer->m_angEyeAngles" );
	n_add_variable( bool, is_defusing, "CCSPlayer->m_bIsDefusing" );
	n_add_variable( bool, is_scoped, "CCSPlayer->m_bIsScoped" );
	n_add_variable_offset( float, collision_change_time, "CCSPlayer->m_bIsScoped", -0x50 ); // @ida: client.dll @ [8B 8E ? ? ? ? F3 0F 10 48 ? E8 ? ? ? ? C7 + 0x2]
	n_add_variable_offset( c_csgo_player_anim_state*, get_animation_state, "CCSPlayer->m_bIsScoped", -0x14 ); // @ida: client.dll @ [8B 8E ? ? ? ? F3 0F 10 48 ? E8 ? ? ? ? C7 + 0x2]
	n_add_variable( bool, is_grabbing_hostage, "CCSPlayer->m_bIsGrabbingHostage" );
	n_add_variable( bool, is_rescuing, "CCSPlayer->m_bIsRescuing" );
	n_add_variable( bool, has_helmet, "CCSPlayer->m_bHasHelmet" );
	n_add_variable( bool, has_heavy_armor, "CCSPlayer->m_bHasHeavyArmor" );
	n_add_variable( bool, has_defuser, "CCSPlayer->m_bHasDefuser" );
	n_add_variable( bool, has_immunity, "CCSPlayer->m_bGunGameImmunity" );
	n_add_variable( bool, is_in_buy_zone, "CCSPlayer->m_bInBuyZone" );
	n_add_pvariable( float, get_flash_max_alpha, "CCSPlayer->m_flFlashMaxAlpha" );
	n_add_variable_offset( float, get_flash_alpha, "CCSPlayer->m_flFlashMaxAlpha", -0x8 );
	n_add_variable( float, get_flash_duration, "CCSPlayer->m_flFlashDuration" );
	n_add_variable_offset( int, get_glow_index, "CCSPlayer->m_flFlashDuration", 0x18 );
	n_add_variable( float, get_lower_body_yaw, "CCSPlayer->m_flLowerBodyYawTarget" );
	n_add_variable( int, get_survival_team, "CCSPlayer->m_nSurvivalTeam" );
	n_add_variable_offset( int, is_used_new_anim_state, "CCSPlayer->m_flLastExoJumpTime", 0x8 );
	n_add_variable( float, get_velocity_modifier, "CCSPlayer->m_flVelocityModifier" );
	n_add_variable( float, get_thirdperson_recoil, "CCSPlayer->m_flThirdpersonRecoil" );
	n_add_pvariable( void*, get_patch_econ_indices, "CCSPlayer->m_vecPlayerPatchEconIndices" );
	n_add_variable( float, get_duck_amount, "CCSPlayer->m_flDuckAmount" );
	n_add_variable( bool, get_wait_for_no_attack, "CCSPlayer->m_bWaitForNoAttack" );

	inline bool is_armored( const int hit_group )
	{
		// @ida isarmored: server.dll @ 55 8B EC 32 D2

		bool bIsArmored = false;

		if ( this->get_armor( ) > 0 )
		{
			switch ( hit_group )
			{
				case hitgroup_generic:
				case hitgroup_chest:
				case hitgroup_stomach:
				case hitgroup_leftarm:
				case hitgroup_rightarm:
				case hitgroup_neck:
					bIsArmored = true;
					break;
				case hitgroup_head:
					if ( this->has_helmet( ) )
						bIsArmored = true;
					[[fallthrough]];
				case hitgroup_leftleg:
				case hitgroup_rightleg:
					if ( this->has_heavy_armor( ) )
						bIsArmored = true;
					break;
				default:
					break;
			}
		}

		return bIsArmored;
	}
#pragma endregion

#pragma region DT_BaseEntity
	n_add_variable( float, get_animation_time, "CBaseEntity->m_flAnimTime" );
	n_add_variable( float, get_simulation_time, "CBaseEntity->m_flSimulationTime" );
	n_add_variable_offset( float, get_old_simulation_time, "CBaseEntity->m_flSimulationTime", 0x4 );
	n_add_variable( vec3_t, get_origin, "CBaseEntity->m_vecOrigin" );
	n_add_variable( vec3_t, get_rotation, "CBaseEntity->m_angRotation" );
	n_add_variable( int, get_effects, "CBaseEntity->m_fEffects" );
	n_add_variable( int, get_team, "CBaseEntity->m_iTeamNum" );
	n_add_variable( c_base_handle, get_owner_entity_handle, "CBaseEntity->m_hOwnerEntity" );
	n_add_pvariable( i_collideable, get_collision, "CBaseEntity->m_Collision" );
	n_add_variable( int, get_collision_group, "CBaseEntity->m_CollisionGroup" );
	n_add_pvariable( bool, is_spotted, "CBaseEntity->m_bSpotted" );

	n_add_variable( vec3_t, get_vec_mins, "CBaseEntity->m_vecMins" );
	n_add_variable( vec3_t, get_vec_maxs, "CBaseEntity->m_vecMaxs" );

	n_add_datafield( vec3_t, get_abs_rotation, this->get_data_desc_map( ), "m_angAbsRotation" );
	n_add_datafield( const matrix3x4_t, get_coordinate_frame, this->get_data_desc_map( ), "m_rgflCoordinateFrame" );
	n_add_datafield( int, get_move_type, this->get_prediction_desc_map( ), "m_MoveType" );
	n_add_variable( c_base_handle, get_move_parent, "CBaseEntity->moveparent" );
#pragma endregion

#pragma region DT_BaseCombatCharacter
	n_add_variable( float, get_next_attack, "CBaseCombatCharacter->m_flNextAttack" );
	n_add_variable( c_base_handle, get_active_weapon_handle, "CBaseCombatCharacter->m_hActiveWeapon" );
	n_add_pvariable( c_base_handle, get_weapons_handle, "CBaseCombatCharacter->m_hMyWeapons" );
	n_add_pvariable( c_base_handle, get_wearables_handle, "CBaseCombatCharacter->m_hMyWearables" );
#pragma endregion

#pragma region DT_BaseAnimating
	n_add_variable( int, get_sequence, "CBaseAnimating->m_nSequence" );
	n_add_pvariable_offset( c_bone_accessor, get_bone_accessor, "CBaseAnimating->m_nForceBone", 0x1C );
	n_add_variable( int, get_hitbox_set, "CBaseAnimating->m_nHitboxSet" );
	n_add_variable( bool, is_client_side_animation, "CBaseAnimating->m_bClientSideAnimation" );
	n_add_variable( float, get_cycle, "CBaseAnimating->m_flCycle" );
	n_add_variable( int, get_animating_body, "CBaseAnimating->m_nBody" );

	[[nodiscard]] std::array<float, MAXSTUDIOPOSEPARAM>& get_pose_parameter( )
	{
		static std::uintptr_t pose_parameter_offset = g_netvar_manager->map_props[ fnv1a::hash_const( "CBaseAnimating->m_flPoseParameter" ) ].offset;
		return *reinterpret_cast< std::array<float, MAXSTUDIOPOSEPARAM>* >( reinterpret_cast< std::uintptr_t >( this ) + pose_parameter_offset );
	}

	[[nodiscard]] c_utl_vector<c_animation_layer>& get_animation_overlays( )
	{
		static const std::uintptr_t animation_overlays_offset = *reinterpret_cast< std::uintptr_t* >( g_game_modules->get( client_dll ).find_pattern( "8B 89 ? ? ? ? 8D 0C D1" ) + 0x2 );
		return *reinterpret_cast< c_utl_vector<c_animation_layer>* >( reinterpret_cast< std::uintptr_t >( this ) + animation_overlays_offset );
	}

	[[nodiscard]] inline c_animation_layer* get_animation_layer( const int layer )
	{
		assert( layer >= 0 && layer < MAXOVERLAYS && "layer out of bound (layer >= 0 && layer < 15)" );

		if ( layer >= 0 && layer < MAXOVERLAYS )
			return &get_animation_overlays( )[ layer ];

		return nullptr;
	}
#pragma endregion

#pragma region custom_function
	int is_max_health( )
	{
		// @ida: client.dll @ [FF 90 ? ? ? ? 85 C0 0F 8F ? ? ? ? 80 + 0x2] / sizeof(std::uintptr_t)
		return c_memory::call_vfunc<int>( this, 123 );
	}

	void think( )
	{
		// @ida: client.dll @ [FF 90 ? ? ? ? FF 35 ? ? ? ? 8B 4C + 0x2] / sizeof(std::uintptr_t)
		c_memory::call_vfunc<void>( this, 139 );
	}

	const char* get_classname( )
	{
		// @ida: client.dll @ [8B 01 FF 90 ? ? ? ? 90 + 0x4] / sizeof(std::uintptr_t)
		return c_memory::call_vfunc<const char*>( this, 143 );
	}

	unsigned int physics_solid_mask_for_entity( )
	{
		// @xref: "func_breakable", "func_breakable_surf"
		return c_memory::call_vfunc<unsigned int>( this, 152 );
	}

	bool is_player( )
	{
		// @xref: "effects/nightvision"
		return c_memory::call_vfunc<bool>( this, 158 );
	}

	bool is_weapon( )
	{
		// @xref: "effects/nightvision"
		return c_memory::call_vfunc<bool>( this, 166 );
	}

	[[nodiscard]] vec3_t get_eye_position( bool should_correct = true )
	{
		vec3_t position = { };

		// get eye position
		c_memory::call_vfunc<void>( this, 169, std::ref( position ) );

		// correct this like it do weapon_shootpos
		// @ida weapon_shootpos: 55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90
		if ( is_used_new_anim_state( ) && should_correct )
		{
			if ( c_csgo_player_anim_state* anim_state = this->get_animation_state( ); anim_state != nullptr )
				modify_eye_position( anim_state, &position );
		}

		// return corrected position
		return position;
	}

	void set_sequence( int sequence )
	{
		// @ida: FF 90 ? ? ? ? 8B 07 8B CF FF 90 ? ? ? ? 8B CF + 0x2
		c_memory::call_vfunc<void>( this, 219, sequence );
	}

	void studio_frame_advance( )
	{
		// @ida: FF 90 ? ? ? ? 8B 07 8B CF FF 90 ? ? ? ? 8B CF + 0xC
		c_memory::call_vfunc<void>( this, 220 );
	}

	float get_layer_sequence_cycle_rate( c_animation_layer* anim_layer, int layer_sequence )
	{
		return c_memory::call_vfunc<float>( this, 223, anim_layer, layer_sequence );
	}

	void update_client_side_animations( )
	{
		// @xref: "UpdateClientSideAnimations"
		// @ida updateclientsideanimations: 55 8B EC 51 56 8B F1 80 BE ? ? ? ? 00 74 ? 8B 06 FF
		c_memory::call_vfunc<void>( this, 224 );
	}

	void pre_think( )
	{
		c_memory::call_vfunc<void>( this, 318 );
	}

	void update_collision_bounds( )
	{
		c_memory::call_vfunc<void>( this, 340 );
	}

	bool physics_run_think( int nThinkMethod )
	{
		// @xref: from sub with "CLIENT:  %s(%s) thinking for %.02f ms!!!\n"
		using physics_run_think_fn = bool( __thiscall* )( void*, int );
		static auto o_physics_run_think = reinterpret_cast< physics_run_think_fn >( g_game_modules->get( client_dll ).find_pattern( "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87" ) );
		return o_physics_run_think( this, nThinkMethod );
	}

	int equip_wearable( c_base_combat_weapon* wearable )
	{
		using equip_wearable_fn = int( __thiscall* )( c_base_combat_weapon* wearable, c_base_entity* player );
		static auto o_equip_wearable = reinterpret_cast< equip_wearable_fn >( g_game_modules->get( client_dll ).find_pattern( "55 8B EC 83 EC 10 53 8B 5D 08 57 8B F9" ) );
		return o_equip_wearable( wearable, this );
	}

	void invalidate_bone_cache( )
	{
		typedef void( __thiscall* invalidate_bone_cache_fn )( void* );
		static auto o_invalidate_bone_cache = reinterpret_cast< invalidate_bone_cache_fn >( g_game_modules->get( client_dll ).find_pattern( "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81" ) );

		if ( this && o_invalidate_bone_cache )
			o_invalidate_bone_cache( this );
	}

	void modify_eye_position( const c_csgo_player_anim_state* anim_state, vec3_t* position ) const;
	static c_base_entity* get_local_player( );
	int						get_sequence_activity( int sequence );
	int						get_max_health( );
	mstudiobbox_t* get_hitbox( int hb_id );
	std::optional<vec3_t>	get_bone_position( int bone_index );
	int						get_bone_by_hash( const fnv1a_t bone_hash ) const;
	std::optional<vec3_t>	get_hitbox_position( const int hitbox );
	std::optional<vec3_t>	get_hit_group_position( const int hit_group );
	void					post_think( );
	bool					is_enemy( c_base_entity* entity );
	bool					is_targeting_local( c_base_entity* local );
	bool					is_visible( c_base_entity* entity, const vec3_t& end_pos, bool smoke_check = false );
	bool					is_breakable( );
	bool					is_reloading( );
	c_weapon_cs_base* get_active_weapon( );
	vec3_t					get_shoot_pos( bool interpolated = false );
	bool					has_c4( );

#pragma endregion
};

class c_smoke_grenade
{
public:
	n_add_variable( int, get_effect_tick_begin, "CSmokeGrenadeProjectile->m_nSmokeEffectTickBegin" );

	float get_max_time( )
	{
		return 19.f;
	}
};

class c_inferno
{
public:
	n_add_variable( int, get_effect_tick_begin, "CInferno->m_nFireEffectTickBegin" );
	n_add_pvariable( bool, get_fire_is_burning, "CInferno->m_bFireIsBurning" );

	n_add_variable( int, get_fire_count, "CInferno->m_fireCount" );

	n_add_pvariable( int, get_fire_x_delta, "CInferno->m_fireXDelta" );
	n_add_pvariable( int, get_fire_y_delta, "CInferno->m_fireYDelta" );
	n_add_pvariable( int, get_fire_z_delta, "CInferno->m_fireZDelta" );

	float get_max_time( );
};

class c_planted_c4
{
public:
#pragma region DT_PlantedC4
	n_add_variable( float, get_blow_time, "CPlantedC4->m_flC4Blow" );
	n_add_variable( float, get_timer_length, "CPlantedC4->m_flTimerLength" );
	n_add_variable( float, get_defuse_length, "CPlantedC4->m_flDefuseLength" );
	n_add_variable( float, get_defuse_count_down, "CPlantedC4->m_flDefuseCountDown" );
	n_add_variable( bool, is_planted, "CPlantedC4->m_bBombTicking" );
	n_add_variable( c_base_handle, get_defuser_handle, "CPlantedC4->m_hBombDefuser" );
	n_add_variable( bool, is_defused, "CPlantedC4->m_bBombDefused" );
	n_add_variable( int, get_bomb_site, "CPlantedC4->m_nBombSite" );

	inline float get_timer( const float server_time )
	{
		return std::clamp( this->get_blow_time( ) - server_time, 0.0f, this->get_timer_length( ) );
	}

	inline float GetDefuseTimer( const float server_time )
	{
		return std::clamp( this->get_defuse_count_down( ) - server_time, 0.0f, this->get_defuse_length( ) );
	}
#pragma endregion
};

class c_base_view_model
{
public:
#pragma region DT_BaseViewModel
	n_add_variable( int, get_model_index, "CBaseViewModel->m_nModelIndex" );
	n_add_variable( c_base_handle, get_owner_handle, "CBaseViewModel->m_hOwner" );
	n_add_variable( c_base_handle, get_weapon_handle, "CBaseViewModel->m_hWeapon" );
#pragma endregion

	void send_view_model_matching_sequence( int sequence )
	{
		c_memory::call_vfunc<void>( this, 247, sequence );
	}

	void set_weapon_model( const char* file_name, c_base_combat_weapon* weapon )
	{
		// @ida setweaponmodel: 57 8B F9 8B 97 ? ? ? ? 83 FA FF 74 6A
		c_memory::call_vfunc<void>( this, 248, file_name, weapon );
	}
};

class c_env_tonemap_controller
{
public:
#pragma region DT_EnvTonemapController
	n_add_variable( bool, is_use_custom_auto_exposure_min, "CEnvTonemapController->m_bUseCustomAutoExposureMin" );
	n_add_variable( bool, is_use_custom_auto_exposure_max, "CEnvTonemapController->m_bUseCustomAutoExposureMax" );
	n_add_variable( bool, is_use_custom_bloom_scale, "CEnvTonemapController->m_bUseCustomBloomScale" );
	n_add_variable( float, get_custom_auto_exposure_min, "CEnvTonemapController->m_flCustomAutoExposureMin" );
	n_add_variable( float, get_custom_auto_exposure_max, "CEnvTonemapController->m_flCustomAutoExposureMax" );
	n_add_variable( float, get_custom_bloom_scale, "CEnvTonemapController->m_flCustomBloomScale" );
	n_add_variable( float, get_custom_bloom_scale_min, "CEnvTonemapController->m_flCustomBloomScaleMinimum" );
	n_add_variable( float, get_bloom_exponent, "CEnvTonemapController->m_flBloomExponent" );
	n_add_variable( float, get_bloom_saturation, "CEnvTonemapController->m_flBloomSaturation" );
#pragma endregion
};

class c_breakable_surface : public c_base_entity, public i_breakable_with_prop_data
{
public:
#pragma region DT_BreakableSurface
	n_add_variable( bool, is_broken, "CBreakableSurface->m_bIsBroken" );
#pragma endregion
};

class c_precipitation : public c_base_entity
{
public:
	n_add_variable( int, get_precipitation_type, "CPrecipitation->m_nPrecipType" );
};

class c_cs_game_rules_proxy
{
public:
	n_add_variable( bool, is_valve_secure, "CCSGameRulesProxy->m_bIsValveDS" );
};

class c_weapon_data
{
public:
	std::byte pad0[ 0x14 ];			// 0x0000
	int max_clip1;					// 0x0014
	int max_clip2;					// 0x0018
	int default_clip1;				// 0x001C
	int default_clip2;				// 0x0020
	int primary_max_reserve_ammo;		// 0x0024
	int secondary_max_reserve_ammo;	// 0x0028
	const char* world_model;		// 0x002C
	const char* view_model;		// 0x0030
	const char* dropped_model;		// 0x0034
	std::byte pad1[ 0x50 ];			// 0x0038
	const char* hud_name;			// 0x0088
	const char* weapon_name;		// 0x008C
	std::byte pad2[ 0x2 ];			// 0x0090
	bool is_melee_weapon;			// 0x0092
	std::byte pad3[ 0x9 ];			// 0x0093
	float weapon_weight;			// 0x009C
	std::byte pad4[ 0x4 ];			// 0x00A0
	int slot;						// 0x00A4
	int position;					// 0x00A8
	std::byte pad5[ 0x1C ];			// 0x00AC
	int weapon_type;				// 0x00C8
	std::byte pad6[ 0x4 ];			// 0x00CC
	int weapon_price;				// 0x00D0
	int kill_award;					// 0x00D4
	const char* animation_prefix;	// 0x00D8
	float cycle_time;				// 0x00DC
	float cycle_time_alt;			// 0x00E0
	std::byte pad8[ 0x8 ];			// 0x00E4
	bool full_auto;					// 0x00EC
	std::byte pad9[ 0x3 ];			// 0x00ED
	int damage;					// 0x00F0
	float head_shot_multiplier;		// 0x00F4
	float armor_ratio;				// 0x00F8
	int iBullets;					// 0x00FC
	float penetration;			// 0x0100
	std::byte pad10[ 0x8 ];			// 0x0104
	float range;					// 0x010C
	float range_modifier;			// 0x0110
	float throw_velocity;			// 0x0114
	std::byte pad11[ 0xC ];			// 0x0118
	bool has_silencer;				// 0x0124
	std::byte pad12[ 0xF ];			// 0x0125
	float max_speed[ 2 ];			// 0x0134
	std::byte pad13[ 0x4 ];			// 0x013C
	float spread[ 2 ];				// 0x0140
	float inaccuracy_crouch[ 2 ];	// 0x0148
	float inaccuracy_stand[ 2 ];		// 0x0150
	std::byte pad14[ 0x8 ];			// 0x0158
	float inaccuracy_jump[ 2 ];		// 0x0160
	float inaccuracy_land[ 2 ];		// 0x0168
	float inaccuracy_ladder[ 2 ];	// 0x0170
	float inaccuracy_fire[ 2 ];		// 0x0178
	float inaccuracy_move[ 2 ];		// 0x0180
	float inaccuracy_reload;		// 0x0188
	int recoil_seed;				// 0x018C
	float recoil_angle[ 2 ];			// 0x0190
	float recoil_angle_variance[ 2 ];	// 0x0198
	float recoil_magnitude[ 2 ];		// 0x01A0
	float recoil_magnitude_variance[ 2 ]; // 0x01A8
	int spread_seed;				// 0x01B0

	bool is_gun( ) const
	{
		switch ( this->weapon_type )
		{
			case weapontype_pistol:
			case weapontype_submachinegun:
			case weapontype_rifle:
			case weapontype_shotgun:
			case weapontype_sniper:
			case weapontype_machinegun:
				return true;
		}

		return false;
	}

	bool is_grenade( ) const
	{
		return this->weapon_type == weapontype_grenade;
	}

	bool is_knife( )
	{
		return this->weapon_type == weapontype_knife;
	}
};

class i_weapon_system
{
public:
	c_weapon_data* get_weapon_data( short item_definition_index )
	{
		return c_memory::call_vfunc<c_weapon_data*>( this, 2, item_definition_index );
	}
};

class i_ref_counted;
class c_econ_item_view
{
public:
	n_add_offset( c_utl_vector<i_ref_counted*>, get_custom_materials, 0x14 );

	c_utl_vector<c_ref_counted*>& get_visuals_data_processors( )
	{
		// @xref: "Original material not found! Name: %s"
		static const std::uintptr_t visuals_data_processors_offset = *reinterpret_cast< std::uintptr_t* >( g_game_modules->get( client_dll ).find_pattern( "81 C7 ? ? ? ? 8B 4F 0C 8B 57 04 89 4C" ) + 0x2 );
		return *reinterpret_cast< c_utl_vector<c_ref_counted*>* >( reinterpret_cast< std::uintptr_t >( this ) + visuals_data_processors_offset );
	}
};

class c_base_combat_weapon : public c_base_entity
{
public:
#pragma region DT_BaseCombatWeapon
	n_add_variable( float, get_next_primary_attack, "CBaseCombatWeapon->m_flNextPrimaryAttack" );
	n_add_variable( float, get_next_secondary_attack, "CBaseCombatWeapon->m_flNextSecondaryAttack" );
	n_add_variable( int, get_ammo, "CBaseCombatWeapon->m_iClip1" );
	n_add_variable( int, get_ammo_reserve, "CBaseCombatWeapon->m_iPrimaryReserveAmmoCount" );
	n_add_variable( int, get_view_model_index, "CBaseCombatWeapon->m_iViewModelIndex" );
	n_add_variable( int, get_world_model_index, "CBaseCombatWeapon->m_iWorldModelIndex" );
	n_add_variable( c_base_handle, get_world_model_handle, "CBaseCombatWeapon->m_hWeaponWorldModel" );
	n_add_variable( int, get_activity, "CBaseCombatWeapon->m_Activity" );

	n_add_datafield( bool, is_reloading, this->get_prediction_desc_map( ), "m_bInReload" );
	n_add_variable( int, get_model_index, "CBaseViewModel->m_nModelIndex" );

#pragma endregion

#pragma region DT_BaseAttributableItem
	n_add_variable( short, get_item_definition_index, "CBaseAttributableItem->m_iItemDefinitionIndex" );
	n_add_variable( int, get_item_id_high, "CBaseAttributableItem->m_iItemIDHigh" );
	n_add_variable( int, get_item_id_low, "CBaseAttributableItem->m_iItemIDLow" );
	n_add_variable( int, get_account_id, "CBaseAttributableItem->m_iAccountID" );
	n_add_variable( int, get_entity_quality, "CBaseAttributableItem->m_iEntityQuality" );
	n_add_pvariable( char, get_custom_name, "CBaseAttributableItem->m_szCustomName" );
	n_add_variable( int, get_owner_xuid_low, "CBaseAttributableItem->m_OriginalOwnerXuidLow" );
	n_add_variable( int, get_owner_xuid_high, "CBaseAttributableItem->m_OriginalOwnerXuidHigh" );
	n_add_variable( int, get_fallback_paint_kit, "CBaseAttributableItem->m_nFallbackPaintKit" );
	n_add_variable( int, get_fallback_seed, "CBaseAttributableItem->m_nFallbackSeed" );
	n_add_variable( float, get_fallback_wear, "CBaseAttributableItem->m_flFallbackWear" );
	n_add_variable( int, get_fallback_stat_trak, "CBaseAttributableItem->m_nFallbackStatTrak" );
	n_add_pvariable( c_econ_item_view, get_econ_item_view, "CBaseAttributableItem->m_Item" );
	n_add_variable( bool, is_initialized, "CBaseAttributableItem->m_bInitialized" );
#pragma endregion

	void set_model_index( int nModelIndex )
	{
		c_memory::call_vfunc<void>( this, 75, nModelIndex );
	}

	[[nodiscard]] bool is_weapon( )
	{
		return c_memory::call_vfunc<bool>( this, 166 );
	}

	[[nodiscard]] float get_spread( )
	{
		return c_memory::call_vfunc<float>( this, 453 );
	}

	[[nodiscard]] float get_inaccuracy( )
	{
		return c_memory::call_vfunc<float>( this, 483 );
	}

	[[nodiscard]] void update_accuracy_penalty( )
	{
		c_memory::call_vfunc<void>( this, 484 );
	}

	c_weapon_data* get_weapon_data( );
};

class cte_fire_bullets
{
public:
	std::byte	pad0[ 0x10 ];				//0x0000
	int			nPlayer;				//0x0010
	int			nItemDefinitionIndex;	//0x0014
	vec3_t		vecOrigin;				//0x0018
	vec3_t		vecAngles;				//0x0024
	int			iWeapon;				//0x0030
	int			nWeaponID;				//0x0034
	int			iMode;					//0x0038
	int			nSeed;					//0x003C
	float		flInaccuracy;			//0x0040
	float		flRecoilIndex;			//0x0044
	float		flSpread;				//0x0048
	int			nSoundType;				//0x004C
}; // Size: 0x0050

class c_weapon_cs_base : public c_base_combat_weapon
{
public:
#pragma region DT_WeaponCSBaseGun
	n_add_variable( int, get_zoom_level, "CWeaponCSBaseGun->m_zoomLevel" );
	n_add_variable( int, get_burst_shots_remaining, "CWeaponCSBaseGun->m_iBurstShotsRemaining" );
	n_add_variable( int, weapon_mode, "CWeaponCSBaseGun->m_weaponMode" );
	n_add_variable( float, get_next_burst_shot, "CWeaponCSBaseGun->m_fNextBurstShot" );
#pragma endregion

#pragma region DT_WeaponCSBase
	n_add_variable( bool, is_burst_mode, "CWeaponCSBase->m_bBurstMode" );
	n_add_variable( float, get_accuracy_penalty, "CWeaponCSBase->m_fAccuracyPenalty" );
	n_add_variable( float, get_fire_ready_time, "CWeaponCSBase->m_flPostponeFireReadyTime" );
	n_add_variable( float, get_last_shot_time, "CWeaponCSBase->m_fLastShotTime" );
#pragma endregion

	c_utl_vector<i_ref_counted*>& get_custom_materials( )
	{
		static auto address = *reinterpret_cast< std::uintptr_t* >( g_game_modules->get( client_dll ).find_pattern( "83 BE ? ? ? ? ? 7F 67" ) + 0x2 ) - 0xC;
		return *reinterpret_cast< c_utl_vector<i_ref_counted*>* >( reinterpret_cast< std::uintptr_t >( this ) + address );
	}

	bool& is_custom_material_initialized( )
	{
		static auto address = *reinterpret_cast< std::uintptr_t* >( g_game_modules->get( client_dll ).find_pattern( "C6 86 ? ? ? ? ? FF 50 04" ) + 0x2 );
		return *reinterpret_cast< bool* >( reinterpret_cast< std::uintptr_t >( this ) + address );
	}

	bool can_shoot( );
};

class c_base_cs_grenade : public c_weapon_cs_base
{
public:
#pragma region DT_BaseCSGrenade
	n_add_variable( bool, is_pin_pulled, "CBaseCSGrenade->m_bPinPulled" );
	n_add_variable( float, get_throw_time, "CBaseCSGrenade->m_fThrowTime" );
	n_add_variable( float, get_throw_strength, "CBaseCSGrenade->m_flThrowStrength" );
	n_add_variable( c_base_handle, get_thrower_handle, "CBaseGrenade->m_hThrower" );
#pragma endregion

#pragma region DT_BaseCSGrenadeProjectile
	n_add_variable( int, get_effect_tick_begin, "CBaseCSGrenadeProjectile->m_nExplodeEffectTickBegin" );
	n_add_variable( vec3_t, get_initial_velocity, "CBaseCSGrenadeProjectile->m_vInitialVelocity" );
	n_add_variable( int, get_animation_time, "CBaseCSGrenadeProjectile->m_flAnimTime" );
	n_add_variable( int, get_nade_body, "CBaseCSGrenadeProjectile->m_nBody" );
	n_add_variable( int, get_force_bone, "CBaseCSGrenadeProjectile->m_nForceBone" );
	n_add_variable_offset( float, get_spawn_time, "CBaseCSGrenadeProjectile->m_vecExplodeEffectOrigin", 0xC );

	inline bool is_throwing( )
	{
		if ( !this->is_pin_pulled( ) )
		{
			float throw_time = this->get_throw_time( );
			if ( throw_time > 0.0f )
				return true;
		}
		return false;
	}
#pragma endregion
};