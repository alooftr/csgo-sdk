#include "utilities.h"
// used: pe32 header
#include "pe32.h"
// used: g_game_modules
#include "modules.h"
// used: g_game_interfaces
#include "../sdk/csgo.h"

std::filesystem::path c_utils::get_game_path( )
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

bool c_utils::line_goes_through_smoke( const vec3_t& start, const vec3_t& end, const bool grenade_bloat )
{
    using line_goes_through_smoke_fn = bool( __cdecl* )( vec3_t, vec3_t, bool );
    static auto o_line_goes_through_smoke = reinterpret_cast< line_goes_through_smoke_fn >( g_game_modules->get( client_dll ).find_pattern(  "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0" ) ); // @xref: "effects/overlaysmoke"
    assert( o_line_goes_through_smoke != nullptr );

    return o_line_goes_through_smoke( start, end, grenade_bloat );
}

std::uintptr_t* c_utils::find_hud_element( const char* name )
{
    // @note: https://www.unknowncheats.me/forum/counterstrike-global-offensive/342743-finding-sigging-chud-pointer-chud-findelement.html

    static auto hud = *reinterpret_cast< void** >( g_game_modules->get( client_dll ).find_pattern( "B9 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 89", 0x1 ) ); // @xref: "CHudWeaponSelection"
    assert( hud != nullptr );

    using find_hud_element_fn = std::uintptr_t* ( __thiscall* )( void*, const char* );
    static auto o_find_hud_element = reinterpret_cast< find_hud_element_fn >( g_game_modules->get( client_dll ).find_pattern( "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28" ) ); // @xref: "[%d] Could not find Hud Element: %s\n"
    assert( o_find_hud_element != nullptr );

    return o_find_hud_element( hud, name );
}

void c_utils::force_update( )
{
    using clear_hud_weapon_icon_fn = int( __thiscall* )( void*, int );
    static auto o_clear_hud_weapon_icon = reinterpret_cast< clear_hud_weapon_icon_fn >( g_game_modules->get( client_dll ).find_pattern( "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B" ) ); // @xref: "WeaponIcon--itemcount"
    assert( o_clear_hud_weapon_icon != nullptr );
    if ( const auto hud_weapons = find_hud_element( "CCSGO_HudWeaponSelection" ) - 0x28; hud_weapons != nullptr )
    {
        // go through all weapons
        for ( std::size_t i = 0; i < *( hud_weapons + 0x20 ); i++ )
            i = o_clear_hud_weapon_icon( hud_weapons, i );
    }

    g_game_interfaces->client_state->force_update( );
}

const char8_t* c_utils::get_weapon_icon( short item_definition_index )
{
	/*
	 * @note: icon code = weapon item definition index in hex
	 * list of other icons:
	 *	"E210" - kevlar
	 *	"E20E" - helmet
	 *	"E20F" - defuser kit
	 *	"E211" - banner
	 *	"E212" - target
	 */

	switch ( item_definition_index )
	{
		case weapon_deagle:
			return u8"\uE001";
		case weapon_elite:
			return u8"\uE002";
		case weapon_fiveseven:
			return u8"\uE003";
		case weapon_glock:
			return u8"\uE004";
		case weapon_ak47:
			return u8"\uE007";
		case weapon_aug:
			return u8"\uE008";
		case weapon_awp:
			return u8"\uE009";
		case weapon_famas:
			return u8"\uE00A";
		case weapon_g3sg1:
			return u8"\uE00B";
		case weapon_galilar:
			return u8"\uE00D";
		case weapon_m249:
			return u8"\uE00E";
		case weapon_m4a1:
			return u8"\uE010";
		case weapon_mac10:
			return u8"\uE011";
		case weapon_p90:
			return u8"\uE013";
		case weapon_mp5sd:
			return u8"\uE017";
		case weapon_ump45:
			return u8"\uE018";
		case weapon_xm1014:
			return u8"\uE019";
		case weapon_bizon:
			return u8"\uE01A";
		case weapon_mag7:
			return u8"\uE01B";
		case weapon_negev:
			return u8"\uE01C";
		case weapon_sawedoff:
			return u8"\uE01D";
		case weapon_tec9:
			return u8"\uE01E";
		case weapon_taser:
			return u8"\uE01F";
		case weapon_hkp2000:
			return u8"\uE020";
		case weapon_mp7:
			return u8"\uE021";
		case weapon_mp9:
			return u8"\uE022";
		case weapon_nova:
			return u8"\uE023";
		case weapon_p250:
			return u8"\uE024";
		case weapon_scar20:
			return u8"\uE026";
		case weapon_sg556:
			return u8"\uE027";
		case weapon_ssg08:
			return u8"\uE028";
		case weapon_knife:
			return u8"\uE02A";
		case weapon_flashbang:
			return u8"\uE02B";
		case weapon_hegrenade:
			return u8"\uE02C";
		case weapon_smokegrenade:
			return u8"\uE02D";
		case weapon_molotov:
			[[fallthrough]];
		case weapon_firebomb:
			return u8"\uE02E";
		case weapon_decoy:
			[[fallthrough]];
		case weapon_diversion:
			return u8"\uE02F";
		case weapon_incgrenade:
			return u8"\uE030";
		case weapon_c4:
			return u8"\uE031";
		case weapon_healthshot:
			return u8"\uE039";
		case weapon_knife_gg:
			[[fallthrough]];
		case weapon_knife_t:
			return u8"\uE03B";
		case weapon_m4a1_silencer:
			return u8"\uE03C";
		case weapon_usp_silencer:
			return u8"\uE03D";
		case weapon_cz75a:
			return u8"\uE03F";
		case weapon_revolver:
			return u8"\uE040";
		case weapon_tagrenade:
			return u8"\uE044";
		case weapon_fists:
			return u8"\uE045";
		case weapon_tablet:
			return u8"\uE048";
		case weapon_melee:
			return u8"\uE04A";
		case weapon_axe:
			return u8"\uE04B";
		case weapon_hammer:
			return u8"\uE04C";
		case weapon_spanner:
			return u8"\uE04E";
		case weapon_knife_bayonet:
			return u8"\uE1F4";
		case weapon_knife_css:
			return u8"\uE1F7";
		case weapon_knife_flip:
			return u8"\uE1F9";
		case weapon_knife_gut:
			return u8"\uE1FA";
		case weapon_knife_karambit:
			return u8"\uE1FB";
		case weapon_knife_m9_bayonet:
			return u8"\uE1FC";
		case weapon_knife_tactical:
			return u8"\uE1FD";
		case weapon_knife_falchion:
			return u8"\uE200";
		case weapon_knife_survival_bowie:
			return u8"\uE202";
		case weapon_knife_butterfly:
			return u8"\uE203";
		case weapon_knife_push:
			return u8"\uE204";
		case weapon_knife_cord:
			return u8"\uE205";
		case weapon_knife_canis:
			return u8"\uE206";
		case weapon_knife_ursus:
			return u8"\uE207";
		case weapon_knife_gypsy_jackknife:
			return u8"\uE208";
		case weapon_knife_outdoor:
			return u8"\uE209";
		case weapon_knife_stiletto:
			return u8"\uE20A";
		case weapon_knife_widowmaker:
			return u8"\uE20B";
		case weapon_knife_skeleton:
			return u8"\uE20D";
		default:
			return u8"\u003F";
	}
}