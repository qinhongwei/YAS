
#ifndef TG_MATHLIB_H
#define TG_MATHLIB_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

namespace TomGine{

struct vec2;
struct vec3;
struct vec4;
struct mat3;
struct mat4;
struct quat;

const float epsilon = 1e-6f;

inline unsigned ilog2(unsigned value)
{
  unsigned f(0), s(32);
  while(s) {
    s>>=1;
    if( value > (unsigned)1<<(f+s) ) f+=s;
  }
  return (f+1);
}

inline unsigned mipmaplvl(unsigned w, unsigned h, unsigned& wn, unsigned& hn)
{
  unsigned lvl(0);
  while(true)
  {
    wn = w>>1;
    hn = h>>1;
    if( wn<<1 == w && hn<<1 == h )
    {
      w = wn;
      h = hn;
      lvl++;
    }else{
      wn = w;
      hn = h;
      return lvl;
    }
  }
  return 0;
}

/**
 * @brief RGBValue of point clouds, accessable as float or long value.
 */
typedef union
{
  struct
  {
    unsigned char b; // Blue channel
    unsigned char g; // Green channel
    unsigned char r; // Red channel
    unsigned char a; // Alpha channel
  };
  float float_value;
} RGBValue;

/*****************************************************************************/
/* vec2                                                                      */
struct vec2 {

  inline vec2() : x(0), y(0) { }
  inline vec2(float x,float y) : x(x), y(y) { }
  inline vec2(const float *v) : x(v[0]), y(v[1]) { }
  inline vec2(const vec2 &v) : x(v.x), y(v.y) { }

  inline int operator==(const vec2 &v) const { return (fabs(x - v.x) < epsilon && fabs(y - v.y) < epsilon); }
  inline int operator!=(const vec2 &v) { return !(*this == v); }

  inline const vec2 operator*(float f) const { return vec2(x * f,y * f); }
  inline const vec2 operator/(float f) const { float vf = 1.0f/f; return vec2(x * vf,y * vf); }
  inline const vec2 operator+(const vec2 &v) const { return vec2(x + v.x,y + v.y); }
  inline const vec2 operator-() const { return vec2(-x,-y); }
  inline const vec2 operator-(const vec2 &v) const { return vec2(x - v.x,y - v.y); }

  inline vec2 &operator*=(float f) { return *this = *this * f; }
  inline vec2 &operator/=(float f) { return *this = *this / f; }
  inline vec2 &operator+=(const vec2 &v) { return *this = *this + v; }
  inline vec2 &operator-=(const vec2 &v) { return *this = *this - v; }

  inline float operator*(const vec2 &v) const { return x * v.x + y * v.y; }

  inline operator float*() { return (float*)&x; }
  inline operator const float*() const { return (float*)&x; }

  inline float &operator[](int i) { return ((float*)&x)[i]; }
  inline const float operator[](int i) const { return ((float*)&x)[i]; }

  inline float length() const { return sqrt(x * x + y * y); }
  inline float normalize() {
    float inv,length = sqrt(x * x + y * y);
    if(length < epsilon) return 0.0;
    inv = 1.0f / length;
    x *= inv;
    y *= inv;
    return length;
  }

  union {
    struct {
      float x,y;
    };
    float v[2];
  };
};

inline vec2 param2polar(const vec2 &vp)
{
  vec2 r, v=vp;
  r.y = v.normalize();

  if(v.x > epsilon){

    if(v.y > epsilon)
      r.x = atan(v.y/v.x);
    else if(v.y < -epsilon)
      r.x = 2.0f * M_PI + atan(v.y/v.x);
    else // v.y == 0.0
      r.x = 0.0;

  }else if(v.x < -epsilon){

    if(v.y > epsilon)
      r.x = M_PI + atan(v.y/v.x);
    else if(v.y < -epsilon)
      r.x = M_PI + atan(v.y/v.x);
    else // v.y == 0.0
      r.x = M_PI;

  }else{ // v.x == 0
    if(v.y > epsilon)
      r.x = M_PI_2;
    else if(v.y < -epsilon)
      r.x = 3.0f * M_PI_2;
    else{
      r.x = 0.0f;
    }
  }
  return r;
}

/*****************************************************************************/
/* vec3                                                                      */
struct vec3 {

  inline vec3() : x(0), y(0), z(0) { }
  inline vec3(float x,float y,float z) : x(x), y(y), z(z) { }
  inline vec3(const float *v) : x(v[0]), y(v[1]), z(v[2]) { }
  inline vec3(const vec2 &v) : x(v.x), y(v.y), z(0.0) { }
  inline vec3(const vec3 &v) : x(v.x), y(v.y), z(v.z) { }
  inline vec3(const vec4 &v);

  inline void random(){ 	x = float(rand())/float(RAND_MAX);
                          y = float(rand())/float(RAND_MAX);
                                                z = float(rand())/float(RAND_MAX); }

  inline int operator==(const vec3 &v) const { return (fabs(x - v.x) < epsilon && fabs(y - v.y) < epsilon && fabs(z - v.z) < epsilon);  }
  inline int operator!=(const vec3 &v) { return !(*this == v); }

  inline const vec3 operator*(float f) const { return vec3(x * f,y * f,z * f); }
  inline const vec3 operator/(float f) const { float vf = 1.0f/f; return vec3(x * vf,y * vf,z * vf); }
  inline const vec3 operator+(const vec3 &v) const { return vec3(x + v.x,y + v.y,z + v.z); }
  inline const vec3 operator-() const { return vec3(-x,-y,-z); }
  inline const vec3 operator-(const vec3 &v) const { return vec3(x - v.x,y - v.y,z - v.z); }
  inline const vec3 operator^(const vec3 &v) const { return vec3(y * v.z - z * v.y,
                                                                 z * v.x - x * v.z, x * v.y - y * v.x); }

  inline vec3 &operator*=(float f) { return *this = *this * f; }
  inline vec3 &operator/=(float f) { return *this = *this / f; }
  inline vec3 &operator+=(const vec3 &v) { return *this = *this + v; }
  inline vec3 &operator-=(const vec3 &v) { return *this = *this - v; }

  inline float operator*(const vec3 &v) const { return x * v.x + y * v.y + z * v.z; }
  inline float operator*(const vec4 &v) const;
  inline vec3 operator*(const mat3 &m) const;

