#include "memory.h"
// used PEB32
#include "pe32.h"
// used: logging namespace
#include "logging.h"


module_handle_info_t c_memory::get_module_handle( const std::string_view module_name )
{
    const _PEB32* peb = reinterpret_cast< _PEB32* >( __readfsdword( 0x30 ) ); // mov eax, fs:[0x30]
    //const _TEB32* pTEB = reinterpret_cast<_TEB32*>(__readfsdword(0x18)); // mov eax, fs:[0x18]
    //const _PEB32* pPEB = pTEB->ProcessEnvironmentBlock;

    if ( module_name.empty( ) )
        return { "image_base_address", peb->ImageBaseAddress };

    const std::wstring wide_module_name( module_name.begin( ), module_name.end( ) );

    for ( LIST_ENTRY* list_entry = peb->Ldr->InLoadOrderModuleList.Flink; list_entry != &peb->Ldr->InLoadOrderModuleList; list_entry = list_entry->Flink )
    {
        const _LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD( list_entry, _LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );

        if ( entry->BaseDllName.Buffer && wide_module_name.compare( entry->BaseDllName.Buffer ) == 0 )
            return { module_name.data( ), entry->DllBase };
    }

    error_log( "[error] module base not found: [{}]", module_name );

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

    error_log( "[error] export not found: [{}]", procedure_name );

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

std::uintptr_t c_memory::find_pattern( const std::string_view module_name, const std::string_view pattern )
{
    void* module_base = get_module_handle( module_name.data( ) ).handle;

    if ( module_base == nullptr )
        throw std::runtime_error( std::vformat( "failed to get handle for: {}", std::make_format_args( module_name ) ) );

    const std::uint8_t* module_address = static_cast< std::uint8_t* >( module_base );
    const IMAGE_DOS_HEADER* dos_header = static_cast< IMAGE_DOS_HEADER* >( module_base );
    const IMAGE_NT_HEADERS* nt_headers = reinterpret_cast< const IMAGE_NT_HEADERS* >( module_address + dos_header->e_lfanew );

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
            debug_log( "pattern [{}] found -> [{:#08x}]", pattern, reinterpret_cast< std::uintptr_t >( &region_start[ i ] ) );
            return reinterpret_cast< std::uintptr_t >( &region_start[ i ] );
        }
    }

    error_log( "pattern not found: [{}]", pattern );
    return 0U;
}

