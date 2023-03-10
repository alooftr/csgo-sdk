#include "utilities.h"
#include "pe32.h"

std::filesystem::path utils::get_game_path( )
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
