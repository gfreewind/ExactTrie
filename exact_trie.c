/*
 * =====================================================================================
 *
 *       Filename:  exact_trie.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/28/2016 11:16:56 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gao Feng(gfree.wind@gmail.com) 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "exact_trie.h"

#define EXACT_TRIE_DEBUG
#define TRIE_MAX_STR_LEN	(1024)
/*******************************************************************************************/
enum {
	TRIE_STRING_END = 0x01,
};

struct trie_node;
struct trie_child {
	struct trie_node *nodes;
	int node_cnt;
	int capacity;
};

struct trie_node {
	struct trie_child child;
	char alpha;
	unsigned int flags;
};

#ifdef EXACT_TRIE_DEBUG
static unsigned int exact_trie_malloc_cnt = 0;
static unsigned int exact_trie_free_cnt = 0;
#endif

static struct trie_node *find_trie_node(const struct trie_child *child, const char *str, int len);
static int insert_trie_node(struct trie_child *child, const char *str, int len);
static void finalize_trie_node(struct trie_child *child);
static int compare_trie_node(const void *n1, const void *n2);
static void free_trie_node(struct trie_child *child);
static int search_trie_child(const struct trie_child *child, const char *str, int len);

static void * trie_malloc(unsigned int size);
static void trie_free(void *ptr);

static void dump_trie_childs(struct trie_child *child, char *str, int index);
static void dump_trie_node(struct trie_node *n, char *str, int index);

/*******************************************************************************************/
struct exact_trie *exact_trie_create(void)
{
	struct exact_trie *root  = trie_malloc(sizeof(*root));

	if (root) {
		memset(root, 0, sizeof(*root));

		root->child = trie_malloc(sizeof(*root->child));
		if (!root->child) {
			trie_free(root);
			return NULL;
		}
	}

	return root;
}

int exact_trie_add(struct exact_trie *exact_trie, const char *str, int len)
{
	struct trie_node *node;

	if (!len) {
		return TRIE_STATUS_EMPTY_STR;
	}

	if (len >= TRIE_MAX_STR_LEN) {
		return TRIE_STATUS_TOO_LONG_STR;
	}

	node = find_trie_node(exact_trie->child, str, len);
	if (node) {
		if (node->flags & TRIE_STRING_END) {
			return TRIE_STATUS_DUP_STR;
		} else {
			node->flags |= TRIE_STRING_END;
			return TRIE_STATUS_OK;
		}
	}

	return insert_trie_node(exact_trie->child, str, len);
}

void exact_trie_finalize(struct exact_trie *trie)
{
	finalize_trie_node(trie->child);
}

void exact_trie_destroy(struct exact_trie *trie)
{
	free_trie_node(trie->child);
	trie_free(trie->child);
	trie_free(trie);

#ifdef EXACT_TRIE_DEBUG
	fprintf(stderr, "ExactTrie: Alloc(%u) Free(%u)\n", 
		exact_trie_malloc_cnt, exact_trie_free_cnt);
#endif
}

int exact_trie_search(const struct exact_trie *trie, const char *str, int len)
{
	if (len == 0) {
		return TRIE_STATUS_EMPTY_STR;
	}

	if (trie->child->node_cnt) {
		return search_trie_child(trie->child, str, len);
	} else {
		return TRIE_STATUS_NO_EXIST;
	}
}

void exact_trie_dump(const struct exact_trie *trie)
{
	char str[TRIE_MAX_STR_LEN];

	dump_trie_childs(trie->child, str, 0);
}

static void dump_trie_node(struct trie_node *n, char *str, int index)
{
	str[index] = n->alpha;
	index++;

	if (n->flags & TRIE_STRING_END) {
		str[index] = '\0';
		fprintf(stdout, "%s\n", str);
	}

	dump_trie_childs(&n->child, str, index);
}

static void dump_trie_childs(struct trie_child *child, char *str, int index)
{
	int i;

	for (i = 0; i < child->node_cnt; ++i) {
		dump_trie_node(child->nodes+i, str, index);
	}
}

static struct trie_node *find_trie_node(const struct trie_child *child, const char *str, int len)
{
	int i;

