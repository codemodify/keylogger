@echo off

:preCleanup()
    cls
    set fileName=svchost
    rem set libs=user32.lib kernel32.lib
    del %fileName%.exe 2>nul
    del %fileName%.obj 2>nul

:build()
    lcc -c -Ic:\lcc\include -A %fileName%.c
    if ERRORLEVEL 1 goto postCleanup()
    lcclnk -subsystem windows -s %fileName%.obj %libs% -o %fileName%.exe

:postCleanup()
    del %fileName%.obj 2>nul

@echo on
