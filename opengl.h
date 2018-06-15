#define GL_VERSION_3_2 0x00000302
#define GL_VERSION_3_3 0x00000303
#define GL_VERSION_4_0 0x00000400
#define GL_VERSION_4_1 0x00000401
#define GL_VERSION_4_2 0x00000402
#define GL_VERSION_4_3 0x00000403
#define GL_VERSION_4_4 0x00000404
#define GL_VERSION_4_5 0x00000405

// typedef int ptrdiff_t;
typedef unsigned int GLhandleARB;
typedef unsigned int GLbitfield;
typedef unsigned char GLboolean;
typedef unsigned char GLubyte;
typedef ptrdiff_t GLsizeiptr;
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef float GLclampf;
typedef float GLfloat;
typedef char GLchar;
typedef int GLsizei;
typedef int GLint;

#define GL_UNSIGNED_SHORT 0x1403
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_INT 0x1405
#define GL_SHORT 0x1402
#define GL_FLOAT 0x1406
#define GL_BYTE 0x1400
#define GL_INT 0x1404
#define GL_TRIANGLE_FAN 0x0006
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_LINE_LOOP 0x0002
#define GL_LINES 0x0001
#define GL_POINTS 0x0000
#define GL_TRIANGLE_STRIP 0x0005
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_COMPILE_STATUS 0x8B81
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_ARRAY_BUFFER 0x8892
#define GL_LINK_STATUS 0x8B82
#define GL_STATIC_DRAW 0x88E4
#define GL_CW 0x0900
#define GL_BACK 0x0405
#define GL_CULL_FACE 0x0B44
#define GL_DEPTH_TEST 0x0B71
#define GL_BLEND 0x0BE2
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_FUNC_ADD 0x8006
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_TEXTURE_2D 0x0DE1
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_R8 0x8229
#define GL_RED 0x1903
#define GL_ALPHA 0x1906
#define GL_INTENSITY 0x8049
#define GL_RGBA 0x1908
#define GL_BGRA 0x80E1
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_NEAREST 0x2600
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_FALSE 0
#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C
#define GL_RENDERER 0x1F01
#define GL_VENDOR 0x1F00
// #define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03

