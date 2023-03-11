#pragma once

#include "../data_types/matrix.h"
#include "../data_types/utl_vector.h"


using RadianEuler = float[ 3 ];
using Quaternion = float[ 4 ];

#pragma region studio_definitions
#define MAXSTUDIOBONECTRLS			4
#define MAXSTUDIOPOSEPARAM			24
#define MAXSTUDIOSKINS				32		// total textures
#define MAXSTUDIOFLEXCTRL			96		// maximum number of flexcontrollers (input sliders)
#define MAXSTUDIOBONES				128		// total bones actually used
#define MAXSTUDIOANIMBLOCKS			256
#define MAXSTUDIOFLEXDESC			1024	// maximum number of low level flexes (actual morph targets)

#define BONE_CALCULATE_MASK			0x1F
#define BONE_PHYSICALLY_SIMULATED	0x01
#define BONE_PHYSICS_PROCEDURAL		0x02
#define BONE_ALWAYS_PROCEDURAL		0x04
#define BONE_SCREEN_ALIGN_SPHERE	0x08
#define BONE_SCREEN_ALIGN_CYLINDER	0x10

#define BONE_USED_MASK				0x0007FF00
#define BONE_USED_BY_ANYTHING		0x0007FF00
#define BONE_USED_BY_HITBOX			0x00000100
#define BONE_USED_BY_ATTACHMENT		0x00000200
#define BONE_USED_BY_VERTEX_MASK	0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0	0x00000400
#define BONE_USED_BY_VERTEX_LOD1	0x00000800
#define BONE_USED_BY_VERTEX_LOD2	0x00001000
#define BONE_USED_BY_VERTEX_LOD3	0x00002000
#define BONE_USED_BY_VERTEX_LOD4	0x00004000
#define BONE_USED_BY_VERTEX_LOD5	0x00008000
#define BONE_USED_BY_VERTEX_LOD6	0x00010000
#define BONE_USED_BY_VERTEX_LOD7	0x00020000
#define BONE_USED_BY_BONE_MERGE		0x00040000
#define BONE_ALWAYS_SETUP			0x00080000

#define BONE_USED_BY_VERTEX_AT_LOD( iLOD ) ( BONE_USED_BY_VERTEX_LOD0 << ( iLOD ) )
#define BONE_USED_BY_ANYTHING_AT_LOD( iLOD ) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD( iLOD ) )

#define MAX_NUM_LODS				8

#define BONE_TYPE_MASK				0x00F00000
#define BONE_FIXED_ALIGNMENT		0x00100000
#define BONE_HAS_SAVEFRAME_POS		0x00200000
#define BONE_HAS_SAVEFRAME_ROT64	0x00400000
#define BONE_HAS_SAVEFRAME_ROT32	0x00800000
#pragma endregion

struct vcollide_t;
struct virtualmodel_t;
struct vertexfileheader_t;

struct studio_sequences_desc_t
{
	std::byte pad[ 0x104 ]; //0
	float fade_in_time; //104
	float fade_out_time; //108
	std::byte pad1[ 0x84 ]; //0
	int num_anim_tags; // 196
};


struct studiohdr_t;
class c_studio_hdr
{
public:
	studiohdr_t* studio_hdr;
	virtualmodel_t* virtual_model;
	void* softbody;
	mutable c_utl_vector<const studiohdr_t*> studio_hdr_cache;
	mutable int		mutable_frame_unlock_counter;
	int* pointer_frame_unlock_counter;
	std::byte		pad0[ 0x8 ];
	c_utl_vector<int>	bone_flags;
	c_utl_vector<int>	bone_parent;
	void* activity_to_sequence;

	studio_sequences_desc_t seqdesc( int sequence )
	{
		using seqdesc_fn = int( __thiscall* )( void*, int );
		static auto o_seqdesc = c_memory::get_absolute<seqdesc_fn>( c_memory::find_pattern( "client.dll", "E8 ? ? ? ? 03 40 04" ) + 0x1 );

		return *reinterpret_cast< studio_sequences_desc_t* >( o_seqdesc( this, sequence ) );
	}
};

