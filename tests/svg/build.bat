@echo off

mkdir build\
pushd build\

del "*.exe"
del "*.obj"

..\..\..\tools\ctime -begin build_timings.ctm

cl /nologo /c /Fosvg.obj /Zi /Fdsvg.pdb ..\code\main.cpp
link /nologo /DEBUG /OUT:svg.exe svg.obj user32.lib gdi32.lib winmm.lib

REM /SUBSYSTEM:windows /NODEFAULTLIB glew32.lib glu32.lib

..\..\..\tools\ctime -end build_timings.ctm
popd