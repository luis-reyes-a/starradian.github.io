#ifndef STRINGS_H //created on September 15th 2021 
#define STRINGS_H

#define litargs(lit)     lit, sizeof(lit)-1
#define strargs(string) (string).str, (string).length
#define strlit(lit)    mkstr(lit, sizeof(lit)-1, sizeof(lit))

//#include "sal.h"
//#undef _USE_ATTRIBUTES_FOR_SAL
//#define _USE_ATTRIBUTES_FOR_SAL 1
//#define PARAM_FORMAT_STRING _Printf_format_string_

// NOTE all calls to duplicate null terminate always
// all strings *should* be null termianted. Most function calls here will fire assert if it's not the case

#define LOCAL_STRING(name, buffer_size) char buffer_for_ ## name[buffer_size]; String name = mkstr_from_buffer(buffer_for_ ## name)

struct String
{
    char *str;
    //DO NOT REARRANGE THESE
    s32 length; 
    s32 size;
    
    char &operator[](s32 index)
    {
        assert(index >= 0 && index < length);
        assert(size >= length);
        return str[index]; 
    }
};

#pragma warning(push)          
#pragma warning(disable: 4455) 
String operator""s(const char *str, size_t length)
{
    assert (length <= S32_MAX);
    String string;
    string.str = (char *)str;
    string.length = (s32)length;
    string.size = (s32)length + 1;
    return string;
}
#pragma warning(pop) 

//NOTE this doesn't check if string length is correct or not
internal nil
is_valid(String string)
{
    nil result = string.str != null;
    assert(result? (string.length >= 0 && string.size >= string.length) :true);
    return result;
}

//TODO intrinsic for this?
internal s32 
strlen(char *string)
{
    s32 length = 0;
    while(string[length])	length += 1;
    return length;
}

internal String
mkstr(char *str, s32 length, s32 size)
{
    assert(size >= length);
    String string;
    string.str = str;
    string.length = length;
    string.size   = size;
    return string;
}

internal String
mkstr(char *str)
{
    s32 length = strlen(str);
    return mkstr(str, length, length + 1);
}

template <u32 SIZE>
internal String
mkstr_from_buffer(char (&buffer)[SIZE], s32 length = 0) 
{
    String string = mkstr(buffer, length, SIZE);
    return string;
}

//NOTE this doesn't check if string length is correct or not
internal nil
is_null_terminated(String string)
{
    nil should_be_null_terminated = (string.size > string.length);
    assert(should_be_null_terminated? (string.str[string.length] == 0) :true);
    return should_be_null_terminated;
}

internal nil
strcompare()
{
    
}

//NOTE this returns true if string points to the exact same substr_length
//chars found in substr, this does not care about main strings null character
internal nil
strsubmatch(char *string, char *substr, s32 substr_length)
{
    for(s32 i = 0; i < substr_length; i += 1)
    {
        if      (!substr[i])              return true;  // NOTE string2 has to be checked first in case 
        else if (!string[i])	          return false; // where they are both the same length...
        else if (string[i] != substr[i]) return false;
    }
    return true;
}

internal nil
strsubmatch(String str, String query)
{
    if (str.length >= query.length)
         return strsubmatch(str.str, query.str, query.length);
    else return false;
}

internal nil
strmatch(char *string1, s32 length1, char *string2, s32 length2)
{
    nil match = false;
    if (length1 == length2)
    {
        match = true; 
        for(s32 i = 0; i < length1; i += 1)
        {
            assert(string1[i]); //length1 incorrect
            assert(string2[i]); //length2 incorrect 
            if (string1[i] != string2[i])
            {
                match = false;
                break;
            }
        }
    }
    return match;
}

//TODO we can speed this up by not calling strlen and isntead just iterating
//both strings until hitting a terminator and checking other has terminator as well...
internal nil
strmatch(char *string1, char *string2)
{ return strmatch(string1, strlen(string1), string2, strlen(string2)); }

internal nil
strmatch(String a, String b)
{ return strmatch(a.str, a.length, b.str, b.length); }

template <s32 SIZE> 
internal nil strmatch(String str, const char (&buffer)[SIZE])
{ return strmatch(str.str, str.length, (char *)buffer, SIZE-1); }