  inline operator float*() { return (float*)&x; }
  inline operator const float*() const { return (float*)&x; }

  inline float &operator[](int i) { return ((float*)&x)[i]; }
  inline const float operator[](int i) const { return ((float*)&x)[i]; }

  inline float length() const { return sqrt(x * x + y * y + z * z); }
  inline float normalize() {
    float length = sqrt(x * x + y * y + z * z);
    if(length < epsilon) return 0.0;
    float inv = 1.0f / length;
    x *= inv;
    y *= inv;
    z *= inv;
    return length;
  }

  inline float squaredNorm() const{
    return x*x + y*y + z*z;
  }

  inline void cross(const vec3 &v1,const vec3 &v2) {
    x = v1.y * v2.z - v1.z * v2.y;
    y = v1.z * v2.x - v1.x * v2.z;
    z = v1.x * v2.y - v1.y * v2.x;
  }

  inline void rotate(float alpha, vec3 r);

  union {
    struct {
      float x,y,z;
    };
    float v[3];
  };
};

inline vec3 normalize(const vec3 &v) {
  float length = v.length();
  if(length < epsilon) return vec3(0,0,0);
  return v / length;
}

inline vec3 cross(const vec3 &v1,const vec3 &v2) {
  vec3 ret;
  ret.x = v1.y * v2.z - v1.z * v2.y;
  ret.y = v1.z * v2.x - v1.x * v2.z;
  ret.z = v1.x * v2.y - v1.y * v2.x;
  return ret;
}

inline vec3 saturate(const vec3 &v) {
  vec3 ret = v;
  if(ret.x < 0.0) ret.x = 0.0;
  else if(ret.x > 1.0f) ret.x = 1.0f;
  if(ret.y < 0.0) ret.y = 0.0;
  else if(ret.y > 1.0f) ret.y = 1.0f;
  if(ret.z < 0.0) ret.z = 0.0;
  else if(ret.z > 1.0f) ret.z = 1.0f;
  return ret;
}

inline vec3 rgb2hsv(const vec3 &rgb){
  float h,s,v;
  float r=rgb.x;
  float g=rgb.y;
  float b=rgb.z;

  float fMax = r;
  float fMin = r;
  if(g > fMax) fMax = g;
  if(b > fMax) fMax = b;
  if(g < fMin) fMin = g;
  if(b < fMin) fMin = b;

  if(fMax-fMin < 0.01) h = 0.0;
  else if(fMax == r) h = 60.0 * (0.0 + (g-b)/(fMax-fMin));
  else if(fMax == g) h = 60.0 * (2.0 + (b-r)/(fMax-fMin));
  else if(fMax == b) h = 60.0 * (4.0 + (r-b)/(fMax-fMin));

  if(h < 0.0) h = h + 360.0;

  if(fMax < 0.01) s = 0.0;
  else s = (fMax-fMin)/fMax;

  v = fMax;

  return vec3(h, s, v);
}

inline vec3 hsv2rgb(const vec3 &hsv)
{
  float p1, p2, p3, i, f;
  float xh;
  float nr,ng,nb;
  float h = hsv.x;
  float s = hsv.y;
  float v = hsv.z;  /* hue (0.0 to 360.0, is circular, 0=360),  s and v are from 0.0 - 1.0) */

  if (h == 360.0)
    h = 0.0;           /* (THIS LOOKS BACKWARDS)       */

  xh = h / 60.;                   /* convert hue to be in 0,6       */
  i = (float)floor((double)xh);    /* i = greatest integer <= h    */
  f = xh - i;                     /* f = fractional part of h     */
  p1 = v * (1 - s);
  p2 = v * (1 - (s * f));
  p3 = v * (1 - (s * (1 - f)));

  switch ((int) i)
  {
  case 0:
    nr = v;
    ng = p3;
    nb = p1;
    break;
  case 1:
    nr = p2;
    ng = v;
    nb = p1;
    break;
  case 2:
    nr = p1;
    ng = v;
    nb = p3;
    break;
  case 3:
    nr = p1;
    ng = p2;
    nb = v;
    break;
  case 4:
    nr = p3;
    ng = p1;
    nb = v;
    break;
  case 5:
    nr = v;
    ng = p1;
    nb = p2;
    break;
  }

  return vec3(nr,ng,nb);
}


/*****************************************************************************/
/* vec4                                                                      */
struct vec4 {

  inline vec4() : x(0), y(0), z(0), w(1) { }
  inline vec4(float x,float y,float z,float w) : x(x), y(y), z(z), w(w) { }
  inline vec4(const float *v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) { }
  inline vec4(const vec3 &v) : x(v.x), y(v.y), z(v.z), w(1) { }
  inline vec4(const vec3 &v,float w) : x(v.x), y(v.y), z(v.z), w(w) { }
  inline vec4(const vec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) { }

  inline void random(){ x = float(rand())/float(RAND_MAX);
                        y = float(rand())/float(RAND_MAX);
                                            z = float(rand())/float(RAND_MAX);
                                                                w = float(rand())/float(RAND_MAX); }

  inline int operator==(const vec4 &v) const { return (fabs(x - v.x) < epsilon && fabs(y - v.y) < epsilon && fabs(z - v.z) < epsilon && fabs(w - v.w) < epsilon); }
  inline int operator!=(const vec4 &v) { return !(*this == v); }

  inline const vec4 operator*(float f) const { return vec4(x * f,y * f,z * f,w * f); }
  inline const vec4 operator/(float f) const { float vf = 1.0f/f; return vec4(x * vf,y * vf,z * vf,w * vf); }
  inline const vec4 operator+(const vec4 &v) const { return vec4(x + v.x,y + v.y,z + v.z,w + v.w); }
  inline const vec4 operator-() const { return vec4(-x,-y,-z,-w); }
  inline const vec4 operator-(const vec4 &v) const { return vec4(x - v.x,y - v.y,z - v.z,z - v.w); }

  inline vec4 &operator*=(float f) { return *this = *this * f; }
  inline vec4 &operator/=(float f) { return *this = *this / f; }
  inline vec4 &operator+=(const vec4 &v) { return *this = *this + v; }
  inline vec4 &operator-=(const vec4 &v) { return *this = *this - v; }

  inline float operator*(const vec3 &v) const { return x * v.x + y * v.y + z * v.z + w; }
  inline float operator*(const vec4 &v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }

