#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_SIZE 256

// Trie structure
struct Trie
{
    struct Trie* children[26];
    int wordCount;
};

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie* newTrie = malloc(sizeof(struct Trie));

    if (newTrie == NULL) {
        return NULL;
    }

    for (int c = 0; c < 26; c++) {
        newTrie->children[c] = NULL;
    }

    newTrie->wordCount = 0;

    return newTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    if (pTrie == NULL) {
        return;
    }

    int wordLength = strlen(word);

    for (int c = 0; c < wordLength; c++) {
        if (pTrie->children[word[c] - 'a'] == NULL) {
            pTrie->children[word[c] - 'a'] = createTrie();
        }

        pTrie = pTrie->children[word[c] - 'a'];
    }

    pTrie->wordCount++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    int wordLength = strlen(word);

    for (int c = 0; c < wordLength && pTrie != NULL; c++) {
        pTrie = pTrie->children[word[c] - 'a'];
    }

    if (pTrie == NULL) {
        return 0;
    } else {
        return pTrie->wordCount;
    }
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL) {
        return NULL;
    }

    for (int c = 0; c < 26; c++) {
        pTrie->children[c] = deallocateTrie(pTrie->children[c]);
    }

    free(pTrie);
    return NULL;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        return -1;
    }

    int dictionarySize = 0;
    fscanf(fp, "%d ", &dictionarySize);

    if (dictionarySize > 256) {
        dictionarySize = 256;
    }

    if (pInWords == NULL) {
        fclose(fp);
        return -1;
    }

    for (int c = 0; c < dictionarySize; c++) {
        pInWords[c] = malloc(sizeof(char) * MAX_WORD_SIZE);
        fscanf(fp, "%s ", pInWords[c]);
    }

    fclose(fp);

    return dictionarySize;
}

int main(void)
{
	char *inWords[256];
	
	// read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}

    // free memory used for read buffer
    for (int c = 0; c < numWords; c++) {
        free(inWords[c]);
    }

	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}

	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}