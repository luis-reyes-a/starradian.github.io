#ifndef MATHS_H //Created on October 19th 2020 10:23 am
#define MATHS_H
#include "intrinsics.h"
//NOTE we use a right handed coordinate system 

internal nil
fprox(f32 value, f32 target, f32 epsilon)
{
    f32 diff = abs(target - value);
    return diff <= epsilon;
}

//TODO intrinsic or better way to do this?...
internal f32
modf(f32 value, s32 *int_val)
{
    assert((value > S32_MIN) && (value < S32_MAX)); //isn't s32 always larger?...
    *int_val = (s32)(value);
    f32 fractional = value - (f32)*int_val;
    return fractional;
}

internal f32
fmod(f32 numerator, f32 denominator)
{
    //NOTE!!! this uses truncation instead of floor_s32 to get num_times_divisible
    //this should be the same beheavior as regular C % modulo operator, although it's implemetation-defined
    //however, in math literature I see more using the floor_s32 variant()
    //honestly, just try to keep numerator and denominator positive and use sign bit logic...
    assert(denominator != 0);
    s32 num_times_divisible = (s32)(numerator / denominator);
    f32 result = numerator - num_times_divisible*denominator;
    return result;
}

//alternates between -1, 1 every PI
internal s32
square_wave(f32 radians)
{
    f32 value  = sin(radians);
    s32 result = (value > 0) ? 1 : -1;
    return result;
}

//alternates between 0, 1
internal s32
square_wave01(f32 radians)
{
    f32 value  = sin(radians);
    s32 result = (value > 0) ? 1 : 0;
    return result;
}

struct Vector2
{
    f32 x, y;
};

internal Vector2
v2(f32 x, f32 y) {	return {x, y};	}

internal Vector2
v2(s32 x, f32 y) {	return {(f32)x, y};	}

internal Vector2
v2(f32 x, s32 y) {	return {x, (f32)y};	}

internal Vector2
v2(s32 x, s32 y) {	return {(f32)x, (f32)y};	}

internal Vector2
v2(Vector2i v)
{	return v2(v.x, v.y);	}



static Vector2
operator+(Vector2 a, Vector2 b) {	return v2(a.x + b.x, a.y + b.y);	}

static Vector2
operator+(Vector2 a, Vector2i b) {	return v2(a.x + b.x, a.y + b.y);	}

static Vector2
operator+(Vector2i a, Vector2 b) {	return v2(a.x + b.x, a.y + b.y);	}

static void
operator+=(Vector2 &a, Vector2 b) {	a = a + b;	}

static Vector2
operator-(Vector2 a, Vector2 b) {	return v2(a.x - b.x, a.y - b.y);	}

static Vector2
operator-(Vector2 a) {	return v2(-a.x, -a.y);	}

static void
operator-=(Vector2 &a, Vector2 b) {	a = a - b;	}

static Vector2
operator*(Vector2 a, f32 scale) {	return v2(a.x * scale, a.y * scale);	}

static Vector2
operator*(f32 scale, Vector2 a) {	return v2(a.x * scale, a.y * scale);	}

static Vector2
operator*(Vector2 a, s32 scale) {	return v2(a.x * scale, a.y * scale);	}

static Vector2
operator*(Vector2 a, Vector2 b) { return v2(a.x*b.x, a.y*b.y); }

static Vector2
operator*(Vector2 a, Vector2i b) { return v2(a.x*b.x, a.y*b.y); }

static Vector2
operator*(Vector2i a, Vector2 b) { return v2(a.x*b.x, a.y*b.y); }

static Vector2
operator*(s32 scale, Vector2 a) {	return v2(a.x * scale, a.y * scale);	}

static void
operator*=(Vector2 &a, f32 scale) {	a = a * scale;	}

static Vector2
operator/(Vector2 a, f32 den) {	
    assert(den); 
    return v2(a.x / den, a.y / den);	
}

static void
operator/=(Vector2 &a, f32 den) {	a = a / den;	}

static bool
operator==(Vector2 a, Vector2 b) 
{	return (a.x == b.x) && (a.y == b.y);	}



internal f32
dot2(Vector2 a, Vector2 b)
{
    f32 result = a.x*b.x + a.y*b.y; 
    return result;   
}

