#pragma once
// used: vector
#include "../data_types/vector.h"
// used: matrix
#include "../data_types/matrix.h"
// used: bone masks, studio classes
#include "../data_types/studio.h"
// used: trace, cplane
#include "ienginetrace.h"

#pragma region modelinfo_enumerations
enum e_bone_index : int
{
	bone_invalid = -1,
	bone_pelvis,
	bone_lean_root,
	bone_cam_driver,
	bone_spine_0,
	bone_spine_1,
	bone_spine_2,
	bone_spine_3,
	bone_neck,
	bone_head,
	bone_clavicle_l,
	bone_arm_upper_l,
	bone_arm_lower_l,
	bone_hand_l,
	bone_finger_middle_meta_l,
	bone_finger_middle_0_l,
	bone_finger_middle_1_l,
	bone_finger_middle_2_l,
	bone_finger_pinky_meta_l,
	bone_finger_pinky_0_l,
	bone_finger_pinky_1_l,
	bone_finger_pinky_2_l,
	bone_finger_index_meta_l,
	bone_finger_index_0_l,
	bone_finger_index_1_l,
	bone_finger_index_2_l,
	bone_finger_thumb_0_l,
	bone_finger_thumb_1_l,
	bone_finger_thumb_2_l,
	bone_finger_ring_meta_l,
	bone_finger_ring_0_l,
	bone_finger_ring_1_l,
	bone_finger_ring_2_l,
	bone_weapon_hand_l,
	bone_arm_lower_l_twist,
	bone_arm_lower_l_twist1,
	bone_arm_upper_l_twist,
	bone_arm_upper_l_twist1,
	bone_clavicle_r,
	bone_arm_upper_r,
	bone_arm_lower_r,
	bone_hand_r,
	bone_finger_middle_meta_r,
	bone_finger_middle_0_r,
	bone_finger_middle_1_r,
	bone_finger_middle_2_r,
	bone_finger_pinky_meta_r,
	bone_finger_pinky_0_r,
	bone_finger_pinky_1_r,
	bone_finger_pinky_2_r,
	bone_finger_index_meta_r,
	bone_finger_index_0_r,
	bone_finger_index_1_r,
	bone_finger_index_2_r,
	bone_finger_thumb_0_r,
	bone_finger_thumb_1_r,
	bone_finger_thumb_2_r,
	bone_finger_ring_meta_r,
	bone_finger_ring_0_r,
	bone_finger_ring_1_r,
	bone_finger_ring_2_r,
	bone_weapon_hand_r,
	bone_arm_lower_r_twist,
	bone_arm_lower_r_twist1,
	bone_arm_upper_r_twist,
	bone_arm_upper_r_twist1,
	bone_leg_upper_l,
	bone_leg_lower_l,
	bone_ankle_l,
	bone_ball_l,
	bone_lfoot_lock,
	bone_leg_upper_l_twist,
	bone_leg_upper_l_twist1,
	bone_leg_upper_r,
	bone_leg_lower_r,
	bone_ankle_r,
	bone_ball_r,
	bone_rfoot_lock,
	bone_leg_upper_r_twist,
	bone_leg_upper_r_twist1,
	bone_finger_pinky_l_end,
	bone_finger_pinky_r_end,
	bone_valvebiped_weapon_bone,
	bone_lh_ik_driver,
	bone_primary_jiggle_jnt,
};

enum e_hitbox_index : int
{
	hitbox_invalid = -1,
	hitbox_head,
	hitbox_neck,
	hitbox_pelvis,
	hitbox_stomach,
	hitbox_thorax,
	hitbox_chest,
	hitbox_upper_chest,
	hitbox_right_thigh,
	hitbox_left_thigh,
	hitbox_right_calf,
	hitbox_left_calf,
	hitbox_right_foot,
	hitbox_left_foot,
	hitbox_right_hand,
	hitbox_left_hand,
	hitbox_right_upper_arm,
	hitbox_right_forearm,
	hitbox_left_upper_arm,
	hitbox_left_forearm,
	hitbox_max
};

enum e_render_flags : unsigned int
{
	render_flags_disable_rendering = 0x01,
	render_flags_haschanged = 0x02,
	render_flags_alternate_sorting = 0x04,
	render_flags_render_with_viewmodels = 0x08,
	render_flags_bloat_bounds = 0x10,
	render_flags_bounds_valid = 0x20,
	render_flags_bounds_always_recompute = 0x40,
	render_flags_is_sprite = 0x80,
	render_flags_force_opaque_pass = 0x100,
};

enum e_translucency_type : int
{
	renderable_is_opaque = 0,
	renderable_is_translucent,
	renderable_is_two_pass
};
#pragma endregion

using mdl_handle_t = std::uint16_t;

struct model_t
{
	std::byte	pad0[ 0x4 ];		//0x0000
	char		name[ 260 ];	//0x0004
	int			load_flags;		//0x0108
	int			server_count;	//0x010C
	int			type;			//0x0110
	int			flags;			//0x0114
	vec3_t		mins;		//0x0118
	vec3_t		maxs;		//0x0124
	float		radius;		//0x0130
	std::byte	pad1[ 0x1C ];		//0x0134
};

