#include "memory.h"
// used PEB32
#include "pe32.h"
// used: logging namespace
#include "logging.h"

bool c_game_modules::setup( )
{
    /* modules list */
    modules[ engine_dll ] = c_memory::get_module_handle( engine_dll );
    modules[ client_dll ] = c_memory::get_module_handle( client_dll );
    modules[ localize_dll ] = c_memory::get_module_handle( localize_dll );
    modules[ materialsystem_dll ] = c_memory::get_module_handle( materialsystem_dll );
    modules[ vgui_dll ] = c_memory::get_module_handle( vgui_dll );
    modules[ vgui2_dll ] = c_memory::get_module_handle( vgui2_dll );
    modules[ shaderpidx9_dll ] = c_memory::get_module_handle( shaderpidx9_dll );
    modules[ gameoverlayrenderer_dll ] = c_memory::get_module_handle( gameoverlayrenderer_dll );
    modules[ physics_dll ] = c_memory::get_module_handle( physics_dll );
    modules[ vstdlib_dll ] = c_memory::get_module_handle( vstdlib_dll );
    modules[ tier0_dll ] = c_memory::get_module_handle( tier0_dll );
    modules[ inputsystem_dll ] = c_memory::get_module_handle( inputsystem_dll );
    modules[ studiorender_dll ] = c_memory::get_module_handle( studiorender_dll );
    modules[ datacache_dll ] = c_memory::get_module_handle( datacache_dll );
    modules[ steam_api_dll ] = c_memory::get_module_handle( steam_api_dll );
    modules[ matchmaking_dll ] = c_memory::get_module_handle( matchmaking_dll );
    modules[ server_dll ] = c_memory::get_module_handle( server_dll );
    modules[ serverbrowser_dll ] = c_memory::get_module_handle( serverbrowser_dll );

    return !modules.empty( );
}

module_handle_info_t c_memory::get_module_handle( const std::string_view module_name )
{
    const _PEB32* peb = reinterpret_cast< _PEB32* >( __readfsdword( 0x30 ) ); 

    if ( module_name.empty( ) )
        return { "image_base_address", peb->ImageBaseAddress };

    const std::wstring wide_module_name( module_name.begin( ), module_name.end( ) );

    for ( LIST_ENTRY* list_entry = peb->Ldr->InLoadOrderModuleList.Flink; list_entry != &peb->Ldr->InLoadOrderModuleList; list_entry = list_entry->Flink )
    {
        const _LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD( list_entry, _LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );

        if ( entry->BaseDllName.Buffer && wide_module_name.compare( entry->BaseDllName.Buffer ) == 0 )
            return { module_name.data( ), entry->DllBase };
    }

    debug_log_error( "[error] module base not found: [{}]", module_name );

    return { nullptr, nullptr };
}

void* c_memory::get_proc_address( const void* module_base, const std::string_view procedure_name )
{
    const std::uint8_t* address = static_cast< const std::uint8_t* >( module_base );
    const IMAGE_DOS_HEADER* dos_header = static_cast< const IMAGE_DOS_HEADER* >( module_base );
    const IMAGE_NT_HEADERS* nt_headers = reinterpret_cast< const IMAGE_NT_HEADERS* >( address + dos_header->e_lfanew );
    const IMAGE_OPTIONAL_HEADER* optional_header = &nt_headers->OptionalHeader;

    const std::uintptr_t export_size = optional_header->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].Size;
    const std::uintptr_t export_address = optional_header->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress;

    if ( export_size <= 0U )
        return nullptr;

    const IMAGE_EXPORT_DIRECTORY* export_directory = reinterpret_cast< const IMAGE_EXPORT_DIRECTORY* >( address + export_address );
    const std::uintptr_t* names_rva = reinterpret_cast< const std::uintptr_t* >( address + export_directory->AddressOfNames );
    const std::uintptr_t* functions_rva = reinterpret_cast< const std::uintptr_t* >( address + export_directory->AddressOfFunctions );
    const std::uint16_t* name_ordinals = reinterpret_cast< const std::uint16_t* >( address + export_directory->AddressOfNameOrdinals );

    // perform binary search
    std::uintptr_t right = export_directory->NumberOfNames;
    std::uintptr_t left = 0;

    while ( right != left )
    {
        const std::uintptr_t middle = left + ( ( right - left ) >> 1U );
        const int result = procedure_name.compare( reinterpret_cast< const char* >( address + names_rva[ middle ] ) );

        if ( result == 0 )
            return const_cast< void* >( static_cast< const void* >( address + functions_rva[ name_ordinals[ middle ] ] ) );

        if ( result > 0 )
            left = middle;
        else
            right = middle;
    }

    debug_log_error( "[error] export not found: [{}]", procedure_name );

    return nullptr;
}

