%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yylineno;
extern FILE* yyin;
void yyerror(const char*);
#define YYERROR_VERBOSE yes
%}

%union {
  char* string;
  double floatValue;
  int intValue;
}

// used tokens (KW is abbreviation for keyword)
%token AND
%token OR
%token EQ
%token NEQ
%token LEQ
%token GEQ
%token LSS
%token GRT
%token KW_BOOLEAN
%token KW_DO
// finde KW_ELSE weiter unten, wegen dangling-else-problem
%token KW_FLOAT
%token KW_FOR
%token KW_IF
%token KW_INT
%token KW_PRINTF
%token KW_RETURN
%token KW_VOID
%token KW_WHILE
%token CONST_INT
%token CONST_FLOAT
%token CONST_BOOLEAN
%token CONST_STRING
// ID = identifier
%token ID

// loese dangling-else-problem durch Festlegung von Praezedenzen
%nonassoc NOELSE
%nonassoc KW_ELSE

%start program

%%

program:
    declassignment ';' program      {}
  | functiondefinition program  {}
  | /* NULL */                      {}
  ;

functiondefinition:
    type id '(' ')' '{' statementlist '}'               {}
  | type id '(' parameterlist ')' '{' statementlist '}' {}
  ;

parameterlist:
    type id                   {}
  | type id ',' parameterlist {}
  ;

functioncall:
    id '(' ')'                {}
  | id '(' assignmentlist ')' {}
  ;

// hinzugefuegt, um * in functioncall aufzuloesen
assignmentlist:
    assignment                {}
  | assignment ',' assignment {}
  ;

statementlist:
    /* NULL */          {}
  | block statementlist {}
  ;

block:
    '{' statementlist '}' {}
  | statement             {}
  ;

statement:
    ifstatement           {}
  | forstatement          {}
  | whilestatement        {}
  | returnstatement ';'   {}
  | dowhilestatement ';'  {}
  | printf ';'            {}
  | declassignment ';'    {}
  | statassignment ';'    {}
  | functioncall ';'      {}
  ;

statblock:
    '{' statementlist '}' {}
  | statement             {}

ifstatement:
    KW_IF '(' assignment ')' statblock %prec NOELSE       {}
  | KW_IF '(' assignment ')' statblock KW_ELSE statblock  {}
  ;

forstatement:
    KW_FOR '(' statassignment ';' expr ';' statassignment ')' statblock {}
  | KW_FOR '(' declassignment ';' expr ';' statassignment ')' statblock {}
  ;

dowhilestatement:
    KW_DO statblock KW_WHILE '(' assignment ')' {}
  ;

whilestatement:
    KW_WHILE '(' assignment ')' statblock {}
  ;

returnstatement:
    KW_RETURN             {}
  | KW_RETURN assignment  {}
  ;

printf:
    KW_PRINTF '(' assignment ')'    {}
  | KW_PRINTF '(' CONST_STRING ')'  {}
  ;

declassignment:
    type id                 {}
  | type id '=' assignment  {}
  ;

type:
    KW_BOOLEAN  {}
  | KW_FLOAT    {}
  | KW_INT      {}
  | KW_VOID     {}
  ;

statassignment:
    id '=' assignment {}
  ;

assignment:
    id '=' assignment {}
  | expr              {}
  ;

expr:
    simpexpr              {}
  | simpexpr EQ simpexpr  {}
  | simpexpr NEQ simpexpr {}
  | simpexpr LEQ simpexpr {}
  | simpexpr GEQ simpexpr {}
  | simpexpr LSS simpexpr {}
  | simpexpr GRT simpexpr {}
  ;

simpexpr:
    '-' term              {}
  | '-' term '+' contterm {}
  | '-' term '-' contterm {}
  | '-' term OR contterm  {}
  | contterm              {}
  ;

// hinzugefuegt, um * in simpexpr aufzuloesen
contterm:
    term                  {}
  | term '+' contterm     {}
  | term '-' contterm     {}
  | term OR contterm      {}
  ;

term:
    factor          {}
  | factor '*' term {}
  | factor '/' term {}
  | factor AND term {}
  ;

factor:
    CONST_INT           {}
  | CONST_FLOAT         {}
  | CONST_BOOLEAN       {}
  | functioncall        {}
  | id                  {}
  | '(' assignment ')'  {}
  ;

id:
    ID {}
  ;


%%

int main(int argc, char* argv[]) {
  if (argc != 2) yyin = stdin;
  else {
    yyin = fopen(argv[1], "r");
    if (yyin == 0) {
      fprintf(stderr, "Fehler: Konnte Datei %s nicht zum lesen oeffnen.\n", argv[1]);
      exit(-1);
    }
  }

  yyparse();

  return 0;
}

void yyerror(const char* msg) {
  fprintf(stderr, "ERROR: %s. Error occured near line %d.\n", msg, yylineno);
  exit(-1);
}
