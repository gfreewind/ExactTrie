/*
 * =====================================================================================
 *
 *       Filename:  trie.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/28/2016 11:16:18 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gao Feng (gfree.wind@gmail.com), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef EXACT_TRIE_H_
#define EXACT_TRIE_H_

#define EXACT_TRIE_DEBUG

#define TRIE_MAX_STR_LEN	(1024)

enum {
	TRIE_STATUS_OK,
	TRIE_STATUS_EMPTY_STR,
	TRIE_STATUS_DUP_STR,
	TRIE_STATUS_MEM_FAILED,
	TRIE_STATUS_NO_EXIST,
	TRIE_STATUS_TOO_LONG_STR,
};

enum trie_match_mode {
	TRIE_MODE_EXACT_MATCH,
	TRIE_MODE_PREFIX_MATCH,
};

struct trie_child;
struct exact_trie {
	struct trie_child *child;
};

struct exact_match {
#ifdef EXACT_TRIE_DEBUG
	char result[TRIE_MAX_STR_LEN];
	int len;
#endif
	enum trie_match_mode match_mode;
	int cont_match;
	void *pos;
};

struct exact_trie *exact_trie_create(void);
int exact_trie_add(struct exact_trie *exact_trie, const char *str, int len, void *data);
void exact_trie_finalize(struct exact_trie *trie);
void exact_trie_destroy(struct exact_trie *trie);
int exact_trie_search(const struct exact_trie *trie, const char *str, int len, struct exact_match *match);
void exact_trie_dump(const struct exact_trie *trie);
void exact_trie_match_show(struct exact_match *match);

#endif

