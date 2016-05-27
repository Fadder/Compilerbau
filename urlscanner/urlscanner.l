/* Compilerbau
 * Aufgabe 3
 * GOYA-Gruppe: cb0
 * Daniel Haase 560364,
 * Kai Kronenberg 558823,
 * Vincent Becker 564544
 */

%option noyywrap
%option nounput
%option yylineno

%{
  #include "urlscanner.h"
  #define YY_NO_INPUT
  char *strdup(char*);
  int fileno(FILE *stream);
%}

LETTER   [a-zA-Z]
ID       LETTER+
STRING   [^ \t\r\n\"]*
LINKTEXT [^<]*

%x ATAG
%x HREF
%x AFTER_HREF
%x TEXT

%%

"<a "             {BEGIN(ATAG);}
<ATAG>"href=\""   {BEGIN(HREF);}
<HREF>{STRING}    {
                    BEGIN(AFTER_HREF);
                    yylval = strdup(yytext);
                    return TOKEN_URL;
                  }
<AFTER_HREF>>     {BEGIN(TEXT);}
<AFTER_HREF>.     {/* do nothing */}
<TEXT>{LINKTEXT}  {
                    yylval = strdup(yytext);
                    return TOKEN_TEXT;
                  }
<TEXT>"</a"       {BEGIN(INITIAL);}
<ATAG>"</a"       {BEGIN(INITIAL);}
<ATAG>.           {/* do nothing */}

<<EOF>> {return EOF;}
\n      {/* do nothing */}
.       {/* do nothing */}
