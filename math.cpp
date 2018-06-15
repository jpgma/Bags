//////////////////////
/// Math

#include <math.h>

V2 operator * (V2 v, r32 s){return{v.x*s, v.y*s};}
r32 operator * (V2 a, V2 b){return (a.x*b.y-a.y*b.x);}
V2 operator + (V2 a, V2 b){return{a.x+b.x, a.y+b.y};}
V2 operator - (V2 a, V2 b){return{a.x-b.x, a.y-b.y};}
r32 Dot(V2 a, V2 b) {return((a.x*b.x)+(a.y*b.y));}

//void operator += (V2 *a, V2 b) { a->x += b.x; a->y += b.y;}

internal r32
Magnitude (V2 v)
{
    r32 mag2 = (v.x * v.x) + (v.y * v.y);
    return sqrtf(mag2);
}

internal void
Normalize (V2 *v)
{
    r32 mag2 = (v->x * v->x) + (v->y * v->y);
    if(mag2 > 0.0f)
    {
        r32 one_over_mag = 1.0f / sqrtf(mag2);
        v->x *= one_over_mag;
        v->y *= one_over_mag;
    }
}

internal V2 
Normalized (V2 v)
{
    V2 res = v;
    Normalize(&res);
    return res;
}


V3 operator * (V3 v, r32 s){return{v.x*s, v.y*s, v.z*s};}
V3 operator + (V3 a, V3 b){return{a.x+b.x, a.y+b.y, a.z+b.z};}
V3 operator - (V3 a, V3 b){return{a.x-b.x, a.y-b.y, a.z-b.z};}
r32 operator * (V3 a, V3 b){return(a.x*b.y - a.y*b.x);} // TODO: fix

internal V3
Cross (V3 a, V3 b)
{
    return { a.y * b.z - a.z * b.y,
             a.z * b.x - a.x * b.z,
             a.x * b.y - a.y * b.x };
}

internal r32
Magnitude (V3 v)
{
    r32 mag2 = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
    return sqrtf(mag2);
}

internal void
Normalize (V3 *v)
{
    r32 mag2 = (v->x * v->x) + (v->y * v->y) + (v->z * v->z);
    if(mag2 > 0.0f)
    {
        r32 one_ver_mag = 1.0f / sqrtf(mag2);
        v->x *= one_ver_mag;
        v->y *= one_ver_mag;
        v->z *= one_ver_mag;
    }
}

V3 operator * (Mat4 m, V3 v)
{
    return
            {
				v.x*m.m11 + v.y*m.m21 + v.z*m.m31 + m.tx,
				v.x*m.m12 + v.y*m.m22 + v.z*m.m32 + m.ty,
				v.x*m.m13 + v.y*m.m23 + v.z*m.m33 + m.tz
            };
}

V4 operator * (Mat4 m, V4 v)
{
    return
            {
				v.x*m.m11 + v.y*m.m12 + v.z*m.m13 + v.w*m.m14,
				v.x*m.m21 + v.y*m.m22 + v.z*m.m23 + v.w*m.m24,
				v.x*m.m31 + v.y*m.m32 + v.z*m.m33 + v.w*m.m34,
				v.x*m.tx + v.y*m.ty + v.z*m.tz + v.w*m.m44
            };
}

Mat4 operator * (Mat4 a, Mat4 b)
{
    return
            {
				a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31 + a.m14*b.tx,
				a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32 + a.m14*b.ty,
				a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33 + a.m14*b.tz,
				a.m11*b.m14 + a.m12*b.m24 + a.m13*b.m34 + a.m14*b.m44,

				a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31 + a.m24*b.tx,
				a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32 + a.m24*b.ty,
				a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33 + a.m24*b.tz,
				a.m21*b.m14 + a.m22*b.m24 + a.m23*b.m34 + a.m24*b.m44,

				a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31 + a.m34*b.tx,
				a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32 + a.m34*b.ty,
				a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33 + a.m34*b.tz,
				a.m31*b.m14 + a.m32*b.m24 + a.m33*b.m34 + a.m34*b.m44,

				a.tx*b.m11 + a.ty*b.m21 + a.tz*b.m31 + a.m44*b.tx,
				a.tx*b.m12 + a.ty*b.m22 + a.tz*b.m32 + a.m44*b.ty,
				a.tx*b.m13 + a.ty*b.m23 + a.tz*b.m33 + a.m44*b.tz,
				a.tx*b.m14 + a.ty*b.m24 + a.tz*b.m34 + a.m44+b.m44
            };
}

