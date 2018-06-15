
#include "opengl_bags.cpp"

global GLenum RENDER_MODES[] = 
{
	GL_LINES,
	GL_LINE_LOOP,
	GL_LINE_STRIP,
	GL_TRIANGLES,
	GL_POINTS,
	GL_TRIANGLE_STRIP,
	GL_TRIANGLE_FAN
};

#define A_POSITION "in_position"
#define A_COLOR "in_color"
#define U_ALPHA "u_alpha"
#define U_COLOR "u_color"
#define U_MVP "u_mvp"
#define A_UV "in_uv"
#define U_TEXTURE_SAMPLER "u_texture_sampler"

struct Line2DShader
{
	GLuint u_dist;
	GLuint u_mvp_location;
	GLuint u_color_location;

	GLuint a_position;

	GLuint id;
};

struct SolidShader
{
	GLuint u_alpha_location;
	GLuint u_mvp_location;

	GLuint a_position;
	GLuint a_color;

	GLuint id;
};

struct TextureShader
{
	GLuint u_texture_sampler_location;
	GLuint u_color_location;
	GLuint u_alpha_location;
	GLuint u_mvp_location;

	GLuint a_position;
	GLuint a_uv;

	GLuint id;
};

struct OpenGLState
{
	u32 version;
	char *vendor_string;
	char *renderer_string;
	char *extensions_string;

	SolidShader solid_shader;
	Line2DShader line2D_shader;
	TextureShader texture_shader;
};

global GLuint circle_vao;
global GLuint rectangle_vao; 
global GLuint quad_vao; 

global u32 GL_VERSION;
global char *VENDOR_STRING;
global char *RENDERER_STRING;
global char *EXTENSIONS_STRING;

void *GetGLFuncAddress(HMODULE opengl32Module, const char *name)
{
  void *p = (void *)wglGetProcAddress(name);
  if(p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1) )
  {
    p = (void *)GetProcAddress(opengl32Module, name);
  }
  return p;
}

