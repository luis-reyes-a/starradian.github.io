#ifndef UBIQUITOUS_H //Created on October 18th 2020 8:32 pm
#define UBIQUITOUS_H

#include "stdint.h"
#include "float.h"
#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

#define true  1
#define false 0 

#define null  nullptr

#define WRITE_TO_NULL_POINTER (*(int *)0 = 1)
#define assert(expr)       ((expr) ? 1 : (__debugbreak(), WRITE_TO_NULL_POINTER)) 
#define cassert(expr, ...) static_assert(expr, "" __VA_ARGS__)
#define breakpoint __debugbreak()

//#define countof(array) (sizeof(array) / sizeof(array[0]))
#define countof(array) _countof(array) //NOTE msvc specific, nicer bc it catches errors when passing raw pointer to it
#define internal      static
#define global        static

#define let auto

#define PI    3.14159265358979323846f
#define TAU   6.28318530717958647692f
#define SQRT2 1.41421356237f //TODO get better approx
#define SQUARED(x) ((x)*(x))

//NOTE don't put these on the same line unless you want to use __COUNTER__
#define UNIQUE_FILE_LOCATION_STRING_INTERNAL2(file, linenum) file "@" #linenum
#define UNIQUE_FILE_LOCATION_STRING_INTERNAL(file, linenum) UNIQUE_FILE_LOCATION_STRING_INTERNAL2(file, linenum)
#define UNIQUE_FILE_LOCATION_STRING strlit(UNIQUE_FILE_LOCATION_STRING_INTERNAL(__FILE__, __LINE__))

//NOTE we use this just to not confuse 4coder from thinking a type is a function...
#define CONSTRUCTOR(typename_funame) typename_funame##::##typename_funame
#define DESTRUCTOR(typename_funame)  typename_funame##::~##typename_funame

typedef int8_t  s8;
typedef int16_t s16; 
typedef int32_t s32;
typedef int64_t s64; 

typedef uint8_t  u8;
typedef uint16_t u16; 
typedef uint32_t u32; 
typedef uint64_t u64; 

typedef s32    nil; 
typedef size_t uintptr; 
typedef float  f32;
typedef double f64;

#define ZERO_STRUCT(var)      zero_memory(var, sizeof (*(var)))
#define COPY_STRUCT(dst, src) copy_memory(dst, src, sizeof(*(src)))

//TODO intrinsic for this?
internal s32
signof(f32 x)
{	return (x < 0) ? -1 : ((x > 0) ? 1 : 0);	}

//TODO intrinsic for this?
internal s32
signof(s32 x)
{	return (x < 0) ? -1 : ((x > 0) ? 1 : 0);	}

internal f32
squared(f32 x) { return x*x; }


#define U8_MAX  UINT8_MAX
#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX
#define U64_MAX UINT64_MAX

#define S8_MIN  INT8_MIN
#define S8_MAX  INT8_MAX
#define S16_MIN INT16_MIN
#define S16_MAX INT16_MAX
#define S32_MIN INT32_MIN
#define S32_MAX INT32_MAX
#define S64_MIN INT64_MIN
#define S64_MAX INT64_MAX

#define F32_MAX FLT_MAX   //TODO does this cause interrupts/exceptions when using?

#define IS_ODD(x)  ((x) & 1)
#define IS_EVEN(x) (!IS_ODD(x))
#define IS_POW2(x) (((x) & ((x) - 1)) == 0)

#define CONCAT_INTERNAL(x,y) x##y
#define CONCAT(x,y) CONCAT_INTERNAL(x,y)

#define SWAP(a, b)        do{ auto  CONCAT(_temp, __LINE__) = a; a = b; b = CONCAT(_temp, __LINE__); } while(0)
#define SWAPt(a, b, type) do{ _type CONCAT(_temp, __LINE__) = a; a = b; b = CONCAT(_temp, __LINE__); } while(0)

#define min2(a, b) (((a) < (b)) ? (a) : (b))
#define max2(a, b) (((a) > (b)) ? (a) : (b))

internal f32
clamp(f32 value, f32 min, f32 max)
{
    assert(min < max);
    return (value < min) ? min : ((value > max) ? max : value);
}

internal void
clamp(f32 *value, f32 min, f32 max) { *value = clamp(*value, min, max); }

internal s32
clamp(s32 value, s32 min, s32 max)
{
    assert(min < max);
    return (value < min) ? min : ((value > max) ? max : value);
}

internal void
clamp(s32 *value, s32 min, s32 max) { *value = clamp(*value, min, max); }

internal f32 
minval2(f32 a, f32 b) { return (a < b) ? a : b; }

internal f32 
maxval2(f32 a, f32 b) { return (a > b) ? a : b; }

internal s32 
minval2(s32 a, s32 b) { return (a < b) ? a : b; }

internal s32 
maxval2(s32 a, s32 b) { return (a > b) ? a : b; }


internal f32
lerp(f32 a, f32 b, f32 t)
{   return a + (b - a)*t;   }

internal f32
unlerp(f32 a, f32 b, f32 value)
{
    f32 divisor = (b - a);
    assert(divisor);
    return (value - a) / divisor;
}


internal f32
unlerp(s32 a, s32 b, s32 value)
{
    s32 divisor = (b - a);
    assert(divisor);
    return (f32)(value - a) / (f32)divisor;
}

internal f32
remap(f32 in_min, f32 in_max, f32 in, f32 out_min, f32 out_max)
{
    assert(in_min != in_max && out_min != out_max);
    f32 result = (in - in_min) / (in_max - in_min); // convert to 0-1 range
    result = out_min + (out_max-out_min)*result; //put it in our desired range
    //NOTE this is the exact same thing as result = lerp(out_min, out_max, unlerp(in_min, in_max, in));
    return result;
}

