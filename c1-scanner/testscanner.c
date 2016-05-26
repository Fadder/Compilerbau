#include <stdio.h>
#include <stdlib.h>
#include "minako.h"

yystype yylval;

int main(int argc, char* argv[]) {
	int token;

	if (argc != 2) yyin = stdin;
	else {
		yyin = fopen(argv[1], "r");
		if (yyin == 0) {
			fprintf(stderr, "Fehler: Konnte Datei %s nicht zum lesen oeffnen.\n", argv[1]);
			exit(-1);
		}
	}

	while ((token = yylex()) != EOF) {
		if (token == ID) printf("Line: %3d\tID       : %s\n", yylineno, yylval.string);
		else if (token == CONST_INT) printf("Line: %3d\tINT      : %d\n", yylineno, yylval.intValue);
		else if (token == CONST_FLOAT) printf("Line: %3d\tFLOAT    : %f\n", yylineno, yylval.floatValue);
		else printf("Line: %3d\tToken Nr.: %d\n", yylineno, token);
	}
	return 0;
}