struct mstudiobone_t
{
	int			name_index;
	int			parent;			// parent bone
	int			bone_controller[ 6 ]; // bone controller index, -1 == none

	// default values
	vec3_t	position;
	Quaternion	world;
	RadianEuler rad_rotation;

	// compression scale
	vec3_t	position_scale;
	vec3_t	euler_scale;

	matrix3x4_t pose_to_bone;
	Quaternion	alignment;
	int			flags;
	int			procedure_type;
	int			procedure_index;	// procedural rule
	mutable int physics_bone;		// index into physically simulated bone
	int			surface_prop_index;	// index into string table for property name
	int			contents;			// see bspflags.h for the contents flags
	int			surface_prop;		// this index must be cached by the loader, not saved in the file
	std::byte	pad0[ 0x1C ];

	inline const char* get_name( )
	{
		if ( !name_index ) return nullptr;
		return ( char* )this + name_index;
	}

	inline void* GetProcedure( ) const
	{
		if ( !procedure_index ) return nullptr;
		return ( std::uint8_t* )this + procedure_index;
	}

	inline const char* get_surface_prop_name( ) const
	{
		if ( !surface_prop_index ) return nullptr;
		return ( char* )this + surface_prop_index;
	}
};

struct mstudiobonecontroller_t
{
	int			bone; // -1 == 0
	int			type; // X, Y, Z, XR, YR, ZR, M
	float		start;
	float		end;
	int			rest; // byte index value at rest
	int			input_field; // 0-3 user set controller, 4 mouth
	std::byte	pad0[ 0x40 ];
};

struct mstudiobbox_t
{
	int			bone;
	int			group; // intersection group
	vec3_t	bb_min; // bounding box
	vec3_t	bb_max;
	int			hitbox_name_index; // offset to the name of the hitbox
	vec3_t	offset_orientation;
	float		radius;
	std::byte	pad0[ 0x10 ];

	inline const char* get_hitbox_name( ) const
	{
		if ( !hitbox_name_index ) return nullptr;
		return ( const char* )this + hitbox_name_index;
	}
};

struct mstudiohitboxset_t
{
	int name_index;
	int hitboxes;
	int hitbox_index;

	inline const char* get_name( ) const
	{
		if ( !name_index ) return nullptr;
		return ( char* )this + name_index;
	}

	inline mstudiobbox_t* get_hitbox( int iHitBox ) const
	{
		if ( iHitBox < 0 || iHitBox >= hitboxes ) return nullptr;
		return ( mstudiobbox_t* )( ( std::uint8_t* )this + hitbox_index ) + iHitBox;
	}
};

class virtualgroup_t
{
public:
	void* pCache;
	c_utl_vector<int> bone_map;
	c_utl_vector<int> master_bone;
	c_utl_vector<int> master_sequence;
	c_utl_vector<int> master_anim;
	c_utl_vector<int> master_attachment;
	c_utl_vector<int> master_pose;
	c_utl_vector<int> master_node;
};

struct virtualsequence_t
{
	int	flags;
	int activity;
	int group;
	int index;
};

struct virtualgeneric_t
{
	int group;
	int index;
};

struct virtualmodel_t
{
	inline virtualgroup_t* get_anim_group( const int animation )
	{
		// Note: user must manage mutex for this
		return &group[ anim[ animation ].group ];
	}

	inline virtualgroup_t* GetSequenceGroup( const int sequence_number )
	{
		// Note: user must manage mutex for this
		return &group[ sequence[ sequence_number ].group ];
	}

	std::byte pad0[ 0x8 ]; // CThreadFastMutex
	c_utl_vector<virtualsequence_t> sequence;
	c_utl_vector<virtualgeneric_t> anim;
	c_utl_vector<virtualgeneric_t> attachment;
	c_utl_vector<virtualgeneric_t> pose;
	c_utl_vector<virtualgroup_t> group;
	c_utl_vector<virtualgeneric_t> node;
	c_utl_vector<virtualgeneric_t> i_k_lock;
	c_utl_vector<unsigned short> autoplay_sequences;
};

