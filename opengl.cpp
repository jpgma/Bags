internal void
LoadGLFunctions()
{
	HMODULE opengl32 = LoadLibraryA("opengl32.dll");
	glCreateShader = (GLCREATESHADER*)GetFunctionAddress(opengl32, "glCreateShader");
	glShaderSource = (GLSHADERSOURCE*)GetFunctionAddress(opengl32, "glShaderSource");
	glCompileShader = (GLCOMPILESHADER*)GetFunctionAddress(opengl32, "glCompileShader");
	glGetShaderiv = (GLGETSHADERIV*)GetFunctionAddress(opengl32, "glGetShaderiv");
	glGetShaderInfoLog = (GLGETSHADERINFOLOG*)GetFunctionAddress(opengl32, "glGetShaderInfoLog");
	glDeleteShader = (GLDELETESHADER*)GetFunctionAddress(opengl32, "glDeleteShader");
	glCreateProgram = (GLCREATEPROGRAM*)GetFunctionAddress(opengl32, "glCreateProgram");
	glAttachShader = (GLATTACHSHADER*)GetFunctionAddress(opengl32, "glAttachShader");
	glLinkProgram = (GLLINKPROGRAM*)GetFunctionAddress(opengl32, "glLinkProgram");
	glGetProgramiv = (GLGETPROGRAMIV*)GetFunctionAddress(opengl32, "glGetProgramiv");
	glGetProgramInfoLog = (GLGETPROGRAMINFOLOG*)GetFunctionAddress(opengl32, "glGetProgramInfoLog");
	glDeleteProgram = (GLDELETEPROGRAM*)GetFunctionAddress(opengl32, "glDeleteProgram");
	glBindAttribLocation = (GLBINDATTRIBLOCATION*)GetFunctionAddress(opengl32, "glBindAttribLocation");
	glDetachShader = (GLDETACHSHADER*)GetFunctionAddress(opengl32, "glDetachShader");
	glUseProgram = (GLUSEPROGRAM*)GetFunctionAddress(opengl32, "glUseProgram");
	glGetUniformLocation = (GLGETUNIFORMLOCATION*)GetFunctionAddress(opengl32, "glGetUniformLocation");
	glGenVertexArrays = (GLGENVERTEXARRAYS*)GetFunctionAddress(opengl32, "glGenVertexArrays");
	glGenBuffers = (GLGENBUFFERS*)GetFunctionAddress(opengl32, "glGenBuffers");
	glBindVertexArray = (GLBINDVERTEXARRAY*)GetFunctionAddress(opengl32, "glBindVertexArray");
	glBindBuffer = (GLBINDBUFFER*)GetFunctionAddress(opengl32, "glBindBuffer");
	glBufferData = (GLBUFFERDATA*)GetFunctionAddress(opengl32, "glBufferData");
	glVertexAttribPointer = (GLVERTEXATTRIBPOINTER*)GetFunctionAddress(opengl32, "glVertexAttribPointer");
	glEnableVertexAttribArray = (GLENABLEVERTEXATTRIBARRAY*)GetFunctionAddress(opengl32, "glEnableVertexAttribArray");
	glFrontFace = (GLFRONTFACE*)GetFunctionAddress(opengl32, "glFrontFace");
	glCullFace = (GLCULLFACE*)GetFunctionAddress(opengl32, "glCullFace");
	glEnable = (GLENABLE*)GetFunctionAddress(opengl32, "glEnable");
	glBlendFunc = (GLBLENDFUNC*)GetFunctionAddress(opengl32, "glBlendFunc");
	glBlendEquation = (GLBLENDEQUATION*)GetFunctionAddress(opengl32, "glBlendEquation");
	glLineWidth = (GLLINEWIDTH*)GetFunctionAddress(opengl32, "glLineWidth");
	glPointSize = (GLPOINTSIZE*)GetFunctionAddress(opengl32, "glPointSize");
	glViewport = (GLVIEWPORT*)GetFunctionAddress(opengl32, "glViewport");
	glClearColor = (GLCLEARCOLOR*)GetFunctionAddress(opengl32, "glClearColor");
	glUniformMatrix4fv = (GLUNIFORMMATRIX4FV*)GetFunctionAddress(opengl32, "glUniformMatrix4fv");
	glUniform1i = (GLUNIFORM1I*)GetFunctionAddress(opengl32, "glUniform1i");
	glVertexAttrib3f = (GLVERTEXATTRIB3F*)GetFunctionAddress(opengl32, "glVertexAttrib3f");
	glBindTexture = (GLBINDTEXTURE*)GetFunctionAddress(opengl32, "glBindTexture");
	glDrawArrays = (GLDRAWARRAYS*)GetFunctionAddress(opengl32, "glDrawArrays");
	glDrawElements = (GLDRAWELEMENTS*)GetFunctionAddress(opengl32, "glDrawElements");
	glUniform3f = (GLUNIFORM3F*)GetFunctionAddress(opengl32, "glUniform3f");
	glGenTextures = (GLGENTEXTURES*)GetFunctionAddress(opengl32, "glGenTextures");
	glTexImage2D = (GLTEXIMAGE2D*)GetFunctionAddress(opengl32, "glTexImage2D");
	glTexParameteri = (GLTEXPARAMETERI*)GetFunctionAddress(opengl32, "glTexParameteri");
	glClear = (GLCLEAR*)GetFunctionAddress(opengl32, "glClear");
	glDeleteVertexArrays = (GLDELETEVERTEXARRAYS*)GetFunctionAddress(opengl32, "glDeleteVertexArrays");
	glDeleteBuffers = (GLDELETEBUFFERS*)GetFunctionAddress(opengl32, "glDeleteBuffers");
	glGetIntegerv = (GLGETINTEGERV*)GetFunctionAddress(opengl32, "glGetIntegerv");
	glUniform1f = (GLUNIFORM1F*)GetFunctionAddress(opengl32, "glUniform1f");
	glGetString = (GLGETSTRING*)GetFunctionAddress(opengl32, "glGetString");
	glDisable = (GLDISABLE*)GetFunctionAddress(opengl32, "glDisable");
}

