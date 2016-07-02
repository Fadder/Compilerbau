/* Compilerbau
 * Aufgabe 5
 * GOYA-Gruppe: cb0
 * Daniel Haase 560364,
 * Kai Kronenberg 558823,
 * Vincent Becker 564544
 */

#include <stdio.h>
#include <stdlib.h>
#include "minako.h"

int currentToken;
int nextToken;


// Hilfsfunktionen ////////////////////////////////////////////////////////////

// liest bei Programmstart die ersten Token, ohne sie zu konsumieren
void initReading(){
  currentToken = yylex();
  nextToken = yylex();
}

// konsumiert ein Token und liest das naechste
void eat(){
  currentToken = nextToken;
  nextToken = yylex();
}

// testet, ob das aktuelle Token dem gewuenschten entspricht
int isToken(int token){
  return (currentToken == token);
}

// testet, ob das naechste Token dem gewuenschten entspricht
int isNextToken(int token){
  return (nextToken == token);
}

// konsumiert, das naechste Token, falls es dem gewuenschtem entspricht
// gibt einen Fehler aus und bricht ab, falls nicht
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

// IsFirstOf-Funktionen ///////////////////////////////////////////////////////
// beschreiben die FIRST-Mengen der Nichtterminale

int isFirstOfFunctioncall(){
  return isToken(ID);
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

int isFirstOfTermOp(){
  int res = 0;
  res |= isToken('*');
  res |= isToken('/');
  res |= isToken(AND);
  return res;
}

int isFirstOfOpfactor(){
  return isFirstOfTermOp();
}

int isFirstOfTerm(){
  return isFirstOfFactor();
}

int isFirstOfSimpexprOp(){
  int res = 0;
  res |= isToken('+');
  res |= isToken('-');
  res |= isToken(OR);
  return res;
}

int isFirstOfOpterm(){
  return isFirstOfSimpexprOp();
}

int isFirstOfSimpexpr(){
  return isToken('-') || isFirstOfTerm();
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

int isFirstOfExpr(){
  return isFirstOfSimpexpr();
}

int isFirstOfAssignment(){
  return isToken(ID) || isFirstOfExpr();
}

int isFirstOfIfstatement(){
  return isToken(KW_IF);
}

int isFirstOfReturnstatement(){
  return isToken(KW_RETURN);
}

int isFirstOfPrintfNT(){
  return isToken(KW_PRINTF);
}

int isFirstOfStatassignment(){
  return isToken(ID);
}

int isFirstOfStatement(){
  int res = 0;
  res |= isFirstOfIfstatement();
  res |= isFirstOfReturnstatement();
  res |= isFirstOfPrintfNT();
  res |= isToken(ID);
  return res;
}

int isFirstOfBlock(){
  return isToken('{') || isFirstOfStatement();
}

// Produktionsfunktionen //////////////////////////////////////////////////////
// beschreiben die Regeln der Grammatik
// sie konsumieren das jeweilige Nichtterminal
// bei erkanntem Fehler, geben sie ihn aus und brechen die Ausfuehrung ab

// type --> KW_BOOLEAN | KW_FLOAT | KW_INT | KW_VOID
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

// functioncall --> ID()
void functioncall(){
  isTokenAndEat(ID);
  isTokenAndEat('(');
  isTokenAndEat(')');
}

// factorWithFirstID --> ID | functioncall
void factorWithFirstID(){
  if(isNextToken('(')){
    functioncall();
  }
  else{
    eat();
  }
}

// Funktionsdeklaration, um Zyklus assignment <--> factor (ueber dritte) aufzuloesen
void assignment();

// factor --> CONST_INT | CONST_FLOAT | CONST_BOOLEAN
//          | functionscall | ID | (assignment)
void factor(){
  if(
    isToken(CONST_INT) ||
    isToken(CONST_FLOAT) ||
    isToken(CONST_BOOLEAN)
  ){
    eat();
  }
  else if(isToken(ID)){
    factorWithFirstID();
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

// termOp --> * | / | &&
// die Operaten, die in der Regel fuer term auftauchen
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

// opfactor --> termop factor | termop factor opfactor
// loest den * in der Regel fuer term auf
void opfactor(){
  while(isFirstOfOpfactor()){
    termOp();
    factor();
  }
}

// term --> factor | factor opfactor
void term(){
  factor();
  if(isFirstOfOpfactor()){
    opfactor();
  }
}

// simpexprOp --> + | - | ||
// die Operaten, die in der Regel fuer simpexpr auftauchen
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

// opterm --> simpexprop term | simpexprop term opterm
// loest den * in der Regel fuer simpexpr auf
void opterm(){
  while(isFirstOfOpterm()){
    simpexprOp();
    term();
  }
}

// simpexpr --> - term | - term opterm | term | term opterm
void simpexpr(){
  if(isToken('-')){
    eat();
  }
  term();
  if(isFirstOfOpterm()){
    opterm();
  }
}

// exprOp --> == | != | <= | >= | < | >
// die Operaten, die in der Regel fuer expr auftauchen
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

// expr --> simpexpr | simpexpr exprOp simpexpr
void expr(){
  simpexpr();
  if(isFirstOfExprOp()){
    exprOp();
    simpexpr();
  }
}

// assignment --> ID = assignment | expr
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

// Funktionsdeklaration, um Zyklus block <--> ifstatement (ueber dritte) aufzuloesen
void block();

// ifstatement --> KW_IF ( assignment ) block
void ifstatement(){
  isTokenAndEat(KW_IF);
  isTokenAndEat('(');
  assignment();
  isTokenAndEat(')');
  block();
}

// returnstatement --> KW_RETURN | KW_RETURN assignment
void returnstatement(){
  isTokenAndEat(KW_RETURN);
  if(isFirstOfAssignment()){
    assignment();
  }
}

// printfNT --> KW_PRINTF ( assignment )
// NT fuer Nichtterminal um Namenskonflikte mit C zu vermeiden
void printfNT(){
  isTokenAndEat(KW_PRINTF);
  isTokenAndEat('(');
  assignment();
  isTokenAndEat(')');
}

// statassignment --> ID = assignment
void statassignment(){
  isTokenAndEat(ID);
  isTokenAndEat('=');
  assignment();
}

// statementWithFirstID --> statassignment | functioncall
void statementWithFirstID(){
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

// statement --> ifstatement | returnstatement ; | printf ;
//             | statassignment ; | functioncall ;
void statement(){
  if(isFirstOfIfstatement()){
    ifstatement();
  }
  else if(isFirstOfReturnstatement()){
    returnstatement();
    isTokenAndEat(';');
  }
  else if(isFirstOfPrintfNT()){
    printfNT();
    isTokenAndEat(';');
  }
  else if(isToken(ID)){
    statementWithFirstID();
  }
  else{
    fprintf(stderr, "ERROR: Syntaxfehler in Zeile (%d)\n", yylineno);
    fprintf(stderr, "\tErwartete TOKEN aus FIRST(statement)\n");
    abort();
  }
}

// Funktionsdeklaration, um Zyklus block <--> statementlist aufzuloesen
void statementlist();

// block --> { statementlist } | statement
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

// statementlist --> block*
// hier keine EBNF, weil auch ohne schoen einfach zu implementieren :)
void statementlist(){
  while(isFirstOfBlock()){
    block();
  }
}

// functiondefinition --> type ID() { statementlist }
void functiondefinition(){
  type();
  isTokenAndEat(ID);
  isTokenAndEat('(');
  isTokenAndEat(')');
  isTokenAndEat('{');
  statementlist();
  isTokenAndEat('}');
}

// program --> functiondefinition* EOF
// auch hier keine EBNF
void program(){
  while(!isToken(EOF)){
    functiondefinition();
  }
  isTokenAndEat(EOF);
}

// Hauptfunktion //////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
  if (argc != 2) yyin = stdin;
  else {
    yyin = fopen(argv[1], "r");
    if (yyin == 0) {
      fprintf(stderr, "Fehler: Konnte Datei %s nicht zum lesen oeffnen.\n", argv[1]);
      exit(-1);
    }
  }

  initReading();
  program(); // konsumiere Startsymbol
  return 0;
}



