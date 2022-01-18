#ifndef INTRINSICS_H
#define INTRINSICS_H
#include "intrin.h" 

internal f32 //SSE1 
sqroot(f32 radians)
{
    __m128 lane = _mm_sqrt_ss(_mm_set_ss(radians));
    f32 result = _mm_cvtss_f32(lane);
    return result;
}


//@TODO intrinsic for this one
internal f32
abs(f32 x)
{
   //TODO can I just remove the sign bit?
   return (x < 0) ? -x : x;
}

//internal s32
//abs(s32 x)
//{
   //return (x < 0) ? -x : x;
//}


//@TODO all these trig functions are SVML
//the cpuid flags vary between them, is that what we care about?
internal f32 
cos(f32 radians)
{
    __m128 lane = _mm_cos_ps(_mm_set_ss(radians));
    f32 result = _mm_cvtss_f32(lane);
    return result;
}

internal f32 
sin(f32 radians)
{
    __m128 lane = _mm_sin_ps(_mm_set_ss(radians));
    f32 result = _mm_cvtss_f32(lane);
    return result;
}

internal f32 
tan(f32 radians)
{
    __m128 lane = _mm_tan_ps(_mm_set_ss(radians));
    f32 result = _mm_cvtss_f32(lane);
    return result;
}

internal f32
acos(f32 ratio)
{
    __m128 lane = _mm_acos_ps(_mm_set_ss(ratio));
    f32 result = _mm_cvtss_f32(lane);
    return result;
}

internal f32
asin(f32 ratio)
{
    __m128 lane = _mm_asin_ps(_mm_set_ss(ratio));
    f32 result = _mm_cvtss_f32(lane);
    return result;
}

internal f32
atan(f32 ratio)
{
    __m128 lane = _mm_atan_ps(_mm_set_ss(ratio));
    f32 result = _mm_cvtss_f32(lane);
    return result;
}

internal f32
atan2(f32 y, f32 x)
{
    assert(!((x == 0) && (y == 0))); //TODO what do we return here/...
    __m128 my = _mm_set_ss(y);
    __m128 mx = _mm_set_ss(x);
    __m128 lane = _mm_atan2_ps(my, mx);
    f32 result = _mm_cvtss_f32(lane);
    return result;
}

//> rounding stuff
internal s32 //SSE
round_s32(f32 value)
{
   return _mm_cvtss_si32(_mm_set_ss(value));
}

internal u32 //SSE
round_u32(f32 value)
{
   assert(value >= 0);
   return to_u32(_mm_cvtss_si32(_mm_set_ss(value))); //TODO is there a convert to u32?
}

internal f32 //SSE
round(f32 value)
{
   return (f32)round_s32(value);
}

internal s32 //SSE4.1
floor_s32(f32 value)
{
   return _mm_cvtss_si32(_mm_floor_ss(_mm_setzero_ps(), _mm_set_ss(value)));
}

internal s32 //SSE4.1
ceil_s32(f32 value)
{
   //memcpy(0, 0, 1024);
   return _mm_cvtss_si32(_mm_ceil_ss(_mm_setzero_ps(), _mm_set_ss(value)));
}

internal f32 //SSE4.1
floor(f32 value)
{
   return _mm_cvtss_f32(_mm_floor_ss(_mm_setzero_ps(), _mm_set_ss(value)));
}

internal f32 //SSE4.1
ceil(f32 value)
{
   //memcpy(0, 0, 1024);
   return _mm_cvtss_f32(_mm_ceil_ss(_mm_setzero_ps(), _mm_set_ss(value)));
}



#if 0
//this is in vcruntime_string.h but why can't I just stick it in here and let compiler patch it up?
//it says name contradicts previous declaration but if so why can't I just use memcpy?
void* __cdecl memcpy(
                     _Out_writes_bytes_all_(_Size) void* _Dst,
                     _In_reads_bytes_(_Size)       void const* _Src,
                     _In_                          size_t      _Size
                     );
#endif




#endif //INTRINSICS_H
