@SET PATH=%PATH%;c:\local\tcc64
@mkdir bin
tcc -shared -o bin\TopMostDll.dll TopMostDll.c -luser32
@REM tcc -o bin\TopMostMenu.exe main_exe.c -luser32
tcc -o bin\a.exe main_exe.c -luser32