internal void
OpenGLInit (System *system, OpenGLAPI *opengl_api)
{
	Win32System *win32sys = (Win32System*)system->platform_specific;

	PIXELFORMATDESCRIPTOR desired_pf = {};
	desired_pf.nSize = sizeof(desired_pf);
	desired_pf.nVersion = 1;
	desired_pf.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER;
	desired_pf.cColorBits = 32;
	desired_pf.cAlphaBits = 32;
	desired_pf.iLayerType = PFD_MAIN_PLANE;
	
	s32 suggested_pf_index = ChoosePixelFormat(win32sys->hdc, &desired_pf);
	PIXELFORMATDESCRIPTOR suggested_pf = {};
	DescribePixelFormat(win32sys->hdc, suggested_pf_index, sizeof(suggested_pf), &suggested_pf);
	SetPixelFormat(win32sys->hdc, suggested_pf_index, &suggested_pf);

	HGLRC glrc = wglCreateContext(win32sys->hdc);
	if(glrc && wglMakeCurrent(win32sys->hdc, glrc))
	{
		LoadGLFunctions();

		s32 major,minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		opengl_api->version = ((u32) major << 8) | ((u32)minor);

		opengl_api->extensions_string = (char *)glGetString(GL_EXTENSIONS);
		opengl_api->renderer_string = (char *)glGetString(GL_RENDERER);
		opengl_api->vendor_string = (char *)glGetString(GL_VENDOR);
	}
	else assert(0);
}