struct studiohdr_t;
struct mstudioseqdesc_t
{
	int base_studio;
	inline studiohdr_t* get_studio_hdr( ) const
	{
		return ( studiohdr_t* )( ( std::uint8_t* )this + base_studio );
	}

	int	label_index;
	inline const char* get_label( ) const
	{
		return ( char* )this + label_index;
	}

	int	activity_name_index;
	inline const char* get_activity_name( ) const
	{
		return ( char* )this + activity_name_index;
	}

	int	flags;

	int	activity;
	int	activity_weight;

	int events;
	int event_index;
	inline void* GetEvent( int iEvent ) const
	{
		if ( iEvent < 0 || iEvent >= events ) return nullptr;
		return ( ( std::uint8_t* )this + event_index ) + iEvent;
	}

	vec3_t bb_min;
	vec3_t bb_max;

	int	blends;
	int	blend_index;

	inline int get_anim_value( int x, int y ) const
	{
		if ( x >= group_size[ 0 ] )
			x = group_size[ 0 ] - 1;

		if ( y >= group_size[ 1 ] )
			y = group_size[ 1 ] - 1;

		int offset = y * group_size[ 0 ] + x;
		short* blends = ( short* )( ( std::uint8_t* )this + blend_index );
		return ( int )blends[ offset ];
	}

	int	movement_index;		// [blend] float array for blended movement
	int group_size[ 2 ];
	int param_index[ 2 ];		// X, Y, Z, XR, YR, ZR
	float param_start[ 2 ];	// local (0..1) starting value
	float param_end[ 2 ];	// local (0..1) ending value
	int	param_parent;

	float fade_in_time;		// ideal cross fate in time (0.2 default)
	float fade_out_time;	// ideal cross fade out time (0.2 default)

	int	entry_node;			// transition node at entry
	int	exit_node;			// transition node at exit
	int	node_flags;			// transition rules

	float entry_phase;		// used to match entry gait
	float exit_phase;		// used to match exit gait

	float last_frame;		// frame that should generation EndOfSequence

	int	next_sequence;		// auto advancing sequences
	int	pose;				// index of delta animation between end and nextseq

	int	i_k_rules;

	int	auto_layers;
	int	auto_layer_index;
	inline void* get_auto_layer( int add_auto_layer ) const
	{
		if ( add_auto_layer < 0 || add_auto_layer >= auto_layers ) return nullptr;
		return ( ( std::uint8_t* )this + auto_layer_index ) + add_auto_layer;
	}

	int	weight_list_index;
	inline float* get_bone_weight( int bone ) const
	{
		return ( ( float* )( ( std::uint8_t* )this + weight_list_index ) + bone );
	}

	int	pose_key_index;
	inline float* get_pose_key( int parameter, int animation ) const
	{
		return ( float* )( ( std::uint8_t* )this + pose_key_index ) + parameter * group_size[ 0 ] + animation;
	}

	int	ik_locks;
	int	ik_lock_index;
	inline void* get_ik_lock( int iIK ) const
	{
		if ( iIK < 0 || iIK >= ik_locks ) return nullptr;
		return ( ( std::uint8_t* )this + ik_lock_index ) + iIK;
	}

	int	key_value_index;
	int	key_value_size;
	inline const char* key_value_text( ) const
	{
		if ( !key_value_size ) return nullptr;
		return ( char* )this + key_value_index;
	}

	int	cycle_pose_index;

	int activity_modifier_index;
	int	activity_modifiers;

	int	anim_tag_index;
	int	anim_tags;

	int	root_driver_index;
	std::byte pad[ 0x8 ];
};

struct mstudioposeparamdesc_t
{
	int name_index;
	int flags;
	float start; // starting value
	float end; // ending value
	float loop; // looping range, 0 for no looping, 360 for rotations, etc.

	inline const char* get_name( ) const
	{
		return ( char* )this + name_index;
	}
};

struct studiohwdata_t;
struct studiohdr_t
{
	int id;
	int	version;
	int checksum;
	char name[ 64 ];
	int	length;