internal f32 //signed area of bivector 
wedge2(Vector2 a, Vector2 b)
{	return a.x*b.y - b.x*a.y;	}

internal f32
normsq(Vector2 v)
{   return v.x*v.x + v.y*v.y;   }

internal f32
norm(Vector2 v)
{   return sqroot(v.x*v.x + v.y*v.y);   }

internal f32
norm_and_normalize(Vector2 *v)
{   
    f32 length = norm(*v);
    assert(length != 0);
    *v /= length;
    return length;
}

internal f32
norm_and_noz(Vector2 *v)
{   
    f32 length_squared = dot2(*v, *v);
    if (length_squared > SQUARED(0.0001))
    {
        f32 length = sqroot(length_squared);
        assert(length != 0);
        *v /= length;
        return length;
    }
    
    *v = {};
    return 0;
}

internal Vector2
normalize(Vector2 a)
{
    f32 length_squared = normsq(a);
    assert(length_squared > 0);
    return a / sqroot(length_squared);
}

internal void
normalize(Vector2 *a) { *a = normalize(*a); }

internal Vector2
noz(Vector2 a, f32 threshold = 0.0001f)
{
    f32 length_squared = normsq(a);
    if (length_squared > SQUARED(threshold))
        return a / sqroot(length_squared);
    else return {};
}

internal void
noz(Vector2 *a, f32 threshold = 0.0001f) { *a = noz(*a, threshold); }

internal nil
fprox(Vector2 a, Vector2 b, f32 epsilon)
{
    nil result = normsq(a - b) < SQUARED(epsilon);
    return result;
}


internal void
clamp_length(Vector2 *v, f32 length)
{
    f32 sqrlen = normsq(*v);
    if ((sqrlen > 0) && (sqrlen > SQUARED(length)))
    {
        *v /= sqroot(sqrlen);
        *v *= length;
    } 
}

internal Vector2
lerp(Vector2 a, Vector2 b, f32 t)
{   return a + (b - a)*t;   }


//NOTE this does not clamp to keep point within the line...
internal Vector2
project_point_onto_line(Vector2 l1, Vector2 l2, Vector2 point)
{
    Vector2 line = l2 - l1;
    line = normalize(line);
    Vector2 result = l1 + line*dot2(line, point - l1);
    return result;
}

//NOTE this is not the same as simply projecting v onto each basis with a dot product, which only works if the basis are orthonormal
//This returns a vector such that result.x*x_basis + result.y*y_basis = v
internal Vector2
decompose(Vector2 x_basis, Vector2 y_basis, Vector2 v)
{
    //the equivalent of this in linear algebra is Cramer's rule but it's easier to understand with bivectors :)
    Vector2 result;
    f32 x_wedge_y = wedge2(x_basis, y_basis);
    assert(x_wedge_y != 0); //otherwise x_basis && y_basis are parallel and you can't project...
    result.x = wedge2(v, y_basis) /  x_wedge_y;
    result.y = wedge2(v, x_basis) / -x_wedge_y; //y_wedge_x
    return result;
}

//NOTE this assumes normal is already normalized...
internal Vector2
project_onto_normal(Vector2 normal, Vector2 v)
{
    Vector2 result = normal * dot2(v, normal);
    return result;
}


internal f32
wrap_angle(f32 angle, f32 a1, f32 a2)
{
    f32 period = a2 - a1;
    f32 result = a1 + fmod(angle - a1, period);
    if (result < a1) result += period;
    if (result >= a2) result = a2 - 0.000001f; //TODO we need something like nextafter() here
    assert(result >= a1 && result < a2);
    return result;
}

//TODO we can speed this up. Instead of calling get_angle_0_to_tau
internal f32
get_smallest_difference(f32 a2, f32 a1)
{
    f32 diff = wrap_angle(abs(a2 - a1), 0, TAU);
    if (diff > PI) return diff - TAU;
    else return diff;
}

