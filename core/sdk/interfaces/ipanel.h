#pragma once

class i_panel
{
public:
	void set_key_board_input_enabled( unsigned int panel_index, bool state )
	{
		return c_memory::call_vfunc<void>( this, 31, panel_index, state );
	}

	void set_mouse_input_enabled( unsigned int panel_index, bool state )
	{
		return c_memory::call_vfunc<void>( this, 32, panel_index, state );
	}

	const char* get_name( int panel_index )
	{
		return c_memory::call_vfunc<const char*>( this, 36, panel_index );
	}

	const char* get_name_class( int panel_index )
	{
		return c_memory::call_vfunc<const char*>( this, 37, panel_index );
	}
};