internal String
advance(String string, s32 amount)
{
    s32 length = string.length - amount;
    if (length <= 0) return {};
    else return mkstr(string.str + amount, length, string.size - amount);
}

internal void
advance(String *string, s32 amount)
{	*string = advance(*string, amount);	}

global String MONTH_STRINGS[12] = {strlit("January"), strlit("Febuary"), strlit("March"), strlit("April"), strlit("May"), 
    strlit("June"), strlit("July"), strlit("August"), strlit("Septemeber"), strlit("October"), strlit("November"), strlit("December")};

internal String
get_month_string(s32 month)
{
    assert(month >= JANUARY && month <= DECEMBER);
    return MONTH_STRINGS[month];
}



template<typename T> internal nil 
is_ascii(T c)
{ 	return(c >= 0 && c <= 127);	}

template<typename T> internal nil 
is_digit(T c)
{ 	return c >= '0' && c <= '9';	}

template<typename T> internal nil
is_uppercase(T c)
{	return c >= 'A' && c <= 'Z';	}

template<typename T> internal nil
is_lowercase(T c)
{	return c >= 'a' && c <= 'z';	}

template<typename T> internal nil
is_alpha(T c)
{	return is_lowercase(c) || is_uppercase(c);	}

template<typename T> internal nil
is_alphanumeric(T c)
{	return is_alpha(c) || is_digit(c);	}

template<typename T> internal char
to_uppercase(T c)
{	return is_lowercase(c) ? (c + ('A' - 'a')) : c;	}

template<typename T> internal char
to_lowercase(T c)
{	return is_uppercase(c) ? (c + ('a' - 'A')) : c;	}


internal String
make_uppercase(String string)
{
    for_index_inc(s32, i, string.length)
        string.str[i] = to_uppercase(string.str[i]);
    return string;
}


internal String
make_lowercase(String string)
{
    for_index_inc(s32, i, string.length)
        string.str[i] = to_lowercase(string.str[i]);
    return string;
}

//TODO is there a simd instruction or a non looped version of this?
internal u32 
pow10_u32(s32 exponent)
{
    assert(exponent >= 0);
    u32 value = 1;
    while(exponent--)
    {
        u32 new_value = value * 10;
        assert((new_value / 10) == value); //overflow check
        value = new_value;
    }
    assert(value != 0); //something really bad happened, (overflow but check failed...?)
    return value;
}

//TODO is there a simd instruction or a non looped version of this?
internal u64
pow10_u64(s32 exponent)
{
    assert(exponent >= 0);
    u64 value = 1;
    while(exponent--)
    {
        u64 new_value = value * 10;
        assert((new_value / 10) == value); //overflow check
        value = new_value;
    }
    assert(value != 0); //something really bad happened, (overflow but check failed...?)
    return value;
}

/*internal String
s32_to_string(char *buffer, s32 buffer_size, s32 value)
{
    
} */

internal u32
string_to_u32(char *string, s32 length)
{
    assert(length > 0);
    u32 value = 0;
    for(s32 i = 0; i < length; i += 1)
    {
        assert(is_digit(string[i]));
        
        u32 new_value = value * 10;
        assert((new_value / 10) == value); //overflow check
        assert((U32_MAX - new_value) >= (u32)(string[i] - '0'));
        new_value += (string[i] - '0');
        
        value = new_value;
    }
    return value;
}

internal u32
string_to_u32(String string)
{ return string_to_u32(string.str, string.length); }

internal u64
string_to_u64(char *string, s32 length)
{
    assert(length > 0);
    u64 value = 0;
    for(s32 i = 0; i < length; i += 1)
    {
        assert(is_digit(string[i]));
        
        u64 new_value = value * 10;
        assert((new_value / 10) == value); //overflow check
        assert((U64_MAX - new_value) >= (u64)(string[i] - '0'));
        new_value += (string[i] - '0');
        
        value = new_value;
    }
    return value;
}

