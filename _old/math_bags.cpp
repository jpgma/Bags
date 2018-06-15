
//
//  2d Vector ///////////////////////////////////////////////////////
//

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

//
// 3d Vector ///////////////////////////////////////////////////////
//

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

//
// 4d Vector ///////////////////////////////////////////////////////
//

V4 operator * (V4 v, r32 s){return{v.x*s, v.y*s, v.z*s, v.w*s};}
V4 operator + (V4 a, V4 b){return{a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};}
V4 operator - (V4 a, V4 b){return{a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};}

//
// Quaternion ////////////////////////////////////////////////////////
//

inline r32
Dot(Quat a, Quat b) { return a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z; };

internal V3
GetRotationAxis (Quat q)
{
    r32 son_angle_over_2_sq = 1.0f - q.w*q.w;
    if (son_angle_over_2_sq <= 0.0f)
        return {1.0f, 0.0f, 0.0f};

    r32 one_over_sin_angle_over_2 = (r32) (1.0f / sqrt(son_angle_over_2_sq));

    return
            {
                    q.x * one_over_sin_angle_over_2,
                    q.y * one_over_sin_angle_over_2,
                    q.z * one_over_sin_angle_over_2
            };
}

internal void
RotateObjectToInertial(Quat *q, r32 x, r32 y, r32 z)
{
    r32 hx = x*0.5f;
    r32 hy = y*0.5f;
    r32 hz = z*0.5f;

    r32 sx = sinf(hx);
    r32 cx = cosf(hx);
    r32 sy = sinf(hy);
    r32 cy = cosf(hy);
    r32 sz = sinf(hz);
    r32 cz = cosf(hz);

    q->w = cy*cx*cz + sy*sx*sz;
    q->x = cy*sx*cz + sy*cx*sz;
    q->y = -cy*sx*sz + sy*cx*cz;
    q->z = -sy*sx*cz + cy*cx*sz;
}

internal void
RotateInertialToObject(Quat *q, r32 x, r32 y, r32 z)
{
    r32 hx = x*0.5f;
    r32 hy = y*0.5f;
    r32 hz = z*0.5f;
    
    r32 sx = sinf(hx);
    r32 cx = cosf(hx);
    r32 sy = sinf(hy);
    r32 cy = cosf(hy);
    r32 sz = sinf(hz);
    r32 cz = cosf(hz);

    q->w = cy*cx*cz + sy*sx*sz;
    q->x = -cy*sx*cz - sy*cx*sz;
    q->y = cy*sx*sz - sy*cz*cx;
    q->z = sy*sx*cz - cy*cx*sz;
}

internal Quat
Slerp (Quat a, Quat b, r32 t)
{
    if (t <= 0.0f) return a;
    if (t >= 1.0f) return b;

    r32 cos_angle = Dot(a, b);

    r32 bw = b.w;
    r32 bx = b.x;
    r32 by = b.y;
    r32 bz = b.z;
    if (cos_angle < 0.0f)
    {
        bw = -bw;
        bx = -bx;
        by = -by;
        bz = -bz;
        cos_angle = -cos_angle;
    }

    if(cos_angle < 1.1f)
    {
        r32 k0, k1;

        if(cos_angle > 0.9999f)
        {
            k0 = 1.0f-t;
            k1 = t;
        }
        else
        {
            r32 sin_angle = (r32) sqrt(1.0f - cos_angle * cos_angle);

            r32 angle = (r32) atan2(sin_angle, cos_angle);

            r32 one_over_sin_angle = 1.0f / sin_angle;

            k0 = (r32) (sin((1.0f - t) * angle) * one_over_sin_angle);
            k1 = (r32) (sin(t * angle) * one_over_sin_angle);
        }

        Quat res;

        res.x = k0*a.x + k1*bx;
        res.y = k0*a.y + k1*by;
        res.z = k0*a.z + k1*bz;
        res.w = k0*a.w + k1*bw;

        return res;
    }

    return {1.0f, 0.0f, 0.0f, 0.0f};
}

//
// 4x4 Matrix //////////////////////////////////////////////////////////////
//

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