internal Mat4 Inverse (Mat4 m);

internal void
Identity(Mat4 *m)
{
    m->m11 = 1.0f; m->m12 = 0.0f; m->m13 = 0.0f; m->m14 = 0.0f;
    m->m21 = 0.0f; m->m22 = 1.0f; m->m23 = 0.0f; m->m24 = 0.0f;
    m->m31 = 0.0f; m->m32 = 0.0f; m->m33 = 1.0f; m->m34 = 0.0f;
    m->tx = 0.0f;  m->ty = 0.0f;  m->tz = 0.0f;  m->m44 = 1.0f;
}

internal Mat4
Transposed(Mat4 m)
{
    Mat4 t = m;

    t.m12 = m.m21;
    t.m13 = m.m31;
    t.m14 = m.tx;
    t.m21 = m.m12;
    t.m23 = m.m32;
    t.m24 = m.ty;
    t.m31 = m.m13;
    t.m32 = m.m23;
    t.m34 = m.tz;
    t.tx = m.m14;
    t.ty = m.m24;
    t.tz = m.m34;

    return t;
}

internal void
ApplyScale(Mat4 *m, V3 s)
{
    m->m11 = m->m11 * s.x;
    m->m12 = m->m12 * s.x;
    m->m13 = m->m13 * s.x;
    m->m14 = m->m14 * s.x;
    m->m21 = m->m21 * s.y;
    m->m22 = m->m22 * s.y;
    m->m23 = m->m23 * s.y;
    m->m24 = m->m24 * s.y;
    m->m31 = m->m31 * s.z;
    m->m32 = m->m32 * s.z;
    m->m33 = m->m33 * s.z;
    m->m34 = m->m34 * s.z;
}

internal void
SetRotation(Mat4 *m, Quat q)
{
    r32 ww = 2.0f * q.w;
    r32 xx = 2.0f * q.x;
    r32 yy = 2.0f * q.y;
    r32 zz = 2.0f * q.z;

    m->m11 = 1.0f - yy*q.y - zz*q.z;
    m->m12 = xx*q.y + ww*q.z;
    m->m13 = xx*q.z - ww*q.x;
    m->m14 = 0.0f;
    m->m21 = xx*q.y - ww*q.z;
    m->m22 = 1.0f - xx*q.x - zz*q.z;
    m->m23 = yy*q.z + ww*q.x;
    m->m24 = 0.0f;
    m->m31 = xx*q.z + ww*q.y;
    m->m32 = yy*q.z - ww*q.x;
    m->m33 = 1.0f - xx*q.x - yy*q.y;
    m->m34 = 0.0f;
}

internal void
ApplyTranslation(Mat4 *m, V3 t)
{
    m->tx += t.x;
    m->ty += t.y;
    m->tz += t.z;
}

internal void
ApplyTransforms (Mat4 *m, V3 t, V3 s, Quat r)
{
    Identity(m);

    Mat4 tmp;
    Identity(&tmp);

    //Dimensionamento
    ApplyScale(m,s);

    //Rotacao //TODO: Fazer funcionar
    SetRotation(&tmp,r);
    *m = *m * tmp;
    
    //Translacao
    ApplyTranslation(m,t);
}

