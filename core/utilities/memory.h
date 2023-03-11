#pragma once

// used: std::vector for getfilledvector
#include <vector>
// used: std::unordered_map for safe_ptr
#include <unordered_map>
// used: winapi, xor_str
#include "../common.h"
// used:: logging namespace
#include "logging.h"

#pragma region memory_modules_definitions
#define engine_dll              xor_str("engine.dll")
#define client_dll              xor_str("client.dll")
#define localize_dll            xor_str("localize.dll")
#define materialsystem_dll      xor_str("materialsystem.dll")
#define vgui_dll                xor_str("vguimatsurface.dll")
#define vgui2_dll               xor_str("vgui2.dll")
#define shaderpidx9_dll			xor_str("shaderapidx9.dll")
#define gameoverlayrenderer_dll xor_str("gameoverlayrenderer.dll")
#define physics_dll				xor_str("vphysics.dll")
#define vstdlib_dll				xor_str("vstdlib.dll")
#define tier0_dll				xor_str("tier0.dll")
#define inputsystem_dll			xor_str("inputsystem.dll")
#define studiorender_dll		xor_str("studiorender.dll")
#define datacache_dll			xor_str("datacache.dll")
#define steam_api_dll			xor_str("steam_api.dll")
#define matchmaking_dll			xor_str("matchmaking.dll")
#define server_dll				xor_str("server.dll")
#define serverbrowser_dll		xor_str("serverbrowser.dll")
#pragma endregion

/* virtualprotect raii wrapper */
class c_wrapped_protect
{
public:
	c_wrapped_protect( void* base_address, const std::size_t length, const DWORD dwFlags ) :
		base_address( base_address ), length( length )
	{
		if ( !VirtualProtect( base_address, length, dwFlags, &old_flags ) )
			throw std::system_error( GetLastError( ), std::system_category( ), xor_str( "failed to protect region" ) );
	}

	~c_wrapped_protect( )
	{
		VirtualProtect( base_address, length, old_flags, &old_flags );
	}
private:
	void* base_address;
	std::size_t		length;
	DWORD			old_flags;
};

struct module_handle_info_t
{
	module_handle_info_t( const char* name, void* handle ) : name{ name }, handle{ handle } { }

	const char* name = nullptr;
	void* handle = nullptr;

	constexpr explicit operator bool( ) noexcept
	{ 
		return handle != nullptr;
	}

	constexpr bool operator==( nullptr_t )
	{
		return handle == nullptr;
	}
};

class c_memory
{
public:
	/* main */

    /// @param moduleName : module name to search base handle for, empty means current module
    /// @returns : base handle of given module name
    static module_handle_info_t get_module_handle( const std::string_view module_name );
    /// alternative of 'GetProcAddress()'
    static void* get_proc_address( const void* module_base, const std::string_view procedure_name );

    /// ida style pattern byte comparison in specific module
    /// @param moduleName : module name where search for pattern
    /// @param pattern : ida style pattern, e.g. "55 8B 40 ? 30", wildcard can be either '?' or "??", blank delimiters are ignored  
    static std::uintptr_t find_pattern( const std::string_view module_name, const std::string_view pattern );
    static std::uintptr_t find_pattern( const std::uint8_t* region_start, const std::uintptr_t region_size, const std::string_view pattern );

	/// @returns : absolute address from relative address
	template <typename t>
	static constexpr auto get_absolute( uintptr_t address ) noexcept
	{
		return ( t )( address + 4 + *reinterpret_cast< std::int32_t* >( address ) );
	}
	/// @returns : all cross-references (x-refs) addresses in given range
	std::vector<std::uintptr_t> get_cross_references( const std::uintptr_t address, std::uintptr_t region_start, const std::size_t region_size );
	/// @param szSectionName : section to get info of (e.g. ".rdata", ".text", etc)
	/// @param puSectionStart : output for section start address
	/// @param puSectionSize : output for section size
	/// @returns : true if section has been found
	bool get_section_info( const std::uintptr_t base_address, const std::string_view section_name, std::uintptr_t* section_start, std::uintptr_t* section_size );
	/// @returns : type descriptor address of given vtable name in given module
	std::uintptr_t get_vtable_type_descriptor( const std::string_view module_name, const std::string_view table_name );
	/// @returns : pointer of given vtable name in given module @credits: hinnie
	std::uintptr_t* get_vtable_pointer( const std::string_view module_name, const std::string_view table_name );

