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

void runApp(void)
{
	Node *pList = NULL, *pFound = NULL;
	UserChoice userSelected = 0;
	PrintChoice print = 0;
	Rating user_rating = 0;
	EditChoice edit = 0;
	SortChoice sort = 0;
	FILE *read = NULL, *write = NULL;
	Boolean loadedSuccessfully = FALSE;
	char readInLine[MAX_ARTIST_NAME] = "",  // same size as MAX_SONG_NAME so I will use in both situations
		p1[50] = { '\0' }, p2[50] = { '\0' }, p3[50] = { '\0' }, p4[50] = { '\0' },
		*pp1 = p1, *pp2 = p2, *pp3 = p3, *pp4 = p4;
	int rand[NUM_SONGS] = { 0 };
	Record newData = { pp1, pp2, pp3, pp4,{ 0,0 },0,0 };
	srand((unsigned)time(NULL)); //seed value

	while (userSelected != EXIT)
	{
		system("cls");
		printMainMenu();
		getChoice(&userSelected);
		system("cls");   // clear screen
		switch (userSelected)
		{
		case LOAD:  // as each record is read from file, insertAtFront
			read = fopen("musicPlaylist.csv", "r");
			loadedSuccessfully = loadRecords(read, &pList);
			if (loadedSuccessfully)
			{
				printf("All records were loaded successfully.\n");
			}
			fclose(read);
			break;
		case STORE:  // writes the current records to the musicPlayList.csv file
		case EXIT:  // does the same, except it will exit loop afterwards
			write = fopen("musicPlaylist.csv", "w");
			if (write != NULL)  // file opened for mode "write"
			{
				storeRecords(write, pList);
			}
			fclose(write);
			break;
		case DISPLAY:  // print all or print all that match an artist
			printPrintMenu();
			getPrintChoice(&print);
			fflush(stdin);
			switch (print)
			{
			case ALL:   // print all song info starting from the top
				printList(pList);
				break;
			case MATCH_ARTIST:
				getArtist(readInLine);
				pFound = artistSearch(pList, readInLine);
				if (pFound != NULL)  // artist was, indeed, found
				{
					printArtistMatch(pFound); // prints the records that match the artist
				}
				break;
			}
			break;
		case INSERT:
			getInsertData(&newData);
			insertFront(&pList, newData);
			break;
		case DELETE:
			printf("For Deletion:\n");
			getSong(readInLine);
			pFound = songSearch(pList, readInLine);
			if (pFound != NULL)
			{
				deleteRecord(pFound, &pList);
			}
			break;
		case EDIT:  // let user find record by artist; they can modify any field
			getArtist(readInLine);
			pFound = artistSearch(pList, readInLine);
			clearLine(readInLine);  //clear out so I can reuse
			if (pFound != NULL)  // artist was, indeed, found
			{
				system("cls");
				printArtistMatch(pFound);  // now user must select the song
				getSong(readInLine);
				pFound = songSearch(pList, readInLine);
				if (pFound != NULL)
				{
					do
					{
						system("cls");
						printEdit();
						getEditChoice(&edit);
						if (edit != STOP)
						{
							editSelection(pFound, edit);
						}
						system("pause");
					} while (edit != STOP);
				}
			}
			break;
		case SORT:
			printSort();
			getSortChoice(&sort);
			Sort(&pList, sort);
			break;
		case RATE:  // allow user to assign a value of 1-5 to a song
			getSong(readInLine);
			pFound = songSearch(pList, readInLine);
			if (pFound != NULL)  // artist was found!
			{
				getRating(&user_rating);
				rateSong(pFound, user_rating);
			}
			break;
		case PLAY:  // allow user to select song; show entirety of record for brief period
					// of time; move on to next song until all songs "played"
			getSong(readInLine);
			pFound = songSearch(pList, readInLine);
			if (pFound != NULL)  // artist was found!
			{
				play(pList, pFound);
			}
			break;
		case SHUFFLE:
			randOrder(rand, NUM_SONGS);
			Shuffle(rand, NUM_SONGS, pList);
			break;
		}
		clearLine(readInLine);
		system("pause");  // after done with operation, give the user a chance to stop
	}
	printf("Thanks for using Kyler's Digital Music Manager!\n");
}

Boolean isEmpty(Node *pList)
{
	Boolean success = FALSE;
	if (pList == NULL)  //list is empty
	{
		success = TRUE;
	}
	return success;
}

Node * makeNode(Record newData)
{
	Node *pMem = NULL;
	pMem = (Node *)malloc(sizeof(Node));
	if (pMem != NULL)  //we successfully allocated memory on the heap
	{
		pMem->data.artist = (char *)malloc(sizeof(char) * strlen(newData.artist) + 1);
		strcpy(pMem->data.artist, newData.artist);
		pMem->data.albumTitle = (char *)malloc(sizeof(char) * strlen(newData.albumTitle) + 1);
		strcpy(pMem->data.albumTitle, newData.albumTitle);
		pMem->data.songTitle = (char *)malloc(sizeof(char) * strlen(newData.songTitle) + 1);
		strcpy(pMem->data.songTitle, newData.songTitle);
		pMem->data.genre = (char *)malloc(sizeof(char) * strlen(newData.genre) + 1);
		strcpy(pMem->data.genre, newData.genre);
		pMem->data.songLength.min = newData.songLength.min;
		pMem->data.songLength.sec = newData.songLength.sec;
		pMem->data.timesPlayed = newData.timesPlayed;
		pMem->data.rating = newData.rating;
		pMem->pNext = NULL;
		pMem->pPrev = NULL;
	}
	return pMem;
}

Boolean insertFront(Node **pList, Record newData)
{
	Node *pMem = NULL, *pStart = NULL;
	Boolean success = FALSE;

	pMem = makeNode(newData);

	if (pMem != NULL)   // Memory successfully allocated
	{
		if (isEmpty(*pList) == FALSE)  // list is NOT empty
		{
			pStart = *pList;
			pMem->pNext = pStart; // second item in list will be **pList
			pStart->pPrev = pMem;  // connects the 2nd node back to the node we're inserting
			*pList = pMem;  // pointer to the start of the list now will point towards pMem
		}
		else  // List is empty
		{
			*pList = pMem;
		}
		success = TRUE;
	}
	else
	{
		printf("WARNING: No memory is available for data insertion!\n");
	}
	return success;
}

void clearLine(char line[])
{
	int i = 0;
	for (i = 0; line[i] != '\0'; ++i)
	{
		line[i] = '\0';
	}
}