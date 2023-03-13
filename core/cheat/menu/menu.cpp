#include "menu.h"

template<std::size_t s>
void c_menu::render_tabs( const char* tabs_id, std::array<c_tabs, s> tabs,int* current_tab, int old_current_tab )
{ 
	auto& style = ImGui::GetStyle( );
	auto& io = ImGui::GetIO( );

	if ( tabs.empty( ) )
		return;

	if ( ImGui::BeginTabBar( tabs_id, ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_NoTooltip ) )
	{
		for ( std::size_t i = 0U; i < tabs.size( ); i++ )
		{
			// add tab
			ImGui::PushStyleColor( ImGuiCol_Text, old_current_tab == i ? ImGui::GetColorU32( ImGuiCol_TabActive ) : ImGui::GetColorU32( ImGuiCol_Text ) );
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

	static float tab_alpha = 1.f;
	if ( old_current_tab != *current_tab )
	{
		tab_alpha = ImLerp( tab_alpha, 0.f, io.DeltaTime * style.AnimationScale );
		if ( tab_alpha <= 0.1f )
			*current_tab = old_current_tab;
	}
	else
	{
		tab_alpha = ImLerp( tab_alpha, 1.f, io.DeltaTime * style.AnimationScale );
	}

	ImGui::PushStyleVar( ImGuiStyleVar_Alpha, tab_alpha );

	// render inner tab
	if ( tabs.at( *current_tab ).fn != nullptr )
		tabs.at( *current_tab ).fn( );

	ImGui::PopStyleVar( );
}

/* predefine tabs */
void tab_rage( );
void tab_legit( );
void tab_visuals( );
void tab_misc( ); 

void c_menu::run( )
{ 
	auto& style = ImGui::GetStyle( );
	auto& io = ImGui::GetIO( );

	static bool old_state = this->open_state;
	static float menu_alpha = 1.f;

	if ( old_state != this->open_state )
	{
		menu_alpha = ImLerp( menu_alpha, 0.f, io.DeltaTime * style.AnimationScale );
		if ( menu_alpha <= 0.1f )
			old_state = this->open_state;
	}
	else
	{
		menu_alpha = ImLerp( menu_alpha, 1.f, io.DeltaTime * style.AnimationScale );
	}

	if ( menu_alpha <= 0.1f )
		return;

	ImGui::Begin( "main_menu", &this->open_state, ImGuiWindowFlags_NoDecoration );
	{
		static const std::array<c_tabs, 4U> tabs =
		{
			c_tabs{ "rage", tab_rage },
			c_tabs{ "legit", tab_legit },
			c_tabs{ "visuals", tab_visuals },
			c_tabs{ "misc", tab_misc },
		};

		this->render_tabs<tabs.size( )>( "main_tabs", tabs, &this->current_tab, this->old_current_tab );
	}
	ImGui::End( );
}

void tab_rage( )
{ 

}

void tab_legit( )
{

}

void tab_visuals( )
{

}

void tab_misc( )
{

}