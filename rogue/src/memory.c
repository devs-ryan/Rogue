/**
 * Ryan Claxton (2016)
 * claxtonr@mail.uoguelph.ca
 *
 * See memory.h for usage
 */

#include "memory.h"


room * createMem() {
    room * roomMem;
    roomMem = malloc(sizeof(room));
    if (roomMem == NULL) {
        printf("Out of memory\n");
        exit(1);
    }
    for (int i = 0; i < 10; i++) {
        roomMem->elements[i] = malloc(sizeof(char)*8);
        if (roomMem->elements[i] == NULL) {
            printf("Out of memory\n");
            exit(1);
        }
    }
    for (int i = 0; i < 4; i++) {
        roomMem->doors[i] = malloc(sizeof(char)*8);
        if (roomMem->doors[i] == NULL) {
            printf("Out of memory\n");
            exit(1);
        }
    }
    return roomMem;
}

gold * createGoldMem(room goldCount[]) {
    gold * goldMem;
    int numGold = goldCount->numGold;
    goldMem = malloc(sizeof(gold)*numGold);
    if (goldMem == NULL) {
        printf("Out of memory\n");
        exit(1);
    }
    return goldMem;
}

player * createPlayerMem() {
    player * playerMem;
    playerMem = malloc(sizeof(player));
    if (playerMem == NULL) {
        printf("Out of memory\n");
        exit(1);
    }
    return playerMem;
}

enemy * createMonsterMem() {
    enemy * monsterMem;
    monsterMem = malloc(sizeof(enemy));
    if (monsterMem == NULL) {
        printf("Out of memory\n");
        exit(1);
    }
    return monsterMem;
}

void freeMonsterMem(enemy * monsterMem) {
    free(monsterMem);
}

void freePlayerMem(player * mem) {
    free(mem);
}

void freeMem(room * mem) {
    for (int i = 0; i < 10; i++) {
        free(mem->elements[i]);
    }
    for (int i = 0; i < 4; i++) {
        free(mem->doors[i]);
    }
    free(mem);
}

void freeGoldMem(gold * mem) {
    free(mem);
}