internal void
LoadGLFunctions()
{
	HMODULE gl32module = LoadLibraryA("opengl32.dll");
	glCreateShader = (GLCREATESHADER*)GetGLFuncAddress(gl32module, "glCreateShader");
	glShaderSource = (GLSHADERSOURCE*)GetGLFuncAddress(gl32module, "glShaderSource");
	glCompileShader = (GLCOMPILESHADER*)GetGLFuncAddress(gl32module, "glCompileShader");
	glGetShaderiv = (GLGETSHADERIV*)GetGLFuncAddress(gl32module, "glGetShaderiv");
	glGetShaderInfoLog = (GLGETSHADERINFOLOG*)GetGLFuncAddress(gl32module, "glGetShaderInfoLog");
	glDeleteShader = (GLDELETESHADER*)GetGLFuncAddress(gl32module, "glDeleteShader");
	glCreateProgram = (GLCREATEPROGRAM*)GetGLFuncAddress(gl32module, "glCreateProgram");
	glAttachShader = (GLATTACHSHADER*)GetGLFuncAddress(gl32module, "glAttachShader");
	glLinkProgram = (GLLINKPROGRAM*)GetGLFuncAddress(gl32module, "glLinkProgram");
	glGetProgramiv = (GLGETPROGRAMIV*)GetGLFuncAddress(gl32module, "glGetProgramiv");
	glGetProgramInfoLog = (GLGETPROGRAMINFOLOG*)GetGLFuncAddress(gl32module, "glGetProgramInfoLog");
	glDeleteProgram = (GLDELETEPROGRAM*)GetGLFuncAddress(gl32module, "glDeleteProgram");
	glBindAttribLocation = (GLBINDATTRIBLOCATION*)GetGLFuncAddress(gl32module, "glBindAttribLocation");
	glDetachShader = (GLDETACHSHADER*)GetGLFuncAddress(gl32module, "glDetachShader");
	glUseProgram = (GLUSEPROGRAM*)GetGLFuncAddress(gl32module, "glUseProgram");
	glGetUniformLocation = (GLGETUNIFORMLOCATION*)GetGLFuncAddress(gl32module, "glGetUniformLocation");
	glGenVertexArrays = (GLGENVERTEXARRAYS*)GetGLFuncAddress(gl32module, "glGenVertexArrays");
	glGenBuffers = (GLGENBUFFERS*)GetGLFuncAddress(gl32module, "glGenBuffers");
	glBindVertexArray = (GLBINDVERTEXARRAY*)GetGLFuncAddress(gl32module, "glBindVertexArray");
	glBindBuffer = (GLBINDBUFFER*)GetGLFuncAddress(gl32module, "glBindBuffer");
	glBufferData = (GLBUFFERDATA*)GetGLFuncAddress(gl32module, "glBufferData");
	glVertexAttribPointer = (GLVERTEXATTRIBPOINTER*)GetGLFuncAddress(gl32module, "glVertexAttribPointer");
	glEnableVertexAttribArray = (GLENABLEVERTEXATTRIBARRAY*)GetGLFuncAddress(gl32module, "glEnableVertexAttribArray");
	glFrontFace = (GLFRONTFACE*)GetGLFuncAddress(gl32module, "glFrontFace");
	glCullFace = (GLCULLFACE*)GetGLFuncAddress(gl32module, "glCullFace");
	glEnable = (GLENABLE*)GetGLFuncAddress(gl32module, "glEnable");
	glBlendFunc = (GLBLENDFUNC*)GetGLFuncAddress(gl32module, "glBlendFunc");
	glBlendEquation = (GLBLENDEQUATION*)GetGLFuncAddress(gl32module, "glBlendEquation");
	glLineWidth = (GLLINEWIDTH*)GetGLFuncAddress(gl32module, "glLineWidth");
	glPointSize = (GLPOINTSIZE*)GetGLFuncAddress(gl32module, "glPointSize");
	glViewport = (GLVIEWPORT*)GetGLFuncAddress(gl32module, "glViewport");
	glClearColor = (GLCLEARCOLOR*)GetGLFuncAddress(gl32module, "glClearColor");
	glUniformMatrix4fv = (GLUNIFORMMATRIX4FV*)GetGLFuncAddress(gl32module, "glUniformMatrix4fv");
	glUniform1i = (GLUNIFORM1I*)GetGLFuncAddress(gl32module, "glUniform1i");
	glVertexAttrib3f = (GLVERTEXATTRIB3F*)GetGLFuncAddress(gl32module, "glVertexAttrib3f");
	glBindTexture = (GLBINDTEXTURE*)GetGLFuncAddress(gl32module, "glBindTexture");
	glDrawArrays = (GLDRAWARRAYS*)GetGLFuncAddress(gl32module, "glDrawArrays");
	glDrawElements = (GLDRAWELEMENTS*)GetGLFuncAddress(gl32module, "glDrawElements");
	glUniform3f = (GLUNIFORM3F*)GetGLFuncAddress(gl32module, "glUniform3f");
	glGenTextures = (GLGENTEXTURES*)GetGLFuncAddress(gl32module, "glGenTextures");
	glTexImage2D = (GLTEXIMAGE2D*)GetGLFuncAddress(gl32module, "glTexImage2D");
	glTexParameteri = (GLTEXPARAMETERI*)GetGLFuncAddress(gl32module, "glTexParameteri");
	glClear = (GLCLEAR*)GetGLFuncAddress(gl32module, "glClear");
	glDeleteVertexArrays = (GLDELETEVERTEXARRAYS*)GetGLFuncAddress(gl32module, "glDeleteVertexArrays");
	glDeleteBuffers = (GLDELETEBUFFERS*)GetGLFuncAddress(gl32module, "glDeleteBuffers");
	glGetIntegerv = (GLGETINTEGERV*)GetGLFuncAddress(gl32module, "glGetIntegerv");
	glUniform1f = (GLUNIFORM1F*)GetGLFuncAddress(gl32module, "glUniform1f");
	glGetString = (GLGETSTRING*)GetGLFuncAddress(gl32module, "glGetString");
	glDisable = (GLDISABLE*)GetGLFuncAddress(gl32module, "glDisable");
}

