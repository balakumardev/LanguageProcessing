/*

	Program: To print out the first and follow of the input grammer.

	By: Bala Kumar (149105348) - CSE 'A'

*/
	
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define PLEN 15

/* Linked List */

typedef struct Node{
	char c;
	struct Node *next;
}*LinkedList;

void allocList(LinkedList **head, int n) {
	*head = (LinkedList *)malloc(n * sizeof(LinkedList));
	for (int i = 0; i < n; i++)
		(*head)[i] = NULL;
}

void printList(LinkedList head) {
	if (!head)
		return;
	else {
		for(LinkedList node = head; node; node = node->next)
			printf("-> %c ", node->c);
		printf("\n");
	}
}

LinkedList returnPtr(LinkedList head, char c) {
	if (!head)
		return NULL;
	else if (head->c == c)
		return head;
	else
		return returnPtr(head->next, c);
}

void pop(LinkedList *head, LinkedList node) {
	if (!*head)
		return;
	if (*head == node) {
		*head = (*head)->next;
	} 
	else {
		LinkedList prev;
		for(prev = *head; prev->next != NULL && prev->next != node; prev = prev->next);
		prev->next = node->next;
		free(node);
	}
}
	
void push(LinkedList *head, char c) {
	if (!*head) {
		*head = (LinkedList)malloc(sizeof(LinkedList));
		(*head)->c = c;
		(*head)->next = NULL;
	} else {
		LinkedList newNode = (LinkedList)malloc(sizeof(LinkedList)), tail;
		newNode->c = c;
		newNode->next = NULL;
		
		
		for (tail = *head; tail->c != c && tail->next; tail = tail->next);
		
		if (!tail->next && tail->c != c)
			tail->next = newNode;
		
	}
}

/* Utility Functions */

int isTerminal(char *nonts,int n, char c) {
	if(c == '%')
		return 0;
	for (int i = 0; i < n; i++) {
		if (nonts[i] == c)
			return 0;
	}
	
	return 1;
}

int returnIndex(char *nonts, int n, char c) {
	for (int i = 0; i < n; i++) {
		if (nonts[i] == c)
			return i;
	}
	return -1;
}

//Returns 1 if the given non-terminal produces Lamda (A -*-> %)
int produceLamda(char **P, int n, char *nonts, char c) {
	int index = returnIndex(nonts, n, c);
	char ch;
	int i = 0;
	while ( (ch = P[index][i++]) != '\0' ) {
		if (ch == '%')
			return 1;
	}
	
	i = 5;
	while (P[index][i] != '\0') {
		if(isspace(P[index][i]))
			i++;
		else if (isTerminal(nonts, n, P[index][i])) {
			while (P[index][i] != '\0' && P[index][i++] != '|');
		}
		else {
			return produceLamda(P, n, nonts, P[index][i]);
		}
	}
	return 0;
}

void substNonTs(LinkedList *list, char *nonts, int n) {
	for (int i = 0; i < n; i++) {
		char nont;
		
		do {
			nont = '\0';
			//Check if the list has any non-terminals in it.
			
			for (LinkedList node = list[i]; node; node = node->next) 
				if (!isTerminal(nonts, n, node->c) && node->c != '%') {
					nont = node->c;
					break;
				}
			
			if (nont){
				//Pop the non-terminal from the list
				LinkedList toRem = returnPtr(list[i], nont);
				pop(&list[i], toRem);
				
				//If there's a cycle
				if(nont != nonts[i]) {
					//Then push their Firstlist into the current one.
					int index = returnIndex(nonts, n, nont);
					for(LinkedList Node = list[index]; Node; Node = Node->next) 
						if (Node->c != '%')
							push(&list[i], Node->c);
				}
			}

			
		}while(nont);
	}
}

