#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "trielib.c"
#define OUT_ME "gtfo"
#define	HELP_ME "help"
#define ITERATION "слышь\nне повторяйся, мудила"
#define NONEXISTENT "ты меня не любишь"


/* 	
 *  stat == -1, if the word was used 
 * 	stat == 0, if it is nonexistent word
 *	stat == 1, if the word is available 
 * 	passing — is the amount of words passing 
 *	through this node
 * 	passing == -1, if the last word left a trace
*/		


char first, last, *response, *request;
const char OUT[5] = OUT_ME;
const char HELP[5] = HELP_ME;
trie *tree;


/*******************************************************************/
/* 	main purpose functions	*/

void game();

/* 	controls game process flow */ 
void abort_(const char *s, int i)
{
	printf("%s\n", s);
	if (!i)	{
		game(tree);
	}
	else {
		free(request);
		free(response);
		trie_wipe(tree, NAMES_RANGE);
		exit(0);
	}
}

/*******************************************************************/
/* checking functions */

/* if any fitting words remained returns 1 */ 
int is_existing(char l)
{
	if (tree->next[trie_name_value(l)]->passing > 0) 
		return 1;
	else 
		return 0;
}

/* finds an answer and cuts it out 	*/
void answer(trie *leaf, int i)		
{
	int j = rand() % NAMES_RANGE;
	for (; leaf->next[j]->passing < 1; j = rand() % NAMES_RANGE);

	response[i] = trie_name_char(j);


	if (leaf->next[j]->stat == 1) {
		last = response[i];
		response[i + 1] = '\0';
		printf("%s", response);
		trie_name_resection(tree, response, 0);
		return;
	}
	else
		answer(leaf->next[j], i + 1);
}


/*******************************************************************/
/*		game process	*/

void game() 
{
		/* if all available words for this letter were used */
	if (!is_existing(last))
		abort_("you lose", 1);

	fgets(request, MAX_LINE_LENGTH, stdin);

		/* if input is longer than allowed */
	if (strlen(request) + 1 == MAX_LINE_LENGTH)
		abort_("request error", 1);

	request[strlen(request) - 1] = '\0';

		/*	if exit is requested	*/
	if (strcmp(request, OUT) == 0) 
		abort_("ok", 1); 


		/*	if help is requested	 */
	if (strcmp(request, HELP) == 0) {
		printf("слабак\nты должен был сказать\n");
		response[0] = last;
		answer(tree->next[trie_name_value(last)], 1);


		printf("\nа я говорю\n");

		if (!is_existing(last))
				abort_("ничья", 1);

		response[0] = last;
		answer(tree->next[trie_name_value(last)], 1);
		abort_("", 0);
	}


	/*	if first letter of request doesn't match last response	*/
	if (trie_name_value(request[0]) != trie_name_value(last)) 	
		abort_("ну хоть попытайся", 0);	

							
	switch(trie_name_status(tree, request, 0)) 
	{
			/* if request word was used before	*/
		case -1:
			abort_(ITERATION, 0);

			/* if request word is nonexistent	*/	
		case 0:
			abort_(NONEXISTENT, 0);

			/* if request word is existing	*/		
		case 1:
			first = trie_name_char(trie_name_value(request[strlen(request) - 1]));

			if (!is_existing(first))
				abort_("you won", 1);

			response[0] = first;
			trie_name_resection(tree, request, 0);
			answer(tree->next[trie_name_value(first)], 1);
			abort_("", 0);
	}
}	
/*******************************************************************/

int main()
{
	request = malloc(sizeof(char) * 38);
	response = malloc(sizeof(char) * 38);

	srand(time(NULL));
	tree = trie_name_vocab("cities.txt");

	answer(tree, 0);
	printf("\nтебе на %c\n", last);
	game();
	return 0;
}


