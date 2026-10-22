@echo off
cd /d "%~dp0"
call "D:\PROGRAMME\visual studio\VC\Auxiliary\Build\vcvars64.bat" >nul
cl /nologo /O2 /GS- /Gs1000000 /EHs-c- /GR- /c main.cpp
link /nologo /subsystem:windows /entry:Entry /nodefaultlib main.obj kernel32.lib user32.lib gdi32.lib gdiplus.lib /out:FPV_OSD_Overlay.exe
del main.obj
pause