/*
internal f32
get_angle_difference(f32 a, f32 b)
{
    assert(0);
    return 0;
}

internal nil
is_angle_between(f32 angle, f32 a, f32 b)
{
    f32 a_to_b     = get_angle_difference(a, b);
    f32 a_to_angle = get_angle_difference(a, angle);
    if (a_to_b > 0)
        return (a_to_angle >= 0) && (a_to_angle <= a_to_b);
    else 
        return (a_to_angle <= 0) && (a_to_angle >= a_to_b);
} */

internal nil
is_vector_between(Vector2 t, Vector2 a, Vector2 b)
{
    f32 a_wedge_b = wedge2(a, b);
    assert(a_wedge_b != 0); //a and b are collinear, indetermine result
    if (a_wedge_b > 0) //shortest angle is counter-clockwise
    {
        f32 a_wedge_t = wedge2(a, t);
        f32 t_wedge_b = wedge2(t, b);
        return (a_wedge_t >= 0) && (t_wedge_b >= 0);
    }
    else
    {
        f32 a_wedge_t = wedge2(a, t);
        f32 t_wedge_b = wedge2(t, b);
        return (a_wedge_t <= 0) && (t_wedge_b <= 0);
    }
}

internal nil
is_angle_between_greater(Vector2 a, Vector2 b, f32 angle)
{
    f32 cos_angle = dot2(normalize(a), normalize(b));
    return cos_angle > cos(angle);
}

//NOTE to reason to have this other than I like reading it
internal nil
is_angle_between_less(Vector2 a, Vector2 b, f32 angle)
{
    f32 cos_angle = dot2(normalize(a), normalize(b));
    return cos_angle < cos(angle);
}

//returns a rotated 90 degrees counter clockwise
internal Vector2
perp(Vector2 a)
{   return v2(-a.y, a.x);   }

//get perp of a in direction of b
internal Vector2
perp(Vector2 a, Vector2 b)
{
    f32 zaxis = a.x*b.y - a.y*b.x;
    //NOTE if a and b are colinear, zaxis is zero
    //and returned result is 0
    Vector2 result;
    result.x = -zaxis*a.y;
    result.y =  zaxis*a.x;
    return result;
}

internal f32
atan(Vector2 v)
{
    f32 radians = atan2(v.y, v.x); //-pi, pi
    return radians;
}

internal Vector2
eix2(f32 rads_counter_clockwise)
{   return v2(cos(rads_counter_clockwise), sin(rads_counter_clockwise));   }

internal Vector2 
rotate(Vector2 a, f32 rads_counter_clockwise)
{
    Vector2 b = eix2(rads_counter_clockwise);
    //just complex multiplication
    return v2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

internal void
rotate(Vector2 *a, f32 rads_counter_clockwise) { *a = rotate(*a, rads_counter_clockwise); }

union Vector3
{
    struct { f32 x, y, z;       };
    struct { Vector2 xy; f32 z; };
    
};

internal Vector3
v3(f32 x, f32 y, f32 z) {	return {x, y, z};	}

internal Vector3
v3(Vector2 v, f32 z) {	return {v.x, v.y, z};	}

internal Vector3
cross(Vector3 a, Vector3 b)
{
    Vector3 result;
    result.x = a.y*b.z - a.z*b.y;
    result.y = a.z*b.x - a.x*b.z;
    result.z = a.x*b.y - a.y*b.x;
    return result;
}

union Vector4
{
    struct{ f32 x, y, z, w; };
    struct{ f32 r, g, b, a; };
    struct{ Vector2 xy, zw; };
};

internal Vector4
v4(f32 x, f32 y, f32 z, f32 w) {	return {x, y, z, w};	}

static Vector4
operator+(Vector4 a, Vector4 b) {	return v4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);	}

static Vector4
operator-(Vector4 a, Vector4 b) {	return v4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);	}

static Vector4
operator*(Vector4 a, f32 s) {	return v4(a.x*s, a.y*s, a.z*s, a.w*s);	}

internal Vector4
lerp(Vector4 a, Vector4 b, f32 t)
{   return a + (b - a)*t;   } 

//NOTE column major
union Matrix3x3
{
    f32 e[9];
    struct
    {
        f32 e11, e21, e31;
        f32 e12, e22, e32;
        f32 e13, e23, e33;
    };
    struct
    {
        Vector3 x_basis;
        Vector3 y_basis;
        Vector3 z_basis;
    };
};

