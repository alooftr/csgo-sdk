#include <map>

#include "imgui_extra.h"

static constexpr std::array<const char*, 166U> key_names =
{
	"none",
	"mouse 1", "mouse 2", "cancel", "mouse 3", "mouse 4", "mouse 5", "",
	"backspace", "tab", "", "", "clear", "enter", "", "",
	"shift", "control", "alt", "pause", "caps", "", "", "", "", "", "",
	"escape", "", "", "", "", "space", "page up", "page down",
	"end", "home", "left", "up", "right", "down", "", "", "",
	"print", "insert", "delete", "",
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"", "", "", "", "", "", "",
	"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
	"l", "m", "n", "o", "p", "q", "r", "s", "t", "u",
	"v", "w", "x", "y", "z", "lwin", "rwin", "", "", "",
	"num0", "num1", "num2", "num3", "num4", "num5",
	"num6", "num7", "num8", "num9",
	"*", "+", "", "-", ".", "/",
	"f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8",
	"f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16",
	"f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24",
	"", "", "", "", "", "", "", "",
	"num lock", "scroll lock",
	"", "", "", "", "", "", "",
	"", "", "", "", "", "", "",
	"lshift", "rshift", "lctrl",
	"rctrl", "lmenu", "rmenu"
};

bool ImGui::MultiCombo( const char* label, std::vector<bool*>& vec_values, const std::string_view* items, int count )
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	if ( window->SkipItems )
		return false;

	const ImGuiStyle& style = g.Style;
	const ImVec2 label_size = CalcTextSize( label, nullptr, true );
	const float active_width = CalcItemWidth( ) - ( label_size.x > 0.0f ? style.ItemInnerSpacing.x + GetFrameHeight( ) : 0.0f );

	std::vector<std::string_view> active_items = { };

	// collect active items
	for ( int i = 0; i < count; i++ )
	{
		if ( *vec_values[ i ] == true )
			active_items.push_back( items[ i ] );
	}

	// fuck it, stl still haven't boost::join, fmt::join replacement
	std::string buffer = { };
	for ( std::size_t i = 0U; i < active_items.size( ); i++ )
	{
		buffer.append( active_items[ i ] );

		if ( i < active_items.size( ) - 1U )
			buffer.append( ", " );
	}

	if ( buffer.empty( ) )
		buffer.assign( "none" );
	else
	{
		const char* wrap_pos = g.Font->CalcWordWrapPositionA( GetCurrentWindow( )->FontWindowScale, &buffer[ 0 ], buffer.data( ) + buffer.length( ), active_width - style.FramePadding.x * 2.0f );
		const std::size_t wrap_length = wrap_pos - &buffer[ 0 ];

		if ( wrap_length > 0U && wrap_length < buffer.length( ) )
		{
			buffer.resize( wrap_length );
			buffer.append( "..." );
		}
	}

	bool value_changed = false;
	if ( BeginCombo( label, buffer.c_str( ) ) )
	{
		for ( int i = 0; i < count; i++ )
		{
			if ( Selectable( items[ i ].data( ), *vec_values[ i ], ImGuiSelectableFlags_DontClosePopups ) )
			{
				*vec_values[ i ] = !*vec_values[ i ];
				value_changed = true;
			}
		}

		EndCombo( );
	}

	return value_changed;
}

void ImGui::ToolTips( const char* tips )
{
	if ( IsItemHovered( ) )
	{
		BeginTooltip( );
		{
			TextUnformatted( tips );
		}
		EndTooltip( );
	}
}

bool ImGui::ListBox( const char* label, int* current_item, std::function<const char* ( int )> lambda, int items_count, int height_in_items )
{
	return ListBox( label, current_item, [ ]( void* data, int index, const char** out_text )
		{
			*out_text = ( *static_cast< std::function<const char* ( int )>* >( data ) )( index );
	return true;
		}, &lambda, items_count, height_in_items );
}