	for (i = 0; i < child->node_cnt; ++i) {
		if (child->nodes[i].alpha == str[0]) {
			if (len == 1) {
				return &child->nodes[i];
			} else {
				return find_trie_node(&child->nodes[i].child, str+1, len-1);
			}
		}
	}

	return NULL;
}


static int insert_trie_node(struct trie_child *child, const char *str, int len)
{
	struct trie_node *nn;
	int i;
	
	for (i = 0; i < child->node_cnt; ++i) {
		if (child->nodes[i].alpha == str[0]) {
			if (len == 1) {
				assert(!(child->nodes[i].flags&TRIE_STRING_END));
				child->nodes[i].flags |= TRIE_STRING_END;
				return TRIE_STATUS_OK;
			} else {
				return insert_trie_node(&child->nodes[i].child, str+1, len-1);
			}
		}
	}

	if (child->node_cnt == child->capacity) {
		struct trie_node * new_nodes;
		int new_capacity;

		new_capacity = child->capacity * 2;
		if (!new_capacity) {
			new_capacity = 4;
		}
		
		new_nodes = trie_malloc(sizeof(*new_nodes)*new_capacity);
		if (!new_nodes) {
			return TRIE_STATUS_MEM_FAILED;
		}
		memset(new_nodes, 0, sizeof(*new_nodes)*new_capacity);

		if (child->node_cnt) {
			memcpy(new_nodes, child->nodes, sizeof(*new_nodes)*child->node_cnt);
			trie_free(child->nodes);
		}
		child->nodes = new_nodes;
		child->capacity = new_capacity;
	}
	nn = &child->nodes[child->node_cnt];
	child->node_cnt++;
	
	nn->alpha = str[0];

	if (len == 1) {
		nn->flags |= TRIE_STRING_END;
		return TRIE_STATUS_OK;
	} else {
		return insert_trie_node(&nn->child, str+1, len-1);
	}
}

static void finalize_trie_node(struct trie_child *child)
{
	int i;

	if (child->node_cnt > 1) {
		qsort(child->nodes, child->node_cnt, sizeof(*child->nodes),
			compare_trie_node);
	}

	for (i = 0; i < child->node_cnt; ++i) {
		finalize_trie_node(&child->nodes[i].child);
	}
}

static int compare_trie_node(const void *n1, const void *n2)
{
	const struct trie_node *tn1 = n1;
	const struct trie_node *tn2 = n2;

	if (tn1->alpha < tn2->alpha) {
		return -1;
	} else if (tn1->alpha > tn2->alpha) {
		return 1;
	}

	// Should not reach here
	assert(0);
	return 0;
}

static void free_trie_node(struct trie_child *child)
{
	int i;

	for (i = 0; i < child->node_cnt; ++i) {
		free_trie_node(&child->nodes[i].child);
	}

	trie_free(child->nodes);
}

static int compare_char_with_node(const void *key, const void *n)
{
	const char *chr = key;
	const struct trie_node *tn = n;

	return (*chr - tn->alpha);
} 

/*
Make sure the node_cnt of child is positive
*/
static int search_trie_child(const struct trie_child *child, const char *str, int len)
{
	struct trie_node *n = NULL;

	if (child->node_cnt > 1) {
		n = bsearch(str, child->nodes, child->node_cnt, sizeof(*child->nodes),
			compare_char_with_node);
	} else {
		if (child->nodes[0].alpha == str[0]) {
			n = child->nodes;
		}
	}

	if (n) {
		if (len == 1) {
			if (n->flags & TRIE_STRING_END) {
				return TRIE_STATUS_OK;
			}
		} else {
			if (n->child.node_cnt) {
				return search_trie_child(&n->child, str+1, len-1);
			}
		}
	}

	return TRIE_STATUS_NO_EXIST;
}


static void * trie_malloc(unsigned int size)
{
#ifdef EXACT_TRIE_DEBUG
	exact_trie_malloc_cnt++;
#endif
	return malloc(size);
}

static void trie_free(void *ptr)
{
	if (ptr) {
		free(ptr);
#ifdef EXACT_TRIE_DEBUG
		exact_trie_free_cnt++;
#endif
	}
}


