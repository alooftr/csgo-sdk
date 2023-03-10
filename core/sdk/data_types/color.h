#pragma once
// used: std::array
#include <array>
#include <d3d9.h>
#include <cmath>

// used: imgui color struct
#include "../../../ext/imgui/imgui.h"

enum
{
	col_r = 0,
	col_g = 1,
	col_b = 2,
	col_a = 3
};

struct col_t
{
public:
	col_t( ) = default;

	/* default col_t constructor (in: 0 - 255) */
	constexpr col_t( std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255 ) :
		colors( { r, g, b, a } )
	{ }

	constexpr col_t( int r, int g, int b, int a = 255 ) :
		colors( { static_cast< std::uint8_t >( r ), static_cast< std::uint8_t >( g ), static_cast< std::uint8_t >( b ), static_cast< std::uint8_t >( a ) } )
	{ }

	/* float col_t constructor (in: 0.0 - 1.0) */
	constexpr col_t( float r, float g, float b, float a = 1.0f ) :
		colors( { static_cast< std::uint8_t >( r * 255.f ), static_cast< std::uint8_t >( g * 255.f ), static_cast< std::uint8_t >( b * 255.f ), static_cast< std::uint8_t >( a * 255.f ) } )
	{ }

	/* output col_t to given variables */
	void get( std::uint8_t& r, std::uint8_t& g, std::uint8_t& b, std::uint8_t& a ) const
	{
		r = colors[ col_r ];
		g = colors[ col_g ];
		b = colors[ col_b ];
		a = colors[ col_a ];
	}

	/* convert col_t to directx argb */
	[[nodiscard]] D3DCOLOR get_d3d( ) const
	{
		return D3DCOLOR_ARGB( colors[ col_a ], colors[ col_r ], colors[ col_g ], colors[ col_b ] );
	}

	/* convert col_t to imgui rgba */
	[[nodiscard]] ImU32 get_u32( const float alpha_multiplier = 1.0f ) const
	{
		return ImGui::GetColorU32( this->get_vec4( alpha_multiplier ) );
	}

	/* convert col_t to imgui vector */
	[[nodiscard]] ImVec4 get_vec4( const float alpha_multiplier = 1.0f ) const
	{
		return ImVec4( this->base<col_r>( ), this->base<col_g>( ), this->base<col_b>( ), this->base<col_a>( ) * alpha_multiplier );
	}

	std::uint8_t& operator[]( const std::size_t i )
	{
		return this->colors[ i ];
	}

	const std::uint8_t& operator[]( const std::size_t i ) const
	{
		return this->colors[ i ];
	}

	bool operator==( const col_t& second ) const
	{
		return this->colors == second.colors;
	}

	bool operator!=( const col_t& second ) const
	{
		return !( operator==( second ) );
	}

	col_t& operator=( const col_t& from )
	{
		colors[ col_r ] = from.colors[ col_r ];
		colors[ col_g ] = from.colors[ col_g ];
		colors[ col_b ] = from.colors[ col_b ];
		colors[ col_a ] = from.colors[ col_a ];
		return *this;
	}

	/* returns certain R/G/B/A value */
	template <std::size_t n>
	[[nodiscard]] std::uint8_t get( ) const
	{
		static_assert( n >= col_r && n <= col_a, "given index is out of range" );
		return colors[ n ];
	}

	/* returns copy of col_t with changed certain R/G/B/A value to given value */
	template <std::size_t n>
	[[nodiscard]] col_t set( const std::uint8_t value ) const
	{
		static_assert( n >= col_r && n <= col_a, "given index is out of range" );

		col_t copy = *this;
		copy.colors[ n ] = value;
		return copy;
	}

	// set which the min value of that index color
	template <std::size_t n>
	[[nodiscard]] col_t set_min( const std::uint8_t value ) const
	{
		static_assert( n >= col_r && n <= col_a, "given index is out of range" );

		col_t copy = *this;
		copy.colors[ n ] = fmin( this->colors[ n ], value );
		return copy;
	}

	/* returns copy of col_t with multiplied certain R/G/B/A value by given value */
	template <std::size_t n>
	[[nodiscard]] col_t multiplier( const float value ) const
	{
		static_assert( n >= col_r && n <= col_a, "given index is out of range" );

		col_t copy = *this;
		copy.colors[ n ] = static_cast< std::uint8_t >( static_cast< float >( copy.colors[ n ] ) * value );
		return copy;
	}

	/* returns copy of col_t with divided certain R/G/B/A value by given value */
	template <std::size_t n>
	[[nodiscard]] col_t divider( const int value ) const
	{
		static_assert( n >= col_r && n <= col_a, "given index is out of range" );

		col_t copy = *this;
		copy.colors[ n ] /= value;
		return copy;
	}

	template <std::size_t n>
	[[nodiscard]] col_t add( const int add ) const
	{
		static_assert( n >= col_r && n <= col_a, "given index is out of range" );

		col_t copy = *this;
		copy.colors[ n ] += add;
		return copy;
	}

	template <std::size_t n>
	[[nodiscard]] col_t sub( const int add ) const
	{
		static_assert( n >= col_r && n <= col_a, "given index is out of range" );

		col_t copy = *this;
		copy.colors[ n ] -= add;
		return copy;
	}