internal f32 //input and output parameters clamped to 01
remap01(f32 in_min, f32 in_max, f32 in, f32 out_min, f32 out_max)
{
    assert(in_min != in_max && out_min != out_max);
    f32 t = (in - in_min) / (in_max - in_min); // convert to 0-1 range
    clamp(&t, 0, 1);
    f32 result = out_min + t*(out_max-out_min); //put it in our desired range
    //NOTE this is the exact same thing as result = lerp(out_min, out_max, unlerp01(in_min, in_max, in));
    return result;
}



internal void
clamp_hi(f32 *value, f32 max) { if (*value > max) *value = max; }

internal void
clamp_lo(f32 *value, f32 min) { if (*value < min) *value = min; }

/* NOTE we could use inheritance here
struct Flags32 
{
    u32 flags;
    nil  has(u32 flag)
        {	return (flags & flag);	}
    void set(u32 _flags)
    {	flags |= _flags;	}
    void clear(u32 _flags)
    {	flags &= ~_flags;	}
}; */


#define setfl(var,    fls)  ( (var).flags |=  (fls) )
#define clearfl(var,  fls)  ( (var).flags &= ~(fls) )
#define togglefl(var, fls)  ( (var).flags ^=  (fls) )
#define hasfl(var, fl)      ( (var).flags &   (fl)  )
#define allfl(var, fls)     (((var).flags &   (fls)) == (fls))

//#define for_index_inc(s32, index_name, count)     for(s32 index_name = 0; index_name < (count); index_name += 1)
//#define for_index_dec(s32, index_name, count)     for(s32 index_name = (count)-1; index_name >= 0; index_name -= 1)

#define for_index_inc(type, i, count)     for(type i = 0; i < (count); i += 1)
#define for_index_dec(type, i, count)     for(type i = (count); ((i) --) > 0;	)

//#define for_index_inc_u32(index_name, count) for(u32 index_name = 0; index_name < (count); index_name += 1)
//#define for_index_dec_u32(index_name, count) for(u32 index_name = (count); ((index_name) --) > 0;	)

// none stupid version of these if we could just specify a 'next_in_list' token in template structs that just use here... :(
//#define sll_foreach(varname, lst)             for(let varname = (lst).head; varname; varname = varname->next_in_list)
//#define sll_foreach_remove(varname, lst)      for(let varname = (lst).pop_head(); varname; varname = (lst).pop_head())
//#define dll_foreach(varname, sentinel)         for(let varname = (sentinel)->next_in_list; varname != (sentinel); varname = varname->next_in_list)
//#define dll_foreach_reverse(varname, sentinel) for(let varname = (sentinel)->prev_in_list; varname != (sentinel); varname = varname->prev_in_list)

#define sll_foreach(varname, lst)         for(let varname = (lst).head; varname; varname = (lst).get_next_of(varname))
#define sll_foreach_remove(varname, lst)  for(let varname = (lst).pop_head(); varname; varname = (lst).pop_head())

#define dll_foreach(varname, lst)         for(let varname = (lst).head; varname; varname = (lst).get_next_of(varname))
#define dll_foreach_reverse(varname, lst) for(let varname = (lst).tail; varname; varname = (lst).get_prev_of(varname))

#define dll_sentinel_foreach(varname, lst)         for(let varname = (lst).get_next_of(&(lst).sentinel); varname != &(lst).sentinel; varname = (lst).get_next_of(varname))
#define dll_sentinel_foreach_reverse(varname, lst) for(let varname = (lst).get_prev_of(&(lst).sentinel); varname != %(lst).sentinel; varname = (lst).get_prev_of(varname))



#define stack_foreach(varname, stack)         for(let varname = (stack).e; varname < ((stack).e + (stack).count); varname += 1)
#define stack_foreach_reverse(varname, stack) for(let varname = (stack).e + (stack).count - 1; varname >= (stack).e; varname -= 1)
//#define stack_consume_reverse(varname, stack) for(let varname = (stack).e + (stack).count - 1; (stack).count > 0; varname = (stack).e + --(stack).count - 1)
#define array_foreach(varname, array)         for(let varname = (array); varname < ((array) + countof(array)); varname += 1)
#define array_foreach_reverse(varname, array) for(let varname = (array) + countof(array)-1; varname >= array; varname -= 1)

#define CLAMP_HI(var, max) (((var) > (max)) ? ((var) = (max)) : (var))
#define CLAMP_LO(var, min) (((var) < (min)) ? ((var) = (min)) : (var))
#define CLAMP(var, min, max) (assert(max > min), CLAMP_LO(var, min), CLAMP_HI(var, max))

template <typename TYPE>
internal u64
array_pointer_index64(TYPE *array, u32 array_size, TYPE *ptr)
{
    assert(ptr >= array && ptr < (array + array_size));
    u64 index64 = (u64)(ptr - array);
    return index64;
}

template <typename TYPE, u32 SIZE>
internal u64
array_pointer_index64(TYPE (&array)[SIZE], TYPE *ptr)
{ return array_pointer_index64(&array[0], SIZE, ptr); }

template <typename TYPE>
internal u32
array_pointer_index32(TYPE *array, u32 array_size, TYPE *ptr)
{
    u64 index64 = array_pointer_index64(array, array_size, ptr);
    assert(index64 <= U32_MAX);
    return (u32)index64;
}

template <typename TYPE, u32 SIZE>
internal u32
array_pointer_index32(TYPE (&array)[SIZE], TYPE *ptr)
{ return array_pointer_index32(&array[0], SIZE, ptr); }


#define ptr_in_base_size(pointer,  base, size)  ( (u8 *)(pointer) >= (u8 *)(base) && (u8 *)(pointer) < ((u8 *)(base) + (size)))
#define ptr_in_base_count(pointer, base, count) ( (pointer) >= (base) && (pointer) < ((base) + (count)) )

