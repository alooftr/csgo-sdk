#pragma once
// needed header for thread safe drawing and rendering
#include <vector>
#include <deque>
#include <string>
#include <any>
#include <shared_mutex>
#include <directxmath.h>

// used: imgui library
#include "../common.h"
// used: color struct
#include "../sdk/data_types/color.h"

// types of thread-safe render objects
enum class e_draw_type : int
{
	none = 0,
	line,
	rect,
	rect_multicolor,
	circle,
	triangle,
	polygon,
	text,
	image,
	shadow_rect,
	shadow_circle,
	shadow_polygon
};


namespace imgui_fonts
{
	// menu main 18
	inline ImFont* whitney;
	// esp flags 12
	inline ImFont* smallest_pixel;
	// esp weapon icons 12
	inline ImFont* weapon_icons;
	// esp flags 20
	inline ImFont* verdana;
	// esp flags 12
	inline ImFont* verdana_small;
}

namespace imgui_images
{
	inline IDirect3DTexture9* menu_bg;
}

struct draw_object_t
{
	draw_object_t( const e_draw_type type, std::any&& object ) :
		type( type ), object( std::move( object ) )
	{ }

	e_draw_type type = e_draw_type::none;
	std::any object = { };
};

#pragma region draw_objects_enumerations
enum e_rect_render_flags : unsigned int
{
	draw_rect_none = 0,
	draw_rect_outline = ( 1 << 0 ),
	draw_rect_border = ( 1 << 1 ),
	draw_rect_filled = ( 1 << 2 )
};

enum e_circle_render_flags : unsigned int
{
	draw_circle_none = 0,
	draw_circle_outline = ( 1 << 0 ),
	draw_circle_filled = ( 1 << 1 )
};

enum e_triangle_render_flags : unsigned int
{
	draw_triangle_none = 0,
	draw_triangle_outline = ( 1 << 0 ),
	draw_triangle_filled = ( 1 << 1 )
};

enum e_polygon_render_flags : unsigned int
{
	draw_polygon_none = 0,
	draw_polygon_outline = ( 1 << 0 ),
	draw_polygon_filled = ( 1 << 1 )
};

enum e_text_render_flags : unsigned int
{
	draw_text_none = 0,
	draw_text_dropshadow = ( 1 << 0 ),
	draw_text_outline = ( 1 << 1 )
};
#pragma endregion

#pragma region draw_objects_structures
struct line_object_t
{
	ImVec2 start = { };
	ImVec2 end = { };
	ImU32 clr = 0x0;
	float thickness = 0.f;
};

struct rect_object_t
{
	ImVec2 pos_min = { };
	ImVec2 pos_max = { };
	ImU32 clr = 0x0;
	unsigned int flags = 0x0;
	ImU32 clr_outline = 0x0;
	float rounding = 0.f;
	ImDrawFlags rounding_corners = ImDrawCornerFlags_None;
	float thickness = 0.f;
};

struct rect_multi_color_object_t
{
	ImVec2 pos_min = { };
	ImVec2 pos_max = { };
	ImU32 clr1 = 0x0;
	ImU32 clr2 = 0x0;
	bool horizontal = false;
	float ronuding = 0.f;
	ImDrawFlags rounding_corners = ImDrawCornerFlags_None;
};

struct circle_object_t
{
	ImVec2 center = { };
	float radius = 0.f;
	ImU32 clr = 0x0;
	int segments = 0;
	unsigned int flags = 0x0;
	ImU32 clr_outline = 0x0;
	float thickness = 0.f;
};

struct triangle_object_t
{
	ImVec2 first = { };
	ImVec2 second = { };
	ImVec2 third = { };
	ImU32 clr = 0x0;
	unsigned int flags = 0x0;
	ImU32 clr_outline = 0x0;
	float thickness = 0.f;
};

struct polygon_object_t
{
	std::vector<ImVec2> points = { };
	ImU32 clr = 0x0;
	unsigned int flags = 0x0;
	ImU32 clr_outline = 0x0;
	bool closed = false;
	float thickness = 0.f;
};

struct text_object_t
{
	const ImFont* font = nullptr;
	float font_size = 0.f;
	ImVec2 position = { };
	std::string text = { };
	ImU32 clr = 0x0;
	unsigned int flags = 0x0;
	ImU32 clr_outline = 0x0;
};

struct image_object_t
{
	ImTextureID texture = nullptr;
	ImVec2 pos_min = { };
	ImVec2 pos_max = { };
	ImU32 clr = 0x0;
	float rounding = 0.f;
	ImDrawFlags rounding_corners = ImDrawCornerFlags_None;
};

