#include "menu.h"

template<std::size_t s>
void c_menu::render_tabs( const char* tabs_id, std::array<c_tabs, s> tabs,int* current_tab )
{ 
	auto& style = ImGui::GetStyle( );
	auto& io = ImGui::GetIO( );

	if ( tabs.empty( ) )
		return;

	if ( ImGui::BeginTabBar( tabs_id, ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_NoTooltip ) )
	{
		for ( std::size_t i = 0U; i < tabs.size( ); i++ )
		{
			// add tab
			ImGui::PushStyleColor( ImGuiCol_Text, *current_tab == i ? ImGui::GetColorU32( ImGuiCol_TabActive ) : ImGui::GetColorU32( ImGuiCol_Text ) );
			if ( ImGui::BeginTabItem( tabs.at( i ).name ) )
			{
				// set current tab index
				*current_tab = i;
				ImGui::EndTabItem( );
			}
			ImGui::PopStyleColor( );
		}
		ImGui::EndTabBar( );
	}

	// render inner tab
	if ( tabs.at( *current_tab ).fn != nullptr )
		tabs.at( *current_tab ).fn( );
}

/* predefine tabs */
void tab_rage( );	void tab_legit( );	void tab_visuals( );	void tab_misc( ); 
void c_menu::run( )
{ 
	auto& style = ImGui::GetStyle( );
	auto& io = ImGui::GetIO( );

	if ( !this->is_opened( ) )
		return;

	ImGui::SetNextWindowSize( { 650,500 }, ImGuiCond_Always );
	ImGui::Begin( "main_menu", &this->open_state, ImGuiWindowFlags_NoDecoration );
	{
		static const std::array<c_tabs, 4U> tabs =
		{
			c_tabs{ "rage", tab_rage },
			c_tabs{ "legit", tab_legit },
			c_tabs{ "visuals", tab_visuals },
			c_tabs{ "misc", tab_misc },
		};

		this->render_tabs<tabs.size( )>( "main_tabs", tabs, &this->current_tab );
	}
	ImGui::End( );
}

void tab_rage( )
{ 
	auto& style = ImGui::GetStyle( );
	auto& io = ImGui::GetIO( );
	static int current_sub_tab = 0;
	ImGui::BeginChild( "sub_tabs", { ImGui::GetContentRegionAvail( ).x / 4.f, 0.f }, true, 0, [] 
		{
			if ( ImGui::BeginListBox( "sub_tabs", ImGui::GetContentRegionAvail( ) ) )
			{
				if ( ImGui::Selectable( "aimbot", current_sub_tab == 0 ) )
					current_sub_tab = 0;

				if ( ImGui::Selectable( "anti-aim", current_sub_tab == 1 ) )
					current_sub_tab = 1;

				if ( ImGui::Selectable( "exploits", current_sub_tab == 2 ) )
					current_sub_tab = 2;

				ImGui::EndListBox( );
			}
		}
	);

	ImGui::SameLine( );

	switch ( current_sub_tab )
	{
		// aimbot
		case 0:
		{
			ImGui::BeginChild( "general", { ImGui::GetContentRegionAvail( ).x / 2.f, 0.f }, true, ImGuiWindowFlags_MenuBar, [ ]
				{

				}
			);

			ImGui::SameLine( );

			ImGui::BeginChild( "advanced", { 0.f, 0.f }, true, ImGuiWindowFlags_MenuBar, [ ]
				{

				}
			);

			break;
		}
	}
}

void tab_legit( )
{

}

void tab_visuals( )
{
	auto& style = ImGui::GetStyle( );
	auto& io = ImGui::GetIO( );
	static int current_sub_tab = 0;
	ImGui::BeginChild( "sub_tabs", { ImGui::GetContentRegionAvail( ).x / 4.f, 0.f }, true, 0, [ ]
		{
			ImGui::ListBoxHeader( "sub_tabs", ImGui::GetContentRegionAvail( ) );
			{
				if ( ImGui::Selectable( "esp", current_sub_tab == 0 ) )
					current_sub_tab = 0;

				if ( ImGui::Selectable( "chams", current_sub_tab == 1 ) )
					current_sub_tab = 1;

				if ( ImGui::Selectable( "effects", current_sub_tab == 2 ) )
					current_sub_tab = 2;
			}
			ImGui::ListBoxFooter( );
		}
	);

	ImGui::SameLine( );

	switch ( current_sub_tab )
	{
		// esp
		case 0:
		{
			ImGui::BeginChild( "general", { ImGui::GetContentRegionAvail( ).x / 2.f, 0.f }, true, ImGuiWindowFlags_MenuBar, [ ]
				{

				}
			);

			ImGui::SameLine( );

			ImGui::BeginChild( "preview", { 0.f, 0.f }, true, ImGuiWindowFlags_MenuBar, [ ]
				{

				}
			);

			break;
		}
	}
}

void tab_misc( )
{
	auto& style = ImGui::GetStyle( );
	auto& io = ImGui::GetIO( );
	static int current_sub_tab = 0;
	ImGui::BeginChild( "sub_tabs", { ImGui::GetContentRegionAvail( ).x / 4.f, 0.f }, true, 0, [ ]
		{
			ImGui::ListBoxHeader( "sub_tabs", ImGui::GetContentRegionAvail( ) );
			{
				if ( ImGui::Selectable( "main", current_sub_tab == 0 ) )
					current_sub_tab = 0;

				if ( ImGui::Selectable( "movement", current_sub_tab == 1 ) )
					current_sub_tab = 1;
			}
			ImGui::ListBoxFooter( );
		}
	);

	ImGui::SameLine( );

	switch ( current_sub_tab )
	{
		// main
		case 0:
		{
			ImGui::BeginChild( "general", { ImGui::GetContentRegionAvail( ).x / 2.f, 0.f }, true, ImGuiWindowFlags_MenuBar, [ ]
				{

				}
			);

			ImGui::SameLine( );

			ImGui::BeginChild( "config", { 0.f, 0.f }, true, ImGuiWindowFlags_MenuBar, [ ]
				{

				}
			);

			break;
		}
	}
}