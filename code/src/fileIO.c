/**
 * Ryan Claxton (2016)
 * claxtonr@mail.uoguelph.ca
 *
 * See fileIO.h for usage
 */

#include "fileIO.h"

void greeting() {
    //greeting ascii art from "http://patorjk.com/software/taag/#p=display&f=Ogre&t=Welcome%0Ato%20Rogue"
    printf("     __    __     _   \n");
    printf("    / / /\\ \\ \\___| | ___ ___  _ __ ___   ___ \n");
    printf("    \\ \\/  \\/ / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\\n");
    printf("     \\  /\\  /  __/ | (_| (_) | | | | | |  __/ \n");
    printf("      \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|\n\n");
    printf("     _            __ \n");
    printf("    | |_ ___     /__\\ ___   __ _ _   _  ___   \n");
    printf("    | __/ _ \\   / \\/// _ \\ / _` | | | |/ _ \\ \n");
    printf("    | || (_) | / _  \\ (_) | (_| | |_| |  __/  \n");
    printf("     \\__\\___/  \\/ \\_/\\___/ \\__, |\\__,_|\\___|  \n");
    printf("                           |___/   \n\n");
    printf("    ~~~ Min Screen Requirements: 87 X 53 ~~~ \n\n");
    printf("          =============================\n");
    printf("           Press the ENTER to continue\n");
    printf("          =============================\n\n");
    
    getchar();
    
}

void getLevels (char * argv[], room * layout[]) {
    //local variables
    FILE * dataFile;
    char readLine[150];
    int roomNum = 0;

    //check open data file
    dataFile = fopen(argv[1], "r");
    if (dataFile == NULL) {
        printf("Could not open data file: \"%s\"\n", argv[1]);
        exit(1);
    }

    //read variables into array
    while (fgets (readLine, 150, dataFile) != NULL) {
        if (readLine[strlen(readLine) - 1] == '\n') {
            readLine[strlen(readLine) - 1] = '\0';
        }
        //local variables
        int state = 0;
        int counter = 0;
        int numDoors = 0;
        int numElements = 0;
        int numGold = 0;
        
        //parsing and storeing in struct
        for (int i = 0; i <= strlen(readLine); i++) {
            switch (state) {
                case 0: //getting rows
                    if (readLine[i] == 'X') {
                        layout[roomNum]->rows[counter] = '\0';
                        state = 1;
                        counter = 0;
                    }
                    else {
                        if (counter < 2) {
                            layout[roomNum]->rows[counter] = readLine[i];
                            counter++;
                        }
                        else {
                            printf("Incorrect Data File: Case 1\n");
                            exit(1);
                        }
                    }
                    break;
                case 1: //getting cols
                    if (readLine[i] == ' ' || readLine[i] == '\0') {
                        layout[roomNum]->cols[counter] = '\0';
                        state = 2;
                        counter = 0;
                    }
                    else {
                        if (counter < 2) {
                            layout[roomNum]->cols[counter] = readLine[i];
                            counter++;
                        }
                        else {
                            printf("Incorrect Data File: Case 2\n");
                            exit(1);
                        }
                    }
                    break;
                case 2: //check for door or element store and first char
                    if (readLine[i] == 'd') {
                        if (numDoors == 4) {
                            printf("Maximum number of doors exceeded\n");
                            exit(1);
                        }
                        else {
                            layout[roomNum]->doors[numDoors][counter] = readLine[i];
                            state = 3;
                        }
                    }
                    else if ((isalpha(readLine[i]))) {
                        if (numElements == 10)
                        {
                            printf("Maximum number of elements exceeded\n");
                            exit(1);
                        }
                        else {
                            if (readLine[i] == 'g' || readLine[i] == 'G') numGold++;
                            layout[roomNum]->elements[numElements][counter] = readLine[i];
                            state = 4;
                        }
                    }
                    else {
                        printf("Incorrect Data File\n");
                        printf("error char = %c%c%c\n",readLine[i-1],readLine[i],readLine[i+1]);
                        exit(1);
                    }
                    counter++;
                    break;
                case 3: //store remaining door info return to state 2
                    if (readLine[i] == ' ' || readLine[i] == '\0') {
                        layout[roomNum]->doors[numDoors][counter] = '\0';
                        counter = 0;
                        state = 2;
                        numDoors++;
                    }
                    else {
                        layout[roomNum]->doors[numDoors][counter] = readLine[i];
                        counter++;
                    }
                    break;
                case 4: //store remaining element info and return to state 2
                    if (readLine[i] == ' ' || readLine[i] == '\0') {
                        layout[roomNum]->elements[numElements][counter] = '\0';
                        counter = 0;
                        state = 2;
                        numElements++;
                    }
                    else {
                        layout[roomNum]->elements[numElements][counter] = readLine[i];
                        counter++;
                    }
                    break;
            }
        }
        layout[roomNum]->numDoors = numDoors;
        layout[roomNum]->numElements = numElements;
        layout[roomNum]->numGold = numGold;
        roomNum++;
        if (roomNum >= 6) break; //ignores anything more then 6 rooms
    }
    fclose(dataFile);
}

