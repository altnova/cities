#include <stdio.h>
#include <stdlib.h>
#define NAMES_RANGE 30
#define MAX_LINE_LENGTH 100
#define NUMBERS_RANGE 11
#define MAX_NUMBER_LENGTH 70
#define WORD_RANGE 27
#define MAX_WORD_LENGTH 50

/*
 *	stat shows is there any words/numbers 
 * 	consisting of the current path
 *	passing shows how many words/numbers are
 *	containing current path
 *	next is a pointer to a next step of a path
 */


typedef struct Trie {
	short stat;
	int passing;
	struct Trie **next;		
}	trie;

trie *node;

/* 	deallocates memory of a trie */
void trie_wipe(trie *leaf, int range)
{
	int i;
	if(leaf->next) 
		for (i = 0; i < range; i++) 
			trie_wipe(leaf->next[i], range);
	free(leaf);
}

void abort_(const char *s, int i);

/****************************************************************************************************/
/*			NAMES 																					*/
/****************************************************************************************************/
//
/*******************************************************************/
/* for names filling */

/*	turns symbol into an array index */
int trie_name_value(char c) 
{
	if (c >= 'A' && c <= 'Z')
		return c - 'A';
	if (c >= 'a' && c <= 'z')
		return c - 'a';
	if (c == ' ')
		return 26;
	if (c == '\'' || c == '`')
		return 27;
	if (c == '-' || c == '.')
		return c - 17;
	abort_("input error", 1);
	return -1;
}

/* 	turns array index into a symbol */
char trie_name_char(int n)
{	
	if (n == 26)
		return ' ';
	if (n == 27)
		return '\'';
	if (n == 28 || n == 29)
		return n + 17;
	return n + 'a';
}

/*******************************************************************/
/*for NAMES info and modifying*/

/* 	
 * 	checks a state of a word
 *	returns -1 	if the request was in the vocabulary
 * 	returns	 0  if the request isn't in the vocabulary
 *	returns	 1 	if the request us in the vocabulary	
 * 	-1/0/1 is node->stat 
 */
int trie_name_status(trie *leaf, char *word, int i) 	
{
	node = leaf->next[trie_name_value(word[i])];
	if (node->passing == 0) 
		return 0;

	/* 	if it is a last letter */
	if (word[i + 1] == '\0')	
		return node->stat;
	return trie_name_status(node, word, i + 1);
}

/*	cuts a word out of trie */
void trie_name_resection(trie* leaf, char *word, int i) 
{
	/*	end of recursion */
	if (word[i] == '\0') { 
		leaf->stat = -1;
		return;
	}
	node = leaf->next[(trie_name_value(word[i]))];

	/* 
	 *	the last word of a current path leaves a trace 
	 * as passing == -1								 
	 */

	leaf->passing--;
	if (leaf->passing == 0) 
		leaf->passing = -1;

	trie_name_resection(node, word, i + 1);
}

/*******************************************************************/
/* filling a trie with names*/

/* inserts a word from file into a trie */
void trie_name_insert(trie *leaf, FILE *fp)
{
	int i;
	char c;
	fscanf(fp, "%c", &c);

	if (feof(fp))
		return;
	/*	
	 *	if memory for this node was not allocated	
	 * 	(no words are passing through this node yet) 
	 */

	if(!leaf->next) {	
		leaf->next = malloc(NAMES_RANGE * sizeof(trie *));
		for (i = 0; i < NAMES_RANGE; i++)
			leaf->next[i] = calloc(1, sizeof(trie));
		/* so leaf->next[i]-> next == 0	*/
	}

	leaf->passing++;
	if (c == '\n' || c == '\0') 
		leaf->stat = 1;
	else 
		trie_name_insert(leaf->next[trie_name_value(c)], fp);
}

/* makes a trie  */
trie * trie_name_vocab(char *file_name)
{
	FILE *fp = fopen(file_name, "r");
	trie *tree = calloc(1, sizeof(trie));
	if (!fp) {
		printf("File %s could not be opened for reading", file_name);
		exit(0);
	}

	while(!feof(fp))
		trie_name_insert(tree, fp);

	fclose(fp);
	return tree;
}

