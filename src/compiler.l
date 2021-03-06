%{
#include <stdio.h>
#include <string.h>

typedef int *list_t, *list_it;
typedef struct {
    list_t brklist;
    list_t contlist;
} a_statjump_t;

typedef struct {
    list_t nextlist;
    int next_ip;
} a_purejump_t;

typedef struct {
    int level;
    int addr;
    int is_arr;
} a_lval_t;

enum sym_type {
    sym_unknown,
    sym_char,
    sym_int,
};


#include "compiler.tab.h"

void line_comment();
void block_comment();


%}
%option yylineno

bsw     [A-Za-z_0-9]
bsd     [0-9]
al_     [A-Za-z_]

%%


[ \t\r\n]+  { ; }
[+\-*/%^&|~!]   { return *yytext; } 
[\(\)\[\]\{\}\<\>]      { return *yytext; } 
[,\.=;:?"']     { return *yytext; } 


"<="    { return T_LE; }
">="    { return T_GE; }
"=="    { return T_EQ; }
"!="    { return T_NE; }
"&&"    { return T_AND; }
"||"    { return T_OR; }


"if"        { return T_IF; }
"else"      { return T_ELSE; }

"while"     { return T_WHILE; }
"do"        { return T_DO; }
"break"     { return T_BREAK; }
"continue"  { return T_CONTINUE; }

"main"      { return T_MAIN; }
"read"      { return T_READ; }
"write"     { return T_WRITE; }

"true"      { return T_TRUE; }
"false"     { return T_FALSE; }


"int"       { return T_INT; }
"char"      { return T_CHAR; }
"bool"      { return T_BOOL; }


{bsd}+(\.{bsd}+)?   { yylval.number = atoi(yytext); return T_NUM; }
{al_}{bsw}*         { yylval.string = strdup(yytext); return T_IDENT; }


"//"    { line_comment(); }
"/*"    { block_comment(); }
%%

int yywrap() {
    return 1;
}

void redirectInput(FILE* file) {
    yyin = file;
}

void line_comment() {
    char c;
    while ((c = input()) != '\n') ;
    ++yylineno;
}

void block_comment() {
    char c;
    while (1) {
        while ((c = input()) != '*')
            if (c == '\n') ++yylineno;

        if ((c = input()) == '/') break;
        unput(c);
    }
}
