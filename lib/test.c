#include <stdio.h>
#include "list.h"

void print_list(LIST *list) {
	NODE *tmp_node;
	tmp_node = list->head->next;
	while (tmp_node->next != NULL) {
		printf("[%d] ", *((int *)tmp_node->value));
		tmp_node = tmp_node->next;
	}
	printf("\n");
}

int main (int argc, char **argv) {
	LIST *list = list_create();
	NODE *nodes[10];
	NODE *tmp_node;
	int i, index;

	for (i = 0; i < 10; ++i) {
		nodes[i] = node_create();
		node_create_value_space(nodes[i], sizeof(int));
		*((int *) nodes[i]->value) = i;
		list_insert(list, nodes[i]);
	}

	printf("=========================\n");
	print_list(list);
	printf("=========================\n");
	printf("object remove: %d\n", list_remove(list, NULL, 3));
	print_list(list);
	printf("=========================\n");
	tmp_node = list_get(list, 5);
	printf("object remove by node: %d\n", list_remove(list, nodes[5], -1));
	printf("target node: %p\n", nodes[5]);
	printf("deleted node: %p\n", tmp_node);
	print_list(list);
}
