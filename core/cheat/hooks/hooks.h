#pragma once

// used: window api and direct x
#include "../../common.h"
// used: detour and min hook
#include "../../utilities/detour_hook.h"
// used: csgo sdk
#include "../../sdk/csgo.h"

namespace vtable
{
	enum
	{
		reset = 16,
		end_scene = 42,
	};
}

namespace g_detour
{
	inline c_detour_hook end_scene;
	inline c_detour_hook reset;
}

namespace g_hooks
{
	bool setup( );
	void restore( );

	long	D3DAPI		hk_end_scene( IDirect3DDevice9* device );
	long	D3DAPI		hk_reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters );
	LRESULT __stdcall	hk_wnd_proc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
}