/* adding a name from stdin */
void trie_name_insert_std(trie *leaf)
{
	int i;
	char c;
	scanf("%c", &c);

	/*	
	 *	if memory for this node was not allocated	
	 * 	(no words are passing through this node yet) 
	 */

	if(!leaf->next) {	
		leaf->next = malloc(NAMES_RANGE * sizeof(trie *));
		for (i = 0; i < NAMES_RANGE; i++)
			leaf->next[i] = calloc(1, sizeof(trie));
		/* so leaf->next[i]-> next == 0	*/
	}

	leaf->passing++;
	if (c == '\n' || c == '\0') 
		leaf->stat = 1;
	else 
		trie_name_insert_std(leaf->next[trie_name_value(c)]);
}

/*******************************************************************/
//
/****************************************************************************************************/
/*			WORDS ORGANIZE 																			*/
/****************************************************************************************************/
//
/*******************************************************************/
/* for words filling */
int trie_word_value(char c) 
{
	if (c >= 'A' && c <= 'Z')
		return c - 'A';
	if (c >= 'a' && c <= 'z')
		return c - 'a';
	return -1;
}

/* 	turns array index into a symbol */
char trie_word_char(int n)
{	
	return n + 'a';
}

/*******************************************************************/
/*for WORDS info and modifying*/

int trie_word_status(trie *leaf, char *word, int i) 	
{
	node = leaf->next[trie_word_value(word[i])];
	if (node->passing == 0) 
		return 0;

	/* 	if it is a last letter */
	if (word[i + 1] == '\0')	
		return node->stat;
	return trie_word_status(node, word, i + 1);
}

/*	cuts a word out of trie */
void trie_word_resection(trie* leaf, char *word, int i) 
{
	/*	end of recursion */
	if (word[i] == '\0') { 
		leaf->stat = -1;
		return;
	}
	node = leaf->next[(trie_word_value(word[i]))];

	/* 
	 *	the last word of a current path leaves a trace 
	 * as passing == -1								 
	 */

	leaf->passing--;
	if (leaf->passing == 0) 
		leaf->passing = -1;

	trie_word_resection(node, word, i + 1);
}

/*******************************************************************/
/* filling a trie */

/* inserts a word from file into a trie */
void trie_word_insert(trie *leaf, FILE *fp, int j)
{
	int i;
	char c;

	if (feof(fp))
		return;

	fscanf(fp, "%c", &c);

	if (trie_word_value(c) == -1){
		if (j == 0)
			return;

		leaf->stat++;
		return;
	} 

	while (trie_word_value(c) == -1) {
		if (feof(fp))
			return;
		fscanf(fp, "%c", &c);
	}
		
	/*	
	 *	if memory for this node was not allocated	
	 * 	(no words are passing through this node yet) 
	 */
	if(!leaf->next) {	
		leaf->next = malloc(WORD_RANGE * sizeof(trie *));
		for (i = 0; i < WORD_RANGE; i++)
			leaf->next[i] = calloc(1, sizeof(trie));
		/* so leaf->next[i]-> next == 0	*/
	}

	leaf->passing++;
	trie_word_insert(leaf->next[trie_word_value(c)], fp, j + 1);
}

/* makes a trie  */
trie * trie_word_vocab(char *file_name)
{
	FILE *fp = fopen(file_name, "r");
	trie *tree = calloc(1, sizeof(trie));
	if (!fp) {
		printf("File %s could not be opened for reading", file_name);
		exit(0);
	}

	while(!feof(fp)) 
		trie_word_insert(tree, fp, 0);

	fclose(fp);
	return tree;
}

/*	adding a word from stdin */
void trie_word_insert_std(trie *leaf)
{
	int i;
	char c;
	scanf("%c", &c);

	/*	
	 *	if memory for this node was not allocated	
	 * 	(no words are passing through this node yet) 
	 */
	if(!leaf->next) {	
		leaf->next = malloc(WORD_RANGE * sizeof(trie *));
		for (i = 0; i < WORD_RANGE; i++)
			leaf->next[i] = calloc(1, sizeof(trie));
		/* so leaf->next[i]-> next == 0	*/
	}

	leaf->passing++;
	if (trie_word_value(c) == -1) 
		leaf->stat++;
	else 
		trie_word_insert_std(leaf->next[trie_word_value(c)]);
}