internal void
Identity(Mat4 *m)
{
    m->m11 = 1.0f; m->m12 = 0.0f; m->m13 = 0.0f; m->m14 = 0.0f;
    m->m21 = 0.0f; m->m22 = 1.0f; m->m23 = 0.0f; m->m24 = 0.0f;
    m->m31 = 0.0f; m->m32 = 0.0f; m->m33 = 1.0f; m->m34 = 0.0f;
    m->tx = 0.0f;  m->ty = 0.0f;  m->tz = 0.0f;  m->m44 = 1.0f;
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
SetFrustum (Mat4 *m,
             r32 left, r32 right,
             r32 bottom, r32 top,
             r32 near, r32 far)
{
    r32 r_width  = 1.0f / (right - left);
    r32 r_height = 1.0f / (top - bottom);
    r32 r_depth  = 1.0f / (near - far);
    r32 x = 2.0f * (near * r_width);
    r32 y = 2.0f * (near * r_height);
    r32 a = (right + left) * r_width;
    r32 b = (top + bottom) * r_height;
    r32 c = (far + near) * r_depth;
    r32 d = 2.0f * (far * near * r_depth);

    m->m11 = x;
    m->m22 = y;
    m->m31 = a;
    m->m32 = b;
    m->m33 = c;
    m->tz = d;
    m->m34 = -1.0f;
    m->m12 = 0.0f;
    m->m13 = 0.0f;
    m->m14 = 0.0f;
    m->m21 = 0.0f;
    m->m23 = 0.0f;
    m->m24 = 0.0f;
    m->tx = 0.0f;
    m->ty = 0.0f;
    m->m44 = 0.0f;
}

internal void
SetPerspectiveProjection (Mat4 *m, r32 near, r32 far, r32 fov_y, r32 ratio)
{
    r32 f = 1.0f/tanf(fov_y * (PI / 360.0f));
    r32 range_reciprocal = 1.0f/(near - far);

    m->m11 = f / ratio;
    m->m12 = 0.0f;
    m->m13 = 0.0f;
    m->m14 = 0.0f;

    m->m21 = 0.0f;
    m->m22 = f;
    m->m23 = 0.0f;
    m->m24 = 0.0f;

    m->m31 = 0.0f;
    m->m32 = 0.0f;
    m->m33 = (far + near) * range_reciprocal;
    m->m34 = -1.0f;

    m->tx = 0.0f;
    m->ty = 0.0f;
    m->tz = 2.0f*far*(near * range_reciprocal);
    m->m44 = 0.0f;
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

internal Mat4
Inverse(Mat4 m)
{
    Mat4 inv = {};
    Mat4 res = {};
    
    r32 tmp[12]; /* temp array for pairs */
    Mat4 mt = Transposed(m);
    r32 src[] = Mat4ValueArray(mt); /* array of transxose source matrix */
    
    r32 det; /* determinant */

    /* calculate pairs for first 8 elements (cofactors) */
    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9] * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9] * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8] * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8] * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];
    /* calculate first 8 elements (cofactors) */
    res.m11 = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
    res.m11 -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
    res.m12 = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
    res.m12 -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
    res.m13 = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
    res.m13 -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
    res.m14 = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
    res.m14 -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
    res.m21 = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
    res.m21 -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
    res.m22 = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
    res.m22 -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
    res.m23 = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
    res.m23 -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
    res.m24 = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
    res.m24 -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
    
    /* calculate pairs for second 8 elements (cofactors) */
    tmp[0] = src[2]*src[7];
    tmp[1] = src[3]*src[6];
    tmp[2] = src[1]*src[7];
    tmp[3] = src[3]*src[5];
    tmp[4] = src[1]*src[6];
    tmp[5] = src[2]*src[5];
    tmp[6] = src[0]*src[7];
    tmp[7] = src[3]*src[4];
    tmp[8] = src[0]*src[6];
    tmp[9] = src[2]*src[4];
    tmp[10] = src[0]*src[5];
    tmp[11] = src[1]*src[4];
    /* calculate second 8 elements (cofactors) */
    res.m31 = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
    res.m31 -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
    res.m32 = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
    res.m32 -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
    res.m33 = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
    res.m33 -= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
    res.m34 = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
    res.m34 -= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
    
    res.tx = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
    res.tx -= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
    res.ty = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
    res.ty -= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
    res.tz = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
    res.tz -= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
    res.m44 = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
    res.m44 -= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
    
    /* calculate determinant */
    det=src[0]*res.m11 + src[1]*res.m12 + src[2]*res.m13 + src[3]*res.m14;
    
    /* calculate matrix inverse */
    det = 1.0f / det;
    inv.m11 = res.m11 * det;
    inv.m12 = res.m12 * det;
    inv.m13 = res.m13 * det;
    inv.m14 = res.m14 * det;
    inv.m21 = res.m21 * det;
    inv.m22 = res.m22 * det;
    inv.m23 = res.m23 * det;
    inv.m24 = res.m24 * det;
    inv.m31 = res.m31 * det;
    inv.m32 = res.m32 * det;
    inv.m33 = res.m33 * det;
    inv.m34 = res.m34 * det;
    inv.tx = res.tx * det;
    inv.ty = res.ty * det;
    inv.tz = res.tz * det;
    inv.m44 = res.m44 * det;

    return inv;
}