	vec3_t eye_position;
	vec3_t illum_position;
	vec3_t hull_min;
	vec3_t hull_max;
	vec3_t view_bb_min;
	vec3_t view_bb_max;

	int	flags;

	int	bones;
	int	bone_index;
	inline mstudiobone_t* get_bone( int bone ) const
	{
		if ( bone < 0 || bone >= bones ) return nullptr;
		return ( mstudiobone_t* )( ( std::uint8_t* )this + bone_index ) + bone;
	}

	int	bone_controllers;
	int	bone_controller_index;
	inline void* get_bone_controller( int bone_indexController ) const
	{
		if ( bone_indexController < 0 || bone_indexController >= bone_controllers ) return nullptr;
		return ( ( std::uint8_t* )this + bone_controller_index ) + bone_indexController;
	}

	int	hitbox_sets;
	int	hitbox_set_index;
	inline mstudiohitboxset_t* get_hitbox_set( int iSet ) const
	{
		if ( iSet < 0 || iSet >= hitbox_sets ) return nullptr;
		return ( mstudiohitboxset_t* )( ( std::uint8_t* )this + hitbox_set_index ) + iSet;
	}

	inline mstudiobbox_t* get_hitbox( int iHitBox, int iSet ) const
	{
		const mstudiohitboxset_t* hitbox_set = get_hitbox_set( iSet );

		if ( hitbox_set == nullptr )
			return nullptr;

		return hitbox_set->get_hitbox( iHitBox );
	}

	inline int get_hitbox_count( int iSet ) const
	{
		const mstudiohitboxset_t* hitbox_set = get_hitbox_set( iSet );

		if ( hitbox_set == nullptr )
			return 0;

		return hitbox_set->hitboxes;
	}

	int	local_animations;
	int	local_animation_index;
	inline void* get_anim_description( int animation ) const
	{
		if ( animation < 0 || animation >= local_animations ) animation = 0;
		return ( ( std::uint8_t* )this + local_animation_index ) + animation;
	}

	int local_sequences;
	int local_sequence_index;
	inline mstudioseqdesc_t* get_local_sequence_description( int sequence ) const
	{
		if ( sequence < 0 || sequence >= local_sequences ) sequence = 0;
		return ( mstudioseqdesc_t* )( ( std::uint8_t* )this + local_sequence_index ) + sequence;
	}

	inline mstudioseqdesc_t& get_sequence_description( int sequence, virtualmodel_t* pVirtualModel = nullptr, const studiohdr_t* pStudioHdr = nullptr ) const
	{
		if ( include_models == 0 || pVirtualModel == nullptr || pStudioHdr == nullptr )
			return *get_local_sequence_description( sequence );

		return *pStudioHdr->get_local_sequence_description( pVirtualModel->sequence[ sequence ].index );
	}

	mutable int activity_list_version;
	mutable int events_indexed;

	int	textures;
	int	texture_index;
	inline void* get_texture( int index ) const
	{
		if ( index < 0 || index >= textures ) return nullptr;
		return ( ( std::uint8_t* )this + texture_index ) + index;
	}

	int cd_textures;
	int	cd_texture_index;
	inline char* get_cd_texture( int index ) const
	{
		if ( index < 0 || index >= cd_textures ) return nullptr;
		return ( ( ( char* )this ) + *( ( int* )( ( std::uint8_t* )this + cd_texture_index ) + index ) );
	}

	int	skin_refs;
	int	skin_families;
	int	skin_index;
	inline short* get_skin_ref( int index ) const
	{
		if ( index < 0 || index >= skin_refs ) return nullptr;
		return ( short* )( ( std::uint8_t* )this + skin_index ) + index;
	}

	int	body_parts;
	int	body_part_index;
	inline void* get_body_part( int index ) const
	{
		if ( index < 0 || index >= body_parts ) return nullptr;
		return ( ( std::uint8_t* )this + body_part_index ) + index;
	}

	int	attachments;
	int	attachment_index;
	inline void* get_attachment( int index ) const
	{
		if ( index < 0 || index >= attachments ) return nullptr;
		return ( ( std::uint8_t* )this + attachment_index ) + index;
	}

