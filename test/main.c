#include <stdio.h>
#include <stdint.h>

#define offsetof(type, member) ((size_t)&(((type *)0)->member))

struct head
{
	struct head *next, *prev;
};

struct foo
{
	char firstName[24];
	char lastName[24];
	struct head h;
};

struct head lists = { &lists, &lists };

void addToList(struct head *h, struct head *n)
{
	n->next = h->next;
	n->prev = h;
	h->next->prev = n;
	h->next = n;
}

int main(void)
{
	struct head *temp = NULL;
	struct foo *ptr = NULL;
	struct foo newFoo = {"Ervin", "Picardal", {&newFoo.h, &newFoo.h}};

	addToList(&lists, &newFoo.h);

	printf("Offset: %lu\n", offsetof(struct foo, h));
	printf("lists.next: %p\n", lists.next);
	printf("newFoo.h: %p\n", &newFoo.h);
	ptr = (struct foo *)(((uint8_t *)lists.next) - offsetof(struct foo, h));

	printf("newFoo: %s\n", ptr->firstName);


	return 0;
}
