#pragma once

#include "istudiorender.h"

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/engine/ivmodelrender.h
using model_instance_handle_t = std::uint16_t;

struct model_render_info_t
{
	vec3_t					origin;
	vec3_t					angles;
	std::byte				pad0[ 0x4 ];
	i_client_renderable* renderable;
	const model_t* model;
	const matrix3x4_t* model_to_world;
	const matrix3x4_t* lighting_offset;
	const vec3_t* lighting_origin;
	int						flags;
	int						entity_index;
	int						skin;
	int						body;
	int						hitbox_set;
	model_instance_handle_t instance;
};

struct renderable_info_t
{
	i_client_renderable* renderable;		// 0x00
	i_client_alpha_property* alpha_property;		// 0x04
	int						enum_count;			// 0x08
	int						render_frame;		// 0x0C
	unsigned short			first_shadow;		// 0x10
	unsigned short			leaf_list;			// 0x12
	short					area;				// 0x14
	std::uint16_t			flags;				// 0x16
	std::uint16_t			flags2;			// 0x18
	vec3_t					bloated_abs_mins;	// 0x1A
	vec3_t					bloated_abs_maxs;	// 0x26
	vec3_t					abs_mins;			// 0x32
	vec3_t					abs_maxs;			// 0x3E
	std::byte				pad0[ 0x4 ];			// 0x4A
}; // Size: 0x4E

struct studiohwdata_t;
struct studio_decal_handle_t;
struct draw_model_state_t
{
	studiohdr_t* studio_hdr;
	studiohwdata_t* studio_hw_data;
	i_client_renderable* renderable;
	const matrix3x4_t* model_to_world;
	studio_decal_handle_t* decals;
	int						draw_flags;
	int						lod;
};

class i_model_render
{
public:
	void forced_material_override( i_material* material, e_override_type override_type = override_normal, int overrides = 0 )
	{
		return c_memory::call_vfunc<void>( this, 1, material, override_type, overrides );
	}

	bool is_forced_material_override( )
	{
		return c_memory::call_vfunc<bool>( this, 2 );
	}

	void suppress_engine_lighting( bool suppress )
	{
		return c_memory::call_vfunc<void>( this, 24, suppress );
	}
};
