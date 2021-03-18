#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Dictionary.h"

const int tableSize = 101;

// Private Types and Functions ------------------------------------------------

// NodeObj type
typedef struct NodeObj{
	char* key;
    char* value;
    struct NodeObj* next;
} NodeObj;

// Node type
typedef NodeObj* Node;

// newNode()
// constructor for the Node type
Node newNode(char* k, char* v){
    Node N = malloc(sizeof(NodeObj));
    N->key = k;
    N->value = v;
    N->next = NULL;
    return N;
}

// freeNode()
// destructor for the Node type
void freeNode(Node* pN){
    if( pN!=NULL && *pN!=NULL ){
       free(*pN);
       *pN = NULL;
    }
}

// freeAllNodes()
// uses recursion to free all the Nodes in list headed by H
void freeAllNodes(Node H){
    if( H!=NULL ){
       freeAllNodes(H->next);
       freeNode(&H);
    }
}

// DictrionaryObj type
typedef struct DictionaryObj{
    Node* table;    // array of linked lists
    int numItems;   // number of pairs in this Dictionary
} DictionaryObj;

// Constructors-Destructors ---------------------------------------------------

// newDictionary()
// Constructor for the Dictionary ADT
Dictionary newDictionary(){
	Dictionary D = malloc(sizeof(DictionaryObj));
    D->table = calloc(tableSize, sizeof(Node));
    D->numItems = 0;

    return D;
}

// freeDictionary()
// Destructor for the Dictionary ADT.
void freeDictionary(Dictionary* pD){
    if(pD != NULL && *pD != NULL){
       makeEmpty(*pD);
       free((*pD)->table);
       free(*pD);
       *pD = NULL;
    }
}

// rotate_left()
// rotate the bits in an unsigned int
unsigned int rotate_left(unsigned int value, int shift) {
    int sizeInBits = 8*sizeof(unsigned int);
    shift = shift & (sizeInBits - 1);
    if ( shift == 0 )
    return value;
    return (value << shift) | (value >> (sizeInBits - shift));
}

// pre_hash()
// turn a string into an unsigned int
unsigned int pre_hash(char* input){
    unsigned int result = 0xBAE86554;
    while (*input){
		result ^= *input++;
		result = rotate_left(result, 5);
    }
	
    return result;
}

// hash()
// turns a string into an int in the range 0 to tableSize-1
int hash(char* key){
	return pre_hash(key)%tableSize;
}

// ADT operations -------------------------------------------------------------

// size()
// Return the number of (key, value) pairs in Dictionary D.
int size(Dictionary D){
    if(D == NULL){
       fprintf(stderr, "Dictionary Error: size() called on NULL Dictionary reference\n");
       exit(EXIT_FAILURE);
    }
	
    return D->numItems;
}

// findKey()
// Returns the Node containing key k, or returns NULL if no such Node exists
Node findKey(Dictionary D, char* k) {
    int n = hash(k);
    Node N = D->table[n];
   
    while(N != NULL){
       if(strcmp(N->key, k) == 0){
          return N;
       }
       N = N->next;
    }
	
    return NULL;
}

// lookup()
// If D contains a pair whose key matches argument k, then return the 
// corresponding value, otherwise return NULL.
char* lookup(Dictionary D, char* k){
	Node N;
    if(D == NULL){
       fprintf(stderr, "Dictionary Error: lookup() called on NULL Dictionary reference\n");
       exit(EXIT_FAILURE);
    }
   
    N = findKey(D, k);
    if(N != NULL){
       return N->value;
    }
    else{
       return NULL;
    }
}

// insert()
// Insert the pair (k,v) into D.
// Pre: lookup(D, k)==NULL (D does not contain a pair whose first member is k.)
void insert(Dictionary D, char* k, char* v){
    Node N;
	int n;
    if(D == NULL){
       fprintf(stderr, "Dictionary Error: calling insert() on NULL Dictionary reference\n");
       exit(EXIT_FAILURE);
    }

    if(findKey(D, k) != NULL){
       fprintf(stderr, "Dictionary Error: cannot insert() duplicate key: \"%s\"\n", k);
       exit(EXIT_FAILURE);
    }
   
    n = hash(k);
   
    N = newNode(k, v);
    N->next = D->table[n];
    D->table[n] = N;
   
    (D->numItems)++;
}

// delete()
// Remove pair whose first member is the argument k from D.
// Pre: lookup(D,k)!=NULL (D contains a pair whose first member is k.)
void delete(Dictionary D, char* k){
    Node N, M, K;
	int n;
	
    if(D == NULL){
       fprintf(stderr, "Dictionary Error: calling delete() on NULL Dictionary reference\n");
       exit(EXIT_FAILURE);
    }
   
    n = hash(k);
    K = findKey(D, k);
   
    if(K == NULL){
       fprintf(stderr, "Dictionary Error: cannot delete() non-existent key: \"%s\"\n", k);
       exit(EXIT_FAILURE);
    }
   
    if(D->table[n] == K){  // Node is at the beginning
       N = D->table[n];
       D->table[n] = D->table[n]->next;
    }
    else{                  // Node is anywhere else
       M = D->table[n];
	   
	   while(M->next != K){
          M = M->next;
       }
	   
       N = M->next;
       M->next = N->next;
    }
	
    N->next = NULL;
    freeNode(&N);
	
    (D->numItems)--;
}

// makeEmpty()
// Reset D to the empty state, the empty set of pairs.
void makeEmpty(Dictionary D){
    if(D == NULL){
       fprintf(stderr, "Dictionary Error: calling makeEmpty() on NULL Dictionary reference\n");
       exit(EXIT_FAILURE);
    }
	
    for(int i = 0; i < tableSize; i++){
       freeAllNodes(D->table[i]);
       D->table[i] = NULL;
    }
	
    D->numItems = 0;
}

// countChars()
// Returns the number of characters in the (key, value) pairs.
int countChars(Dictionary D){
	Node N;
    int count = 0;
	
	for(int i = 0; i < tableSize; i++){
        if(D->table[i] != NULL){
            N = D->table[i];
            while(N != NULL){
                count += strlen(N->key);
                count++;
                count += strlen(N->value);
                count++;
                N = N->next;
            }
        }
    }
	
    return count;   
}

// Other Operations -----------------------------------------------------------

// DictionaryToString()
// Determines a text representation of the current state of Dictionary D. Each 
// (key, value) pair is represented as the chars in key, followed by a space,
// followed by the chars in value, followed by a newline '\n' char. The pairs 
// occur in alphabetical order by key. The function returns a pointer to a char 
// array, allocated from heap memory, containing the text representation 
// described above, followed by a terminating null '\0' char. It is the 
// responsibility of the calling function to free this memory.
char* DictionaryToString(Dictionary D){
	Node N;
	char* str;
	int count;
    if(D == NULL){
       fprintf(stderr, "Dictionary Error: calling DictionaryToString() on NULL Dictionary reference\n");
       exit(EXIT_FAILURE);
    }
	
    count = countChars(D);
    str = calloc(count + 1, sizeof(char));
   
    for(int i = 0; i < tableSize; i++){
       if(D->table[i] != NULL){
          N = D->table[i];
          while(N != NULL){
             strcat(str, N->key);
             strcat(str, " ");
             strcat(str, N->value);
             strcat(str, "\n");
             N = N->next;
          }
       }
    }
    str[count] = '\0';
	
    return str;
}