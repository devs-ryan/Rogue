/**********************
 CIS 2500 - Assignment 3
 Ryan Claxton 0905979
 claxtonr@mail.uoguelph.ca
 Mar 6/2016
 This program is a "Rogue-Like" computer game
 that accepts an input file describing the layout
 of a single level with 6 rooms and a movable character.
 Assignment 3 for cis2500
 ************************/

#include <stdio.h>
#include "fileIO.h"
#include "memory.h"
#include "rogueInit.h"
#include "roguePlay.h"


/**
 * main
 * reads a data file and generates level for user to play
 * allows the user to move character using ncurses
 * IN: argv[1](data file location)
 * OUT: Amount of gold collected
 * POST: N/A
 * ERROR: Return 1: Program terminated early. Reasons: data file not found, invalid arguments
 */
int main (int argc, char * argv[]) {
    //local variables
    room * rooms[6];
    gold * roomGold[6];
    char room[100];
    FILE * outputFile;
    srand(time(NULL));
    
    //-- GENERATE RANDOM FILE --//
    outputFile = fopen("randLevels.txt", "w");
    if (outputFile == NULL) {
        printf("Could not open output file\n\n");
        exit(1);
    }
    for (int i = 0; i < 6; i++) {
        generate(room);
        room[strlen(room) - 1] = '\0';
        if (i == 0) {
            if (strlen(room) < 50)
                strcat(room," h0,0"); //add hero
            else {
                for (int j = strlen(room); j > 0; j--) {
                    if(isalpha(room[j]) && room[j] != ',') {
                        room[j] = 'h'; //replace last item with hero
                        break;
                    }
                }
            }
        }
        fprintf(outputFile, "%s\n", room);
    }
    fclose(outputFile);
    
    //check correct number of arguments
    if (argc != 2) {
        printf ("Not a valid argument, Try again\n");
        return 1;
    }
    //create struct memory for 6 levels
    for (int i = 0; i < 6; i++) {
        rooms[i] = createMem();
    }
    //greeting
    greeting();
    
    //parse levels
    getLevels (argv, rooms);
    
    //create gold memory for 6 levels
    for (int i = 0; i < 6; i++) {
        roomGold[i] = createGoldMem(rooms[i]);
    }
    
    //game functions
    initGame(rooms, roomGold);
    gameLoop(rooms, roomGold);
    
    //free memory for rooms and gold
    for (int i = 0; i < 6; i++) {
        freeMem(rooms[i]);
        freeGoldMem(roomGold[i]);
    }
}


