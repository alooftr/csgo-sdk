#include "utilities.h"
// used: pe32 header
#include "pe32.h"
// used c_memory
#include "memory.h"

std::filesystem::path g_utils::get_game_path( )
{
    static bool got_path = false;
    static std::filesystem::path path;

    if ( !got_path )
    {
        static _PEB32* peb = reinterpret_cast< _PEB32* >( __readfsdword( 0x30 ) );
        path = peb->ProcessParameters->ImagePathName.Buffer;
        path.remove_filename( );
        got_path = true;
    }

    return path;
}

bool g_utils::line_goes_through_smoke( const vec3_t& start, const vec3_t& end, const bool grenade_bloat )
{
    using line_goes_through_smoke_fn = bool( __cdecl* )( vec3_t, vec3_t, bool );
    static auto o_line_goes_through_smoke = reinterpret_cast< line_goes_through_smoke_fn >( c_memory::find_pattern( "client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0" ) ); // @xref: "effects/overlaysmoke"
    assert( o_line_goes_through_smoke != nullptr );

    return o_line_goes_through_smoke( start, end, grenade_bloat );
}
