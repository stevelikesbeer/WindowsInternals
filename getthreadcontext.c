#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <processthreadsapi.h> // GetContext
#include <errhandlingapi.h> // GetLastError
#include <winbase.h> // FormatMessage

void PrintFormattedErrorMessage(DWORD errorCode);

// windows api calls require the structs to be aligned to 16 bits.
// For some reason (a bug according to github), the LPCONTEXT isn't aligned. Wrap it to align it.
typedef struct {
    LPCONTEXT context;
} __attribute__((aligned(16))) ContextWrapper;

int main(int argc, char* argv[])
{
    HANDLE hThread = GetCurrentThread();
    ContextWrapper contextWrapper;

    if(!GetThreadContext(hThread, contextWrapper.context))
    {
        DWORD lastError = GetLastError();
        puts("failed to get thread context.");
        PrintFormattedErrorMessage(lastError);

        return EXIT_FAILURE;
    }

    printf("%08x ", contextWrapper.context->EFlags); // not sure how to print EFlags in binary. Kind of sucks

    return EXIT_SUCCESS;
}

void PrintFormattedErrorMessage(DWORD errorCode){
        LPTSTR errorMessage;

        if(!FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER + FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, 
            errorCode,
            0,
            (LPTSTR)&errorMessage,
            0,
            NULL
        ))
        {
            puts("Format message failed!");
        }
        else
        {
            printf("\nError Code: %d \nError Message: %s \n", errorCode, errorMessage);
        }

        return;
}