#pragma once

#include <corecrt_malloc.h>

template <class t, class n = int>
class c_utl_memory
{
public:
	t& operator[]( n i )
	{
		return memory[ i ];
	}

	const t& operator[]( n i ) const
	{
		return memory[ i ];
	}

	t* base( )
	{
		return memory;
	}

	[[nodiscard]] int num_allocated( ) const
	{
		return i_allocation_count;
	}

	[[nodiscard]] bool is_externally_allocated( ) const
	{
		return grow_size < 0;
	}

	void grow( const int iCount = 1 )
	{
		if ( is_externally_allocated( ) )
			return;

		int allocation_requested = i_allocation_count + iCount;
		int new_allocation_count = 0;

		if ( grow_size )
			i_allocation_count = ( ( 1 + ( ( allocation_requested - 1 ) / grow_size ) ) * grow_size );
		else
		{
			if ( !i_allocation_count )
				i_allocation_count = ( 31 + sizeof( t ) ) / sizeof( t );

			while ( i_allocation_count < allocation_requested )
				i_allocation_count <<= 1;
		}

		if ( static_cast< int >( static_cast< n >( new_allocation_count ) ) < allocation_requested )
		{
			if ( static_cast< int >( static_cast< n >( new_allocation_count ) ) == 0 && static_cast< int >( static_cast< n >( new_allocation_count - 1 ) ) >= allocation_requested )
				--new_allocation_count;
			else
			{
				if ( static_cast< int >( static_cast< n >( allocation_requested ) ) != allocation_requested )
					return;

				while ( static_cast< int >( static_cast< n >( new_allocation_count ) ) < allocation_requested )
					new_allocation_count = ( new_allocation_count + allocation_requested ) / 2;
			}
		}

		i_allocation_count = new_allocation_count;

		if ( memory != nullptr )
			memory = static_cast< t* >( realloc( memory, i_allocation_count * sizeof( t ) ) );
		else
			memory = static_cast< t* >( malloc( i_allocation_count * sizeof( t ) ) );
	}

protected:
	t* memory;
	int i_allocation_count;
	int grow_size;
};
