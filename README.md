
### What is it
Simple & Basic Key Logger for windows.

### Compile
1. Install LCC 32 bit  http://www.cs.virginia.edu/~lcc-win32/
2. dowload the attachment
3. open cmd.exe
4. set PATH=%PATH%;c:\lcc\bin
5. cd c:\svchost
6. compile.cmd

### Download Binary
https://raw.github.com/ukoreh/keylogger/master/binary.zip

### Example
![](https://raw.github.com/ukoreh/keylogger/master/keylogger.png) 

### Notes
Note, when installing LCC make sure to disable security essentials from MS
         It was due to "Microsoft Security Essentials" on my computer. I went
         to settings and excluded C:\lcc and reinstalled lcc. Libs are then
         build correctly.

What to expect 

1. execute once to start log keys
2. execute once more to stop log keys
3. the created file is in %windir%\system32\drivers\etc\systems.sam
4. no file roatation
5. tested in XP
