#include <ncurses.h>
#include <stdlib.h>

/**
 * Definition of the Player struct
*/
typedef struct Player
{
    int xPos;
    int yPos;
    int health;

    // Monster ** monsters; // Array to pointers of monsters
    // Item ** items;
} Player;

/**
 * Definition of the Room struct
*/
typedef struct Room
{
    int xPos;
    int yPos;
    int witdh;
    int height;

} Room;

int screenSetup();
Room ** mapSetup();
Room * createRoom(int, int, int, int);
Player * playerSetup();
int handleInput(int, Player *);
int playerMove(int, int, Player *);
int checkMove(int, int, Player *);


int main(void) {

    Player * player;
    int ch; // player input

    screenSetup();

    // Draw map
    mapSetup();    
    // Create player and add it to map
    player = playerSetup(10, 15, 25); 

    while((ch = getch()) != 'x'){ 
        mvprintw(2,1, "Player input : %c ", ch);
        handleInput(ch, player);
    }
    endwin();
    return 0;
}


/*
    ROOM FUNCTIONS
*/

Room * createRoom(int y, int x, int heigth, int width) {
 Room * room;
 room = malloc(sizeof(Room));
 room->yPos = y;
 room->xPos = x;
 room->height = heigth;
 room->witdh = width;

 for(int i = 0; i <= width; i++) {
    for(int j = 0; j <= heigth; j++){
        if(i == 0 || i == width) {
            if (j == 0 || j == heigth) {
                mvprintw(y+j, x+i, "O");
            } else {
                mvprintw(y+j, x+i, "|");
            }
        } else if (j == 0 || j == heigth) {
            mvprintw(y+j, x+i, "-");
        } else {
            mvprintw(y+j, x+i, ".");
        }
    }
 }
 return room;
}


int screenSetup() {
    initscr();
    printw("Roguelike ! (Type 'x' to quit)");
    noecho();
    refresh();

    return 0;
}


Room ** mapSetup() {
    Room ** rooms; // Array of rooms
    rooms = malloc(sizeof(Room) * 6);
    rooms [0] = createRoom(7, 12, 6, 14);
    return 0;
}


Player * playerSetup() {
    Player * newPlayer;
    newPlayer = malloc(sizeof(Player));
    newPlayer->xPos = 15;
    newPlayer->yPos = 10;
    newPlayer->health = 25;
    playerMove(10, 15, newPlayer);
    return newPlayer;
}


int playerMove(int y, int x, Player * player) {
    // Replace old position
    mvprintw(player->yPos, player->xPos, ".");
    // Change player position and move cursor
    player->xPos = x;
    player->yPos = y;
    mvprintw(player->yPos, player->xPos, "@");
    move(player->yPos, player->xPos);
}

int checkMove(int newY, int newX, Player * player) {
    int location = mvinch(newY, newX);
    switch (location)
    {
    case '.':
        playerMove(newY, newX, player);
        break;
    
    default:
        move(player->yPos, player->xPos);
        break;
    }
}

int handleInput (int input, Player * player) {
    int newY;
    int newX;
    switch (input)
    {
    case 'z':
    case 'Z':
        newY = player->yPos-1;
        newX = player->xPos;
        break;
    case 'q':
    case 'Q':
        newY = player->yPos;
        newX = player->xPos-1;
        break;
    case 's':
    case 'S':
        newY = player->yPos+1;
        newX = player->xPos;
        break;
    case 'd':
    case 'D':
        newY = player->yPos;
        newX = player->xPos+1;
        break;
    }
    checkMove(newY, newX, player);
    return 0;
}