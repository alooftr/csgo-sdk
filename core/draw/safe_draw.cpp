#include "safe_draw.h"

//used: window handle
#include "../sdk/csgo.h"

// including memory font
#include "../../resources/whitney.h"
#include "../../resources/smallest_pixel.h"
#include "../../resources/weapon_icons.h"

void c_safe_draw::setup( IDirect3DDevice9* device )
{
	ImGui::CreateContext( );
	ImGui_ImplWin32_Init( g_game_interfaces->window_handle );
	ImGui_ImplDX9_Init( device );

	// color style
	auto& style = ImGui::GetStyle( );

	auto colors = style.Colors;
#pragma region imgui_colors
	colors[ ImGuiCol_Text ] = ImVec4( 0.90f, 0.90f, 0.90f, 1.00f );
	colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.60f, 0.60f, 0.60f, 1.00f );
	colors[ ImGuiCol_WindowBg ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.85f );
	colors[ ImGuiCol_ChildBg ] = ImVec4( 0.11f, 0.11f, 0.14f, 0.92f );
	colors[ ImGuiCol_PopupBg ] = ImVec4( 0.11f, 0.11f, 0.14f, 0.92f );
	colors[ ImGuiCol_Border ] = ImVec4( 0.50f, 0.50f, 0.50f, 0.50f );
	colors[ ImGuiCol_BorderShadow ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
	colors[ ImGuiCol_FrameBg ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.44f );
	colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.47f, 0.47f, 0.69f, 0.40f );
	colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.42f, 0.41f, 0.64f, 0.69f );
	colors[ ImGuiCol_TitleBg ] = ImVec4( 0.27f, 0.27f, 0.54f, 0.83f );
	colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.32f, 0.32f, 0.63f, 0.87f );
	colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.40f, 0.40f, 0.80f, 0.20f );
	colors[ ImGuiCol_MenuBarBg ] = ImVec4( 0.40f, 0.40f, 0.55f, 0.80f );
	colors[ ImGuiCol_ScrollbarBg ] = ImVec4( 0.20f, 0.25f, 0.30f, 0.60f );
	colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( 0.40f, 0.40f, 0.80f, 0.30f );
	colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 0.40f, 0.40f, 0.80f, 0.40f );
	colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( 0.41f, 0.39f, 0.80f, 0.60f );
	colors[ ImGuiCol_CheckMark ] = ImVec4( 0.90f, 0.90f, 0.90f, 0.50f );
	colors[ ImGuiCol_SliderGrab ] = ImVec4( 0.40f, 0.40f, 0.55f, 0.80f );
	colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 0.41f, 0.39f, 0.80f, 0.60f );
	colors[ ImGuiCol_Button ] = ImVec4( 0.35f, 0.40f, 0.61f, 0.62f );
	colors[ ImGuiCol_ButtonHovered ] = ImVec4( 0.40f, 0.48f, 0.71f, 0.79f );
	colors[ ImGuiCol_ButtonActive ] = ImVec4( 0.46f, 0.54f, 0.80f, 1.00f );
	colors[ ImGuiCol_Header ] = ImVec4( 0.35f, 0.40f, 0.61f, 0.62f );
	colors[ ImGuiCol_HeaderHovered ] = ImVec4( 0.40f, 0.48f, 0.71f, 0.79f );
	colors[ ImGuiCol_HeaderActive ] = ImVec4( 0.46f, 0.54f, 0.80f, 1.00f );
	colors[ ImGuiCol_Separator ] = ImVec4( 0.50f, 0.50f, 0.50f, 0.60f );
	colors[ ImGuiCol_SeparatorHovered ] = ImVec4( 0.60f, 0.60f, 0.70f, 1.00f );
	colors[ ImGuiCol_SeparatorActive ] = ImVec4( 0.70f, 0.70f, 0.90f, 1.00f );
	colors[ ImGuiCol_ResizeGrip ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.10f );
	colors[ ImGuiCol_ResizeGripHovered ] = ImVec4( 0.78f, 0.82f, 1.00f, 0.60f );
	colors[ ImGuiCol_ResizeGripActive ] = ImVec4( 0.78f, 0.82f, 1.00f, 0.90f );
	colors[ ImGuiCol_Tab ] = ImVec4( 0.34f, 0.34f, 0.68f, 0.79f );
	colors[ ImGuiCol_TabHovered ] = ImVec4( 0.45f, 0.45f, 0.90f, 0.80f );
	colors[ ImGuiCol_TabActive ] = ImVec4( 0.40f, 0.40f, 0.73f, 0.84f );
	colors[ ImGuiCol_TabUnfocused ] = ImVec4( 0.28f, 0.28f, 0.57f, 0.82f );
	colors[ ImGuiCol_TabUnfocusedActive ] = ImVec4( 0.35f, 0.35f, 0.65f, 0.84f );
	colors[ ImGuiCol_PlotLines ] = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
	colors[ ImGuiCol_PlotLinesHovered ] = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
	colors[ ImGuiCol_PlotHistogram ] = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
	colors[ ImGuiCol_PlotHistogramHovered ] = ImVec4( 1.00f, 0.60f, 0.00f, 1.00f );
	colors[ ImGuiCol_TableHeaderBg ] = ImVec4( 0.27f, 0.27f, 0.38f, 1.00f );
	colors[ ImGuiCol_TableBorderStrong ] = ImVec4( 0.31f, 0.31f, 0.45f, 1.00f );
	colors[ ImGuiCol_TableBorderLight ] = ImVec4( 0.26f, 0.26f, 0.28f, 1.00f );
	colors[ ImGuiCol_TableRowBg ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
	colors[ ImGuiCol_TableRowBgAlt ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.07f );
	colors[ ImGuiCol_TextSelectedBg ] = ImVec4( 0.00f, 0.00f, 1.00f, 0.35f );
	colors[ ImGuiCol_DragDropTarget ] = ImVec4( 1.00f, 1.00f, 0.00f, 0.90f );
	colors[ ImGuiCol_NavHighlight ] = ImVec4( 0.45f, 0.45f, 0.90f, 0.80f );
	colors[ ImGuiCol_NavWindowingHighlight ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.70f );
	colors[ ImGuiCol_NavWindowingDimBg ] = ImVec4( 0.80f, 0.80f, 0.80f, 0.20f );
	colors[ ImGuiCol_ModalWindowDimBg ] = ImVec4( 0.20f, 0.20f, 0.20f, 0.35f );
	colors[ ImGuiCol_WindowShadow ] = ImVec4( 0.08f, 0.08f, 0.08f, 0.35f );
#pragma endregion

#pragma region imgui_style
	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2( 8, 8 );
	style.WindowRounding = 8.f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2( 32, 32 );
	style.WindowTitleAlign = ImVec2( 0.5f, 0.5f );
	style.ChildRounding = 4.f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 8.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2( 4, 2 );
	style.FrameRounding = 2.f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2( 8, 4 );
	style.ItemInnerSpacing = ImVec2( 4, 4 );
	style.IndentSpacing = 6.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 6.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 0.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 8.0f;
	style.TabBorderSize = 1.0f;
	style.ButtonTextAlign = ImVec2( 0.5f, 0.5f );
	style.SelectableTextAlign = ImVec2( 0.5f, 0.5f );
	style.MouseCursorScale = 0.85f;
	style.AntiAliasedFill = true;
	style.AntiAliasedLines = true;
	style.AntiAliasedLinesUseTex = true;
#pragma endregion

	auto& io = ImGui::GetIO( );

	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	ImFontConfig arial_config;
	arial_config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;
	arial_config.MergeMode = true;

	ImFontConfig whitney_config;
	whitney_config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting;
	ImFormatString( whitney_config.Name, IM_ARRAYSIZE( whitney_config.Name ), "Whitney" );
	imgui_fonts::whitney = io.Fonts->AddFontFromMemoryCompressedTTF( whitney_compressed_data, whitney_compressed_size, 16.f, &whitney_config, io.Fonts->GetGlyphRangesCyrillic( ) );
	// this will merge with whitney font
	io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\ArialUni.ttf", 16.f, &arial_config, io.Fonts->GetGlyphRangesChineseFull( ) );

	ImFontConfig verdana_config;
	verdana_config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;
	imgui_fonts::verdana = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Verdana.ttf", 20.f, &verdana_config, io.Fonts->GetGlyphRangesDefault( ) );
	// this will merge with verdena font
	io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\ArialUni.ttf", 20.f, &arial_config, io.Fonts->GetGlyphRangesChineseFull( ) );
	imgui_fonts::verdana_small = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Verdana.ttf", 12.f, &verdana_config, io.Fonts->GetGlyphRangesDefault( ) );
	// this will merge with verdena font
	io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\ArialUni.ttf", 12.f, &arial_config, io.Fonts->GetGlyphRangesChineseFull( ) );

	ImFontConfig smallest_pixel_config;
	smallest_pixel_config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting;
	ImFormatString( smallest_pixel_config.Name, IM_ARRAYSIZE( smallest_pixel_config.Name ), "smallest-pixel" );
	imgui_fonts::smallest_pixel = io.Fonts->AddFontFromMemoryCompressedTTF( smallest_pixel_compressed_data, smallest_pixel_compressed_size, 10.f, &smallest_pixel_config, io.Fonts->GetGlyphRangesCyrillic( ) );

	ImFontConfig weapon_icons_config;
	weapon_icons_config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_LightHinting;
	constexpr ImWchar icon_ranges[ ] =
	{
		0xE000, 0xF8FF, // Private Use Area
		0
	};
	ImFormatString( weapon_icons_config.Name, IM_ARRAYSIZE( weapon_icons_config.Name ), "weapon-icons" );
	imgui_fonts::weapon_icons = io.Fonts->AddFontFromMemoryCompressedTTF( weapon_icons_compressed_data, weapon_icons_compressed_size, 14.f, &weapon_icons_config, icon_ranges );

	did_setup = ImGuiFreeType::BuildFontAtlas( io.Fonts );
}

