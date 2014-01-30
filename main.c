#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "hashtab.h"

typedef struct Node
{
	char *str;
	size_t count;
} Node;

Node *Node_new(const char *s)
{
	Node *node = malloc(sizeof(Node));
	node->str = (char *) malloc(strlen(s) + 1);
	strcpy(node->str, s);

	return node;
}

static void Node_delete(Node *node)
{
	free(node->str);
	free(node);
}

static int Node_eq(const void *entry, const void *element)
{
	return strcmp(((const Node *) entry)->str, ((const Node *) element)->str) == 0;
}

static void key_del(void *key)
{
	Node_delete((Node *) key);
}

static hashval_t hash(const void *node)
{
	return htab_hash_string(((Node *) node)->str);
}

static void add(htab_t table, const char *str, size_t count)
{
	Node *node = Node_new(str);
	void **slot = htab_find_slot(table, node, INSERT);

	if (slot != NULL) {
		node->count = count;
		*slot = node;
	}
}

size_t get(htab_t table, const char *str)
{
	Node *key = Node_new(str);
	Node *node = (Node *) htab_find(table, key);

	if (node == NULL) {
		return -1;
	}

	return node->count;
}

int main(int argc, const char *argv[])
{
	printf("Hello world\n");

	uint size = 419999;
	htab_t table = htab_create_alloc(size, hash, Node_eq, key_del, calloc, free);

	printf("Inserting 42 under hello...\n");
	add(table, "hello", 42);

	printf("Inserting 9000 under world...\n");
	add(table, "world", 9000);

	printf("Reading back values...\n");
	printf("hello: %lu\n", get(table, "hello"));
	printf("world: %lu\n", get(table, "world"));
	printf("foo: %lu\n", get(table, "foo"));
	printf("bar: %lu\n", get(table, "bar"));

	return 0;
}
