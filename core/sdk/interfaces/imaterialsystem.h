#pragma once

#include "imatrendercontext.h"

using material_handle_t = std::uint16_t;

#pragma region materialsystem_enumerations
enum e_create_render_target_flags : unsigned int
{
	createrendertargetflags_hdr = 0x00000001,
	createrendertargetflags_automipmap = 0x00000002,
	createrendertargetflags_unfilterable_ok = 0x00000004,
	createrendertargetflags_noedram = 0x00000008,
	createrendertargetflags_temp = 0x00000010
};

enum e_texture_flags : unsigned int
{
	textureflags_pointsample = 0x00000001,
	textureflags_trilinear = 0x00000002,
	textureflags_clamps = 0x00000004,
	textureflags_clampt = 0x00000008,
	textureflags_anisotropic = 0x00000010,
	textureflags_hint_dxt5 = 0x00000020,
	textureflags_srgb = 0x00000040,
	textureflags_normal = 0x00000080,
	textureflags_nomip = 0x00000100,
	textureflags_nolod = 0x00000200,
	textureflags_all_mips = 0x00000400,
	textureflags_procedural = 0x00000800,
	textureflags_onebitalpha = 0x00001000,
	textureflags_eightbitalpha = 0x00002000,
	textureflags_envmap = 0x00004000,
	textureflags_rendertarget = 0x00008000,
	textureflags_depthrendertarget = 0x00010000,
	textureflags_nodebugoverride = 0x00020000,
	textureflags_singlecopy = 0x00040000,
	textureflags_unused_00080000 = 0x00080000,
	textureflags_unused_00100000 = 0x00100000,
	textureflags_unused_00200000 = 0x00200000,
	textureflags_unused_00400000 = 0x00400000,
	textureflags_nodepthbuffer = 0x00800000,
	textureflags_unused_01000000 = 0x01000000,
	textureflags_clampu = 0x02000000,
	textureflags_vertextexture = 0x04000000,
	textureflags_ssbump = 0x08000000,
	textureflags_unused_10000000 = 0x10000000,
	textureflags_border = 0x20000000,
	textureflags_unused_40000000 = 0x40000000,
	textureflags_unused_80000000 = 0x80000000
};

enum e_clear_flags : unsigned int
{
	view_clear_color = 0x1,
	view_clear_depth = 0x2,
	view_clear_full_target = 0x4,
	view_no_draw = 0x8,
	view_clear_obey_stencil = 0x10,
	view_clear_stencil = 0x20
};

enum e_render_target_size_mode : int
{
	rt_size_no_change = 0,					// only allowed for render targets that don't want a depth buffer
	rt_size_default,						// don't play with the specified width and height other than making sure it fits in the framebuffer.
	rt_size_picmip,							// apply picmip to the render target's width and height.
	rt_size_hdr,							// frame_buffer_width / 4
	rt_size_full_frame_buffer,				// same size as frame buffer, or next lower power of 2 if we can't do that.
	rt_size_offscreen,						// target of specified size, don't mess with dimensions
	rt_size_full_frame_buffer_rounded_up,	// same size as the frame buffer, rounded up if necessary for systems that can't do non-power of two textures.
	rt_size_replay_screenshot,				// rounded down to power of 2, essentially...
	rt_size_literal							// use the size passed in. don't clamp it to the frame buffer size. really.
};

enum e_material_render_target_depth : unsigned int
{
	material_rt_depth_shared = 0x0,
	material_rt_depth_separate = 0x1,
	material_rt_depth_none = 0x2,
	material_rt_depth_only = 0x3
};
#pragma endregion

// @credits: https://github.com/pmrowla/hl2sdk-csgo/blob/master/public/materialsystem/materialsystem_config.h
struct material_video_mode_t
{
	int				width;
	int				height;
	e_image_format format;
	int				refresh_rate;
};

