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

Boolean loadRecords(FILE *infile, Node **pList)
{
	Boolean success = FALSE, inserted = FALSE;
	char line[MAX_CHARS_READ] = "", p1[50] = { '\0' }, p2[50] = { '\0' }, p3[50] = { '\0' }, p4[50] = { '\0' },
		*pp1 = p1, *pp2 = p2, *pp3 = p3, *pp4 = p4;
	int lineNum = 1;
	Record r = { pp1, pp2, pp3, pp4,{ 0,0 },0,0 };

	while (fgets(line, MAX_CHARS_READ, infile) != NULL) // not at end of file
	{
		if (line[0] == '\"')   // this is the " character which means artist has first AND last name
		{
			strcpy(r.artist, strtok(line, "\""));   // copy until second "
		}
		else
		{
			strcpy(r.artist, strtok(line, ","));  // copy until ,
		}
		strcpy(r.albumTitle, strtok(NULL, ","));  //NULL means continue from previous token generated
		strcpy(r.songTitle, strtok(NULL, ","));
		strcpy(r.genre, strtok(NULL, ","));
		r.songLength.min = atoi(strtok(NULL, ":"));
		r.songLength.sec = atoi(strtok(NULL, ","));
		r.timesPlayed = (unsigned int)atoi(strtok(NULL, ","));
		r.rating = (Rating)atoi(strtok(NULL, "\n"));
		inserted = insertFront(pList, r); //insert the node at the front of the list!
		if (inserted)   //allocated space for node!
		{
			success = TRUE;
		}
		else
		{
			printf("WARNING: No memory to store line %d\n", lineNum);
		}
		++lineNum;
	}
	return success;
}

void printList(Node *pList)
{
	Node *pCur = NULL;
	pCur = pList;
	if (pCur)  // indicating we have a non empty list
	{
		while (pCur != NULL) // while we're not at the end of the list
		{
			printf(" Artist:\t\t%s\n", pCur->data.artist);
			printf(" Album Title:\t\t%s\n", pCur->data.albumTitle);
			printf(" Song Title:\t\t%s\n", pCur->data.songTitle);
			printf(" Genre:\t\t\t%s\n", pCur->data.genre);
			printf(" Song Length:\t\t%d:%d\n", pCur->data.songLength.min, pCur->data.songLength.sec);
			printf(" Times Played:\t\t%u\n", pCur->data.timesPlayed);
			printf(" Rating:\t\t%d out of 5\n", pCur->data.rating);
			pCur = pCur->pNext; // advance through the list
			system("pause");
			system("cls");
		}
	}
	else
	{
		printf("List is empty.\n");
	}
}

void printArtistMatch(Node *pFirstOcc)
{
	Node *pCur = pFirstOcc, *pArtist = pFirstOcc;

	if (pCur)  // indicating we have a non empty list
	{
		while (pCur != NULL) // while we're not at the end of the list
		{
			if (strcmp(pCur->data.artist, pArtist->data.artist) == 0)
			{  // this indicates that the current record matches the artist ^
				printf(" Artist:\t\t%s\n", pCur->data.artist);
				printf(" Album Title:\t\t%s\n", pCur->data.albumTitle);
				printf(" Song Title:\t\t%s\n", pCur->data.songTitle);
				printf(" Genre:\t\t\t%s\n", pCur->data.genre);
				printf(" Song Length:\t\t%d:%d\n", pCur->data.songLength.min, pCur->data.songLength.sec);
				printf(" Times Played:\t\t%u\n", pCur->data.timesPlayed);
				printf(" Rating:\t\t%d out of 5\n", pCur->data.rating);
				system("pause");
				system("cls");
			}
			pCur = pCur->pNext; // advance through the list
		}
	}
	else
	{
		printf("List is empty.\n");
	}
}

Node * artistSearch(Node *pList, char readInLine[])
{
	Boolean success = FALSE;
	Node * pCur = pList;

	while (pCur != NULL && success == FALSE)  // we're not done searching
	{
		if (strcmp(pCur->data.artist, readInLine) == 0) // we have a match!
		{
			success = TRUE;  // this will break me out of the loop
		}
		else  // keep searching
		{
			pCur = pCur->pNext;
		}
	}
	if (success == FALSE)
	{
		printf("Error: artist not found\n");
		pCur = NULL;
	}
	return pCur;
}

void storeRecords(FILE *outfile, Node *pList)
{
	Node *pCur = NULL;
	int i = 0, space = 0;

	pCur = pList;  // set pCur to start of list

	while (pCur != NULL)
	{
		for (i = 0; pCur->data.artist[i] != '\0'; ++i)
		{
			if (pCur->data.artist[i] == ' ') // then artist has first and last name
			{
				++space;
			}
		}
		if (space)  
		{
			fputs("\"", outfile);
			fputs(pCur->data.artist, outfile);
			fputs("\"", outfile);
		}
		else
		{
			fputs(pCur->data.artist, outfile);
		}
		fprintf(outfile, ",");
		fputs(pCur->data.albumTitle, outfile);
		fprintf(outfile, ",");
		fputs(pCur->data.songTitle, outfile);
		fprintf(outfile, ",");
		fputs(pCur->data.genre, outfile);
		fprintf(outfile, ",");
		fprintf(outfile, "%d:%d,", pCur->data.songLength.min, pCur->data.songLength.sec);
		fprintf(outfile, "%u,%d\n", pCur->data.timesPlayed, pCur->data.rating);
		pCur = pCur->pNext;
	}
}

void rateSong(Node *pSong, Rating rating)
{
	pSong->data.rating = rating;
}

