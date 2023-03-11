#pragma once
#include <cstddef>
#include <cstdint>

#include "../interfaces/ikeyvaluesys.h"
using get_symbol_proc_fn = bool( __cdecl* )( const char* );

class c_key_values
{
public:
	enum e_key_type : int
	{
		type_none = 0,
		type_string,
		type_int,
		type_float,
		type_ptr,
		type_wstring,
		type_color,
		type_uint64,
		type_compiled_int_byte,
		type_compiled_int_0,
		type_compiled_int_1,
		type_numtypes
	};

	c_key_values( const char* name, void* unk = nullptr, h_key_symbol case_insensitive_key_name = INVALID_KEY_SYMBOL );
	~c_key_values( );

	void* operator new( std::size_t alloc_size );
	void operator delete( void* memory );

	const char* get_name( );

	static c_key_values* from_string( const char* name, const char* value );
	void load_from_buffer( char const* resource_name, const char* buffer, void* file_system = nullptr, const char* path_id = nullptr, get_symbol_proc_fn fn_evaluate_symbol_proc = nullptr );
	bool load_from_file( void* file_system, const char* resource_name, const char* path_id = nullptr, get_symbol_proc_fn evaluate_symbol_proc = nullptr );

	c_key_values* find_key( const char* name, const bool create );

	int get_int( const char* name, const int default_value );
	float get_float( const char* name, const float default_value );
	const char* get_string( const char* name, const char* default_value );

	void set_string( const char* name, const char* string_value );
	void set_int( const char* name, const int value );
	void set_uint64( const char* name, const int low_value, const int high_value );

	inline void set_bool( const char* name, const bool value )
	{
		set_int( name, value ? 1 : 0 );
	}

private:
	std::uint32_t key_name : 24; // 0x00
	std::uint32_t key_name_case_sensitive1 : 8; // 0x3 // byte, explicitly specify bits due to packing
	char* value; // 0x04
	wchar_t* wide_value; // 0x08

	union
	{
		int value_int;
		float value_float;
		void* value_ptr;
		unsigned char arr_color[ 4 ];
	}; // 0x0C

	std::int8_t data_type; // 0x10
	bool has_escape_sequences; // 0x11
	std::uint16_t key_name_case_sensitive2; // 0x12
	void* unknown14; // 0x14 // seems like IKeyValuesSystem*, but why do they need it here? also calling smth on destructor and cleans up
	bool has_case_insensitive_key_symbol; // 0x18
	c_key_values* peer; // 0x1C
	c_key_values* sub; // 0x20
	c_key_values* chain; // 0x24
	get_symbol_proc_fn expression_get_symbol_proc; // 0x28
};
static_assert( sizeof( c_key_values ) == 0x2C );