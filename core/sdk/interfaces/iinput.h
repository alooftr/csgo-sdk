#pragma once

#define multiplayer_backup 150

class i_input
{
	std::byte			pad0[ 0xC ];				// 0x00
public:
	bool				track_ir_available;			// 0x0C
	bool				mouse_initialized;			// 0x0D
	bool				mouse_active;				// 0x0E
private:
	std::byte			pad1[ 0x9A ];				// 0x0F
public:
	bool				camera_in_third_person;		// 0xA9
	std::byte			pad2[ 0x2 ];				// 0xAA
	vec3_t				camera_offset;				// 0xAC
private:
	std::byte			pad3[ 0x38 ];				// 0xB8
public:
	c_user_cmd* commands;							// 0xF0
	c_verified_user_cmd* verified_commands;			// 0xF4

	[[nodiscard]] c_user_cmd* get_user_cmd( const int sequence_number ) const
	{
		return &commands[ sequence_number % multiplayer_backup ];
	}

	[[nodiscard]] c_verified_user_cmd* get_verified_cmd( const int sequence_number ) const
	{
		return &verified_commands[ sequence_number % multiplayer_backup ];
	}
};
static_assert( sizeof( i_input ) == 0xF8 );