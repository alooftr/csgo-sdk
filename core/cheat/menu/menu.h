#pragma once

#include "imgui_extra.h"

class c_menu
{
public:
	void run( );

	bool& is_opened( )
	{
		return this->open_state;
	}

	void set_state( bool state )
	{
		this->open_state = state;
	}

private:
	bool open_state = true;

	// tabs holder
	class c_tabs
	{
	public:
		const char* name = nullptr;
		std::function<void( )> fn = nullptr;
	};

	template <std::size_t s>
	void render_tabs( const char* tabs_id, std::array<c_tabs, s > tabs, int* current_tab );

	/* 2 int for lerping animation */
	int current_tab = 0;
};

inline const std::unique_ptr<c_menu> g_menu{ new c_menu( ) };