struct Range_f32
{
    f32 min, max;
};


struct Range_s32
{
    s32 min, max;
};

internal nil
is_valid(Range_s32 range) //NOTE we allow "null" ranges where max == min
{	return (range.max >= range.min);	}

internal Range_s32 
range_s32(s32 min, s32 max)
{
    Range_s32 range = {min, max};
    assert(is_valid(range));
    return range;
}

internal s32
get_length(Range_s32 range)
{
    s32 length = range.max - range.min;
    assert(length >= 0);
    return length;
}

internal nil
in_range(Range_s32 range, s32 x)
{	return (x >= range.min) && (x < range.max);	}

internal nil
in_range(Range_s32 range, Range_s32 subrange)
{	return in_range(range, subrange.min) && in_range(range, subrange.max - 1);	}




static Range_s32
operator+(Range_s32 range, s32 offset)
{	return range_s32(range.min + offset, range.max + offset);	}

static void
operator+=(Range_s32 &range, s32 offset)
{	range = range + offset;	}

static Range_s32
operator-(Range_s32 range, s32 offset)
{	return range_s32(range.min - offset, range.max - offset);	}

static void
operator-=(Range_s32 &range, s32 offset)
{	range = range - offset;	}


template <typename T>
struct Defer_Scope_End {
	T lambda;
	Defer_Scope_End(T lambda) : lambda(lambda)	{ }
	~Defer_Scope_End() { lambda(); }
};

template <typename T>
internal Defer_Scope_End<T> 
make_defer_scope_end(T lambda) 
{	return Defer_Scope_End<T>(lambda);	}

#define defer(code)   const auto& CONCAT(defer__, __LINE__) = make_defer_scope_end( [&]() {code} )



internal s32 
to_s32(uintptr value)
{	assert(value <= S32_MAX);	return (s32)(value);	}

internal s32 
to_s32(u32 value)
{	assert(value <= S32_MAX);	return (s32)(value);	}

internal u32
to_u32(uintptr value)
{	assert(value <= U32_MAX);	return (u32)(value);	}

internal u32 
to_u32(s32 value)
{	assert(value >= 0);	return (u32)(value);	}

internal u8
to_u8(s32 value)
{	assert((value >= 0) && (value <= U8_MAX));	return (u8)(value);	}

internal u16
to_u16(u32 value) { assert(value <= U16_MAX); return (u16)(value); }


//NOTE if you want something slightly better, look into PCG 
internal u32 
xorshift(u32 *seed)
{
    assert(*seed);
    //Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" 
    u32 x = *seed;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *seed = x;
    return x;
}


internal s32
get_random_index(u32 *seed, s32 size)
{
    assert(size > 0);
    xorshift(seed);
    return *seed % size;
}

internal nil
do_random_chance(u32 *seed, s32 num_chances, s32 total_num_chances)
{
    nil index = get_random_index(seed, total_num_chances);
    return index < num_chances;
}

//TODO is this a valid way to do random occurence
internal nil
pull_random_chance(u32 *seed, s32 num_chances, s32 *total_num_chances)
{
    assert(*total_num_chances > 0);
    assert(num_chances <= *total_num_chances);
    nil index = get_random_index(seed, *total_num_chances);
    *total_num_chances -= num_chances;
    return index < num_chances;
    
    //s32 num_chances = 36;
    //if      (pull_random_chance(seed, 1, &num_chances))  /* 1/36  chance*/;
    //else if (pull_random_chance(seed, 12, &num_chances)) /*12/35 chance*/;
    //else if (pull_random_chance(seed, 12, &num_chances)) /*12/23 chance*/;
    //else if (pull_random_chance(seed, 6, &num_chances))  /* 6/11 chance*/;
    //else if (pull_random_chance(seed, 5, &num_chances))  /* 5/ 5 chance*/;
}

internal f32
get_random_t(u32 *seed)
{
    f32 result = (f32)xorshift(seed) / (f32)U32_MAX;
    assert(result >= 0 && result <= 1.0f);
    return result;
}


internal s32
get_random_in_range(u32 *seed, s32 min, s32 max)
{
    assert(max > min);
    s32 offset = get_random_index(seed, max - min);
    return min + offset;
}

internal s32
get_random_in_range(u32 *seed, Range_s32 range)
{
    s32 result = range.min;
    s32 length = get_length(range);
    if (length > 0)
        result += get_random_index(seed, length);
    return result;
}


/*
internal nil
get_random_chance(u32 *seed, f32 percent_chance_happening)
{
   
}

internal s32
random_between(u32 *seed, s32 min, s32 max)
{
   s32 result;
#if 0
   result = (s32)(min + (max - min)*random01(seed));
#else
   result = min + get_random_index(seed, max - min + 1);
#endif
   assert(result >= min && result <= max);
   return result;
}
*/

//internal Direction 
//random_direction(u32 *seed)
//{
//return (Direction)random_between(seed, 0, 3);
//}


//NOTE just to make my editor highlight these members
internal void push_head(); 
internal void push_tail(); 
internal void push_tail_slowly(); 
internal void pop_head(); 
internal void pop_tail(); 
internal void remove(); 


#define SLL(type, next)       Singly_Linked_List<type, offsetof(type, next)>
#define DLL(type, prev, next) Doubly_Linked_List<type, offsetof(type, prev), offsetof(type, next)>

template <typename TYPE, u32 NEXT_BYTE_OFFSET>
struct Singly_Linked_List
{
    TYPE *head;
    //NOTE var must be address of the var, so pointer
    #define NEXT(varptr) (*((TYPE **)((u8 *)(varptr) + NEXT_BYTE_OFFSET)))
    #define PREV(varptr) (*((TYPE **)((u8 *)(varptr) + PREV_BYTE_OFFSET)))
    
