// ******************************************************************************************************
// this file doesn't work because I need to call ntoskrnl.exe directly (and not ntdll.dll like I thought)
//   and can't easily from user mode
// ******************************************************************************************************

#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "EProcessIncludes/EProcess.h"
#include <libloaderapi.h>

void PrintFormattedErrorMessage( DWORD errorCode );
void TestingEPROCESS( HANDLE hProcess );

//NTSTATUS PsLookupProcessByProcessId( HANDLE ProcessId, PEPROCESS *Process );
typedef NTSTATUS( WINAPI *PsLookupProcessByProcessId )( HANDLE ProcessId, PEPROCESS *Process );

int main( int argc, char *argv[] )
{
    LPCTSTR applicationName = "C:\\Program Files\\Notepad++\\notepad++.exe"; // IN [OPTIONAL]
    LPTSTR commandLine = NULL;                                               // IN OUT [OPTIONAL]
    LPSECURITY_ATTRIBUTES processAttributes = NULL;                          // IN [OPTIONAL] //whether the returned handle to the new process object can be inherited by child processes. If NULL the handle cannot be inherited.
    LPSECURITY_ATTRIBUTES threadAttributes = NULL;                           // IN [OPTIONAL] //whether the returned handle to the new thread object can be inherited by child processes. If NULL, the handle cannot be inherited.
    bool shouldInheritHandles = true;                                        // IN
    DWORD creationFlags = 0;                                                 // IN            // 0 means no creation flags. There are enums that you can bitwise OR together for this value.
    LPVOID environment = NULL;                                               // IN [OPTIONAL] // use the environment of the parent process
    LPCTSTR currentDirectory = NULL;                                         // IN [OPTIONAL] // if null, same directory as parent
    STARTUPINFO startupInfo = { sizeof( STARTUPINFO ), 0 };                  // IN
    PROCESS_INFORMATION processInformation = { 0 };                          // OUT

    // I'm not sure why I need any of these next three statements, they're on microsofts example, but the call is successful without them
    //  The ZeroMemory calls aren't needed because we initialize the structs to {0}
    // ZeroMemory( &startupInfo, sizeof(startupInfo) );

    // don't need to initialize cb here because we're already setting the size during initialization
    // startupInfo.cb = sizeof(startupInfo);
    // ZeroMemory( &processInformation, sizeof(processInformation) );

    if( !CreateProcess( applicationName,
                        commandLine,
                        processAttributes,
                        threadAttributes,
                        shouldInheritHandles,
                        creationFlags,
                        environment,
                        currentDirectory,
                        &startupInfo,
                        &processInformation ) )
    {
        DWORD lastError = GetLastError();
        puts( "Creating process failed!" );
        PrintFormattedErrorMessage( lastError );
        return EXIT_FAILURE;
    }

    TestingEPROCESS( processInformation.hProcess );

    WaitForSingleObject( processInformation.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle( processInformation.hProcess );
    CloseHandle( processInformation.hThread );

    return EXIT_SUCCESS;
}

void TestingEPROCESS( HANDLE hProcess )
{
    //HMODULE ntdll = LoadLibrary( "ntdll.dll" );
    //void *test = GetProcAddress( ntdll, "PsLookupProcessByProcessId" );
    //NTSTATUS( *fpPsLookupProcessByProcessId )( HANDLE, PEPROCESS * ) = GetProcAddress( ntdll, "PsLookupProcessByProcessId" );
    //NTSTATUS FARPROC *test( HANDLE, PEPROCESS * ) = GetProcAddress( ntdll, "PsLookupProcessByProcessId" );

    // it turns out PsLookupProcessByProcessId is in ntoskrnl and not ntdll.dll, oh well it fun was trying. This can't be done so easily
    HMODULE ntdll = LoadLibrary( "ntdll.dll" );
    PEPROCESS process;
    PsLookupProcessByProcessId psLookupProcessByProcessId = ( PsLookupProcessByProcessId ) GetProcAddress( ntdll, "PsLookupProcessByProcessId" );
    if( psLookupProcessByProcessId == NULL )
    {
        DWORD errorCode = GetLastError();
        puts( "the pointer is null" );
        PrintFormattedErrorMessage( errorCode );
        return;
    }
    if( psLookupProcessByProcessId( hProcess, &process ) != 0 )
    {
        puts( "Failed to call the psLookupProcessByProcessId function!" );
        return;
    }
    puts( "Successfully called the psLookupProcessByProcessId function!" );
}

void PrintFormattedErrorMessage( DWORD errorCode )
{
    LPTSTR errorMessage;

    if( !FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER + FORMAT_MESSAGE_FROM_SYSTEM,
                        NULL,
                        errorCode,
                        0,
                        ( LPTSTR ) &errorMessage,
                        0,
                        NULL ) )
    {
        puts( "Format message failed!" );
    }
    else
    {
        printf( "\nError Code: %d \nError Message: %s \n", errorCode, errorMessage );
    }
}