	int	transitions;
	int	transition_index;
	int	transition_name_index;
	inline std::uint8_t* get_transition( int index ) const
	{
		if ( index < 0 || index >= ( transitions * transitions ) ) return nullptr;
		return ( std::uint8_t* )( ( std::uint8_t* )this + transition_index ) + index;
	}

	int	flex_descs;
	int	flex_desc_index;
	inline void* get_flex_description( int index ) const
	{
		if ( index < 0 || index >= flex_descs ) return nullptr;
		return ( ( std::uint8_t* )this + flex_desc_index ) + index;
	}

	int flex_controllers;
	int	flex_controller_index;
	inline void* get_flex_controller( int index ) const
	{
		if ( index < 0 || index >= flex_controllers ) return nullptr;
		return ( ( std::uint8_t* )this + flex_controller_index ) + index;
	}

	int flex_rules;
	int	flex_rule_index;
	inline void* get_flex_rule( int index ) const
	{
		if ( index < 0 || index >= flex_rules ) return nullptr;
		return ( ( std::uint8_t* )this + flex_rule_index ) + index;
	}

	int	ik_chains;
	int	ik_chain_index;
	inline void* get_ik_chain( int index ) const
	{
		if ( index < 0 || index >= ik_chains ) return nullptr;
		return ( ( std::uint8_t* )this + ik_chain_index ) + index;
	}

	int	mouths;
	int mouth_index;
	inline void* get_mouth( int index ) const
	{
		if ( index < 0 || index >= mouths ) return nullptr;
		return ( ( std::uint8_t* )this + mouth_index ) + index;
	}

	int	pose_parameters;
	int	pose_parameter_index;
	inline void* get_pose_parameter( int index ) const
	{
		if ( index < 0 || index >= pose_parameters ) return nullptr;
		return ( ( std::uint8_t* )this + pose_parameter_index ) + index;
	}

	int	surface_prop_index;
	inline const char* get_surface_prop( ) const
	{
		if ( !surface_prop_index ) return nullptr;
		return ( char* )this + surface_prop_index;
	}

	int	key_value_index;
	int	key_value_size;
	inline const char* key_value_text( ) const
	{
		if ( !key_value_size ) return nullptr;
		return ( char* )this + key_value_index;
	}

	int	ik_autoplay_locks;
	int	ik_autoplay_lock_index;
	inline void* get_ik_autoplay_lock( int index ) const
	{
		if ( index < 0 || index >= ik_autoplay_locks ) return nullptr;
		return ( ( std::uint8_t* )this + ik_autoplay_lock_index ) + index;
	}

	float mass;
	int	contents;

	int	include_models;
	int	include_model_index;
	inline void* get_model_group( int index ) const
	{
		if ( index < 0 || index >= include_models ) return nullptr;
		return ( ( std::uint8_t* )this + include_model_index ) + index;
	}

	int	virtual_model;

	int	anim_block_name_index;
	inline const char* get_anim_block_name( ) const
	{
		return ( ( char* )this ) + anim_block_name_index;
	}

	int	anim_blocks;
	int	anim_block_index;
	inline void* get_anim_block( int index ) const
	{
		if ( index < 0 || index >= anim_blocks ) return nullptr;
		return ( ( ( std::uint8_t* )this ) + anim_block_index ) + index;
	}

	int	anim_block_model;

	int	bone_table_by_name_index;
	inline const std::uint8_t* get_bone_table_sorted_by_name( ) const
	{
		return ( std::uint8_t* )this + bone_table_by_name_index;
	}

	int vertex_base;
	int index_base;

	std::uint8_t directional_light_dot;
	std::uint8_t root_lod;
	std::uint8_t allowed_root_lo_ds;
	std::byte pad0[ 0x5 ];

	int	flex_controller_ui;
	int	flexcontroller_ui_index;

	float vert_anim_fixed_point_scale;
	mutable int surface_prop_lookup;
	int	studio_hdr2_index;

	std::byte pad1[ 0x4 ];
};
