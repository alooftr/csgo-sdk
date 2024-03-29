#pragma once
// used: std::format
#include <format>
// used: std::ofstream
#include <fstream>
// used: std::cout, std::endl
#include <iostream>

#include "../common.h"

#define foreground_white    (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define foreground_yellow	(FOREGROUND_RED | FOREGROUND_GREEN)
#define foreground_cyan        (FOREGROUND_BLUE | FOREGROUND_GREEN)
#define foreground_magenta        (FOREGROUND_RED | FOREGROUND_BLUE)
#define foreground_black    0

#define foreground_intense_red	(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define foreground_intense_green	(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define foreground_intense_blue	(FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define foreground_intense_white	(foreground_white | FOREGROUND_INTENSITY)
#define foreground_intense_yellow	(foreground_yellow | FOREGROUND_INTENSITY)
#define foreground_intense_cyan	(foreground_cyan | FOREGROUND_INTENSITY)
#define foreground_intense_magenta	(foreground_magenta | FOREGROUND_INTENSITY)

class c_logger
{
	FILE* stream = nullptr;
	std::uint16_t console_color = foreground_white;
public:
	bool attach_console( const char* title );
	void detach_console( );

	template <typename ... Args_t>
	void print( const std::string_view text, const Args_t& ... argList )
	{
#ifdef _DEBUG
		const std::string time = std::vformat( xor_str( "[{:%d-%m-%Y-%X}]->" ), std::make_format_args( std::chrono::system_clock::now( ) ) );

		// print to console
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), foreground_intense_green );
		std::cout << time;
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), console_color );

		if constexpr ( sizeof...( argList ) > 0 )
			std::cout << std::vformat( text, std::make_format_args( argList... ) ) << std::endl;
		else
			std::cout << text << std::endl;
#endif
	}

	inline void push_color( const std::uint16_t col_t )
	{
		console_color = col_t;
	}

	inline void pop_color( )
	{
		console_color = foreground_intense_white;
	}
};

inline const std::unique_ptr<c_logger> g_logger{ new c_logger( ) };

#define debug_log( log, ... ) g_logger->push_color( foreground_intense_white ); \
g_logger->print( xor_str( log ), __VA_ARGS__ ); \
g_logger->pop_color( );

#define debug_log_ok( log, ... ) g_logger->push_color( foreground_intense_cyan ); \
g_logger->print( xor_str( log ), __VA_ARGS__ ); \
g_logger->pop_color( );

#define debug_log_error( log, ...) g_logger->push_color( foreground_intense_red); \
g_logger->print( xor_str( log ), __VA_ARGS__ ); \
g_logger->pop_color( );