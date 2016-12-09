/**
 * Ryan Claxton (Jan 2016)
 * claxtonr@mail.uoguelph.ca
 *
 * This file fileIO functions for assignment 3
 * CIS2500 see main for more details.
 *
 */

#ifndef __CLAXTONR_FILEIO__
#define __CLAXTONR_FILEIO__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include "memory.h"
#include "rogueInit.h"

/**
 * greeting
 * displayes greeting output
 * IN: N/A
 * OUT: prints start game message to stdOUT
 * POST: N/A
 * ERROR: N/A
 */
void greeting();

/**
 * getLevels
 * reads a data file, seperates into global struct
 * IN: argv, pointer to struct to return to main
 * OUT: N/A
 * POST: N/A
 * ERROR: exit(1) if data file is invalid or cannot be opened
 */
void getLevels (char * argv[], room * layout[]);

/**
 * endGame
 * displayes end game output
 * IN: gold total
 * OUT: prints end game message to stdOUT
 * POST: N/A
 * ERROR: N/A
 */
void endGameText();

/**
 * doorGen
 * generates doors for a single room
 * IN: level char, num doors, room width and height
 * OUT: N/A
 * POST: altered room string
 * ERROR: N/A
 */
void doorGen(char doors[100], int numDoors, int width, int height);

/**
 * doorGen
 * generates doors for a single room
 * IN: level char, num monsters or items, room width and height
 * OUT: N/A
 * POST: altered room string
 * ERROR: N/A
 */
void genItemMonsters(char itMons[100], int numItMons, int width, int height);

/**
 * generate
 * generates room
 * IN: room string
 * OUT: N/A
 * POST: altered room string
 * ERROR: N/A
 */
void generate(char room[100]);

#endif /* fileIO_h */
