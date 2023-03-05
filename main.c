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
    // Room * room;
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

    // Monster ** monsters; // Array to pointers of monsters
    // Item ** items;

} Room;

int screenSetup();
Room ** mapSetup();
Room * createRoom(int, int, int, int);
int drawRoom(Room *);
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
        mvprintw(1,0, "Player input : %c ", ch);
        handleInput(ch, player);
    }
    endwin();
    return 0;
}


/*
    ROOM FUNCTIONS
*/

/**
 * Creates a room pointer and allocates size
 * @param y The Y position of the room (Top)
 * @param x The X position of the room (Left)
 * @param heigth The heigth of the room
 * @param width The width of the room
 * @return A pointer to the room created
*/
Room * createRoom(int y, int x, int heigth, int width) {
 Room * room;
 room = malloc(sizeof(Room));
 room->yPos = y;
 room->xPos = x;
 room->height = heigth;
 room->witdh = width;
 return room;
}

/**
 * Draws a room on the map
 * @param room A pointer to the room that's drawn
 * @return Zero if not failed
*/
int drawRoom(Room * room) {
    for(int i = 0; i <= room->witdh; i++) {
        for(int j = 0; j <= room->height; j++){
            // Draw angles and vertical walls
            if(i == 0 || i == room->witdh) {
                if (j == 0 || j == room->height) {
                    mvprintw(room->yPos+j, room->xPos+i, "O");
                } else {
                    mvprintw(room->yPos+j, room->xPos+i, "|");
                }
            // Draw horizontal walls
            } else if (j == 0 || j == room->height) {
                mvprintw(room->yPos+j, room->xPos+i, "-");
            // Draw floor
            } else {
                mvprintw(room->yPos+j, room->xPos+i, ".");
            }
        }
    }
    return 0;
}



int screenSetup() {
    initscr();
    printw("Roguelike ! (Type 'x' to quit)");
    noecho();
    refresh();

    return 0;
}

/**
 * Setup the whole map
 * @return An array of rooms
*/
Room ** mapSetup() {
    Room ** rooms; // Array of rooms
    rooms = malloc(sizeof(Room) * 6);

    rooms [0] = createRoom(7, 12, 6, 14);
    drawRoom(rooms[0]);
    mvprintw(1,0, "Address of the room 0 : %p", rooms[0]);

    rooms [1] = createRoom(12, 32, 18, 9);
    drawRoom(rooms[1]);
    mvprintw(2,0, "Address of the room 1 : %p", rooms[1]);
    return rooms;
}

/**
 * Setup the player and place it on the map.
 * Todo :
 *     - Make attributes dependant to a class/race system
 *     - Don't hardcode spawn
*/
Player * playerSetup() {
    Player * newPlayer;
    newPlayer = malloc(sizeof(Player));
    newPlayer->xPos = 15;
    newPlayer->yPos = 10;
    newPlayer->health = 25;
    playerMove(10, 15, newPlayer);
    return newPlayer;
}

/**
 * Moves the player on designated location, draw
 * floor back and move cursor on player
 * @param y Y location to move to
 * @param x X location to move to
 * @param player Pointer to the player
 * @return Zero if not failed
*/
int playerMove(int y, int x, Player * player) {
    // Replace old position
    mvprintw(player->yPos, player->xPos, ".");
    // Change player position and move cursor
    player->xPos = x;
    player->yPos = y;
    mvprintw(player->yPos, player->xPos, "@");
    move(player->yPos, player->xPos);

    return 0;
}


/**
 * Check if the move triggered by user input
 * is valid, calls playerMove()
 * @param newY The Y location that's checked
 * @param newX The X location that's checked
 * @param player A pointer to the player
 * @return Zero if not failed
*/
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

/**
 * Handle the user input and calls checkMove() depending
 * on input
 * @param input The char inputed by user
 * @param player Pointer to the player
 * @return Zero if not failed
*/
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