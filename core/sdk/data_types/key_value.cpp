#include "key_value.h"

#include "../csgo.h"

c_key_values::c_key_values( const char* name, void* unk, h_key_symbol case_insensitive_key_name )
{
	using c_key_values_constructor_fn = void( __thiscall* )( void*, const char*, void*, h_key_symbol );
	static c_key_values_constructor_fn o_constructor = reinterpret_cast< c_key_values_constructor_fn >( g_game_modules->get( client_dll ).find_pattern( "55 8B EC 56 8B F1 33 C0 8B 4D 0C 81" ) ); // @xref: client.dll -> "OldParticleSystem_Destroy"
	o_constructor( this, name, unk, case_insensitive_key_name );
}

c_key_values::~c_key_values( )
{
	using c_key_values_destructor_fn = void( __thiscall* )( void*, int );
	static c_key_values_destructor_fn o_destructor = reinterpret_cast< c_key_values_destructor_fn >( g_game_modules->get( client_dll ).find_pattern( "56 8B F1 E8 ? ? ? ? 8B 4E 14" ) );
	o_destructor( this, 1 );
}

void* c_key_values::operator new( std::size_t size )
{
	// manually allocate memory, because game constructor doesn't call it automatically
	return g_game_interfaces->key_values_system->alloc_key_values_memory( size );
}

void c_key_values::operator delete( void* memory )
{
	// do nothing, because game destructor will automatically free memory
	// I::KeyValuesSystem->FreeKeyValuesMemory(pMemory);
	( void )memory;
}

const char* c_key_values::get_name( )
{
	return g_game_interfaces->key_values_system->get_string_for_symbol( this->key_name_case_sensitive1 | ( this->key_name_case_sensitive2 << 8 ) );
}

c_key_values* c_key_values::from_string( const char* name, const char* v )
{
	static auto o_from_string = c_memory::get_absolute<std::uintptr_t>( g_game_modules->get( client_dll ).find_pattern( "E8 ? ? ? ? 83 C4 04 89 45 D8" ) + 0x1 ); // @xref: "#empty#", "#int#"
	c_key_values* key_values = nullptr;

	if ( o_from_string == 0U )
		return nullptr;

	__asm
	{
		push 0
		mov edx, v
		mov ecx, name
		call o_from_string
		add esp, 4
		mov key_values, eax
	}

	return key_values;
}

void c_key_values::load_from_buffer( char const* resource_name, const char* buffer, void* file_system, const char* path_id, get_symbol_proc_fn fn_evaluate_symbol_proc )
{
	using load_from_buffer_fn = void( __thiscall* )( void*, const char*, const char*, void*, const char*, void*, void* );
	static auto o_load_from_buffer = reinterpret_cast< load_from_buffer_fn >( g_game_modules->get( client_dll ).find_pattern( "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89" ) ); // @xref: "KeyValues::LoadFromBuffer(%s%s%s): Begin"
	assert( o_load_from_buffer != nullptr );

	o_load_from_buffer( this, resource_name, buffer, file_system, path_id, fn_evaluate_symbol_proc, nullptr );
}

bool c_key_values::load_from_file( void* file_system, const char* resource_name, const char* path_id, get_symbol_proc_fn fn_evaluate_symbol_proc )
{
	using load_from_file_fn = bool( __thiscall* )( void*, void*, const char*, const char*, void* );
	static auto o_load_from_file = reinterpret_cast< load_from_file_fn >( g_game_modules->get( client_dll ).find_pattern( "55 8B EC 83 E4 F8 83 EC 14 53 56 8B 75 08 57 FF" ) ); // @xref: "rb"
	assert( o_load_from_file != nullptr );

	return o_load_from_file( this, file_system, resource_name, path_id, fn_evaluate_symbol_proc );
}

c_key_values* c_key_values::find_key( const char* name, const bool create )
{
	using find_key_fn = c_key_values * ( __thiscall* )( void*, const char*, bool );
	static auto o_find_key = reinterpret_cast< find_key_fn >( g_game_modules->get( client_dll ).find_pattern( "55 8B EC 83 EC 1C 53 8B D9 85 DB" ) );
	assert( o_find_key != nullptr );

	return o_find_key( this, name, create );
}

int c_key_values::get_int( const char* name, const int iDefaultValue )
{
	c_key_values* sub_key = this->find_key( name, false );

	if ( sub_key == nullptr )
		return iDefaultValue;

	switch ( sub_key->data_type )
	{
		case type_string:
			return std::atoi( sub_key->value );
		case type_wstring:
			return _wtoi( sub_key->wide_value);
		case type_float:
			return static_cast< int >( sub_key->value_float );
		case type_uint64:
			// can't convert, since it would lose data
			assert( false );
			return 0;
		default:
			break;
	}

	return sub_key->value_int;
}

float c_key_values::get_float( const char* name, const float flDefaultValue )
{
	c_key_values* sub_key = this->find_key( name, false );

	if ( sub_key == nullptr )
		return flDefaultValue;

	switch ( sub_key->data_type )
	{
		case type_string:
			return static_cast< float >( std::atof( sub_key->value ) );
		case type_wstring:
			return std::wcstof( sub_key->wide_value, nullptr );
		case type_float:
			return sub_key->value_float;
		case type_int:
			return static_cast< float >( sub_key->value_int );
		case type_uint64:
			return static_cast< float >( *reinterpret_cast< std::uint64_t* >( sub_key->value ) );
		case type_ptr:
		default:
			return 0.0f;
	}
}

const char* c_key_values::get_string( const char* name, const char* szDefaultValue )
{
	using get_string_fn = const char* ( __thiscall* )( void*, const char*, const char* );
	static auto o_get_string = reinterpret_cast< get_string_fn >( g_game_modules->get( client_dll ).find_pattern( "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 8B 5D 08" ) );
	assert( o_get_string != nullptr );

	return o_get_string( this, name, szDefaultValue );
}

void c_key_values::set_string( const char* name, const char* szStringValue )
{
	c_key_values* sub_key = find_key( name, true );

	if ( sub_key == nullptr )
		return;

	using set_string_fn = void( __thiscall* )( void*, const char* );
	static auto o_set_string = reinterpret_cast< set_string_fn >( g_game_modules->get( client_dll ).find_pattern( "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01" ) );
	assert( o_set_string != nullptr );

	o_set_string( sub_key, szStringValue );
}

void c_key_values::set_int( const char* name, const int iValue )
{
	c_key_values* sub_key = find_key( name, true );

	if ( sub_key == nullptr )
		return;

	sub_key->value_int = iValue;
	sub_key->data_type = type_int;
}

void c_key_values::set_uint64( const char* name, const int nLowValue, const int nHighValue )
{
	c_key_values* sub_key = find_key( name, true );

	if ( sub_key == nullptr )
		return;

	// delete the old value
	delete[ ] sub_key->value;

	// make sure we're not storing the WSTRING - as we're converting over to STRING
	delete[ ] sub_key->wide_value;
	sub_key->wide_value = nullptr;

	sub_key->value = new char[ sizeof( std::uint64_t ) ];
	*reinterpret_cast< std::uint64_t* >( sub_key->value ) = static_cast< std::uint64_t >( nHighValue ) << 32ULL | nLowValue;
	sub_key->data_type = type_uint64;
}