	/// @returns : virtual function pointer of specified class at given index
    template <typename T = void*>
    static constexpr T get_vfunc( void* this_ptr, std::size_t index )
    {
        return ( *static_cast< T** >( this_ptr ) )[ index ];
    }
    // call virtual function of specified class at given index
    // @note: references should be wrapped with std::ref call!
    template <typename T, typename ... Args_t>
    static constexpr T call_vfunc( void* this_ptr, std::size_t index, Args_t... arg_list )
    {
        using virtual_fn = T( __thiscall* )( void*, decltype( arg_list )... );
        return ( *static_cast< virtual_fn** >( this_ptr ) )[ index ]( this_ptr, arg_list... );
    }

    /* misc: converter */
    /* todo: bytes to pattern... */
    static std::vector<std::optional<std::uint8_t>> pattern_to_bytes( const std::string_view pattern );
	static std::string bytes_to_pattern( const std::uint8_t* bytes, const std::size_t size );
};

/* modules pointer as we normally hooking with game's modules memory */
class c_module
{
public:
	c_module( ) = default;
	c_module( const char* module_name ) : name{ module_name }
	{
		this->handle = c_memory::get_module_handle( module_name ).handle;
	}
	c_module( module_handle_info_t info )
	{
		this->handle = info.handle;
		this->name = info.name;
		debug_log_ok( "setup module: {} -> [{:#08X}]", this->name, reinterpret_cast< std::uintptr_t >( this->handle ) );
	}

	std::uintptr_t find_pattern( const std::string_view pattern, std::uintptr_t offset = 0x0 )
	{
		return c_memory::find_pattern( this->name, pattern ) + offset;
	}

	void* get_handle( )
	{
		return this->handle;
	}

	void* get_proc_address( const std::string_view proc_name )
	{
		return c_memory::get_proc_address( this->handle, proc_name );
	}

	// only used for get_interface
private:
	class c_interface_register
	{
	public:
		void* ( __cdecl* create_fn )( );	//0x0
		const char* name;					//0x4
		c_interface_register* next;			//0x8
	};
public:
	template<typename t>
	t* get_interface( const std::string_view name )
	{
		debug_log( "finding [{}] exportex interface from [{}]", name.data( ), this->name );

		const auto get_register_list = [this]( ) -> c_interface_register*
		{
			void* o_create_interface = c_memory::get_proc_address( this->handle, "CreateInterface" );

			if ( o_create_interface == nullptr )
				throw std::runtime_error( "failed get CreateInterface address" );

			const std::uintptr_t create_interface_relative = reinterpret_cast< std::uintptr_t >( o_create_interface ) + 0x5;
			const std::uintptr_t create_interface = create_interface_relative + 4U + *reinterpret_cast< std::int32_t* >( create_interface_relative );
			return **reinterpret_cast< c_interface_register*** >( create_interface + 0x6 );
		};

		for ( auto register_list = get_register_list( ); register_list != nullptr; register_list = register_list->next )
		{
			// found needed interface
			if ( ( std::string_view( register_list->name ).compare( 0U, name.length( ), name ) == 0 &&
				// and it have digits after name
				std::atoi( register_list->name + name.length( ) ) > 0 ) ||
				// or given full name with hardcoded digits
				name.compare( register_list->name ) == 0 )
			{
				// capture our interface
				auto interface_pointer = register_list->create_fn( );

				// log interface address
				debug_log_ok( "found {} interface -> [{:#08x}]", register_list->name, reinterpret_cast< std::uintptr_t >( interface_pointer ) );

				return static_cast< t* >( interface_pointer );
			}
		}

		debug_log_error( "failed to find interface \"{}\" in \"{}\"", name, this->name );

		return nullptr;
	}
private:
    void* handle = nullptr;
	const char* name = nullptr;
};

/* holding all the game's loaded module */
class c_game_modules
{
public:
	bool setup( );

	auto get( std::string name )
	{
		return this->modules[ name ];
	}
private:
	std::unordered_map<std::string, c_module> modules = { };
};

inline const std::unique_ptr<c_game_modules> g_game_modules{ new c_game_modules( ) };