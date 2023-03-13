#pragma once

#include "../../common.h"

// used: std::function
#include <functional>
// used: col_t
#include "../../sdk/data_types/color.h"

namespace ImGui
{
    bool MultiCombo( const char* label, std::vector<bool*>& vec_values, const std::string_view* items, int count );
    void ToolTips( const char* tips );
    bool ListBox( const char* label, int* current_item, std::function<const char* ( int )> lambda, int items_count, int height_in_items );
    bool KeyBind( const char* label, int* value );
    bool ColorPicker( const char* label, col_t* color, bool alpha );
    bool SelectableButton( const char* label, bool* selected, const ImVec2& size_arg = ImVec2( 0, 0 ), ImGuiButtonFlags flags = 0 );
    void BeginChild( const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags, std::function<void( )> fn ); 
}
