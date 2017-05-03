/*

	Program: To remove the comments and trim the whitespaces from the input file.

	By: Bala Kumar (149105348) - CSE 'A'

*/
	
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
	FILE *ip = fopen("input.c", "r"),
		*op = fopen("output.c", "w");

	if(!ip || !op) {
		fprintf(stderr, "Couldnt open the file\n");
		exit(0);
	}

	int chcurr, chlast;
	int skip = 0, ctr = 0, lines = 0;

	for( ; (chcurr = fgetc(ip)) != EOF; chlast = chcurr) {

		if (chlast == '/' && chcurr == '/') {
			while( (chcurr = fgetc(ip)) != '\n' );
				continue;
		}

		else if (chlast == '/' && chcurr == '*') {
			for( ;(chcurr = fgetc(ip)) != '/' && chlast != '*'; chlast = chcurr);
			skip = 1;
			continue;
		}


		if (chcurr == '/')
			continue;
		if (chlast == '/' && !skip) {
			fputc('/', op);
			skip = 0;
			continue;
		}
		if (chcurr == '"') {
			fputc(chcurr, op);
			while ( (chcurr = fgetc(ip)) != '"' ) {
				fputc(chcurr, op);
			}

			fputc('"', op);
			continue;
		}

		if(isspace(chcurr) && isspace(chlast))
			continue;
		if(isspace(chcurr)) {
			if (chcurr == '\n' && ctr == 0)
				;
			else if (chcurr == '\n') {
				ctr = 0;
				lines++;
			}
			else
				ctr++;

			fputc(' ', op);
		}
		else {
			ctr++;
			fputc(chcurr, op);
		}
	}

	printf ("No. of lines : %d\n", lines);
	fclose(ip);
	fclose(op);
}
 
