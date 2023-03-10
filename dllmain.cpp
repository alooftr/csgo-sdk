// used: sleep_for
#include <thread>
// used: logging namespace
#include "core/utilities/logging.h"
// used: g_mem pointer for get module
#include "core/utilities/memory.h"
// used: g_modules && g_interfaces
#include "core/sdk/csgo.h"
// used: netvar
#include "core/sdk/netvar/netvar.h"

DWORD WINAPI on_attach( LPVOID parameter )
{
    try
    {
		// GetModuleHandle to prevent the dll loaded too soon ( pe header no wroky )
		while ( GetModuleHandle( serverbrowser_dll ) == nullptr )
			std::this_thread::sleep_for( 200ms );

#ifdef _DEBUG
        if ( !logging::attach_console( xor_str( "csgo-sdk dev's console" ) ) )
            throw std::runtime_error( xor_str( "failed to attach console" ) );
#endif // _DEBUG

		debug_log( "init csgo-sdk..." );
		// timer for injection time
		time_t start, end;
		time( &start );
		{
			if ( !g_game_modules->setup( ) )
				throw std::runtime_error( xor_str( "failed to setup game modules" ) );

			if ( !g_game_interfaces->setup( ) )
				throw std::runtime_error( xor_str( "failed to setup game interfaces" ) );

			if ( !g_game_interfaces->setup( ) )
				throw std::runtime_error( xor_str( "failed to setup game interfaces" ) );

			if ( !g_netvar_manager->setup( xor_str( "sdk_netvar_dump.txt" ) ) )
				throw std::runtime_error( xor_str( "failed to setup netvar manager" ) );
		}
		time( &end );

		// finished
		debug_log( "finished setup in {} seconds", static_cast< double >( end - start ) );
    }
    catch ( const std::exception& ex )
    {
        // print error message
        error_log( "[error] {}", ex.what( ) );

#ifdef _DEBUG
        // show error message window (or replace to your exception handler)
        _RPT0( _CRT_ERROR, ex.what( ) );
#else
        // unload
        FreeLibraryAndExitThread( static_cast< HMODULE >( parameter ), EXIT_FAILURE );
#endif
    }

	return 1UL;
}

DWORD WINAPI on_detach( LPVOID parameter )
{
	// panic key
	while ( !GetAsyncKeyState( VK_END ) )
		std::this_thread::sleep_for( 200ms );

#ifdef _DEBUG
    logging::detach_console( );
#endif // _DEBUG

    // free our library memory from process and exit from our thread
    FreeLibraryAndExitThread( ( HMODULE )parameter, EXIT_SUCCESS );
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	if ( ul_reason_for_call == DLL_PROCESS_ATTACH )
	{
		// disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH reasons to call
		DisableThreadLibraryCalls( hModule );

		// basic process check
		if ( c_memory::get_module_handle( xor_str( "csgo.exe" ) ) == nullptr )
		{
			MessageBox( nullptr, xor_str( "this cannot be injected in another process\nopen <csgo.exe> to inject" ), xor_str( "csgo-sdk" ), MB_OK );
			return FALSE;
		}

		// create main thread
		if ( auto hThread = CreateThread( nullptr, 0U, on_attach, hModule, 0UL, nullptr ); hThread != nullptr )
			CloseHandle( hThread );

		// create detach thread
		if ( auto hThread = CreateThread( nullptr, 0U, on_detach, hModule, 0UL, nullptr ); hThread != nullptr )
			CloseHandle( hThread );

		return TRUE;
	}

	return FALSE;
}

