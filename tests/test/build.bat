@echo off

mkdir build\
pushd build\

del "*.exe"
del "*.obj"

set NAME="test"

..\..\..\tools\ctime -begin build_timings.ctm

cl /nologo /c /Fo%NAME%.obj /Zi /Fd%NAME%.pdb ..\code\main.cpp
link /nologo /DEBUG /OUT:%NAME%.exe %NAME%.obj user32.lib gdi32.lib winmm.lib

REM /SUBSYSTEM:windows /NODEFAULTLIB glew32.lib glu32.lib

..\..\..\tools\ctime -end build_timings.ctm
popd