#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int tt[5][2] = { {1, 0}, {1, 2}, {3, 0}, {1, 4}, {4, 4} }, state = 0, input = -1;
	
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

	if (state == 4) {
		printf("Input Accepted.\n");
	}
	else {
		printf("Input Rejected.\n");
	}
}