internal GLuint
OpenGLCompileShader (const char *source, GLenum type)
{
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &source, 0);

	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint max_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

		char *log = (char*)PlatformAllocateMemory(max_length);
		glGetShaderInfoLog(shader, max_length, &max_length, (GLchar*)log);
		
		glDeleteShader(shader);

		// ShowSystemPopupTextWindow("error", log);
		MessageBoxA(NULL, log, "Error", MB_ICONEXCLAMATION|MB_OK);

		return 0;
	}

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
		GLint max_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

		char *log = (char*)PlatformAllocateMemory(max_length);
		glGetProgramInfoLog(program, max_length, &max_length, (GLchar*)log);
		
		glDeleteProgram(program);

		// ShowSystemPopupTextWindow("error", log);
		MessageBoxA(NULL, log, "Error", MB_ICONEXCLAMATION|MB_OK);
		
		PlatformDeallocateMemory((void*)log, max_length);
	}
}

internal void
OpenGLInitSolidShader (SolidShader *solid_shader)
{
	GLuint vertex_shader = OpenGLCompileShader(SOLID_VERTEX_TXT, GL_VERTEX_SHADER);
	GLuint fragment_shader = OpenGLCompileShader(SOLID_FRAGMENT_TXT, GL_FRAGMENT_SHADER);
	solid_shader->id = OpenGLCreateShaderProgram(vertex_shader, fragment_shader);
	
	solid_shader->a_position = 0;
	glBindAttribLocation(solid_shader->id, solid_shader->a_position, A_POSITION);
	
	OpenGLLinkShaderProgram(solid_shader->id);
	glDetachShader(solid_shader->id, vertex_shader);
	glDetachShader(solid_shader->id, fragment_shader);
	glUseProgram(solid_shader->id);
	solid_shader->u_color_location = glGetUniformLocation(solid_shader->id, U_COLOR);
	solid_shader->u_alpha_location = glGetUniformLocation(solid_shader->id, U_ALPHA);
	solid_shader->u_mvp_location = glGetUniformLocation(solid_shader->id, U_MVP);
	glUseProgram(0);
}

