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

int main(int argc, const char *argv[])
{
	printf("Hello world\n");

	uint size = 419999;
	htab_t table = htab_create_alloc(size, hash, Node_eq, key_del, calloc, free);

	printf("Inserting 42 under hello...\n");
	//printf("hash of 'hello': %u\n", htab_hash_string("hello"));
	Node *hello_node = Node_new("hello");
	void **slot = htab_find_slot(table, hello_node, INSERT);
	if (*slot == NULL)
	{
		hello_node->count = 42;
		*slot = hello_node;
	}

	printf("Inserting 9000 under world...\n");
	Node *world_node = Node_new("world");
	slot = htab_find_slot(table, world_node, INSERT);
	if (*slot == NULL)
	{
		world_node->count = 9000;
		*slot = world_node;
	}

	printf("Reading back values...\n");
	Node *hello_key = Node_new("hello");
	Node *hello = (Node *) htab_find(table, hello_key);
	printf("hello: %lu\n", hello->count);

	return 0;
}