    //NOTE this is defined for my sll_foreach macro
    TYPE *get_next_of(TYPE *element) { return NEXT(element); }
    
    
    TYPE *push_head(TYPE *element)
    {
        assert(NEXT(element) == null);
        NEXT(element) = head;
        head = element;
        return head;
    }
    
    TYPE *push_tail_slowly(TYPE *element)
    {
        assert(NEXT(element) == null);
        if (!head) head = element;
        else
        {
            TYPE *tail = head;
            while(NEXT(tail)) tail = NEXT(tail);
            NEXT(tail) = element;
        }
        return element;
    }
    
    TYPE *pop_head()
    {
        TYPE *result = head;
        if (result)
        {
            head = NEXT(head);
            NEXT(result) = null;
        }
        return result;
    }
    
    TYPE *remove(TYPE *prev, TYPE *elem)
    {
        assert(elem);
        if (prev)
        {
            assert(NEXT(prev) == elem);
            NEXT(prev) = NEXT(elem);
            NEXT(elem) = null;
            return elem;
        }
        else //elem must be head then
        {
            assert(elem == head);
            return pop_head();
        }
    }
};


template <typename TYPE, u32 PREV_BYTE_OFFSET, u32 NEXT_BYTE_OFFSET> 
struct Doubly_Linked_List
{
    TYPE *head;
    TYPE *tail;
    
    //NOTE just for dll_foreach macro
    inline TYPE *get_next_of(TYPE *element) { return NEXT(element); }
    inline TYPE *get_prev_of(TYPE *element) { return PREV(element); }
    
    nil will_init_head_and_tail(TYPE *element)
    {
        assert(PREV(element) == null);
        assert(NEXT(element) == null);
        assert((head && tail) || (!head && !tail));
        
        if (!head)
        {
            head = tail = element;
            return true;
        }    
        else return false;
    }
    
    TYPE *push_head(TYPE *element)
    {       
        if (!will_init_head_and_tail(element))
        {
            NEXT(element) = head;
            PREV(head)    = element;
            head = element;
        }
        
        assert(NEXT(tail) == null);
        assert(PREV(head) == null);
        return element;
    }
    TYPE *push_tail(TYPE *element)
    {
        if (!will_init_head_and_tail(element))
        {
            PREV(element) = tail;
            NEXT(tail)    = element;
            tail = element;
        }
        
        assert(NEXT(tail) == null);
        assert(PREV(head) == null);
        return element;
    }
    
    TYPE *pop_head() 
    { 
        if (head)
        {
            TYPE *result = head;
            assert(PREV(head) == null);
            if (NEXT(head))
            {
                head = NEXT(head);
                PREV(head) = null;
            }
            else 
            {
                assert(head == tail);
                head = tail = null;
            }    
            
            NEXT(result) = null;
            return result;
        }
        else return null;
    }
    TYPE *pop_tail()
    {
        if (tail)
        {
            TYPE *result = tail;
            assert(NEXT(tail) == null);
            if (PREV(tail))
            {
                tail = PREV(tail);
                NEXT(tail) = null;
            }
            else 
            {
                assert(tail == head);
                tail = head = null;
            } 
            
            PREV(result) = null;
            return result;
        }
        else return null;
    } 
    
    #if DEBUG_BUILD
    nil is_in_list(TYPE *element)
    {
        dll_foreach(node, *this) if (node == element) return true;
        return false;
    }
    #endif
    
    TYPE *remove(TYPE *element)
    {
        //assert(is_in_list(element)); 
        if      (element == head) return pop_head();
        else if (element == tail) return pop_tail();
        else 
        {
            //if this fails then element doesn't have prev or next pointer
            PREV(NEXT(element)) = PREV(element);
            NEXT(PREV(element)) = NEXT(element);
            NEXT(element) = PREV(element) = null;
            return element;
        }
    }
    #undef NEXT
    #undef PREV
};



#if 0
template <typename TYPE, u32 PREV_BYTE_OFFSET, u32 NEXT_BYTE_OFFSET> 
struct Doubly_Linked_List_Sentinel
{
    TYPE sentinel;
    
    
    TYPE *push_head(TYPE *element)
    {
        if (!NEXT(sentinel))
        {
            assert(!PREV(sentinel));
            NEXT(sentinel) = &sentinel;
            PREV(sentinel) = &sentinel;
        }
        assert(!PREV(sentinel) ? !NEXT(sentinel) : true);
        
        
        assert(PREV(element) == null);
        assert(NEXT(element) == null);
        
        NEXT(element) = NEXT(sentinel);
        PREV(element) = sentinel;
        
        PREV(NEXT(sentinel)) = element;
        NEXT(sentinel)       = element;
        return element;
    }
    TYPE *push_tail(TYPE *element)
    {
        if (!PREV(sentinel))
        {
            assert(!NEXT(sentinel));
            PREV(sentinel) = &sentinel;
            NEXT(sentinel) = &sentinel;
        }
        assert(!NEXT(sentinel) ? !PREV(sentinel) : true);
        
        assert(PREV(element) == null);
        assert(NEXT(element) == null);
        
        PREV(element) = PREV(sentinel);
        NEXT(element) = sentinel;
        
        NEXT(PREV(sentinel)) = element;
        PREV(sentinel)       = element;
        return element;
    }
    
    TYPE *remove(TYPE *element)
    {
        PREV(NEXT(element)) = PREV(element);
        NEXT(PREV(element)) = NEXT(element);
        return element;
    }
    
    TYPE *pop_head()
    {
        if (NEXT(sentinel) != &sentinel)
        {
            return dll_remove_element(NEXT(sentinel));
        }
        else return null;
    }
    
