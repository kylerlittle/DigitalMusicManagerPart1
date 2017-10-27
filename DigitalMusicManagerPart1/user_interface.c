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

void printMainMenu(void)
{
	printf("____MAIN MENU____\n");
	printf(" (1)\tload\n");
	printf(" (2)\tstore\n");
	printf(" (3)\tdisplay\n");
	printf(" (4)\tinsert\n");
	printf(" (5)\tdelete\n");
	printf(" (6)\tedit\n");
	printf(" (7)\tsort\n");
	printf(" (8)\trate\n");
	printf(" (9)\tplay\n");
	printf(" (10)\tshuffle\n");
	printf(" (11)\texit\n");
}

void getChoice(UserChoice *choice)
{
	printf("Please enter a numerical option <1-11>: ");
	do
	{
		scanf("%d", choice);
	} while ((*choice < LOAD) || (*choice > EXIT));
}

void printPrintMenu(void)
{
	printf("\t1.  Print all records.\n");
	printf("\t2.  Print all records that match an artist.\n");
}

void getPrintChoice(PrintChoice *choice)
{
	printf("Please enter a numerical option <1 or 2>: ");
	do
	{
		scanf("%d", choice);
	} while ((*choice < ALL) || (*choice > MATCH_ARTIST));
}

void getArtist(char readInLine[])
{
	char firstName[MAX_ARTIST_NAME] = { '\0' }, indicator = '\0';

	printf("Type in an artist's name: ");
	getStdin(readInLine);
	//scanf("%s", readInLine);
	//scanf("%c", &indicator);
	//if (indicator != '\n')  //user typed in two words
	//{
	//	scanf("%s", firstName);
	//	strcat(readInLine, " ");
	//	strcat(readInLine, firstName);
	//}
}

void getSong(char readInLine[])
{
	char wordTwo[MAX_SONG_NAME] = { '\0' }, wordThree[MAX_SONG_NAME] = { '\0' },
		indicator = '\0';

	printf("Type in a song name: ");
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
	//		scanf("%s", wordThree);
	//		strcat(readInLine, " ");
	//		strcat(readInLine, wordThree);
	//	}
	//}
}

void getRating(Rating *choice)
{
	printf("Enter a rating for this song <1-5>: ");
	do
	{
		scanf("%d", choice);
	} while ((*choice < SHIT) || (*choice > HOT_DAMN));
}

void printEdit(void)
{
	printf("Which field would you like to edit? Enter <0-7>\n");
	printf(" (1)\t\tArtist\n");
	printf(" (2)\t\tAlbum Title\n");
	printf(" (3)\t\tSong Title\n");
	printf(" (4)\t\tGenre\n");
	printf(" (5)\t\tSong Length\n");
	printf(" (6)\t\tTimes Played\n");
	printf(" (7)\t\tRating\n\n");
	printf(" (0)\t\tEXIT. I am done editing.\n");
}

void getEditChoice(EditChoice *pChoice)
{
	printf("\t Field: ");
	do
	{
		scanf("%d", pChoice);
	} while ((*pChoice < STOP) || (*pChoice > RATING));
}

void getStdin(char string[])
{
	char indicator = '\0', extraWord[MAX_NEW_FIELD] = { '\0' };

	scanf("%s", string);
	scanf("%c", &indicator);
	if (indicator != '\n')  //user typed in two words
	{
		scanf("%s", extraWord);
		strcat(string, " ");
		strcat(string, extraWord);
		scanf("%c", &indicator);
		if (indicator != '\n')  //user typed in three words
		{
			scanf("%s", extraWord);						//until I fix my fgets, I will
			strcat(string, " ");					//use this method so that my
			strcat(string, extraWord);				//program actually works
			scanf("%c", &indicator);
			if (indicator != '\n')  //user typed in four words
			{
				scanf("%s", extraWord);
				strcat(string, " ");
				strcat(string, extraWord);
			}
		}
	}
}

void getInsertData(Record *pData)
{
	char entry[MAX_NEW_FIELD] = { '\0' };
	int num = 0;

	printf("Inserting New Record:\n");
	printf("Artist: ");
	getStdin(entry);
	strcpy(pData->artist, entry);
	clearLine(entry);
	printf("Album Title: ");
	getStdin(entry);
	strcpy(pData->albumTitle, entry);
	clearLine(entry);
	printf("Song Title: ");
	getStdin(entry);
	strcpy(pData->songTitle, entry);
	clearLine(entry);
	printf("Genre: ");
	getStdin(entry);
	strcpy(pData->genre, entry);
	clearLine(entry);
	printf("Song Length <Min:Sec>: ");
	getStdin(entry);
	pData->songLength.min = atoi(strtok(entry, ":"));
	pData->songLength.sec = atoi(strtok(NULL, "\n"));
	printf("Times Played: ");
	scanf("%d", &num);
	pData->timesPlayed = (unsigned int)num;
	printf("Rating <1-5>: ");
	scanf("%d", &num);
	pData->rating = (Rating)num;
}

void printSort(void)
{
	printf("SORT MENU\n");
	printf(" (1)\tSort based on artist (A-Z)\n");
	printf(" (2)\tSort based on album title (A-Z)\n");
	printf(" (3)\tSort based on rating (1-5)\n");
	printf(" (4)\tSort based on times played (largest-smallest)\n");
}

void getSortChoice(SortChoice *pChoice)
{
	printf("Enter a method to sort by <1-4>: ");
	do
	{
		scanf("%d", pChoice);
	} while ((*pChoice < BY_ARTIST) || (*pChoice > BY_TIMES_PLAYED));
}