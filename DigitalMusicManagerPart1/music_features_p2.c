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

#include "musicManager.h"


void deleteRecord(Node *pSong, Node **pList)
{
	Node *pDelete = pSong;

	if (pSong->pNext == NULL)  // deleting last item in list
	{
		pDelete->pPrev->pNext = NULL;
	}
	else if (pSong == *pList)  // deleting first item in list
	{
		*pList = pSong->pNext;   // change start of list
		pDelete->pNext->pPrev = NULL;
	}
	else  // deleting from the middle of the list
	{
		pDelete->pPrev->pNext = pDelete->pNext;
		pDelete->pNext->pPrev = pDelete->pPrev;
	}
	free(pSong->data.artist);
	free(pSong->data.albumTitle);
	free(pSong->data.songTitle);
	free(pSong->data.genre);
	free(pSong);
}

int sizeofList(Node *pList)
{
	Node *pCur = pList;
	int size = 0;
	while (pCur != NULL)
	{
		++size;
		pCur = pCur->pNext;
	}
	return size;
}

Boolean condition(Node *C, SortChoice choice)
{
	Boolean success = FALSE;

	switch (choice)
	{
	case BY_ARTIST:
		if (strcmp(C->data.artist, C->pPrev->data.artist) < 0)
		{
			success = TRUE;
		}
		break;
	case BY_ALB_TITLE:
		if (strcmp(C->data.albumTitle, C->pPrev->data.albumTitle) < 0)
		{
			success = TRUE;
		}
		break;
	case BY_RATING:
		if ((C->data.rating) < (C->pPrev->data.rating))
		{
			success = TRUE;
		}
		break;
	case BY_TIMES_PLAYED:
		if (C->data.timesPlayed < C->pPrev->data.timesPlayed)
		{
			success = TRUE;
		}
		break;
	}

	return success;
}

void Sort(Node **pList, SortChoice sort)
{
	Node *U = *pList, *C = NULL, *pBef = NULL, *pCur = NULL, *pTemp = NULL;
	int passes = 1;

	while (U->pNext != NULL)  //U needs to point at the end of the list to start
	{
		U = U->pNext;
	}
	while (passes < NUM_SONGS)  // while unsorted section > one element
		// U->pPrev->pPrev != NULL
	{
		pBef = *pList;  //pBef should point at 1st node
		C = pBef->pNext;  //set C to initially point at 2nd node
		while (C != U->pNext && C->pNext != NULL && C->pPrev != NULL)  // while C has not passed U
			//(C != U->pNext || C != U) && C != NULL && C->pPrev != NULL
		{
			if (condition(C, sort))
			{
				if (pBef->pPrev == NULL)  // dealing with the first two items in list
				{
					pCur = C;
					C->pNext->pPrev = pBef;  // connect pointer in front of C's pPrev to node behind C
					C->pPrev->pNext = C->pNext; // node behind C's pNext set to node in front of C
					C->pNext = pBef;
					C->pPrev = NULL;
					*pList = pCur;  //change the start of the list to point at C
				}
				else if (C->pNext == NULL)  // dealing with last two items in list
				{
					C->pPrev->pPrev->pNext = C;  // two nodes behind C has a pNext which should now point at C
					C->pPrev = C->pPrev->pPrev;  // C's pPrev should point at two nodes before C
					pBef->pPrev = C;  // update middle node's pointers
					pBef->pNext = NULL;
				}
				else   // somewhere in the middle of the list
				{
					pCur = C->pNext;  // need this so I don't lose my link to the node after C
					C->pPrev->pPrev->pNext = C;  //connect 1st node to 3rd
					C->pNext->pPrev = pBef;  //connect 4th node to 2nd
					C->pNext = pBef;  // update C's pointers
					C->pPrev = pBef->pPrev;
					pBef->pPrev = C;  // update node before C's pointers
					pBef->pNext = pCur;
				}
				C = pBef;  // C should point to original position
			}
			if (C->pNext != NULL)
			{
				C = C->pNext;  // move C to the right one position
				pBef = C->pPrev;  // move pBef to the right one position
			}
		}
		U = U->pPrev; // move U to the left one position
		++passes;
	}
	printf("Records were successfully sorted!\n");
}

void randOrder(int random[], int size)
{
	int i = 0, num = 0, chosen[NUM_SONGS] = { 0 };

	for (i = 0; i < size; ++i)
	{
		do
		{
			num = rand() % size;
		} while (chosen[num]);
		chosen[num] = PICKED;
		random[i] = num;
	}
}

void playSingleRecord(Node *pPlay)
{
	printf("\t\tMode: \"SHUFFLE\"\n");
	printf(" Artist:\t\t%s\n", pPlay->data.artist);
	printf(" Album Title:\t\t%s\n", pPlay->data.albumTitle);
	printf(" Song Title:\t\t%s\n", pPlay->data.songTitle);
	printf(" Genre:\t\t\t%s\n", pPlay->data.genre);
	printf(" Song Length:\t\t%d:%d\n", pPlay->data.songLength.min, pPlay->data.songLength.sec);
	printf(" Times Played:\t\t%u\n", pPlay->data.timesPlayed);
	printf(" Rating:\t\t%d out of 5\n", pPlay->data.rating);
	system("pause");
	system("cls");
}

void Shuffle(int randOrder[], int size, Node *pList)
{
	int i = 0, position = 0;
	Node *pPlay = pList;

	for (i = 0; i < size; ++i)
	{     //find node at position randOrder[i]
		  //pList will be at position 0
		if (randOrder[i] > position)   // record is at a position > than what we're currently at
		{
			while (randOrder[i] != position)
			{
				pPlay = pPlay->pNext;
				++position;
			}
		}
		else  // record is at a position < than what we're currently at
		{
			while (randOrder[i] != position)
			{
				pPlay = pPlay->pPrev;
				--position;
			}
		}
		//pass in that pointer to a playSingleRecord function
		playSingleRecord(pPlay);
	}
}