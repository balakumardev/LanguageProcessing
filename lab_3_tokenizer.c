#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 32

const char *keywords[] = { "auto", "double", "int",	"struct", "break", "else", "long", "switch", "case",
"enum", "register", "typedef", "char", "extern", "return", "union" , "const", "float", "short",	"unsigned",
"continue",	"for", "signed", "void", "default",	"goto" , "sizeof", "volatile", "do", "if", "static" , "while"};
	
char operators1[] = { '+', '-', '/', '*', '<', '>', '=', '(', ')', ',', '|', '&', '.', '[',']', '!', '^', '%' };
const char *operators2[] = { ">=", "<=", "<<", ">>", "&&", "||", "==", "->", "?:", "!=", "+=", "-=", "/=", "*=", "<<=", ">>=", "&=", "|=", "//", "/*"};

int isOperator(char op) {
	for (int i = 0; i < sizeof(operators1) / sizeof(char); i++) {
		if (op == operators1[i])
			return 1;
	}
	return 0;
}

int is2Operator (const char *key) {
	for (int i = 0; i < sizeof(operators2) / sizeof(const char *); i++) {
		if (!strcmp (operators2[i], key))
			return 1;
	}
	return 0;
}

int isKeyword (const char *key, int len) {
	char *temp = (char *)malloc(sizeof(char) * (len + 1));
	strncpy(temp, key, len);
	temp[len] = '\0';
	
	for (int i = 0; i < sizeof(keywords) / sizeof(const char *); i++) 
		if (!strcmp (keywords[i], temp))
			return 1;

	return 0;
}

int main(void) {
	FILE *fin = fopen("input.txt", "r+"), *fout = fopen("output.txt", "w");
	int line = 1, ptr = -1;
	char curr, lahead, buffer[MAX];
	if (!fin) {
		fprintf(stderr,"Can\'t open the file : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	//Bala Kumar ***
	while(!feof(fin)) {
		// Check for NewLine And WhiteSpace
		for ( ; isspace(curr = getc(fin)) && curr != '\n'; );
		if (curr == '\n') 
			line++;
		else
			ungetc(curr, fin);
		
		//Check for Literals
		for ( ; isdigit(curr = getc(fin)); buffer[++ptr] = curr);
		
		//Check for optional '.'
		if (curr == '.') {
			for ( buffer[++ptr] = curr; isdigit(curr = getc(fin)); buffer[++ptr] = curr);
			fprintf(fout,"%15.*s - %15s - %3d\n", ptr + 1, buffer, "FLOAT LITERAL", line);
			ptr = -1;
			
		} else {
			ungetc(curr, fin);
			if (ptr >= 0) {
				fprintf(fout,"%15.*s - %15s - %3d\n", ptr + 1, buffer, "INTEGER LITERAL", line);
				ptr = -1;
			}
		}
		
		//Check for String Literal
		if ((curr = getc(fin)) == '\"') {
			buffer[++ptr] = curr;
			for ( ; (curr = getc(fin)) != '\"' ; buffer[++ptr] = curr);
			buffer[++ptr] = curr;
			fprintf(fout, "%15.*s - %15s - %3d\n", ptr + 1, buffer, "STRING LITERAL", line);
			ptr = -1;
		} else {
			ungetc(curr, fin);
		}
		
		//Check for keywords or identifier
		for( ;isalpha(curr = getc(fin)); buffer[++ptr] = curr);
		ungetc(curr, fin);
		 
		if ( ptr >= 0 ) {
			if (isKeyword(buffer, ptr + 1)) {
				fprintf(fout,"%15.*s - %15s - %3d\n", ptr + 1, buffer, "KEYWORD", line);
				ptr = -1;
			}
			else {
				for ( ;isalnum(curr = getc(fin)); buffer[++ptr] = curr);
				ungetc(curr, fin);
				fprintf(fout,"%15.*s - %15s - %3d\n", ptr + 1, buffer, "IDENTIFIER", line);
				ptr = -1;
			} 
		}
		
		//Check for operators 
		if(isOperator(curr = getc(fin)) || curr == '?') {
			lahead = getc(fin);
			if (isOperator(lahead) || lahead == ':'){ //If the lookahead is also an operator then, this might be a 2Operator
				char tempOp[3];
				tempOp[0] = curr;
				tempOp[1] = lahead;
				tempOp[2] = '\0';
				
				if(is2Operator(tempOp)) { //If its a 2Operator, treat it as one.
					ptr = -1;
					
					//Ignore if Comment Operator (// or /*) found.
					if ( !strcmp(tempOp, "//") ) {
						while ( (curr = getc(fin)) != '\n' );
						ungetc('\n', fin);
					} else if ( !strcmp(tempOp, "/*") ) {
						while ( (curr = getc(fin)) != '*' || (curr = getc(fin)) != '/' ) {
							if (curr == '\n')
								line++;
						}		
					} else {	
						fprintf(fout,"%15s - %15s - %3d\n", tempOp, "OPERATOR", line);
					}
				}	
				else { //Else treat the both as a seperate operator
					ungetc(lahead, fin);
					fprintf(fout,"%15c - %15s - %3d\n", curr, "OPERATOR", line);
					ptr = -1;
				}
			}
			else { //Its not a 2Operator, so undo the getc and print the Operator
				ungetc(lahead, fin);
				fprintf(fout,"%15c - %15s - %3d\n", curr, "OPERATOR", line);
				ptr = -1;
			}
		}
		else
			ungetc(curr, fin);
		
		if ( (curr = getc(fin)) == ';') {
			fprintf(fout,"%15c - %15s - %3d\n", curr, "TERMINATOR", line);
		} else if ( (ungetc(curr, fin), curr = getc(fin)) == '{' || curr == '}' ) {
			fprintf(fout,"%15c - %15s - %3d\n", curr, "BRACES", line);
		}
		else
			ungetc(curr, fin);
	}
	
}