internal void
InitOpenGL (System *system, OpenGLState *opengl_state)
{
	PIXELFORMATDESCRIPTOR desired_pf = {};
	desired_pf.nSize = sizeof(desired_pf);
	desired_pf.nVersion = 1;
	desired_pf.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER;
	desired_pf.cColorBits = 32;
	desired_pf.cAlphaBits = 32;
	desired_pf.iLayerType = PFD_MAIN_PLANE;
	
	i32 suggested_pf_index = ChoosePixelFormat(system->hdc, &desired_pf);
	PIXELFORMATDESCRIPTOR suggested_pf = {};
	DescribePixelFormat(system->hdc, suggested_pf_index, sizeof(suggested_pf), &suggested_pf);
	SetPixelFormat(system->hdc, suggested_pf_index, &suggested_pf);

	HGLRC glrc = wglCreateContext(system->hdc);
	if(glrc && wglMakeCurrent(system->hdc, glrc))
	{
		LoadGLFunctions();
		// render_info->initialized = true;

		i32 major,minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		opengl_state->version = ((u32) major << 8) | ((u32)minor);

		opengl_state->extensions_string = (char *)glGetString(GL_EXTENSIONS);
		opengl_state->renderer_string = (char *)glGetString(GL_RENDERER);
		opengl_state->vendor_string = (char *)glGetString(GL_VENDOR);
	}
	else assert(0);
}

internal GLuint
OpenGLCompileShader (const char *filename, GLenum type)
{
	GLuint shader = glCreateShader(type);

	const GLchar *source = (const GLchar *)GetTextFileContents(filename, ASCII_encoding);
	glShaderSource(shader, 1, &source, 0);

	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		char *log = (char*)calloc(1, maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, (GLchar*)log);
		
		glDeleteShader(shader);

		ShowSystemPopupTextWindow("error", log);

		return 0;
	}

	Deallocate((void*)source, GetLength(source));

	return shader;
}

internal GLuint
OpenGLCreateShaderProgram (GLuint vertex_shader, GLuint fragment_shader)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	return program;
}

internal void
OpenGLLinkShaderProgram (GLuint program)
{
	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		char *log = (char*)Allocate(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, (GLchar*)log);
		
		glDeleteProgram(program);

		ShowSystemPopupTextWindow("error", log);
		
		Deallocate((void*)log, maxLength);

		return;
	}
}

