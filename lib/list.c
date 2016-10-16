#include "list.h"

#ifndef string
#define string
#include <string.h>
#endif

#ifndef stdlib
#define stdlib
#include <stdlib.h>
#endif

LIST* list_create() {
	LIST *list = (LIST *) malloc(sizeof(LIST));
	list_init(list);
	return list;
}

void list_init(LIST *list) {
	NODE *head = node_create();
	NODE *tail = node_create();
	head->prev = NULL;
	head->next = tail;
	head->value = NULL;
	tail->prev = head;
	tail->next = NULL;
	tail->value = NULL;
	list->head = head;
	list->tail = tail;
	list->size = 0;
}

int list_insert_by_value(LIST *list, void *value, int value_size) {
	NODE *new_node = node_create();
	NODE *tmp_node;
	new_node->value = value;
	new_node->value_size = value_size;

	tmp_node = list->tail->prev;
	list->tail->prev = new_node;
	new_node->prev = tmp_node;
	new_node->next = list->tail;
	tmp_node->next = new_node;

	return (list->size)++;
}

int list_insert(LIST *list, NODE *node) {
	NODE *tmp_node;

	tmp_node = list->tail->prev;
	list->tail->prev = node;
	node->prev = tmp_node;
	node->next = list->tail;
	tmp_node->next = node;

	return (list->size)++;	
}

int list_remove(LIST *list, NODE *node, int index) {
	int i;
	int list_size = list->size;
	NODE *tmp_node, *prev, *next;

	if (node != NULL) {
		tmp_node = list->head->next;
		i = 0;
		while (tmp_node->next != NULL) {
			if (tmp_node == node) {
				tmp_node->prev->next = tmp_node->next;
				tmp_node->next->prev = tmp_node->prev;
				free(tmp_node->value);
				free(tmp_node);
				list->size--;
				return i;
			}
			tmp_node = tmp_node->next;
			++i;
		}
		return -1;
	}
	else {
		if (index >= list_size || index < 0) {
			return -1;
		}
		tmp_node = list_get(list, index);
		tmp_node->prev->next = tmp_node->next;
		tmp_node->next->prev = tmp_node->prev;
		free(tmp_node->value);
		free(tmp_node);
		list->size--;
		return index;
	}
}

NODE* list_get(LIST *list, int index) {
	int i;
	NODE *tmp_node;
	if (index > list->size || index < -1) {
		return NULL;
	}
	if (index == -1) {
		return list->head;
	}
	else if (index == list->size) {
		return list->tail;
	}
	if (index < list->size / 2) {
		tmp_node = list->head;
		for (i = 0; i <= index; ++i) {
			tmp_node = tmp_node->next;
		}
		return tmp_node;
	}
	else {
		tmp_node = list->tail;
		for (i = list->size - 1; i >= index; --i) {
			tmp_node = tmp_node->prev;
		}
		return tmp_node;
	}
}

int list_search(LIST *list, void *value, int size) {
	NODE *tmp_node;
	int index = 0;

	tmp_node = list->head->next;
	while (tmp_node->next != NULL) {
		if (!memcmp(tmp_node->value, value, size)) {
			return index;
		}
		tmp_node = tmp_node->next;
		++index;
	}
	return -1;
}

NODE* node_create() {
	NODE *node = (NODE *) malloc(sizeof(NODE));
	memset(node, 0, sizeof(NODE));
	return node;
}

void* node_create_value_space(NODE *node, int value_size) {
	node->value = (void *) malloc(value_size);
	node->value_size = value_size;
	return node->value;
}