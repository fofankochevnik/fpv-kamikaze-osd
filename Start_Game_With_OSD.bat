@echo off
start "" "%~dp0OSD_Overlay\FPV_OSD_Overlay.exe"
timeout /t 1 /nobreak >nul
start "" "%~dp0FPVKamikazeDrone\Binaries\Win64\FPVKamikazeDrone-Win64-Shipping.exe"
