/**
 * Ryan Claxton (Jan 2016)
 * claxtonr@mail.uoguelph.ca
 *
 * This file contains the game loop functions for Rogue
 * functions for assignment 3 CIS2500.
 * see main for more details.
 */

#ifndef __CLAXTONR_ROGUEPLAY__
#define __CLAXTONR_ROGUEPLAY__


#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include "memory.h"
#include "rogueInit.h"
#include "fileIO.h"

/**
 * gameLoop
 * loops game of rogue until user enters q
 * IN: rooms, gold info
 * OUT: game output to ncurses terminal and returns gold to main
 * POST: N/A
 * ERROR: N/A
 */
void gameLoop(room * currentRoom[], gold * levelGold[]);

/**
 * placeHero
 * places hero in the starting location
 * IN: level data
 * OUT: shows hero on ncurses terminal
 * POST: N/A
 * ERROR: exit(1) no hero is found in the data file
 */
void placeHero(room * rooms[]);

/**
 * drawHero
 * draws the hero at a y, x coordinate on the screen
 * IN: y and x coordinates
 * OUT: game output to ncurses terminal
 * POST: N/A
 * ERROR: N/A
 */
void drawHero(int yLoc, int xLoc);

/**
 * movePlayer
 * moves character on ncurses screen, checks for invalid moves, returns state data on items picked up
 * IN: user input
 * OUT: game output to ncurses terminal, state of result of move (q = game over, 0 = normal, 1 = attack)
 * POST: updates gold counter if gold is picked up
 * ERROR: N/A
 */
char movePlayer(char move, room * level[], gold * levelGold[], player * myPlayer, int numMonsters, enemy * monsterInfo[]);

/**
 * getCollision
 * checks for collisions and returns state to what they are
 * IN: y and x coordinates
 * OUT: state indicating what collision has occured
 * POST: N/A
 * ERROR: N/A
 */
collision getCollision(int yLoc, int xLoc);

/**
 * updateStatus
 * updates status bar
 * IN: player info
 * OUT: status bar update in ncurses
 * POST: N/A
 * ERROR: N/A
 */
void updateStatus(player * status);

/**
 * addGold
 * searches for gold value and adds to total
 * IN: room data, gold data, player loc, player info
 * OUT: N/A
 * POST: updates gold counter to new total
 * ERROR: N/A
 */
void addGold(room * currentRoom[], gold * checkGold[], int yLoc, int xLoc, player * myPlayer);

/**
 * getRoomNum
 * searches for the room the player is in currently
 * IN: y pos, x pos
 * OUT: room number
 * POST: N/A
 * ERROR: N/A
 */
int getRoomNum(int y, int x);

/**
 * pickupItem
 * adds item to inventory and generates stats
 * IN: player info, collision type
 * OUT: N/A
 * POST: adds item to player inventory
 * ERROR: N/A
 */
void pickupItem(player * myPlayer, collision itemType);

/**
 * exitGame
 * displays game end info and closes ncurses
 * IN: player info
 * OUT: displays end game info to terminal
 * POST: N/A
 * ERROR: N/A
 */
void exitGame(player * myPlayer);

/**
 * usePotion
 * use potion or displays out of potions to screen
 * IN: player info
 * OUT: action message
 * POST: health restored for hero
 * ERROR: N/A
 */
void usePotion(player * myPlayer);

/**
 * findMonsters
 * populates list of monsters in memory
 * IN: pointer to monsters struct
 * OUT: number of monsters found
 * POST: filled struct with monster locations and info
 * ERROR: N/A
 */
int findMonsters(enemy * getMonsters[]);

/**
 * fightMonsters
 * combat initiated by player action
 * IN: player info, player location, number of enemies, enemy pointer
 * OUT: 0 for regular attack, 1 for monster died, 2 player died
 * POST: health changes/ attack changes to monster or player
 * ERROR: N/A
 */
int fightMonster(int yLoc, int xLoc, player * playerInfo, int numEnemy, enemy * enemyInfo[]);

/**
 * enemyPatrols
 * moves enemys according to type
 * IN: player info, cycle number, enemy info, number of monsters, check to see if player attacked
 * OUT: 0 for regular patrol, 1 if hero has died and game over
 * POST: health changes/ attack changes to monster or player, enemy locations updated
 * ERROR: N/A
 */
int enemyPatrols(player * playerInfo, int cycleNum, enemy * enemyInfo[], int numMonsters, char checkAttack);

/**
 * combat
 * combat algorithm
 * IN: enemy ID num, player info, enemy info
 * OUT: 0 for regular attack, 1 for monster died, 2 player died
 * POST: health changes/ attack changes to monster or player
 * ERROR: N/A
 */
int combat(int enemyID, player * playerInfo, enemy * enemyInfo[]);

#endif /* roguePlay_h */