internal Matrix3x3
identity3x3()
{
    Matrix3x3 result = {};
    result.e11 = result.e22 = result.e33 = 1;
    return result;
}

internal Vector3
multiply(Matrix3x3 *m, Vector3 v)
{
    Vector3 result;
    result.x = m->e11*v.x + m->e12*v.y + m->e13*v.z;
    result.y = m->e21*v.x + m->e22*v.y + m->e23*v.z;
    result.z = m->e31*v.x + m->e32*v.y + m->e33*v.z;
    return result;
}

internal Matrix3x3
multiply(Matrix3x3 *m, Matrix3x3 *columns)
{
    Matrix3x3 result;
    result.x_basis = multiply(m, columns->x_basis);
    result.y_basis = multiply(m, columns->y_basis);
    result.z_basis = multiply(m, columns->z_basis);
    return result;
}

//// rects
struct Rect2i
{
    Vector2i min;
    Vector2i max;
};

internal nil
is_valid(Rect2i rect)
{	return (rect.max.x > rect.min.x) && (rect.max.y > rect.min.y);	}

internal Range_s32
get_y_range(Rect2i rect)
{
    Range_s32 range;
    range.min = rect.min.y;
    range.max = rect.max.y;
    assert(is_valid(range));
    return range;
}

internal Range_s32
get_x_range(Rect2i rect)
{
    Range_s32 range;
    range.min = rect.min.x;
    range.max = rect.max.x;
    assert(is_valid(range));
    return range;
}


internal Rect2i
r2i(Vector2i min, Vector2i max)
{
    Rect2i rect;
    rect.min = min;
    rect.max = max;
    assert(is_valid(rect));
    return rect; 
}

internal Rect2i
r2i(Vector2i center, s32 width, s32 height)
{
    Rect2i rect;
    rect.min.x = center.x - (width/2);
    rect.min.y = center.y - (height/2);
    rect.max.x = rect.min.x + width;
    rect.max.y = rect.min.y + height;
    assert(is_valid(rect));
    return rect;
}


internal s32 
get_width(Rect2i rect)
{
    assert(is_valid(rect));
    return rect.max.x - rect.min.x;
}

internal s32 
get_height(Rect2i rect)
{
    assert(is_valid(rect));
    return rect.max.y - rect.min.y;
}

internal Vector2i
get_center(Rect2i rect)
{
    assert(is_valid(rect));
    Vector2i pos;
    pos.x = rect.min.x + (get_width(rect)  / 2);
    pos.y = rect.min.y + (get_height(rect) / 2);
    return pos;
}

internal Rect2i
inverted_infinity_rect2i()
{
    Rect2i rect; //NOTE this one isn't valid :p
    rect.max.x = rect.max.y = S32_MIN;
    rect.min.x = rect.min.y = S32_MAX;
    return rect;
}

internal Vector2i
get_min(Vector2i a, Vector2i b)
{
    Vector2i result = v2i(min2(a.x, b.x), min2(a.y, b.y));
    return result;
}

internal Vector2i
get_max(Vector2i a, Vector2i b)
{
    Vector2i result = v2i(max2(a.x, b.x), max2(a.y, b.y));
    return result;
}




internal nil
is_point_inside(Rect2i rect, s32 x, s32 y)
{
    assert(is_valid(rect));
    return(x >= rect.min.x && x < rect.max.x && y >= rect.min.y && y < rect.max.y);
}

internal nil
do_overlap(Rect2i a, Rect2i b)
{
    assert(is_valid(a) && is_valid(b));
    nil overlap_x;
    if (b.min.x < a.min.x)
        overlap_x = (b.max.x-1) >= a.min.x;
    else
        overlap_x = b.min.x < a.max.x;
    nil overlap_y;
    if (b.min.y < a.min.y)
        overlap_y = (b.max.y-1) >= a.min.y;
    else
        overlap_y = b.min.y < a.max.y;
    return overlap_x && overlap_y;
}

internal void
expand(Rect2i *rect, s32 width, s32 height)
{
    assert(IS_EVEN(width));
    assert(IS_EVEN(height));
    rect->min.x += width/2;
    rect->max.x -= width/2;
    rect->min.y += height/2;
    rect->max.y -= height/2;
    assert(is_valid(*rect));
}