    TYPE *pop_tail()
    {
        if (PREV(sentinel) != &sentinel)
        {
            return dll_remove_element(PREV(sentinel));
        }
        else return null;
    }
    
    #undef NEXT
    #undef PREV
};
#endif



template <typename TYPE, s32 max_count>
struct Stack 
{
    s32 count;   
    TYPE e[max_count];    
    
    s32 left()    
    {    
        assert(count >= 0 && count <= (max_count));    
        return (max_count) - count;    
    }    
    
    TYPE *push()    
    {    
        assert(count >= 0 && count <= (max_count));    
        return (count < (max_count)) ? (e + count++) : 0;    
    } 
    TYPE *push(TYPE *var_to_set_to)    
    {
        TYPE *result = push();
        *result = *var_to_set_to;
        return result;   
    } 
    TYPE *push(TYPE var_to_set_to) { return push(&var_to_set_to); } 
    
    TYPE *insert_by_shift(s32 index, TYPE value_to_set_to) //inserts
    {
        assert(count < max_count);
        assert(index >= 0 && index <= count); //<= because we may want to insert at new last element
        count += 1;
        for(s32 next = count - 1; next >= (index + 1); next -= 1)
            e[next] = e[next - 1];
        e[index] = value_to_set_to;
        return e + index;
    }
    TYPE *insert_by_swap(s32 index, TYPE value_to_set_to) //inserts
    {
        assert(count < max_count);
        assert(index >= 0 && index <= count); //<= because we may want to insert at new last element
        *(e + count++) = e[index];
        e[index] = value_to_set_to;
        return e + index;
    }
    TYPE *pop()     
    {     
        assert(count >= 0 && count <= (max_count));    
        return (count > 0) ? (e + --count) : 0;    
    }    
    TYPE *remove(s32 index) /*NOTE this moves last element to index*/	 
	{
		assert(count >= 0 && count <= (max_count));    
        assert(index >= 0 && index < count);    
		SWAP(e[index], e[count - 1]);	 
        return pop();	 
	}
    TYPE *remove(TYPE *element)/*NOTE this moves last element to index*/	 
	{
        uintptr index = (uintptr)(element - e);
        return remove(to_s32(index));
	}
    TYPE *remove_randomly(u32 *seed)/*NOTE this moves last element to index*/	 
	{
        s32 index = get_random_index(seed, count);
        return remove(index);
	}
    TYPE &operator[](s32 index)
    {
        assert(count >= 0 && count <= (max_count));    
        assert(index >= 0 && index < count);    
        return e[index];    
    }
    TYPE *operator+(s32 offset)
    {
        assert(count >= 0 && count <= (max_count));    
        assert(offset >= 0 && offset < count);    
        return e + offset;    
    }
};

//template <typename TYPE, s32 size>
//internal Stack<TYPE>
//make_stack_from_array(TYPE (&array)[SIZE])

template <typename T>
struct Slice
{
    T *e;
    s32 max_count;
    s32 count;
    
    s32 left()    
    {    
        assert(count >= 0 && count <= (max_count));    
        return (max_count) - count;    
    }    
    T *push()    
    {    
        assert(count >= 0 && count <= (max_count));    
        return (count < (max_count)) ? (e + count++) : 0;    
    }    
    T *pop()     
    {     
        assert(count >= 0 && count <= (max_count));    
        return (count > 0) ? (e + --count) : 0;    
    }     
    T *remove(s32 index)/*NOTE this moves last element to index*/	 
    {
        assert(count >= 0 && count <= (max_count));    
        assert(index >= 0 && index < count);    
        SWAP(e[index], e[count - 1]);	 
        return pop();	 
    }
    T &operator[](s32 index)
    {
        assert(count >= 0 && count <= (max_count));    
        assert(index >= 0 && index < count);    
        return e[index];    
    }
    T *operator+(s32 offset)
    {
        assert(count >= 0 && count <= (max_count));    
        assert(offset >= 0 && offset < count);    
        return e + offset;    
    }
}; 

#if 0
template <typename TYPE>
internal Slice<TYPE>
make_slice(TYPE *array, u32 max_count)
{
    Slice<TYPE> slice = {};
    slice.e = array;
    slice.max_count = max_count;
    return slice;
}
#endif

template <typename TYPE, s32 max_count>
struct Stack_Front_Back 
{
    s32 front_count; 
    s32 back_count;
    TYPE e[max_count]; 
    
    s32 left()
    {
        assert(front_count <= max_count);
        assert(back_count <= max_count);
        return max_count - front_count - back_count;
    }
    
    TYPE *front_push()
    {
        TYPE *result = 0;
        if (left())
            result = e + front_count++;
        return result;
    }
    
    TYPE *front_pop()
    {
        TYPE *result = 0;
        if (front_count > 0)
            result = e + --front_count;
        return result;
    }
    
    TYPE *back_push()
    {
        TYPE *result = 0;
        if (left())
            result = (e + max_count) - ++back_count;
        return result;
    }
    
    TYPE *back_pop()
    {
        TYPE *result = 0;
        if (back_count > 0)
            result = (e + max_count) - back_count++;
        return result;
    }
    
    //I doubt we'll ever need this one honestly...
    //TYPE *remove(s32 index)/*NOTE this moves last element to index*/	 
	//{
        //if (index >= 0 && index < front_count)
        //{
            //SWAP(e[index], e[front_count - 1]);	 
            //return front_pop();	
        //}
        //else
        //{
            //assert(index >= (max_count - back_count)  &&  index < max_count);
            //SWAP(e[index], e[max_count - back_count]);	
            //return back_pop();	
        //}
	//}
    
    TYPE *front_remove(s32 index)
    {
        assert(index >= 0 && index < front_count);
        SWAP(e[index], e[front_count - 1]);	 
        return front_pop();	
    }
    
