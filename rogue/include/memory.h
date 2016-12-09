/**
 * Ryan Claxton (Jan 2016)
 * claxtonr@mail.uoguelph.ca
 *
 * This file contains the struct definitions and memory
 * functions for assignment 3 CIS2500.
 * see main for more details.
 */

#ifndef __CLAXTONR_MEMORY__
#define __CLAXTONR_MEMORY__

#include <stdio.h>
#include <stdlib.h>

struct goldInfo {
    int locX[10];
    int locY[10];
    int amount[10];
}typedef gold;

struct itemInfo {
    int rating;
    char type;
}typedef item;

struct monsterInfo {
    char type;
    int locY;
    int locX;
    int health;
    int attack;
    int speed;
}typedef enemy;

struct playerInfo {
    int health;
    int goldTotal;
    int numPotions;
    int numItems;
    int attack;
    item items[5];
}typedef player;

struct layout {
    int numDoors;
    int numElements;
    int numGold;
    int yOrigin;
    int xOrigin;
    char rows[8];
    char cols[8];
    char * doors[4];
    char * elements[10];
}typedef room;

enum collisionState {
    floor,
    wall,
    door,
    hall,
    empty,
    goldPickup,
    stairs,
    weaponPickupRare,
    weaponPickupCommon,
    equipmentPickup,
    monster,
    potionPickup
}typedef collision;

/**
 * createRoomMem
 * Creates memory for a Single room
 * IN: N/A
 * OUT: pointer to allocated memory
 * POST: N/A
 * ERROR: Exit(1) if out of memory
 */
room * createMem();

/**
 * createGoldRoomMem
 * Creates memory for a levels gold
 * IN: N/A
 * OUT: pointer to allocated memory
 * POST: N/A
 * ERROR: Exit(1) if out of memory
 */
gold * createGoldMem(room goldCount[]);

/**
 * createPlayerMem
 * Creates memory for a player
 * IN: N/A
 * OUT: pointer to allocated memory
 * POST: N/A
 * ERROR: Exit(1) if out of memory
 */
player * createPlayerMem();

/**
 * createMonsterMem
 * Creates memory for all monsters
 * IN: N/A
 * OUT: pointer to allocated memory
 * POST: N/A
 * ERROR: Exit(1) if out of memory
 */
enemy * createMonsterMem();

/**
 * freePlayerMem
 * Frees player memory
 * IN: pointer to player to be set free
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void freePlayerMem(player * mem);

/**
 * freeMonsterMem
 * Frees monster memory
 * IN: pointer to monsters to be set free
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void freeMonsterMem(enemy * monsterMem);

/**
 * freeMem
 * Frees memory for a Single room
 * IN: pointer to room to be set free
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void freeMem(room * mem);

/**
 * freeGoldMem
 * Frees gold memory for a Single room
 * IN: pointer to gold to be set free
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void freeGoldMem(gold * mem);




#endif /* memory_h */