bool ImGui::KeyBind( const char* label, int* value )
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	if ( window->SkipItems )
		return false;

	ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID( label );

	const float w = CalcItemWidth( );
	const ImVec2 label_size = CalcTextSize( label, nullptr, true );

	const ImRect frame_bb( window->DC.CursorPos + ImVec2( label_size.x > 0.0f ? style.ItemInnerSpacing.x + GetFrameHeight( ) : 0.0f, 0.0f ), window->DC.CursorPos + ImVec2( w, label_size.x > 0.0f ? label_size.y + style.FramePadding.y : 0.f ) );
	const ImRect total_bb( frame_bb.Min, frame_bb.Max );

	ItemSize( total_bb, style.FramePadding.y );
	if ( !ItemAdd( total_bb, id, &frame_bb, ImGuiItemFlags_Inputable ) )
		return false;

	const bool hovered = ItemHoverable( frame_bb, id );
	if ( hovered )
	{
		SetHoveredID( id );
		g.MouseCursor = ImGuiMouseCursor_TextInput;
	}

	const bool clicked = hovered && io.MouseClicked[ 0 ];
	const bool double_clicked = hovered && g.IO.MouseDoubleClicked[ 0 ];
	if ( clicked || double_clicked )
	{
		if ( g.ActiveId != id )
		{
			memset( io.MouseDown, 0, sizeof( io.MouseDown ) );
			memset( io.KeysDown, 0, sizeof( io.KeysDown ) );
			*value = 0;
		}

		SetActiveID( id, window );
		FocusWindow( window );
	}

	bool value_changed = false;
	if ( int k = *value; g.ActiveId == id )
	{
		for ( int n = 0; n < IM_ARRAYSIZE( io.MouseDown ); n++ )
		{
			if ( IsMouseDown( n ) )
			{
				switch ( n )
				{
					case 0:
						k = VK_LBUTTON;
						break;
					case 1:
						k = VK_RBUTTON;
						break;
					case 2:
						k = VK_MBUTTON;
						break;
					case 3:
						k = VK_XBUTTON1;
						break;
					case 4:
						k = VK_XBUTTON2;
						break;
				}

				value_changed = true;
				ClearActiveID( );
			}
		}

		if ( !value_changed )
		{
			for ( int n = VK_BACK; n <= VK_RMENU; n++ )
			{
				if ( IsKeyDown( (ImGuiKey) n ) )
				{
					k = n;
					value_changed = true;
					ClearActiveID( );
				}
			}
		}

		if ( IsKeyPressed( ImGuiKey_Escape ) )
		{
			*value = 0;
			ClearActiveID( );
		}
		else
			*value = k;
	}

	char chBuffer[ 64 ] = { };
	sprintf_s( chBuffer, sizeof( chBuffer ), "[ %s ]", value != 0 && g.ActiveId != id ? key_names.at( *value ) : g.ActiveId == id ? "press" : "none" );

	PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( style.FramePadding.x, -1 ) );
	window->DrawList->AddText( ImVec2( frame_bb.Max.x - CalcTextSize( chBuffer ).x, total_bb.Min.y + style.FramePadding.y ), GetColorU32( g.ActiveId == id ? ImGuiCol_Text : ImGuiCol_TextDisabled ), chBuffer );

	PopStyleVar( );
	return value_changed;
}

bool ImGui::ColorPicker( const char* label, col_t* color, bool alpha )
{
	auto flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview;
	auto vec = color->get_vec4( );

	if ( ColorEdit4( label, &vec.x, flags | ( alpha ? ImGuiColorEditFlags_AlphaBar : ImGuiColorEditFlags_NoAlpha ) ) )
	{
		*color = col_t( vec.x, vec.y, vec.z, vec.w );

		return true;
	}

	return false;
}

bool ImGui::SelectableButton( const char* label, bool* selected, const ImVec2& size_arg, ImGuiButtonFlags flags )
{
	ImGuiWindow* window = GetCurrentWindow( );
	if ( window->SkipItems )
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID( label );
	const ImVec2 label_size = CalcTextSize( label, NULL, true );

	ImVec2 pos = window->DC.CursorPos;
	if ( ( flags & ImGuiButtonFlags_AlignTextBaseLine ) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset ) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize( size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f );

	const ImRect bb( pos, pos + size );
	ItemSize( size, style.FramePadding.y );
	if ( !ItemAdd( bb, id ) )
		return false;

	if ( g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat )
		flags |= ImGuiButtonFlags_Repeat;

	bool hovered, held;
	bool pressed = ButtonBehavior( bb, id, &hovered, &held, flags );

	if ( hovered || held )
		g.MouseCursor = ImGuiMouseCursor_Hand;

	// Animation
	static std::map<ImGuiID, ImColor> anim_map;
	auto anim = anim_map.find( ImColor( ) );

	if ( anim == anim_map.end( ) )
	{
		anim_map.insert( { ImGui::GetItemID( ), ImColor( ) } );
		anim = anim_map.find( ImGui::GetItemID( ) );
	}

	// Render
	const ImU32 col = GetColorU32( ( held && hovered ) ? ImGuiCol_ButtonActive : *selected ? ImGuiCol_ButtonHovered : ImGuiCol_ChildBg );
	anim->second = ImLerp( anim->second, ImColor( col ).Value, g.IO.DeltaTime * style.AnimationScale );
	RenderNavHighlight( bb, id );
	RenderFrame( bb.Min, bb.Max, anim->second, true, style.FrameRounding );

	if ( g.LogEnabled )
		LogSetNextTextDecoration( "[", "]" );
	RenderTextClipped( bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb );


	// Automatically close popups
	//if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
	//    CloseCurrentPopup();

	IMGUI_TEST_ENGINE_ITEM_INFO( id, label, g.LastItemData.StatusFlags );
	return pressed;
}

void ImGui::BeginChild( const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags, std::function<void( )> fn )
{ 
	ImGui::BeginChild( str_id, size_arg, border, extra_flags );
	{
		if ( extra_flags & ImGuiWindowFlags_MenuBar )
		{
			if ( ImGui::BeginMenuBar( ) )
			{
				ImGui::TextUnformatted( str_id );
				ImGui::EndMenuBar( );
			}
		}

		ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( ImGui::GetStyle( ).FramePadding.x, -1 ) );
		fn( );
		ImGui::PopStyleVar( );
	}
	ImGui::EndChild( );
}
