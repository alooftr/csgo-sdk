#pragma once

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/string_t.h
struct string_t
{
public:
	bool operator!( ) const
	{
		return ( value == nullptr );
	}
	bool operator==( const string_t& rhs ) const
	{
		return ( value == rhs.value );
	}
	bool operator!=( const string_t& rhs ) const
	{
		return ( value != rhs.value );
	}
	bool operator<( const string_t& rhs ) const
	{
		return ( reinterpret_cast< void* >( const_cast< char* >( value ) ) < reinterpret_cast< void* >( const_cast< char* >( rhs.value ) ) );
	}

	const char* c_str( ) const
	{
		return ( value ) ? value : "";
	}
protected:
	const char* value;
};