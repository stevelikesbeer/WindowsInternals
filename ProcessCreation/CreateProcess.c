#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

void PrintFormattedErrorMessage( DWORD errorCode );

int main( int argc, char* argv[] )
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

    if ( !CreateProcess( applicationName,
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

    WaitForSingleObject( processInformation.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle( processInformation.hProcess );
    CloseHandle( processInformation.hThread );

    return EXIT_SUCCESS;
}

void PrintFormattedErrorMessage( DWORD errorCode )
{
    LPTSTR errorMessage;

    if ( !FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER + FORMAT_MESSAGE_FROM_SYSTEM,
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