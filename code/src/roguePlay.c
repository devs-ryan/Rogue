/**
 * Ryan Claxton (2016)
 * claxtonr@mail.uoguelph.ca
 *
 * See roguePlay.h for usage
 */

#include "roguePlay.h"

void gameLoop(room * level[], gold * levelGold[]) {
    //local variables
    char input;
    int cycle = 1;
    int enemyStatus;
    
    //initialize player
    player * myPlayer;
    myPlayer = createPlayerMem();
    myPlayer->health = 50;
    myPlayer->numPotions = 1;
    myPlayer->goldTotal = 0;
    myPlayer->attack = 5;
    myPlayer->numItems = 0;
    
    //initialize monsters
    enemy * allMonsters[60];
    for (int i = 0; i < 60; i++) {
        allMonsters[i] = createMonsterMem();
    }
    int numMonsters = findMonsters(allMonsters);
    
    updateStatus(myPlayer);
    placeHero(level); //start game
    do {
        input = getch();
        if (input == 'w' || input == 's' || input == 'a' || input == 'd') {
            input = movePlayer(input, level, levelGold, myPlayer, numMonsters, allMonsters);
            enemyStatus = enemyPatrols(myPlayer, cycle, allMonsters, numMonsters, input);
            
            if (enemyStatus == 1) {
                input = 'q';
            }
            
            cycle++;
            if (cycle == 5) { // reset cyle
                cycle = 1;
            }
        }
        else if (input == 'p') {
            usePotion(myPlayer);
        }

    } while (input != 'q');
    
    //end game and free game mem
    exitGame(myPlayer);
    freePlayerMem(myPlayer);
    for (int i = 0; i < 60; i++) {
         freeMonsterMem(allMonsters[i]);
    }

}

void placeHero(room * rooms[]) {
    int heroFound = 0;
    for (int i = 0; i < 6; i++) {
        int numElements = rooms[i]->numElements;
        for (int j = 0; j < numElements; j++) {
            char check = rooms[i]->elements[j][0];
            if (check == 'h') {
                //get location X and Y of hero from mid string
                char temp[6];
                int locY;
                int locX;
                if (rooms[i]->elements[j][2] == ',') {
                    locY = rooms[i]->elements[j][1] - '0';
                    locX = atoi(strcpy(temp, &rooms[i]->elements[j][3]));
                }
                else {
                    locY = atoi(strncpy(temp, &rooms[i]->elements[j][1],2));
                    locX = atoi(strcpy(temp, &rooms[i]->elements[j][4]));
                }
                //get room origin
                int y = rooms[i]->yOrigin;
                int x = rooms[i]->xOrigin;
                //initialize hero
                drawHero(locY + y, locX + x);
                heroFound = 1;
                break;
            }
        }
    }
    if (heroFound != 1) {
        endwin();
        printf("No hero was found in the data file,\nTry again.\n");
        exit(1);
    }
}

void drawHero(int yLoc, int xLoc) {
    mvaddch(yLoc, xLoc, '@');
    move(yLoc, xLoc);
}