  inline operator float*() { return (float*)&x; }
  inline operator const float*() const { return (float*)&x; }

  inline float &operator[](int i) { return ((float*)&x)[i]; }
  inline const float operator[](int i) const { return ((float*)&x)[i]; }

  inline float length() const { return sqrt(x * x + y * y + z * z + w * w); }
  inline float normalize() {
    float length = sqrt(x * x + y * y + z * z + w * w);
    if(length < epsilon) return 0.0;
    float inv = 1.0f / length;
    x *= inv;
    y *= inv;
    z *= inv;
    w *= inv;
    return length;
  }

  union {
    struct {
      float x,y,z,w;
    };
    float v[4];
  };
};

inline vec3::vec3(const vec4 &v) {
  x = v.x;
  y = v.y;
  z = v.z;
}

inline float vec3::operator*(const vec4 &v) const {
  return x * v.x + y * v.y + z * v.z + v.w;
}

inline vec4 normalize(const vec4 &v) {
  float length = v.length();
  if(length < epsilon) return vec4(0,0,0,0);
  return v / length;
}

inline vec4 saturate(const vec4 &v) {
  vec4 ret = v;
  if(ret.x < 0.0) ret.x = 0.0;
  else if(ret.x > 1.0f) ret.x = 1.0f;
  if(ret.y < 0.0) ret.y = 0.0;
  else if(ret.y > 1.0f) ret.y = 1.0f;
  if(ret.z < 0.0) ret.z = 0.0;
  else if(ret.z > 1.0f) ret.z = 1.0f;
  if(ret.w < 0.0) ret.w = 0.0;
  else if(ret.w > 1.0f) ret.w = 1.0f;
  return ret;
}

/*****************************************************************************/
/* mat3                                                                      */
struct mat3 {

  mat3() {
    mat[0] = 1.0; mat[3] = 0.0; mat[6] = 0.0;
    mat[1] = 0.0; mat[4] = 1.0; mat[7] = 0.0;
    mat[2] = 0.0; mat[5] = 0.0; mat[8] = 1.0;
  }
  mat3(const float *m) {
    mat[0] = m[0]; mat[3] = m[3]; mat[6] = m[6];
    mat[1] = m[1]; mat[4] = m[4]; mat[7] = m[7];
    mat[2] = m[2]; mat[5] = m[5]; mat[8] = m[8];
  }
  mat3(const mat3 &m) {
    mat[0] = m[0]; mat[3] = m[3]; mat[6] = m[6];
    mat[1] = m[1]; mat[4] = m[4]; mat[7] = m[7];
    mat[2] = m[2]; mat[5] = m[5]; mat[8] = m[8];
  }
  mat3(const vec3 &v1, const vec3 &v2, const vec3 &v3){
    mat[0] = v1.x; mat[3] = v2.x; mat[6] = v3.x;
    mat[1] = v1.y; mat[4] = v2.y; mat[7] = v3.y;
    mat[2] = v1.z; mat[5] = v2.z; mat[8] = v3.z;
  }
  mat3(	const float &f1, const float &f2, const float &f3,
        const float &f4, const float &f5, const float &f6,
        const float &f7, const float &f8, const float &f9){
    mat[0] = f1; mat[3] = f2; mat[6] = f3;
    mat[1] = f4; mat[4] = f5; mat[7] = f6;
    mat[2] = f7; mat[5] = f8; mat[8] = f9;
  }
  mat3(const mat4 &m);

  vec3 operator*(const vec3 &v) const {
    vec3 ret;
    ret[0] = mat[0] * v[0] + mat[3] * v[1] + mat[6] * v[2];
    ret[1] = mat[1] * v[0] + mat[4] * v[1] + mat[7] * v[2];
    ret[2] = mat[2] * v[0] + mat[5] * v[1] + mat[8] * v[2];
    return ret;
  }
  vec4 operator*(const vec4 &v) const {
    vec4 ret;
    ret[0] = mat[0] * v[0] + mat[3] * v[1] + mat[6] * v[2];
    ret[1] = mat[1] * v[0] + mat[4] * v[1] + mat[7] * v[2];
    ret[2] = mat[2] * v[0] + mat[5] * v[1] + mat[8] * v[2];
    ret[3] = v[3];
    return ret;
  }
  mat3 operator*(float f) const {
    mat3 ret;
    ret[0] = mat[0] * f; ret[3] = mat[3] * f; ret[6] = mat[6] * f;
    ret[1] = mat[1] * f; ret[4] = mat[4] * f; ret[7] = mat[7] * f;
    ret[2] = mat[2] * f; ret[5] = mat[5] * f; ret[8] = mat[8] * f;
    return ret;
  }
  mat3 operator*(const mat3 &m) const {
    mat3 ret;
    ret[0] = mat[0] * m[0] + mat[3] * m[1] + mat[6] * m[2];
    ret[1] = mat[1] * m[0] + mat[4] * m[1] + mat[7] * m[2];
    ret[2] = mat[2] * m[0] + mat[5] * m[1] + mat[8] * m[2];
    ret[3] = mat[0] * m[3] + mat[3] * m[4] + mat[6] * m[5];
    ret[4] = mat[1] * m[3] + mat[4] * m[4] + mat[7] * m[5];
    ret[5] = mat[2] * m[3] + mat[5] * m[4] + mat[8] * m[5];
    ret[6] = mat[0] * m[6] + mat[3] * m[7] + mat[6] * m[8];
    ret[7] = mat[1] * m[6] + mat[4] * m[7] + mat[7] * m[8];
    ret[8] = mat[2] * m[6] + mat[5] * m[7] + mat[8] * m[8];
    return ret;
  }
  mat3 operator+(const mat3 &m) const {
    mat3 ret;
    ret[0] = mat[0] + m[0]; ret[3] = mat[3] + m[3]; ret[6] = mat[6] + m[6];
    ret[1] = mat[1] + m[1]; ret[4] = mat[4] + m[4]; ret[7] = mat[7] + m[7];
    ret[2] = mat[2] + m[2]; ret[5] = mat[5] + m[5]; ret[8] = mat[8] + m[8];
    return ret;
  }
  mat3 operator-(const mat3 &m) const {
    mat3 ret;
    ret[0] = mat[0] - m[0]; ret[3] = mat[3] - m[3]; ret[6] = mat[6] - m[6];
    ret[1] = mat[1] - m[1]; ret[4] = mat[4] - m[4]; ret[7] = mat[7] - m[7];
    ret[2] = mat[2] - m[2]; ret[5] = mat[5] - m[5]; ret[8] = mat[8] - m[8];
    return ret;
  }

