%option noyywrap
%option yylineno

%{
  #include "urlscanner.h"
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