internal V3
ScreenToWorld(V3 screenp, Mat4 modelview, Mat4 projection, r32 *viewport)
{
    V3 res = {};

    //Transformation matrices
    Mat4 m, A;
    V4 in, out;
    //Calculation for inverting a matrix, compute projection x modelview
    //and store in A[16]
    A = projection * modelview;
    //Now compute the inverse of matrix A
    m = Inverse(A);
    // if(glhInvertMatrixf2(A, m)==0) return 0;
    //Transformation of normalized coordinates between -1 and 1
    in.x = (2.0f*((r32)(screenp.x)/(viewport[2])))-1.0f;
    in.y = 1.0f-(2.0f*((r32)(screenp.y)/(viewport[3])));
    in.z = 2.0f*screenp.z-1.0f;
    in.w = 1.0f;
    //Objects coordinates
    out = m * in;

    if(out.w == 0.0f)
     return res;

    out.w = 1.0f/out.w;
    res.x = out.x*out.w;
    res.y = out.y*out.w;
    res.z = out.z*out.w;
    return res;
}

//
// Curves //////////////////////////////////////////////////////
//

#define RECURSION_LIMIT 8
#define R32_EPSILON 1.19209290e-7f
#define PATH_DISTANCE_EPSILON 1.0f
#define curve_angle_tolerance_epsilon 0.01f
#define m_angle_tolerance 0.0f
#define m_cusp_limit 0.0f

internal u32 
AdaptivelySubdivideBezier(V2 p0, V2 p1, V2 p2, V2 p3, V2 **points, MemoryPool *memory)
{
    u32 point_count = 0;
    *points = AllocateFromMemoryPool(memory, V2, 1);
    *points[point_count++] = p0;

    

    AllocateFromMemoryPool(memory, V2, 1);
    *points[point_count++] = p3;
    return point_count;
}

inline V2
InterpolateCubicBezier2D(V2 p0, V2 p1, V2 p2, V2 p3, r32 t)
{
    r32 one_minus_t = (1.0f - t);
    r32 one_minus_t_squared = one_minus_t*one_minus_t;
    r32 one_minus_t_cubed = one_minus_t_squared*(1.0f - t);
    V2 p = (p0*one_minus_t_cubed) + (p1*(3.0f*t*one_minus_t_squared)) + (p2*(3.0f*t*t*one_minus_t)) + (p3*(t*t*t));
    return p;
}

internal u32
GenerateCubicBezier2D(V2 p0, V2 p1, V2 p2, V2 p3, u32 resolution, V2 *points)
{
    u32 count = 0;
    V2 *p = points;
    r32 dt = (1.0f/(r32)resolution);
    for (r32 t = 0.0f; t <= 1.0f; t += dt)
    {
        count++;
        *p = InterpolateCubicBezier2D(p0, p1, p2, p3, t);
        ++p;
    }
    return count;
}

