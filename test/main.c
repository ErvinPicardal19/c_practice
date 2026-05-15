#include <stdio.h>
#include <stdint.h>

#define offsetof(type, member) ((size_t)&(((type *)0)->member))
#define container_of(ptr, t, m) (t *)(((uint8_t *)ptr) - offsetof(t, m))

#define list_for_each_entry()

struct list
{
	struct list *next, *prev;
};

struct foo
{
	char firstName[24];
	char lastName[24];
	struct list head;
};

struct list foos = { &lists, &lists };

void addToList(struct list *head, struct list *new)
{
	new->next = head->next;
	new->prev = head;
	head->next->prev = new;
	head->next = new;
}

int main(void)
{
	struct list *temp = NULL;
	struct foo *ptr = NULL;
	struct foo newFoo = {"Ervin", "Picardal", {&newFoo.head, &newFoo.head}};

	addToList(&foos, &newFoo.head);

	//ptr = container_of(foos.next, struct foo, head);



	return 0;
}
