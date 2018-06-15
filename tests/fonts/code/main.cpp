
#include <stdio.h>
#include <malloc.h>

#include "..\..\..\definitions.h"
#include "..\..\..\math.h"
#include "..\..\..\platform.h"
#include "..\..\..\strings.cpp"
#include "..\..\..\win32.h"
#include "..\..\..\opengl.h"
#include "..\..\..\fonts.cpp"

#define GAME_TITLE "FONTS"

void main()
{
    const char *filename = "data\\arial.ttf";
    // const char *filename = "data\\trench.otf";

    b32 supported = ParseOpentypeFont(filename);

    printf("\n\"%s\" is%ssupported\n", filename, (supported ? " " : " not "));
}

#include "..\..\..\math.cpp"
#include "..\..\..\platform.cpp"
#include "..\..\..\win32.cpp"
