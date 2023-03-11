#pragma once

#include "memory.h"

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