std::vector<std::optional<std::uint8_t>> c_memory::pattern_to_bytes( const std::string_view pattern )
{
    std::vector<std::optional<std::uint8_t>> bytes = { };
    auto begin = pattern.cbegin( );
    const auto end = pattern.cend( );

    // convert pattern into bytes
    while ( begin < end )
    {
        // check is current byte a wildcard
        if ( *begin == '?' )
        {
            // check is two-character wildcard
            if ( ++begin; *begin == '?' )
                ++begin;

            // ignore that
            bytes.emplace_back( std::nullopt );
        }
        // check is not space
        else if ( *begin != ' ' )
        {
            // convert current 4 bits to hex
            std::uint8_t byte = static_cast< std::uint8_t >( ( ( *begin >= 'A' ? ( ( ( *begin - 'A' ) & ( ~( 'a' ^ 'A' ) ) ) + 10 ) : ( *begin <= '9' ? *begin - '0' : 0x0 ) ) | 0xF0 ) << 4 );

            // convert next 4 bits to hex and assign to byte
            if ( ++begin; *begin != ' ' )
                byte |= static_cast< std::uint8_t >( *begin >= 'A' ? ( ( ( *begin - 'A' ) & ( ~( 'a' ^ 'A' ) ) ) + 10 ) : ( *begin <= '9' ? *begin - '0' : 0x0 ) );

            bytes.emplace_back( byte );
        }

        ++begin;
    }

    return bytes;
}

std::string c_memory::bytes_to_pattern( const std::uint8_t* bytes, const std::size_t size )
{
    constexpr const char* hex_digits = "0123456789ABCDEF";
    const std::size_t hex_length = ( size << 1U ) + size;

    // construct pre-reserved string filled with spaces
    std::string pattern( hex_length - 1U, ' ' );

    for ( std::size_t i = 0U, n = 0U; i < hex_length; n++, i += 3U )
    {
        const std::uint8_t current_byte = bytes[ n ];

        // manually convert byte to chars
        pattern[ i ] = hex_digits[ ( ( current_byte & 0xF0 ) >> 4 ) ];
        pattern[ i + 1U ] = hex_digits[ ( current_byte & 0x0F ) ];
    }

    return pattern;
}

std::uintptr_t c_memory::find_pattern( const std::string_view module_name, const std::string_view pattern )
{
    void* module_base = get_module_handle( module_name.data( ) ).handle;

    if ( module_base == nullptr )
        throw std::runtime_error( std::vformat( "failed to get handle for: {}", std::make_format_args( module_name ) ) );

    const std::uint8_t* module_address = static_cast< std::uint8_t* >( module_base );
    const IMAGE_DOS_HEADER* dos_header = static_cast< IMAGE_DOS_HEADER* >( module_base );
    const IMAGE_NT_HEADERS* nt_headers = reinterpret_cast< const IMAGE_NT_HEADERS* >( module_address + dos_header->e_lfanew );

    debug_log( "finding pattern [{}] from [{}]", pattern, module_name.data( ) );
    return find_pattern( module_address, nt_headers->OptionalHeader.SizeOfImage, pattern );
}


std::uintptr_t c_memory::find_pattern( const std::uint8_t* region_start, const std::uintptr_t region_size, const std::string_view pattern )
{
    const std::vector<std::optional<std::uint8_t>> bytes = pattern_to_bytes( pattern );

    // check for bytes sequence match
    for ( std::uintptr_t i = 0U; i < region_size - bytes.size( ); ++i )
    {
        bool found = true;

        for ( std::uintptr_t s = 0U; s < bytes.size( ); ++s )
        {
            // compare byte and skip if wildcard
            if ( bytes[ s ].has_value( ) && region_start[ i + s ] != bytes[ s ].value( ) )
            {
                found = false;
                break;
            }
        }

        // return valid address
        if ( found )
        {
            debug_log_ok( "found [{}] -> [{:#08x}]", pattern, reinterpret_cast< std::uintptr_t >( &region_start[ i ] ) );
            return reinterpret_cast< std::uintptr_t >( &region_start[ i ] );
        }
    }

    debug_log_error( "pattern not found: [{}]", pattern );
    return 0U;
}

std::vector<std::uintptr_t> c_memory::get_cross_references( const std::uintptr_t address, std::uintptr_t region_start, const std::size_t region_size )
{
    std::vector<std::uintptr_t> cross_references = { };

    // convert the address over to an ida pattern string
    const std::string pattern = bytes_to_pattern( reinterpret_cast< const std::uint8_t* >( &address ), sizeof( std::uintptr_t ) );
    // get the end of the section (in our case the end of the .rdata section)
    const std::uintptr_t region_end = region_start + region_size;

    while ( region_start > 0U && region_start < region_end )
    {
        // @todo: findpattern shouldn't report on fail
        std::uintptr_t reference_address = find_pattern( reinterpret_cast< std::uint8_t* >( region_start ), region_size, pattern.c_str( ) );

        // if the xref is 0 it means that there either were no xrefs, or there are no remaining xrefs
        if ( reference_address == 0U )
            break;

        cross_references.push_back( reference_address );

        // skip current xref for next search
        region_start = reference_address + sizeof( std::uintptr_t );
    }

    return cross_references;
}

