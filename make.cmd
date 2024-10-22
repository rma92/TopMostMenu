REM @SET PATH=%PATH%;c:\local\tcc64
@if not exist bin64 mkdir bin64
@if not exist bin32 mkdir bin32
@REM tcc -o bin\TopMostMenu.exe main_exe.c -luser32
C:\local\tcc64\tcc -shared -o bin64\TopMostDll.dll TopMostDll.c -luser32
C:\local\tcc64\tcc -o bin64\a.exe main_exe.c -luser32

C:\local\tcc32\tcc -shared -o bin32\TopMostDll.dll TopMostDll.c -luser32
C:\local\tcc32\tcc -o bin32\a.exe main_exe.c -luser32

