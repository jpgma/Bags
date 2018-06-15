
/*
- ATUALMENTE -
- Usando nanosvg e nanosvgrast p/ parse e rasterizacao dos svgs

- PLANOS -

-Renderizacao de shapes estaticos-
	- pre processamento (no packing dos assets):
		- obter curvas bezier cubicas dos paths
		- subdividir as adaptivamente curvas na escala do maior LOD esperado(m_distance_tolerance), gerando contornos (algoritmo em Anti-Grain Geometry)
			- multiplos LOD's? provavelmente desnecessario mas possivelmente util em optimizacao
		- constrained Delaunay 2D triangulation p/ tesselamento dos contornos
	- renderizacao (em runtime):
		- colocar shapes tesselados na GPU (no loading dos shapes)
		- usar shaders especializados p/ renderizar
-Renderizacao de shapes animados-
	- pre processamento:
		- obter curvas bezier cubicas dos paths
		- subdivisao De-Casteljau estática p/ cada keyframe
			- subidv. adaptiva causaria # de verts diferentes p/ cada keyframe, potencialmente
		- constrained Delaunay 2D triangulation p/ tesselamento dos contornos
	- renderizacao:
		- trabalhando com tri. meshes
			- analisar possibilidade de tess. em real time (2o e 3o passos do pre proc. em runtime)
		- skinning
			- GPU? CPU? ambos?
		- morphing
			- 2 keyframes na gpu, parametro entre 0 e 1 indicando % do 2o frame como input do shader
*/

#if OPENGL_RENDERER
#else
	internal void RenderTexturedQuad(RendererState *,Mat4,r32,V3,u32,b32) {}
	internal void RendererRGBABitmapToGPU(u32 *,u32,u32,u8 *) {}
	internal void RenderShapeImmediate(RendererState *,Mat4,r32,Shape *) {}
	internal void RenderSolidImmediate(RendererState *,Mat4,r32,V3 *,V3 *,u32,u16 *,u32,u32) {}
#endif

// This is janky AF, consertar obtendo tamanho adequado da janela
internal V3
ScreenSpaceToWorldSpace(RendererState *renderer_state, V2 screen_p, b32 fullscreen)
{
	V3 res = {};

	r32 x = (2.0f * screen_p.x) / renderer_state->window_width - 1.0f;
	r32 y = 1.0f - (2.0f * screen_p.y) / renderer_state->window_height;
	V4 normalized_screen_pointer = V4(x,y,0.0f,1.0f);
	Mat4 inv_mvp = Inverse(renderer_state->wvp);
	V4 camera_p = V4(renderer_state->camera.p.x,renderer_state->camera.p.y,renderer_state->camera.p.z, 0.0f);
	V4 world_p = (inv_mvp * normalized_screen_pointer * 3.0f) + camera_p;
	if(!fullscreen) world_p.y -= 0.5f;
	// else world_p.y -= 0.05f;
	world_p.z = renderer_state->camera.focus.z;

	//d=1.0 c=2.00
	//d=2.0 c=2.66
	//d=3.0 c=3.33
	//d=4.0 c=4.00
	//d=5.0 c=4.66
	r32 d = renderer_state->camera.p.z - renderer_state->camera.focus.z;
	r32 coef = 2.0f + ((d-1.0f)*0.66f);
	world_p.x = (world_p.x*coef);
	world_p.y = (world_p.y*coef);
	res = world_p.xyz;

	return res;
}

internal V2
ScreenSpaceToCameraSpace (RendererState *renderer_state, V2 screen_p, b32 fullscreen)
{
	V2 res = {};
	
	r32 x = (2.0f * screen_p.x) / renderer_state->window_width - 1.0f;
	r32 y = 1.0f - (2.0f * screen_p.y) / renderer_state->window_height;
	V4 normalized_screen_pointer = V4(x,y,0.0f,1.0f);
	Mat4 inv_mvp = Inverse(renderer_state->gui_projection_matrix);
	V4 camera_p = V4(renderer_state->camera.p.x,renderer_state->camera.p.y,renderer_state->camera.p.z, 0.0f);
	V4 cam_p = (inv_mvp * normalized_screen_pointer * 3.0f) + camera_p;
	if(!fullscreen) cam_p.y -= 0.43f;
	// else cam_p.y -= 0.05f;
	cam_p.z = renderer_state->camera.focus.z;

	//d=1.0 c=2.00
	//d=2.0 c=2.66
	//d=3.0 c=3.33
	//d=4.0 c=4.00
	//d=5.0 c=4.66
	// r32 d = renderer_state->camera.p.z - renderer_state->camera.focus.z;
	// r32 coef = 2.0f + ((d-1.0f)*0.66f);
	// cam_p.x = (cam_p.x*coef);
	// cam_p.y = (cam_p.y*coef);
	res = cam_p.xy;

	return res;
}

internal V2
WorldSpaceToScreenSpace (RendererState *renderer_state, V3 world_p, b32 fullscreen)
{
	V2 res = {};

	r32 d = Absolute(renderer_state->camera.p.z - world_p.z);
	r32 coef = 2.0f + ((d-1.0f)*0.66f);
	V4 world_p_ = V4(world_p.x/coef, world_p.y/coef, world_p.z, 1.5f);

	if(!fullscreen) world_p_.y += 0.5f;
	else world_p_.y += 0.05f;
	V4 camera_p = V4(renderer_state->camera.p.x,renderer_state->camera.p.y,renderer_state->camera.p.z, 0.0f);
	Mat4 mvp = renderer_state->wvp;
	V4 nsp = mvp * ((world_p_ - camera_p) * (1.0f/3.0f));

	res.x = ((nsp.x+1.0f)*renderer_state->window_width) / 2.0f;
	res.y = ((nsp.y-1.0f)*renderer_state->window_height) / -2.0f;
	res.y += 6.37f; //wtf

	return res;
}

internal b32
PointInsideScaledRect (RendererState *renderer_state, V2 point, V3 center, V2 size)
{
	center = center * renderer_state->distance;
	size = size * renderer_state->distance;
	return IsPointInsideRect(point, center.xy, size);
}