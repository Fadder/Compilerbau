%option noyywrap
%option nodefault
%option nounput
%option yylineno

%{
  #define YY_NO_INPUT
  #include "minako.h"
  char *strdup(char*);
  int fileno(FILE *stream);

%}

%x BLOCKCOMMENT
%x LINECOMMENT

DIGIT   [0-9]
INTEGER {DIGIT}+
FLOAT2  {INTEGER}"."{INTEGER}|"."{INTEGER}
LETTER  [a-zA-Z]

%%

"&&" {return AND;}
"||" {return OR;}
"==" {return EQ;}
"!=" {return NEQ;}
"<=" {return LEQ;}
">=" {return GEQ;}
"<"  {return LSS;}
">"  {return GRT;}

"bool"   {return KW_BOOLEAN;}
"do"     {return KW_DO;}
"else"   {return KW_ELSE;}
"float"  {return KW_FLOAT;}
"for"    {return KW_FOR;}
"if"     {return KW_IF;}
"int"    {return KW_INT;}
"printf" {return KW_PRINTF;}
"return" {return KW_RETURN;}
"void"   {return KW_VOID;}
"while"  {return KW_WHILE;}

{INTEGER}                         {
                                  yylval.intValue = atoi(yytext);
                                  return CONST_INT;
                                }

{FLOAT2}([eE]([-+])?{INTEGER})? |
{INTEGER}[eE]([-+])?{INTEGER}   {
                                  yylval.floatValue = atof(yytext);
                                  return CONST_FLOAT;
                                }

"true"|"false"                  {
                                  yylval.intValue = atoi(yytext);
                                  return CONST_BOOLEAN;
                                }

"\""[^\n\'"']*"\""              {
                                  yylval.string = strdup(yytext);
                                  return CONST_STRING;
                                }

({LETTER})+({DIGIT}|{LETTER})*  {
                                  yylval.string = strdup(yytext);
                                  return ID;
                                }

"//"               {BEGIN(LINECOMMENT);}
<LINECOMMENT>.     {}
<LINECOMMENT>"\n"  {BEGIN(INITIAL);}

"/*"               {BEGIN(BLOCKCOMMENT);}
<BLOCKCOMMENT>.    {/* do nothing */}
<BLOCKCOMMENT>"\n" {/* do nothing */}
<BLOCKCOMMENT>"*/" {BEGIN(INITIAL);}

[ \t\n\r] {/* do nothing */}
<<EOF>>   {return EOF;}
.         {return *yytext;}