internal u32
GenerateCubicBezier2D(V2 p0, V2 p1, V2 p2, V2 p3, u32 resolution, r32 *point_xys)
{
    u32 count = 0;
    r32 *xy = point_xys;
    r32 dt = (1.0f/(r32)resolution);
    for (r32 t = 0.0f; t <= 1.0f; t += dt)
    {
        count++;
        V2 p = InterpolateCubicBezier2D(p0, p1, p2, p3, t);
        *(xy++) = p.x;
        *(xy++) = p.y;
    }
    return count;
}

inline V3
InterpolateCubicBezier3D(V3 p0, V3 p1, V3 p2, V3 p3, r32 t)
{
    r32 one_minus_t = (1.0f - t);
    r32 one_minus_t_squared = one_minus_t*one_minus_t;
    r32 one_minus_t_cubed = one_minus_t_squared*(1.0f - t);
    V3 p = (p0*one_minus_t_cubed) + (p1*(3.0f*t*one_minus_t_squared)) + (p2*(3.0f*t*t*one_minus_t)) + (p3*(t*t*t));
    return p;
}

internal u32
GenerateCubicBezier3D(V3 p0, V3 p1, V3 p2, V3 p3, u32 resolution, V3 *points)
{
    u32 count = 0;
    V3 *p = points;
    r32 dt = (1.0f/(r32)(resolution-1));
    for (r32 t = 0.0f; t < (1.0f+dt); t += dt)
    {
        count++;
        *p = InterpolateCubicBezier3D(p0, p1, p2, p3, t);
        ++p;
    }
    return count;
}

inline V2
InterpolateQuadraticBezier2D(V2 p0, V2 p1, V2 p2, r32 t)
{
    r32 one_minus_t = (1.0f - t);
    r32 one_minus_t_squared = one_minus_t*one_minus_t;
    V2 p = (p0*one_minus_t_squared) + (p1*(2.0f*t*one_minus_t)) + (p2*(t*t));
    return p;
}

internal u32
GenerateQuadraticBezier2D(V2 p0, V2 p1, V2 p2, u32 resolution, V2 *points)
{
    u32 count = 0;
    V2 *p = points;
    r32 dt = (1.0f/(r32)resolution);
    for (r32 t = 0.0f; t <= 1.0f; t += dt)
    {
        count++;
        *p = InterpolateQuadraticBezier2D(p0, p1, p2, t);
        ++p;
    }
    return count;
}

internal u32
GenerateQuadraticBezier2D(V2 p0, V2 p1, V2 p2, u32 resolution, r32 *point_xys)
{
    u32 count = 0;
    r32 *xy = point_xys;
    r32 dt = (1.0f/(r32)resolution);
    for (r32 t = 0.0f; t <= 1.0f; t += dt)
    {
        count++;
        V2 p = InterpolateQuadraticBezier2D(p0, p1, p2, t);
        *(xy++) = p.x;
        *(xy++) = p.y;
    }
    return count;
}

inline V3
InterpolateQuadraticBezier3D(V3 p0, V3 p1, V3 p2, r32 t)
{
    r32 one_minus_t = (1.0f - t);
    r32 one_minus_t_squared = one_minus_t*one_minus_t;
    V3 p = (p0*one_minus_t_squared) + (p1*(2.0f*t*one_minus_t)) + (p2*(t*t));
    return p;
}

internal u32
GenerateQuadraticBezier3D(V3 p0, V3 p1, V3 p2, u32 resolution, V3 *points)
{
    u32 count = 0;
    V3 *p = points;
    r32 dt = (1.0f/(r32)resolution);
    for (r32 t = 0.0f; t <= 1.0f; t += dt)
    {
        count++;
        *p = InterpolateQuadraticBezier3D(p0, p1, p2, t);
        ++p;
    }
    return count;
}