char movePlayer(char move, room * level[], gold * levelGold[], player * myPlayer, int numMonsters, enemy * monsterInfo[]) {
    //local variables
    int xLoc, yLoc;
    int oldX, oldY;
    int fight = 0;
    collision checkMove = floor;
    getyx(stdscr, yLoc, xLoc);
    oldX = xLoc;
    oldY = yLoc;
    
    //check and move
    switch (move) {
        case 'w':
            yLoc--;
            break;
        case 's':
            yLoc++;
            break;
        case 'd':
            xLoc++;
            break;
        case 'a':
            xLoc--;
            break;
        default:
            break;
    }
    checkMove = getCollision(yLoc,xLoc);
    switch (checkMove) {
            
        //move onto floor
        case floor:
            mvprintw(0, 0, "                                                                   ");
            if ((mvinch(oldY + 1, oldX) == '|') ||
                (mvinch(oldY - 1, oldX) == '|') ||
                (mvinch(oldY, oldX + 1) == '-') ||
                (mvinch(oldY, oldX - 1) == '-')) {
                mvaddch(oldY, oldX, '+');
                drawHero(yLoc, xLoc);
            }
            else {
                mvaddch(oldY, oldX, '.');
                drawHero(yLoc, xLoc);
            }
                break;
            
        //move into wall
        case wall:
            move(oldY, oldX);
            break;
            
        //door enter
        case door:
            mvprintw(0, 0, "Hero opened a door!                                              ");
            if ((mvinch(oldY + 1, oldX) == '.') ||
                (mvinch(oldY - 1, oldX) == '.') ||
                (mvinch(oldY, oldX + 1) == '.') ||
                (mvinch(oldY, oldX - 1) == '.')) {
                mvaddch(oldY, oldX, '.');
                drawHero(yLoc, xLoc);
            }
            else {
                mvaddch(oldY, oldX, '#');
                drawHero(yLoc, xLoc);
            }
            break;
            
        //move into hall
        case hall:
            mvprintw(0, 0, "                                                                 ");
            if ((mvinch(oldY + 1, oldX) == '|') ||
                (mvinch(oldY - 1, oldX) == '|') ||
                (mvinch(oldY, oldX + 1) == '-') ||
                (mvinch(oldY, oldX - 1) == '-')) {
                mvaddch(oldY, oldX, '+');
                drawHero(yLoc, xLoc);
            }
            else {
                mvaddch(oldY, oldX, '#');
                drawHero(yLoc, xLoc);
            }
            break;
            
        //empty space
        case empty:
            mvprintw(0, 0, "                                                              ");
            move(oldY, oldX);
        break;
        
        //gold picked up
        case goldPickup:
            mvprintw(0, 0, "Hero picked up gold!                                          ");
            addGold(level, levelGold, yLoc, xLoc, myPlayer);
            updateStatus(myPlayer);
            mvaddch(oldY, oldX, '.');
            drawHero(yLoc, xLoc);
            break;
        
        //potion picked up
        case potionPickup:
            mvprintw(0, 0, "Hero picked up potion!                                        ");
            myPlayer->numPotions = myPlayer->numPotions + 1;
            updateStatus(myPlayer);
            mvaddch(oldY, oldX, '.');
            drawHero(yLoc, xLoc);
            break;
        
        case weaponPickupCommon:
            if (myPlayer->numItems < 5) {
                pickupItem(myPlayer, checkMove);
                mvprintw(0, 0, "Hero picked up common weapon!                            ");
                updateStatus(myPlayer);
            }
            else {
                mvprintw(0, 0, "Hero stepped on common weapon, and broke it!             ");
            }
            mvaddch(oldY, oldX, '.');
            drawHero(yLoc, xLoc);
            break;
       
        //item pick ups
        case weaponPickupRare:
            if (myPlayer->numItems < 5) {
                pickupItem(myPlayer, checkMove);
                mvprintw(0, 0, "Hero picked up rare weapon!                             ");
                updateStatus(myPlayer);
            }
            else {
                mvprintw(0, 0, "Hero stepped on rare weapon, and broke it!              ");
            }
            mvaddch(oldY, oldX, '.');
            drawHero(yLoc, xLoc);
            break;
        case equipmentPickup:
            if (myPlayer->numItems < 5) {
                pickupItem(myPlayer, checkMove);
                mvprintw(0, 0, "Hero picked up equipment!                              ");
                updateStatus(myPlayer);
            }
            else {
                mvprintw(0, 0, "Hero stepped on equipment, and broke it!               ");
            }
            mvaddch(oldY, oldX, '.');
            drawHero(yLoc, xLoc);
            break;
        
        //stairs
        case stairs:
            return 'q';
            break;
            
        //monster
        case monster:
            mvprintw(0, 0, "Hero is attacking! Press any key to continue.              ");
            getch();
            fight = fightMonster(yLoc, xLoc, myPlayer, numMonsters, monsterInfo);
            if (fight == 2) {
                return 'q';
            }
            else if (fight == 1) {
                mvaddch(oldY, oldX, '.');
                drawHero(yLoc, xLoc);
                
            }
            else {
                updateStatus(myPlayer);
                drawHero(oldY, oldX);
                return '1';
            }
            break;
            
        default:
        break;
    }
    return '0';
}

