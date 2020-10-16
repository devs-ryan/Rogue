/**
 * Ryan Claxton (2016)
 * claxtonr@mail.uoguelph.ca
 *
 * See rogueInit.h for usage
 */

#include "rogueInit.h"

void initGame(room * level[], gold * levelGold[]) {
    int rows;
    int cols;
    
    initscr(); //initalize ncurses
    noecho();
    cbreak();
    curs_set(0);
    
    getmaxyx(stdscr, rows, cols); //check min screen size
    if (rows < 53 || cols < 87) {
        endwin();
        printf("Minimum screen size requirments not met,\nTry again.\n");
        exit(1);
    }
    
    //print levels
    printRoom(level[0], levelGold[0], 14, 14);
    printRoom(level[1], levelGold[1], 14, 42);
    printRoom(level[2], levelGold[2], 14, 70);
    printRoom(level[3], levelGold[3], 37, 14);
    printRoom(level[4], levelGold[4], 37, 42);
    printRoom(level[5], levelGold[5], 37, 70);
    connectHalls();
    
}

void printRoom(room drawRoom[], gold fillGold[], int y, int x) {
    //get dimensions and origin
    int height = atoi (drawRoom->rows);
    int width = atoi (drawRoom->cols);
    if ((height > 18) || (width > 23)
        || (height < 10) || (width < 10)) {
        endwin();
        printf("Invalid Room Size,\nTry again.\n");
        exit(1);
    }
    int yOrigin = findOrigin(height, y);
    int xOrigin = findOrigin(width, x);
    drawRoom->yOrigin = yOrigin;
    drawRoom->xOrigin = xOrigin;
    
    //generate level
    drawFloor(xOrigin, yOrigin, width, height);
    drawWalls(xOrigin, yOrigin, width, height);
    drawDoors(drawRoom, xOrigin, yOrigin, width, height);
    drawElements(drawRoom, fillGold, xOrigin, yOrigin, width, height);

}

int findOrigin(int dimension, int centre) {
    int origin;
    //check for odd
    if (dimension % 2 == 0) {
         origin = centre - (dimension/2);
    }
    else {
        origin = centre - (dimension/2) + 1; //if odd divide by 2 add 1
    }
    return origin;
}

void drawFloor(int x, int y, int w, int h) {
    for (int i = x; i < (w + x); i++) {
        for (int j = y; j < (h + y); j++) {
            mvaddch(j, i, '.');
        }
    }
}

void drawWalls(int x, int y, int w, int h) {
    for (int i = y - 1; i < (h + y + 1); i++) {
        mvaddch(i, x - 1, '|');
        mvaddch(i, w + x, '|');
    }
    for (int i = x - 1; i < (w + x + 1); i++) {
        mvaddch(y - 1, i, '-');
        mvaddch(h + y, i, '-');
    }
}

void drawDoors(room drawDoors[], int x, int y, int w, int h) {
    //number of doors to draw
    int numDoors = drawDoors->numDoors;
    int readY;
    char roomID;
    for (int i = 0; i < numDoors; i++) { //loop
        char d = drawDoors->doors[i][1];
        char temp[3];
        int loc = atoi(strcpy(temp, &drawDoors->doors[i][2]));
        
        //check doors are valid on wall
        if ((d == 'n' || d == 's') && loc > (w - 1)) {
            endwin();
            printf("Invalid Door Location,\nTry again.\n");
            exit(1);
        }
        else if  ((d == 'e' || d == 'w') && loc > (h - 1)){
            endwin();
            printf("Invalid Door Location,\nTry again.\n");
            exit(1);
        }
        
        //check wall and draw door
        switch (d) {
            case 'n':
                mvaddch(y - 1, x + loc, '+');
                readY = y - 1;
                break;
            case 'e':
                mvaddch(y + loc, x + w, '+');
                readY = y + loc;
                break;
            case 's':
                mvaddch(y + h, x + loc, '+');
                readY = y + h;
                break;
            case 'w':
                mvaddch(y + loc, x - 1, '+');
                readY = y + loc;
                break;
        }
        //draw halls
        
        if (readY > 25) {
            roomID = 's';
        }
        else {
            roomID = 'n';
        }
        drawHalls(roomID, d, w, loc, y, x);
    }
}

