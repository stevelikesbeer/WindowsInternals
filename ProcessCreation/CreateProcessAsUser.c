#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

void PrintFormattedErrorMessage( DWORD errorCode );
bool SetPrivilege( HANDLE hToken, LPCTSTR privilege, bool isEnabled );

// this doesn't work because I don't know enough about tokens yet
int main( int argc, char *argv[] )
{
    HANDLE userToken = NULL;
    if( !OpenProcessToken( GetCurrentProcess(), TOKEN_READ, &userToken ) )
    {
        DWORD errorCode = GetLastError();
        puts( "Could not read the current user token!" );
        PrintFormattedErrorMessage( errorCode );
        return EXIT_FAILURE;
    }

    if( !SetPrivilege( userToken, SE_INCREASE_QUOTA_NAME, true ) )
    {
        DWORD errorCode = GetLastError();
        puts( "Could not assign SE_INCREASE_QUOTA_NAME privilege!" );
        PrintFormattedErrorMessage( errorCode );
        return EXIT_FAILURE;
    }

    // if( !SetPrivilege( userToken, SE_ASSIGNPRIMARYTOKEN_NAME, true ) )
    // {
    //     DWORD errorCode = GetLastError();
    //     puts( "Could not assign SE_ASSIGNPRIMARYTOKEN_NAME privilege!" );
    //     PrintFormattedErrorMessage( errorCode );
    //     return EXIT_FAILURE;
    // }

    LPCTSTR applicationName = "C:\\Program Files\\Notepad++\\notepad++.exe";
    LPTSTR commandLine = NULL;
    LPSECURITY_ATTRIBUTES processAttributes = NULL;
    LPSECURITY_ATTRIBUTES threadAttributes = NULL;
    bool shouldInheritHandles = true;
    DWORD creationFlags = 0;
    LPVOID environment = NULL;
    LPCTSTR currentDirectory = NULL;
    STARTUPINFO startupInfo = { sizeof( STARTUPINFO ),0 };
    PROCESS_INFORMATION processInformation = { 0 };

    if( !CreateProcessAsUser( userToken,
                              applicationName,
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
        DWORD errorCode = GetLastError();
        puts( "Could not create process!" );
        PrintFormattedErrorMessage( errorCode );
        return EXIT_FAILURE;
    }

    WaitForSingleObject( processInformation.hProcess, INFINITE );

    CloseHandle( processInformation.hProcess );
    CloseHandle( processInformation.hThread );

    return EXIT_SUCCESS;
}

bool SetPrivilege( HANDLE hToken, LPCTSTR privilege, bool isEnabled )
{
    TOKEN_PRIVILEGES tokenPrivileges;
    LUID luid;
    if( !LookupPrivilegeValue( NULL,        // look up privilege on local system
                               privilege,   // name of the privilege
                               &luid ) )    // recieves the local uid of the privilege
    {
        DWORD errorCode = GetLastError();
        puts( "Could not find privilege!" );
        PrintFormattedErrorMessage( errorCode );
        return false;
    }

    tokenPrivileges.PrivilegeCount = 1;
    tokenPrivileges.Privileges[0].Luid = luid;

    if( isEnabled )
    {
        tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    }
    else
    {
        tokenPrivileges.Privileges[0].Attributes = 0;
    }

    if( !AdjustTokenPrivileges( hToken,
                                false,
                                &tokenPrivileges,
                                sizeof( TOKEN_PRIVILEGES ),
                                ( PTOKEN_PRIVILEGES ) NULL,
                                ( PDWORD ) NULL
    ) )
    {
        DWORD errorCode = GetLastError();
        puts( "Could not adjust token privileges!" );
        PrintFormattedErrorMessage( errorCode );
        return false;
    }

    if( GetLastError() == ERROR_NOT_ALL_ASSIGNED )
    {
        puts( "The token does not have the specific privilege!" );
        return false;
    }

    return true;
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