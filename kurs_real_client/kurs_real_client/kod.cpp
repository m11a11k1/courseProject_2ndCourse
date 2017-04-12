#include <windows.h>
#include <stdio.h>
#include <tchar.h>

void _tmain( )
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
	int a[20];
	TCHAR argv[20];
	_tcscpy( argv, _T("kurs_client.exe"));
    if( !CreateProcess( NULL,   
        argv,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        CREATE_NO_WINDOW,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return;
    }

    // Close process and thread handles. 

    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}