typedef GLuint GLCREATESHADER(GLenum);
GLCREATESHADER *glCreateShader;
typedef void GLSHADERSOURCE(GLuint,GLsizei,const GLchar **,const GLint*);
GLSHADERSOURCE *glShaderSource;
typedef void GLCOMPILESHADER(GLuint);
GLCOMPILESHADER *glCompileShader;
typedef void GLGETSHADERIV(GLuint,GLenum,GLint*);
GLGETSHADERIV *glGetShaderiv;
typedef void GLGETSHADERINFOLOG(GLuint,GLsizei,GLsizei*,GLchar*);
GLGETSHADERINFOLOG *glGetShaderInfoLog;
typedef void GLDELETESHADER(GLuint);
GLDELETESHADER *glDeleteShader;
typedef GLuint GLCREATEPROGRAM(void);
GLCREATEPROGRAM *glCreateProgram;
typedef void GLATTACHSHADER(GLuint,GLuint);
GLATTACHSHADER *glAttachShader;
typedef void GLLINKPROGRAM(GLuint);
GLLINKPROGRAM *glLinkProgram;
typedef void GLGETPROGRAMIV(GLuint, GLenum, GLint*);
GLGETPROGRAMIV *glGetProgramiv;
typedef void GLGETPROGRAMINFOLOG(GLuint, GLsizei, GLsizei*, GLchar*);
GLGETPROGRAMINFOLOG *glGetProgramInfoLog;
typedef void GLDELETEPROGRAM(GLuint);
GLDELETEPROGRAM *glDeleteProgram;
typedef void GLBINDATTRIBLOCATION(GLuint, GLuint, const GLchar*);
GLBINDATTRIBLOCATION *glBindAttribLocation;
typedef void GLDETACHSHADER(GLuint, GLuint);
GLDETACHSHADER *glDetachShader;
typedef void GLUSEPROGRAM(GLuint);
GLUSEPROGRAM *glUseProgram;
typedef GLint GLGETUNIFORMLOCATION(GLuint, const GLchar*);
GLGETUNIFORMLOCATION *glGetUniformLocation;
typedef void GLGENVERTEXARRAYS(GLsizei, GLuint*);
GLGENVERTEXARRAYS *glGenVertexArrays;
typedef void GLGENBUFFERS(GLsizei, GLuint*);
GLGENBUFFERS *glGenBuffers;
typedef void GLBINDVERTEXARRAY(GLuint);
GLBINDVERTEXARRAY *glBindVertexArray;
typedef void GLBINDBUFFER(GLenum, GLuint);
GLBINDBUFFER *glBindBuffer;
typedef void GLBUFFERDATA(GLenum, GLsizeiptr, const void*, GLenum);
GLBUFFERDATA *glBufferData;
typedef void GLVERTEXATTRIBPOINTER(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);
GLVERTEXATTRIBPOINTER *glVertexAttribPointer;
typedef void GLENABLEVERTEXATTRIBARRAY(GLuint);
GLENABLEVERTEXATTRIBARRAY *glEnableVertexAttribArray;
typedef void GLFRONTFACE(GLenum);
GLFRONTFACE *glFrontFace;
typedef void GLCULLFACE(GLenum);
GLCULLFACE *glCullFace;
typedef void GLENABLE(GLenum);
GLCULLFACE *glEnable;
typedef void GLBLENDFUNC(GLenum, GLenum);
GLBLENDFUNC *glBlendFunc;
typedef void GLBLENDEQUATION(GLenum);
GLBLENDEQUATION *glBlendEquation;
typedef void GLLINEWIDTH(GLfloat);
GLLINEWIDTH *glLineWidth;
typedef void GLPOINTSIZE(GLfloat);
GLPOINTSIZE *glPointSize;
typedef void GLVIEWPORT(GLint, GLint, GLsizei,GLsizei);
GLVIEWPORT *glViewport;
typedef void GLCLEARCOLOR(GLclampf,GLclampf,GLclampf,GLclampf);
GLCLEARCOLOR *glClearColor;
typedef void GLCLEAR(GLbitfield);
GLCLEAR *glClear;
typedef void GLUNIFORMMATRIX4FV(GLint, GLsizei, GLboolean, const GLfloat*);
GLUNIFORMMATRIX4FV *glUniformMatrix4fv;
typedef void GLUNIFORM1I(GLint, GLint);
GLUNIFORM1I *glUniform1i;
typedef void GLVERTEXATTRIB3F(GLuint, GLfloat, GLfloat, GLfloat);
GLVERTEXATTRIB3F *glVertexAttrib3f;
typedef void GLBINDTEXTURE(GLenum, GLuint);
GLBINDTEXTURE *glBindTexture;
typedef void GLDRAWARRAYS(GLenum, GLint, GLsizei);
GLDRAWARRAYS *glDrawArrays;
typedef void GLDRAWELEMENTS(GLenum, GLsizei, GLenum, const void*);
GLDRAWELEMENTS *glDrawElements;
typedef void GLUNIFORM3F(GLint, GLfloat, GLfloat, GLfloat);
GLUNIFORM3F *glUniform3f;
typedef void GLGENTEXTURES(GLsizei, GLuint*);
GLGENTEXTURES *glGenTextures;
typedef void GLTEXIMAGE2D(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*);
GLTEXIMAGE2D *glTexImage2D;
typedef void GLTEXPARAMETERI(GLenum, GLenum, GLint);
GLTEXPARAMETERI *glTexParameteri;
typedef void GLDELETEVERTEXARRAYS(GLsizei, const GLuint *);
GLDELETEVERTEXARRAYS *glDeleteVertexArrays;
typedef void GLDELETEBUFFERS(GLsizei, const GLuint *);
GLDELETEBUFFERS *glDeleteBuffers;
typedef void GLGETINTEGERV(GLenum, GLint *);
GLGETINTEGERV *glGetIntegerv;
typedef void GLUNIFORM1F(GLint,GLfloat);
GLUNIFORM1F *glUniform1f;
typedef const GLubyte* GLGETSTRING(GLenum);
GLGETSTRING *glGetString;
typedef void GLDISABLE(GLenum);
GLDISABLE *glDisable;