bool c_memory::get_section_info( const std::uintptr_t base_address, const std::string_view section_name, std::uintptr_t* section_start, std::uintptr_t* section_size )
{
    const IMAGE_DOS_HEADER* dos_header = reinterpret_cast< IMAGE_DOS_HEADER* >( base_address );
    if ( dos_header->e_magic != IMAGE_DOS_SIGNATURE )
        return false;

    const IMAGE_NT_HEADERS* nt_headers = reinterpret_cast< IMAGE_NT_HEADERS* >( base_address + dos_header->e_lfanew );
    if ( nt_headers->Signature != IMAGE_NT_SIGNATURE )
        return false;

    IMAGE_SECTION_HEADER* section_header = IMAGE_FIRST_SECTION( nt_headers );
    std::uint16_t number_of_sections = nt_headers->FileHeader.NumberOfSections;

    while ( number_of_sections > 0U )
    {
        // if we're at the right section
        if ( section_name.starts_with( reinterpret_cast< const char* >( section_header->Name ) ) )
        {
            if ( section_start != nullptr )
                *section_start = base_address + section_header->VirtualAddress;

            if ( section_size != nullptr )
                *section_size = section_header->SizeOfRawData;

            return true;
        }

        section_header++;
        number_of_sections--;
    }

    return false;
}

std::uintptr_t c_memory::get_vtable_type_descriptor( const std::string_view module_name, const std::string_view table_name )
{
    // type descriptor names look like this: '.?AVC_CSPlayer@@'
    const std::string type_descriptor_name = std::string( ".?AV" ).append( table_name ).append( "@@" );
    const std::string pattern = bytes_to_pattern( reinterpret_cast< const std::uint8_t* >( type_descriptor_name.data( ) ), type_descriptor_name.size( ) );

    // get rtti type descriptor, located 0x8 bytes before the string
    if ( const std::uintptr_t uTypeDescriptor = find_pattern( module_name, pattern ); uTypeDescriptor != 0U )
        return uTypeDescriptor - 0x8;

    debug_log_error( "[error] virtual table type descriptor not found: [{}] [{}]", module_name, table_name );

    return 0U;
}

std::uintptr_t* c_memory::get_vtable_pointer( const std::string_view module_name, const std::string_view table_name )
{
    const std::uintptr_t base_address = reinterpret_cast< std::uintptr_t >( get_module_handle( module_name ).handle );

    if ( base_address == 0U )
        return nullptr;

    const std::uintptr_t type_descriptor = get_vtable_type_descriptor( module_name, table_name );

    if ( type_descriptor == 0U )
        return nullptr;

    // we only need to get xrefs that are inside the .rdata section (there sometimes are xrefs in .text, so we have to filter them out)
    std::uintptr_t r_data_start = 0U, r_data_size = 0U;
    if ( !get_section_info( base_address, xor_str( ".rdata" ), &r_data_start, &r_data_size ) )
        return nullptr;

    // go through all xrefs of the type descriptor
    for ( const auto& cross_reference : get_cross_references( type_descriptor, r_data_start, r_data_size ) )
    {
        // get offset of vtable in complete class, 0 means it's the class we need, and not some class it inherits from
        if ( const int vtable_offset = *reinterpret_cast< int* >( cross_reference - 0x8 ); vtable_offset != 0 )
            continue;

        // get the object locator
        const std::uintptr_t object_locator = cross_reference - 0xC;

        // get a pointer to the vtable
        std::string pattern = bytes_to_pattern( reinterpret_cast< const std::uint8_t* >( &object_locator ), sizeof( std::uintptr_t ) );
        const std::uintptr_t vtable_address = find_pattern( reinterpret_cast< std::uint8_t* >( r_data_start ),r_data_size, pattern.c_str( ) ) + 0x4;

        // check is valid offset
        if ( vtable_address <= sizeof( std::uintptr_t ) )
            return nullptr;

        // check for .text section
        std::uintptr_t text_start = 0U, text_size = 0U;
        if ( !get_section_info( base_address, xor_str( ".text" ), &text_start, &text_size ) )
            return nullptr;

        // convert the vtable address to an ida pattern
        pattern = bytes_to_pattern( reinterpret_cast< const std::uint8_t* >( &vtable_address ), sizeof( std::uintptr_t ) );
        return reinterpret_cast< std::uintptr_t* >( find_pattern( reinterpret_cast< std::uint8_t* >( text_start ), text_size, pattern.c_str( ) ) );
    }

    debug_log_error( "[error] virtual table pointer not found: [{}] [{}]", module_name, table_name );

    return nullptr;
}