struct material_system_config_t
{
	material_video_mode_t video_mode;
	float				monitor_gamma;
	float				gamma_tv_range_min;
	float				gamma_tv_range_max;
	float				gamma_tv_exponent;
	bool				gamma_tv_enabled;
	bool				triple_buffered;
	int					aa_samples;
	int					force_anisotropic_level;
	int					skip_mip_levels;
	int					dx_support_level;
	unsigned int		flags;
	bool				edit_mode;
	unsigned char		proxies_test_mode;
	bool				compressed_textures;
	bool				filter_lightmaps;
	bool				filter_textures;
	bool				reverse_depth;
	bool				buffer_primitives;
	bool				draw_flat;
	bool				measure_fill_rate;
	bool				visualize_fill_rate;
	bool				no_transparency;
	bool				software_lighting;
	bool				allow_cheats;
	char				show_mip_levels;
	bool				show_low_res_image;
	bool				show_normal_map;
	bool				mip_map_textures;
	unsigned char		fullbright;
	bool				fast_no_bump;
	bool				suppress_rendering;
	bool				draw_gray;
	bool				show_specular;
	bool				show_diffuse;
	std::uint32_t		windowed_size_limit_width;
	std::uint32_t		windowed_size_limit_height;
	int					aa_quality;
	bool				shadow_depth_texture;
	bool				motion_blur;
	bool				support_flashlight;
	bool				paint_enabled;
	std::byte			pad0[ 0xC ];
};

class i_material_system
{
public:
	e_image_format get_back_buffer_format( )
	{
		return c_memory::call_vfunc<e_image_format>( this, 36 );
	}

	i_material* create_material( const char* name, c_key_values* key_values )
	{
		return c_memory::call_vfunc<i_material*>( this, 83, name, key_values );
	}

	i_material* find_material( char const* material_name, const char* texture_group_name = TEXTURE_GROUP_MODEL, bool complain = true, const char* complain_prefix = nullptr )
	{
		return c_memory::call_vfunc<i_material*>( this, 84, material_name, texture_group_name, complain, complain_prefix );
	}

	material_handle_t first_material( )
	{
		return c_memory::call_vfunc<material_handle_t>( this, 86 );
	}

	material_handle_t next_material( material_handle_t material )
	{
		return c_memory::call_vfunc<material_handle_t>( this, 87, material );
	}

	material_handle_t invalid_material( )
	{
		return c_memory::call_vfunc<material_handle_t>( this, 88 );
	}

	i_material* get_material( material_handle_t material )
	{
		return c_memory::call_vfunc<i_material*>( this, 89, material );
	}

	int	get_num_materials( )
	{
		return c_memory::call_vfunc<int>( this, 90 );
	}

	i_texture* find_texture( char const* texture_name, const char* texture_group_name, bool complain = true, int additional_creation_flags = 0 )
	{
		return c_memory::call_vfunc<i_texture*>( this, 91, texture_name, texture_group_name, complain, additional_creation_flags );
	}

	void begin_render_target_allocation( )
	{
		c_memory::call_vfunc<void>( this, 94 );
	}

	void end_render_target_allocation( )
	{
		c_memory::call_vfunc<void>( this, 95 );
	}

	void force_begin_render_target_allocation( )
	{
		const bool bOldState = disable_render_target_allocation_forever( );

		disable_render_target_allocation_forever( ) = false;
		begin_render_target_allocation( );
		disable_render_target_allocation_forever( ) = bOldState;
	}

	void force_end_render_target_allocation( )
	{
		const bool bOldState = disable_render_target_allocation_forever( );

		disable_render_target_allocation_forever( ) = false;
		end_render_target_allocation( );
		disable_render_target_allocation_forever( ) = bOldState;
	}

	i_texture* create_render_target_texture( int width, int height, e_render_target_size_mode size_mode, e_image_format format, e_material_render_target_depth depth = material_rt_depth_shared )
	{
		return c_memory::call_vfunc<i_texture*>( this, 96, width, height, size_mode, format, depth );
	}

