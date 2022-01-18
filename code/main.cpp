#include "stdio.h"
#undef    EOF
#include "ubiquitous.h"
#include "strings.h"
#include "maths.h"
#include "lexer.h"

#include "win32.h" //for find first file and friends

internal void *
open_entire_file_null_terminate(Memory_Arena *arena, String filepath)
{
    void *result = null;
    if (FILE *file = fopen(filepath.str, "rb"))
    {
        fseek(file, 0, SEEK_END);   
        u32 size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        result = push_size(arena, size+1);
        if (result)
        {
            size_t blocks_read = fread(result, size, 1, file);
            if (blocks_read == 1)
                ((u8 *)result)[size] = 0;
            else fprintf(stderr, "open_entire_file_null_terminate with %s failed", filepath.str);
        }
        
        fclose(file);
    }
    return result;
}


internal void
outputf(FILE *output_handle, char *fmt, ...)
{
    assert(output_handle);
    va_list args;
    va_start(args, fmt);
    vfprintf(output_handle, fmt, args); 
    va_end(args);
}

enum 
{
    BLOG_ITEM_BODY_START = 1 << 0,
    BLOG_ITEM_BODY_END   = 1 << 1,
    
};

struct Blog_Item
{
    enum Type : u32
    {
        BODY,
        BOLD,
        ITALICS,
        ORDERED_LIST,
        UNORDERED_LIST,
        LINK,
    };
    
    Type type;
    u32  flags;
    union 
    {
        struct //for TEXT, BOLD, ITALICS 
        {
            String header;
            String text;     
        };
        
        struct 
        {
            String link_url;
            String link_text;
        };
        
        Blog_Item *list_items;
        //SLL(Blog_Item, next_in_list) list_items;
    };
    
    
    Blog_Item *prev_in_list;
    Blog_Item *next_in_list;
};

struct Blog_File 
{
    Memory_Arena *arena; //while building
    String title;
    
    DLL (Blog_Item, prev_in_list, next_in_list) items;
};

internal Blog_Item *
add_blog_item(Blog_File *blog, Blog_Item::Type type)
{
    Blog_Item *item = push_struct(blog->arena, Blog_Item);
    if (item)
    {
        *item = {};
        item->type = type;
        
        blog->items.push_tail(item);
    }
    return item;
}

internal String
eat_text_within_parenthesis(Lexer *lexer)
{
    String text = {};
    Token token = lexer->peek_token();
    if (token.type == '(')
    {
        lexer->eat_token();
        text.str = token.str.str + token.str.length;
        
        
        s32 balance = -1;
        while(balance != 0)
        {
            token = lexer->eat_token();
            if      (token.type == '(') balance -= 1;
            else if (token.type == ')') balance += 1;
            else if (token.type == Token::EOF) break;
        }
        
        if (token.type == ')')
        {
            text.length = (s32)(token.str.str - text.str); 
            text.size = text.length;
            
            if (text.length <= 0) text = {};
        }
        else text = {};
    }
    return text;
}