    TYPE *back_remove(s32 index)
    {
        assert(index >= (max_count - back_count)  &&  index < max_count);
        SWAP(e[index], e[max_count - back_count]);	
        return back_pop();		
    }
    
    
    TYPE &operator[](s32 index)
    {
        assert((index >= 0 && index < front_count) ||
            (index >= (max_count - back_count) && index < max_count));
        return e[index];    
    }
    TYPE *operator+(s32 index)
    {
        assert((index >= 0 && index < front_count) ||
            (index >= (max_count - back_count) && index < max_count));
        return e + index;    
    }
};


template<typename TYPE, typename LAMBDA>
internal void
insertion_sort(TYPE *array, s32 array_count, LAMBDA compare)
{
    for(s32 index = 0; index < array_count; index += 1)
    {
        TYPE *elem = array + index;
        TYPE *prev = elem - 1;
        if (prev < array)	prev = 0;
        
        while(prev)
        {
            if (compare(prev, elem) > 0)
            {
                SWAP(*prev, *elem);
                elem = prev;
                prev = elem - 1;
                if (prev < array)	prev = 0;
            }
            else break;
        }
    }
}



internal void
copy_memory(void *destination, void *source, uintptr size)
{
    //TODO intrinsic for this?
    u8 *dst = (u8 *)destination;
    u8 *src = (u8 *)source;
    assert(!ptr_in_base_size(src,        dst, size)); //ensure memory regions do not overlap
    assert(!ptr_in_base_size(src+size-1, dst, size)); //call move_memory() if they do overlap....
    for(uintptr i = 0; i < size; i += 1) 
        dst[i] = src[i];
}

internal void
move_memory(void *destination, void *source, uintptr size)
{
    u8 *dst = (u8 *)destination;
    u8 *src = (u8 *)source;
    if (dst < src)
    {
        // Copy forwards
        for(uintptr i = 0; i < size; i += 1) 
            dst[i] = src[i];    
    }
    else if (dst > src)
    {
        // Copy backwards
        for(uintptr i = size; (i--) > 0;	)
            dst[i] = src[i];
    }
    //else no point in copying...
}

internal void
zero_memory(void *data, uintptr size)
{
    //TODO intrinsic for this?
    u8 *at = (u8 *)data;
    for(uintptr i = 0; i < size; i += 1) at[i] = 0;
}

internal nil
is_memory_equal(void *ap, void *bp, uintptr size)
{
    u8 *a = (u8 *)ap;
    u8 *b = (u8 *)bp;
    for(uintptr i = 0; i < size; i += 1)
        if (a[i] != b[i])	return false;
    return true;
}

enum Month : u32
{
    JANUARY, FEBUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER,
};

struct Date_Time
{
    Month month;
    s32 day;
    s32 hour;
    s32 min;
    s32 sec;
    s32 year;
};



//// memory arenas

/* Some notes on memory management in operating systems

	Every process has it's own virtual memory space that maps into RAM. 
	This means (void *)0x80000 actually refers to different RAM region on different processes. This allows
	-privacy since each process touches it's own memory
	-reliability since a process can't corrupt another process's memory

	Mapping vram to ram is done in 4KB chunks called *pages*
	AMD64 cpus use 48bit addressing meaning we can represent up to 256TB of virtual memory, which is a lot of pages!!!
	Since we need to store mappings, this is done in some sparse structure

	*Commiting* memory basically 'reserves' physical pages. It increases some counter of phsycial pages used and the commit will
	fail if there aren't any more pages...
	*Commiting* != *allocating* memory!!! It just means when we allocate later on, we are gurannted to get memory back 
	since the commit check passed before

	NOTE all physical pages returned from the operating system are required to be zeroed out!!! Super nice!!!
*/

//just a simple linear allocator that can push and pop. We don't record allocations... 
//calls to pop() are expected to pass correct prev alloc size
struct Memory_Arena 
{
    u8 *data;
    u32 size;
    u32 used;
    u32 used_on_scratch_plus_one;
};

internal Memory_Arena 
make_memory_arena(void *data, u32 size)
{
    assert(data && size);
    Memory_Arena arena = {};
    arena.data = (u8 *)data;
    arena.size = size;
    return arena;
}



#define push_size(arena, size, ...)                    push_size_internal(arena, size,      __VA_ARGS__)
#define push_struct(arena,  type, ...)         (type *)push_size(arena, sizeof(type),       __VA_ARGS__)
#define push_array(arena, type, count, ...)    (type *)push_size(arena, sizeof(type)*count, __VA_ARGS__)
#define push_slice(arena, type, max_count, ...)        make_slice(push_array(arena, type, count, __VA_ARGS__), max_count) 

#define ZERO_MEMORY (1 << 0)
#define ALIGN2      (1 << 1)
#define ALIGN4      (1 << 2)
#define ALIGN8      (1 << 3)


#define Align__(n)  ALIGN##n
#define Align(n)    Align__(n)

#define ALIGNED_POINTER_UP(ptr, n)    (((uintptr)(ptr) + ((n)-1)) & ~((n)-1)) //if not aligned, get next higher aligned address
#define ALIGNED_POINTER_DOWN(ptr, n)  (((uintptr)(ptr) +       0) & ~((n)-1)) //if not aligned, get prev lower  aligned address
//#define ALIGNED_POINTER_PREV(ptr, n)    (((uintptr)(ptr) - ((n)-1)) & ~((n)-1))

//#define ALIGNED_POINTER2(ptr) (((uintptr)(ptr) + 1) & ~1)
//#define ALIGNED_POINTER4(ptr) (((uintptr)(ptr) + 3) & ~3)
//#define ALIGNED_POINTER8(ptr) (((uintptr)(ptr) + 7) & ~7)

