#pragma once

// used: std::vector for getfilledvector
#include <vector>
// used: std::unordered_map for safe_ptr
#include <unordered_map>
// used: winapi, xor_str
#include "../common.h"
// used: minhook library
#include "../../ext/minhook/minhook.h"
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
};

/*
 * detour hooking implementation using minhook
 * @credits: https://github.com/TsudaKageyu/minhook
 */

class c_detour_hook
{
public:
	c_detour_hook( ) = default;

	explicit c_detour_hook( void* func, void* detour )
		: base_fn( func ), replace_fn( detour )
	{ }

	/* first hook setup */
	bool create( void* func, void* detour )
	{
		base_fn = func;

		if ( base_fn == nullptr )
			return false;

		replace_fn = detour;

		if ( replace_fn == nullptr )
			return false;

		const MH_STATUS status = MH_CreateHook( base_fn, replace_fn, &original_fn );

		if ( status != MH_OK )
			throw std::runtime_error( std::format( "failed to create hook function, status: {}\nbase function -> {:#08x}", MH_StatusToString( status ), reinterpret_cast< std::uintptr_t >( base_fn ) ) );

		if ( !this->replace( ) )
			return false;

		return true;
	}

	/* replace function to our modified */
	bool replace( )
	{
		// check is hook be created
		if ( base_fn == nullptr )
			return false;

		// check is function not already hooked
		if ( hooked )
			return false;

		const MH_STATUS status = MH_EnableHook( base_fn );

		if ( status != MH_OK )
			throw std::runtime_error( std::format( "failed to enable hook function, status: {}\nbase function -> {:#08X} address", MH_StatusToString( status ), reinterpret_cast< std::uintptr_t >( base_fn ) ) );

		// switch hook state
		hooked = true;
		return true;
	}

	/* replace swaped modified function back to original */
	bool restore( )
	{
		// check is function hooked
		if ( !hooked )
			return false;

		const MH_STATUS status = MH_DisableHook( base_fn );

		if ( status != MH_OK )
			throw std::runtime_error( std::format( "failed to restore hook, status: {}\n base function -> {:#08X} address", MH_StatusToString( status ), reinterpret_cast< std::uintptr_t >( base_fn ) ) );

		// switch hook state
		hooked = false;
		return true;
	}

	/* get original function pointer (not a call!) */
	template <typename fn>
	fn get_original( )
	{
		return static_cast< fn >( original_fn );
	}

	/* returns hook state */
	inline bool is_hooked( ) const
	{
		return hooked;
	}

private:
	// Values
	/* hook state */
	bool hooked = false;
	/* function base address */
	void* base_fn = nullptr;
	/* in future that is being modified and replace original */
	void* replace_fn = nullptr;
	/* saved function to get available restore hook later */
	void* original_fn = nullptr;
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
		debug_log( "setup module: {} -> {:#08X}", this->name, reinterpret_cast< std::uintptr_t >( this->handle ) );
	}

	template<typename t = std::uintptr_t>
	t find_pattern( const std::string_view pattern, std::uintptr_t offset = 0x0 )
	{
		return static_cast< t >( c_memory::find_pattern( this->name, pattern ) + offset );
	}

	void* get_handle( )
	{
		return this->handle;
	}

	void* get_proc_address( const std::string_view proc_name )
	{
		return c_memory::get_proc_address( this->handle, proc_name );
	}

private:
	// only used for get_interface
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
				debug_log( "found {} interface -> {:#08x}", register_list->name, reinterpret_cast< std::uintptr_t >( interface_pointer ) );

				return static_cast< t* >( interface_pointer );
			}
		}

		error_log( "failed to find interface \"{}\" in \"{}\"", name, this->name );

		return nullptr;
	}
private:
    void* handle = nullptr;
	const char* name = nullptr;
};