#define SQRT2 (1.41421356237)
#define PI (3.1415927)
#define RAD_45  (PI*0.25f)
#define RAD_90  (PI*0.5f)
#define RAD_180 PI
#define RAD_360 (2.0f*PI)

#pragma pack(push, 1) 
struct V2
{
	r32 x,y;
};
#pragma pack(pop)
#define V2(x,y) v2(x,y)
V2 operator * (V2 v, r32 s);
r32 operator * (V2 a, V2 b);
V2 operator + (V2 a, V2 b);
V2 operator - (V2 a, V2 b);
r32 Dot (V2 a, V2 b);
internal r32 Magnitude (V2 v);
internal void Normalize (V2 *v);
internal V2 Normalized (V2 v);
V2 v2(r32 x, r32 y) {return {x,y};}

#pragma pack(push, 1) 
union V3
{
    struct
    {
        r32 x,y,z;
    };

    struct
    {
        r32 r,g,b;
    };

    V2 xy;
};
#pragma pack(pop)
#define V3(x,y,z) v3(x,y,z)
#define V2_V3(v,z) v3((v).x, (v).y, z)
V3 operator * (V3 v, r32 s);
V3 operator + (V3 a, V3 b);
V3 operator - (V3 a, V3 b);
r32 operator * (V3 a, V3 b);
internal V3 Cross (V3 a, V3 b);
internal r32 Magnitude (V3 v);
internal void Normalize (V3 *v);
V3 v3(r32 x, r32 y, r32 z) {return {x,y,z};}

#pragma pack(push, 1) 
union V4
{
    struct
    {
        r32 x,y,z,w;
    };

    struct
    {
        r32 r,g,b,a;
    };

    V2 xy;
    V3 xyz;
    V3 rgb;
};
#pragma pack(pop)
#define V4(x,y,z,w) v4(x,y,z,w)
V4 v4(r32 x, r32 y, r32 z, r32 w) {return {x,y,z,w};}

#pragma pack(push, 1) 
struct Quat
{
    r32 x,y,z,w;
};
#pragma pack(pop)
#define QUAT_ZERO {0.0f,0.0f,0.0f,1.0f}
#define QUAT_45 {0.0f,0.0f,-0.382683456f,0.923879504f}
#define QUAT_90 {0.0f,0.0f,-0.707106769f,0.707106769f}
#define QUAT_180 {0.0f,0.0f,-1.0f,-4.37113883e-08f}

#pragma pack(push, 1)
struct Mat4
{
	r32 m11, m12, m13, m14;
    r32 m21, m22, m23, m24;
    r32 m31, m32, m33, m34;
    r32 tx,  ty,  tz,  m44;
};
#pragma pack(pop)
#define Mat4Identity {1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f}
V3 operator * (Mat4 m, V3 v);
V4 operator * (Mat4 m, V4 v);
Mat4 operator * (Mat4 a, Mat4 b);
internal Mat4 Inverse (Mat4 m);
internal void Identity (Mat4 *m);
internal Mat4 Transposed (Mat4 m);
internal void ApplyScale (Mat4 *m, V3 s);
internal void SetRotation (Mat4 *m, Quat q);
internal void ApplyTranslation (Mat4 *m, V3 t);
internal void ApplyTransforms (Mat4 *m, V3 t, V3 s, Quat r);
internal void SetLookAt (Mat4 *m, r32 eye_x, r32 eye_y, r32 eye_z, r32 center_x, r32 center_y, r32 center_z, r32 up_x, r32 up_y, r32 up_z);
internal void SetOrthographicProjection (Mat4 *m, r32 left, r32 right, r32 bottom, r32 top, r32 near, r32 far);

inline r32 Map0to1(r32 min, r32 max, r32 x);
internal V2 InterpolateCubicBezier2D(V2 p0, V2 p1, V2 p2, V2 p3, r32 t);
internal b32 IsPointInsideRect(V2 point, V2 rcenter, V2 rsize);
internal b32 IsPointInsideCircle(V2 p, V2 c, r32 r);