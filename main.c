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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

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

#define PERF_TEST_STR_CNT	(1024)
#define PERF_TEST_LOOPS		(100000)

struct str_set {
	char **str;
	int cnt;
};

struct perf_result {
	int match_times;
	int cost_secs;
};

/***************************************************************************************************************/
static void performance_tests_with_iter_memcmp(int str_len, int str_cnt, int test_str_cnt, int loops);
static void generate_random_str(char *buf, int len);
static void create_random_str_set(struct str_set *str_set, int str_len, int str_cnt, struct exact_trie *trie);
static void destroy_random_str_set(struct str_set *str_set);

/***************************************************************************************************************/
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
	
	performance_tests_with_iter_memcmp(2, 8, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(2, 16, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(2, 32, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	
	performance_tests_with_iter_memcmp(4, 8, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(4, 16, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(4, 32, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(4, 64, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(4, 128, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);

	performance_tests_with_iter_memcmp(8, 8, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(8, 16, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(8, 32, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(8, 64, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(8, 128, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);

	return 0;
}

static void performance_tests_with_iter_memcmp(int str_len, int str_cnt, int test_str_cnt, int loops)
{
	struct exact_trie *trie;
	struct str_set pattern_set;
	struct str_set rand_str_set;
	struct perf_result mem_pf, trie_pf;
	int i, j, k;
	time_t start, end;

	memset(&pattern_set, 0, sizeof(pattern_set));

	trie = exact_trie_create();
	if (!trie) {
		fprintf(stderr, "Fail to create exact trie\n");
		exit(1);
	}

	create_random_str_set(&pattern_set, str_len, str_cnt, trie);	
	exact_trie_finalize(trie);

	create_random_str_set(&rand_str_set, str_len, test_str_cnt, NULL);

	memset(&mem_pf, 0, sizeof(mem_pf));
	memset(&trie_pf, 0, sizeof(trie_pf));

	start = time(NULL);
	for (i = 0; i < test_str_cnt; ++i) {
		for (j = 0; j < loops; ++j) {
			for (k = 0; k < str_cnt; ++k) {
				if (0 == memcmp(pattern_set.str[k], rand_str_set.str[i], str_len)) {
					mem_pf.match_times++;
					break;
				}
			}
		}
	}
	end = time(NULL);
	mem_pf.cost_secs = end-start;

	start = time(NULL);
	for (i = 0; i < test_str_cnt; ++i) {
		for (j = 0; j < loops; ++j) {
			struct exact_match match;

			memset(&match, 0, sizeof(match));
			if (TRIE_STATUS_OK == exact_trie_search(trie, rand_str_set.str[i], str_len, &match)) {
				trie_pf.match_times++;
			}
		}
	}
	end = time(NULL);
	trie_pf.cost_secs = end-start;

	destroy_random_str_set(&pattern_set);
	destroy_random_str_set(&rand_str_set);

	fprintf(stdout, "\n");
	fprintf(stdout, "Perf Test: ExactTrie with IterMemcmp\n");
	fprintf(stdout, "Insert %u random strings(length is %u):\n", str_cnt, str_len);
	fprintf(stdout, "Lookup %u random strings(loop %u times):\n", test_str_cnt, loops);
	fprintf(stdout, "IterMemcmp match %u times, cost %d secs\n", mem_pf.match_times, mem_pf.cost_secs);
	fprintf(stdout, "ExactTrie match %u times, cost %d secs\n", trie_pf.match_times, trie_pf.cost_secs);
	fprintf(stdout, "\n");

	exact_trie_destroy(trie);
}

static void create_random_str_set(struct str_set *str_set, int str_len, int str_cnt, struct exact_trie *trie)
{
	int ret, i;
	
	memset(str_set, 0, sizeof(*str_set));

	str_set->str = malloc(sizeof(*str_set->str)*str_cnt);
	if (!str_set->str) {
		fprintf(stderr, "Fail to malloc\n");
		exit(1);
	}
	
	/* prepare the random pattern */
	for (i = 0; i < str_cnt; ++i) {
		str_set->str[i] = malloc(str_len);
		if (!str_set->str[i]) {
			fprintf(stderr, "Fail to malloc\n");
			exit(1);
		}

		if (trie) {
			do {
				generate_random_str(str_set->str[i], str_len);
				ret = exact_trie_add(trie, str_set->str[i], str_len, NULL);
			} while (ret != TRIE_STATUS_OK);
		}
	}
	str_set->cnt = str_cnt;
}

static void destroy_random_str_set(struct str_set *str_set)
{
	int i;

	for (i = 0; i < str_set->cnt; ++i) {
		free(str_set->str[i]);
	}

	free(str_set->str);
}

static void generate_random_str(char *buf, int len)
{
	int fd;

	fd = open("/dev/urandom", O_RDONLY);

	read(fd, buf, len);

	close(fd);
}



