
internal GUIWindow *
GUI_CreateWindow (GUIState *gui_state, const char *title, u32 flags)
{
	GUIWindow *res = AllocateFromMemoryPool(&gui_state->memory,GUIWindow,1);
	
	res->flags = flags;
	u32 title_length = GetLength(title);
	assert(title_length <= GUI_MAX_WINDOW_TITLE_SIZE);
	CopyString(title,res->title,title_length);
	res->scroll_t = 0.0f;
	res->maximized = false;
	res->closed = false;
	res->p = V2(0.0f,0.0f);
	res->s = V2(0.0f,0.0f);

	return res;
}

internal void 
GUI_Begin (GUIState *gui_state, Input *input)
{

}

internal void 
GUI_End (GUIState *gui_state)
{

}

internal void 
GUI_BeginWindow (GUIState *gui_state, GUIWindow *window)
{
	// GUIItem *item;
	// if(gui_state->first_free_item)
	// {
	// 	item = gui_state->first_free_item;
	// 	gui_state->first_free_item = gui_state->first_free_item->next;
	// }
	// else
	// {
	// 	item = AllocateFromMemoryPool(&gui_state->memory,GUIItem,1);
	// }

	// item->type = GUI_ITEM_WINDOW;
	// item->state = (void*)res;
	// item->first_child = 0;
	// item->next = gui_state->first_item;
	// gui_state->first_item = item;
}

internal b32 
GUI_IsWindowAttached (GUIWindow *window)
{
	b32 res = false;
	return res;
}

internal void 
GUI_AttachWindow (GUIState *gui_state, GUIWindow *window, u32 flags)
{

}

internal void 
GUI_EndWindow (GUIState *gui_state)
{

}

internal void 
GUI_BeginList (GUIState *gui_state, GUIList *list, V2 list_size, V2 list_pad, u32 flags)
{

}

internal void 
GUI_AddToList (GUIState *gui_state, GUIListItem *item)
{

}

internal GUIListItem *
GUI_EndList (GUIState *gui_state, GUIList *list)
{
	GUIListItem *res = 0;
	return res;
}

internal GUISolidButton *
GUI_CreateSolidButton (GUIState *gui_state, V3 *positions, u32 point_count, u32 mode)
{
	GUISolidButton *res = AllocateFromMemoryPool(&gui_state->memory, GUISolidButton, 1);
	res->p = V2(0.0f,0.0f);
	res->s = V2(0.0f,0.0f);
	res->mode = mode;
	res->point_count = point_count;
	return res;
}

internal b32 
GUI_SolidButton (GUIState *gui_state, GUISolidButton *button, V2 button_size, V2 pad_size, u32 flags)
{
	b32 res = false;
	return res;
}


// internal b32
// DoButton (Input *input, RendererState *renderer_state, ButtonGUI *button, MemoryPool *memory, MemoryPool *temp_memory, r32 dt)
// {
// 	Pointer mouse = GetMousePointer(input);

// 	b32 mouse_is_hovering = IsPointInsideRect(mouse.camera_space_p, button->p, button->s);
// 	b32 clicked = (mouse_is_hovering && (WasKeyJustPressed(input, KEY_LEFT_MOUSE_BUTTON)));

// 	{
// 		Mat4 mvp;

// 		V3 color = (mouse_is_hovering ? button->highlight.rgb : button->color.rgb);
// 		r32 a = (mouse_is_hovering ? button->highlight.a : button->color.a);
// 		r32 s = ((mouse_is_hovering && IsKeyDown(input, KEY_LEFT_MOUSE_BUTTON)) ? 1.0f : 1.0f);

// 		ApplyTransforms (&mvp, V2_V3(button->p,-0.0f), V2_V3(button->s,1.0f)*s, QUAT_ZERO);
// 		mvp = (mvp * renderer_state->gui_projection_matrix);
// 		{
// 			V3 positions[] = {V3(-0.5f,0.5f,0.0f), V3(0.5f,0.5f,0.0f), V3(0.5f,-0.5f,0.0f), V3(-0.5f,-0.5f,0.0f)};
// 			const u32 point_count = (sizeof(positions)/sizeof(V3));
// 			V3 colors[point_count];
// 			u16 indices[] = {0,1,2, 0,2,3};
// 			u32 index_count = (sizeof(indices)/sizeof(u16));
// 			SET_COLORS(colors,color, point_count);
// 			RenderSolidImmediate (renderer_state, mvp, a, 
// 									    positions, colors, point_count, indices, index_count, TRIANGLES);
// 		}

// 		if(renderer_state->font_bag.loaded)
// 		{
// 			r32 text_size = button->s.y*0.6f*s;
// 			V2 text_p = (button->p-(button->s*0.5f)) + V2(button->s.y*0.6f*0.5f, button->s.y*0.6f*0.5f);
// 			V3 text_color = (mouse_is_hovering ? button->color.rgb : button->highlight.rgb);
// 			RenderText(renderer_state, &renderer_state->debug_font, 
// 							renderer_state->gui_projection_matrix, 
// 							button->label, 
// 							renderer_state->debug_font_res, text_size, 
// 							text_p, text_color, button->highlight.a, 
// 							memory, temp_memory);
// 		}
// 	}

// 	return clicked;
// }

// internal void
// DoBox (Input *input, RendererState *renderer_state, GUIBox *box, u32 click_sound, MemoryPool *memory, MemoryPool *temp_memory, r32 dt)
// {
// 	Pointer mouse = GetMousePointer(input);