internal void
OpenGLInitTextureShader (TextureShader *texture_shader)
{
	GLuint vertex_shader = OpenGLCompileShader(TEXTURE_VERTEX_TXT, GL_VERTEX_SHADER);
	GLuint fragment_shader = OpenGLCompileShader(TEXTURE_FRAGMENT_TXT, GL_FRAGMENT_SHADER);
	texture_shader->id = OpenGLCreateShaderProgram(vertex_shader, fragment_shader);
	
	texture_shader->a_position = 0;
	glBindAttribLocation(texture_shader->id, texture_shader->a_position, A_POSITION);
	texture_shader->a_uv = 0;
	glBindAttribLocation(texture_shader->id, texture_shader->a_uv, A_UV);
	
	OpenGLLinkShaderProgram(texture_shader->id);
	glDetachShader(texture_shader->id, vertex_shader);
	glDetachShader(texture_shader->id, fragment_shader);
	
	glUseProgram(texture_shader->id);

	texture_shader->u_texture_sampler_location = glGetUniformLocation(texture_shader->id, U_TEXTURE_SAMPLER);
	texture_shader->u_alpha_location = glGetUniformLocation(texture_shader->id, U_ALPHA);
	texture_shader->u_mvp_location = glGetUniformLocation(texture_shader->id, U_MVP);
	
	glGenVertexArrays(1, &texture_shader->quad_vao);
	u32 vbos[2];
	glGenBuffers(2, vbos);
	glBindVertexArray(texture_shader->quad_vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(GLfloat), rectangle_positions, GL_STATIC_DRAW);
	glVertexAttribPointer(texture_shader->a_position, 2, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(texture_shader->a_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(GLfloat), rectangle_uv, GL_STATIC_DRAW);
	glVertexAttribPointer(texture_shader->a_uv, 2, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(texture_shader->a_uv);

	glUseProgram(0);
}

internal void
InitOpenGLRenderer (System *system, Renderer *renderer)
{
	Win32System *win32sys = (Win32System *)system->platform_specific;
	OpenGLAPI *opengl_api = (OpenGLAPI *)renderer->api_specific;

	OpenGLInit(system, opengl_api);
	OpenGLInitSolidShader(&opengl_api->solid_shader);

	// glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	glLineWidth(1.5f);
	glPointSize(10.0f);

	RECT window_rect = {};
	GetWindowRect(win32sys->window, &window_rect);
	r32 width = window_rect.right - window_rect.left;
	r32 height = window_rect.bottom - window_rect.top;
	renderer->window_width = (s32) width;
	renderer->window_height = (s32) height;

	glViewport(0.0f,0.0f,renderer->window_width,renderer->window_height);

	SetLookAt(&renderer->view,
				0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);

	r32 ratio = width/height;
	SetOrthographicProjection(&renderer->projection,
								-ratio, +ratio,
								-1.0f, +1.0f,
								+0.0f, +1.0f);

	SetOrthographicProjection(&renderer->gui_projection,
								-ratio, +ratio,
								-1.0f, +1.0f,
								+0.0f, +1.0f);
	
	renderer->initialized = true;
}

internal void
RenderSolid (Renderer *renderer, Mat4 mvp, V4 color, u32 id, u32 count, u16 *indices, u16 index_count, u32 mode)
{
	SolidShader solid_shader = ((OpenGLAPI*)renderer->api_specific)->solid_shader;
	glUseProgram(solid_shader.id);
	glUniformMatrix4fv(solid_shader.u_mvp_location, 1, FALSE, (GLfloat*)&mvp);
	glUniform3f(solid_shader.u_color_location, (GLfloat)color.r, (GLfloat)color.g, (GLfloat)color.b);
	glUniform1f(solid_shader.u_alpha_location, (GLfloat)color.a);
	glBindVertexArray(id);
	if(indices)
	{
		glDrawElements(GL_RENDER_MODES[mode], count, GL_UNSIGNED_SHORT, indices);
	}
	else
	{
		glDrawArrays(GL_RENDER_MODES[mode], 0, count);
	}
}

internal void
RenderSolidImmediate (Renderer *renderer, Mat4 mvp, V4 color, r32 *positions, u32 count, u16 *indices, u16 index_count, u32 mode)
{
	SolidShader solid_shader = ((OpenGLAPI*)renderer->api_specific)->solid_shader;

	u32 vaos[1];
	glGenVertexArrays(1, vaos);
	u32 vbos[1];
	glGenBuffers(1, vbos);
	glUseProgram(solid_shader.id);
	glBindVertexArray(vaos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, count*2*sizeof(GLfloat), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(solid_shader.a_position, 2, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(solid_shader.a_position);

	glUseProgram(solid_shader.id);
	glUniformMatrix4fv(solid_shader.u_mvp_location, 1, FALSE, (GLfloat*)&mvp);
	glUniform3f(solid_shader.u_color_location, (GLfloat)color.r, (GLfloat)color.g, (GLfloat)color.b);
	glUniform1f(solid_shader.u_alpha_location, (GLfloat)color.a);
	glBindVertexArray(vaos[0]);
	if(indices)
	{
		glDrawElements(GL_RENDER_MODES[mode], index_count, GL_UNSIGNED_SHORT, indices);
	}
	else
	{
		glDrawArrays(GL_RENDER_MODES[mode], 0, count);
	}

	glDeleteVertexArrays(1, vaos);
	glDeleteBuffers(1, vbos);
}

internal void
BitmapToGPU (u32 *texture_id, u8 *pixel_data, u32 pixel_width, u32 pixel_height)
{
	glGenTextures(1, texture_id);
	glBindTexture(GL_TEXTURE_2D, *texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, pixel_width, pixel_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

internal void
RenderTexturedQuad (TextureShader texture_shader, Mat4 mvp, u32 texture_id, r32 alpha)
{
	glUseProgram(texture_shader.id);
	glBindVertexArray(texture_shader.quad_vao);

	glUniformMatrix4fv(texture_shader.u_mvp_location, 1, FALSE, (GLfloat*)&mvp);
	glUniform1f(texture_shader.u_alpha_location, (GLfloat)alpha);
	glUniform1i(texture_shader.u_texture_sampler_location, 0);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, rectangle_indices);
}
