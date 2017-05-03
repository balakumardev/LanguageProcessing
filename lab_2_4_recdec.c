/*

	Program: Recursive Descent Parser for the following grammer.
	
	Grammer:	S-> aAb
				A-> cd/c
				
	By: Bala Kumar (149105348) - CSE 'A'

*/

#include <stdio.h>
#include <stdbool.h>

char string[100], *input;

bool term(char t) {
	if (t == *input++)
		return true;
	return false;
}
bool A1() {
	return term('c') && term('d');
}
bool A2() {
	return term('c');
}
bool A() {
	char *save = input;

	return A1() || (input = save, A2());
}

bool S() {
	return term('a') && A() && term('b');
}
int main() {
	printf("Please enter the input: ");
	scanf("%s", string);

	input = string;

	if (S() && *input == '\0') {
		printf("Sucessfully Parsed\n");
	}
	else {
		printf("Parsing Failed.\n");
	}
}

