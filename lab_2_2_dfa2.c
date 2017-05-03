#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int tt[6][2] = { {2, 1}, {1, 1}, {1, 3}, {5, 4}, {5, 4}, {5, 3} }, state = 0, input = -1;
	
	char binstr[100];

	printf("Enter the input : ");
	scanf("%s", binstr);

	while(binstr[++input] != '\0') {
		if (binstr[input] == 'a')
			state = tt[state][0];
		else if (binstr[input] == 'b')
			state = tt[state][1];
		else if (binstr[input] == '$') {
			state = 0;
			break;
		}
		else {
			printf("Invalid input.\n");
			exit(0);
		}
	}

	if (state == 3) {
		printf("Input Accepted.\n");
	}
	else {
		printf("Input Rejected.\n");
	}
}
