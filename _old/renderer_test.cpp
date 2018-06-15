
global const char *svg_names[10] = 
{
	"data\\raw\\hero.svg",
	"data\\raw\\cave.svg",
	"data\\raw\\mozilla.svg",
	"data\\raw\\homer.svg",
	"data\\raw\\meditate.svg",
	"data\\raw\\car.svg",
	"data\\raw\\symbol.svg",
	"data\\raw\\gallardo.svg",
	"data\\raw\\logo.svg",
	"data\\raw\\ship.svg",
};
#define DEBUG_FONT_NAME "Segoe UI"
#define DEBUG_FONT_RES 512
global b32 initialized;

struct RendererTestState
{
	SGW world;
	SGWPosition wp;

	VECImage test_images[10];
	V3 p[10];
	V2 s[10];

	V2 mouse_move_start;
};
global RendererTestState *rendertest_state;

internal void
RunRendererTest (RendererState *renderer_state, Input *input, System *system, MemoryPool *permanent_memory, MemoryPool *temporary_memory, r32 dt)
{
	if(!initialized)
	{
		renderer_state->clear_color = V3(0.1f,0.1f,0.1f);

		rendertest_state = AllocateFromMemoryPool(permanent_memory, RendererTestState, 1);
		u8 *pop = PushMemoryPool(temporary_memory);

		for (u32 i = 0; i < 10; ++i)
		{
			RawFileContents rfc = GetRawFileContents(svg_names[i],temporary_memory);
			char *svg_text = (char*)rfc.contents;
			rendertest_state->test_images[i].nsvg_img = nsvgParse(svg_text, "px", 96);
			rendertest_state->test_images[i].flags = VECIMAGE_FLAG_RASTERIZE;
			PopMemoryPool(temporary_memory, pop);

			r32 ratio = (rendertest_state->test_images[i].nsvg_img->width/rendertest_state->test_images[i].nsvg_img->height);
			rendertest_state->p[i] = V3(0.0f,0.0f,0.0f);//V3(-10.0f,0.0f,0.0f) + (V3(2.0f,0.0f,0.0f) * (r32)i);
			rendertest_state->s[i] = V2(1.0f*ratio,1.0f);
		}

		renderer_state->camera.p = V3(0.0f,0.0f,1.0f);
		renderer_state->camera.focus = V3(0.0f,0.0f,0.0f);
		renderer_state->camera.up = V3(0.0f,1.0f,0.0f);
		renderer_state->distance = 1.0f;
		
		renderer_state->font_bag = GetFontBAG("data\\fonts.bag", permanent_memory, temporary_memory);
		renderer_state->debug_font = renderer_state->font_bag.fonts[GetFontIndex(renderer_state->font_bag,DEBUG_FONT_NAME)];
		renderer_state->debug_font_res = DEBUG_FONT_RES;

		SGWBuilder builder = {};	
		SGW_BeginBuild(&builder);
		SGW_RegionsFromSVG("data\\raw\\cave.svg", &builder, {-1,-1,0}, {1,1,0});
		rendertest_state->world = SGW_EndBuild(&builder, permanent_memory);
		rendertest_state->wp = {0,0,0};

		initialized = true;
	}

	Pointer mouse = GetMousePointer(input);
	Camera *camera = &renderer_state->camera;
	SGW *world = &rendertest_state->world;

	RendererEnableDepth();

	Mat4 model_matrix = {};
	Identity(&model_matrix);

	if(WasKeyJustPressed(input, KEY_LEFT_MOUSE_BUTTON))
	{
		rendertest_state->mouse_move_start = ScreenSpaceToCameraSpace(renderer_state, mouse.screen_space_p, system->fullscreen);
	}
	else if(IsKeyDown(input, KEY_LEFT_MOUSE_BUTTON))
	{
		V2 mouse_move_current = ScreenSpaceToCameraSpace(renderer_state, mouse.screen_space_p, system->fullscreen);
		V2 mouse_move_dir = mouse_move_current - rendertest_state->mouse_move_start;
		Normalize(&mouse_move_dir);

		camera->p.x += mouse_move_dir.x * dt * 5.0f;
		camera->p.y += mouse_move_dir.y * dt * 5.0f;
		camera->focus.x = camera->p.x;
		camera->focus.y = camera->p.y;

		rendertest_state->mouse_move_start = mouse_move_current;
	}

	if(IsKeyDown(input, KEY_L_CTRL))
	{
		renderer_state->distance += (mouse.wheel * 0.001f);
		renderer_state->distance = Clamp(0.01f,100.0f,renderer_state->distance);
	}
	else if(WasKeyJustReleased(input, KEY_L_CTRL))
	{
		renderer_state->bitmap_memory.used = 0;
		for (u32 i = 0; i < 1; ++i)
			rendertest_state->test_images[i].flags |= VECIMAGE_FLAG_RASTERIZE;
	}

	Mat4 view_matrix = {};
	Identity(&view_matrix);
	SetLookAt (&view_matrix, 
				camera->p.x,camera->p.y,1.0f,  
				camera->focus.x,camera->focus.y,0.0f,  
				camera->up.x,camera->up.y,camera->up.z);
	view_matrix = view_matrix * model_matrix;
	renderer_state->world_view_matrix = view_matrix;

	Mat4 projection_matrix = {};
	Identity(&projection_matrix);
	r32 ratio = renderer_state->window_ratio;
	// SetFrustum (&projection_matrix, -ratio, ratio, -1.0f, 1.0f, 0.01f, 1.0f);
	SetPerspectiveProjection (&projection_matrix, 0.0f, 10.0f, 90.0f, ratio);
	// SetOrthographicProjection(&projection_matrix,
	//                      		-ratio, ratio, 
	//                      		-1.0f, 1.0f,
	//                      		0.0f, 1.0f);
	renderer_state->world_projection_matrix = projection_matrix;

	renderer_state->wvp = view_matrix * projection_matrix;

	{
		V3 positions[] = 
		{
			V3(-0.5f,0.0f,0.0f),
			V3(+0.5f,0.0f,0.0f),
			V3( 0.0f,+0.5f,0.0f),
			V3( 0.0f,-0.5f,0.0f),
		};
		V3 colors[] = 
		{
			V3(0.2f,1.0f,0.2f),
			V3(0.2f,1.0f,0.2f),
			V3(0.2f,1.0f,0.2f),
			V3(0.2f,1.0f,0.2f),
		};

		V3 mouse_p = ScreenSpaceToWorldSpace(renderer_state, mouse.screen_space_p, system->fullscreen);
		ApplyTransforms(&model_matrix, mouse_p, V3(1.0f,1.0f,1.0f), QUAT_ZERO);
		Mat4 mvp = (model_matrix*view_matrix) * projection_matrix;
		RenderSolidImmediate(renderer_state, mvp, 1.0f, positions, colors, 4, 0,0, LINES);

		// V3 zero = ScreenSpaceToWorldSpace(renderer_state, V2(0.0f,0.0f), system->fullscreen);

		// Resolution res = GetScreenResolution(system);
		// renderer_state->window_width = res.width;
		// renderer_state->window_height = res.height;

		V2 screen_p = WorldSpaceToScreenSpace(renderer_state, mouse_p, system->fullscreen);

		char txt[128];
		sprintf(txt, "sm[%.2f,%.2f] wm[%.2f,%.2f]\nsm[%.2f,%.2f] cz[%.2f]", mouse.screen_space_p.x,mouse.screen_space_p.y,
																   mouse_p.x,mouse_p.y,
																   screen_p.x,screen_p.y,
																   renderer_state->camera.p.z);
		// sprintf(txt, "mw[%.2f,%.2f] ms[%.2f,%.2f]\ncam[%.2f]scr[%u,%u]", mouse_p.x,mouse_p.y,mouse.screen_space_p.x,mouse.screen_space_p.y,camera->p.z,res.width,res.height);
		V2 point = ScreenSpaceToCameraSpace(renderer_state, screen_p, system->fullscreen);
		RenderText (renderer_state, &renderer_state->debug_font, (renderer_state->gui_projection_matrix), txt, DEBUG_FONT_RES, 0.25f, point, V3(1.0f,1.0f,1.0f), 1.0f, permanent_memory, temporary_memory);
	
		ApplyTransforms(&model_matrix, V2_V3(point,0.0f), V3(1.0f,1.0f,1.0f), QUAT_ZERO);
		mvp = (model_matrix*renderer_state->gui_view_matrix)*renderer_state->gui_projection_matrix;
		RenderSolidImmediate(renderer_state, mvp, 1.0f, positions, colors, 4, 0,0, LINES);
	}

	for (u32 i = 0; i < 1; ++i)
	{
		V3 position = rendertest_state->p[i];
		V2 size = rendertest_state->s[i];
		RenderVECImage(renderer_state, position, size, QUAT_ZERO, 1.0f, &rendertest_state->test_images[i], system->fullscreen);
	}

	SGW_GetCurrentRegion(world, &rendertest_state->wp, &rendertest_state->p[0].xy, V2(0.0f,0.0f));
	SGW_UpdateAndRender(renderer_state, world, dt);
}