internal s32
string_to_s32(char *string, s32 length)
{
    assert(length > 0);
    nil is_negative = false;
    if (string[0] == '-')
    {
        is_negative = true;
        string += 1;
        length -= 1;
    }
    
    s32 result;
    u32 u32_val = string_to_u32(string, length);
    if (is_negative)
    {
        assert(u32_val <= (u32)-S32_MIN);
        result = -(s32)u32_val;
    }
    else
    {
        assert(u32_val <= (u32)S32_MAX);
        result = (s32)u32_val;
    }
    return result;
}

internal s64
string_to_s64(char *string, s32 length)
{
    assert(length > 0);
    nil is_negative = false;
    if (string[0] == '-')
    {
        is_negative = true;
        string += 1;
        length -= 1;
    }
    
    s64 result;
    u64 u64_val = string_to_u64(string, length);
    if (is_negative)
    {
        assert(u64_val <= (u64)-S64_MIN);
        result = -(s64)u64_val;
    }
    else
    {
        assert(u64_val <= (u64)S64_MAX);
        result = (s64)u64_val;
    }
    
    return result;
}

//TODO how do we want to guard against overflow errors when 
//user passes in a very large string like 3.1423444444444444444444444444444444444
//for the decimal part we could just truncate the length
//but what about for the whole value part? ....
internal f32
string_to_f32(char *string, s32 length)
{
    assert(length > 0);
    f32 sign = 1.0f;
    if (string[0] == '-')
    {
        sign = -1.0f;
        string += 1;
        length -= 1;
    }
    
    s32 decimal_index = length;
    for(s32 i = 0; i < length; i += 1)
    {
        assert(is_digit(string[i]) || string[i] == '.');
        if (string[i] == '.')	
        { 
            decimal_index = i;
            break;
        }
    }
    
    f32 result = (f32)string_to_u32(string, decimal_index);
    if (decimal_index != length)
    {
        s32 fractional_length = length - (decimal_index + 1);
        if (fractional_length > 0)
        {
            //TODO clamp fractional_length here to some reasonable number....
            f32 fractional_value = (f32)string_to_u64(string + decimal_index + 1, fractional_length);
            f32 fractional_power = (f32)pow10_u64(fractional_length);
            assert(fractional_power != 0);
            result += (fractional_value / fractional_power);
        }
    }
    return sign*result;
}

internal s32
find_first_char_offset(String string, char c)
{
    for_index_inc(s32, i, string.length)
        if (string.str[i] == c)	return i;
    return -1;
}

internal s32
find_first_substring_offset(String string, String substring)
{
    for_index_inc(s32, i, string.length)
    {
        s32 length = string.length - i;
        if (length < substring.length) break;
        
        if (strmatch(string.str + i, substring.length, substring.str, substring.length))
            return i;
    }
    return -1;
}

 

internal void
null_terminate(String string)
{
    assert(string.str);
    assert(string.length < string.size);
    string.str[string.length] = 0;
}

internal String //gets rid of .whatever
remove_file_extension(String string)
{ 
    for(s32 i = string.length - 1; i >= 0; i -= 1)
    {
        if (string[i] == '.') 
        {
            string.length = i;
            break;
        }
    }
    return string;
}

internal String //gets rid of C:/path/.../
remove_file_path(String string)
{
    for_index_dec(s32, i, string.length)
    {
        if (string[i] == '/' || string[i] == '\\')
            return advance(string, i + 1);        
    } 
    return string;
}

internal String //C:/dir1/file.txt -> C:/dir1/ -> C:/ -> C:/
remove_current_file_directory(String string)
{
    //NOTE -2 to handle case where we end in directory backslash
    for (s32 i = string.length - 2; i >= 0; i -= 1)
    {
        if (string[i] == '/' || string[i] == '\\') 
        {
            string.length = i + 1;
            break;
        }
    }
    return string;
}

internal void
remove_current_file_directory(String *string)
{ *string = remove_current_file_directory(*string); }

internal String
get_file_extension(String string)
{
    for(s32 at = string.length - 1; at >= 0; at -= 1)
    {
        if (string[at] == '.')
        {
            String result;
            result.str = string.str + at;
            result.length = string.length - at;
            return result;
        }
    }
    return {};
}

internal void
append(String *string, String to_append)
{
    s32 left = string->size - string->length;
    if (left > to_append.length) // greater than for null termaintor
    {
        for_index_inc(s32, i, to_append.length)
            string->str[string->length + i] = to_append[i];
        string->length += to_append.length;
        string->str[string->length] = 0;
    }
}

