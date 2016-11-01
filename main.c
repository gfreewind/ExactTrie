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
	"ikuai8",
	"test12",
	"tesa",
	"tesa123",
	"tesa12",
	"xyz",
	"xyz1",
	"xyz2",
	"xyz3",
	"xyz12",
	"xyz13",
	"xyz21",
	"xyz23",
	"xyz123",
	"xyz132",
	
};

static const char *part_pattern[] = {
	"tes",
	"net",
	"ab",
	"yz",
};

static const char *match_str[] = {
	"test",
	"we",
	"internet",
	"baby",
	"ikuai8",
	"tes",
	"net",
	"ab",
	"test12",
	"tesa",
	"tesa123",
	"tesa12",
	"xyz",
	"xyz1",
	"xyz2",
	"xyz3",
	"xyz12",
	"xyz13",
	"xyz21",
	"xyz23",
	"xyz123",
	"xyz132",
};

static const char *cont_match_str = "xyz123";
static int cont_match_times = 4;

static const char *prefix_match_str[] = {
	"test",
	"we",
	"internet",
	"baby",
	"ikuai8",
	"tes",
	"net",
	"ab",
	"test12",
	"tesa",
	"tesa123",
	"tesa12",
	"test1",
	"testa12345",
	"abc",
	"netb",
};

static const char *no_match_str[] = {
	"te",
	"ter",
	"ikuai",
	"hiwe",
	"weare",
	"aby",
	"test123"
};

static const char *prefix_no_match_str[] = {
	"t",
	"e",
	"adfsdfabc",
	"bab",
	"baba1",
};

#define ARRAY_SIZE(array)	(sizeof(array)/sizeof(array[0]))

int main(void)
{
	struct exact_trie *trie;
	struct exact_match match;
	int ret, i;

	trie = exact_trie_create();
	if (!trie) {
		fprintf(stderr, "Fail to create trie\n");
		exit(1);
	}

	for (i = 0; i < ARRAY_SIZE(pattern); ++i) {
		ret = exact_trie_add(trie, pattern[i], strlen(pattern[i]), (void*)(long)(i+1));
		if (ret != TRIE_STATUS_OK) {
			fprintf(stderr, "TestCase1: unexpected ret(%d) of string(%s)\n", 
				ret, pattern[i]);
			exit(1);
		}
	}
	fprintf(stdout, "TestCase1: Pass inserting multiple patterns\n");

	for (i = 0; i < ARRAY_SIZE(pattern); ++i) {
		ret = exact_trie_add(trie, pattern[i], strlen(pattern[i]), (void*)(long)i);
		if (ret != TRIE_STATUS_DUP_STR) {
			fprintf(stderr, "TestCase2: unexpected ret(%d) of string(%s)\n", 
				ret, pattern[i]);
			exit(1);
		}
	}
	fprintf(stdout, "TestCase2: Pass forbid duplicated patterns test\n");

	for (i = 0; i < ARRAY_SIZE(part_pattern); ++i) {
		ret = exact_trie_add(trie, part_pattern[i], strlen(part_pattern[i]), (void*)(long)(100+i+1));
		if (ret != TRIE_STATUS_OK) {
			fprintf(stderr, "TestCase3: unexpected ret(%d) of string(%s)\n", 
				ret, part_pattern[i]);
			exit(1);
		}
	}
	fprintf(stdout, "TestCase3: Pass inserting part patterns\n");

	exact_trie_finalize(trie);

	exact_trie_dump(trie);

	memset(&match, 0, sizeof(match));
	match.match_mode = TRIE_MODE_EXACT_MATCH;

	for (i = 0; i < ARRAY_SIZE(match_str); ++i) {
		ret = exact_trie_search(trie, match_str[i], strlen(match_str[i]), &match);
		if (ret != TRIE_STATUS_OK) {
			fprintf(stderr, "TestCase4: unexpected ret(%d) of string(%s)\n", 
				ret, match_str[i]);
			exit(1);
		}
		fprintf(stdout, "\tThe exact match result of \"%s\" is ", match_str[i]);
		exact_trie_match_show(&match);
	}
	fprintf(stdout, "TestCase4: Pass match pattern\n");

	for (i = 0; i < ARRAY_SIZE(no_match_str); ++i) {
		ret = exact_trie_search(trie, no_match_str[i], strlen(no_match_str[i]), &match);
		if (ret != TRIE_STATUS_NO_EXIST) {
			fprintf(stderr, "TestCase5: unexpected ret(%d) of string(%s)\n", 
				ret, no_match_str[i]);
			exit(1);
		}
	}
	fprintf(stdout, "TestCase5: Pass no_match pattern\n");

	match.match_mode = TRIE_MODE_PREFIX_MATCH;
	for (i = 0; i < ARRAY_SIZE(prefix_match_str); ++i) {
		ret = exact_trie_search(trie, prefix_match_str[i], strlen(prefix_match_str[i]), &match);
		if (ret != TRIE_STATUS_OK) {
			fprintf(stderr, "TestCases6: unexpectd ret(%d) of string(%s)\n",
				ret, prefix_match_str[i]);
			exit(1);
		}		
		fprintf(stdout, "\tThe prefix match result of \"%s\" is ", prefix_match_str[i]);
		exact_trie_match_show(&match);
	}
	fprintf(stdout, "TestCase6: Pass prefix_match\n");

	for (i = 0; i < ARRAY_SIZE(prefix_no_match_str); ++i) {
		ret = exact_trie_search(trie, prefix_no_match_str[i], strlen(prefix_no_match_str[i]), &match);
		if (ret != TRIE_STATUS_NO_EXIST) {
			fprintf(stderr, "TestCases7: unexpected ret(%d) of string(%s)\n",
				ret, prefix_no_match_str[i]);
			exit(1);
		}
	}
	fprintf(stdout, "TestCase7: Pass prefix_no_match\n");

	int match_times = 0;
	memset(&match, 0, sizeof(match));
	match.match_mode = TRIE_MODE_PREFIX_MATCH;
	match.cont_match = 1;
	while ((ret = exact_trie_search(trie, cont_match_str, strlen(cont_match_str), &match)) == TRIE_STATUS_OK) {
		match_times++;
		fprintf(stdout, "The prefix match result of \"%s\" is ", cont_match_str);
		exact_trie_match_show(&match);
	}
	if (match_times != cont_match_times) {
		fprintf(stderr, "TestCase8: Continue match only matches %u time, epxect %u\n",
			match_times, cont_match_times);
		exit(1);
	}
	fprintf(stdout, "TestCase8: Pass the cont_match\n");

	exact_trie_destroy(trie);


	fprintf(stdout, "\n\n\nPassed all test cases!!!\n\n\n");

	return 0;
}


