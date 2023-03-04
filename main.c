#include <ncurses.h>
#include <stdlib.h>


typedef struct Player
{
    int xPos;
    int yPos;
    int health;
} Player;


int screenSetup();
int mapSetup();
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




int screenSetup() {
    initscr();
    printw("Roguelike ! (Type 'x' to quit)");
    noecho();
    refresh();

    return 0;
}


int mapSetup() {
    mvprintw(7,12,  "O------------O");  // mvprintw(yAxis, xAxis, stringToPrint);
    mvprintw(8,12,  "|............|");
    mvprintw(9,12,  "|............|");
    mvprintw(10,12, "|............|");
    mvprintw(11,12, "|............|");
    mvprintw(12,12, "O------------O");

    mvprintw(4,35, "O------------O");  // mvprintw(yAxis, xAxis, stringToPrint);
    mvprintw(5,35, "|............|");
    mvprintw(6,35, "|............|");
    mvprintw(7,35, "|............|");
    mvprintw(8,35, "|............|");
    mvprintw(9,35, "O------------O");

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
        break;
    }
    move(player->yPos, player->xPos);
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