#pragma once
// used: InterlockedDecrement
#include <windows.h>

class i_ref_counted
{
public:
	virtual int add_reference( ) = 0;
	virtual int release( ) = 0;
};

class c_ref_counted
{
public:
	virtual void destructor( char bDelete ) = 0;
	virtual bool on_final_release( ) = 0;

	void release( )
	{
		if ( InterlockedDecrement( &ref_count ) == 0 && on_final_release( ) )
			destructor( 1 );
	}

private:
	volatile long ref_count;
};

template <class T>
class c_base_auto_ptr
{
public:
	c_base_auto_ptr( ) :
		object( nullptr )
	{ }

	c_base_auto_ptr( T* object ) :
		object( object )
	{ }

	operator const void* ( ) const
	{
		return object;
	}

	operator void* ( ) const
	{
		return object;
	}

	operator const T* ( ) const
	{
		return object;
	}

	operator const T* ( )
	{
		return object;
	}

	operator T* ( )
	{
		return object;
	}

	int	operator=( int i )
	{
		object = nullptr;
		return 0;
	}

	T* operator=( T* obj )
	{
		object = obj;
		return obj;
	}

	bool operator!( ) const
	{
		return ( !object );
	}

	bool operator==( const void* obj ) const
	{
		return ( object == obj );
	}

	bool operator!=( const void* obj ) const
	{
		return ( object != obj );
	}

	bool operator==( T* obj ) const
	{
		return operator==( static_cast< void* >( obj ) );
	}

	bool operator!=( T* obj ) const
	{
		return operator!=( static_cast< void* >( obj ) );
	}

	bool operator==( const c_base_auto_ptr<T>& second_ptr ) const
	{
		return operator==( static_cast< const void* >( second_ptr ) );
	}

	bool operator!=( const c_base_auto_ptr<T>& second_ptr ) const
	{
		return operator!=( static_cast< const void* >( second_ptr ) );
	}

	T* operator->( )
	{
		return object;
	}

	T& operator*( )
	{
		return *object;
	}

	T** operator&( )
	{
		return &object;
	}

	const T* operator->( ) const
	{
		return object;
	}

	const T& operator*( ) const
	{
		return *object;
	}

	T* const* operator&( ) const
	{
		return &object;
	}

protected:
	c_base_auto_ptr( const c_base_auto_ptr<T>& second_ptr ) :
		object( second_ptr.object )
	{ }

	void operator=( const c_base_auto_ptr<T>& second_ptr )
	{
		object = second_ptr.object;
	}

	T* object;
};

template <class T>
class c_ref_ptr : public c_base_auto_ptr<T>
{
	typedef c_base_auto_ptr<T> c_base_class;
public:
	c_ref_ptr( )
	{ }

	c_ref_ptr( T* pInit )
		: c_base_class( pInit )
	{ }

	c_ref_ptr( const c_ref_ptr<T>& pRefPtr )
		: c_base_class( pRefPtr )
	{ }

	~c_ref_ptr( )
	{
		if ( c_base_class::object != nullptr )
			c_base_class::object->release( );
	}

	void operator=( const c_ref_ptr<T>& ref_ptr )
	{
		c_base_class::operator=( ref_ptr );
	}

	int operator=( int i )
	{
		return c_base_class::operator=( i );
	}

	T* operator=( T* sec )
	{
		return c_base_class::operator=( sec );
	}

	operator bool( ) const
	{
		return !c_base_class::operator!( );
	}

	operator bool( )
	{
		return !c_base_class::operator!( );
	}

	void safe_release( )
	{
		if ( c_base_class::object != nullptr )
			c_base_class::object->release( );

		c_base_class::object = nullptr;
	}

	void assign_add_reference( T* from )
	{
		if ( from != nullptr )
			from->add_reference( );

		safe_release( );
		c_base_class::object = from;
	}

	void add_reference_assign_to( T*& pTo )
	{
		if ( c_base_class::object != nullptr )
			c_base_class::object->add_reference( );

		safe_release( pTo );
		pTo = c_base_class::object;
	}
};
