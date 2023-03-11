#pragma once
#include "../data_types/string_struct.h"
#include "../definitions/definitions.h"

class i_breakable_with_prop_data
{
public:
	// Damage modifiers
	virtual void		set_dmg_mod_bullet( float dmg_mod ) = 0;
	virtual void		set_dmg_mod_club( float dmg_mod ) = 0;
	virtual void		set_dmg_mod_explosive( float dmg_mod ) = 0;
	virtual float		get_dmg_mod_bullet( ) = 0;
	virtual float		get_dmg_mod_club( ) = 0;
	virtual float		get_dmg_mod_explosive( ) = 0;
	virtual float		get_dmg_mod_fire( ) = 0;

	// Explosive
	virtual void		set_explosive_radius( float radius ) = 0;
	virtual void		set_explosive_damage( float damage ) = 0;
	virtual float		get_explosive_radius( ) = 0;
	virtual float		get_explosive_damage( ) = 0;

	// Physics damage tables
	virtual void		set_physics_damage_table( string_t table_name ) = 0;
	virtual string_t	get_physics_damage_table( ) = 0;

	// Breakable chunks
	virtual void		set_breakable_model( string_t model ) = 0;
	virtual string_t 	get_breakable_model( ) = 0;
	virtual void		set_breakable_skin( int skin ) = 0;
	virtual int			get_breakable_skin( ) = 0;
	virtual void		set_breakable_count( int count ) = 0;
	virtual int			get_breakable_count( ) = 0;
	virtual void		set_max_breakable_size( int size ) = 0;
	virtual int			get_max_breakable_size( ) = 0;

	// LOS blocking
	virtual void		set_prop_data_blocks_los( bool blocks_los ) = 0;
	virtual void		set_prop_data_is_ai_walkable( bool blocks_los ) = 0;

	// Interactions
	virtual void		set_interaction( e_property_data_interactions interaction ) = 0;
	virtual bool		has_interaction( e_property_data_interactions interaction ) = 0;

	// Multiplayer physics mode
	virtual void		set_physics_mode( int iMode ) = 0;
	virtual int			get_physics_mode( ) = 0;

	// Multiplayer breakable spawn behavior
	virtual void		set_multiplayer_break_mode( e_multiplayer_break mode ) = 0;
	virtual e_multiplayer_break get_multiplayer_break_mode( ) const = 0;

	// Used for debugging
	virtual void		set_base_prop_data( string_t base ) = 0;
	virtual string_t	get_base_prop_data( ) = 0;
};

class i_multiplayer_physics
{
public:
	virtual int		get_multiplayer_physics_mode( ) = 0;
	virtual float	get_mass( ) = 0;
	virtual bool	is_asleep( ) = 0;
};
