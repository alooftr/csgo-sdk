#pragma once

// used:std::filesystem
#include <filesystem>

#include "../common.h"

#include "../sdk/data_types/vector.h"

namespace g_utils
{
	std::filesystem::path get_game_path( );

	inline std::string random_string( const size_t length )
	{
		std::string r;
		static const char bet[ ] = { xor_str( "ABCDEFGHIJKLMNOPQRSTUVWXYabcdefghijklmnopqrstuvwxyzZ1234567890" ) };
		srand( ( unsigned )time( NULL ) * 5 );
		for ( size_t i = 0; i < length; ++i )
			r += bet[ rand( ) % ( sizeof( bet ) - 1 ) ];
		return r;
	}

	inline void flash_window( HWND hwnd )
	{
		FLASHWINFO fi;
		fi.cbSize = sizeof( FLASHWINFO );
		fi.hwnd = hwnd;
		fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
		fi.uCount = 0;
		fi.dwTimeout = 0;
		FlashWindowEx( &fi );
	}

	bool line_goes_through_smoke( const vec3_t& start, const vec3_t& end, const bool grenade_bloat = true );
}