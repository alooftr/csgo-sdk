#pragma once
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/datacache/imdlcache.h
#include "iclientmodeshared.h"

#pragma region mdlcache_enumerations
enum
{
	mdlhandle_invalid = static_cast< mdl_handle_t >( ~0 )
};

enum mdl_cache_data_type_t : int
{
	mdlcache_studiohdr = 0,
	mdlcache_studiohwdata,
	mdlcache_vcollide,
	mdlcache_animblock,
	mdlcache_virtualmodel,
	mdlcache_vertexes,
	mdlcache_decodedanimblock,
};

enum mdl_cache_flush_t : unsigned int
{
	mdlcache_flush_studiohdr = 0x01,
	mdlcache_flush_studiohwdata = 0x02,
	mdlcache_flush_vcollide = 0x04,
	mdlcache_flush_animblock = 0x08,
	mdlcache_flush_virtualmodel = 0x10,
	mdlcache_flush_autoplay = 0x20,
	mdlcache_flush_vertexes = 0x40,
	mdlcache_flush_combined_data = 0x80,
	mdlcache_flush_ignorelock = 0x80000000,
	mdlcache_flush_all = 0xFFFFFFFF
};
#pragma endregion

class i_mdl_cache_notify
{
public:
	virtual void on_data_loaded( mdl_cache_data_type_t type, mdl_handle_t model ) = 0;
	virtual void on_combiner_pre_cache( mdl_handle_t old, mdl_handle_t new_handle ) = 0;
	virtual void on_data_unloaded( mdl_cache_data_type_t type, mdl_handle_t model ) = 0;
	virtual bool should_supress_load_warning( mdl_handle_t model ) = 0;
};

class i_mdl_cache : public i_app_system
{
public:
	virtual void set_cache_notify( i_mdl_cache_notify* notify ) = 0;
	virtual mdl_handle_t find_mdl( const char* mdl_relative_path ) = 0;
	virtual int add_reference( mdl_handle_t model ) = 0;
	virtual int release( mdl_handle_t model ) = 0;
	virtual int get_reference( mdl_handle_t model ) = 0;
	virtual studiohdr_t* get_studio_hdr( mdl_handle_t model ) = 0;
	virtual studiohwdata_t* get_hardware_data( mdl_handle_t model ) = 0;
	virtual vcollide_t* get_v_collide( mdl_handle_t model ) = 0;
	virtual vcollide_t* get_v_collide( mdl_handle_t model, float scale ) = 0;
	virtual unsigned char* get_anim_block( mdl_handle_t model, int block, bool preload_if_missing ) = 0;
	virtual bool has_anim_block_been_preloaded( mdl_handle_t model, int block ) = 0;
	virtual virtualmodel_t* get_virtual_model( mdl_handle_t model ) = 0;
	virtual int get_autoplay_list( mdl_handle_t model, unsigned short** pOut ) = 0;
	virtual vertexfileheader_t* get_vertex_data( mdl_handle_t model ) = 0;
	virtual void touch_all_data( mdl_handle_t model ) = 0;
	virtual void set_user_data( mdl_handle_t model, void* pData ) = 0;
	virtual void* get_user_data( mdl_handle_t model ) = 0;
	virtual bool is_error_model( mdl_handle_t model ) = 0;
	virtual bool is_over_budget( mdl_handle_t model ) = 0;
	virtual void flush( mdl_cache_flush_t flush_flags = mdlcache_flush_all ) = 0;
	virtual void flush( mdl_handle_t model, mdl_cache_flush_t flush_flags = mdlcache_flush_all ) = 0;
	virtual const char* get_model_name( mdl_handle_t model ) = 0;
	virtual void* get_cache_section( mdl_cache_data_type_t type ) = 0;
	virtual virtualmodel_t* get_virtual_model_fast( const studiohdr_t* studio_hdr, mdl_handle_t model ) = 0;
	virtual void begin_lock( ) = 0;
	virtual void end_lock( ) = 0;
	virtual int* get_frame_unlock_counter_ptr_old( ) = 0;
	virtual void finish_pending_loads( ) = 0;
	virtual vcollide_t* get_v_collide_ex( mdl_handle_t model, bool synchronous_load = true ) = 0;
	virtual bool get_v_collide_size( mdl_handle_t model, int* collide_size ) = 0;
	virtual bool get_async_load( mdl_cache_data_type_t type ) = 0;
	virtual bool set_async_load( mdl_cache_data_type_t type, bool async ) = 0;
	virtual void begin_map_load( ) = 0;
	virtual void end_map_load( ) = 0;
	virtual void mark_as_loaded( mdl_handle_t model ) = 0;
	virtual void init_preload_data( bool rebuild ) = 0;
	virtual void shutdown_preload_data( ) = 0;
	virtual bool is_data_loaded( mdl_handle_t model, mdl_cache_data_type_t type ) = 0;
	virtual int* get_frame_unlock_counter_ptr( mdl_cache_data_type_t type ) = 0;
	virtual studiohdr_t* lock_studio_hdr( mdl_handle_t model ) = 0;
	virtual void unlock_studio_hdr( mdl_handle_t model ) = 0;
	virtual bool preload_model( mdl_handle_t model ) = 0;
	virtual void reset_error_model_status( mdl_handle_t model ) = 0;
	virtual void mark_frame( ) = 0;
	virtual void begin_coarse_lock( ) = 0;
	virtual void end_coarse_lock( ) = 0;
	virtual void reload_v_collide( mdl_handle_t model ) = 0;
	virtual bool release_anim_block_allocator( ) = 0;
	virtual bool restore_hardware_data( mdl_handle_t model, void* async_vtx_control, void* async_vvd_control ) = 0;
	virtual void disable_v_collide_load( ) = 0;
	virtual void enable_v_collide_load( ) = 0;
	virtual void disable_file_not_found_warnings( ) = 0;
	virtual void enable_file_not_found_warnings( ) = 0;
	virtual bool process_pending_hardware_restore( ) = 0;
	virtual void unload_queued_hardware_data( ) = 0;
	virtual void dump_dictionary_state( ) = 0;
	virtual mdl_handle_t	create_combined_model( const char* model_name ) = 0;
	virtual bool create_combined_model( mdl_handle_t model ) = 0;
	virtual bool set_combine_models( mdl_handle_t model, const c_utl_vector<void*>& models_to_combine ) = 0;
	virtual bool finish_combined_model( mdl_handle_t model, void* function, void* user_data = nullptr ) = 0;
	virtual bool is_combined_placeholder( mdl_handle_t model ) = 0;
	virtual bool is_combined_model( mdl_handle_t model ) = 0;
	virtual int get_num_combined_sub_models( mdl_handle_t model ) = 0;
	virtual void get_combined_sub_model_filename( mdl_handle_t model, int sub_model_index, char* result, int result_size ) = 0;
	virtual void* get_combined_material_kv( mdl_handle_t model, int atlas_group = 0 ) = 0;
	virtual void update_combiner( ) = 0;
	virtual void* get_combined_internal_asset( int asset_type, const char* asset_id = nullptr, int* size = nullptr ) = 0;
	virtual void set_combiner_flags( unsigned int flags ) = 0;
	virtual void ClearCombinerFlags( unsigned int flags ) = 0;
	virtual void debug_combiner_info( ) = 0;
};