struct Ray_Intersection_Line
{
    Vector2 pos;
    Vector2 dir; //normalize
    f32 line_length;
};

struct Ray_Intersection_Result
{
    nil intersect;
    Vector2 x;
    f32 length_along_first;
    f32 length_along_second;
};

internal Ray_Intersection_Result
find_ray_intersection(Ray_Intersection_Line *p, Ray_Intersection_Line *l)
{
    Ray_Intersection_Result result = {};
    
    f32 l_wedge_p = wedge2(l->dir, p->dir);
    if (l_wedge_p)
    {
        result.intersect = true;
        f32 along_l = wedge2(p->pos, p->dir) /  l_wedge_p;
        f32 along_p = wedge2(l->pos, l->dir) / -l_wedge_p;
        
        result.x = along_p*p->dir + along_l*l->dir;
        
        Vector2 p_to_x = result.x - p->pos;
        result.length_along_first  = norm(p_to_x) * signof(dot2(p_to_x, p->dir));
        Vector2 l_to_x = result.x - l->pos;
        result.length_along_second = norm(l_to_x) * signof(dot2(l_to_x, l->dir));
    }
    return result;
}

internal nil
find_line_intersection(Vector2 p1, Vector2 p2, Vector2 l1, Vector2 l2, Vector2 *out_vector)
{
    //@NOTE this doesn't return true if lines are parallel or overlapping
    //just returns true in case where there is a single intersection point
    Ray_Intersection_Line ray_p;
    ray_p.pos = p1;
    ray_p.dir = p2 - p1;
    ray_p.line_length = norm(ray_p.dir);
    ray_p.dir = ray_p.dir / ray_p.line_length;
    
    Ray_Intersection_Line ray_l;
    ray_l.pos = l1;
    ray_l.dir = l2 - l1;
    ray_l.line_length = norm(ray_l.dir);
    ray_l.dir = ray_l.dir / ray_l.line_length;
    
    Ray_Intersection_Result result  = find_ray_intersection(&ray_p, &ray_l);
    nil intersect_as_lines = false;
    if (result.intersect &&
        result.length_along_first  >= 0 && result.length_along_first  <= ray_p.line_length &&        
        result.length_along_second >= 0 && result.length_along_second <= ray_l.line_length)
    {
        intersect_as_lines = true;
        *out_vector = result.x;
    }
    return intersect_as_lines;
}

//internal nil
//find_line_intersection(Vector2 p1, Vector2 p2, Vector2 l1, Vector2 l2, Vector2 *out)
//{
//LineIntersection x = find_line_intersection(p1,p2,l1,l2);
//nil intersects = x.type == LINE_INTERSECTION_SINGLE;
//if (intersects)
//{
//*out = x.x;
//}
//return intersects;
//}

#if 0
internal nil
find_line_intersection(Vector2 l1, Vector2 l2, Vector2 p1, Vector2 p2, Vector2 *result)
{
    Vector2 l = l2 - l1;
    f32 l_length = norm(l);
    assert(l_length);
    l = l / l_length; //normalize
    Vector2 p = p2 - p1;
    f32 p_length = norm(p);
    assert(p_length);
    p = p / p_length; //normalize
    f32 l_wedge_p = wedge2(l, p);
    nil intersect = false;
    if (l_wedge_p)
    {
        f32 along_l = wedge2(p1, p) /  l_wedge_p;
        f32 along_p = wedge2(l1, l) / -l_wedge_p;
        if (along_l >= 0 && along_l <= l_length &&
            along_p >= 0 && along_p <= p_length)
        {
            intersect = true;
            //*result = (l1 + l*along_l);// + (p1 + p*along_p);
            //*result = p1 +  p*along_p;
            f32 t_of_p = along_p / p_length;
            f32 t_of_l = along_l / l_length;
            *result = l*along_l = p*along_p;
        }
    }
    return intersect;
}
#endif

internal Vector2
expect_line_intersection(Vector2 l1, Vector2 l2, Vector2 p1, Vector2 p2)
{
    Vector2 result;
    nil found = find_line_intersection(l1, l2, p1, p2, &result);
    assert(found);
    return result;
}