struct shadow_rect_object_t
{
	ImVec2 pos_min = { };
	ImVec2 pos_max = { };
	ImU32 clr = 0x0;
	float thickness = 0.0f;
	float rounding = 0.f;
	ImDrawFlags rounding_corners = ImDrawCornerFlags_None;
};

struct shadow_circle_object_t
{
	ImVec2 center = { };
	float radius = 0.f;
	ImU32 clr = 0x0;
	float thickness = 0.0f;
	int num_segments = 0;
};

struct shadow_polygon_object_t
{
	std::vector<ImVec2> points = { };
	ImU32 clr = 0x0;
	float thickness = 0.f;
};
#pragma endregion

class c_safe_draw
{
public:
	// setup fonts, style...
	void setup( );
	void destroy( );

	bool did_setup = false;

	// render the deque queue
	void render_draw_data( ImDrawList* draw_list );

	// thread safe drawing for using on paint_tranverse hook
	void clear_draw_data( );
	void swap_draw_data( );

	// main render
	void add_line( const ImVec2& start, const ImVec2& end, const col_t& clr, float thickness = 1.0f );
	void add_rect( const ImVec2& pos_min, const ImVec2& pos_max, const col_t& clr, unsigned int flags = draw_rect_none, const col_t& clr_outline = col_t( 0, 0, 0, 255 ), float rounding = 0.f, ImDrawFlags rounding_corners = ImDrawCornerFlags_None, float thickness = 1.0f );
	void add_rect_multi_color( const ImVec2& pos_min, const ImVec2& pos_max, const col_t& clr1, const col_t& clr2, bool horizontal = false, float rounding = 0.0f, ImDrawFlags rounding_corners = ImDrawCornerFlags_None );
	void add_circle( const ImVec2& center, float radius, const col_t& clr, int segments = 12, unsigned int flags = draw_circle_none, const col_t& clr_outline = col_t( 0, 0, 0, 255 ), float thickness = 1.0f );
	void add_triangle( const ImVec2& first, const ImVec2& second, const ImVec2& third, const col_t& clr, unsigned int flags = draw_triangle_none, const col_t& clr_outline = col_t( 0, 0, 0, 255 ), float thickness = 1.0f );
	void add_polygon( std::vector<ImVec2>& points, const col_t& clr, unsigned int flags = draw_polygon_filled, const col_t& clr_outline = col_t( 0, 0, 0, 255 ), bool closed = true, float thickness = 1.0f );
	void add_text( const ImFont* font, const ImVec2& pos_min, const std::string& str, const col_t& clr, unsigned int flags = draw_text_none, const col_t& clr_outline = col_t( 0, 0, 0, 255 ) );
	void add_image( ImTextureID texture, const ImVec2& pos_min, const ImVec2& pos_max, const col_t& clr = col_t( 255, 255, 255, 255 ), float rounding = 0.f, ImDrawFlags rounding_corners = ImDrawCornerFlags_None );

	void add_shadow_circle( const ImVec2& center, float radius, const col_t& clr, float thickness = 24.0f, int num_segments = 12 );
	void add_shadow_rect( const ImVec2& pos_min, const ImVec2 pos_max, const col_t& clr, float thickness = 24.0f, float rounding = 0.0f, ImDrawFlags rounding_corners = ImDrawCornerFlags_None );
	void add_shadow_polygon( std::vector<ImVec2>& points, const col_t& clr, float thickness = 24.0f );

	// bindings
	void add_draw_list_rect( ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, ImU32 clr, unsigned int flags = draw_rect_none, ImU32 clr_outline = IM_COL32( 0, 0, 0, 255 ), float rounding = 0.f, ImDrawFlags roundingCorners = ImDrawCornerFlags_None, float flThickness = 1.0f );
	void add_draw_list_text( ImDrawList* draw_list, const ImFont* font, const ImVec2& pos, const std::string& str, ImU32 clr, unsigned int flags = draw_text_none, ImU32 clr_outline = IM_COL32( 0, 0, 0, 255 ) );

	// Extras
	inline ImVec2 get_screen_size( )
	{
		return ImGui::GetIO( ).DisplaySize;
	};
	inline ImVec2 get_text_size( ImFont* font, const char* text )
	{
		return font->CalcTextSizeA( font->FontSize, FLT_MAX, 0.0f, text );
	};

private:
	/* saved data to draw */
	std::deque<draw_object_t> draw_data = { };
	/* thread-safe data to draw */
	std::deque<draw_object_t> safe_draw_data = { };
	/* thread-safe render mutex */
	std::shared_mutex draw_mutex = { };
};

inline const std::unique_ptr<c_safe_draw> g_safe_draw{ new c_safe_draw( ) };