#include <stdio.h>
#include <stdlib.h>
#include "minako.h"

yystype yylval;
int currentToken;
int nextToken;

void init(){
  currentToken = yylex();
  nextToken = yylex();
}

void eat(){
  //printf("ate token:%i (might be char '%c')\n", currentToken, currentToken);
  currentToken = nextToken;
  nextToken = yylex();
}

int isToken(int token){
  return (currentToken == token);
}

int isNextToken(int token){
  return (nextToken == token);
}

void isTokenAndEat(int token){
  if(isToken(token)){
    eat();
  }
  else{
    fprintf(stderr, "ERROR: Syntaxfehler in Zeile (%d)\n", yylineno);
    fprintf(stderr, "\tGelesenes  TOKEN #%i (might be char '%c')\n", currentToken, currentToken);
    fprintf(stderr, "\tErwartetes TOKEN #%i (might be char '%c')\n", token, token);
    abort();
  }
}

void type(){
  if( isToken(KW_BOOLEAN) ||
      isToken(KW_FLOAT) ||
      isToken(KW_INT) ||
      isToken(KW_VOID)){
    eat();
  }
  else{
    fprintf(stderr, "ERROR: Syntaxfehler in Zeile (%d)\n", yylineno);
    fprintf(stderr, "\tErwartete TOKEN aus FIRST(type)\n");
    abort();
  }
}

int isFirstOfFunctioncall(){
  return isToken(ID);
}

void functioncall(){
  isTokenAndEat(ID);
  isTokenAndEat('(');
  isTokenAndEat(')');
}

int isFirstOfFactor(){
  int res = 0;
  res |= isToken(CONST_INT);
  res |= isToken(CONST_FLOAT);
  res |= isToken(CONST_BOOLEAN);
  res |= isFirstOfFunctioncall();
  res |= isToken(ID);
  res |= isToken('(');
  return res;
}

void assignment();

void factor(){
  if(
    isToken(CONST_INT) ||
    isToken(CONST_FLOAT) ||
    isToken(CONST_BOOLEAN)
  ){
    eat();
  }
  else if(isToken(ID)){
    if(nextToken == '('){
      functioncall();
    }
    else{
      eat();
    }
  }
  else if(isToken('(')){
    eat();
    assignment();
    isTokenAndEat(')');
  }
  else{
    fprintf(stderr, "ERROR: Syntaxfehler in Zeile (%d)\n", yylineno);
    fprintf(stderr, "\tErwartete TOKEN aus FIRST(factor)\n");
    abort();
  }
}

int isFirstOfTermOp(){
  int res = 0;
  res |= isToken('*');
  res |= isToken('/');
  res |= isToken(AND);
  return res;
}

void termOp(){
  if( isToken('*') ||
      isToken('/') ||
      isToken(AND)
  ){
    eat();
  }
  else{
    fprintf(stderr, "ERROR: Syntaxfehler in Zeile (%d)\n", yylineno);
    fprintf(stderr, "\tErwartete TOKEN aus FIRST(termop)\n");
    abort();
  }
}

int isFirstOfOpfactor(){
  return isFirstOfTermOp();
}

void opfactor(){
  while(isFirstOfOpfactor()){
    termOp();
    factor();
  }
}

int isFirstOfTerm(){
  return isFirstOfFactor();
}

void term(){
  factor();
  if(isFirstOfOpfactor()){
    opfactor();
  }
}

int isFirstOfSimpexprOp(){
  int res = 0;
  res |= isToken('+');
  res |= isToken('-');
  res |= isToken(OR);
  return res;
}

void simpexprOp(){
  if( isToken('+') ||
      isToken('-') ||
      isToken(OR)
  ){
    eat();
  }
  else{
    fprintf(stderr, "ERROR: Syntaxfehler in Zeile (%d)\n", yylineno);
    fprintf(stderr, "\tErwartete TOKEN aus FIRST(simpexprop)\n");
    abort();
  }
}

int isFirstOfOpterm(){
  return isFirstOfSimpexprOp();
}

void opterm(){
  while(isFirstOfOpterm()){
    simpexprOp();
    term();
  }
}

int isFirstOfSimpexpr(){
  return isToken('-') || isFirstOfTerm();
}

void simpexpr(){
  if(isToken('-')){
    eat();
  }
  term();
  if(isFirstOfOpterm()){
    opterm();
  }
}

int isFirstOfExprOp(){
  int res = 0;
  res |= isToken(EQ);
  res |= isToken(NEQ);
  res |= isToken(LEQ);
  res |= isToken(GEQ);
  res |= isToken(GRT);
  res |= isToken(LSS);
  return res;
}