internal Vector2
get_dimensions_for_aspect_ratio(f32 aspect_ratio, Vector2 dim)
{
    assert(dim.x > 0 && dim.y > 0 && aspect_ratio > 0);
    f32 original_aspect_ratio = dim.x / dim.y;
    if (aspect_ratio > original_aspect_ratio) //we want a rect wider than current, keep width, adjust height
        dim.y = (dim.x / aspect_ratio);
    else //we want a rect taller than current, keep height, adjust width
        dim.x = (dim.y * aspect_ratio);
    return dim;
}

internal Vector2i
get_dimensions_for_aspect_ratio(f32 aspect_ratio, Vector2i dim)
{
    Vector2 vector2 = v2(dim.x, dim.y);
    vector2 = get_dimensions_for_aspect_ratio(aspect_ratio, vector2);
    return v2i((s32)vector2.x, (s32)vector2.y);
}

struct Rect2f
{
    Vector2 min, max;
};

internal Vector2
get_top_left(Rect2f rect)
{ return v2(rect.min.x, rect.max.y); } 

internal Vector2
get_bot_right(Rect2f rect)
{ return v2(rect.max.x, rect.min.y); }


internal Rect2f
inverted_infinity_rect2f()
{
    Rect2f rect; //NOTE this one isn't valid :p
    rect.max.x = rect.max.y = -F32_MAX;
    rect.min.x = rect.min.y =  F32_MAX;
    return rect;
}

internal nil
is_valid(Rect2f rect)
{
    return (rect.max.x > rect.min.x) && (rect.max.y > rect.min.y);
}

internal Rect2f
r2f_xx_yy(f32 x1, f32 x2, f32 y1, f32 y2)
{
    Rect2f rect;
    if (x1 < x2)
    {
        rect.min.x = x1;
        rect.max.x = x2;
    }
    else
    {
        rect.min.x = x2;
        rect.max.x = x1;
    }
    if (y1 < y2)
    {
        rect.min.y = y1;
        rect.max.y = y2;
    }
    else
    {
        rect.min.y = y2;
        rect.max.y = y1;
    }
    assert(is_valid(rect));
    return rect;
}

internal Rect2f
make_enclosing_rect2f(Vector2 p1, Vector2 p2)
{
    Rect2f rect;
    rect.min.x = min2(p1.x, p2.x);
    rect.max.x = max2(p1.x, p2.x);
    rect.min.y = min2(p1.y, p2.y);
    rect.max.y = max2(p1.y, p2.y);
    return rect;
}

internal Rect2f 
expand(Rect2f rect, f32 width, f32 height)
{
    Rect2f result;
    result.min.x = rect.min.x - width*0.5f;
    result.max.x = rect.max.x + width*0.5f;
    result.min.y = rect.min.y - height*0.5f;
    result.max.y = rect.max.y + height*0.5f;
    return result;
}

internal void
expand(Rect2f *rect, f32 width, f32 height)
{ *rect = expand(*rect, width, height); }

//internal Rect2f 
//expand_unsafe(Rect2f rect, f32 width, f32 height)
//{
    //Rect2f result;
    //result.min.x = rect.min.x - width*0.5f;
    //result.max.x = rect.max.x + width*0.5f;
    //result.min.y = rect.min.y - height*0.5f;
    //result.max.y = rect.max.y + height*0.5f;
    //return result;
//}

internal Rect2f 
expand(Rect2f rect, f32 dim)
{ return expand(rect, dim, dim); }

internal void
expand(Rect2f *rect, f32 dim)
{ *rect = expand(*rect, dim, dim); }

//internal Rect2f 
//expand_unsafe(Rect2f rect, f32 dim)
//{ return expand_unsafe(rect, dim, dim); }

/*
internal Rect2f 
expand(Vector2 center, f32 width, f32 height)
{
   assert(width > 0 && height > 0);
   Rect2f result;
   result.min.x = center.x - width*0.5f;
   result.max.x = center.x + width*0.5f;
   result.min.y = center.y - height*0.5f;
   result.max.y = center.y + height*0.5f;
   assert(is_valid(result));
   return result;
}

internal Rect2f 
expand(Vector2 center, f32 dim)
{ return expand(center, dim, dim); } */