Node * songSearch(Node *pList, char readInLine[])
{
	Boolean success = FALSE;
	Node * pCur = pList;

	while (pCur != NULL && success == FALSE)  // we're not done searching
	{
		if (strcmp(pCur->data.songTitle, readInLine) == 0) // we have a match!
		{
			success = TRUE;  // this will break me out of the loop
		}
		else  // keep searching
		{
			pCur = pCur->pNext;
		}
	}
	if (success == FALSE)
	{
		printf("Error: song not found\n");
		pCur = NULL;
	}
	return pCur;
}

void editSelection(Node *pEditThis, EditChoice choice)
{
	char readInLine[MAX_ENTRY] = "", wordTwo[MAX_EDIT] = { '\0' }, wordThree[MAX_EDIT] = { '\0' },
		wordFour[MAX_EDIT] = { '\0' }, indicator = '\0', *pMem = NULL;

	//My stdin is not working! So I am forced to use a scanf... to accomodate multiple words
	//fgets(readInLine, MAX_ENTRY - 1, stdin);
	//readInLine[strlen(readInLine) - 1] = '\0';

	printf("Change field to? ");
	getStdin(readInLine);
	//scanf("%s", readInLine);
	//scanf("%c", &indicator);
	//if (indicator != '\n')  //user typed in two words
	//{
	//	scanf("%s", wordTwo);
	//	strcat(readInLine, " ");
	//	strcat(readInLine, wordTwo);
	//	scanf("%c", &indicator);
	//	if (indicator != '\n')  //user typed in three words
	//	{
	//		scanf("%s", wordThree);						//until I fix my fgets, I will
	//		strcat(readInLine, " ");					//use this method so that my
	//		strcat(readInLine, wordThree);				//program actually works
	//		scanf("%c", &indicator);
	//		if (indicator != '\n')  //user typed in four words
	//		{
	//			scanf("%s", wordFour);
	//			strcat(readInLine, " ");
	//			strcat(readInLine, wordFour);
	//		}
	//	}
	//}
	
	switch (choice)
	{
	case ARTIST:
		pMem = (char *)realloc(pEditThis->data.artist, sizeof(char) * strlen(readInLine) + 1);
		pEditThis->data.artist = pMem;  // change char * value to point at newly allocated memory
		strcpy(pEditThis->data.artist, readInLine);
		break;
	case ALBUM_TITLE:
		pMem = (char *)realloc(pEditThis->data.albumTitle, sizeof(char) * strlen(readInLine) + 1);
		pEditThis->data.albumTitle = pMem;  // change char * value to point at newly allocated memory
		strcpy(pEditThis->data.albumTitle, readInLine);
		break;
	case SONG_TITLE:
		pMem = (char *)realloc(pEditThis->data.songTitle, sizeof(char) * strlen(readInLine) + 1);
		pEditThis->data.songTitle = pMem;  // change char * value to point at newly allocated memory
		strcpy(pEditThis->data.songTitle, readInLine);
		break;
	case GENRE:
		pMem = (char *)realloc(pEditThis->data.genre, sizeof(char) * strlen(readInLine) + 1);
		pEditThis->data.genre = pMem;  // change char * value to point at newly allocated memory
		strcpy(pEditThis->data.genre, readInLine);
		break;
	case SONG_LENGTH:
		pEditThis->data.songLength.min = atoi(strtok(readInLine, ":"));
		pEditThis->data.songLength.sec = atoi(strtok(NULL, ":"));  //will stop at null char anyways
		break;
	case TIMES_PLAYED:
		pEditThis->data.timesPlayed = (unsigned int)atoi(readInLine);
		break;
	case RATING:
		pEditThis->data.rating = atoi(readInLine);
	}
}

void play(Node *pList, Node *pSong)
{
	Node *pCur = pSong, *pStart = pSong;
	Boolean selectedTheFirst = FALSE;

	if (pList == pSong)  // the user selected the first song
	{
		selectedTheFirst = TRUE;
	}
	while (pCur != NULL) // while we're not at the end of the list
	{
		printf(" Artist:\t\t%s\n", pCur->data.artist);
		printf(" Album Title:\t\t%s\n", pCur->data.albumTitle);
		printf(" Song Title:\t\t%s\n", pCur->data.songTitle);
		printf(" Genre:\t\t\t%s\n", pCur->data.genre);
		printf(" Song Length:\t\t%d:%d\n", pCur->data.songLength.min, pCur->data.songLength.sec);
		printf(" Times Played:\t\t%u\n", pCur->data.timesPlayed);
		printf(" Rating:\t\t%d out of 5\n", pCur->data.rating);
		pCur = pCur->pNext; // advance through the list
		system("pause");
		system("cls");
	}
	if (selectedTheFirst == FALSE)  // we're not done printing the list!
	{
		pCur = pList;   // go back to the start
		while (pCur != pStart)
		{
			printf(" Artist:\t\t%s\n", pCur->data.artist);
			printf(" Album Title:\t\t%s\n", pCur->data.albumTitle);
			printf(" Song Title:\t\t%s\n", pCur->data.songTitle);
			printf(" Genre:\t\t\t%s\n", pCur->data.genre);
			printf(" Song Length:\t\t%d:%d\n", pCur->data.songLength.min, pCur->data.songLength.sec);
			printf(" Times Played:\t\t%u\n", pCur->data.timesPlayed);
			printf(" Rating:\t\t%d out of 5\n", pCur->data.rating);
			pCur = pCur->pNext; // advance through the list
			system("pause");
			system("cls");
		}
	}
}