	i_texture* create_named_render_target_texture_ex( const char* name, int width, int height, e_render_target_size_mode size_mode, e_image_format format, e_material_render_target_depth depth = material_rt_depth_shared, unsigned int texture_flags = 0U, unsigned int render_target_flags = createrendertargetflags_hdr )
	{
		return c_memory::call_vfunc<i_texture*>( this, 97, name, width, height, size_mode, format, depth, texture_flags, render_target_flags );
	}

	i_texture* create_named_render_target_texture( const char* name, int width, int height, e_render_target_size_mode size_mode, e_image_format format, e_material_render_target_depth depth = material_rt_depth_shared, bool clamp_tex_coords = true, bool auto_mip_map = false )
	{
		return c_memory::call_vfunc<i_texture*>( this, 98, name, width, height, size_mode, format, depth, clamp_tex_coords, auto_mip_map );
	}

	i_texture* create_named_render_target_texture_ex2( const char* name, int width, int height, e_render_target_size_mode size_mode, e_image_format format, e_material_render_target_depth depth = material_rt_depth_shared, unsigned int texture_flags = 0U, unsigned int render_target_flags = createrendertargetflags_hdr )
	{
		return c_memory::call_vfunc<i_texture*>( this, 99, name, width, height, size_mode, format, depth, texture_flags, render_target_flags );
	}

	i_mat_render_context* get_render_context( )
	{
		return c_memory::call_vfunc<i_mat_render_context*>( this, 115 );
	}

	void finish_render_target_allocation( )
	{
		c_memory::call_vfunc<void>( this, 136 );
	}

	// i realize if i call this all textures will be unloaded and load time will suffer horribly
	void re_enable_render_target_allocation( )
	{
		c_memory::call_vfunc<void>( this, 137 );
	}

	bool& disable_render_target_allocation_forever( )
	{
		// @xref: "Tried BeginRenderTargetAllocation after game startup. If I let you do this, all users would suffer.\n"
		static auto disable_render_target_allocation_forever = *reinterpret_cast< std::uintptr_t* >( c_memory::find_pattern( "materialsystem.dll", "80 B9 ? ? ? ? ? 74 0F" ) + 0x2 );
		return *reinterpret_cast< bool* >( reinterpret_cast< std::uintptr_t >( this ) + disable_render_target_allocation_forever );
	}
};

class c_mat_render_context_ptr : public c_ref_ptr<i_mat_render_context>
{
	typedef c_ref_ptr<i_mat_render_context> c_base_class;
public:
	c_mat_render_context_ptr( ) = default;

	c_mat_render_context_ptr( i_mat_render_context* init ) : c_base_class( init )
	{
		if ( c_base_class::object != nullptr )
			c_base_class::object->begin_render( );
	}

	c_mat_render_context_ptr( i_material_system* from ) : c_base_class( from->get_render_context( ) )
	{
		if ( c_base_class::object != nullptr )
			c_base_class::object->begin_render( );
	}

	~c_mat_render_context_ptr( )
	{
		if ( c_base_class::object != nullptr )
			c_base_class::object->end_render( );
	}

	i_mat_render_context* operator=( i_mat_render_context* second_context )
	{
		if ( second_context != nullptr )
			second_context->begin_render( );

		return c_base_class::operator=( second_context );
	}

	void safe_release( )
	{
		if ( c_base_class::object != nullptr )
			c_base_class::object->end_render( );

		c_base_class::safe_release( );
	}

	void assign_add_reference( i_mat_render_context* from )
	{
		if ( c_base_class::object )
			c_base_class::object->end_render( );

		c_base_class::assign_add_reference( from );
		c_base_class::object->begin_render( );
	}

	void get_from( i_material_system* from )
	{
		assign_add_reference( from->get_render_context( ) );
	}

private:
	c_mat_render_context_ptr( const c_mat_render_context_ptr& ref_ptr );
	void operator=( const c_mat_render_context_ptr& second_ref_ptr );
};
