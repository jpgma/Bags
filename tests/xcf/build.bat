@echo off

mkdir build\
pushd build\

del "*.exe"
del "*.obj"

..\..\..\tools\ctime -begin build_timings.ctm

cl /nologo /c /Foxcf.obj /Zi /Fdxcf.pdb ..\code\main.cpp
link /nologo /DEBUG /OUT:xcf.exe xcf.obj user32.lib gdi32.lib winmm.lib

..\..\..\tools\ctime -end build_timings.ctm
popd