internal u32 
CubicBezierLineIntersectionTs (V2 p0, V2 p1, V2 p2, V2 p3, V2 l0, V2 l1, r32 *t)
{
    u32 count = 0;

    r32 A = l1.y-l0.y;      //A=y2-y1
    r32 B = l0.x-l1.x;      //B=x1-x2
    r32 C = l0.x*(l0.y-l1.y) + l0.y*(l1.x-l0.x);  //C=x1*(y1-y2)+y1*(x2-x1)
 
    // coeficientes
    // d = P0
    // c = 3*P1-3*P0
    // b = 3*P2-6*P1+3*P0
    // a = P3-3*P2+3*P1-P0
    V2 d = p0;
    V2 c = (p1*3.0f) - (p0*3.0f);
    V2 b = (p2*3.0f) - (p1*6.0f) + (p0*3.0f);
    V2 a = p3 - (p2*3.0f) + (p1*3.0f) - p0;
 
    r32 P0 = A*a.x+B*a.y;     /*t^3*/
    r32 P1 = A*b.x+B*b.y;     /*t^2*/
    r32 P2 = A*c.x+B*c.y;     /*t*/
    r32 P3 = A*d.x+B*d.y + C; /*1*/
 
    // var r=cubicRoots(P);
    {
        r32 A = P1/P0;
        r32 B = P2/P0;
        r32 C = P3/P0;

        r32 Q = (3.0f*B - powf(A,2))/9.0f;
        r32 R = (9.0f*A*B - 27.0f*C - 2.0f*powf(A,3))/54.0f;
        r32 D = powf(Q,3) + powf(R,2.0f);    // polynomial discriminant

        r32 Im;
        if (D >= 0.0f)                                 // complex or duplicate roots
        {
            r32 S = (((R + sqrtf(D)) > 0.0f) ? 1.0f : -1.0f) * powf(Absolute(R + sqrtf(D)), (1.0f/3.0f));
            r32 T = (((R - sqrtf(D)) > 0.0f) ? 1.0f : -1.0f) * powf(Absolute(R - sqrtf(D)), (1.0f/3.0f));

            t[0] = -A/3.0f + (S + T);                    // real root
            t[1] = -A/3.0f - (S + T)/2.0f;                  // real part of complex root
            t[2] = -A/3.0f - (S + T)/2.0f;                  // real part of complex root
            Im = Absolute(sqrtf(3.0f) * (S - T)/2.0f);    // complex part of root pair   

            /*discard complex roots*/
            if (Im != 0.0f)
            {
                t[1] = -1.0f;
                t[2] = -1.0f;
            }

        }
        else                                          // distinct real roots
        {
            r32 th = acosf(R / sqrtf(-powf(Q,3)));

            t[0] = 2.0f*sqrtf(-Q)*cosf(th/3.0f) - A/3.0f;
            t[1] = 2.0f*sqrtf(-Q)*cosf((th + 2*PI)/3.0f) - A/3.0f;
            t[2] = 2.0f*sqrtf(-Q)*cosf((th + 4.0f*PI)/3.0f) - A/3.0f;
            Im = 0.0f;
        }

        /*discard out of spec roots*/
        count = 3;
        for (u32 i = 0; i < 3; ++i)
        {
            if((t[i] > 1.0f) || (t[i] < 0.0f)) 
            {
                count--;
                t[i] = -1.0f;
            }
        }
    }
 
    /*verify the roots are in bounds of the linear segment*/
    for (u32 i = 0; i < 3; i++)
    {
        if(t[i] != -1.0f)
        {
            V2 p = {};
            p.x = a.x*t[i]*t[i]*t[i] + b.x*t[i]*t[i] + c.x*t[i] + d.x;
            p.y = a.y*t[i]*t[i]*t[i] + b.y*t[i]*t[i] + c.y*t[i] + d.y;            
     
            /*above is intersection point assuming infinitely long line segment,
              make sure we are also in bounds of the line*/
            r32 s;
            if ((l1.x-l0.x) != 0.0f)           /*if not vertical line*/
                s = (p.x-l0.x)/(l1.x-l0.x);
            else
                s = (p.y-l0.y)/(l1.y-l0.y);
     
            /*in bounds?*/
            if ((t[i] < 0.0f) || (t[i] > 1.0f) || (s < 0.0f) || (s > 1.0f))
            {
                t[i] = -1.0f;
                count--;
            }
        }
    }

    if(count > 0)
    {
        for (u32 i = 0; i < 3; ++i)
        {
            for (u32 j = i+1; j < 3; ++j)
            {
                if(t[i] < t[j])
                {
                    r32 tmp = t[i];
                    t[i] = t[j];
                    t[j] = tmp;
                }
            }
        }
    }

    return count;
}