void drawElements(room drawElements[], gold fillGold[], int x, int y, int w, int h) {
    int numElements = drawElements->numElements;
    int goldCount = 0;
    for (int i = 0; i < numElements; i++) {
        //variables
        char e = drawElements->elements[i][0];
        char temp[6];
        int locX;
        int locY;
        char monster;
        
        //get location X and Y from mid string
        if (drawElements->elements[i][2] == ',') {
            locY = drawElements->elements[i][1] - '0';
            locX = atoi(strcpy(temp, &drawElements->elements[i][3]));
        }
        else {
            locY = atoi(strncpy(temp, &drawElements->elements[i][1],2));
            locX = atoi(strcpy(temp, &drawElements->elements[i][4]));
        }
        //check to make sure element is on map
        if ((locX >= w) || (locY >= h)) {
            endwin();
            printf("Invalid Element Location,\nTry again.\n");
            exit(1);
        }
        //draw elements
        switch (e) {
            case 'z':
                mvaddch(y + locY, x + locX, '<');
                break;
            case 'a':
                mvaddch(y + locY, x + locX, '>');
                break;
            case 'g':
                mvaddch(y + locY, x + locX, '*');
                //fill gold struct
                fillGold->locX[goldCount] = (x + locX);
                fillGold->locY[goldCount] = (y + locY);
                srand(time(0) + i);
                fillGold->amount[goldCount] = genRand(0, 50);
                goldCount++;
                break;
            case 'G':
                mvaddch(y + locY, x + locX, '8');
                //fill gold struct
                fillGold->locX[goldCount] = (x + locX);
                fillGold->locY[goldCount] = (y + locY);
                srand(time(0) + i);
                fillGold->amount[goldCount] = genRand(50, 250);
                goldCount++;
                break;
            case 'w':
                mvaddch(y + locY, x + locX, ')');
                break;
            case 'W':
                mvaddch(y + locY, x + locX, '(');
                break;
            case 'e':
                mvaddch(y + locY, x + locX, ']');
                break;
            case 'm':
                srand(time(0) + i);
                switch (genRand(1, 4)) {
                    case 1:
                        monster = 'A';
                        break;
                    case 2:
                        monster = 'B';
                        break;
                    case 3:
                        monster = 'Z';
                        break;
                    case 4:
                        monster = 'S';
                        break;
                }
                mvaddch(y + locY, x + locX, monster);
                break;
            case 'M':
                mvaddch(y + locY, x + locX, 'T');
                break;
            case 'p':
                mvaddch(y + locY, x + locX, '!');
                break;
        }
    }
}

int genRand(int min, int max) {
    int randNum = (rand()%(max-min+1))+min;
    return randNum;
}

void drawHalls(char roomLoc, char wallLoc, int roomWidth, int doorLoc, int yOrigin, int xOrigin) {
    int screenY;
    int screenX;
    getyx(stdscr, screenY, screenX);
    screenX = screenX - 1;
    
    //top row room doors
    if (roomLoc == 'n') {
        switch (wallLoc) {
            case 'n':
                mvaddch(screenY - 1, screenX, '#');
                if (doorLoc < (roomWidth / 2)) {
                    for (int i  = screenX; i > (xOrigin - 4) ; i--) {
                        mvaddch(screenY - 2, i, '#');
                    }
                    for (int i = screenY - 2; i < 26; i++) {
                        mvaddch(i, xOrigin - 3, '#');
                    }
                }
                else {
                    for (int i  = screenX; i < (xOrigin + roomWidth + 3) ; i++) {
                        mvaddch(screenY - 2, i, '#');
                    }
                    for (int i = screenY - 2; i < 26; i++) {
                        mvaddch(i, xOrigin + roomWidth + 2, '#');
                    }
                }
                break;
            case 'e':
                mvaddch(screenY, screenX + 1, '#');
                for (int i = screenY; i < 26; i++) {
                    mvaddch(i, screenX + 2, '#');
                }
                break;
            case 's':
                for (int i = screenY + 1; i < 26; i++) {
                    mvaddch(i, screenX, '#');
                }
                break;
            case 'w':
                mvaddch(screenY, screenX - 1, '#');
                for (int i = screenY; i < 26; i++) {
                    mvaddch(i, screenX - 2, '#');
                }
                break;
        }
    }
    //bottom row room doors
    if (roomLoc == 's') {
        switch (wallLoc) {
            case 'n':
                for (int i = screenY - 1; i > 24; i--) {
                    mvaddch(i, screenX, '#');
                }
                break;
            case 'e':
                mvaddch(screenY, screenX + 1, '#');
                for (int i = screenY; i > 24; i--) {
                    mvaddch(i, screenX + 2, '#');
                }
                break;
            case 's':
                mvaddch(screenY + 1, screenX, '#');
                if (doorLoc < (roomWidth / 2)) {
                    for (int i  = screenX; i > (xOrigin - 4) ; i--) {
                        mvaddch(screenY + 2, i, '#');
                    }
                    for (int i = screenY + 2; i > 24; i--) {
                        mvaddch(i, xOrigin - 3, '#');
                    }
                }
                else {
                    for (int i  = screenX; i < (xOrigin + roomWidth + 3) ; i++) {
                        mvaddch(screenY + 2, i, '#');
                    }
                    for (int i = screenY + 2; i > 24; i--) {
                        mvaddch(i, xOrigin + roomWidth + 2, '#');
                    }
                }
                break;
            case 'w':
                mvaddch(screenY, screenX - 1, '#');
                for (int i = screenY; i > 24; i--) {
                    mvaddch(i, screenX - 2, '#');
                }
                break;
        }
    }
}

void connectHalls() {
	int checkSwitch = 0;
	int startX, endX;
	for (int i = 0; i < 88; i++) {
		if (checkSwitch == 0) {
			if (mvinch(25, i) == '#') {
				startX = i;
				checkSwitch = 1;
			}
		}
		else {
			if (mvinch(25, i) == '#') {
				endX = i;
			}
		}
	}
	for (int i = startX; i < endX; i++) {
		mvaddch(25, i, '#');
	}
}