internal void
append_valist(String *string, char *fmt, va_list list)
{
    assert(string->str);
    char *at = string->str + string->length; 
    assert(ptr_in_base_count(at, string->str, string->size));
    
    s32 amt_left  = string->size - string->length; 
    assert(amt_left > 0);
    s32 amt_added = stbsp_vsnprintf(at, amt_left, fmt, list);
    string->length += amt_added;
}

internal void
appendf(String *string, char *fmt, ...)
{   
    va_list list;
    va_start(list, fmt);
    append_valist(string, fmt, list);
    va_end(list);
}

//TODO redundant after making appendf()
//internal void
//format(String *str, char *fmt, ...)
//{
    //assert(is_valid(*str));
    //va_list list;
    //va_start(list, fmt);    
    //str->length = stbsp_vsnprintf(str->str, str->size, fmt, list);
    //va_end(list);
//}


#define LOCAL_STRING_BUILDER(name, buffer_size) char buffer_for_ ## name[buffer_size]; String_Builder name = make_string_builder(buffer_for_ ## name, (buffer_size))

struct String_Builder 
{
    char *buffer;
    u32 size;
    u32 current_string_offset;
    u32 current_string_length;
};

internal String_Builder
make_string_builder(char *buffer, s32 buffer_size)
{
    assert(buffer && buffer_size > 1);
    String_Builder builder = {};
    builder.buffer = buffer;
    builder.size = buffer_size;
    return builder;
}

internal void
append(String_Builder *builder, char *str, s32 length)
{
    char *at = builder->buffer + builder->current_string_offset + builder->current_string_length;
    assert(ptr_in_base_count(at, builder->buffer, builder->size));
    
    s32 amt_added = 0;
    s32 amt_left  = (s32)(builder->buffer + builder->size - at - 1); //-1 for null terminator
    assert(amt_left > 0);
    
    while(amt_added < amt_left && amt_added < length)
    {
        at[amt_added] = str[amt_added];
        assert(at[amt_added]); //length is incorrect
        amt_added += 1;
    }
    builder->current_string_length += amt_added;
}

internal void
append(String_Builder *builder, char *str) 
{   append(builder, str, strlen(str));   }

internal void
append(String_Builder *builder, String string) 
{   append(builder, string.str, string.length);   }

internal void
append_valist(String_Builder *builder, char *fmt, va_list list)
{
    char *at = builder->buffer + builder->current_string_offset + builder->current_string_length;
    assert(ptr_in_base_count(at, builder->buffer, builder->size));
    
    s32 amt_left  = (s32)((builder->buffer + builder->size) - at); //no -1, because  snprintf null terminates always 
    assert(amt_left > 0);
    s32 amt_added = stbsp_vsnprintf(at, amt_left, fmt, list); 
    builder->current_string_length += amt_added;
}



internal void
appendf(String_Builder *builder, char *fmt, ...)
{   
    va_list list;
    va_start(list, fmt);
    append_valist(builder, fmt, list);
    va_end(list);
}

internal void
remove_trailing_whitespace(String_Builder *builder)
{
    assert(builder->current_string_length > 0);
    u32 index = builder->current_string_offset + builder->current_string_length - 1;
    for(;;)
    {
        if (builder->buffer[index] == ' '  || builder->buffer[index] == '\t' ||
            builder->buffer[index] == '\n' || builder->buffer[index] == '\r')
        {
            builder->buffer[index] = 0;
            assert(builder->current_string_length > 0);
            builder->current_string_length -= 1;
        }
        else break;
        
        if (index == builder->current_string_offset)	break;
    }
}

//NOTE final call to mkstr null terminates the current string and returns it
internal String
mkstr(String_Builder *builder)
{
    //ensure we have room to null terminate
    assert((builder->current_string_offset + builder->current_string_length) < builder->size);
    String result = mkstr(builder->buffer + builder->current_string_offset,
        builder->current_string_length,
        builder->current_string_length + 1);
    result.str[result.length] = 0; 
    
    builder->current_string_offset += builder->current_string_length + 1;
    builder->current_string_length = 0;
    return result;
}