int main(void) {
	int n, i;
	char **P, *nonts;
	LinkedList *first;
	LinkedList *follow;
	
	printf("Please enter the number of productions in your grammer : ");
	scanf("%d", &n);
	
	//Allocate memory
	P = (char **)malloc(n * sizeof(char *));
	for(int i = 0; i < n; i++)
		P[i] = (char *)malloc(PLEN * sizeof(char));
	nonts = (char *)malloc(n * sizeof(char));
	
	allocList(&first, n);
	allocList(&follow, n);
	
	
	//Input productions.
	fflush(stdin);
	printf("Enter your productions : \n" \
			"[FORMAT : Please seperate the head, arrow, body, and the alternatives using space. Ex : S -> AB | % (Lamda)]\n");
	for (int i = 0; i < n; i++)
		gets(P[i]);
	
	//Obtain non-terminals from the productions
	for (int i = 0; i < n; i++)
		nonts[i] = P[i][0];
	
	//Calculate FIRST of every non-terminal and store it in FIRST[] LinkedList
	for(int i = 0; i < n; i++) {
		int j = 5;
		while (P[i][j] != '\0') {
			if(isspace(P[i][j]) || P[i][j] == '|')
				j++;
			// Push any terminal found as it will be in FIRST(A)
			else if (isTerminal(nonts, n, P[i][j])) { 
				push(&first[i], P[i][j]);
				while (P[i][j] != '\0' && P[i][j++] != '|');
			}
			else if (P[i][j] == '%') {
				push(&first[i], '%');
				while (P[i][j] != '\0' && P[i][j++] != '|');
			}
			else {
				//If non-terminal is found, then add the FIRST(Non-terminal) to the list (will be dealt with on 2nd pass)
				push(&first[i], P[i][j]);
				
				//If the non-terminal produces Lamda, it can disappear. So, look at the next symbol
				if (produceLamda(P, n, nonts, P[i][j])) {
					j++;
					if (isspace(P[i][j]) || P[i][j] == '\0') //If the body of the production is exhausted, and every nonterminal produces Lamda 
						push(&first[i], '%'); // Push Lamda
						
				}
				else  // Else look at the next production
					while (P[i][j] != '\0' && P[i][j++] != '|');
				
			}
		}
	}
	
	//Pass-2 for substituting any Non-terminals in the FIRST lists
	substNonTs(first, nonts, n);
	
	printf("FIRST :\n");
	for(int i = 0; i < n; i++) {
		printf("%c : ", nonts[i]);
		printList(first[i]);
	}
	
	//Calculating FOLLOW
	push(&follow[0], '$');
	
	for(int i = 0; i < n; i++)
		for (int j = 5; P[i][j] != '\0'; j++) 
			if (!isTerminal(nonts, n, P[i][j]) && P[i][j] != '%') {
				int index = returnIndex(nonts, n, P[i][j]);
				
				if (P[i][j + 1] == ' ' || P[i][j + 1] == '\0') 
					push(&follow[index], nonts[i]);
				else if (isTerminal(nonts, n, P[i][j + 1]))
					push(&follow[index], P[i][j + 1]);
				else {
					//Its a non-terminal A. So, push FIRST(A) to the FOLLOW list
					int rememberCurr = j,
							nonTIndex, lamdaFlag = 0;
					do {
						lamdaFlag = 0;
						nonTIndex = returnIndex(nonts, n, P[i][j + 1]);
						
						for(LinkedList Node = first[nonTIndex]; Node; Node = Node->next) {
								if (Node->c == '%')
									lamdaFlag = 1;
								else
									push(&follow[index], Node->c);
						}
						
						if(lamdaFlag) {
							j++;
							if(isTerminal(nonts, n, P[i][j + 1])) {
								if (P[i][j + 1] == ' ' || P[i][j + 1] == '\0' || P[i][j] == '|') 
									push(&follow[index], nonts[i]);
								else
									push(&follow[index], P[i][j + 1]);
								lamdaFlag = 0;
							}
										
						}
						
					} while(lamdaFlag);
					j = rememberCurr;
				}	
			}
	
	substNonTs(follow, nonts, n);
	
	printf("\nFOLLOW :\n");
	for(int i = 0; i < n; i++) {
		printf("%c : ", nonts[i]);
		printList(follow[i]);
	}
} 