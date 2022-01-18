#ifndef LEXER_H //created on December 6th 2021 
#define LEXER_H

struct Token
{
    enum Type : u32
    {
        INVALID,
        IDENTIFIER,
        INTEGER,
        EOF,
    };
    
    Type   type;
    String str; //just a slice into the open file, 
};

internal nil
is_identifier(Token *token, String string) 
{ return (token->type == Token::IDENTIFIER) && strmatch(token->str, string); }

struct Lexer
{
    char *entire_file;
    char *at;
    //uintptr filesize;
    
    nil is_whitespace(char c)  { return (c == ' ') || (c == '\n') || (c == '\r') || (c == '\t'); }
    nil at_whitespace()        { return is_whitespace(*at); }
    nil at_c_comment_start()   { return (at[0] == '/' && at[1] == '*'); }
    nil at_cpp_comment_start() { return (at[0] == '/' && at[1] == '/'); }
    
    void skip_pass_whitespace_and_comments(nil stop_at_newline = false)
    {
        while(at_whitespace() || at_c_comment_start() || at_cpp_comment_start())
        {
            if(at_whitespace())	while(at_whitespace())	at += 1;
            else if(at_cpp_comment_start()) //c++ comment
            {
                while(*at)
                {
                    if(at[0] == '\n')
                    {
                        if (stop_at_newline)	return;
                        at += 1;
                        break;
                    }
                    else if(at[0] == '\r' && at[1] == '\n')
                    {
                        if (stop_at_newline)	return;
                        at += 2;
                        break;
                    }
                    else at += 1;
                }
            }
            else /*c comment*/
            {
                // TODO incomplete this assumes comments are always closed off
                // we could have a trailing junk '\' at the end of the character stream 
                while(*at && !(at[0] == '*' && at[1] == '/'))
                    at += 1;
                if(*at)
                {
                    assert(at[0] == '*' && at[1] == '/');
                    at += 2;
                }
            }
        }
    }
    
    Token eat_token()
    {
        skip_pass_whitespace_and_comments();
        Token token = {};
        switch(*at)
        {
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case '_': 
        {
            token.type = Token::IDENTIFIER;
            token.str.str = at;
            while(is_alphanumeric(*at) || *at == '_') at += 1;
            token.str.length = token.str.size = to_s32((uintptr)at - (uintptr)token.str.str);    
        } break;
        
        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9': case '0': 
        {
            token.type = Token::INTEGER;
            token.str.str = at;
            while(is_digit(*at)) at += 1;
            token.str.length = token.str.size = to_s32((uintptr)at - (uintptr)token.str.str);
        } break;
        
        
        //TODO incomplete and we don't handle aggregate types like !=, 
        case '#': case '(': case ')': case '{': case '}': case '[': case ']': case ',': case ';':
        case ':': case '~': case '=': case '*': case '+': case '-': case '&': case '<': case '>':
        case '@':
        {
            token.type = (Token::Type) *at;
            token.str = mkstr(at, 1, 1);
            at += 1;  
        } break;
        
        case 0: 
        {
            token.type = Token::EOF; 
            token.str = mkstr(at, 1, 1);
        } break;
        
        
        
        default: 
        {
            token.type = Token::INVALID;
            token.str = mkstr(at, 1, 1);
            at += 1; 
        } break; //NOTE for unhandled cases, we eat 1 char and return Invalid Token
        }
        return token;
    }
    
    Token peek_token()
    {
        char *old_at = at;
        Token t = eat_token();
        at = old_at;
        return t;
    }
    
    void set_cursor_after_token(Token *token)
    { if (token->str.str) at = token->str.str + token->str.length; }
    
    void set_cursor_at_token(Token *token)
    { 
        if (token->str.str) 
            at = token->str.str; 
    }
    
    //eats only of type type, otherwise returns null token
    Token eat_token(Token::Type type)
    {
        char *old_at = at;
        Token t = eat_token();
        if (t.type == type) return t;
        else //revert back
        {
            at = old_at;
            return {};
        }
    }
    
    //eats only of type type, otherwise returns null token
    Token eat_token(char c) { return eat_token((Token::Type) c); }
    
    nil eat_identifier(String identifier)
    {
        char *old_at = at;
        Token t = eat_token();
        if (t.type == Token::IDENTIFIER && strmatch(t.str, identifier)) return true;
        else //revert back
        {
            at = old_at;
            return false;
        }
    }    
};



#endif //LEXER_H