//this is basically appendf, mkstr(), we have this we can inline it as function argument...
internal String
push_stringf(String_Builder *builder, char *fmt, ...)
{
    assert(builder->current_string_length == 0);
    va_list list;
    va_start(list, fmt);
    append_valist(builder, fmt, list);
    va_end(list);
    return mkstr(builder);
}

internal String
duplicate(String_Builder *builder, String str)
{
    assert(builder->current_string_length == 0);
    append(builder, str);
    return mkstr(builder);
}

internal String
duplicate(String_Builder *builder, char *str, s32 length)
{
    assert(builder->current_string_length == 0);
    append(builder, str, length);
    return mkstr(builder);
}




internal void
duplicate(String *str, String tocopy)
{
    str->length = 0;
    if (str->size > tocopy.length)
    {
        for_index_inc(s32, i, tocopy.length)
            str->str[i] = tocopy.str[i];
        str->length = tocopy.length;
        str->str[str->length] = 0;
    }   
}


internal String_Builder
push_string_builder(Memory_Arena *arena, s32 buffer_size)
{
    char *buffer = push_array(arena, char, buffer_size);
    return make_string_builder(buffer, buffer_size);
}



// NOTE we allow size to be passed in here in case we want to allocate with extra space for some reason
// it's kind of silly because I never do that...
internal String
duplicate(Memory_Arena *arena, char *str, s32 length)
{
    assert(length > 0);
    
    String result = {};
    result.str = push_array(arena, char, length + 1);
    if (result.str)
    {
        result.length = length;
        result.size   = length + 1;
        
        for(s32 i = 0; i < result.length; i += 1)
        {
            assert(str[i]); //length of str was incorrect
            result.str[i] = str[i];
        }
        result.str[result.length] = 0; //null terminate
    }
    return result;
}

internal String
duplicate(Memory_Arena *arena, String string)
{	return duplicate(arena, string.str, string.length);	}

//always null terminated
internal String
push_stringf(Memory_Arena *arena, char *fmt, ...)
{
    assert(arena->used < arena->size);
    assert(!did_allocate_scratch_block(arena)); //can't do allocations if we have active scratch blocks
    
    va_list list;
    va_start(list, fmt);
    char *buffer = (char *)(arena->data + arena->used);
    u32 buffer_size   = arena->size - arena->used;
    u32 result_length = stbsp_vsnprintf(buffer, buffer_size, fmt, list);
    va_end(list);
    
    String result = mkstr(buffer, result_length, result_length + 1);
    arena->used += result.size;
    
    assert(arena->used <= arena->size);
    return result;
}


//TODO better place for this

//TODO manually create this
//global String DIR4_STRINGS[] = {
    //strlit("NORTH"), 
    //strlit("EAST"), 
    //strlit("SOUTH"), 
    //strlit("WEST"), 
    //strlit("NO_DIRECTION")
//};

internal String
get_string(Direction8 dir)
{
    switch(dir)
    {
    case NORTH: return "NORTH"s;
    case EAST:  return "EAST"s;
    case SOUTH: return "SOUTH"s;
    case WEST:  return "WEST"s;
    case NORTHEAST: return "NORTHEAST"s;
    case NORTHWEST: return "NORTHWEST"s;
    case SOUTHEAST: return "SOUTHEAST"s;
    case SOUTHWEST: return "SOUTHWEST"s;
    default:    return strlit("NO_DIRECTION8");
    };
}


//internal f32        get_angle_0_to_tau(f32 angle);
//internal Direction8 to_direction8(f32 angle);
//TODO get rid of this, currently metaprogram is calling get_string(e->facing_dir)
//internal String
//get_string(f32 angle)
//{
    //Direction8 dir = to_direction8(angle);
    //return get_string(dir);
//}

 

/*
internal String
get_direction_string(Direction8 dir)
{
    switch(dir)
    {
    case NORTH: return strlit("NORTH");
    case EAST:  return strlit("EAST");
    case SOUTH: return strlit("SOUTH");
    case WEST:  return strlit("WEST");
    case NO_DIRECTION: return strlit("NO DIRECTION");
    default: return strlit("INVALID Direction8 value");
    }
} */






#endif //STRINGS_H
