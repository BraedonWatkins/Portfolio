#define _CRT_SECURE_NO_WARNINGS

#define CHAR2INT(c) ((int)c - (int)'a')


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

typedef struct node
{
	//Lets get freqie
	int freq, sumFreq, maxFreq;
	struct node* child[26];

}node;

//Debugging tingz (^)>
void printTrie(node* root)
{
	for (int i = 0; i < 26; i++)
	{
		if (root->child[i] != NULL)
		{
			printf("Char: %c, Freq: %d, Sum: %d\n", (i + 'a'), root->child[i]->freq, root->child[i]->sumFreq);
			printTrie(root->child[i]);
		}
	}

}

void InitNode(node* newNode)
{
	//When a new node is made, ensure its child ptrs are null
	for (int i = 0; i < 26; i++)
	{
		newNode->child[i] = NULL;
	}

	//Make all data within a node 0 instead of garbage
	newNode->freq = 0;
	newNode->maxFreq = 0;
	newNode->sumFreq = 0;
}

//Inserts new letters into the trie as requested by query one
node* insertFunction(node* root, char inWord[], int frequency, int* counter)
{
	//When there are no more letters in requested word, return word
	if (*counter == strlen(inWord))
	{
		root->freq += frequency;
		
		return root;
	}
	
	//If there are more letters, determine if it exists in trie or not
	int childComp = CHAR2INT(inWord[*counter]);
	if (root->child[childComp] == NULL)
	{
		//Create new node since the current letter is not in the trie
		node* newNode = malloc(sizeof(node));
		InitNode(newNode);
		root->child[childComp] = newNode;

		//Iterate to next letter and perform recursive call
		(*counter)++;
		insertFunction(newNode, inWord, frequency, counter);
	}
	else
	{
		//Letter already exists in trie, iterate to next letter and  perform recursive call
		(*counter)++;
		insertFunction(root->child[childComp], inWord, frequency, counter);
	}
}

//Given a new node update sumFreq variable for all nodes in the trie
node* sumFreq(node* current, int* sumFrequency)
{
	//Determine for each child ptr if they exist or null
	for (int i = 0; i < 26; i++)
	{
		//If null then sumFreq must be equal to this node's freq
		if (current->child[i] == NULL)
		{
			current->sumFreq = current->freq;
		}
		else
		{
			//If child does exist then drill down until no children left
			sumFreq(current->child[i], sumFrequency);
		}
	}


	//Now that all children sumFreq accounted for update in trie
	for (int i = 0; i < 26; i++)
	{
		//For all children of the current node sumFreq is the total of all chidlrens' sumFreq
		if (current->child[i] != NULL)
		{
			current->sumFreq += current->child[i]->sumFreq;
		}
	}

}

//Create new words as requested by in file
void QueryOne(node* root, char inWord[], int frequency)
{
	//Create int ptr to help insert function, then call it
	int* counter = malloc(sizeof(int));
	*counter = 0;
	node* current = insertFunction(root, inWord, frequency, counter);

	//Create int ptr to help update sumFreq function, call it
	int* sumFrequency = malloc(sizeof(int));
	*sumFrequency = 0;
	sumFreq(root, sumFrequency);

	//Free int ptrs
	free(counter);
	free(sumFrequency);
}

//Given an incomplete word traverse to the last letter node and return it
node* searchIncomplete(node* current, char inWord[], int* counter)
{
	//If we reach the end of our given word this is last letter, return it
	if ((*counter) >= strlen(inWord))
	{
		return current;
	}

	//Determine if next letter exists in the trie
	for (int i = 0; i < 26; i++)
	{
		//If next letter exists in the trie, increment then recursive call
		if (current->child[i] != NULL && (i == CHAR2INT(inWord[*counter])))
		{
			(*counter)++;
			return searchIncomplete(current->child[i], inWord, counter);
		}
	}

	//If we have not reached the end of our word then it's not in the trie, return NULL
	return NULL;
}

//Determine the next letter based on incomplete word, handle edge cases
void QueryTwo(node* root, char inWord[], FILE* out)
{
	//Create an int ptr to assist searchIncomplete then call it
	int* sCounter = malloc(sizeof(int));
	*sCounter = 0;
	node* temp = searchIncomplete(root, inWord, sCounter);

	//Handle depending on if the word was in the trie or not
	if (temp == NULL)
	{
		fprintf(out, "unknown word");
	}
	else
	{
		//Find highest sumFreq amoong children and store it as maxFreq for parent
		for (int i = 0; i < 26; i++)
		{
			if (temp->child[i] != NULL && temp->child[i]->sumFreq > (temp->maxFreq))
			{
				temp->maxFreq = temp->child[i]->sumFreq;
			}
		}

		//Determine which children have priority to be printed
		int j = 0;
		for (int i = 0; i < 26; i++)
		{
			//For all children that meet max freq, print them
			if (temp->child[i] != NULL && temp->child[i]->sumFreq == temp->maxFreq)
			{
				fprintf(out, "%c", (i + 'a'));
			}

			//If no children have priority then we have a complete word but no letters to continue, word unknown
			if (j == 25)
			{
				fprintf(out, "unknown word");
			}
			if (temp->child[i] == NULL)
			{
				j++;
			}
		}
	}

	//Formatting
	fprintf(out, "\n");

}

//This function reads input and calls queries
int DriverFunction(FILE* in, FILE* out, node* root)
{
	//Initializing Trie
	int query;
	char inWord[2000001];
	int frequency;

	//Read input
	fscanf(in, "%d", &query);
	fscanf(in, "%s", inWord);

	//Call query one or query two
	if (query == 1)
	{
		fscanf(in, "%d", &frequency);
		QueryOne(root, inWord, frequency);
	}
	else if (query == 2)
	{
		QueryTwo(root, inWord, out);
	}

}

void freeTree(node* root)
{
	for (int i = 0; i < 26; i++)
	{
		//Loop through all children that exist, until reaching nodes that have no children
		if (root->child[i] != NULL)
		{
			freeTree(root->child[i]);
		}
	}

	//Once at a node with no children, free it. Return and repeat.
	free(root);
}

void main()
{
	atexit(report_mem_leak);

	//Initializing variables
	FILE* in = fopen("in.txt", "r");
	FILE* out = fopen("out.txt", "w");
	int commandNum;
	fscanf(in, "%d", &commandNum);

	node* root = malloc(sizeof(node));
	InitNode(root);

	//Call driver function for each command
	for (int i = 0; i < commandNum; i++)
	{
		DriverFunction(in, out, root);
	}

	//Free memory
	fclose(in);
	fclose(out);
	freeTree(root);

}
