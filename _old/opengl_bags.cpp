#define GL_VERSION_3_2 0x00000302
#define GL_VERSION_3_3 0x00000303
#define GL_VERSION_4_0 0x00000400
#define GL_VERSION_4_1 0x00000401
#define GL_VERSION_4_2 0x00000402
#define GL_VERSION_4_3 0x00000403
#define GL_VERSION_4_4 0x00000404
#define GL_VERSION_4_5 0x00000405

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

internal void OpenGLInit(System *system);