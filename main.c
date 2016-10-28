/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Test routine
 *
 *        Version:  1.0
 *        Created:  10/28/2016 11:14:17 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gao Feng (gfree.wind@gmail.com) 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "exact_trie.h"

static const char *pattern[] = {
	"test",
	"we",
	"internet",
	"baby",
	"ikuai8"
};

static const char *part_pattern[] = {
	"tes",
	"net",
	"ab",
};

#define ARRAY_SIZE(array)	(sizeof(array)/sizeof(array[0]))

int main(void)
{
	struct exact_trie *trie;
	int ret, i;

	trie = exact_trie_create();
	if (!trie) {
		fprintf(stderr, "Fail to create trie\n");
		exit(1);
	}

	for (i = 0; i < ARRAY_SIZE(pattern); ++i) {
		ret = exact_trie_add(trie, pattern[i], strlen(pattern[i]));
		if (ret != TRIE_STATUS_OK) {
			fprintf(stderr, "TestCase1: unexpected ret(%d)\n", ret);
			exit(1);
		}
	}

	for (i = 0; i < ARRAY_SIZE(pattern); ++i) {
		ret = exact_trie_add(trie, pattern[i], strlen(pattern[i]));
		if (ret != TRIE_STATUS_DUP_STR) {
			fprintf(stderr, "TestCase2: unexpected ret(%d)\n", ret);
			exit(1);
		}
	}

	for (i = 0; i < ARRAY_SIZE(part_pattern); ++i) {
		ret = exact_trie_add(trie, part_pattern[i], strlen(part_pattern[i]));
		if (ret != TRIE_STATUS_OK) {
			fprintf(stderr, "TestCase3: unexpected ret(%d)\n", ret);
			exit(1);
		}
	}

	exact_trie_finalize(trie);

	exact_trie_destroy(trie);

	return 0;
}


