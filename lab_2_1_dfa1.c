#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int tt[4][2] = { {0, 1}, {2, 3}, {0, 1}, {2, 3} }, state = 0, input = -1;
	
	char binstr[100];

	printf("Enter the binary string : ");
	scanf("%s", binstr);

	while(binstr[++input] != '\0') {
		if (binstr[input] == '0')
			state = tt[state][0];
		else if (binstr[input] == '1')
			state = tt[state][1];
		else if (binstr[input] == '$') {
			state = 1; // Any Non-final state
			break;
		}
		else {
			printf("Invalid input.\n");
			exit(0);
		}
	}

	if (state == 0) {
		printf("Input Accepted.\n");
	}
	else {
		printf("Input Rejected.\n");
	}
}
