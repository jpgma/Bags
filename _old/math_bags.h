
#include <math.h> //TODO: get rid of this

#define SQRT2 (1.41421356237)
#define PI (3.1415927)
#define RAD_45  (PI*0.25f)
#define RAD_90  (PI*0.5f)
#define RAD_180 PI
#define RAD_360 (2.0f*PI)

struct V2
{
    r32 x,y;
};
#define V2(x,y) v2(x,y)
V2 operator * (V2 v, r32 s);
r32 operator * (V2 a, V2 b);
V2 operator + (V2 a, V2 b);
V2 operator - (V2 a, V2 b);
r32 Dot (V2 a, V2 b);
internal r32 Magnitude (V2 v);
internal void Normalize (V2 *v);
V2 v2(r32 x, r32 y) {return {x,y};}


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
#define V4(x,y,z,w) v4(x,y,z,w)
V4 operator + (V4 a, V4 b);
V4 operator - (V4 a, V4 b);
V4 operator * (V4 v, r32 s);
V4 v4(r32 x, r32 y, r32 z, r32 w) {return {x,y,z,w};}



struct Quat
{
    r32 x,y,z,w;
};
#define QUAT_ZERO {0.0f,0.0f,0.0f,1.0f}
#define QUAT_45 {0.0f,0.0f,-0.382683456f,0.923879504f}
#define QUAT_90 {0.0f,0.0f,-0.707106769f,0.707106769f}
#define QUAT_180 {0.0f,0.0f,-1.0f,-4.37113883e-08f}
inline r32 Dot(Quat a, Quat b);
internal V3 GetRotationAxis (Quat q);
internal void RotateObjectToInertial(Quat *q, r32 x, r32 y, r32 z);
internal void RotateInertialToObject(Quat *q, r32 x, r32 y, r32 z);
internal Quat Slerp (Quat a, Quat b, r32 t);



struct Mat4
{
    r32 m11, m12, m13, m14;
    r32 m21, m22, m23, m24;
    r32 m31, m32, m33, m34;
    r32 tx,  ty,  tz,  m44;
};
#define Mat4Identity {1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f}
#define Mat4ValueArray(m) {(m).m11, (m).m12, (m).m13, (m).m14, (m).m21, (m).m22, (m).m23, (m).m24,(m).m31, (m).m32, (m).m33, (m).m34,(m).tx,  (m).ty,  (m).tz,  (m).m44}
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
internal V3 ScreenToWorld (V3 screenp, Mat4 modelview, Mat4 projection, r32 *viewport);
internal void SetPerspectiveProjection (Mat4 *m, r32 near, r32 far, r32 fov_y, r32 ratio);
internal void SetFrustum (Mat4 *m, r32 left, r32 right, r32 bottom, r32 top, r32 near, r32 far);
internal void SetOrthographicProjection (Mat4 *m, r32 left, r32 right, r32 bottom, r32 top, r32 near, r32 far);
internal void SetLookAt (Mat4 *m, r32 eye_x, r32 eye_y, r32 eye_z, r32 center_x, r32 center_y, r32 center_z, r32 up_x, r32 up_y, r32 up_z);


struct MemoryPool;
internal u32 AdaptivelySubdivideBezier(V2 p0, V2 p1, V2 p2, V2 p3, V2 **points, MemoryPool *memory);
inline V2 InterpolateCubicBezier2D (V2 p0, V2 p1, V2 p2, V2 p3, r32 t);
internal u32 GenerateCubicBezier2D (V2 p0, V2 p1, V2 p2, V2 p3, u32 resolution, V2 *points);
internal u32 GenerateCubicBezier2D (V2 p0, V2 p1, V2 p2, V2 p3, u32 resolution, r32 *point_xys);
inline V3 InterpolateCubicBezier3D (V3 p0, V3 p1, V3 p2, V3 p3, r32 t);
internal u32 GenerateCubicBezier3D (V3 p0, V3 p1, V3 p2, V3 p3, u32 resolution, V3 *points);
inline V2 InterpolateQuadraticBezier2D (V2 p0, V2 p1, V2 p2, r32 t);
internal u32 GenerateQuadraticBezier2D (V2 p0, V2 p1, V2 p2, u32 resolution, V2 *points);
internal u32 GenerateQuadraticBezier2D (V2 p0, V2 p1, V2 p2, u32 resolution, r32 *point_xys);
inline V3 InterpolateQuadraticBezier3D (V3 p0, V3 p1, V3 p2, r32 t);
internal u32 GenerateQuadraticBezier3D (V3 p0, V3 p1, V3 p2, u32 resolution, V3 *points);
internal u32 CubicBezierLineIntersectionTs (V2 p0, V2 p1, V2 p2, V2 p3, V2 l0, V2 l1, r32 *t);
internal void SplitCubicBezierInT (V2 *curve, r32 t, V2 *left, V2 *right);

inline u32 Min (u32 a, u32 b);
inline r32 Absolute (r32 r);
inline b32 AreCollinear(V2 a, V2 b, V2 c);
inline r32 Clamp (r32 min, r32 max, r32 val);
internal b32 IsPointInsideCircle(V2 p, V2 c, r32 r);
internal b32 IsPointInsideRect(V2 point, V2 rcenter, V2 rsize);
internal b32 LineSegmentsIntersect (V2 p0, V2 p1, V2 p2, V2 p3);