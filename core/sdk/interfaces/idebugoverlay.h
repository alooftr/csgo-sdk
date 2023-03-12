#pragma once

class overlay_text_t;
class i_debug_overlay
{
public:
	virtual void			add_entity_text_overlay( int iEntityIndex, int line_offset, float duration, int r, int g, int b, int a, const char* fmt, ... ) = 0;
	virtual void			add_box_overlay( const vec3_t& origin, const vec3_t& abs_min, const vec3_t& abs_max, const vec3_t& orientation, int r, int g, int b, int a, float duration ) = 0;
	virtual void			add_sphere_overlay( const vec3_t& origin, float radius, int theta, int phi, int r, int g, int b, int a, float duration ) = 0;
	virtual void			add_triangle_overlay( const vec3_t& p1, const vec3_t& p2, const vec3_t& p3, int r, int g, int b, int a, bool no_depth_test, float duration ) = 0;
	virtual void			add_line_overlay( const vec3_t& origin, const vec3_t& dest, int r, int g, int b, bool no_depth_test, float duration ) = 0;
	virtual void			add_text_overlay( const vec3_t& origin, float duration, const char* fmt, ... ) = 0;
	virtual void			add_text_overlay( const vec3_t& origin, int line_offset, float duration, const char* fmt, ... ) = 0;
	virtual void			add_screen_text_overlay( float x, float y, float duration, int r, int g, int b, int a, const char* text ) = 0;
	virtual void			add_swept_box_overlay( const vec3_t& start, const vec3_t& end, const vec3_t& min, const vec3_t& max, const vec3_t& angles, int r, int g, int b, int a, float duration ) = 0;
	virtual void			add_grid_overlay( const vec3_t& origin ) = 0;
	virtual void			add_coord_frame_overlay( const matrix3x4_t& frame, float scale, int color_table[ 3 ][ 3 ] = nullptr ) = 0;
	virtual int				screen_position( const vec3_t& point, vec3_t& screen ) = 0;
	virtual int				screen_position( float x, float y, vec3_t& screen ) = 0;
	virtual overlay_text_t* get_first( ) = 0;
	virtual overlay_text_t* get_next( overlay_text_t* current ) = 0;
	virtual void			clear_dead_overlays( ) = 0;
	virtual void			clear_all_overlays( ) = 0;
	virtual void			add_text_overlay_rgb( const vec3_t& origin, int line_offset, float duration, float r, float g, float b, float a, const char* fmt, ... ) = 0;
	virtual void			add_text_overlay_rgb( const vec3_t& origin, int line_offset, float duration, int r, int g, int b, int a, const char* fmt, ... ) = 0;
	virtual void			add_line_overlay_alpha( const vec3_t& origin, const vec3_t& dest, int r, int g, int b, int a, bool no_depth_test, float duration ) = 0;
	virtual void			add_box_overlay2( const vec3_t& origin, const vec3_t& abs_min, const vec3_t& abs_max, const vec3_t& orientation, const col_t& face_color, const col_t& edge_color, float duration ) = 0;
	virtual void			add_line_overlay( const vec3_t& origin, const vec3_t& dest, int r, int g, int b, int a, float thickness, float duration ) = 0;
	virtual void			purge_text_overlays( ) = 0;
	virtual void			add_capsule_overlay( const vec3_t& abs_min, const vec3_t& abs_max, const float& radius, int r, int g, int b, int a, float duration ) = 0;
	virtual void			draw_pill( vec3_t& abs_min, vec3_t& abs_max, float radius, int r, int g, int b, int a, float duration ) = 0;
};