void c_safe_draw::render_draw_data( ImDrawList* draw_list )
{
	std::unique_lock<std::shared_mutex> lock( draw_mutex );

	if ( safe_draw_data.empty( ) )
		return;

	for ( const auto& data : safe_draw_data )
	{
		if ( !data.object.has_value( ) )
			continue;

		switch ( data.type )
		{
			case e_draw_type::line:
			{
				const auto& object = std::any_cast< line_object_t >( data.object );
				draw_list->AddLine( object.start, object.end, object.clr, object.thickness );
				break;
			}
			case e_draw_type::rect:
			{
				const auto& object = std::any_cast< rect_object_t >( data.object );
				if ( ( object.pos_min.x == object.pos_max.x && object.pos_min.y == object.pos_max.y ) || object.pos_min.x == NULL || object.pos_min.y == NULL )
					break;

				add_draw_list_rect( draw_list, object.pos_min, object.pos_max, object.clr, object.flags, object.clr_outline, object.rounding, object.rounding_corners, object.thickness );
				break;
			}
			case e_draw_type::rect_multicolor:
			{
				const auto& object = std::any_cast< rect_multi_color_object_t >( data.object );
				draw_list->AddRectFilledMultiColorRounded( object.pos_min, object.pos_max, object.clr1, object.horizontal ? object.clr1 : object.clr2, object.clr2, object.horizontal ? object.clr2 : object.clr1, object.ronuding, object.rounding_corners );
				break;
			}
			case e_draw_type::circle:
			{
				const auto& object = std::any_cast< circle_object_t >( data.object );

				if ( object.flags & draw_circle_filled )
					draw_list->AddCircleFilled( object.center, object.radius, object.clr, object.segments );
				else
					draw_list->AddCircle( object.center, object.radius, object.clr, object.segments, object.thickness );

				if ( object.flags & draw_circle_outline )
					draw_list->AddCircle( object.center, object.radius + 1.0f, object.clr_outline, object.segments, object.thickness + 1.0f );

				break;
			}
			case e_draw_type::triangle:
			{
				const auto& object = std::any_cast< triangle_object_t >( data.object );

				if ( object.flags & draw_triangle_filled )
					draw_list->AddTriangleFilled( object.first, object.second, object.third, object.clr );
				else
					draw_list->AddTriangle( object.first, object.second, object.third, object.clr, object.thickness );

				if ( object.flags & draw_triangle_outline )
					draw_list->AddTriangle( object.first, object.second, object.third, object.clr_outline, object.thickness + 1.0f );

				break;
			}
			case e_draw_type::polygon:
			{
				const auto& object = std::any_cast< polygon_object_t >( data.object );

				if ( object.flags & draw_polygon_filled )
					draw_list->AddConvexPolyFilled( object.points.data( ), object.points.size( ), object.clr );
				else
					draw_list->AddPolyline( object.points.data( ), object.points.size( ), object.clr, object.closed, object.thickness );

				if ( object.flags & draw_polygon_outline )
					draw_list->AddPolyline( object.points.data( ), object.points.size( ), object.clr_outline, object.closed, object.thickness + 1.0f );

				break;
			}
			case e_draw_type::text:
			{
				const auto& object = std::any_cast< text_object_t >( data.object );
				add_draw_list_text( draw_list, object.font, object.position, object.text, object.clr, object.flags, object.clr_outline );
				break;
			}
			case e_draw_type::image:
			{
				const auto& object = std::any_cast< image_object_t >( data.object );
				draw_list->AddImageRounded( object.texture, object.pos_min, object.pos_max, ImVec2( 0, 0 ), ImVec2( 1, 1 ), object.clr, object.rounding, object.rounding_corners );
				break;
			}
			case e_draw_type::shadow_rect:
			{
				const auto& object = std::any_cast< shadow_rect_object_t >( data.object );
				draw_list->AddShadowRect( object.pos_min, object.pos_max, object.clr, object.thickness, ImVec2( 0, 0 ), object.rounding_corners, object.rounding );
				break;
			}
			case e_draw_type::shadow_circle:
			{
				const auto& object = std::any_cast< shadow_circle_object_t >( data.object );
				draw_list->AddShadowCircle( object.center, object.radius, object.clr, object.thickness, ImVec2( 0, 0 ), ImDrawListFlags_AntiAliasedFill, object.num_segments );
				break;
			}
			case e_draw_type::shadow_polygon:
			{
				const auto& object = std::any_cast< shadow_polygon_object_t >( data.object );
				draw_list->AddShadowConvexPoly( object.points.data( ), object.points.size( ), object.clr, object.thickness, ImVec2( 0, 0 ), ImDrawListFlags_AntiAliasedFill );
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

void c_safe_draw::clear_draw_data( )
{
	if ( !draw_data.empty( ) )
		draw_data.clear( );
}

void c_safe_draw::swap_draw_data( )
{
	std::unique_lock<std::shared_mutex> lock( draw_mutex );
	draw_data.swap( safe_draw_data );
}

void c_safe_draw::add_line( const ImVec2& start, const ImVec2& end, const col_t& clr, float thickness )
{
	draw_data.emplace_back( e_draw_type::line, std::make_any<line_object_t>( start, end, clr.get_u32( ), thickness ) );
}

void c_safe_draw::add_rect( const ImVec2& pos_min, const ImVec2& pos_max, const col_t& clr, unsigned int flags, const col_t& clr_outline, float rounding, ImDrawCornerFlags rounding_corners, float thickness )
{
	draw_data.emplace_back( e_draw_type::rect, std::make_any<rect_object_t>( pos_min, pos_max, clr.get_u32( ), flags, clr_outline.set<col_a>( clr.get<col_a>( ) ).get_u32( ), rounding, rounding_corners, thickness ) );
}

void c_safe_draw::add_rect_multi_color( const ImVec2& pos_min, const ImVec2& pos_max, const col_t& clr1, const col_t& clr2, bool horizontal, float rounding, ImDrawFlags rounding_corners )
{
	draw_data.emplace_back( e_draw_type::rect_multicolor, std::make_any<rect_multi_color_object_t>( pos_min, pos_max, clr1.get_u32( ), clr2.get_u32( ), horizontal, rounding, rounding_corners ) );
}

void c_safe_draw::add_circle( const ImVec2& center, float radius, const col_t& clr, int segments, unsigned int flags, const col_t& clr_outline, float thickness )
{
	draw_data.emplace_back( e_draw_type::circle, std::make_any<circle_object_t>( center, radius, clr.get_u32( ), segments, flags, clr_outline.set<col_a>( clr.get<col_a>( ) ).get_u32( ), thickness ) );
}

void c_safe_draw::add_triangle( const ImVec2& first, const ImVec2& second, const ImVec2& third, const col_t& clr, unsigned int flags, const col_t& clr_outline, float thickness )
{
	draw_data.emplace_back( e_draw_type::triangle, std::make_any<triangle_object_t>( first, second, third, clr.get_u32( ), flags, clr_outline.set<col_a>( clr.get<col_a>( ) ).get_u32( ), thickness ) );
}

void c_safe_draw::add_polygon( std::vector<ImVec2>& points, const col_t& clr, unsigned int flags, const col_t& clr_outline, bool closed, float thickness )
{
	draw_data.emplace_back( e_draw_type::polygon, std::make_any<polygon_object_t>( std::move( points ), clr.get_u32( ), flags, clr_outline.set<col_a>( clr.get<col_a>( ) ).get_u32( ), closed, thickness ) );
}

void c_safe_draw::add_text( const ImFont* font, const ImVec2& pos, const std::string& str, const col_t& clr, unsigned int flags, const col_t& clr_outline )
{
	if ( font->ContainerAtlas == nullptr )
		return;

	// check is only one flag selected
	IM_ASSERT( ImIsPowerOfTwo( flags == draw_text_none || flags & ( draw_text_dropshadow | draw_text_outline ) ) );

	draw_data.emplace_back( e_draw_type::text, std::make_any<text_object_t>( font, font->FontSize, pos, str, clr.get_u32( ), flags, clr_outline.set<col_a>( clr.get<col_a>( ) ).get_u32( ) ) );
}

void c_safe_draw::add_image( ImTextureID texture, const ImVec2& pos_min, const ImVec2& pos_max, const col_t& clr, float rounding, ImDrawFlags rounding_corners )
{
	draw_data.emplace_back( e_draw_type::image, std::make_any<image_object_t>( texture, pos_min, pos_max, clr.get_u32( ), rounding, rounding_corners ) );
}

void c_safe_draw::add_shadow_circle( const ImVec2& center, float radius, const col_t& clr, float thickness, int num_segments )
{
	draw_data.emplace_back( e_draw_type::shadow_circle, std::make_any<shadow_circle_object_t>( center, radius, clr.get_u32( ), thickness, num_segments ) );
}

void c_safe_draw::add_shadow_rect( const ImVec2& pos_min, const ImVec2 pos_max, const col_t& clr, float thickness, float rounding, ImDrawFlags rounding_corners )
{
	draw_data.emplace_back( e_draw_type::shadow_rect, std::make_any<shadow_rect_object_t>( pos_min, pos_max, clr.get_u32( ), thickness, rounding, rounding_corners ) );
}

void c_safe_draw::add_shadow_polygon( std::vector<ImVec2>& points, const col_t& clr, float thickness )
{
	draw_data.emplace_back( e_draw_type::shadow_polygon, std::make_any<shadow_polygon_object_t>( points, clr.get_u32( ), thickness ) );
}

void c_safe_draw::add_draw_list_rect( ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, ImU32 clr, unsigned int flags, ImU32 clr_outline, float rounding, ImDrawCornerFlags rounding_corners, float thickness )
{
	if ( flags & draw_rect_border )
		draw_list->AddRect( ImVec2( pos_min.x + 1.0f, pos_min.y + 1.0f ), ImVec2( pos_max.x - 1.0f, pos_max.y - 1.0f ), clr_outline, rounding, rounding > 0.0f ? rounding_corners : ImDrawCornerFlags_None, thickness );

	if ( flags & draw_rect_outline )
		draw_list->AddRect( ImVec2( pos_min.x - 1.0f, pos_min.y - 1.0f ), ImVec2( pos_max.x + 1.0f, pos_max.y + 1.0f ), clr_outline, rounding, rounding > 0.0f ? rounding_corners : ImDrawCornerFlags_None, thickness );

	if ( flags & draw_rect_filled )
		draw_list->AddRectFilled( pos_min, pos_max, clr, rounding, rounding > 0.0f ? rounding_corners : ImDrawCornerFlags_None );
	else
		draw_list->AddRect( pos_min, pos_max, clr, rounding, rounding > 0.0f ? rounding_corners : ImDrawCornerFlags_None, thickness );
}

void c_safe_draw::add_draw_list_text( ImDrawList* draw_list, const ImFont* font, const ImVec2& pos, const std::string& str, ImU32 clr, unsigned int flags, ImU32 clr_outline )
{
	// set font texture
	if ( font == nullptr || font->ContainerAtlas == nullptr )
		font = ImGui::GetDefaultFont( );

	draw_list->PushTextureID( font->ContainerAtlas->TexID );

	if ( flags & draw_text_dropshadow )
		draw_list->AddText( font, font->FontSize, ImVec2( pos.x - 1.0f, pos.y + 1.0f ), clr_outline, str.c_str( ) );
	else if ( flags & draw_text_outline )
	{
		draw_list->AddText( font, font->FontSize, ImVec2( pos.x + 1.0f, pos.y - 1.0f ), clr_outline, str.c_str( ) );
		draw_list->AddText( font, font->FontSize, ImVec2( pos.x - 1.0f, pos.y + 1.0f ), clr_outline, str.c_str( ) );
	}

	draw_list->AddText( font, font->FontSize, pos, clr, str.data( ) );
	draw_list->PopTextureID( );
}