collision getCollision(int yLoc, int xLoc) {
    //local variables
    collision state;
    int readScreen = 0;
    readScreen = mvinch(yLoc, xLoc);
    //check colision
    if (readScreen == '+') {
        state = door; // complete
    }
    else if (readScreen == '8' || readScreen == '*') {
        state = goldPickup; //complete
    }
    else if (readScreen == '-' || readScreen == '|') {
        state = wall; //complete
    }
    else if (readScreen == '#') {
        state = hall; //complete
    }
    else if (readScreen == '.') {
        state = floor; //complete
    }
    else if ((readScreen == '<') || (readScreen == '>')) {
        state = stairs; //stairs
    }
    else if (readScreen == '(') {
        state = weaponPickupRare; //complete
    }
    else if (readScreen == ')') {
        state = weaponPickupCommon; //complete
    }
    else if (readScreen == 'A' || readScreen == 'B' ||
             readScreen == 'S' || readScreen == 'Z' ||
             readScreen == 'T') {
        state = monster;
    }
    else if (readScreen == '!') {
        state = potionPickup; //complete
    }
    else if (readScreen == ']') {
        state = equipmentPickup; //complete
    }
    else {
        state = empty; //complete
    }
        
    return state;
}

void updateStatus(player * status) {
    mvprintw(50, 0, "Health: %d, Potions: %d, Attack: %d, Inv: %d/5, Gold: %d                  ",
             status->health, status->numPotions, status->attack, status->numItems, status->goldTotal);
}

void addGold(room * currentRoom[], gold * checkGold[], int yLoc, int xLoc, player * myPlayer) {
    //local variables
    int roomNum = getRoomNum(yLoc, xLoc) - 1;
    int numGold = currentRoom[roomNum]->numGold;
    int foundGold = 0;
    
    //find gold and add to pointer
    for (int i = 0; i < numGold; i++) {
        int checkY = checkGold[roomNum]->locY[i];
        int checkX = checkGold[roomNum]->locX[i];
        if ((checkY == yLoc) && (checkX == xLoc)) {
            foundGold = checkGold[roomNum]->amount[i];
            myPlayer->goldTotal = myPlayer->goldTotal + foundGold;
            break;
        }
    }
}

int getRoomNum(int y, int x) {
    int room;
    if (y < 25) {
        if (x < 30) {room = 1;} else if (x < 59) {room = 2;} else {room = 3;}
    }
    else {
        if (x < 30) {room = 4;} else if (x < 59) {room = 5;} else {room = 6;}
    }
    return room;
}

void pickupItem(player * myPlayer, collision itemType) {
    int numItems = myPlayer->numItems;
    srand(time(NULL));
    switch (itemType) {
        case weaponPickupCommon:
            myPlayer->items[numItems].type = ')';
            myPlayer->items[numItems].rating = genRand(1, 9);
            break;
        case weaponPickupRare:
            myPlayer->items[numItems].type = '(';
            myPlayer->items[numItems].rating = genRand(10, 15);
            break;
        case equipmentPickup:
            myPlayer->items[numItems].type = ']';
            myPlayer->items[numItems].rating = genRand(0, 20);
            break;
        default:
            break;
    }
    myPlayer->numItems = myPlayer->numItems + 1;
    
}