internal void 
SplitCubicBezierInT (V2 *curve, r32 t, V2 *left, V2 *right)
{
    //left
    V2 p0 = curve[0];
    V2 p1 = curve[1];
    V2 p2 = curve[2];
    V2 p3 = curve[3];

    V2 p01 = V2((p1.x-p0.x)*t + p0.x,
                (p1.y-p0.y)*t + p0.y);
    V2 p12 = V2((p2.x-p1.x)*t + p1.x,
                (p2.y-p1.y)*t + p1.y);
    V2 p23 = V2((p3.x-p2.x)*t + p2.x,
                (p3.y-p2.y)*t + p2.y);
    V2 p012 = V2((p12.x-p01.x)*t + p01.x,
                 (p12.y-p01.y)*t + p01.y);
    V2 p123 = V2((p23.x-p12.x)*t + p12.x,
                 (p23.y-p12.y)*t + p12.y);
    V2 p0123 = V2((p123.x-p012.x)*t + p012.x,
                  (p123.y-p012.y)*t + p012.y);

    left[0] = p0;
    left[1] = p01;
    left[2] = p012;
    left[3] = p0123;

    //right
    V2 tmp = p0;
    p0 = p3;
    p3 = tmp;
    tmp = p1;
    p1 = p2;
    p2 = tmp;

    t = 1.0f - t;

    p01 = V2((p1.x-p0.x)*t + p0.x,
             (p1.y-p0.y)*t + p0.y);
    p12 = V2((p2.x-p1.x)*t + p1.x,
             (p2.y-p1.y)*t + p1.y);
    p23 = V2((p3.x-p2.x)*t + p2.x,
             (p3.y-p2.y)*t + p2.y);
    p012 = V2((p12.x-p01.x)*t + p01.x,
             (p12.y-p01.y)*t + p01.y);
    p123 = V2((p23.x-p12.x)*t + p12.x,
             (p23.y-p12.y)*t + p12.y);
    p0123 = V2((p123.x-p012.x)*t + p012.x,
              (p123.y-p012.y)*t + p012.y);

    right[0] = p0;
    right[1] = p01;
    right[2] = p012;
    right[3] = p0123;
}

//
// Misc /////////////////////////////////////////////////////////////////
//

inline u32 
Min (u32 a, u32 b)
{
    return ((a < b) ? a : b);
}

inline r32
Clamp (r32 min, r32 max, r32 val)
{
    return (val > max ? max : (val < min ? min : val));
}

inline r32
Absolute(r32 r)
{
    return ((r <=-0.0f) ? -r : r);
}

internal b32
IsPointInsideCircle(V2 p, V2 c, r32 r)
{
    b32 res = false;
    V2 d = p - c;
    if(Magnitude(d) <= r) res = true;
    return res;
}

internal b32
IsPointInsideRect(V2 point, V2 rcenter, V2 rsize)
{
    b32 res = false;
    if((point.x >= rcenter.x-(rsize.x*0.5f)) &&
        (point.x <= rcenter.x+(rsize.x*0.5f)) &&
        (point.y >= rcenter.y-(rsize.y*0.5f)) &&
        (point.y <= rcenter.y+(rsize.y*0.5f)))
    {
        res = true;
    }
    return res;
}

internal b32 
LineSegmentsIntersect (V2 p0, V2 p1, V2 p2, V2 p3)
{
    b32 res = false;

    V2 s1 = p1 - p0;
    V2 s2 = p3 - p2;

    V2 u = p0 - p2;

    r32 ip = 1.0f / (-s2.x * s1.y + s1.x * s2.y);

    r32 s = (-s1.y * u.x + s1.x * u.y) * ip;
    r32 t = ( s2.x * u.y - s2.y * u.x) * ip;

    if (s >= 0.0f && s <= 1.0f && t >= 0.0f && t <= 1.0f) 
    {
        res = true;
    }

    return res;
}

inline b32
AreCollinear(V2 a, V2 b, V2 c)
{
    return ((b.y-a.y)*(c.x-b.x) == (c.y-b.y)*(b.x-a.x));
}