  mat3 &operator*=(float f) { return *this = *this * f; }
  mat3 &operator*=(const mat3 &m) { return *this = *this * m; }
  mat3 &operator+=(const mat3 &m) { return *this = *this + m; }
  mat3 &operator-=(const mat3 &m) { return *this = *this - m; }

  operator float*() { return mat; }
  operator const float*() const { return mat; }

  float &operator[](int i) { return mat[i]; }
  const float operator[](int i) const { return mat[i]; }

  mat3 transpose() const {
    mat3 ret;
    ret[0] = mat[0]; ret[3] = mat[1]; ret[6] = mat[2];
    ret[1] = mat[3]; ret[4] = mat[4]; ret[7] = mat[5];
    ret[2] = mat[6]; ret[5] = mat[7]; ret[8] = mat[8];
    return ret;
  }
  float det() const {
    float det;
    det = mat[0] * mat[4] * mat[8];
    det += mat[3] * mat[7] * mat[2];
    det += mat[6] * mat[1] * mat[5];
    det -= mat[6] * mat[4] * mat[2];
    det -= mat[3] * mat[1] * mat[8];
    det -= mat[0] * mat[7] * mat[5];
    return det;
  }
  mat3 inverse() const {
    mat3 ret;
    float idet = 1.0f / det();
    ret[0] =  (mat[4] * mat[8] - mat[7] * mat[5]) * idet;
    ret[1] = -(mat[1] * mat[8] - mat[7] * mat[2]) * idet;
    ret[2] =  (mat[1] * mat[5] - mat[4] * mat[2]) * idet;
    ret[3] = -(mat[3] * mat[8] - mat[6] * mat[5]) * idet;
    ret[4] =  (mat[0] * mat[8] - mat[6] * mat[2]) * idet;
    ret[5] = -(mat[0] * mat[5] - mat[3] * mat[2]) * idet;
    ret[6] =  (mat[3] * mat[7] - mat[6] * mat[4]) * idet;
    ret[7] = -(mat[0] * mat[7] - mat[6] * mat[1]) * idet;
    ret[8] =  (mat[0] * mat[4] - mat[3] * mat[1]) * idet;
    return ret;
  }

  void zero() {
    mat[0] = 0.0; mat[3] = 0.0; mat[6] = 0.0;
    mat[1] = 0.0; mat[4] = 0.0; mat[7] = 0.0;
    mat[2] = 0.0; mat[5] = 0.0; mat[8] = 0.0;
  }
  void identity() {
    mat[0] = 1.0; mat[3] = 0.0; mat[6] = 0.0;
    mat[1] = 0.0; mat[4] = 1.0; mat[7] = 0.0;
    mat[2] = 0.0; mat[5] = 0.0; mat[8] = 1.0;
  }
  void rotate(const vec3 &axis,float angle) {
    float rad = angle * (M_PI / 180.0f);
    float c = cos(rad);
    float s = sin(rad);
    vec3 v = axis;
    v.normalize();
    float xx = v.x * v.x;
    float yy = v.y * v.y;
    float zz = v.z * v.z;
    float xy = v.x * v.y;
    float yz = v.y * v.z;
    float zx = v.z * v.x;
    float xs = v.x * s;
    float ys = v.y * s;
    float zs = v.z * s;
    mat[0] = (1.0f - c) * xx + c; mat[3] = (1.0f - c) * xy - zs; mat[6] = (1.0f - c) * zx + ys;
    mat[1] = (1.0f - c) * xy + zs; mat[4] = (1.0f - c) * yy + c; mat[7] = (1.0f - c) * yz - xs;
    mat[2] = (1.0f - c) * zx - ys; mat[5] = (1.0f - c) * yz + xs; mat[8] = (1.0f - c) * zz + c;
  }
  void rotate(float x,float y,float z,float angle) {
    rotate(vec3(x,y,z),angle);
  }
  void rotate_x(float angle) {
    float rad = angle * (M_PI / 180.0f);
    float c = cos(rad);
    float s = sin(rad);
    mat[0] = 1.0; mat[3] = 0.0; mat[6] = 0.0;
    mat[1] = 0.0; mat[4] = c; mat[7] = -s;
    mat[2] = 0.0; mat[5] = s; mat[8] = c;
  }
  void rotate_y(float angle) {
    float rad = angle * (M_PI / 180.0f);
    float c = cos(rad);
    float s = sin(rad);
    mat[0] = c; mat[3] = 0.0; mat[6] = s;
    mat[1] = 0.0; mat[4] = 1.0; mat[7] = 0.0;
    mat[2] = -s; mat[5] = 0.0; mat[8] = c;
  }
  void rotate_z(float angle) {
    float rad = angle * (M_PI / 180.0f);
    float c = cos(rad);
    float s = sin(rad);
    mat[0] = c; mat[3] = -s; mat[6] = 0.0;
    mat[1] = s; mat[4] = c; mat[7] = 0.0;
    mat[2] = 0.0; mat[5] = 0.0; mat[8] = 1.0;
  }
  void scale(const vec3 &v) {
    mat[0] = v.x; mat[3] = 0.0; mat[6] = 0.0;
    mat[1] = 0.0; mat[4] = v.y; mat[7] = 0.0;
    mat[2] = 0.0; mat[5] = 0.0; mat[8] = v.z;
  }
  void scale(float x,float y,float z) {
    scale(vec3(x,y,z));
  }
  void orthonormalize() {
    vec3 x(mat[0],mat[1],mat[2]);
    vec3 y(mat[3],mat[4],mat[5]);
    vec3 z;
    x.normalize();
    z.cross(x,y);
    z.normalize();
    y.cross(z,x);
    y.normalize();
    mat[0] = x.x; mat[3] = y.x; mat[6] = z.x;
    mat[1] = x.y; mat[4] = y.y; mat[7] = z.y;
    mat[2] = x.z; mat[5] = y.z; mat[8] = z.z;
  }