class c_phys_collide;
class i_material;
class c_utl_buffer;
class i_client_renderable;

class i_model_info
{
public:
	virtual							~i_model_info( )
	{ }
	virtual const model_t* get_model( int model_index ) const = 0;
	virtual int						get_model_index( const char* name ) const = 0;
	virtual const char* get_model_name( const model_t* model ) const = 0;
	virtual vcollide_t* get_v_collide( const model_t* model ) const = 0;
	virtual vcollide_t* get_v_collide( int model_index ) const = 0;
	virtual vcollide_t* get_v_collide( const model_t* model, float scale ) const = 0;
	virtual vcollide_t* get_v_collide( int model_index, float scale ) const = 0;
	virtual void					get_model_bounds( const model_t* model, vec3_t& mins, vec3_t& maxs ) const = 0;
	virtual void					get_model_render_bounds( const model_t* model, vec3_t& mins, vec3_t& maxs ) const = 0;
	virtual int						get_model_frame_count( const model_t* model ) const = 0;
	virtual int						get_model_type( const model_t* model ) const = 0;
	virtual void* get_model_extra_data( const model_t* model ) = 0;
	virtual bool					model_has_material_proxy( const model_t* model ) const = 0;
	virtual bool					is_translucent( model_t const* model ) const = 0;
	virtual bool					is_translucent_two_pass( const model_t* model ) const = 0;
private:
	virtual void					unused0( )
	{ }
public:
	virtual e_translucency_type		compute_translucency_type( const model_t* model, int nSkin, int nBody ) = 0;
	virtual int						get_model_material_count( const model_t* model ) const = 0;
	virtual void					get_model_materials( const model_t* model, int iCount, i_material** materials ) = 0;
	virtual bool					is_model_vertex_lit( const model_t* model ) const = 0;
	virtual const char* get_model_key_value_text( const model_t* model ) = 0;
	virtual bool					get_model_key_value( const model_t* model, c_utl_buffer& buf ) = 0;
	virtual float					get_model_radius( const model_t* model ) = 0;
	virtual const studiohdr_t* find_model( const studiohdr_t* studio_hdr, void** cache, const char* model_name ) const = 0;
	virtual const studiohdr_t* find_model( void* cache ) const = 0;
	virtual virtualmodel_t* get_virtual_model( const studiohdr_t* studio_hdr ) const = 0;
	virtual std::uint8_t* get_anim_block( const studiohdr_t* studio_hdr, int block ) const = 0;
	virtual bool					has_anim_block_been_preloaded( studiohdr_t const* studio_hdr, int block ) const = 0;
	virtual void					get_modelmaterial_color_and_lighting( const model_t* model, const vec3_t& origin, const vec3_t& angles, trace_t* trace, vec3_t& lighting, vec3_t& color ) = 0;
	virtual void					get_illumination_point( const model_t* model, i_client_renderable* renderable, const vec3_t& origin, const vec3_t& angles, vec3_t* lighting_center ) = 0;
	virtual int						get_model_contents( int model_index ) const = 0;
	virtual studiohdr_t* get_studio_model( const model_t* model ) = 0;
	virtual int						get_model_sprite_width( const model_t* model ) const = 0;
	virtual int						get_model_sprite_height( const model_t* model ) const = 0;
	virtual void					set_level_screen_fade_range( float min, float max ) = 0;
	virtual void					get_level_screen_fade_range( float* min, float* max ) const = 0;
	virtual void					set_view_screen_fade_range( float min, float max ) = 0;
	virtual unsigned char			compute_level_screen_fade( const vec3_t& abs_origin, float radius, float fade_scale ) const = 0;
	virtual unsigned char			compute_view_screen_fade( const vec3_t& abs_origin, float radius, float fade_scale ) const = 0;
	virtual int						get_autoplay_list( const studiohdr_t* studio_hdr, unsigned short** autoplay_list ) const = 0;
	virtual c_phys_collide* get_collide_for_virtual_terrain( int index ) = 0;
	virtual bool					is_using_fb_texture( const model_t* model, int skin, int body, i_client_renderable* client_renderable ) const = 0;
	virtual const model_t* find_or_load_model( const char* szName ) const = 0;
	virtual mdl_handle_t				get_cache_handle( const model_t* model ) const = 0;
	virtual int						get_brush_model_plane_count( const model_t* model ) const = 0;
	virtual void					get_brush_model_plane( const model_t* model, int index, cplane_t& plane, vec3_t* origin ) const = 0;
	virtual int						get_surfaceprops_for_virtual_terrain( int index ) = 0;
	virtual bool					uses_env_cubemap( const model_t* model ) const = 0;
	virtual bool					uses_static_lighting( const model_t* model ) const = 0;
	virtual int						register_dynamic_model( const char* name, bool client_side ) = 0;
	virtual int						register_combined_dynamic_model( const char* name, mdl_handle_t handle ) = 0;
	virtual void					update_combined_dynamic_model( int model_index, mdl_handle_t handle ) = 0;
	virtual int						begin_combined_model( const char* name, bool reuse_existing ) = 0;
};