typedef HGLRC WINAPI WGLCREATECONTEXT(HDC);
WGLCREATECONTEXT *wglCreateContext;
typedef BOOL WINAPI WGLMAKECURRENT(HDC, HGLRC);
WGLMAKECURRENT *wglMakeCurrent;

global GLenum GL_RENDER_MODES[] = 
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
#define A_UV "in_uv"
#define A_TEXTURE "in_texture"
#define A_COLOR "in_color"
#define U_TEXTURE_SAMPLER "u_texture_sampler"
#define U_ALPHA "u_alpha"
#define U_COLOR "u_color"
#define U_MVP "u_mvp"

#define SOLID_VERTEX_TXT \
"#version 330\n"\
\
"uniform mat4 " U_MVP ";\n"\
\
"in vec2 " A_POSITION ";\n"\
\
"void main(void)\n"\
"{\n"\
	"  gl_Position = " U_MVP " * vec4(" A_POSITION ", 0.0, 1.0);\n"\
"}\n"

#define SOLID_FRAGMENT_TXT \
"#version 330\n"\
\
"precision highp float;\n"\
\
"uniform vec3 " U_COLOR ";\n"\
"uniform float " U_ALPHA ";\n"\
\
"out vec4 out_color;\n"\
\
"void main(void)\n"\
"{\n"\
	"  out_color = vec4(" U_COLOR "," U_ALPHA ");\n"\
"}\n"

#define TEXTURE_VERTEX_TXT \
"#version 330\n"\
\
"uniform mat4 "U_MVP";\n"\
\
"in vec2 "A_POSITION";\n"\
"in vec2 "A_UV";\n"\
\
"out vec2 uv;\n"\
\
"void main(void)\n"\
"{\n"\
	"gl_Position = "U_MVP" * vec4("A_POSITION", 0.0, 1.0);\n"\
	"uv = "A_UV";\n"\
"}\n"\

#define TEXTURE_FRAGMENT_TXT \
"#version 330\n"\
\
"precision highp float;\n"\
\
"uniform sampler2D "U_TEXTURE_SAMPLER";\n"\
"uniform float "U_ALPHA";\n"\
\
"in vec2 uv;\n"\
\
"out vec4 out_Color;\n"\
\
"void main(void)\n"\
"{\n"\
	"out_Color = texture("U_TEXTURE_SAMPLER", uv);\n"\
	"out_Color.a *= "U_ALPHA";\n"\
"}\n"\

struct SolidShader
{
	GLuint u_color_location;
	GLuint u_alpha_location;
	GLuint u_mvp_location;

	GLuint a_position;

	GLuint id;
};

struct TextureShader
{
	GLuint u_texture_sampler_location;
	GLuint u_alpha_location;
	GLuint u_mvp_location;

	GLuint a_position;
	GLuint a_uv;

	GLuint id;

	GLuint quad_vao;
};


struct OpenGLAPI
{
	u32 version;
	char *vendor_string;
	char *renderer_string;
	char *extensions_string;

	SolidShader solid_shader;
};

// internal void OpenGLInit(System *system, OpenGLAPI *opengl_api);
internal void InitOpenGLRenderer (System *system, Renderer *renderer);
internal void OpenGLInitTextureShader (TextureShader *texture_shader);
internal void BitmapToGPU (u32 *texture_id, u8 *pixel_data, u32 pixel_width, u32 pixel_height);
internal void RenderTexturedQuad (TextureShader texture_shader, Mat4 mvp, u32 texture_id, r32 alpha);