internal nil 
is_aligned(void *ptr, uintptr alignment)
{
    assert((alignment & (alignment - 1)) == 0);
    return (ALIGNED_POINTER_UP(ptr, alignment) - (uintptr)ptr) == 0;
}

internal nil
is_next_allocation_aligned(Memory_Arena *arena, uintptr alignment)
{
    assert((alignment & (alignment - 1)) == 0);
    uintptr ptr = (uintptr)(arena->data + arena->used);
    return (ALIGNED_POINTER_UP(ptr, alignment) - ptr) == 0;
}

internal void * 
align_next_allocation(Memory_Arena *arena, uintptr alignment)
{
    assert((alignment & (alignment - 1)) == 0);
    
    uintptr ptr = (uintptr)(arena->data + arena->used);
    u32 offset = (u32)(ALIGNED_POINTER_UP(ptr, alignment) - ptr);
    
    if ((arena->size - arena->used) >= offset)
    {
        arena->used += offset;
        return (void *)(ptr + offset);    
    }
    else return null;
}

internal nil
did_allocate_scratch_block(Memory_Arena *arena)
{ return arena->used_on_scratch_plus_one > 0; }

internal void *
push_size_internal(Memory_Arena *arena, u32 size, u32 flags = 0)
{
    assert(size > 0);
    assert(arena->data); //never set this arena up, dingus
    assert(!did_allocate_scratch_block(arena)); //can't do allocations if we have active scratch blocks
    assert(arena->size && (size <= arena->size));
    
    uintptr result = (uintptr)(arena->data + arena->used);
    if      (flags & ALIGN2) result = (uintptr)align_next_allocation(arena, 2);
    else if (flags & ALIGN4) result = (uintptr)align_next_allocation(arena, 4);
    else if (flags & ALIGN8) result = (uintptr)align_next_allocation(arena, 8);
    
    if (result && (arena->size - arena->used) >= size)
    {
        arena->used += size;
        if (flags & ZERO_MEMORY)	zero_memory((void *)result, size);
    }
    else
    {
        result = 0;
        assert(0); //TODO handle this
    }
    return (void *)result;
}



//NOTE if size=0, just eat up rest of arena
internal Memory_Arena
push_memory_arena(Memory_Arena *arena, u32 size, u32 flags = 0)
{
    //TODO we have to do this here unless we want push_size to return the size it was 
    //able to push on and make the return type more complex.
    if      (flags & ALIGN2) align_next_allocation(arena, 2);
    else if (flags & ALIGN4) align_next_allocation(arena, 4);
    else if (flags & ALIGN8) align_next_allocation(arena, 8);
    flags &= ~(ALIGN2|ALIGN4|ALIGN8); //just to not do the alignment again
    
    if (size == 0) size = arena->size - arena->used;
    Memory_Arena result = make_memory_arena(push_size(arena, size, flags), size);
    return result;
}


internal void
clear(Memory_Arena *arena)
{
    assert(arena->size && arena->used < arena->size);
    if (arena->used) 
        zero_memory(arena->data, arena->used);
    arena->used = 0;
}

#define front_push_size(pushbuffer, size, ...)                    front_push_internal(pushbuffer, size,      __VA_ARGS__)
#define front_push_struct(pushbuffer,  type, ...)         (type *)front_push_size(pushbuffer, sizeof(type),       __VA_ARGS__)
#define front_push_array(pushbuffer, type, count, ...)    (type *)front_push_size(pushbuffer, sizeof(type)*count, __VA_ARGS__)

#define back_push_size(pushbuffer, size, ...)                    back_push_internal(pushbuffer, size,      __VA_ARGS__)
#define back_push_struct(pushbuffer,  type, ...)         (type *)back_push_size(pushbuffer, sizeof(type),       __VA_ARGS__)
#define back_push_array(pushbuffer, type, count, ...)    (type *)back_push_size(pushbuffer, sizeof(type)*count, __VA_ARGS__)

struct Front_Back_Push_Buffer
{
    u8 *data;
    u32 datasize;
    u32 front_used;
    u32 back_used;
};

internal u32
get_remaining_size(Front_Back_Push_Buffer *pushbuffer)
{
    u32 size = pushbuffer->datasize;
    u32 used = pushbuffer->front_used + pushbuffer->back_used;
    return (size >= used) ? (size - used) : 0;
}

internal void * 
front_align_next_allocation(Front_Back_Push_Buffer *pushbuffer, uintptr alignment)
{
    u32 size_left = get_remaining_size(pushbuffer);
    if (size_left == 0) return null;
    
    assert((alignment & (alignment - 1)) == 0);
    
    uintptr ptr = (uintptr)(pushbuffer->data + pushbuffer->front_used);
    u32 offset = (u32)(ALIGNED_POINTER_UP(ptr, alignment) - ptr);
    
    if (size_left >= offset)
    {
        pushbuffer->front_used += offset;
        return (void *)(ptr + offset);    
    }
    else return null;
}

internal void *
front_push_internal(Front_Back_Push_Buffer *pushbuffer, u32 size, u32 flags = 0)
{
    assert(pushbuffer->data);
    uintptr result = (uintptr)(pushbuffer->data + pushbuffer->front_used);
    if (flags & ALIGN2) result = (uintptr)front_align_next_allocation(pushbuffer, 2);
    if (flags & ALIGN4) result = (uintptr)front_align_next_allocation(pushbuffer, 4);
    if (flags & ALIGN8) result = (uintptr)front_align_next_allocation(pushbuffer, 8);
    
    if (result && get_remaining_size(pushbuffer) >= size)
    {
        pushbuffer->front_used += size;    
        if (flags & ZERO_MEMORY) zero_memory((void *)result, size);
        return (void *)result;    
    }
    else assert(0); //TODO handle this
}

