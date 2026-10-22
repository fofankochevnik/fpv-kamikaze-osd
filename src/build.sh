set -e
clang++ --target=x86_64-pc-windows-msvc -O2 -std=c++17 -fno-exceptions -fno-rtti -fno-stack-protector -mstack-probe-size=1000000 -fno-builtin -ffreestanding -Wall -Wno-unused-variable -Wno-unused-but-set-variable -Wno-misleading-indentation -c main.cpp -o main.obj
lld-link /nologo /subsystem:windows /entry:Entry /nodefaultlib /out:FPV_OSD_Overlay.exe main.obj lib/kernel32.lib lib/user32.lib lib/gdi32.lib lib/gdiplus.lib /opt:ref
rm -f main.obj