internal void
InitLine2DShader(Line2DShader *line2D_shader)
{
	GLuint vertex_shader = OpenGLCompileShader("data\\shaders\\2Dline_vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragment_shader = OpenGLCompileShader("data\\shaders\\2Dline_fragment.glsl", GL_FRAGMENT_SHADER);
	line2D_shader->id = OpenGLCreateShaderProgram(vertex_shader, fragment_shader);
	
	line2D_shader->a_position = 0;
	glBindAttribLocation(line2D_shader->id, line2D_shader->a_position, A_POSITION);
	
	OpenGLLinkShaderProgram(line2D_shader->id);
	glDetachShader(line2D_shader->id, vertex_shader);
	glDetachShader(line2D_shader->id, fragment_shader);
	glUseProgram(line2D_shader->id);
	line2D_shader->u_mvp_location = glGetUniformLocation(line2D_shader->id, U_MVP);
	line2D_shader->u_color_location = glGetUniformLocation(line2D_shader->id, U_COLOR);
	glUseProgram(0);
}

internal void
InitSolidShader(SolidShader *solid_shader)
{
	GLuint vertex_shader = OpenGLCompileShader("data\\shaders\\solid_vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragment_shader = OpenGLCompileShader("data\\shaders\\solid_fragment.glsl", GL_FRAGMENT_SHADER);
	solid_shader->id = OpenGLCreateShaderProgram(vertex_shader, fragment_shader);
	
	solid_shader->a_position = 0;
	glBindAttribLocation(solid_shader->id, solid_shader->a_position, A_POSITION);
	solid_shader->a_color = 1;
	glBindAttribLocation(solid_shader->id, solid_shader->a_color, A_COLOR);
	
	OpenGLLinkShaderProgram(solid_shader->id);
	glDetachShader(solid_shader->id, vertex_shader);
	glDetachShader(solid_shader->id, fragment_shader);
	glUseProgram(solid_shader->id);
	solid_shader->u_mvp_location = glGetUniformLocation(solid_shader->id, U_MVP);
	solid_shader->u_alpha_location = glGetUniformLocation(solid_shader->id, U_ALPHA);
	glUseProgram(0);
}

internal void
InitTextureShader(TextureShader *texture_shader)
{
	GLuint vertex_shader = OpenGLCompileShader("data\\shaders\\texture_vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragment_shader = OpenGLCompileShader("data\\shaders\\texture_fragment.glsl", GL_FRAGMENT_SHADER);
	texture_shader->id = OpenGLCreateShaderProgram(vertex_shader, fragment_shader);
	
	texture_shader->a_position = 0;
	glBindAttribLocation(texture_shader->id, texture_shader->a_position, A_POSITION);
	texture_shader->a_uv = 1;
	glBindAttribLocation(texture_shader->id, texture_shader->a_uv, A_UV);
	
	OpenGLLinkShaderProgram(texture_shader->id);
	glDetachShader(texture_shader->id, vertex_shader);
	glDetachShader(texture_shader->id, fragment_shader);
	glUseProgram(texture_shader->id);
	texture_shader->u_color_location = glGetUniformLocation(texture_shader->id, U_COLOR);
	texture_shader->u_alpha_location = glGetUniformLocation(texture_shader->id, U_ALPHA);
	texture_shader->u_mvp_location = glGetUniformLocation(texture_shader->id, U_MVP);
	texture_shader->u_texture_sampler_location = glGetUniformLocation(texture_shader->id, U_TEXTURE_SAMPLER);
	glUseProgram(0);
}

internal void
RendererResize (RendererState *renderer_state, u32 top_x, u32 top_y, u32 width, u32 height)
{
	if(renderer_state && renderer_state->initialized)
	{
		renderer_state->top_x = top_x;
		renderer_state->top_y = top_y;
		renderer_state->window_width = width;
		renderer_state->window_height = height;

		renderer_state->min_y = -1.0f;
		renderer_state->max_y = 1.0f;
		renderer_state->near = 1.0f;
		renderer_state->far = 10.0f;

		renderer_state->window_ratio = (r32)width / (r32)height;

		SetOrthographicProjection(&renderer_state->gui_projection_matrix,
		                     		-renderer_state->window_ratio, renderer_state->window_ratio,
		                     		-1.0f, 1.0f, 0.0f, 1.0f);
		
		glViewport(0.0f,0.0f,renderer_state->window_width,renderer_state->window_height);
	}
}

internal void
InitRenderer (System *system, RendererState *renderer_state, MemoryPool *memory, MemoryPool *temp_memory)
{
	OpenGLState *opengl_state = AllocateFromMemoryPool(memory, OpenGLState, 1);
	renderer_state->renderer = (void*)opengl_state;
	InitOpenGL(system,opengl_state);

	renderer_state->temp_memory = temp_memory;

	InitLine2DShader(&opengl_state->line2D_shader);
	InitSolidShader(&opengl_state->solid_shader);
	InitTextureShader(&opengl_state->texture_shader);

	// u32 pos = 0;
	// r32 point_data[(CIRCLE_RESOLUTION*2)] = {};
	// //point_data[pos++] = 0.0f;
	// //point_data[pos++] = 0.0f;
	// r32 rate = RAD_360/CIRCLE_RESOLUTION;
	// for(r32 a = 0.0f; a < RAD_360; a += rate)
	// {
	// 	V2 p = {cosf(a), sinf(a)};
	// 	Normalize(&p);
	// 	point_data[pos++] = p.x;
	// 	point_data[pos++] = p.y;
	// }
	// circle_vao = PointDataToGPU((V2*)point_data, CIRCLE_RESOLUTION);

	// rectangle_vao = PointDataToGPU((V2*)rectangle_xy, 4);
	
	TextureShader texture_shader = opengl_state->texture_shader;

	glGenVertexArrays(1, &quad_vao);
	u32 vbos[2];
	glGenBuffers(2, vbos);
	glUseProgram(texture_shader.id);
	glBindVertexArray(quad_vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, 4*3*sizeof(GLfloat), (r32*)rectangle_positions, GL_STATIC_DRAW);
	glVertexAttribPointer(texture_shader.a_position, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(texture_shader.a_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(GLfloat), (r32*)rectangle_uv, GL_STATIC_DRAW);
	glVertexAttribPointer(texture_shader.a_uv, 2, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(texture_shader.a_uv);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	glLineWidth(1.5f);
	glPointSize(10.0f);

	renderer_state->rasterizer = nsvgCreateRasterizer();

	//TODO: calcular com base nas VECImages carregadas e res da tela
	u32 bitmap_memory_size = GIGABYTES(1);
	InitializeMemoryPool(&renderer_state->bitmap_memory, bitmap_memory_size);

	renderer_state->initialized = true;
	RendererResize(renderer_state, 0,0,system->resolution.width,system->resolution.height);
}

inline void RendererEnableDepth ()
{
	glEnable(GL_DEPTH_TEST);
}

inline void RendererDisableDepth ()
{
	glDisable(GL_DEPTH_TEST);
}

inline void RendererSetLineWidth(r32 width)
{
	glLineWidth(width);
}

inline void RendererSetPointSize(r32 size)
{
	glPointSize(size);
}

internal void 
BeginRendering (RendererState *renderer_state)
{
	glClearColor(renderer_state->clear_color.r, renderer_state->clear_color.g, renderer_state->clear_color.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

internal void 
RenderVECImage (RendererState *renderer_state, V3 position, V2 size, Quat rotation, r32 alpha, VECImage *image, b32 fullscreen)
{
	position = position * renderer_state->distance;
	size = size * renderer_state->distance;
	if(image->flags & VECIMAGE_FLAG_RASTERIZE)
	{
		//-0.1181034*x + 1.111638
		//1.35 - 0.525*x + 0.2208333*x^2 - 0.05*x^3 + 0.004166667*x^4
		// r32 d = renderer_state->camera.focus.z - position.z;
		// r32 coef = 1.0f;//2.0f + ((d-1.0f)*0.66f);

		// r32 world_width = size.x * coef;
		// r32 pixel_width = WorldSpaceToScreenSpace(renderer_state, V3(world_width,0.0f,0.0f), fullscreen).x;
		
		r32 left = (position.x - (size.x*0.5f));
		left = WorldSpaceToScreenSpace(renderer_state, V3(left,0.0f,position.z), fullscreen).x;
		r32 right = position.x + (size.x*0.5f);
		right = WorldSpaceToScreenSpace(renderer_state, V3(right,0.0f,position.z), fullscreen).x;
		r32 pixel_width = roundf(Absolute(right - left));

		r32 scale = pixel_width / image->nsvg_img->width;
		image->width = image->nsvg_img->width*scale;
		image->height = image->nsvg_img->height*scale;
		image->bitmap = 	AllocateFromMemoryPool(&renderer_state->bitmap_memory, u8, image->width*image->height*4);
		nsvgRasterize(renderer_state->rasterizer, image->nsvg_img, 0,0,scale, image->bitmap, image->width, image->height, image->width*4);
		RendererRGBABitmapToGPU(&image->bitmap_id, image->width, image->height, image->bitmap);
		image->flags &= ~VECIMAGE_FLAG_RASTERIZE;
	}
	

	Mat4 mvp = {};
	ApplyTransforms(&mvp, position, V2_V3(size,1.0f), rotation);
	mvp = (mvp * renderer_state->world_view_matrix) * renderer_state->world_projection_matrix;
	RenderTexturedQuad(renderer_state, mvp, alpha, V3(1.0f,1.0f,1.0f),image->bitmap_id,true);
}

internal void
RendererSolidDataToGPU (RendererState *renderer_state, V3 *positions, V3 *colors, u32 count, u32 *points_id, u32 *positions_id, u32 *colors_id)
{
	SolidShader solid_shader = ((OpenGLState*)renderer_state->renderer)->solid_shader;

	if(!*points_id)
	{
		glGenVertexArrays(1, points_id);
	}
	if(!*positions_id)
	{
		glGenBuffers(1, positions_id);
	}
	if(!*colors_id)
	{
		glGenBuffers(1, colors_id);
	}

	glUseProgram(solid_shader.id);
	glBindVertexArray(*points_id);
	glBindBuffer(GL_ARRAY_BUFFER, *positions_id);
	glBufferData(GL_ARRAY_BUFFER, count*3*sizeof(GLfloat), (r32*)positions, GL_STATIC_DRAW);
	glVertexAttribPointer(solid_shader.a_position, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(solid_shader.a_position);
	
	glBindBuffer(GL_ARRAY_BUFFER, *colors_id);
	glBufferData(GL_ARRAY_BUFFER, count*3*sizeof(GLfloat), (r32*)colors, GL_STATIC_DRAW);
	glVertexAttribPointer(solid_shader.a_color, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(solid_shader.a_color);
}

internal void
RenderSolid (RendererState *renderer_state, Mat4 mvp, r32 alpha, u32 id, u32 count, u16 *indices, u32 mode)
{
	SolidShader solid_shader = ((OpenGLState*)renderer_state->renderer)->solid_shader;

	glUseProgram(solid_shader.id);

	GLfloat mvp_vals[] = Mat4ValueArray(mvp);
	glUniformMatrix4fv(solid_shader.u_mvp_location, 1, FALSE, (GLfloat*)&mvp_vals[0]);
	glUniform1f(solid_shader.u_alpha_location, (GLfloat)alpha);

	glBindVertexArray(id);
	if(indices)
	{
		glDrawElements(RENDER_MODES[mode], count, GL_UNSIGNED_SHORT, indices);
	}
	else
	{
		glDrawArrays(RENDER_MODES[mode], 0, count);
	}
}

internal void
RenderTexturedQuad (RendererState *renderer_state, Mat4 mvp, r32 alpha, V3 color, u32 texture_id, b32 outline)
{
	TextureShader texture_shader = ((OpenGLState*)renderer_state->renderer)->texture_shader;

	glUseProgram(texture_shader.id);
	glBindVertexArray(quad_vao);

	GLfloat mvp_vals[] = Mat4ValueArray(mvp);
	glUniformMatrix4fv(texture_shader.u_mvp_location, 1, FALSE, (GLfloat*)&mvp_vals[0]);
	glUniform3f(texture_shader.u_color_location, (GLfloat)color.x,(GLfloat)color.y,(GLfloat)color.z);
	glUniform1f(texture_shader.u_alpha_location, (GLfloat)alpha);
	glUniform1i(texture_shader.u_texture_sampler_location, 0);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, rectangle_indices);

	if(outline)
		RenderSolid(renderer_state, mvp, alpha, quad_vao, 6, rectangle_indices, LINE_LOOP);
}

internal void
RenderSolidImmediate (RendererState *renderer_state, Mat4 mvp, r32 alpha, V3 *positions, V3 *colors, u32 count, u16 *indices, u32 index_count, u32 mode)
{
	SolidShader solid_shader = ((OpenGLState*)renderer_state->renderer)->solid_shader;

	glUseProgram(solid_shader.id);

	GLfloat mvp_vals[] = Mat4ValueArray(mvp);
	glUniformMatrix4fv(solid_shader.u_mvp_location, 1, FALSE, (GLfloat*)&mvp_vals[0]);
	glUniform1f(solid_shader.u_alpha_location, (GLfloat)alpha);

	// GenerateTemps(2);

	// glBindVertexArray(temp_vaos[temp_vao_count]);

	// glBindBuffer(GL_ARRAY_BUFFER, temp_vbos[temp_vbo_count-1]);
	// glBufferData(GL_ARRAY_BUFFER, count*3*sizeof(GLfloat), (r32*)positions, GL_STATIC_DRAW);
	// glVertexAttribPointer(solid_shader.a_position, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	// glEnableVertexAttribArray(solid_shader.a_position);

	// glBindBuffer(GL_ARRAY_BUFFER, temp_vbos[temp_vbo_count]);
	// glBufferData(GL_ARRAY_BUFFER, count*3*sizeof(GLfloat), (r32*)colors, GL_STATIC_DRAW);
	// glVertexAttribPointer(solid_shader.a_color, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	// glEnableVertexAttribArray(solid_shader.a_color);
	
	u32 vao = 0;
	u32 vbos[] = {0,0};

	RendererSolidDataToGPU(renderer_state, positions, colors, count, &vao, &vbos[0], &vbos[1]);	
	glBindVertexArray(vao);

	if(indices)
	{
		glDrawElements(RENDER_MODES[mode], index_count, GL_UNSIGNED_SHORT, indices);
	}
	else
	{
		glDrawArrays(RENDER_MODES[mode], 0, count);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(2, vbos);
}

internal void 
RenderBezierImmediate (RendererState *renderer_state, Mat4 mvp, r32 alpha,
							V3 p0, V3 p1, V3 p2, V3 p3, V3 color)
{
	const u32 count = 16;
	V3 positions[count];
	GenerateCubicBezier3D(p0, p1, p2, p3, count, positions);
	V3 colors[count];
	SET_COLORS(colors,color,count);

	// u8 *pop = PushMemoryPool(renderer_state->temp_memory);
	// V2 *positions_;
	// u32 count = AdaptivelySubdivideBezier(p0.xy, p1.xy, p2.xy, p3.xy, &positions_, renderer_state->temp_memory);
	// V3 *positions = AllocateFromMemoryPool(renderer_state->temp_memory, V3, count);
	// for (u32 i = 0; i < count; ++i) positions[i] = V2_V3(positions_[i],0.01f);
	// V3 *colors = AllocateFromMemoryPool(renderer_state->temp_memory, V3, count);
	// SET_COLORS(colors,color,count);

	RenderSolidImmediate (renderer_state, mvp, alpha, positions, colors, count, 0, 0, LINE_STRIP);
	
	// PopMemoryPool(renderer_state->temp_memory, pop);
}

internal void 
RenderShapeImmediate (RendererState *renderer_state, Mat4 mvp, r32 alpha, Shape *shape)
{
	for (u32 i = 0; i < shape->path_count; ++i)
	{
		Path *path = shape->paths+i;
		for (u32 i = 0; i < (path->point_count-1); i += 3)
		{
			V3 p0 = V2_V3(path->points[i+0],0.0f);
			V3 p1 = V2_V3(path->points[i+1],0.0f);
			V3 p2 = V2_V3(path->points[i+2],0.0f);
			V3 p3 = V2_V3(path->points[i+3],0.0f);
			RenderBezierImmediate(renderer_state,mvp,alpha, p0,p1,p2,p3, V3(1.0f,1.0f,1.0f));
		}
	}
}

internal void 
RendererRGBABitmapToGPU (u32 *texture_id, u32 pixel_width, u32 pixel_height, u8 *pixel_data)
{
	assert(texture_id)
	glGenTextures(1, texture_id);
	glBindTexture(GL_TEXTURE_2D, *texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, pixel_width, pixel_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}