	/* returns certain R/G/B/A float value (in: 0 - 255, out: 0.0 - 1.0) */
	template <std::size_t n>
	[[nodiscard]] float base( ) const
	{
		static_assert( n >= col_r && n <= col_a, "given index is out of range" );
		return colors[ n ] / 255.f;
	}

	/* convert col_t to float array (in: 0 - 255, out: 0.0 - 1.0) */
	[[nodiscard]] std::array<float, 3U> base( ) const
	{
		std::array<float, 3U> base_color = { };

		base_color[ col_r ] = this->base<col_r>( );
		base_color[ col_g ] = this->base<col_g>( );
		base_color[ col_b ] = this->base<col_b>( );

		return base_color;
	}

	/* set col_t from float array (in: 0.0 - 1.0, out: 0 - 255) */
	static col_t from_base3( float base[ 3 ] )
	{
		return col_t( base[ 0 ], base[ 1 ], base[ 2 ] );
	}

	/* convert col_t to float array w/ alpha (in: 0 - 255, out: 0.0 - 1.0) */
	[[nodiscard]] std::array<float, 4U> base_alpha( ) const
	{
		std::array<float, 4U> base_color = { };

		base_color[ col_r ] = this->base<col_r>( );
		base_color[ col_g ] = this->base<col_g>( );
		base_color[ col_b ] = this->base<col_b>( );
		base_color[ col_a ] = this->base<col_a>( );

		return base_color;
	}

	/* set col_t from float array w/ alpha (in: 0.0 - 1.0, out: 0 - 255) */
	static col_t from_base4( float base[ 4 ] )
	{
		return col_t( base[ 0 ], base[ 1 ], base[ 2 ], base[ 3 ] );
	}

	[[nodiscard]] float hue( ) const
	{
		if ( colors[ col_r ] == colors[ col_g ] && colors[ col_g ] == colors[ col_b ] )
			return 0.f;

		const float r = this->base<col_r>( );
		const float g = this->base<col_g>( );
		const float b = this->base<col_b>( );

		const float flMax = std::max( r, std::max( g, b ) ), flMin = std::min( r, std::min( g, b ) );

		if ( flMax == flMin )
			return 0.f;

		const float flDelta = flMax - flMin;
		float hue = 0.f;

		if ( flMax == r )
			hue = ( g - b ) / flDelta;
		else if ( flMax == g )
			hue = 2.f + ( b - r ) / flDelta;
		else if ( flMax == b )
			hue = 4.f + ( r - g ) / flDelta;

		hue *= 60.f;

		if ( hue < 0.f )
			hue += 360.f;

		return hue / 360.f;
	}

	[[nodiscard]] float saturation( ) const
	{
		const float r = this->base<col_r>( );
		const float g = this->base<col_g>( );
		const float b = this->base<col_b>( );

		const float flMax = std::max( r, std::max( g, b ) ), flMin = std::min( r, std::min( g, b ) );
		const float flDelta = flMax - flMin;

		if ( flMax == 0.f )
			return flDelta;

		return flDelta / flMax;
	}

	[[nodiscard]] float brightness( ) const
	{
		const float r = this->base<col_r>( );
		const float g = this->base<col_g>( );
		const float b = this->base<col_b>( );

		return std::max( r, std::max( g, b ) );
	}

	/* return RGB col_t converted from HSB/HSV col_t */
	static col_t from_hsb( float hue, float saturation, float brightness, float alpha = 1.0f )
	{
		const float h = std::fmodf( hue, 1.0f ) / ( 60.0f / 360.0f );
		const int i = static_cast< int >( h );
		const float f = h - static_cast< float >( i );
		const float p = brightness * ( 1.0f - saturation );
		const float q = brightness * ( 1.0f - saturation * f );
		const float t = brightness * ( 1.0f - saturation * ( 1.0f - f ) );

		float r = 0.0f, g = 0.0f, b = 0.0f;

		switch ( i )
		{
			case 0:
				r = brightness, g = t, b = p;
				break;
			case 1:
				r = q, g = brightness, b = p;
				break;
			case 2:
				r = p, g = brightness, b = t;
				break;
			case 3:
				r = p, g = q, b = brightness;
				break;
			case 4:
				r = t, g = p, b = brightness;
				break;
			case 5:
			default:
				r = brightness, g = p, b = q;
				break;
		}

		return col_t( r, g, b, alpha );
	}

	unsigned long to_hex( )
	{
		return ( ( ( colors[ col_r ] ) << 24 ) | ( ( colors[ col_g ] ) << 16 ) | ( ( colors[ col_b ] ) << 8 ) | ( ( colors[ col_a ] ) ) );
	}

	static col_t from_hex( unsigned long hex )
	{
		int r = ( hex >> 24 ) & 0xFF;
		int g = ( hex >> 16 ) & 0xFF;
		int b = ( hex >> 8 ) & 0xFF;
		int a = ( hex ) & 0xFF;

		return col_t( r, g, b, a );
	}

private:
	std::array<std::uint8_t, 4U> colors = { 255, 255, 255, 255 };
};