  // Vector/Matrix Conversions
  void fromAngleAxis(float angle, const vec3& axis)
  {
    float s = sin(angle), c = cos(angle);
    float v = (float)1.0 - c, x = axis.x*v, y = axis.y*v, z = axis.z*v;

    mat[0] = axis.x*x + c;
    mat[1] = axis.x*y - axis.z*s;
    mat[2] = axis.x*z + axis.y*s;

    mat[3] = axis.y*x + axis.z*s;
    mat[4] = axis.y*y + c;
    mat[5] = axis.y*z - axis.x*s;

    mat[6] = axis.z*x - axis.y*s;
    mat[7] = axis.z*y + axis.x*s;
    mat[8] = axis.z*z + c;
  }

  // creates rotation matrix from rotation vector (= axis plus angle).
  void fromRotVector(const vec3& r)
  {
    vec3 axis(r);
    float angle = axis.normalize();
    fromAngleAxis(angle, axis);
  }

  void getAxisAngle(vec3 &axis, float &angle)
  {
    angle = acos(( mat[0] + mat[4] + mat[8] - 1.0f)/2.0f);
    axis.x = (mat[5] - mat[7])/sqrt(pow(mat[5] - mat[7],2)+pow(mat[6] - mat[2],2)+pow(mat[1] - mat[3],2));
    axis.y = (mat[6] - mat[2])/sqrt(pow(mat[5] - mat[7],2)+pow(mat[6] - mat[2],2)+pow(mat[1] - mat[3],2));
    axis.z = (mat[1] - mat[3])/sqrt(pow(mat[5] - mat[7],2)+pow(mat[6] - mat[2],2)+pow(mat[1] - mat[3],2));
  }

  void print(){
    printf(" %f %f %f\n", mat[0], mat[3], mat[6]);
    printf(" %f %f %f\n", mat[1], mat[4], mat[7]);
    printf(" %f %f %f\n", mat[2], mat[5], mat[8]);
  }

  float mat[9];
};

inline vec3 vec3::operator*(const mat3 &m) const
{
  return vec3(x * m.mat[0] + y * m.mat[1] + z * m.mat[2],
      x * m.mat[3] + y * m.mat[4] + z * m.mat[5],
      x * m.mat[6] + y * m.mat[7] + z * m.mat[8]);
}

inline void vec3::rotate(float alpha, vec3 r)
{
  vec3 v,s,t,n;
  mat3 M, Mt, Rx, X;

  if(alpha != 0.0f){

    r.normalize();
    s.cross(r,vec3(1,0,0));

    if(s.length() < 0.001f)
      s.cross(r, vec3(0,1,0));

    s.normalize();
    t.cross(r,s);

    Mt = mat3(r,s,t);
    M = mat3(Mt);
    M = M.transpose();

    Rx = mat3(	1.0f,	0.0f,	0.0f,
                0.0f,	cosf(alpha),	-sinf(alpha),
                0.0f,	sinf(alpha),	cosf(alpha));

    X = Mt*Rx*M;

    v = vec3(x,y,z);
    n = X*v;

    x = n.x;
    y = n.y;
    z = n.z;
  }
}

/*****************************************************************************/
/* mat4                                                                      */
struct mat4 {

  mat4() {
    mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
    mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = 0.0;
    mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
  }
  mat4(const vec3 &v) {
    translate(v);
  }
  mat4(float x,float y,float z) {
    translate(x,y,z);
  }
  mat4(const vec3 &axis,float angle) {
    rotate(axis,angle);
  }
  mat4(float x,float y,float z,float angle) {
    rotate(x,y,z,angle);
  }
  mat4(const mat3 &m) {
    mat[0] = m[0]; mat[4] = m[3]; mat[8] = m[6]; mat[12] = 0.0;
    mat[1] = m[1]; mat[5] = m[4]; mat[9] = m[7]; mat[13] = 0.0;
    mat[2] = m[2]; mat[6] = m[5]; mat[10] = m[8]; mat[14] = 0.0;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
  }
  mat4(const mat3 &m, const vec3 &v) {
    mat[0] = m[0]; mat[4] = m[3]; mat[8] = m[6]; mat[12] = v[0];
    mat[1] = m[1]; mat[5] = m[4]; mat[9] = m[7]; mat[13] = v[1];
    mat[2] = m[2]; mat[6] = m[5]; mat[10] = m[8]; mat[14] = v[2];
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
  }
  mat4(const float *m) {
    mat[0] = m[0]; mat[4] = m[4]; mat[8] = m[8]; mat[12] = m[12];
    mat[1] = m[1]; mat[5] = m[5]; mat[9] = m[9]; mat[13] = m[13];
    mat[2] = m[2]; mat[6] = m[6]; mat[10] = m[10]; mat[14] = m[14];
    mat[3] = m[3]; mat[7] = m[7]; mat[11] = m[11]; mat[15] = m[15];
  }
  mat4(const mat4 &m) {
    mat[0] = m[0]; mat[4] = m[4]; mat[8] = m[8]; mat[12] = m[12];
    mat[1] = m[1]; mat[5] = m[5]; mat[9] = m[9]; mat[13] = m[13];
    mat[2] = m[2]; mat[6] = m[6]; mat[10] = m[10]; mat[14] = m[14];
    mat[3] = m[3]; mat[7] = m[7]; mat[11] = m[11]; mat[15] = m[15];
  }