void exitGame(player * myPlayer) {
    endwin();
    endGameText();
    printf("               ===================\n");
    printf("               |    GOLD TOTAL   |\n");
    printf("               ===================\n");
    printf("                   *%d GOLD*\n", myPlayer->goldTotal);
    printf("               ===================\n");
    printf("               | INVENTORY ITEMS |\n");
    printf("               ===================\n");
    if (myPlayer->numItems == 0) {
        printf("                      NONE\n");
    }
    for (int i = 0; i < myPlayer->numItems; i++) {
        char check = myPlayer->items[i].type;
        switch (check) {
            case '(':
                printf("         Rare weapon,  Attack Rating: %2d\n", myPlayer->items[i].rating);
                break;
            case ')':
                printf("       Common weapon,  Attack Rating: %2d\n", myPlayer->items[i].rating);
                break;
            case ']':
                printf("           Equipment, Defence Rating: %2d\n", myPlayer->items[i].rating);
                break;
            default:
                break;
        }
    }
    printf("\n");
}


void usePotion(player * myPlayer) {
    //local variables
    int xLoc, yLoc;
    getyx(stdscr, yLoc, xLoc);
    
    if (myPlayer->numPotions == 0) {
        mvprintw(0, 0, "Out of potions!                                           ");
        drawHero(yLoc, xLoc);
    }
    else {
        myPlayer->numPotions = myPlayer->numPotions - 1;
        myPlayer->health = 50;
        mvprintw(0, 0, "Hero recovered full health!                               ");
        updateStatus(myPlayer);
        drawHero(yLoc, xLoc);
    }
}

int findMonsters(enemy * getMonsters[]) {
    int readScreen = 0;
    int monsterCount = 0;

    for (int y = 0; y <= 53; y++) {
        for (int x = 0; x <= 87; x++) {
            readScreen = mvinch(y, x);

            switch (readScreen) {
                case 'A':
                    getMonsters[monsterCount]->type = 'A';
                    getMonsters[monsterCount]->locY = y;
                    getMonsters[monsterCount]->locX = x;
                    getMonsters[monsterCount]->health = 5;
                    getMonsters[monsterCount]->attack = 1;
                    getMonsters[monsterCount]->speed = 2;
                    monsterCount++;
                    break;
                case 'B':
                    getMonsters[monsterCount]->type = 'B';
                    getMonsters[monsterCount]->locY = y;
                    getMonsters[monsterCount]->locX = x;
                    getMonsters[monsterCount]->health = 2;
                    getMonsters[monsterCount]->attack = 5;
                    getMonsters[monsterCount]->speed = 4;
                    monsterCount++;
                    break;
                case 'S':
                    getMonsters[monsterCount]->type = 'S';
                    getMonsters[monsterCount]->locY = y;
                    getMonsters[monsterCount]->locX = x;
                    getMonsters[monsterCount]->health = 5;
                    getMonsters[monsterCount]->attack = 5;
                    getMonsters[monsterCount]->speed = 4;
                    monsterCount++;
                    break;
                case 'Z':
                    getMonsters[monsterCount]->type = 'Z';
                    getMonsters[monsterCount]->locY = y;
                    getMonsters[monsterCount]->locX = x;
                    getMonsters[monsterCount]->health = 15;
                    getMonsters[monsterCount]->attack = 5;
                    getMonsters[monsterCount]->speed = 2;
                    monsterCount++;
                    break;
                case 'T':
                    getMonsters[monsterCount]->type = 'T';
                    getMonsters[monsterCount]->locY = y;
                    getMonsters[monsterCount]->locX = x;
                    getMonsters[monsterCount]->health = 50;
                    getMonsters[monsterCount]->attack = 5;
                    getMonsters[monsterCount]->speed = 3;
                    monsterCount++;
                    break;
                default:
                    break;
            }
        }
    }
    return monsterCount;
}

int fightMonster(int yLoc, int xLoc, player * playerInfo, int numEnemy, enemy * enemyInfo[]) {
    int enemyID = 0;
    //find enemy
    for (int i = 0; i < numEnemy; i++) {
        if (enemyInfo[i]->locY == yLoc && enemyInfo[i]->locX == xLoc) {
            enemyID = i;
            break;
        }
    }
    //combat
    return combat(enemyID, playerInfo, enemyInfo);

}

