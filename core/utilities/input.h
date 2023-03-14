#pragma once

// used: window api
#include <Windows.h>
// used: std::array
#include <array>

enum class e_key_state : int
{
	none,
	down,
	up,
	released
};

/*
 * input system
 * listen and handle key states
 */
namespace g_input
{
	// Values
	/* saved window messages handler */
	inline WNDPROC old_wnd_proc = nullptr;
	/* last processed key states */
	inline std::array<e_key_state, 256U> key_state = { };

	// Get
	/* set our window messages proccesor */
	bool setup( );
	/* restore window messages processor and clear saved pointer */
	void restore( );
	/* process input window message and save keys states in array */
	bool process( UINT msg, WPARAM wparam, LPARAM lparam );

	/* is given key being held */
	inline bool is_key_down( const std::uint32_t button_code )
	{
		return key_state.at( button_code ) == e_key_state::down;
	}

	/* was given key released */
	inline bool is_key_released( const std::uint32_t button_code )
	{
		if ( key_state.at( button_code ) == e_key_state::released )
		{
			key_state.at( button_code ) = e_key_state::up;
			return true;
		}

		return false;
	}
}

/*
	 * key bind system
*/

enum e_key_type : int
{
	type_hold = 0,
	type_toggle,
	type_hold_off,
	type_on
};

struct key_t
{
	std::uint32_t key = 0;
	e_key_type type = type_hold;
	
	bool& is_active( );
private:
	bool active = false;
};