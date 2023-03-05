#include <ncurses.h>
#include <stdlib.h>
#include <time.h>



typedef struct Position {
    int x;
    int y;
    // TILE_TYPE
} Position;

/**
 * Definition of the Player struct
*/
typedef struct Player
{
    Position position;
    int health;
    // Room * room;
} Player;

/**
 * Definition of the Room struct
*/
typedef struct Room
{
    Position position;
    int witdh;
    int height;

    Position ** door;
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

    srand(time(NULL));
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
    room->position.y = y;
    room->position.x = x;
    room->height = heigth;
    room->witdh = width;
    room->door = malloc(sizeof(Position) * 4);
    
    // Top door
    int topDoorX = rand() % (width - 2) + x +1;
    
    
    room->door[0] = malloc(sizeof(Position));
    room->door[0]->x = topDoorX;
    room->door[0]->y = y;

    // Bottom door
    int botDoorX = rand() % (width - 2) + x +1;
    room->door[1] = malloc(sizeof(Position));
    room->door[1]->x = botDoorX;
    room->door[1]->y = heigth + y;

    // Left door
    int leftDoorY = rand() % (heigth - 2) + y + 1;
    room->door[2] = malloc(sizeof(Position));
    room->door[2]->x = x;
    room->door[2]->y = leftDoorY;

    // Right door
    int rightDoorY = rand() % (heigth - 2) + y + 1;
    room->door[3] = malloc(sizeof(Position));
    room->door[3]->x = width + x;
    room->door[3]->y = rightDoorY;
 
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
                    mvprintw(room->position.y+j, room->position.x+i, "O");
                } else {
                    mvprintw(room->position.y+j, room->position.x+i, "|");
                }
            // Draw horizontal walls
            } else if (j == 0 || j == room->height) {
                mvprintw(room->position.y+j, room->position.x+i, "-");
            // Draw floor
            } else {
                mvprintw(room->position.y+j, room->position.x+i, ".");
            }
        }
        // Top door draw
        mvprintw(room->door[0]->y, room->door[0]->x, "+");
        // Bottom door draw
        mvprintw(room->door[1]->y, room->door[1]->x, "+");
        // Left door draw
        mvprintw(room->door[2]->y, room->door[2]->x, "+");
        // Right door draw
        mvprintw(room->door[3]->y, room->door[3]->x, "+");
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
    mvprintw(2,0, "Address of the room 0 : %p", rooms[0]);

    rooms [1] = createRoom(12, 32, 18, 9);
    drawRoom(rooms[1]);
    mvprintw(3,0, "Address of the room 1 : %p", rooms[1]);
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
    newPlayer->position.x = 15;
    newPlayer->position.y = 10;
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
    mvprintw(player->position.y, player->position.x, ".");
    // Change player position and move cursor
    player->position.x = x;
    player->position.y = y;
    mvprintw(player->position.y, player->position.x, "@");
    move(player->position.y, player->position.x);

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
        move(player->position.y, player->position.x);
        break;
    }
    return 0;
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
        newY = player->position.y-1;
        newX = player->position.x;
        break;
    case 'q':
    case 'Q':
        newY = player->position.y;
        newX = player->position.x-1;
        break;
    case 's':
    case 'S':
        newY = player->position.y+1;
        newX = player->position.x;
        break;
    case 'd':
    case 'D':
        newY = player->position.y;
        newX = player->position.x+1;
        break;
    }
    checkMove(newY, newX, player);
    return 0;
}