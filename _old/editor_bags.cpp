
struct EditorState
{
	b32 initialzed;
	u32 click_sound_id;

	GUIState gui_state;

	GUIWindow *editor_window;

	GUIWindow *assetm_window;
	GUIList *level_svg_list;
	GUIListItem *first_level_svg;
	GUISolidButton *add_button, *rem_button;
};
global EditorState *GLOBAL_EDITORSTATE_P;

global r32 runtime_milliseconds;

global GUITheme LIGHT_THEME = {V3(1.0f,1.0f,1.0f), V3(0.5f,0.5f,0.5f), V3(1.0f,1.0f,1.0f), V3(0.0f,0.0f,0.0f), 1.0f};
global GUITheme DARK_THEME  = {V3(0.0f,0.0f,0.0f), V3(0.2f,0.2f,0.2f), V3(0.0f,0.0f,0.0f), V3(1.0f,1.0f,1.0f), 1.0f};

internal void
UpdateAndRenderEditor (Input *input, RendererState *renderer_state, SoundState *sound_state, r32 dt)
{
	// if(!GLOBAL_EDITORSTATE_P || !GLOBAL_EDITORSTATE_P->initialzed)
	// {
	// 	if(!GLOBAL_EDITORSTATE_P)
	// 		GLOBAL_EDITORSTATE_P = AllocateFromMemoryPool(&game_memory.permanent, EditorState, 1);

	// 	// SVG test_svg = ParseLevelSVG ("data\\raw\\test_0_ill.svg", &game_memory.permanent,&game_memory.transient);
	// 	// SVG_Path *path = test_svg.first_path;
	// 	// NSVGimage* image = nsvgParseFromFile("data\\raw\\test_world_0011.svg", "px", 96);
	// 	// NSVGshape *shape = image->shapes;
	// 	// NSVGpath *path = shape->paths;
	// 	// if(path)
	// 	// {
	// 	// 	NSVGrasterizer *rast = nsvgCreateRasterizer();
	// 	// 	u32 w = image->width;
	// 	// 	u32 h = image->height;
	// 	// 	img_ratio = (r32)w/(r32)h;
	// 	// 	u8 *img = AllocateFromMemoryPool(&game_memory.permanent, u8, (w*h*4));
	// 	// 	nsvgRasterize(rast, image, 0,0,1, img, w, h, w*4, &game_memory.transient);
	// 	// 	RendererRGBABitmapToGPU(&image_id, w, h, img);
	// 	// }
	// 	// {
	// 	// 	shape.shape = image->shapes;
	// 	// }
	// 	// nsvgDelete(image);

	// 	GUIState *gui_state = &GLOBAL_EDITORSTATE_P->gui_state;
	// 	InitializeMemoryPool (&gui_state->memory, MEGABYTES(16));
	// 	gui_state->theme = DARK_THEME;
	// 	GLOBAL_EDITORSTATE_P->editor_window = GUI_CreateWindow(gui_state, "EDITOR", (GUI_WINDOW_TITLED|GUI_WINDOW_MINIMIZABLE|GUI_WINDOW_SCROLLABLE));
	// 	GLOBAL_EDITORSTATE_P->editor_window->p = V2(0.0f,0.0f);
	// 	GLOBAL_EDITORSTATE_P->editor_window->s = V2(2.0f,2.0f);

	// 	GLOBAL_EDITORSTATE_P->assetm_window = GUI_CreateWindow(gui_state, "Asset Manager", (GUI_WINDOW_TITLED|GUI_WINDOW_MINIMIZABLE|GUI_WINDOW_SCROLLABLE|GUI_WINDOW_DETACHABLE));
	// 	GLOBAL_EDITORSTATE_P->add_button = GUI_CreateSolidButton(gui_state, 0, 0, LINES);
	// 	GLOBAL_EDITORSTATE_P->rem_button = GUI_CreateSolidButton(gui_state, 0, 0, LINES);

	// 	GLOBAL_EDITORSTATE_P->click_sound_id = debug_LoadEntireSound(sound_state, "data\\raw\\audio\\sfx_click.wav", &game_memory.assets, &game_memory.transient);
		

	// 	GLOBAL_EDITORSTATE_P->initialzed = true;
	// }

	// Mat4 model_matrix, view_matrix, projection_matrix;
	// view_matrix = renderer_state->world_view_matrix;
	// projection_matrix = renderer_state->world_projection_matrix;

	// Pointer mouse = GetMousePointer(input);

	// RendererEnableDepth();


	// ApplyTransforms (&model_matrix, V2_V3(mouse.world_space_p,0.0f), V3(1.0f,1.0f,1.0f), QUAT_45);
	// Mat4 mvp = (model_matrix * view_matrix) * projection_matrix;
	// {
	// 	V3 positions[] = 
	// 	{
	// 		V2_V3(/*mouse.world_space_p+*/V2(0.0f,0.1f),0.0f), 
	// 		V2_V3(mouse.world_space_p+V2(0.0f,-0.1f),0.0f), 
	// 		V2_V3(/*mouse.world_space_p+*/V2(0.1f,0.0f),0.0f), 
	// 		V2_V3(/*mouse.world_space_p+*/V2(-0.1f,0.0f),0.0f)
	// 	};
	// 	V3 colors[4];
	// 	SET_COLORS(colors,V3(1.0f,0.2f,1.0f),4);
	// 	RenderSolidImmediate (renderer_state, mvp, 1.0f, positions, colors, 4, 0, 0, LINES);
	// }

	// RendererDisableDepth();
	
	// GUIState *gui_state = &GLOBAL_EDITORSTATE_P->gui_state;
	// GUI_Begin(gui_state, input);
	
	// GUIWindow *editor_window = GLOBAL_EDITORSTATE_P->editor_window;
	// GUIWindow *assetm_window = GLOBAL_EDITORSTATE_P->assetm_window;
	
	// GUI_BeginWindow(gui_state, assetm_window);
	// {
	// 	V2 list_size = assetm_window->s * 0.8f;
	// 	V2 list_pad = V2(0.0f,0.0f);
	// 	GUIList *level_svg_list = GLOBAL_EDITORSTATE_P->level_svg_list;
	// 	GUI_BeginList(gui_state, level_svg_list, list_size, list_pad, (GUI_ALLIGN_TOP|GUI_ALLIGN_HCENTER));
	// 	GUIListItem *src = GLOBAL_EDITORSTATE_P->first_level_svg;
	// 	while(src)
	// 	{
	// 		GUI_AddToList(gui_state, src);
	// 		src = src->next;
	// 	}
	// 	GUIListItem *hovering = GUI_EndList(gui_state, level_svg_list);
		
	// 	if(hovering)
	// 	{
	// 		//show preview?
	// 	}

	// 	V2 button_size = V2(0.1f,0.1f);
	// 	V2 pad_size = button_size * 0.5f;
	// 	V2 solid_hsize = (button_size * 0.8f) * 0.5f;
	// 	b32 add_button_pressed, rem_button_pressed;
	// 	{
	// 		// V3 positions[] = {V3(-solid_hsize.x,0.0f,0.0f), V3(solid_hsize.x,0.0f,0.0f), V3(0.0f,-solid_hsize.y,0.0f), V3(0.0f,solid_hsize.y,0.0f)};
	// 		// u32 point_count = sizeof(positions) / sizeof(V3);
	// 		add_button_pressed = GUI_SolidButton(gui_state,
	// 												  GLOBAL_EDITORSTATE_P->add_button,
	// 												  button_size, pad_size, 
	// 												  (GUI_ALLIGN_AT_LEFT_BORDERS|GUI_ALLIGN_BELOW));
	// 	}
	// 	{
	// 		// V3 positions[] = {V3(-solid_hsize.x,0.0f,0.0f), V3(solid_hsize.x,0.0f,0.0f)};
	// 		// u32 point_count = sizeof(positions) / sizeof(V3);
	// 		rem_button_pressed = GUI_SolidButton(gui_state,
	// 												  GLOBAL_EDITORSTATE_P->rem_button,
	// 												  button_size, pad_size,
	// 												  GUI_ALLIGN_TO_RIGHT);
	// 	}
	// 	if(add_button_pressed)
	// 	{
	// 		//Add level svg
	// 	}
	// 	else if(rem_button_pressed)
	// 	{
	// 		//Remove level svg
	// 	}
	// }
	// GUI_EndWindow(gui_state);

	// GUI_BeginWindow(gui_state, editor_window);
	// {
	// 	if(GUI_IsWindowAttached(assetm_window))
	// 		GUI_AttachWindow(gui_state, assetm_window, (GUI_ALLIGN_TOP|GUI_ALLIGN_HCENTER));


	// }
	// GUI_EndWindow(gui_state);

	// GUI_End(gui_state);

// 	runtime_milliseconds += (dt * 1000.0f);
// 	FormatTimeString (txt, "runtime:", (u32)runtime_milliseconds, "\n");
// 	point = RenderText(GLOBAL_RENDERERSTATE_P, &GLOBAL_RENDERERSTATE_P->debug_font, 
// 						GLOBAL_RENDERERSTATE_P->gui_projection_matrix,
// 						txt, 
// 						DEBUG_FONT_RES, 0.15f, point, V3(1.0f,1.0f,1.0f), 1.0f, 
// 						&game_memory.permanent, &game_memory.transient);

// 	char p_used[50];
// 	FormatMemoryString (p_used, "", game_memory.permanent.used, "");
// 	char p_total[50];
// 	FormatMemoryString (p_total, "", game_memory.permanent.size, "");
	
// 	char t_used[50];
// 	FormatMemoryString (t_used, "", game_memory.transient.used, "");
// 	char t_total[50];
// 	FormatMemoryString (t_total, "", game_memory.transient.size, "");

// 	char a_used[50];
// 	FormatMemoryString (a_used, "", game_memory.assets.used, "");
// 	char a_total[50];
// 	FormatMemoryString (a_total, "", game_memory.assets.size, "");

// 	sprintf(txt, 
// 			"permanent: %s/%s\n\
// 			transient: %s/%s\n\
// 			assets:    %s/%s\n",
// 			p_used,p_total, t_used,t_total, a_used,a_total);
// 	point = RenderText(renderer_state, &renderer_state->debug_font, 
// 					   renderer_state->gui_projection_matrix, 
// 					   txt, 
// 					   DEBUG_FONT_RES, 0.15f, point, V3(1.0f,1.0f,1.0f), 1.0f, 
// 					   &game_memory.permanent, &game_memory.transient);

// 	sprintf(txt, "mouse: [%.0f,%.0f] [%.2f,%.2f] [%.2f,%.2f]\n", 
// 			mouse.screen_space_p.x,mouse.screen_space_p.y,
// 			mouse.camera_space_p.x,mouse.camera_space_p.y,
// 			mouse.world_space_p.x,mouse.world_space_p.y);
// 	point = RenderText(renderer_state, &renderer_state->debug_font, 
// 					   renderer_state->gui_projection_matrix, 
// 					   txt, 
// 					   DEBUG_FONT_RES, 0.15f, point, V3(1.0f,1.0f,1.0f), 1.0f, 
// 					   &game_memory.permanent, &game_memory.transient);

// 	sprintf(txt, "screen: [%d,%d] [%d,%d]\n", 
// 			renderer_state->top_x,renderer_state->top_y,
// 			renderer_state->window_width,renderer_state->window_height);
// 	point = RenderText(renderer_state, &renderer_state->debug_font, 
// 					   renderer_state->gui_projection_matrix, 
// 					   txt, 
// 					   DEBUG_FONT_RES, 0.15f, point, V3(1.0f,1.0f,1.0f), 1.0f, 
// 					   &game_memory.permanent, &game_memory.transient);

// #ifdef OPENGL_RENDERER
// 	OpenGLState *opengl_state = (OpenGLState *)renderer_state->renderer;
// 	if(opengl_state)
// 	{
// 		sprintf(txt,"gpu: %s, %s\n",opengl_state->vendor_string, opengl_state->renderer_string);
// 		point = RenderText(renderer_state, &renderer_state->debug_font, 
// 					   renderer_state->gui_projection_matrix, 
// 					   txt, 
// 					   DEBUG_FONT_RES, 0.15f, point, V3(1.0f,1.0f,1.0f), 1.0f, 
// 					   &game_memory.permanent, &game_memory.transient);
// 	}
// #endif

// 	PlayingSound *playing_sound = sound_state->first_playing_sound;
// 	while(playing_sound)
// 	{
// 		char current[56];
// 		u32 samples_played = playing_sound->samples_played;
// 		r32 current_time = (((r32)samples_played)/sound_state->system_buffer_samples_per_second) * 1000.0f;
// 		FormatTimeString (current, "", (u32)current_time, "");
// 		char total[56];
// 		u32 sample_count = sound_state->loaded_sounds[playing_sound->loaded_id].sample_count;
// 		r32 total_time = (((r32)sample_count)/sound_state->system_buffer_samples_per_second) * 1000.0f;
// 		FormatTimeString (total, "", (u32)total_time, "");
// 		sprintf(txt, "id:%d t: %s/%s\n", playing_sound->loaded_id,current,total);
// 		point = RenderText(renderer_state, &renderer_state->debug_font, 
// 							renderer_state->gui_projection_matrix, 
// 							txt, 
// 							DEBUG_FONT_RES, 0.15f, point, V3(1.0f,1.0f,1.0f), 1.0f, 
// 							&game_memory.permanent, &game_memory.transient);
// 		playing_sound = playing_sound->next;
// 	}
}