  vec3 operator*(const vec3 &v) const {
    vec3 ret;
    ret[0] = mat[0] * v[0] + mat[4] * v[1] + mat[8] * v[2] + mat[12];
    ret[1] = mat[1] * v[0] + mat[5] * v[1] + mat[9] * v[2] + mat[13];
    ret[2] = mat[2] * v[0] + mat[6] * v[1] + mat[10] * v[2] + mat[14];
    return ret;
  }
  vec4 operator*(const vec4 &v) const {
    vec4 ret;
    ret[0] = mat[0] * v[0] + mat[4] * v[1] + mat[8] * v[2] + mat[12] * v[3];
    ret[1] = mat[1] * v[0] + mat[5] * v[1] + mat[9] * v[2] + mat[13] * v[3];
    ret[2] = mat[2] * v[0] + mat[6] * v[1] + mat[10] * v[2] + mat[14] * v[3];
    ret[3] = mat[3] * v[0] + mat[7] * v[1] + mat[11] * v[2] + mat[15] * v[3];
    return ret;
  }
  mat4 operator*(float f) const {
    mat4 ret;
    ret[0] = mat[0] * f; ret[4] = mat[4] * f; ret[8] = mat[8] * f; ret[12] = mat[12] * f;
    ret[1] = mat[1] * f; ret[5] = mat[5] * f; ret[9] = mat[9] * f; ret[13] = mat[13] * f;
    ret[2] = mat[2] * f; ret[6] = mat[6] * f; ret[10] = mat[10] * f; ret[14] = mat[14] * f;
    ret[3] = mat[3] * f; ret[7] = mat[7] * f; ret[11] = mat[11] * f; ret[15] = mat[15] * f;
    return ret;
  }
  mat4 operator*(const mat4 &m) const {
    mat4 ret;
    ret[0] = mat[0] * m[0] + mat[4] * m[1] + mat[8] * m[2] + mat[12] * m[3];
    ret[1] = mat[1] * m[0] + mat[5] * m[1] + mat[9] * m[2] + mat[13] * m[3];
    ret[2] = mat[2] * m[0] + mat[6] * m[1] + mat[10] * m[2] + mat[14] * m[3];
    ret[3] = mat[3] * m[0] + mat[7] * m[1] + mat[11] * m[2] + mat[15] * m[3];
    ret[4] = mat[0] * m[4] + mat[4] * m[5] + mat[8] * m[6] + mat[12] * m[7];
    ret[5] = mat[1] * m[4] + mat[5] * m[5] + mat[9] * m[6] + mat[13] * m[7];
    ret[6] = mat[2] * m[4] + mat[6] * m[5] + mat[10] * m[6] + mat[14] * m[7];
    ret[7] = mat[3] * m[4] + mat[7] * m[5] + mat[11] * m[6] + mat[15] * m[7];
    ret[8] = mat[0] * m[8] + mat[4] * m[9] + mat[8] * m[10] + mat[12] * m[11];
    ret[9] = mat[1] * m[8] + mat[5] * m[9] + mat[9] * m[10] + mat[13] * m[11];
    ret[10] = mat[2] * m[8] + mat[6] * m[9] + mat[10] * m[10] + mat[14] * m[11];
    ret[11] = mat[3] * m[8] + mat[7] * m[9] + mat[11] * m[10] + mat[15] * m[11];
    ret[12] = mat[0] * m[12] + mat[4] * m[13] + mat[8] * m[14] + mat[12] * m[15];
    ret[13] = mat[1] * m[12] + mat[5] * m[13] + mat[9] * m[14] + mat[13] * m[15];
    ret[14] = mat[2] * m[12] + mat[6] * m[13] + mat[10] * m[14] + mat[14] * m[15];
    ret[15] = mat[3] * m[12] + mat[7] * m[13] + mat[11] * m[14] + mat[15] * m[15];
    return ret;
  }
  mat4 operator+(const mat4 &m) const {
    mat4 ret;
    ret[0] = mat[0] + m[0]; ret[4] = mat[4] + m[4]; ret[8] = mat[8] + m[8]; ret[12] = mat[12] + m[12];
    ret[1] = mat[1] + m[1]; ret[5] = mat[5] + m[5]; ret[9] = mat[9] + m[9]; ret[13] = mat[13] + m[13];
    ret[2] = mat[2] + m[2]; ret[6] = mat[6] + m[6]; ret[10] = mat[10] + m[10]; ret[14] = mat[14] + m[14];
    ret[3] = mat[3] + m[3]; ret[7] = mat[7] + m[7]; ret[11] = mat[11] + m[11]; ret[15] = mat[15] + m[15];
    return ret;
  }
  mat4 operator-(const mat4 &m) const {
    mat4 ret;
    ret[0] = mat[0] - m[0]; ret[4] = mat[4] - m[4]; ret[8] = mat[8] - m[8]; ret[12] = mat[12] - m[12];
    ret[1] = mat[1] - m[1]; ret[5] = mat[5] - m[5]; ret[9] = mat[9] - m[9]; ret[13] = mat[13] - m[13];
    ret[2] = mat[2] - m[2]; ret[6] = mat[6] - m[6]; ret[10] = mat[10] - m[10]; ret[14] = mat[14] - m[14];
    ret[3] = mat[3] - m[3]; ret[7] = mat[7] - m[7]; ret[11] = mat[11] - m[11]; ret[15] = mat[15] - m[15];
    return ret;
  }

  mat4 &operator*=(float f) { return *this = *this * f; }
  mat4 &operator*=(const mat4 &m) { return *this = *this * m; }
  mat4 &operator+=(const mat4 &m) { return *this = *this + m; }
  mat4 &operator-=(const mat4 &m) { return *this = *this - m; }

  operator float*() { return mat; }
  operator const float*() const { return mat; }

  float &operator[](int i) { return mat[i]; }
  const float operator[](int i) const { return mat[i]; }

  mat4 rotation() const {
    mat4 ret;
    ret[0] = mat[0]; ret[4] = mat[4]; ret[8] = mat[8]; ret[12] = 0;
    ret[1] = mat[1]; ret[5] = mat[5]; ret[9] = mat[9]; ret[13] = 0;
    ret[2] = mat[2]; ret[6] = mat[6]; ret[10] = mat[10]; ret[14] = 0;
    ret[3] = 0; ret[7] = 0; ret[11] = 0; ret[15] = 1;
    return ret;
  }
  mat4 transpose() const {
    mat4 ret;
    ret[0] = mat[0]; ret[4] = mat[1]; ret[8] = mat[2]; ret[12] = mat[3];
    ret[1] = mat[4]; ret[5] = mat[5]; ret[9] = mat[6]; ret[13] = mat[7];
    ret[2] = mat[8]; ret[6] = mat[9]; ret[10] = mat[10]; ret[14] = mat[11];
    ret[3] = mat[12]; ret[7] = mat[13]; ret[11] = mat[14]; ret[15] = mat[15];
    return ret;
  }
  mat4 transpose_rotation() const {
    mat4 ret;
    ret[0] = mat[0]; ret[4] = mat[1]; ret[8] = mat[2]; ret[12] = mat[12];
    ret[1] = mat[4]; ret[5] = mat[5]; ret[9] = mat[6]; ret[13] = mat[13];
    ret[2] = mat[8]; ret[6] = mat[9]; ret[10] = mat[10]; ret[14] = mat[14];
    ret[3] = mat[3]; ret[7] = mat[7]; ret[14] = mat[14]; ret[15] = mat[15];
    return ret;
  }

