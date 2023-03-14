#include "input.h"

// used: g_game_interfaces
#include "../sdk/csgo.h"
// used: g_hooks namespace
#include "../cheat/hooks/hooks.h"

// dx9 library
#include "../common.h"

bool g_input::setup( )
{
	// trying to grab the window handle if our device exist
	D3DDEVICE_CREATION_PARAMETERS creation_parameters = { };
	while ( FAILED( g_game_interfaces->device->GetCreationParameters( &creation_parameters ) ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

	g_game_interfaces->window_handle = creation_parameters.hFocusWindow;

	old_wnd_proc = reinterpret_cast< WNDPROC >( SetWindowLongPtrW( g_game_interfaces->window_handle, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( g_hooks::hk_wnd_proc ) ) );

	if ( old_wnd_proc == nullptr )
		return false;

	return true;
}

void g_input::restore( )
{
	if ( old_wnd_proc != nullptr )
	{
		SetWindowLongPtrW( g_game_interfaces->window_handle, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( old_wnd_proc ) );
		old_wnd_proc = nullptr;
	}
}

bool g_input::process( UINT msg, WPARAM wparam, LPARAM lparam )
{
	// current active key
	int key = 0;
	// current active key state
	e_key_state state = e_key_state::none;

	switch ( msg )
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if ( wparam < 256U )
			{
				key = wparam;
				state = e_key_state::down;
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if ( wparam < 256U )
			{
				key = wparam;
				state = e_key_state::up;
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
			key = VK_LBUTTON;
			state = msg == WM_LBUTTONUP ? e_key_state::up : e_key_state::down;
			break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
			key = VK_RBUTTON;
			state = msg == WM_RBUTTONUP ? e_key_state::up : e_key_state::down;
			break;
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
			key = VK_MBUTTON;
			state = msg == WM_MBUTTONUP ? e_key_state::up : e_key_state::down;
			break;
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_XBUTTONDBLCLK:
			key = ( GET_XBUTTON_WPARAM( wparam ) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2 );
			state = msg == WM_XBUTTONUP ? e_key_state::up : e_key_state::down;
			break;
		default:
			return false;
	}

	// save key states
	if ( state == e_key_state::up && key_state.at( key ) == e_key_state::down ) // if swap states it will be pressed state
		key_state.at( key ) = e_key_state::released;
	else
		key_state.at( key ) = state;

	return true;
}

bool& key_t::is_active( )
{
	switch ( this->type )
	{
		case type_hold:
		{
			this->active = g_input::is_key_down( this->key );

			break;
		}
		case type_toggle:
		{
			if ( g_input::is_key_released( this->key ) )
				this->active = !this->active;

			break;
		}
		case type_hold_off:
		{
			this->active = !g_input::is_key_down( this->key );

			break;
		}
		case type_on:
		{
			this->active = true;

			break;
		}
	}

    return this->active;
}
