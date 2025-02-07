//return (PEB*)(__readgsqword(0x60))
//__readgsqword(offsetof(NT_TIB, Self));
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <winternl.h>
#include <stdio.h>

int main( int argc, char *argv[] )
{
    // the gs register points to the TEB, providing a 0x60 offset gets us to the address of the PEB
    //      You can look at the TIB (TEB) wikipedia page for more offsets, or windbg `dt nt!_teb`
    PPEB peb = ( PPEB ) __readgsqword( 0x60 );

    printf( "\nCurrent Image Path: %wZ\n", peb->ProcessParameters->ImagePathName );
    printf( "Is the process being debugged: %s\n\n", peb->BeingDebugged ? "True" : "False" );

    printf( "Loaded Module List: \n" );
    PPEB_LDR_DATA ldrData = peb->Ldr;
    PLIST_ENTRY ldrDataHead = &ldrData->InMemoryOrderModuleList;
    PLIST_ENTRY ldrDataCurrent = ldrDataHead;

    ldrDataCurrent = ldrDataCurrent->Flink;
    while( ldrDataCurrent != ldrDataHead ) // it's a circular linked list, make sure we don't loop forever.
    {
        //The CONTAINING_RECORD macro returns the base address of an instance of a structure given the type of the 
        // structure and the address of a field within the containing structure
        // > address [in] 
        //      A pointer to a field in an instance of a structure of type Type.
        // > type [in] 
        //      The name of the type of the structure whose base address is to be returned.
        // > field [in] 
        //      The name of the field pointed to by Address and which is contained in a structure of type Type.

        // In my own words: we have ldrDataCurrent. This is a field in some LDR_DATA_TABLE_ENTRY struct. We want to get a reference to ldrDataCurrent's parent struct.
        //   So, we pass in our ldrDataCurrent (the address of it), we say what the parent struct type is (LDR_DATA_TABLE_ENTRY), 
        //   then we tell the CONTAINING_RECORD function that in the LDR_DATA_TABLE_ENTRY struct, the ldrDataCurrent instance is the field name `InMemoryOrderLinks`
        //   this allows CONTAINING_RECORD to use that data to provide us the base address of ldrDataCurrent's parent (in our case LDR_DATA_TABLE_ENTRY)
        // It's basically taking our ldrDataCurrent, and calculating a negative offset to find the base of the parent
        LDR_DATA_TABLE_ENTRY *entry = ( LDR_DATA_TABLE_ENTRY * ) CONTAINING_RECORD( ldrDataCurrent, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks );
        printf( "Full DLL Name:\t %wZ\n", &entry->FullDllName );
        // There are more fields that we can't see in the enum. See EProcessIncludes/LdrDataTableOffsets.txt

        ldrDataCurrent = ldrDataCurrent->Flink;
    }

    printf( "\n" );

    // trying to read from the PEB via offset since our peb structure just has a bunch of unnamed reserved variables
    //    this actually works.... I get the offsets from windbg `dt nt!_peb`
    char *base = ( char * ) peb;
    size_t offsetMajorVersion = 0x118;
    size_t offsetMinorVersion = 0x11c;
    size_t offsetBuildNumber = 0x120;
    size_t offsetNumberOfProcessors = 0x0b8;
    unsigned int *OSMajorVersion = ( unsigned int * ) ( base + offsetMajorVersion );
    unsigned int *OSMinorVersion = ( unsigned int * ) ( base + offsetMinorVersion );
    unsigned short *OSBuildNumber = ( unsigned short * ) ( base + offsetBuildNumber );
    unsigned int *numberOfProcessors = ( unsigned int * ) ( base + offsetNumberOfProcessors );

    printf( "OS Version: %d.%d Build: %d\n", *OSMajorVersion, *OSMinorVersion, *OSBuildNumber );
    printf( "Number of logical Processors: %d", *numberOfProcessors );

    return EXIT_SUCCESS;
}