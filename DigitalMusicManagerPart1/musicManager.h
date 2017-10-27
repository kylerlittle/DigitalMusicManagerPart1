/*******************************************************************************
* Programmer: Kyler Little													  *
* Class: CptS 121; Lab Section 6                                              *
* Programming Assignment: Digital Music Manager & Doubly Linked Lists -- P2   *
* Date: 2/10/2017                                                             *
*                                                                             *
* Description: This program manipulates our music collection from a .csv file *
*			   based on attributes such as artist, album title, song title,   *
*			   genre, song length, number times played, and rating. It allows *
*			   the user to perform some basic functions with it and then	  *
*			   save his/her edits.											  *
*                                                                             *
* Relevant Formulas: Refer to each function definition.                       *
*                                                                             *
* Format of record in input file:			  Drake 	(artist string)       *
*                                             Own It	(album string)        *
*                                             Red		(song string)         *
*                                             Pop		(genre string)        *
*											  3:45		(length Duration)     *
*			    							  3			(played unsigned int) *
*			    							  4			(rating int)		  *

*                                                                             *
* Format of output file (output.dat): See problem specification.              *
******************************************************************************/


#ifndef DIGITAL_MUSIC_MANAGER
#define DIGITAL_MUSIC_MANAGER

/* Standard Libraries Included */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Defined Macroconstants */
#define MAX_CHARS_READ 150
#define MAX_ARTIST_NAME 30
#define MAX_SONG_NAME 30
#define MAX_EDIT 30
#define MAX_ENTRY 100
#define MAX_NEW_FIELD 40
#define NUM_SONGS 9
#define PICKED 1

/* Defined Data Types */
typedef enum boolean
{
	FALSE, TRUE
} Boolean;

typedef enum rating
{
	SHIT = 1, BAD, ALRIGHT, GOOD, HOT_DAMN
} Rating;

typedef enum userChoice
{
	LOAD = 1, STORE, DISPLAY, INSERT, DELETE, EDIT, SORT, RATE, PLAY, SHUFFLE, EXIT
} UserChoice;

typedef enum printChoice
{
	ALL = 1, MATCH_ARTIST
} PrintChoice;

typedef enum editChoice
{
	STOP, ARTIST, ALBUM_TITLE, SONG_TITLE, GENRE, SONG_LENGTH, TIMES_PLAYED, RATING
} EditChoice;

typedef enum sortChoice
{
	BY_ARTIST = 1, BY_ALB_TITLE, BY_RATING, BY_TIMES_PLAYED
} SortChoice;

typedef struct duration
{
	unsigned int min;
	unsigned int sec;
} Duration;

typedef struct record
{
	char *artist;
	char *albumTitle;
	char *songTitle;
	char *genre;
	Duration songLength;
	unsigned int timesPlayed;
	Rating rating;
} Record;

typedef struct node
{
	Record data;
	struct node * pNext;
	struct node * pPrev;
} Node;


/**********			Function Prototypes			**********/

//   EXECUTION.C FUNCTIONS
//Description: the overall function that allows the user to navigate thru the menu
void runApp(void);

//Description: If list is empty, FALSE is return. Otherwise, TRUE.
//Requires a pointer to the start of the list
//we are not modifying pList, so only one * is needed.
Boolean isEmpty(Node *pList);

//Description: This function allocates space for a Node on the heap & initializes
//			   its data with newData.
Node * makeNode(Record newData);

//Description: So long as malloc allocated space on the heap for a node, this function
//			   inserts the node at the front of the list. It says pList, the pointer to
//			   the start of the list, to the new front.
Boolean insertFront(Node **pList, Record newData);

//Description: This function sets each entry in line[] to the null character.
//			   used to clear readInLine each time through execution loop!
void clearLine(char line[]);




//   MUSIC_FEATURES_P1.C FUNCTIONS
//Description: this function reads in data lines from "infile," stores each field
//			   from the csv file to its associated field in the node, and then
//			   adds the node to the front of the list. It does include error checks.
Boolean loadRecords(FILE *infile, Node **pList);

//Description: this function simply prints each field in a Record, to the screen.
//			   It starts printing at pList
void printList(Node *pList);

//Description: Because my artistSearch function (below) returns a pointer to the first
//			   occurrence of an artist, I didn't need to check for previous occurrences
//			   in the list. Thus, this function simply prints all records which match
//			   a particular artist name, beginning with *pFirstOcc.
void printArtistMatch(Node *pFirstOcc);

//Description: If artist is found, a pointer to the node is found
//			   If he/she is not found, the null pointer is returned
//			   If found, it is the FIRST instance of the artist
Node * artistSearch(Node *pList, char readInLine[]);