/*******************************************************************/
//
/****************************************************************************************************/
/*			NUMBERS 																				*/
/****************************************************************************************************/
//
/*******************************************************************/
/* for filling numbers */

/*	turns number into an array index */
int trie_num_value(char c) 
{
	if (c != '.' && c != ',')
		return c - '0';
	else
		return 10;
	abort_("input error", 1);
	return -1;
}

/* 	turns array index into a number */
char trie_num_char(int n)
{	
	if (n == 10)
		return ',';

	return n + '0';
}

/*******************************************************************/
/*for NUMBERS info and modifying*/

/* 	
 * 	checks a state of a number
 *	returns -1 	if the request was in the vocabulary
 * 	returns	 0  if the request isn't in the vocabulary
 *	returns	 1 	if the request us in the vocabulary	
 * 	-1/0/1 is node->stat 
 */
int trie_num_status(trie *leaf, char *word, int i) 	
{
	node = leaf->next[trie_num_value(word[i])];
	if (node->passing == 0) 
		return 0;

	/* 	if it is a last letter */
	if (word[i + 1] == '\0')	
		return node->stat;
	return trie_num_status(node, word, i + 1);
}

void trie_num_resection(trie* leaf, char *word, int i) 
{
	/*	end of recursion */
	if (word[i] == '\0') { 
		leaf->stat = -1;
		return;
	}
	node = leaf->next[(trie_num_value(word[i]))];

	/* 
	 *	the last word of a current path leaves a trace 
	 * as passing == -1								 
	 */

	leaf->passing--;
	if (leaf->passing == 0) 
		leaf->passing = -1;

	trie_num_resection(node, word, i + 1);
}

/*******************************************************************/
/* filling a trie */
/*NUMBERS*/

/* inserts a word from file into a trie */
void trie_num_insert(trie *leaf, FILE *fp)
{
	int i;
	char c;
	fscanf(fp, "%c", &c);

	if (feof(fp))
		return;
	/*	
	 *	if memory for this node was not allocated	
	 * 	(no words are passing through this node yet) 
	 */
	if(!leaf->next) {	
		leaf->next = malloc(NUMBERS_RANGE * sizeof(trie *));
		for (i = 0; i < NUMBERS_RANGE; i++)
			leaf->next[i] = calloc(1, sizeof(trie));
		/* so leaf->next[i]-> next == 0	*/
	}

	leaf->passing++;
	if (c == '\n' || c == '\0' || c == ' ') 
		leaf->stat = 1;
	else 
		trie_num_insert(leaf->next[trie_num_value(c)], fp);
}

/* makes a trie  */
trie * trie_num_vocab(char* file_name)
{
	FILE *fp = fopen(file_name, "r");
	trie *tree = calloc(1, sizeof(trie));
	if (!fp) {
		printf("File %s could not be opened for reading", file_name);
		exit(0);
	}

	while(!feof(fp))
		trie_num_insert(tree, fp);

	fclose(fp);
	return tree;
}

/*	adding a number from stdin */
void trie_num_insert_std(trie *leaf)
{
	int i;
	char c;
	scanf("%c", &c);

	/*	
	 *	if memory for this node was not allocated	
	 * 	(no words are passing through this node yet) 
	 */
	if(!leaf->next) {	
		leaf->next = malloc(NAMES_RANGE * sizeof(trie *));
		for (i = 0; i < NAMES_RANGE; i++)
			leaf->next[i] = calloc(1, sizeof(trie));
		/* so leaf->next[i]-> next == 0	*/
	}

	leaf->passing++;
	if (c == '\n' || c == '\0') 
		leaf->stat = 1;
	else 
		trie_num_insert_std(leaf->next[trie_num_value(c)]);
}

/*******************************************************************/
//
/*****************************************************************************************************/




















