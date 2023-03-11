#pragma once

#include "../entity.h"

class i_client_networkable;
class i_client_unknown;
class i_client_entity;
class c_base_entity;
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/icliententitylist.h
class i_client_entity_list
{
public:
	virtual i_client_networkable*	get_client_networkable( int index ) = 0;
	virtual i_client_networkable*	get_client_networkable_from_handle( c_base_handle nettworkable ) = 0;
	virtual i_client_unknown*		get_client_unknown_from_handle( c_base_handle unknown ) = 0;
	virtual i_client_entity*		get_client_entity( int index ) = 0;
	virtual i_client_entity*		get_client_entity_from_handle( c_base_handle entity ) = 0;
	virtual int						number_of_entities( bool include_non_networkable ) = 0;
	virtual int						get_highest_entity_index( ) = 0;
	virtual void					set_max_entities( int max ) = 0;
	virtual int						get_max_entities( ) = 0;

	/* get entity from index */
	template <class t = c_base_entity>
	inline t* get( const int index )
	{
		return static_cast< t* >( get_client_entity( index ) );
	}

	/* get entity from handle */
	template <class t = c_base_entity>
	inline t* get( const c_base_handle handle )
	{
		return static_cast< t* >( get_client_entity_from_handle( handle ) );
	}
};