internal void * 
back_align_next_allocation(Front_Back_Push_Buffer *pushbuffer, uintptr alignment)
{
    u32 size_left = get_remaining_size(pushbuffer);
    if (size_left == 0) return null;
    
    assert((alignment & (alignment - 1)) == 0);
    
    uintptr ptr = (uintptr)(pushbuffer->data + pushbuffer->datasize - pushbuffer->back_used - 1);
    u32 offset = (u32)(ptr - ALIGNED_POINTER_DOWN(ptr, alignment));
    
    if (size_left >= offset)
    {
        pushbuffer->back_used += offset;
        return (void *)(ptr + offset);    
    }
    else return null;
}

internal void *
back_push_internal(Front_Back_Push_Buffer *pushbuffer, u32 size, u32 flags = 0)
{
    assert(pushbuffer->data);
    uintptr result = (uintptr)(pushbuffer->data + pushbuffer->datasize - pushbuffer->back_used - 1);
    if (flags & ALIGN2) result = (uintptr)back_align_next_allocation(pushbuffer, 2);
    if (flags & ALIGN4) result = (uintptr)back_align_next_allocation(pushbuffer, 4);
    if (flags & ALIGN8) result = (uintptr)back_align_next_allocation(pushbuffer, 8);
    
    if (result && get_remaining_size(pushbuffer) >= size)
    {
        pushbuffer->back_used += size;    
        if (flags & ZERO_MEMORY) zero_memory((void *)result, size);
        return (void *)result;    
    }
    else assert(0); //TODO handle this
}

struct Scratch_Block 
{
    Memory_Arena *parent;
    Memory_Arena  arena;
    //TODO this wasts some memory due to alignment requests. 
    //We could just store old_used in parent_arena to reset it
    //if we only want one scratch block per arena. But we could also 
    //do the recursion thing like 
    
    Scratch_Block(Memory_Arena *parent_arena, u32 size = 0, u32 flags = 0)
    {
        parent = parent_arena;
        u32 parent_used = parent->used;
        
        arena  = push_memory_arena(parent_arena, size, flags); 
        parent->used_on_scratch_plus_one = parent_used + 1;
    }
    
    //NOTE destructors get called in reverse order as the objects were created
    //meaning there's on problem with making multiple scratch blocks with same arena
    //and there's no issue when making a scratch block from a scratch block, ...it's just an arena
    ~Scratch_Block()
    {
        //TODO do some more checking in here I suppose
        assert(parent->used_on_scratch_plus_one > 0);
        parent->used = parent->used_on_scratch_plus_one - 1;
        parent->used_on_scratch_plus_one = 0;
        
        
        //TODO get rid of this zero_memory because I want the user
        //to manually zero it out when memory is retrieved so there is never
        //any confusion ....
        //zero_memory(this->arena.data, this->arena.used);
    }
    
    operator Memory_Arena*() { return &this->arena; }
};


struct Vector2u
{
    u32 x, y;
};

struct Vector2i
{
    s32 x, y;
};

internal Vector2i
v2i(s32 x, s32 y)
{
    Vector2i v;
    v.x = x;
    v.y = y;
    return v;
}

static Vector2i
operator+(Vector2i a, Vector2i b)   { return v2i(a.x + b.x, a.y + b.y); }

static Vector2i
operator-(Vector2i a, Vector2i b)   { return v2i(a.x - b.x, a.y - b.y); }

static void
operator+=(Vector2i &a, Vector2i b) { a = a + b; }

static Vector2i
operator*(Vector2i a, s32 scale)    { return v2i(a.x * scale, a.y * scale); }

static Vector2i
operator*(s32 scale, Vector2i a)    { return v2i(a.x * scale, a.y * scale); }

static Vector2i
operator*(Vector2i a, Vector2i b)   { return v2i(a.x * b.x, a.y * b.y); }

static bool
operator==(Vector2i a, Vector2i b)  { return (a.x == b.x) && (a.y == b.y); }

static bool
operator!=(Vector2i a, Vector2i b)  { return !(a == b); }

enum Direction8 {
    NORTH,     //0
    NORTHEAST, //1
    EAST,      //2
    SOUTHEAST, //3
    SOUTH,     //4
    SOUTHWEST, //5
    WEST,      //6
    NORTHWEST, //7
    NO_DIRECTION8,
};

internal Direction8
flip(Direction8 dir)
{
	switch(dir)
	{
    case NORTH: return SOUTH;
    case EAST:  return WEST;
    case SOUTH: return NORTH;
    case WEST:  return EAST;
    case NORTHWEST:  return SOUTHEAST;
    case NORTHEAST:  return SOUTHWEST;
    case SOUTHWEST:  return NORTHEAST;
    case SOUTHEAST:  return NORTHWEST;
    default:    assert(0); return NO_DIRECTION8;
    }
}

internal Direction8
rotate_90ccw(Direction8 dir)
{
	switch(dir)
	{
    case NORTH: return WEST;
    case EAST:  return NORTH;
    case SOUTH: return EAST;
    case WEST:  return SOUTH;
    case NORTHEAST:  return NORTHWEST;
    case NORTHWEST:  return SOUTHWEST;
    case SOUTHWEST:  return SOUTHEAST;
    case SOUTHEAST:  return NORTHEAST;
    default:    assert(0); return NO_DIRECTION8;
    }
}

internal Direction8
rotate_90cw(Direction8 dir)
{
	switch(dir)
    {
    case NORTH: return EAST;
    case EAST:  return SOUTH;
    case SOUTH: return WEST;
    case WEST:  return NORTH;
    case NORTHEAST:  return SOUTHEAST;
    case SOUTHEAST:  return SOUTHWEST;
    case SOUTHWEST: return NORTHWEST;
    case NORTHWEST:  return NORTHEAST; 
    default:    assert(0); return NO_DIRECTION8;
    }
}


#endif //UBIQUITOUS_H



