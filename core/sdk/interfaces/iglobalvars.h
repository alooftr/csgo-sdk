#pragma once

class i_global_vars_base
{
public:
	float			real_time;					//0x00
	int				frame_count;				//0x04
	float			abs_frame_time;				//0x08
	float			abs_frame_start_time;		//0x0C
	float			current_time;				//0x10
	float			frame_time;				//0x14
	int				max_clients;				//0x18
	int				tick_count;					//0x1C
	float			interval_per_tick;			//0x20
	float			interpolation_amount;		//0x24
	int				frame_simulation_ticks;		//0x28
	int				network_protocol;			//0x2C
	void* save_data;					//0x30
	bool			client;					//0x34
	bool		    remote_client;				//0x35
	int				timestamp_networking_base;	//0x36
	int				timestamp_randomize_window;	//0x3A
}; // Size: 0x3E