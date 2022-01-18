@echo off


rem /Oi use intrinsics, /Od Disables optimization, /O1 creates small code, /O2 creates fast code
rem /Gm rebuilds more often if src changes
rem /Zi create complete debug info, /Z7 creates c 7.0 debugging info
rem /Ge enables stack probes, /Gs enables probes for every function
rem /MT compiles with multithreated static runtime lib, /MTd with debug version of that runtime lib
rem /GR generates run time info
rem /EH uses exception handling, EHa- turns it all off
rem /FC use full path of source code files
rem /W4 show most warnings, /Wall show all warnings, /WX treat all warnings as errors
rem /Fm create mapfile

set debug_flags=/Zi /Oi /Od /Gs /Fm 
set disable_flags=/D_CRT_SECURE_NO_WARNINGS /GR- /EHa- /nologo /wd4201 /wd4505 /wd4100 /wd4189 /wd4456 /wd4324

set cflags=/MT /Gm- /FC /W4 /WX %debug_flags% %disable_flags% 
set lflags=/link /opt:ref /incremental:no
set imports=user32.lib Gdi32.lib winmm.lib opengl32.lib


IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

cl %cflags% /Fe:main.exe ..\code\main.cpp %lflags% %imports%

popd