int enemyPatrols(player * playerInfo, int cycleNum, enemy * enemyInfo[], int numMonsters, char checkAttack) {
    int heroY, heroX;
    int randNum;
    int status;
    getyx(stdscr, heroY, heroX);
    srand(time(0) + heroY + heroX);
    randNum = genRand(1, 4);

    
    for (int i = 0; i < numMonsters; i++) {
        int readScreen;
        int enemyY, enemyX;
        int oldEnemyY, oldEnemyX;
        char monsterType;
        monsterType = enemyInfo[i]->type;
        enemyY = enemyInfo[i]->locY;
        enemyX = enemyInfo[i]->locX;
        oldEnemyY = enemyY;
        oldEnemyX = enemyX;
        
        //skip dead monsters
        if (enemyInfo[i]->health < 1) {
            continue;
        }

        switch (monsterType) {
            case 'B':
                switch (randNum) {
                    case 1:
                        enemyY--;
                        break;
                    case 2:
                        enemyY++;
                        break;
                    case 3:
                        enemyX++;
                        break;
                    case 4:
                        enemyX--;
                        break;
                }
                break;
            case 'S':
                if (cycleNum <= 2) {
                    enemyX++;
                }
                else if (cycleNum >= 3) {
                    enemyX--;
                }
                break;
            case 'Z':
                if (cycleNum == 1 || cycleNum == 3) {
                    enemyY--;
                }
                else if (cycleNum == 2 || cycleNum == 4) {
                    enemyY++;
                }
                break;
            case 'T':
                if (cycleNum == 1) {
                    enemyY--;
                }
                else if (cycleNum == 2) {
                    enemyX++;
                }
                else if (cycleNum == 3) {
                    enemyY++;
                }
                else if (cycleNum == 4) {
                    enemyX--;
                }
                break;
            default:
                break;
        }
        //update position
        readScreen = mvinch(enemyY, enemyX);
        if (readScreen == '.') {
            enemyInfo[i]->locY = enemyY;
            enemyInfo[i]->locX = enemyX;
            mvaddch(oldEnemyY, oldEnemyX, '.');
            mvaddch(enemyY, enemyX, monsterType);
        }
        else if (readScreen == '@') {
            //enemy attacking
            if (checkAttack == '1') {
                mvprintw(0, 25, "~Press any key to continue.");
                getch();
            }
            mvprintw(0, 0, "Enemy is attacking! Press any key to continue.                                 ");
            getch();
            status = combat(i, playerInfo, enemyInfo);
            if (status == 2) {
                return 1;
            }
            else if (status == 1) {
                mvaddch(oldEnemyY, oldEnemyX, '.');
            }
            else {
                updateStatus(playerInfo);
            }
        }
    }
    drawHero(heroY, heroX);
    return 0;
}
                    

int combat(int enemyID, player * playerInfo, enemy * enemyInfo[]) {
    if (genRand(1, 10) > enemyInfo[enemyID]->speed) {
        enemyInfo[enemyID]->health = enemyInfo[enemyID]->health - playerInfo->attack;
        mvprintw(0, 0, "Hero caused %d damage!                                    ", playerInfo->attack);
    }
    else {
        if (enemyInfo[enemyID]->type == 'A') {
            playerInfo->attack = playerInfo->attack - enemyInfo[enemyID]->attack;
            mvprintw(0, 0, "Hero lost %d attack!                                  ", enemyInfo[enemyID]->attack);
        }
        else {
            playerInfo->health = playerInfo->health - enemyInfo[enemyID]->attack;
            mvprintw(0, 0, "Hero lost %d health!                                  ", enemyInfo[enemyID]->attack);
        }
    }
    //check dead
    if (playerInfo->health < 1) {
        mvprintw(0, 0, "Hero died! Press any key to exit                          ", enemyInfo[enemyID]->attack);
        getch();
        return 2;
    }
    else if (enemyInfo[enemyID]->health < 1) {
        mvprintw(0, 0, "Hero killed %c!                                           ", enemyInfo[enemyID]->type);
        return 1;
    }
    else {
        return 0;
    }
}
 
                     
                     
                     
                     
                     
                     
                     
                     
                     
                     
                     
                     
