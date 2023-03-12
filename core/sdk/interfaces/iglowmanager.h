#pragma once

#define end_of_free_list -1
#define entry_in_use -2

enum e_glow_render_style : int
{
	glow_render_style_default = 0,
	glow_render_style_rimglow3d,
	glow_render_style_edge_highlight,
	glow_render_style_edge_highlight_pulse,
	glow_render_style_count
};

class c_base_entity;
class i_glow_object_manager
{
public:
	struct glow_object_t
	{
		void set( const col_t& color, bool render_when_occluded = true,  bool full_bloom = false, float bloom_amount = 1.f, const int render_style = glow_render_style_default ) // @note: styles not used cuz other styles doesnt have ignorez flag and needed to rebuild glow
		{
			this->color = color.base_alpha( );
			this->bloom_amount = bloom_amount;
			this->render_when_occluded = render_when_occluded;
			this->render_when_unoccluded = !render_when_occluded;
			this->render_style = render_style;
			this->full_bloom_render = full_bloom;
		}

		inline bool is_empty( ) const
		{
			return next_free_slot != entry_in_use;
		}

		int						next_free_slot;					// 0x00
		c_base_entity* entity;						// 0x04
		std::array<float, 4U>	color;						// 0x08
		bool					alpha_capped_by_render_alpha;		// 0x18
	private:
		std::byte				pad0[ 0x3 ];						// 0x19 - pack 1 bool as 4 bytes
	public:
		float					alpha_function_of_max_velocity;	// 0x1C
		float					bloom_amount;					// 0x20
		float					pulse_overdrive;				// 0x24
		bool					render_when_occluded;			// 0x28
		bool					render_when_unoccluded;			// 0x29
		bool					full_bloom_render;				// 0x2A
	private:
		std::byte				pad1[ 0x1 ];						// 0x2B  - pack 3 bool as 4 bytes
	public:
		int						full_bloom_stencil_test_value;		// 0x2C
		int						render_style;					// 0x30
		int						split_screen_slot;				// 0x34
	}; // Size: 0x38

	struct glow_box_object_t
	{
		vec3_t	position;
		vec3_t	orientation;
		vec3_t	mins;
		vec3_t	maxs;
		float	birth_time_index;
		float	termination_time_index;
		col_t	color;
	};

	c_utl_vector<glow_object_t> glow_object_definitions;
	int first_free_slot;
	c_utl_vector<glow_box_object_t> glow_box_definitions;
};