// 	Mat4 mvp;
// 	{
// 		{
// 			V3 positions[] = {V3(-0.5f,0.5f,0.0f), V3(0.5f,0.5f,0.0f), V3(0.5f,-0.5f,0.0f), V3(-0.5f,-0.5f,0.0f)};
// 			const u32 point_count = (sizeof(positions)/sizeof(V3));
// 			V3 colors[point_count];
// 			u16 indices[] = {0,1,2, 0,2,3};
// 			u32 index_count = (sizeof(indices)/sizeof(u16));
// 			SET_COLORS(colors,box->box_color.rgb, point_count);
			
// 			ApplyTransforms (&mvp, V2_V3(box->p,-0.0f), V2_V3(box->s,1.0f), QUAT_ZERO);
// 			mvp = (mvp * renderer_state->gui_projection_matrix);
// 			RenderSolidImmediate (renderer_state, mvp, box->box_color.a, 
// 									    positions, colors, point_count, indices, index_count, TRIANGLES);
			
// 			r32 y_off = GUIBOX_ITEM_TITLE_HEIGHT*1.5f;
// 			for (u32 i = 0; i < box->item_count; ++i)
// 			{
// 				GUIBoxItem *item = box->items + i;

// 				V2 size = V2(box->s.x*0.9f, GUIBOX_ITEM_TITLE_HEIGHT);
// 				item->p = box->p + V2(0.0f,box->s.y*0.5f) - V2(0.0f,y_off);

// 				b32 mouse_is_hovering = IsPointInsideRect(mouse.camera_space_p, item->p, size);
// 				b32 clicked = (mouse_is_hovering && (WasKeyJustPressed(input, KEY_LEFT_MOUSE_BUTTON)));

// 				if(clicked)
// 					item->maximized = !item->maximized;

// 				if(item->maximized)
// 				{
// 					if(item->t < GUIBOX_ITEM_MAXIM_TIME)
// 						item->t += dt;
// 				}
// 				else
// 				{
// 					if(item->t > 0.0f)
// 						item->t -= dt;
// 				}
// 				item->t = Clamp(0.0f, GUIBOX_ITEM_MAXIM_TIME, item->t);
// 				r32 dy = (GUIBOX_ITEM_TITLE_HEIGHT*50.0f) * item->t;
				
// 				V4 color = (mouse_is_hovering ? box->item_highlight : box->item_color);

// 				ApplyTransforms (&mvp, V2_V3(item->p,-0.0f), V2_V3(size,1.0f), QUAT_ZERO);
// 				mvp = (mvp * renderer_state->gui_projection_matrix);
// 				SET_COLORS(colors,color.rgb, point_count);
// 				RenderSolidImmediate (renderer_state, mvp, color.a, 
// 										    positions, colors, point_count, indices, index_count, TRIANGLES);
				
// 				{
// 					V2 s = V2(GUIBOX_ITEM_TITLE_HEIGHT,GUIBOX_ITEM_TITLE_HEIGHT)*0.6f;
// 					V2 p = item->p + V2((size.x*0.5f) - (GUIBOX_ITEM_TITLE_HEIGHT*0.5f), 0.0f);
// 					Quat r = Slerp(QUAT_ZERO, QUAT_45, (item->t/GUIBOX_ITEM_MAXIM_TIME));
// 					ApplyTransforms (&mvp, V2_V3(p,-0.0f), V2_V3(s,1.0f), r);
// 					mvp = (mvp * renderer_state->gui_projection_matrix);
// 					V3 positions[] = {V3(-0.5f,0.0f,0.0f), V3(0.5f,0.0f,0.0f), V3(0.0f,-0.5f,0.0f), V3(0.0f,0.5f,0.0f)};
// 					const u32 point_count = (sizeof(positions)/sizeof(V3));
// 					V3 colors[point_count];
// 					SET_COLORS(colors,box->box_color.rgb, point_count);
// 					RenderSolidImmediate (renderer_state, mvp, box->box_color.a, 
// 											    positions, colors, point_count, 0, 0, LINES);
// 				}

// 				if(item->maximized)
// 				{
// 					V2 s = V2(size.x, dy);
// 					V2 p = item->p - V2(0.0f,(GUIBOX_ITEM_TITLE_HEIGHT*0.5f) + (s.y*0.5f));
// 					ApplyTransforms (&mvp, V2_V3(p,0.0f), V2_V3(s,1.0f), QUAT_ZERO);
// 					mvp = (mvp * renderer_state->gui_projection_matrix);
// 					SET_COLORS(colors,box->item_color.rgb, point_count);
// 					RenderSolidImmediate (renderer_state, mvp, box->item_color.a, 
// 											    positions, colors, point_count, indices, index_count, TRIANGLES);
// 				}

// 				y_off += dy + GUIBOX_ITEM_TITLE_HEIGHT*1.5f;
// 			}

// 		}
// 		// {
// 		// 	V3 positions[] = {V3(-0.499f,-0.499f,0.0f), V3(-0.499f,0.499f,0.0f), V3(0.499f,0.499f,0.0f)};
// 		// 	const u32 point_count = (sizeof(positions)/sizeof(V3));
// 		// 	V3 colors[point_count];
// 		// 	SET_COLORS(colors,box->item_color.rgb, point_count);
// 		// 	RenderSolidImmediate (renderer_state, mvp, box->item_color.a, 
// 		// 							    positions, colors, point_count, 0, 0, LINE_STRIP);
// 		// }
// 	}
// }
