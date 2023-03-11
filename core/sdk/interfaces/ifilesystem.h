#pragma once

class i_file_system
{
public:
	int read( void* output, int size, void* handle )
	{
		return c_memory::call_vfunc<int>( this, 0, output, size, handle );
	}

	void* open( const char* fileName, const char* options, const char* path_id )
	{
		return c_memory::call_vfunc<void*>( this, 2, fileName, options, path_id );
	}

	void close( void* handle )
	{
		return c_memory::call_vfunc<void>( this, 3, handle );
	}

	unsigned int size( void* handle )
	{
		return c_memory::call_vfunc<unsigned int>( this, 7, handle );
	}
};
