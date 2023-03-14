#include "hooks.h"
// used: input system
#include "../../utilities/input.h"
// used: c_safe_draw
#include "../../draw/safe_draw.h"
// used: g_menu
#include "../menu/menu.h"

#define create_hk_vfunc( detour, _this_ptr, index, fn ) if ( !detour.create( c_memory::get_vfunc( _this_ptr, index ), &fn)) return false;


bool g_hooks::setup( )
{
	try
	{
		if ( MH_Initialize( ) != MH_OK )
			throw std::runtime_error( xor_str( "failed setup minhook" ) );

		// setup imgui font/style/colors
		if ( !g_safe_draw->did_setup )
			g_safe_draw->setup( );

		// setup our hook fns
		create_hk_vfunc( g_detour::end_scene, g_game_interfaces->device, vtable::end_scene, hk_end_scene );
		create_hk_vfunc( g_detour::reset, g_game_interfaces->device, vtable::reset, hk_reset );


	}
	catch ( const std::exception& ex )
	{
		// print error message
		debug_log_error( "[error] {}", ex.what( ) );

#ifdef _DEBUG
		// show error message window (or replace to your exception handler)
		_RPT0( _CRT_ERROR, ex.what( ) );
#endif
	}

	return true;
}

void g_hooks::restore( )
{ 
	MH_DisableHook( MH_ALL_HOOKS );
	MH_RemoveHook( MH_ALL_HOOKS );

	MH_Uninitialize( );
}

#pragma region game_window
long D3DAPI g_hooks::hk_end_scene( IDirect3DDevice9* device )
{
	static auto o_end_scene = g_detour::end_scene.get_original<decltype( &hk_end_scene )>( );
	static void* used_address = nullptr;

	try
	{
		// anti obs
		if ( used_address == nullptr )
		{
			// search for gameoverlay address
			MEMORY_BASIC_INFORMATION info;
			VirtualQuery( _ReturnAddress( ), &info, sizeof( MEMORY_BASIC_INFORMATION ) );

			char module_name[ MAX_PATH ];
			GetModuleFileName( static_cast< HMODULE >( info.AllocationBase ), module_name, MAX_PATH );

			// check if its the module we need
			if ( strstr( module_name, "gameoverlayrenderer.dll" ) != nullptr )
				used_address = _ReturnAddress( );
		}

		// if we currently at gameoverlayrenderer address
		if ( _ReturnAddress( ) == used_address )
		{
			if ( g_safe_draw->did_setup )
			{
				// main imgui frame
				ImGui_ImplDX9_NewFrame( );
				ImGui_ImplWin32_NewFrame( );
				ImGui::NewFrame( );
				{

					g_menu->run( );

				}
				ImGui::EndFrame( );
				ImGui::Render( );

				// render draw lists from draw data
				ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
			}
		}
	}
	catch ( const std::exception& ex )
	{
		debug_log_error( "[error] {}", ex.what( ) );

		// show errors box
#ifdef _DEBUG
		_RPT0( _CRT_ERROR, ex.what( ) );
#endif // _DEBUG
	}

	return o_end_scene( device );
}

long D3DAPI g_hooks::hk_reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters )
{
	static auto o_reset = g_detour::reset.get_original<decltype( &hk_reset )>( );

	if ( !g_safe_draw->did_setup )
		return o_reset( device, presentation_parameters );

	ImGui_ImplDX9_InvalidateDeviceObjects( );

	const HRESULT rst = o_reset( device, presentation_parameters );

	// get directx device and create fonts texture
	if ( rst == D3D_OK )
	{
		ImGui_ImplDX9_CreateDeviceObjects( );
	}

	return rst;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
LRESULT __stdcall g_hooks::hk_wnd_proc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	if ( ImGui_ImplWin32_WndProcHandler( hwnd, message, wparam, lparam ) )
		return 1L;

	g_input::process( message, wparam, lparam );

	if ( g_input::is_key_released( VK_INSERT ) )
		g_menu->set_state( !g_menu->is_opened( ) );

	// handling mouse input when menu is open
	if ( g_menu->is_opened( ) )
	{
		if ( ( wparam & VK_LBUTTON || wparam & VK_RBUTTON ) || ImGui::GetIO( ).WantCaptureKeyboard )
			return 1L;
	}

	return CallWindowProcW( g_input::old_wnd_proc, hwnd, message, wparam, lparam );
}
#pragma endregion