// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 27;

// Number of words loaded from dictionary file
int dictsize;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Obtain hash value of word to get bucket it is stored in
    int bucket = hash(word);

    //Get pointer to track nodes in linked list and set to start of appropriate index in hash table
    node *crsr = NULL;
    crsr = table[bucket];

    while (crsr != NULL)
    {
        if (strcasecmp(crsr->word, word) == 0)
        {
            return true;
        }
        crsr = crsr->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    /*This is a very basic hash function, may revisit in the future to improve.
    27 buckets, sorting by the first letter of the word, with a space for 'others'*/

    // make hash function case insensitive
    int letter = tolower((int)word[0]);

    // default bucket is 0, ie if the first char is not a letter
    int bucket = 0;

    // if the letter is a letter from a to z
    if (letter > 96 && letter < 123)
    {
        // a goes to 1, z goes to 27 etc
        bucket = letter - 96;
    }
    return bucket;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Reset count of words in dictionary
    dictsize = 0;

    // Open dictionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // Initialise first word in dict
    char *dword = malloc(LENGTH * sizeof(char));
    fscanf(dict, "%s", dword);
    // check word has loaded
    if (dword == NULL)
    {
        return false;
    }

    do
    {
        //create & allocate memory for node
        node *n = malloc(sizeof(node));
        n->next = NULL;

        // assign node word
        strcpy(n->word, dword);

        //hash word to get bucket
        int bucket = hash(dword);

        // if bucket is empty, assign bucket to node
        if (table[bucket] == NULL)
        {
            table[bucket] = n;
        }
        else
        {
            // insert node into first position in bucket
            // assign node->next to appropriate bucket, and then start of bucket to node->word
            n->next = table[bucket]->next;
            table[bucket]->next = n;
        }

        // add one to count
        dictsize++;
    }
    while (fscanf(dict, "%s", dword) != EOF);

    free(dword);
    fclose(dict);
    // return true if file loaded
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dictsize;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // For each bucket in hash table
    for (int index = 0; index < N; index++)
    {
        // initialise cursor to start of linked list
        node *cursor = table[index];

        //for (node *cursor = table[index]; cursor != NULL; cursor = cursor->next)
        while (cursor != NULL)
        {
            // assign tmp to same as cursor
            node *tmp = cursor;

            // move cursor to next item
            cursor = cursor->next;

            //free tmp
            free(tmp);
        }
    }
    return true;
}