void exprOp(){
  if( isToken(EQ) ||
      isToken(NEQ) ||
      isToken(LEQ) ||
      isToken(GEQ) ||
      isToken(GRT) ||
      isToken(LSS)
  ){
    eat();
  }
  else{
    fprintf(stderr, "ERROR: Syntaxfehler in Zeile (%d)\n", yylineno);
    fprintf(stderr, "\tErwartete TOKEN aus FIRST(exprop)\n");
    abort();
  }
}

int isFirstOfExpr(){
  return isFirstOfSimpexpr();
}

void expr(){
  simpexpr();
  if(isFirstOfExprOp()){
    exprOp();
    simpexpr();
  }
}

int isFirstOfAssignment(){
  return isToken(ID) || isFirstOfExpr();
}

void assignment(){
  if(isToken(ID) && isNextToken('=')){
    eat();
    eat();
    assignment();
  }
  else if(isFirstOfExpr()){
    expr();
  }
  else{
    fprintf(stderr, "ERROR: Syntaxfehler in Zeile (%d)\n", yylineno);
    fprintf(stderr, "\tErwartete TOKEN aus FIRST(assignment)\n");
    abort();
  }
}

int isFirstOfIfstatement(){
  return isToken(KW_IF);
}

void block();

void ifstatement(){
  isTokenAndEat(KW_IF);
  isTokenAndEat('(');
  assignment();
  isTokenAndEat(')');
  block();
}

int isFirstOfReturnstatement(){
  return isToken(KW_RETURN);
}

void returnstatement(){
  isTokenAndEat(KW_RETURN);
  if(isFirstOfAssignment()){
    assignment();
  }
}

int isFirstOfPrintf(){
  return isToken(KW_PRINTF);
}

void printfT(){
  isTokenAndEat(KW_PRINTF);
  isTokenAndEat('(');
  assignment();
  isTokenAndEat(')');
}

int isFirstOfStatassignment(){
  return isToken(ID);
}

void statassignment(){
  isTokenAndEat(ID);
  isTokenAndEat('=');
  assignment();
}

int isFirstOfStatementFirstID(){
  return isToken(ID);
}

void statementFirstID(){
  if(isNextToken('(')){
    functioncall();
    isTokenAndEat(';');
  }
  else if(isNextToken('=')){
    statassignment();
    isTokenAndEat(';');
  }
  else{
    fprintf(stderr, "ERROR: Syntaxfehler in Zeile (%d)\n", yylineno);
    fprintf(stderr, "\tErwartete TOKEN aus FIRST(statementfirstID)\n");
    abort();
  }
}

int isFirstOfStatement(){
  int res = 0;
  res |= isFirstOfIfstatement();
  res |= isFirstOfReturnstatement();
  res |= isFirstOfPrintf();
  res |= isFirstOfStatementFirstID();
  return res;
}

void statement(){
  if(isFirstOfIfstatement()){
    ifstatement();
  }
  else if(isFirstOfReturnstatement()){
    returnstatement();
    isTokenAndEat(';');
  }
  else if(isFirstOfPrintf()){
    printfT();
    isTokenAndEat(';');
  }
  else if(isFirstOfStatementFirstID()){
    statementFirstID();
  }
  else{
    fprintf(stderr, "ERROR: Syntaxfehler in Zeile (%d)\n", yylineno);
    fprintf(stderr, "\tErwartete TOKEN aus FIRST(statement)\n");
    abort();
  }
}

void statementlist();

int isFirstOfBlock(){
  return isToken('{') || isFirstOfStatement();
}

void block(){
  if(isToken('{')){
    eat('{');
    statementlist();
    isTokenAndEat('}');
  }
  else if(isFirstOfStatement()){
    statement();
  }
  else{
    fprintf(stderr, "ERROR: Syntaxfehler in Zeile (%d)\n", yylineno);
    fprintf(stderr, "\tErwartete TOKEN aus FIRST(block)\n");
    abort();
  }
}

void statementlist(){
  while(isFirstOfBlock()){
    block();
  }
}

void functiondefinition(){
  type();
  isTokenAndEat(ID);git
  isTokenAndEat('(');
  isTokenAndEat(')');
  isTokenAndEat('{');
  statementlist();
  isTokenAndEat('}');
}

void program(){
  while(!isToken(EOF)){
    functiondefinition();
  }
  isTokenAndEat(EOF);
}

int main(int argc, char* argv[]) {
  if (argc != 2) yyin = stdin;
  else {
    yyin = fopen(argv[1], "r");
    if (yyin == 0) {
      fprintf(stderr, "Fehler: Konnte Datei %s nicht zum lesen oeffnen.\n", argv[1]);
      exit(-1);
    }
  }

  init();
  program();
  return 0;
}



