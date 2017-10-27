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

int main(void)
{
	runApp();

	return 0;
}