internal Rect2f 
r2f(Vector2 min, Vector2 max)
{
    Rect2f rect;
    rect.min = min;
    rect.max = max;
    return rect; 
}

internal Rect2f
r2f(Vector2 center, f32 width, f32 height) { return r2f(center - v2(width, height)*0.5f, center + v2(width, height)*0.5f); }

internal Rect2f
r2f(Vector2i center, f32 width, f32 height) { return r2f(v2(center), width, height); }



internal Vector2
get_center(Rect2f rect)
{ return lerp(rect.min, rect.max, 0.5f); }

internal f32
get_width(Rect2f rect)
{ return rect.max.x - rect.min.x; }

internal f32
get_height(Rect2f rect)
{ return rect.max.y - rect.min.y; }

internal void
get_center_dim(Rect2f rect, Vector2 *center, Vector2 *dim)
{
    dim->x = get_width(rect);
    dim->y = get_height(rect);
    center->x = rect.min.x + 0.5f*dim->x;
    center->y = rect.min.y + 0.5f*dim->y;
}

internal nil
is_point_inside(Rect2f rect, f32 x, f32 y)
{
    //assert(is_valid(rect));
    return(x >= rect.min.x && x < rect.max.x && y >= rect.min.y && y < rect.max.y);
}

internal nil
is_point_inside(Rect2f rect, Vector2 v)
{ return is_point_inside(rect, v.x, v.y); }

internal nil
do_overlap(Rect2f a, Rect2f b)
{
    assert(is_valid(a) && is_valid(b));
    f32 b_width  = b.max.x - b.min.x;
    f32 b_height = b.max.y - b.min.y;
    Vector2 b_center = b.min + v2(b_width, b_height)*0.5f;
    a = expand(a, b_width, b_height);
    return is_point_inside(a, b_center);
}

internal nil
is_point_inside_circle(Vector2 circle_center, f32 radius, Vector2 point)
{
    f32 distance_from_circle_squared = normsq(point - circle_center);
    return distance_from_circle_squared <= SQUARED(radius);
}


internal s32
line_intersects_rect(Vector2 p1, Vector2 p2, Rect2f rect, f32 *tmin)
{
    //NOTE because of floating point imprecision we may fail on some cases if we are intersecting
    //right on one of the corners
    //we know do if y <= max.y because of this
    //even though my rects are traditionally inclusive, exclusive
    //TODO in the corner case, should we still say we intersected twice?
    //because the game may use that to mean we passed entirely through
    //in the future we will want to handle the corner cases more precisely
    
    assert(is_valid(rect));
    s32 num_intersection_points = 0;
    f32 t = *tmin = F32_MAX;
    
    //test vertical wall collide
    f32 dx = (p2.x - p1.x);
    if (dx)
    {
        t = (rect.min.x - p1.x) / dx; //left wall
        if (t >= 0 && t <= 1.0f)
        {
            f32 y = lerp(p1.y, p2.y, t);
            if (y >= rect.min.y && y <= rect.max.y)
            {
                *tmin = min2(*tmin, t);
                num_intersection_points += 1;
            }
        }
        
        t = (rect.max.x - p1.x) / dx; //right wall
        if (t >= 0 && t <= 1.0f)
        {
            f32 y = lerp(p1.y, p2.y, t);
            if (y >= rect.min.y && y <= rect.max.y)
            {
                *tmin = min2(*tmin, t);
                num_intersection_points += 1;
            }
        }
    }
    
    //test horizontal wall collide
    f32 dy = (p2.y - p1.y);
    if (dy)
    {
        t = (rect.min.y - p1.y) / dy; //bottom wall
        if (t >= 0 && t <= 1.0f)
        {
            f32 x = lerp(p1.x, p2.x, t);
            if (x >= rect.min.x && x <= rect.max.x)
            {
                *tmin = min2(*tmin, t);
                num_intersection_points += 1;
            }
        }
        
        t = (rect.max.y - p1.y) / dy; //top wall
        if (t >= 0 && t <= 1.0f)
        {
            f32 x = lerp(p1.x, p2.x, t);
            if (x >= rect.min.x && x <= rect.max.x)
            {
                *tmin = min2(*tmin, t);
                num_intersection_points += 1;
            }
        }
    }
    
    assert(num_intersection_points < 3);
    assert(num_intersection_points ? (*tmin >= 0 && *tmin <= 1.0f) : true);
    return num_intersection_points;
}





