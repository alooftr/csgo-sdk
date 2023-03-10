#include "logging.h"

bool logging::attach_console( const char* title )
{
	if ( !AllocConsole( ) )
		return false;

	if ( AttachConsole( ATTACH_PARENT_PROCESS ) )
		return false;

	if ( freopen_s( &stream, "CONOUT$", "w", stdout ) != 0 )
		return false;

	// set console window title
	if ( !SetConsoleTitle( title ) )
		return false;

	return true;
}

void logging::detach_console( )
{
	fclose( stream );

	FreeConsole( );

	if ( const auto hConsoleWnd = GetConsoleWindow( ); hConsoleWnd != nullptr )
		PostMessageW( hConsoleWnd, WM_CLOSE, 0U, 0L );
}