//Description: This function writes the current contents of the list to outfile.
//			   Technically, since it prints from the list start, it will write the
//			   list in the reverse order in which it was obtained.
void storeRecords(FILE *outfile, Node *pList);

//Description: If song is found, a pointer to the node is found
//			   If the song is not found, the null pointer is returned
Node * songSearch(Node *pList, char readInLine[]);

//Precondition: User has already selected a valid song and rated it
//Description: This function merely changes the rating in the List itself.
void rateSong(Node *pSong, Rating rating);

//Preconditions: User has selected which field they would like to edit from a corresponding
//				 artist, which they have also already selected. This function allows the
//				 user to type in what they would like to change a field to, and the function
//				 then carries out that modification.
void editSelection(Node *pEditThis, EditChoice choice);

//Precondition: User selected a song that is in the list
//Description: This function "plays" all songs in the list, starting at the song
//			   they selected.
void play(Node *pList, Node *pSong);




//   MUSIC_FEATURES_P2.C FUNCTIONS
//Description: Requires a pointer to the start of the list and a pointer to the node
//		       wished to be deleted. If the two are the same, then separate measures
//			   are taken. This function deletes the node pointed to be pSong, and adjusts
//			   the list accordingly. 
//Precondition: pSong is not NULL
void deleteRecord(Node *pSong, Node **pList);

// Description: determines the size of a singly linked list; returns the size as an integer
int sizeofList(Node *pList);

// Description: If two nodes need to be switched, TRUE is returned; otherwise, FALSE
//				is returned. It is a helper function for "Sort" since there are multiple
//				options by which we can sort.
Boolean condition(Node *C, SortChoice choice);

// Bubble Sort Algorithm is from Andy's PA #8 in CS 121; I had to make modifications though
// Steps to Swap:
//       (While moving thru the list, have a pBef point to the node before C)
// 1. Connect the surrounding nodes (1st and 4th) if swapping in middle
// 2. Store pPrev & pNext of C into temporary pointers
// 3. Modify C's pPrev & pNext to point to correct locations
// 4. Modify pBefore's pPrev & pNext to the temporary pointers from (2)
// Description: the algorithm is a typical bubble sort; if two nodes need to be swapped,
//				then they are swapped according to the algorithm above. This function
//				should work for all 4 methods beacuse of my helper function above. However,
//				I seem to be having an off-by-one error or a pointer error. My function 
//				works for "artist" but not for the others... even though it should. 
void Sort(Node **pList, SortChoice sort);

//Fill random[] array with integers ranging from 0 to size - 1 with NO REPEATS
void randOrder(int random[], int size);

//Description: Helper function for Shuffle; prints a single record to the screen
void playSingleRecord(Node *pPlay);

//Description: "Shuffles" the songs! It plays the songs until all songs have been
//				played. Songs are played accorded to my random integer array
//				generated by "randOrder" function
void Shuffle(int randOrder[], int size, Node *pList);




//   USER_INTERFACE.C FUNCTIONS
void printMainMenu(void);

//Description: Modifies choice to an integer between 1 and 11, inclusive
void getChoice(UserChoice *choice);

//Description: Prints off a "print menu." I.e. print all? or print from an artist?
void printPrintMenu(void);

//Description: Modifies choice to an integer, 1 or 2
void getPrintChoice(PrintChoice *choice);

//Description: Prompts user to enter an artist's name; reads the string
//			   of characters into readInLine character array
void getArtist(char readInLine[]);

//Description: Prompts user to enter a song name; reads the string
//			   of characters into readInLine character array
void getSong(char readInLine[]);

//Description: Prompts the user to enter a rating <1-5> for the song they selected
void getRating(Rating *choice);

//Description: Prints off an "edit menu." I.e. which field would you like to edit?
void printEdit(void);

//Description: gets the user's choice for which field they would like to edit
//			   this is a number between 0 and 7. If the user selects 0, he/she
//			   is done editing!
void getEditChoice(EditChoice *pChoice);

// Description: gets the user's input to the keyboard, up to 4 words; I needed to make
//				this because my stdin and fgets function is not working (probably because
//				I'm working on a virtual machine.
void getStdin(char string[]);

// Description: gets the user's input data to store in a new record; then it actually
//				stores the data by using a pointer to pData (to retain changes)
void getInsertData(Record *pData);

// Description: simply prints the menu for a user to select a sorting option
void printSort(void);

// Description: gets the user's selection choice (an integral value from 1-4)
//				which determines how he/she would like to sort the data
void getSortChoice(SortChoice *pChoice);





#endif