//misc stuff


//TODO better place for this
internal Direction8
get_cardinal_direction(f32 angle)
{
    angle = wrap_angle(angle, 0, TAU);
    if     (angle > TAU*(1.0f/8) && angle <= TAU*(3.0f/8)) return NORTH;
    else if (angle > TAU*(3.0f/8) && angle <= TAU*(5.0f/8)) return WEST;
    else if (angle > TAU*(5.0f/8) && angle <= TAU*(7.0f/8)) return SOUTH;
    else                                                   return EAST;
}

internal Direction8
get_cardinal_direction(Vector2 dir)
{
    if (normsq(dir) > 0)
    {
        f32 rads = atan(dir);
        return get_cardinal_direction(rads);
    }
    else return NO_DIRECTION8;
}

internal Vector2
get_direction_vector(Direction8 dir)
{
    constexpr f32 sqrt2_over_2 = 0.70710678118f;
    switch(dir)
    {
    case NORTH:   return v2( 0,  1);
    case EAST:    return v2( 1,  0);
    case SOUTH:   return v2( 0, -1); 
    case WEST:    return v2(-1,  0);
    
    case NORTHEAST:   return v2( sqrt2_over_2,  sqrt2_over_2);
    case SOUTHEAST:   return v2( sqrt2_over_2, -sqrt2_over_2);
    case SOUTHWEST:   return v2(-sqrt2_over_2, -sqrt2_over_2);
    case NORTHWEST:   return v2(-sqrt2_over_2,  sqrt2_over_2);
    
    case NO_DIRECTION8: return v2(0,0); 
    default:  assert(0); return {};
    }
}

internal Vector2i
get_manhattan_vector2i(Direction8 dir)
{
    switch(dir)
    {
    case NORTH:   return v2i( 0,  1);
    case EAST:    return v2i( 1,  0);
    case SOUTH:   return v2i( 0, -1); 
    case WEST:    return v2i(-1,  0);
    
    case NORTHEAST:   return v2i( 1,  1);
    case SOUTHEAST:   return v2i( 1, -1);
    case SOUTHWEST:   return v2i(-1, -1);
    case NORTHWEST:   return v2i(-1,  1);
    
    case NO_DIRECTION8: return v2i(0,0); 
    default:  assert(0); return {};
    }
}

internal Vector2
get_manhattan_vector(Direction8 dir)
{
    switch(dir)
    {
    case NORTH:   return v2( 0,  1);
    case EAST:    return v2( 1,  0);
    case SOUTH:   return v2( 0, -1); 
    case WEST:    return v2(-1,  0);
    
    case NORTHEAST:   return v2( 1,  1);
    case SOUTHEAST:   return v2( 1, -1);
    case SOUTHWEST:   return v2(-1, -1);
    case NORTHWEST:   return v2(-1,  1);
    
    case NO_DIRECTION8: return v2(0,0); 
    default:  assert(0); return {};
    }
}

internal Vector2i
get_direction_vector2i(Direction8 dir)
{
    switch(dir)
    {
    case NORTH: return v2i( 0,  1);
    case EAST:  return v2i( 1,  0);
    case SOUTH: return v2i( 0, -1); 
    case WEST:  return v2i(-1,  0);
    default:    return v2i( 0,  0);
    }
}

struct Line2f
{
    Vector2 start;
    Vector2 delta;
};

internal nil
is_valid(Line2f line) { return normsq(line.delta) > 0; }

internal Line2f
l2f(Vector2 start, Vector2 delta)
{
    Line2f line = {start, delta};
    return line;
}

internal Line2f
l2f(Vector2i start, Vector2i delta)
{
    Line2f line = {v2(start), v2(delta)};
    return line;
}

internal Line2f
l2f_pos(Vector2 start, Vector2 end)
{
    Line2f line = {start, end - start};
    return line;
}


#endif //MATHS_H
