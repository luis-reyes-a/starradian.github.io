#ifndef WIN32_H //created on January 17th 2022 
#define WIN32_H

#include "windows.h" 

struct File_Description 
{
    String filename;
    u64 filesize;
    nil is_folder;
};

struct File_Iterator 
{
    WIN32_FIND_DATA ffd;
    HANDLE handle;
    
    File_Description desc;
    File_Description *ptr;
    
    CONSTRUCTOR(File_Iterator)(String filepath) //NOTE for everything just do foler/* 
	{
        char search_wildcard[MAX_PATH];
        assert(countof(search_wildcard) > (filepath.length+1)); //NOTE plus 1 for *
        //NOTE empty string means iterate current directory...
        if (filepath.length > 0) copy_memory(search_wildcard, filepath.str, filepath.length);
        search_wildcard[filepath.length]   = '*';
        search_wildcard[filepath.length+1] = 0;
        
	    handle = FindFirstFileA(search_wildcard, &ffd);
	    ptr = 0;
	    if(handle != INVALID_HANDLE_VALUE) 
	    {
            desc.is_folder = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
            desc.filename = mkstr(ffd.cFileName);
            
            LARGE_INTEGER bignum;
            bignum.LowPart = ffd.nFileSizeLow;
            bignum.HighPart = ffd.nFileSizeHigh;
            desc.filesize = bignum.QuadPart;
            
            ptr = &desc;
        }
    }
    
    DESTRUCTOR(File_Iterator)() 
	{ FindClose(handle); }
    
    
        
    nil get_next()
	{
	    if( FindNextFileA(handle, &ffd) )
	    {
            desc.is_folder = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
            desc.filename = mkstr(ffd.cFileName);
            
            LARGE_INTEGER bignum;
            bignum.LowPart = ffd.nFileSizeLow;
            bignum.HighPart = ffd.nFileSizeHigh;
            desc.filesize = bignum.QuadPart;
            return true;
        }
	    else
	    {
            desc = {};
            ptr = null;
            return false;
        }
    }
    
    nil valid() { return ptr != null; }  
};


internal Date_Time
get_date_time()
{
    SYSTEMTIME systime;
    GetLocalTime(&systime);
    Date_Time date_time = {};
    date_time.month = (Month)(systime.wMonth - 1);
    date_time.day   = systime.wDay;
    date_time.hour  = systime.wHour;
    date_time.min   = systime.wMinute;
    date_time.sec   = systime.wSecond;// + ((real32)systime.wMilliseconds) / 1000.0f;
    date_time.year   = systime.wYear;// + ((real32)systime.wMilliseconds) / 1000.0f;
    return date_time;
}

internal void *
win32_allocate(uintptr size)
{
    //MEM_RESERVE just reserves a region of VIRTUAL memory
    //MEM_COMMIT actually maps physical pages to that region, returns zeroed pages
    //MEM_RESET AND MEM_RESET_UNDO do NOT zero out memory again, they serve to LOCK
    //the memory ranges but it doesn't actually decommit and commit again
    void *result = VirtualAlloc(0, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    return result;
}

struct Heap_Scratch_Block 
{    
    Memory_Arena arena;
    Heap_Scratch_Block(u32 size)
    {
        arena = make_memory_arena(win32_allocate(size), size);
    }
    
    //NOTE destructors get called in reverse order as the objects were created
    //meaning there's on problem with making multiple scratch blocks with same arena
    //and there's no issue when making a scratch block from a scratch block, ...it's just an arena
    ~Heap_Scratch_Block()
    {
        VirtualFree(arena.data, 0, MEM_RELEASE); 
    }
    
    operator Memory_Arena*() { return &this->arena; }
};

internal String
get_executable_directory(char *buffer, u32 buffer_size)
{
    String exe_directory = mkstr(buffer, 0, buffer_size);
    exe_directory.length = GetModuleFileNameA(NULL, exe_directory.str, exe_directory.size);
    remove_current_file_directory(&exe_directory);
    null_terminate(exe_directory);
    return exe_directory;
}

internal void
make_exe_current_directory()
{
    char exe_directory_buffer[MAX_PATH];
    String exe_directory;
    exe_directory = mkstr_from_buffer(exe_directory_buffer);
    exe_directory.length = GetModuleFileNameA(NULL, exe_directory.str, exe_directory.size);
    remove_current_file_directory(&exe_directory);
    null_terminate(exe_directory);
    
    SetCurrentDirectoryA(exe_directory.str);
}




#endif //WIN32_H