void endGameText() {
    //game over ascii art from "http://patorjk.com/software/taag/#p=display&f=Ogre&t=GAME%20OVER"
    printf("   ___   _              __     ___         __  __  \n");
    printf("  / _ \\ /_\\    /\\/\\    /__\\   /___\\/\\   /\\/__\\/__\\ \n");
    printf(" / /_\\///_\\\\  /    \\  /_\\    //  //\\ \\ / /_\\ / \\// \n");
    printf("/ /_\\\\/  _  \\/ /\\/\\ \\//__   / \\_//  \\ V //__/ _  \\  \n");
    printf("\\____/\\_/ \\_/\\/    \\/\\__/   \\___/    \\_/\\__/\\/ \\_/  \n");
}

void doorGen(char doors[100], int numDoors, int width, int height){
    int temp = 0;
    char temp2[8];
    switch (numDoors) {
        case 1:
            temp = genRand(1,4);
            switch (temp) {
                case 1: //north
                    strcat(doors,"dn");
                    sprintf(temp2,"%d",genRand(0,width-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    break;
                case 2: //south
                    strcat(doors,"ds");
                    sprintf(temp2,"%d",genRand(0,width-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    break;
                case 3: //east
                    strcat(doors,"de");
                    sprintf(temp2,"%d",genRand(0,height-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    break;
                case 4: //west
                    strcat(doors,"dw");
                    sprintf(temp2,"%d",genRand(0,height-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    break;
            }
            break;
        case 2:
            temp = genRand(1,6);
            switch (temp) {
                case 1: //ns
                    strcat(doors,"dn");
                    sprintf(temp2,"%d",genRand(0,width-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    strcat(doors,"ds");
                    sprintf(temp2,"%d",genRand(0,width-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    break;
                case 2: //nw
                    strcat(doors,"dn");
                    sprintf(temp2,"%d",genRand(0,width-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    strcat(doors,"dw");
                    sprintf(temp2,"%d",genRand(0,height-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    break;
                case 3: //ne
                    strcat(doors,"dn");
                    sprintf(temp2,"%d",genRand(0,width-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    strcat(doors,"de");
                    sprintf(temp2,"%d",genRand(0,height-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    break;
                case 4: //sw
                    strcat(doors,"ds");
                    sprintf(temp2,"%d",genRand(0,width-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    strcat(doors,"dw");
                    sprintf(temp2,"%d",genRand(0,height-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    break;
                case 5: //se
                    strcat(doors,"ds");
                    sprintf(temp2,"%d",genRand(0,width-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    strcat(doors,"de");
                    sprintf(temp2,"%d",genRand(0,height-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    break;
                case 6: //ew
                    strcat(doors,"de");
                    sprintf(temp2,"%d",genRand(0,height-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    strcat(doors,"dw");
                    sprintf(temp2,"%d",genRand(0,height-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    break;
            }
            break;
        case 3:
            temp = genRand(1,4);
            switch (temp) {
                case 1: //nse
                    strcat(doors,"dn");
                    sprintf(temp2,"%d",genRand(0,width-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    strcat(doors,"ds");
                    sprintf(temp2,"%d",genRand(0,width-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    strcat(doors,"de");
                    sprintf(temp2,"%d",genRand(0,height-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    break;
                case 2: //sew
                    strcat(doors,"ds");
                    sprintf(temp2,"%d",genRand(0,width-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    strcat(doors,"de");
                    sprintf(temp2,"%d",genRand(0,height-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    strcat(doors,"dw");
                    sprintf(temp2,"%d",genRand(0,height-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    break;
                case 3: //new
                    strcat(doors,"dn");
                    sprintf(temp2,"%d",genRand(0,width-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    strcat(doors,"de");
                    sprintf(temp2,"%d",genRand(0,height-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    strcat(doors,"dw");
                    sprintf(temp2,"%d",genRand(0,height-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    break;
                case 4: //nsw
                    strcat(doors,"dn");
                    sprintf(temp2,"%d",genRand(0,width-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    strcat(doors,"ds");
                    sprintf(temp2,"%d",genRand(0,width-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    strcat(doors,"dw");
                    sprintf(temp2,"%d",genRand(0,height-1));
                    strcat(doors,temp2);
                    strcat(doors," ");
                    break;
            }
            break;
        case 4:
            strcat(doors,"dn");
            sprintf(temp2,"%d",genRand(0,width-1));
            strcat(doors,temp2);
            strcat(doors," ");
            strcat(doors,"ds");
            sprintf(temp2,"%d",genRand(0,width-1));
            strcat(doors,temp2);
            strcat(doors," ");
            strcat(doors,"de");
            sprintf(temp2,"%d",genRand(0,height-1));
            strcat(doors,temp2);
            strcat(doors," ");
            strcat(doors,"dw");
            sprintf(temp2,"%d",genRand(0,height-1));
            strcat(doors,temp2);
            strcat(doors," ");
            break;
    }
}

void genItemMonsters(char itMons[100], int numItMons, int width, int height){
    int prev[10];
    int temp;
    int count = 0;
    int checkTemp;
    char toPrint[10];
    int widthLoc, heightLoc;
    bool locCheck = false;
    for (int i = 0; i < numItMons; i++) {
        temp = genRand(1,10);
        //draw elements
        switch (temp) {
            case 1:
                strcat(itMons,"z");
                break;
            case 2:
                strcat(itMons,"a");
                break;
            case 3:
                strcat(itMons,"g");
                break;
            case 4:
                strcat(itMons,"G");
                break;
            case 5:
                strcat(itMons,"w");
                break;
            case 6:
                strcat(itMons,"W");
                break;
            case 7:
                strcat(itMons,"e");
                break;
            case 8:
                strcat(itMons,"m");
                break;
            case 9:
                strcat(itMons,"M");
                break;
            case 10:
                strcat(itMons,"P");
                break;
        }
        do {
            //generate location
            locCheck = true;
            widthLoc = genRand(0,width-1);
            heightLoc = genRand(0,height-1);
            checkTemp = widthLoc + heightLoc * (heightLoc - widthLoc);
            //check taken
            for (int i = 0; i < count; i++) {
                if (checkTemp == prev[i])
                    locCheck = false;
            }
        } while (locCheck == false);
        sprintf(toPrint,"%d,%d ",heightLoc,widthLoc);
        strcat(itMons,toPrint);
        prev[count] = checkTemp;
        count++;
    }
}

void generate(char room[100]) {
    int width = genRand(10,23);
    int height = genRand(10,18);
    int numDoors = genRand(1,4);
    int numItemMonsters = genRand(0,10);
    char temp2[5];
    sprintf(temp2,"%d",height);
    strcpy(room, temp2);
    strcat(room, "X");
    sprintf(temp2,"%d",width);
    strcat(room, temp2);
    strcat(room, " ");
    doorGen(room, numDoors, width, height);
    genItemMonsters(room, numItemMonsters, width, height);
}

