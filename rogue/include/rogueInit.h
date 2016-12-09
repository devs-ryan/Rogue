/**
 * Ryan Claxton (Jan 2016)
 * claxtonr@mail.uoguelph.ca
 *
 * This file contains the initialization for Rogue
 * functions for assignment 3 CIS2500.
 * see main for more details.
 */

#ifndef __CLAXTONR_ROGUEINIT__
#define __CLAXTONR_ROGUEINIT__

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include <string.h>
#include "memory.h"

/**
 * initGame
 * Initializes the game board for rogue based on text file input
 * IN: Pointer to level sturct
 * OUT: completed game board displayed to ncurses terminal
 * POST: N/A
 * ERROR: exit(1) if minumin dimension of terminal window not met
 */
void initGame(room * level[], gold * levelGold[]);

/**
 * printRoom
 * Draws a single complete room with eliments
 * IN: pointer to room struct, pointer to gold struct, center coordinates (y,x)
 * OUT: completed room displayed to ncurses terminal
 * POST: N/A
 * ERROR: Exit(1) if max room size exceeded
 */
void printRoom(room drawRoom[], gold fillGold[], int, int);

/**
 * findOrigin
 * gets the top left coordinates for a room based on center point and size
 * IN: accepts y or x coordinate and the cooresponding width/height
 * OUT: top or left most coordinate of a room
 * POST: N/A
 * ERROR: N/A
 */
int findOrigin(int dimension, int centre);

/**
 * drawFloor
 * draws the floor area of a single room
 * IN: origin coordinates(x,y), height, width
 * OUT: completed floor area drawn to ncurses terminal
 * POST: N/A
 * ERROR: N/A
 */
void drawFloor(int x, int y, int w, int h);

/**
 * drawWalls
 * draws the walls of a single room
 * IN: origin coordinates(x,y), height, width
 * OUT: completed wall area drawn to ncurses terminal
 * POST: N/A
 * ERROR: N/A
 */
void drawWalls(int x, int y, int w, int h);

/**
 * drawDoors
 * draws the doors of a single room
 * IN: room struct, gold struct origin coordinates(x,y), height, width
 * OUT: completed doors drawn to ncurses terminal
 * POST: N/A
 * ERROR: exit(1) if door is not located in a wall area
 */
void drawDoors(room drawDoors[], int x, int y, int w, int h);

/**
 * drawElements
 * draws the elements of a single room
 * IN: room struct, gold struct, origin coordinates(x,y), height, width
 * OUT: completed elements drawn to ncurses terminal/ filled gold values
 * POST: N/A
 * ERROR: exit(1) if elements are drawn in an area not located in the room
 */
void drawElements(room drawElements[],gold fillGold[], int x, int y, int w, int h);

/**
 * getRand
 * generates a random number
 * IN: min/max number to generate
 * OUT: random number
 * POST: N/A
 * ERROR: N/A
 */
int genRand(int min, int max);

/**
 * drawsHalls
 * draws halls to connect rooms together
 * IN: room location, wall location, room width, door location, room origin
 * OUT:  N/A
 * POST: room hallways outputted to ncurses
 * ERROR: N/A
 */
void drawHalls(char roomLoc, char wallLoc, int roomWidth, int doorLoc, int yOrigin, int xOrigin);

/**
 * connectHalls
 * connects door halls together
 * IN: N/A
 * OUT:  N/A
 * POST: hallways outputted to ncurses
 * ERROR: N/A
 */
void connectHalls();

#endif /* rogueInit_h */