internal void
SetLookAt (Mat4 *m,
             r32 eye_x, r32 eye_y, r32 eye_z,
             r32 center_x, r32 center_y, r32 center_z,
             r32 up_x, r32 up_y, r32 up_z)
{
    r32 fx = center_x - eye_x;
    r32 fy = center_y - eye_y;
    r32 fz = center_z - eye_z;

    r32 rlf = 1.0f / Magnitude(V3(fx, fy, fz));
    fx *= rlf;
    fy *= rlf;
    fz *= rlf;

    r32 sx = fy * up_z - fz * up_y;
    r32 sy = fz * up_x - fx * up_z;
    r32 sz = fx * up_y - fy * up_x;

    r32 rls = 1.0f / Magnitude(V3(sx, sy, sz));
    sx *= rls;
    sy *= rls;
    sz *= rls;

    r32 ux = sy * fz - sz * fy;
    r32 uy = sz * fx - sx * fz;
    r32 uz = sx * fy - sy * fx;

    m->m11 = sx;
    m->m12 = ux;
    m->m13 = -fx;
    m->m14 = 0.0f;

    m->m21 = sy;
    m->m22 = uy;
    m->m23 = -fy;
    m->m24 = 0.0f;

    m->m31 = sz;
    m->m32 = uz;
    m->m33 = -fz;
    m->m34 = 0.0f;

    m->tx = -eye_x;
    m->ty = -eye_y;
    m->tz = -eye_z;
    m->m44 = 1.0f;
}

internal void
SetOrthographicProjection (Mat4 *m,
                           r32 left, r32 right,
                           r32 bottom, r32 top,
                           r32 near, r32 far)
{
    r32 r_width  = 1.0f / (right - left);
    r32 r_height = 1.0f / (top - bottom);
    r32 r_depth  = 1.0f / (far - near);
    r32 x =  2.0f * (r_width);
    r32 y =  2.0f * (r_height);
    r32 z = -2.0f * (r_depth);
    r32 tx = -(right + left) * r_width;
    r32 ty = -(top + bottom) * r_height;
    r32 tz = -(far + near) * r_depth;

    m->m11 = x;
    m->m22  = y;
    m->m33 = z;
    m->tx = tx;
    m->ty = ty;
    m->tz = tz;
    m->m44 = 1.0f;
    m->m12 = 0.0f;
    m->m13 = 0.0f;
    m->m14 = 0.0f;
    m->m21 = 0.0f;
    m->m23 = 0.0f;
    m->m24 = 0.0f;
    m->m31 = 0.0f;
    m->m32 = 0.0f;
    m->m34 = 0.0f;
}

inline r32 
Map0to1(r32 min, r32 max, r32 x)
{
    r32 res = (x-min) / (max-min);
    if(res < 0.0f) res = 0.0f;
    else if(res > 1.0f) res = 1.0f;
    return res;
}

inline r32 
MapMinToMax (r32 min0, r32 max0, r32 t0, r32 min, r32 max)
{
    r32 res = (t0-min0) / (max0-min0);
    if(res < 0.0f) res = 0.0f;
    else if(res > 1.0f) res = 1.0f;
    res = min + (res*(max-min));
    return res;
}

internal V2
InterpolateCubicBezier2D(V2 p0, V2 p1, V2 p2, V2 p3, r32 t)
{
    r32 one_minus_t = (1.0f - t);
    r32 one_minus_t_squared = one_minus_t*one_minus_t;
    r32 one_minus_t_cubed = one_minus_t_squared*(1.0f - t);
    V2 p = (p0*one_minus_t_cubed) + (p1*(3.0f*t*one_minus_t_squared)) + (p2*(3.0f*t*t*one_minus_t)) + (p3*(t*t*t));
    return p;
}

internal b32
IsPointInsideRect(V2 point, V2 rcenter, V2 rsize)
{
    b32 res = false;
    if((point.x > rcenter.x-(rsize.x*0.5f)) &&
        (point.x < rcenter.x+(rsize.x*0.5f)) &&
        (point.y > rcenter.y-(rsize.y*0.5f)) &&
        (point.y < rcenter.y+(rsize.y*0.5f)))
    {
        res = true;
    }
    return res;
}

internal b32
IsPointInsideCircle(V2 p, V2 c, r32 r)
{
    b32 res = false;
    V2 d = p - c;
    if(Magnitude(d) < r) res = true;
    return res;
}