  float det() const {
    float det;
    det = mat[0] * mat[5] * mat[10];
    det += mat[4] * mat[9] * mat[2];
    det += mat[8] * mat[1] * mat[6];
    det -= mat[8] * mat[5] * mat[2];
    det -= mat[4] * mat[1] * mat[10];
    det -= mat[0] * mat[9] * mat[6];
    return det;
  }

  mat4 inverse() const {
    mat4 ret;
    float idet = 1.0f / det();
    ret[0] =  (mat[5] * mat[10] - mat[9] * mat[6]) * idet;
    ret[1] = -(mat[1] * mat[10] - mat[9] * mat[2]) * idet;
    ret[2] =  (mat[1] * mat[6] - mat[5] * mat[2]) * idet;
    ret[3] = 0.0;
    ret[4] = -(mat[4] * mat[10] - mat[8] * mat[6]) * idet;
    ret[5] =  (mat[0] * mat[10] - mat[8] * mat[2]) * idet;
    ret[6] = -(mat[0] * mat[6] - mat[4] * mat[2]) * idet;
    ret[7] = 0.0;
    ret[8] =  (mat[4] * mat[9] - mat[8] * mat[5]) * idet;
    ret[9] = -(mat[0] * mat[9] - mat[8] * mat[1]) * idet;
    ret[10] =  (mat[0] * mat[5] - mat[4] * mat[1]) * idet;
    ret[11] = 0.0;
    ret[12] = -(mat[12] * ret[0] + mat[13] * ret[4] + mat[14] * ret[8]);
    ret[13] = -(mat[12] * ret[1] + mat[13] * ret[5] + mat[14] * ret[9]);
    ret[14] = -(mat[12] * ret[2] + mat[13] * ret[6] + mat[14] * ret[10]);
    ret[15] = 1.0;
    return ret;
  }

  void zero() {
    mat[0] = 0.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
    mat[1] = 0.0; mat[5] = 0.0; mat[9] = 0.0; mat[13] = 0.0;
    mat[2] = 0.0; mat[6] = 0.0; mat[10] = 0.0; mat[14] = 0.0;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 0.0;
  }
  void identity() {
    mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
    mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = 0.0;
    mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
  }
  void rotate(const vec3 &axis,float angle) {
    float rad = angle * (M_PI / 180.0f);
    float c = cos(rad);
    float s = sin(rad);
    vec3 v = axis;
    v.normalize();
    float xx = v.x * v.x;
    float yy = v.y * v.y;
    float zz = v.z * v.z;
    float xy = v.x * v.y;
    float yz = v.y * v.z;
    float zx = v.z * v.x;
    float xs = v.x * s;
    float ys = v.y * s;
    float zs = v.z * s;
    mat[0] = (1.0f - c) * xx + c; mat[4] = (1.0f - c) * xy - zs; mat[8] = (1.0f - c) * zx + ys; mat[12] = 0.0;
    mat[1] = (1.0f - c) * xy + zs; mat[5] = (1.0f - c) * yy + c; mat[9] = (1.0f - c) * yz - xs; mat[13] = 0.0;
    mat[2] = (1.0f - c) * zx - ys; mat[6] = (1.0f - c) * yz + xs; mat[10] = (1.0f - c) * zz + c; mat[14] = 0.0;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
  }
  void rotate(float x,float y,float z,float angle) {
    rotate(vec3(x,y,z),angle);
  }
  void rotate_x(float angle) {
    float rad = angle * (M_PI / 180.0f);
    float c = cos(rad);
    float s = sin(rad);
    mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
    mat[1] = 0.0; mat[5] = c; mat[9] = -s; mat[13] = 0.0;
    mat[2] = 0.0; mat[6] = s; mat[10] = c; mat[14] = 0.0;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
  }
  void rotate_y(float angle) {
    float rad = angle * (M_PI / 180.0f);
    float c = cos(rad);
    float s = sin(rad);
    mat[0] = c; mat[4] = 0.0; mat[8] = s; mat[12] = 0.0;
    mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = 0.0;
    mat[2] = -s; mat[6] = 0.0; mat[10] = c; mat[14] = 0.0;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
  }
  void rotate_z(float angle) {
    float rad = angle * (M_PI / 180.0f);
    float c = cos(rad);
    float s = sin(rad);
    mat[0] = c; mat[4] = -s; mat[8] = 0.0; mat[12] = 0.0;
    mat[1] = s; mat[5] = c; mat[9] = 0.0; mat[13] = 0.0;
    mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
  }
  void scale(const vec3 &v) {
    mat[0] = v.x; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
    mat[1] = 0.0; mat[5] = v.y; mat[9] = 0.0; mat[13] = 0.0;
    mat[2] = 0.0; mat[6] = 0.0; mat[10] = v.z; mat[14] = 0.0;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
  }
  void scale(float x,float y,float z) {
    scale(vec3(x,y,z));
  }
  void translate(const vec3 &v) {
    mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = v.x;
    mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = v.y;
    mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = v.z;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
  }
  void translate(float x,float y,float z) {
    translate(vec3(x,y,z));
  }
  void reflect(const vec4 &plane) {
    float x = plane.x;
    float y = plane.y;
    float z = plane.z;
    float x2 = x * 2.0f;
    float y2 = y * 2.0f;
    float z2 = z * 2.0f;
    mat[0] = 1.0f - x * x2; mat[4] = -y * x2; mat[8] = -z * x2; mat[12] = -plane.w * x2;
    mat[1] = -x * y2; mat[5] = 1.0f - y * y2; mat[9] = -z * y2; mat[13] = -plane.w * y2;
    mat[2] = -x * z2; mat[6] = -y * z2; mat[10] = 1.0f - z * z2; mat[14] = -plane.w * z2;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
  }
  void reflect(float x,float y,float z,float w) {
    reflect(vec4(x,y,z,w));
  }

