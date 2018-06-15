
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define GAME_TITLE "MetaSerialize"
// #define DEBUG_BUILD true
// #define OPENGL_RENDERER true

#include "..\bags.h"
// #include "..\definitions.h"
// #include "..\math.h"
// #include "..\platform.h"
// #include "..\memory.h"
// #include "..\memory.cpp"
#include "..\win32.cpp"

int main(int argc, char *argv[])
{
	if(argc >= 4)
	{
		MemoryPool memory;
		InitializeMemoryPool(&memory, MEGABYTES(2));

		RawFileContents src_file = GetRawFileContents(argv[0], &memory);
		RawFileContents dst_file = GetRawFileContents(argv[2], &memory);

		if(src_file.contents && dst_file.contents)
		{
			char *src_text = (char *)src_file.contents;
			printf("\n-----------------\nsrc: %s\n-----------------\n", src_text);
			
		}

		FreeMemoryPool(&memory);
	}
	else printf("Chamada incorreta!\nmeta_serialize \"arquivo_fonte\" \"nome_do_struct\" \"arquivo_destino\" \"linha_destino\"");

	return 0;
}