internal void
parse_blog_file(String filepath)
{
    LOCAL_STRING(out_filepath, MAX_PATH);
    append(&out_filepath, remove_file_path(remove_file_extension(filepath)));
    append(&out_filepath, ".html"s);
    
    Heap_Scratch_Block scratch(1024*1024*8);
    printf("Parsing %s into %s\n", filepath.str, out_filepath.str);
    char *blogcontents = (char *)open_entire_file_null_terminate(scratch, filepath);
    
    Blog_File blog = {};
    blog.arena = &scratch.arena;
    if (blogcontents)
    {
        Lexer lexer = {blogcontents, blogcontents};
        for (Token token = lexer.eat_token(); token.type != Token::EOF; token = lexer.eat_token())
        {
            if (token.type != '@') continue;
            token = lexer.eat_token();
            
            if (is_identifier(&token, "title"s))
            {
                blog.title = eat_text_within_parenthesis(&lexer);
            }
            else if (is_identifier(&token, "body"s))
            {
                Blog_Item *body = add_blog_item(&blog, Blog_Item::BODY);
                body->header = eat_text_within_parenthesis(&lexer);
                setfl(*body, BLOG_ITEM_BODY_START);
                 
                token = lexer.peek_token();
                while(token.type != Token::EOF)
                {
                    token = lexer.eat_token(); 
                    if (token.type != '@') //means continue eating plain text 
                    {
                        if (!body)
                            body = add_blog_item(&blog, Blog_Item::BODY);
                        if (!body->text.str)
                            body->text.str = token.str.str;
                        continue;
                    }
                    
                    Token at_symbol = token;
                    token = lexer.eat_token();
                    
                    if (body && body->text.str) //finish off body if there is one
                    {
                        assert(at_symbol.type == '@');
                        body->text.length = (s32)(at_symbol.str.str - body->text.str);
                        body->text.size = body->text.length;
                        if (body->text.length <= 0) body->text = {}; //TODO maybe remove node as well    
                    }
                    
                    
                    if (token.type == Token::IDENTIFIER && 
                        (   strmatch(token.str, "link"s) ||
                            strmatch(token.str, "bold"s) ||
                            strmatch(token.str, "italics"s)))
                    {
                        
                        body = null;
                        
                        if(strmatch(token.str, "link"s))
                        {
                            Blog_Item *item = add_blog_item(&blog, Blog_Item::LINK);
                            item->link_text = eat_text_within_parenthesis(&lexer);
                            continue;    
                        }
                        else if(strmatch(token.str, "bold"s))
                        {
                            Blog_Item *item = add_blog_item(&blog, Blog_Item::BOLD);
                            item->text = eat_text_within_parenthesis(&lexer);
                            continue;    
                        }
                        else if(strmatch(token.str, "italics"s))
                        {
                            Blog_Item *item = add_blog_item(&blog, Blog_Item::ITALICS);
                            item->text = eat_text_within_parenthesis(&lexer);
                            continue;    
                        }
                        else fprintf(stderr, "Failed to handle case"); //reached new tag to
                    }
                    else //reached a @marker() that cannot be used within <p></p> so we abort out
                    {
                        if (body) setfl(*body, BLOG_ITEM_BODY_END);
                        lexer.set_cursor_at_token(&at_symbol);
                        break;
                    }
                }
                
                if (token.type == Token::EOF && body)
                {
                    setfl(*body, BLOG_ITEM_BODY_END);
                    if (body->text.str)
                    {
                        body->text.length = (s32)(token.str.str - body->text.str);
                        body->text.size = body->text.length;
                        if (body->text.length <= 0) body->text = {}; //TODO maybe remove node as well    
                    }
                }
                    
            }
            //printf("Token %.*s\n", token.str.length, token.str.str);
        }
    }
    
    #if 0
    printf("Item test here\n");
    dll_foreach(item, blog.items)
    {
        
        
        switch(item->type)
        {
        case Blog_Item::BODY:
        {
            printf("Item(Body)\n");
            if (item->header.str)
                printf("    header(%.*s)\n", item->header.length, item->header.str);
            printf("    text(%.*s)\n", item->text.length, item->text.str);
            
        } break;
        
        case Blog_Item::BOLD:
        printf("Item(bold)\n");
        printf("    text(%.*s)\n", item->text.length, item->text.str);
        break;
        
        
        case Blog_Item::ITALICS:
        printf("Item(italics)\n");
        printf("    text(%.*s)\n", item->text.length, item->text.str);
        break;
        
        //case Blog_Item::ORDERED_LIST,
        //case Blog_Item::UNORDERED_LIST,
        case Blog_Item::LINK:
        printf("Item(link)\n");
        printf("    link_text(%.*s)\n", item->link_text.length, item->link_text.str);
        break;
        }
        //printf("Item(%u), {%.*s}end\n", );
    }
    breakpoint;
    #else
    if (FILE *out = fopen(out_filepath.str, "wb"))
    {
        defer({ fclose(out); });
        u32 current_indent = 0;
        //#define START_END_TAG(tag) outputf(out, "<" tag ">"); defer({ outputf(out, "</" tag ">"); });
        
        
        outputf(out, "<!DOCTYPE html>\n");
        outputf(out, "<html lang=\"en\">\n");
        
        {
            outputf(out, "<head>\n");
            outputf(out, "<meta charset=\"utf-8\">\n");
            outputf(out, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n");
            outputf(out, "<meta name=\"author\" content=\"Luis Reyes\">\n");
            outputf(out, "<title>%.*s</title>", blog.title.length, blog.title.str);
            outputf(out, "</head>\n\n");
        }
        
        {
            outputf(out, "<body>\n");
            dll_foreach(item, blog.items)
            {
                switch(item->type)
                {
                case Blog_Item::BODY:
                {
                    if (item->header.str)
                        outputf(out, "<h1>%.*s</h1>", item->header.length, item->header.str);
                    
                    if (item->flags & BLOG_ITEM_BODY_START)
                        outputf(out, "<p>");
                    
                    outputf(out, "%.*s", item->text.length, item->text.str);
                    
                    if (item->flags & BLOG_ITEM_BODY_END)
                        outputf(out, "</p>\n");
                } break;
                
                case Blog_Item::BOLD:
                {
                    outputf(out, "<b>%.*s</b>", item->text.length, item->text.str);
                } break;
                
                case Blog_Item::ITALICS:
                {
                    outputf(out, "<i>%.*s</i>", item->text.length, item->text.str);
                } break;
                
                
                case Blog_Item::ORDERED_LIST:
                {
                    
                } break;
                
                case Blog_Item::UNORDERED_LIST:
                {
                    
                } break;
                
                case Blog_Item::LINK:
                {
                    //TODO
                } break;
                
                default: fprintf(stderr, "Didn't handle blog item case (%d)", item->type); break;
                }
            }
            
            outputf(out, "</body>\n");
        }
        
        outputf(out, "</html>");
    }
    #endif
}


int main(int argc, char **argv)
{
    char exe_dir_buffer[MAX_PATH];
    String exe_directory = get_executable_directory(exe_dir_buffer, countof(exe_dir_buffer));
    String directory = remove_current_file_directory(exe_directory); 
    null_terminate(directory); //TODO I don't like this call, everything should just auto null-termiante
    SetCurrentDirectoryA(directory.str);
     
    
    
    printf("Current directory is %s\n", directory.str);
    for (File_Iterator it("stuff\\"s); it.valid(); it.get_next())
    {
        if (it.desc.is_folder) continue;
        
        String ext = get_file_extension(it.desc.filename);
        if (strmatch(ext, ".blog"))
        {
            LOCAL_STRING(filepath, MAX_PATH);
            appendf(&filepath, "stuff\\%s", it.desc.filename.str);
            parse_blog_file(filepath);    
        }
    }
}