  void perspective(float fov,float aspect,float znear,float zfar) {
    if(fabs(fov - 90.0f) < epsilon) fov = 89.9f;
    float y = tan(fov * M_PI / 360.0f);
    float x = y;
    if(aspect > 1)
      x *= aspect;
    else
      y /= aspect;
    mat[0] = 1.0f / x; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
    mat[1] = 0.0; mat[5] = 1.0f / y; mat[9] = 0.0; mat[13] = 0.0;
    mat[2] = 0.0; mat[6] = 0.0; mat[10] = -(zfar + znear) / (zfar - znear); mat[14] = -(2.0f * zfar * znear) / (zfar - znear);
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = -1.0; mat[15] = 0.0;
  }
  void look_at(const vec3 &eye,const vec3 &dir,const vec3 &up) {
    vec3 x,y,z;
    mat4 m0,m1;
    z = eye - dir;
    z.normalize();
    x.cross(up,z);
    x.normalize();
    y.cross(z,x);
    y.normalize();
    m0[0] = x.x; m0[4] = x.y; m0[8] = x.z; m0[12] = 0.0;
    m0[1] = y.x; m0[5] = y.y; m0[9] = y.z; m0[13] = 0.0;
    m0[2] = z.x; m0[6] = z.y; m0[10] = z.z; m0[14] = 0.0;
    m0[3] = 0.0; m0[7] = 0.0; m0[11] = 0.0; m0[15] = 1.0;
    m1.translate(-eye);
    *this = m0 * m1;
  }
  void look_at(const float *eye,const float *dir,const float *up) {
    look_at(vec3(eye),vec3(dir),vec3(up));
  }
  void print() const{
    printf(" %f %f %f %f\n", mat[0], mat[4], mat[8], mat[12]);
    printf(" %f %f %f %f\n", mat[1], mat[5], mat[9], mat[13]);
    printf(" %f %f %f %f\n", mat[2], mat[6], mat[10], mat[14]);
    printf(" %f %f %f %f\n", mat[3], mat[7], mat[11], mat[15]);
  }

  float mat[16];
};

inline mat3::mat3(const mat4 &m) {
  mat[0] = m[0]; mat[3] = m[4]; mat[6] = m[8];
  mat[1] = m[1]; mat[4] = m[5]; mat[7] = m[9];
  mat[2] = m[2]; mat[5] = m[6]; mat[8] = m[10];
}

/*****************************************************************************/
/* quat                                                                      */
struct quat {

  quat() : x(0), y(0), z(0), w(1) { }
  quat(const vec3 &dir,float angle) {
    set(dir,angle);
  }
  quat(float x,float y,float z,float angle) {
    set(x,y,z,angle);
  }
  quat(const mat3 &m) {
    float trace = m[0] + m[4] + m[8];
    if(trace > 0.0) {
      float s = sqrt(trace + 1.0f);
      q[3] = 0.5f * s;
      s = 0.5f / s;
      q[0] = (m[5] - m[7]) * s;
      q[1] = (m[6] - m[2]) * s;
      q[2] = (m[1] - m[3]) * s;
    } else {
      static int next[3] = { 1, 2, 0 };
      int i = 0;
      if(m[4] > m[0]) i = 1;
      if(m[8] > m[3 * i + i]) i = 2;
      int j = next[i];
      int k = next[j];
      float s = sqrt(m[3 * i + i] - m[3 * j + j] - m[3 * k + k] + 1.0f);
      q[i] = 0.5f * s;
      if(s != 0) s = 0.5f / s;
      q[3] = (m[3 * j + k] - m[3 * k + j]) * s;
      q[j] = (m[3 * i + j] + m[3 * j + i]) * s;
      q[k] = (m[3 * i + k] + m[3 * k + i]) * s;
    }
  }

  operator float*() { return (float*)&x; }
  operator const float*() const { return (float*)&x; }

  float &operator[](int i) { return ((float*)&x)[i]; }
  const float operator[](int i) const { return ((float*)&x)[i]; }

  quat operator*(const quat &q) const {
    quat ret;
    ret.x = w * q.x + x * q.x + y * q.z - z * q.y;
    ret.y = w * q.y + y * q.w + z * q.x - x * q.z;
    ret.z = w * q.z + z * q.w + x * q.y - y * q.x;
    ret.w = w * q.w - x * q.x - y * q.y - z * q.z;
    return ret;
  }

  void set(const vec3 &dir,float angle) {
    float length = dir.length();
    if(length != 0.0) {
      length = 1.0f / length;
      float sinangle = sin(angle * (M_PI / 180.0f) / 2.0f);
      x = dir[0] * length * sinangle;
      y = dir[1] * length * sinangle;
      z = dir[2] * length * sinangle;
      w = cos(angle * (M_PI / 180.0f) / 2.0f);
    } else {
      x = y = z = 0.0;
      w = 1.0;
    }
  }
  void set(float x,float y,float z,float angle) {
    set(vec3(x,y,z),angle);
  }

  void slerp(const quat &q0,const quat &q1,float t) {
    float k0,k1,cosomega = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
    quat q;
    if(cosomega < 0.0) {
      cosomega = -cosomega;
      q.x = -q1.x;
      q.y = -q1.y;
      q.z = -q1.z;
      q.w = -q1.w;
    } else {
      q.x = q1.x;
      q.y = q1.y;
      q.z = q1.z;
      q.w = q1.w;
    }
    if(1.0 - cosomega > 1e-6) {
      float omega = acos(cosomega);
      float sinomega = sin(omega);
      k0 = sin((1.0f - t) * omega) / sinomega;
      k1 = sin(t * omega) / sinomega;
    } else {
      k0 = 1.0f - t;
      k1 = t;
    }
    x = q0.x * k0 + q.x * k1;
    y = q0.y * k0 + q.y * k1;
    z = q0.z * k0 + q.z * k1;
    w = q0.w * k0 + q.w * k1;
  }

  mat3 to_matrix() const {
    mat3 ret;
    float x2 = x + x;
    float y2 = y + y;
    float z2 = z + z;
    float xx = x * x2;
    float yy = y * y2;
    float zz = z * z2;
    float xy = x * y2;
    float yz = y * z2;
    float xz = z * x2;
    float wx = w * x2;
    float wy = w * y2;
    float wz = w * z2;
    ret[0] = 1.0f - (yy + zz); ret[3] = xy - wz; ret[6] = xz + wy;
    ret[1] = xy + wz; ret[4] = 1.0f - (xx + zz); ret[7] = yz - wx;
    ret[2] = xz - wy; ret[5] = yz + wx; ret[8] = 1.0f - (xx + yy);
    return ret;
  }

  union {
    struct {
      float x,y,z,w;
    };
    float q[4];
  };
};

} // namespace TomGine

#endif /* TG_MATHLIB_H */
