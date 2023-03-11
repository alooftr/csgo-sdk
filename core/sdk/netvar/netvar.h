#pragma once
// used: for output file
#include <fstream>
// used: for map props
#include <unordered_map>

// used: data struct
#include "../data_types/map.h"
#include "../data_types/table.h"
// used: hashing
#include "../../utilities/fnv1a.h"

class c_netvar_manager
{
public:
	struct netvar_object_t
	{
		recv_prop_t* recv_prop = nullptr;
		std::uintptr_t offset = 0U;
	};

	// get
	/* fill map with netvars and also dump it to given file */
	bool setup( const char* file_name );
	/*
	 * stores the variables of objects in the hierarchy
	 * used to iterate through an object's data descriptions from data map
	 */
	std::uintptr_t find_in_data_map( data_map_t* map, const fnv1a_t field_hash );

	// Values
	/* logging counters */
	int stored_props = 0;
	int stored_tables = 0;
	/* networkable properties map */
	std::unordered_map<fnv1a_t, netvar_object_t> map_props = { };

	// Extra
	std::string get_property_type( const recv_prop_t* recv_prop ) const;
private:
	/*
	 * recursively stores networked properties info from data tables in our map
	 * and also format our dump and write values to file
	 */
	void store_props( const char* class_name, recv_table_t* recv_table, const std::uintptr_t offset, const int depth );

	// Values
	/* output file */
	std::ofstream dump_file = { };
};

inline const std::unique_ptr<c_netvar_manager> g_netvar_manager{ new c_netvar_manager( ) };

#pragma region netvar_definitions
/* add function to get variable with additional offset from netvar offset */
#define n_add_variable_offset( type, function_name, net_var, additional )								\
	[[nodiscard]] std::add_lvalue_reference_t<type> function_name()										\
	{																										\
		static constexpr fnv1a_t hash = fnv1a::hash_const(net_var);										\
		static std::uintptr_t offset = g_netvar_manager->map_props[hash].offset;						\
		return *(std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset + additional);	\
	}

/* add function to get netvar variable */
#define n_add_variable( type, function_name, net_var ) n_add_variable_offset( type, function_name, net_var, 0U )

/* add function to get variable pointer with additional offset from netvar offset */
#define n_add_pvariable_offset( type, function_name, net_var, additional )								\
	[[nodiscard]] std::add_pointer_t<type> function_name()													\
	{																										\
		static constexpr fnv1a_t hash = fnv1a::hash_const(net_var);										\
		static std::uintptr_t offset = g_netvar_manager->map_props[hash].offset;						\
		return (std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset + additional);	\
	}

/* add function to get netvar variable pointer */
#define n_add_pvariable( type, function_name, net_var ) n_add_pvariable_offset( type, function_name, net_var, 0U )

/* add function to get csplayerresource variable for entity by index from netvar offset */
#define n_add_resource_variable( type, function_name, net_var )														\
	[[nodiscard]] std::add_lvalue_reference_t<type> function_name(int index)											\
	{																													\
		static constexpr fnv1a_t hash = fnv1a::hash_const(net_var);													\
		static std::uintptr_t offset = g_netvar_manager->map_props[hash].offset;									\
		return *(std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset + index * sizeof(type));	\
	}

/* add function to get datamap variable */
#define n_add_datafield( type, function_name, map, field )											\
	[[nodiscard]] std::add_lvalue_reference_t<type> function_name()										\
	{																										\
		static constexpr fnv1a_t hash = fnv1a::hash_const(field);										\
		static std::uintptr_t offset = g_netvar_manager->find_in_data_map(map, hash);					\
		return *(std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset);				\
	}

/* add function to get datamap variable pointer */
#define n_add_pdatafield( type, function_name, map, field )											\
	[[nodiscard]] std::add_pointer_t<type> function_name()													\
	{																										\
		static constexpr fnv1a_t hash = fnv1a::hash_const(field);										\
		static std::uintptr_t offset = g_netvar_manager->find_in_data_map(map, hash);					\
		return (std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset);				\
	}

/* add function to get variable by offset */
#define n_add_offset( type, function_name, offset )														\
	[[nodiscard]] std::add_lvalue_reference_t<type> function_name()										\
	{																										\
		return *(std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset);				\
	}

/* add function to get variable pointer by offset */
#define n_add_poffset( type, function_name, offset )														\
	[[nodiscard]] std::add_pointer_t<type> function_name()													\
	{																										\
		return (std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset);				\
	}
#pragma endregion