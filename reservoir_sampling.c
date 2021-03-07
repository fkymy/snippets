#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node
{
	int key;
	struct node *next;
} node;

void reservoir_sampling(node *head)
{
	int result;
	node *current;
	int n;

	if (head == NULL)
		return ;
	srand(time(NULL));
	result = head->key;
	current = head;
	n = 2;
	while (current != NULL)
	{
		if (rand() % n == 0)
			result = current->key;
		current = current->next;
		n++;
	}
	printf("randomly selected key %d\n", result);
}

void push(node **head_ref, int key)
{
	node *n = (node *)malloc(sizeof(node));
	n->key = key;
	n->next = (*head_ref);
	(*head_ref) = n;
}

int main()
{
	node *head = NULL;
	push(&head, 5);
	push(&head, 10);
	push(&head, 3);
	push(&head, 8);
	push(&head, 20);

	reservoir_sampling(head);
	return (0);
}