class c_mdl_cache_critical_section
{
public:
	c_mdl_cache_critical_section( i_mdl_cache* cache )
		: cache( cache )
	{
		this->cache->begin_lock( );
	}

	~c_mdl_cache_critical_section( )
	{
		this->cache->end_lock( );
	}
private:
	i_mdl_cache* cache;
};

class c_mdl
{
public:
	std::byte	pad[ 0x3C ]; // 0x0000
	mdl_handle_t model_handle; // 0x003C
	std::byte	pad0[ 0x8 ]; // 0x003E
	col_t		color; // 0x0046
	std::byte	pad1[ 0x2 ]; // 0x004A
	int			skin; // 0x004C
	int			body; // 0x0050
	int			sequence; // 0x0054
	int			lod; // 0x0058
	float		playback_rate; // 0x005C
	float		time; // 0x0060
	float		current_anim_end_time; // 0x0064
	float		flex_controls[ MAXSTUDIOFLEXCTRL * 4 ]; // 0x0068
	vec3_t		view_target; // 0x0668
	bool		world_space_view_target; // 0x0674
	bool		use_sequence_playback_fps; // 0x0675
	std::byte	pad2[ 0x2 ]; // 0x0676
	void* proxy_data; // 0x0678
	float		time_basis_adjustment; // 0x067C
	std::byte	pad3[ 0x4 ]; // 0x0680 --isn't correct after this point iirc
	c_utl_vector<int> sequence_follow_loop; // 0x0684
	matrix3x4_t	model_to_world; // 0x0698
	bool		request_bone_merge_takeover; // 0x06C8
}; // Size: 0x06C9 // 0x6D0?

class i_texture;
class i_render_to_rt_helper_object
{
public:
	virtual void draw( const matrix3x4_t& root_to_world ) = 0;
	virtual bool bounding_sphere( vec3_t& center, float& radius ) = 0;
	virtual i_texture* get_env_cube_map( ) = 0;
};

typedef unsigned short mdl_handle_t;
class c_merged_mdl : public i_render_to_rt_helper_object
{
public:
	virtual ~c_merged_mdl( )
	{ }
	virtual void set_mdl( mdl_handle_t model_handle, void* custom_material_owner = nullptr, void* proxy_data = nullptr ) = 0;
	virtual void set_mdl( const char* model_path, void* custom_material_owner = nullptr, void* proxy_data = nullptr ) = 0;

	void setup_bones_for_attachment_queries( )
	{
		static auto setup_bones_for_attachment_queries_addr = c_memory::find_pattern( "client.dll", "55 8B EC 83 EC 14 83 3D ? ? ? ? ? 53" );

		assert( setup_bones_for_attachment_queries_addr != 0 );

		reinterpret_cast< void( __thiscall* )( void* ) >( setup_bones_for_attachment_queries_addr )( this );
	}

	void set_merge_mdl( const char* model_path, void* custom_material_owner = nullptr, void* proxy_data = nullptr, const bool request_bone_merge_takeover = false )
	{
		static auto set_merge_mdl_addr = c_memory::find_pattern( "client.dll", "55 8B EC 57 8B F9 8B 0D ? ? ? ? 85 C9 75" );

		assert( set_merge_mdl_addr != 0 );

		reinterpret_cast< void( __thiscall* )( void*, const char*, void*, void*, bool ) >( set_merge_mdl_addr )( this, model_path, custom_material_owner, proxy_data, request_bone_merge_takeover );
	}

	void set_sequence( const int32_t sequence, const bool use_sequence_playback_fps )
	{
		root_mdl.sequence = sequence;
		root_mdl.use_sequence_playback_fps = use_sequence_playback_fps;
		root_mdl.time_basis_adjustment = root_mdl.time;
	}

	c_mdl root_mdl;
};