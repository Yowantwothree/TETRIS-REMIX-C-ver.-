#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "tetris_mech.h"

// To use ncurses library for terminal graphics compatible across platforms
#include <ncurses/ncurses.h>

// To play music using SDL2 library compatible across platforms
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// USER DEFINED FUNCTIONS
// FOR UI DESIGN
// to print on a specific (x,y) coordinate of the terminal
void move_c(int x, int y)
{
    move(y, x); // for the programmer's ease
}

// to draw the main frame of the program
void draw_UI()
{
    // top
    move_c(0, 0);
    printw("[][][][][][][][][][]");
    move_c(mid_x - 2, 0);
    printw("========================");
    move_c(right_x, 0);
    printw("[][][][][][][][][][][][][][][][][]");

    // side
    for (int i = 1; i < left_y - 1; i++)
    {
        move_c(0, i);
        printw("[]");
        move_c(left_width + left_x - 2, i);
        printw("[]");
        move_c(right_x, i);
        printw("[]");
        move_c(right_width + right_x - 2, i);
        printw("[]");
    }

    // insides
    for (int i = 1; i < mid_y - 1; i++)
    {
        move_c(2, i);
        printw("                ");
        move_c(mid_x - 1, i);
        printw("| . . . . . . . . . .|");
        move_c(right_x + 2, i);
        printw("                              ");
    }

    // bottom
    move_c(0, left_y - 1);
    printw("[][][][][][][][][][]");
    move_c(mid_x - 2, mid_y - 1);
    printw("========================");
    move_c(right_x, right_y - 1);
    printw("[][][][][][][][][][][][][][][][][]");

    refresh();
}

// for left column, includes how to's and directions
void draw_left_menu()
{
    int alpha = 2;
    move_c(2, alpha);
    printw(" [TETRIS REMIX] ");
    move_c(2, alpha + 2);
    printw("  [P] Pause");
    move_c(2, alpha + 3);
    printw("  [0] Quit");

    int a = 7;
    move_c(2, a);
    printw("   [CONTROLS]");
    move_c(2, a + 2);
    printw(" [A] MOVE LEFT");
    move_c(2, a + 3);
    printw(" [D] MOVE RIGHT");
    move_c(2, a + 4);
    printw(" [S] MOVE DOWN");
    move_c(2, a + 5);
    printw(" [W/R] ROTATE");
    move_c(2, a + 6);
    printw(" [SPACE] QUICK");

    refresh();
}

// for gimmicks
// returns the main mechanics of the game based on the level
char *mechanic(int track)
{
    switch (track)
    { //"                          "
    case 1:
        return "8-BIT: NORMAL GAMEMODE    ";
    case 31:
        return "No gimmick from 8-bit     ";
    case 2:
        return "HYPER POP: POP TILES      ";
    case 32:
        return "Randomly pop tiles";
    case 3:
        return "KDA: SUMMON SPOTLIGHTS    ";
    case 33:
        return "Spotlights speed up block ";
    case 4:
        return "PIANO: CALMS THE BOARD    ";
    case 34:
        return "Gravity resets to initial ";
    case 5:
        return "EMO: FLOOD WITH TEARS     ";
    case 35:
        return "Only 5 top lines visible  ";
    case 6:
        return "TRUE DAMAGE: MIC DROPS    ";
    case 36:
        return "Bonus score for 3-4 clear ";
    case 7:
        return "JAZZ: TUNE OUT GAPS       ";
    case 37:
        return "Fill gaps in solid blocks ";
    case 8:
        return "ILLBEATS: RAISES THE BEATS";
    case 38:
        return "Raise current solid blocks";
    case 9:
        return "DISCO: GROOVY WAVES       ";
    case 39:
        return "Move blocks to right/left ";
    case 10:
        return "PUNK: BREAK THE RULES     ";
    case 40:
        return "+scores 4 scoreless drops ";
    case 11:
        return "EDM: LASER RAVE MANIA     ";
    case 41:
        return "Laser destroys columns    ";
    case 12:
        return "PENTAKILL: FIFTH DEATH    ";
    case 42:
        return "XTRA clear from 4 clears  ";
    case 13:
        return "COUNTRY: GRIME DUST CRIME ";
    case 43:
        return "Hides alternating rows    ";
    case 14:
        return "HEARTSTEEL: HEART ROBS    ";
    case 44:
        return "Destroy solid with hearts ";
    case 15:
        return "MIXMASTER: BOARD REMIX    ";
    case 45:
        return "Remix game with effects   ";
    case 16:
        return "MAESTRO: THE VIRTUOSO     ";
    case 46:
        return "MASTER OF THE GAME        ";
    }
}

// tracks included per level
char *track_title(int level)
{
    switch (level)
    {
    case 1:
        return "8-BIT";
    case 2:
        return "HYPER POP";
    case 3:
        return "8-BIT X HYPER POP";
    case 4:
        return "KDA";
    case 5:
        return "8-BIT X KDA";
    case 6:
        return "PIANO";
    case 7:
        return "8-BIT X KDA X HYPERPOP";
    case 8:
        return "EMO";
    case 9:
        return "EMO X HYPERPOP X KDA";
    case 10:
        return "TRUEDAMAGE";
    case 11:
        return "TRUE DAMAGE X PIANO X EMO";
    case 12:
        return "JAZZ";
    case 13:
        return "JAZZ X EMO";
    case 14:
        return "ILLBEATS";
    case 15:
        return "ILLBEATS X JAZZ";
    case 16:
        return "DISCO";
    case 17:
        return "DISCO X JAZZ X HYPERPOP";
    case 18:
        return "PUNK";
    case 19:
        return "PUNK X KDA";
    case 20:
        return "EDM";
    case 21:
        return "EDM X HYPERPOP X EMO";
    case 22:
        return "PENTAKILL";
    case 23:
        return "PENTAKILL X JAZZ X PUNK";
    case 24:
        return "COUNTRY";
    case 25:
        return "COUNTRY X PENTAKILL";
    case 26:
        return "HEARTSTEEL";
    case 27:
        return "HEARTSTEEL X KDA X TRUE";
    case 28:
        return "MIXMASTER";
    case 29:
        return "MIXMASTER X EDM X H-POP";
    case 30:
        return "MAESTRO";
    default:
        return "N/A";
    }
}

// for the mechanics of the gimmicks
void mech(int level)
{
    // to print the mechanics of the gimmicks on the right column
    int a, b, c;
    switch (level)
    {
    case 1:
        a = 1;
        b = 0;
        c = 0;
        break;
    case 2:
        a = 2;
        b = 0;
        c = 0;
        break;
    case 3:
        a = 1;
        b = 2;
        c = 0;
        break;
    case 4:
        a = 3;
        b = 0;
        c = 0;
        break;
    case 5:
        a = 1;
        b = 3;
        c = 0;
        break;
    case 6:
        a = 4;
        b = 0;
        c = 0;
        break;
    case 7:
        a = 1;
        b = 2;
        c = 3;
        break;
    case 8:
        a = 5;
        b = 0;
        c = 0;
        break;
    case 9:
        a = 5;
        b = 2;
        c = 3;
        break;
    case 10:
        a = 6;
        b = 0;
        c = 0;
        break;
    case 11:
        a = 6;
        b = 5;
        c = 4;
        break;
    case 12:
        a = 7;
        b = 0;
        c = 0;
        break;
    case 13:
        a = 7;
        b = 5;
        c = 0;
        break;
    case 14:
        a = 8;
        b = 0;
        c = 0;
        break;
    case 15:
        a = 8;
        b = 7;
        c = 0;
        break;
    case 16:
        a = 9;
        b = 0;
        c = 0;
        break;
    case 17:
        a = 9;
        b = 7;
        c = 2;
        break;
    case 18:
        a = 10;
        b = 0;
        c = 0;
        break;
    case 19:
        a = 10;
        b = 3;
        c = 0;
        break;
    case 20:
        a = 11;
        b = 0;
        c = 0;
        break;
    case 21:
        a = 11;
        b = 2;
        c = 5;
        break;
    case 22:
        a = 12;
        b = 0;
        c = 0;
        break;
    case 23:
        a = 12;
        b = 7;
        c = 10;
        break;
    case 24:
        a = 13;
        b = 0;
        c = 0;
        break;
    case 25:
        a = 13;
        b = 12;
        c = 0;
        break;
    case 26:
        a = 14;
        b = 0;
        c = 0;
        break;
    case 27:
        a = 14;
        b = 3;
        c = 6;
        break;
    case 28:
        a = 15;
        b = 0;
        c = 0;
        break;
    case 29:
        a = 15;
        b = 11;
        c = 2;
        break;
    case 30:
        a = 16;
        b = 0;
        c = 0;
        break;
    }

    move_c(right_x + 4, 13);
    printw("%s", mechanic(a));
    move_c(right_x + 4, 14);
    printw("%s", mechanic(a + 30));
    if (b)
    {
        move_c(right_x + 4, 16);
        printw("%s", mechanic(b));
        move_c(right_x + 4, 17);
        printw("%s", mechanic(b + 30));
        if (c)
        {
            move_c(right_x + 4, 19);
            printw("%s", mechanic(c));
            move_c(right_x + 4, 20);
            printw("%s", mechanic(c + 30));
        }
    }
}

// for right column, includes game general info, time, current & next tetromino, and other changing info in-game
void draw_right_menu(const int seconds, int current, int next, int score, int mode, int level, int highscore)
{
    // timers, levels, and scores
    move_c(right_x + 2, 2);
    printw("  Time: %02d:%02d", seconds / 60, seconds % 60);
    move_c(right_x + 2, 4);
    printw("  Score:     %s:", mode == 1 ? "Highscore" : "Goal"); // either prints the gamemode's goal score or user's highscore
    move_c(right_x + 2, 5);
    // starting at 2000, then +200 for each level, and +1000 * level/10 if level is multiple of 10
    int goal = 2500 + (level - 1) * 250 + (level % 10 == 0) * 1000 * (level / 10);
    printw("  %06d      %06d", score, mode == 1 ? highscore : goal);

    // show the current and next tetromino
    char pxl[3] = {"[]"};
    char none[3] = {"  "};
    move_c(right_x + 4, 7);
    printw("Current:     Next:");
    move_c(right_x + 4, 8);
    printw("  %s%s       ", (current % 6 == 0 || current % 2 == 1) ? pxl : none, (current % 6 == 0 || current == 1 || current == 2) ? pxl : none);
    printw("  %s%s  ", (next % 6 == 0 || next % 2 == 1) ? pxl : none, (next % 6 == 0 || next == 1 || next == 2) ? pxl : none);
    move_c(right_x + 4, 9);
    printw("%s%s%s%s     ", (current % 2 == 0 && current != 6 || current == 5) ? pxl : none, (current != 1) ? pxl : none, current != 0 ? pxl : none, (current % 2 == 1 && current != 5 || current == 4) ? pxl : none);
    printw("%s%s%s%s", (next % 2 == 0 && next != 6 || next == 5) ? pxl : none, (next != 1) ? pxl : none, next != 0 ? pxl : none, (next % 2 == 1 && next != 5 || next == 4) ? pxl : none);
    refresh();

    // if the game uses goals/level than highscore, also print the level mechanics
    if (level)
    {
        move_c(right_x + 4, 11);
        printw("LEVEL%02d - %s", level, track_title(level));
        mech(level);
    }
}

// notification within the grid
void notify()
{
    // print top
    move_c(mid_x, 8);
    printw("[][][][][][][][][][]");
    // sides and insides
    for (int i = 9; i < 13; i++)
    {
        move_c(mid_x, i);
        printw("[]");
        move_c(mid_x + 2, i);
        printw("                ");
        move_c(mid_width + mid_x - 6, i);
        printw("[]");
    }
    // bottom
    move_c(mid_x, 13);
    printw("[][][][][][][][][][]");

    refresh();
}

// when 'P' is pressed
void pause_game()
{
    notify();
    move_c(mid_x + 2, 10);
    printw("      GAME      ");
    move_c(mid_x + 2, 11);
    printw("     PAUSED     ");

    refresh();
}

// when game is over
void game_over()
{
    notify();
    move_c(mid_x + 2, 10);
    printw("      GAME      ");
    move_c(mid_x + 2, 11);
    printw("      OVER      ");

    refresh();
}

// confirmation to players, returns 1 if yes, else 0
int confirmation()
{
    notify();
    move_c(mid_x + 2, 10);
    printw(" Are You Sure?? ");
    move_c(mid_x + 2, 11);
    printw(" [1] YES [0] NO ");

    refresh();
    nodelay(stdscr, FALSE);
    int confirm = '2';
    do
    {
        confirm = getch();
    } while (confirm != '1' && confirm != '0');
    return confirm == '1' ? 1 : 0; // change key '1' to int 1, and to 0
}

// game title screen animation
void open_game()
{
    for (int i = 0; i < 12; i++)
    {
        move_c(0, 0);
        if (i > 10)
            printw("[][][]  [][][]  [][][]  [][][]  [] [][][]           \n");
        if (i > 9)
            printw("  []    []        []    []  []  [] []                 \n");
        if (i > 8)
            printw("  []    [][]      []    [][][]  [] [][][]            \n");
        if (i > 7)
            printw("  []    []        []    [][]    []     []            \n");
        if (i > 6)
            printw("  []    [][][]    []    []  []  [] [][][]              \n");
        if (i > 5)
            printw("                                                        \n");
        if (i > 4)
            printw("[][][]  [][][]  []      []  [] []      []              \n");
        if (i > 3)
            printw("[]  []  []      [][]  [][]  []   []  []                 \n");
        if (i > 2)
            printw("[][][]  [][]    []  []  []  []     []                   \n");
        if (i > 1)
            printw("[][]    []      []      []  []   []  []                  \n");
        if (i > 0)
            printw("[]  []  [][][]  []      []  [] []      []                \n");

        // For speed of animation and a posed delay
        int x = 200;
        if (i > 10)
            x += 2800;
        refresh();
        napms(x);
    }
    clear();
    refresh();
}

// for main menu where player can choose game modes, see their progress and other settings
void draw_main_menu(user player)
{
    // left main menu
    int alpha = 2;
    move_c(2, alpha);
    printw("   [MAIN MENU]  ");
    move_c(2, alpha + 2);
    printw(" [1] CAMPAIGN");
    move_c(2, alpha + 3);
    printw("     LEVEL");
    move_c(2, alpha + 5);
    printw(" [2] ENDLESS");
    move_c(2, alpha + 6);
    printw("     REMIX");
    alpha++;
    move_c(2, alpha + 7);
    printw(" [3] ENDLESS");
    move_c(2, alpha + 8);
    printw("     CLASSIC");
    move_c(2, alpha + 10);
    printw(" [4] HOW TO");
    move_c(2, alpha + 11);
    printw("     PLAY");
    move_c(2, alpha + 13);
    printw(" [5] SETTINGS");
    move_c(2, alpha + 15);
    printw(" [0] QUIT GAME");

    user USER = user_check();
    // right main menu
    move_c(right_x + 2, 2);
    printw(" [WELCOME TO TETRIS REMIX]");
    move_c(right_x + 2, 4);
    printw(" [ %10s ]", USER.name);
    move_c(right_x + 2, 5);
    printw(" [CMPN] LEVEL: %02d", USER.campaign_lvl);
    move_c(right_x + 2, 6);
    printw(" [ERMX] HIGH SCORE: %06d", USER.endless_r);
    move_c(right_x + 2, 7);
    printw(" [ECLS] HIGH SCORE: %06d", USER.endless_c);
    move_c(right_x + 2, 11);
    printw(" [THIS GAME IS THE]");
    move_c(right_x + 2, 12);
    printw(" [FINAL PROJECT FOR CS103]");
    move_c(right_x + 2, 13);
    printw(" [ BY: ]");
    move_c(right_x + 2, 15);
    printw(" [SUHI-AN, EURY MARC]");
    move_c(right_x + 2, 16);
    printw(" [PADERNA, MICHAEL JEROME]");
    move_c(right_x + 2, 17);
    printw(" [SALTING, MARVIN]");
    move_c(right_x + 2, 18);
    printw(" [ATIZADO, SEAN]");
    move_c(right_x + 2, 19);
    printw(" [MUNOZ, JOSE]");

    refresh();
}

// ncurses colors
void color()
{
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
}

// FOR USER DATA
// take user data from file and returns user
user user_check()
{
    user temp_user = {1, "", 0, 0, 0};
    FILE *file = fopen("tetris_data.txt", "r");
    if (file == NULL)
    {
        file = fopen("tetris_data.txt", "w"); // create empty
        if (file)
            fclose(file);
        return temp_user;
    }

    fscanf(file, "%d ", &temp_user.newplayer);
    fgets(temp_user.name, sizeof(temp_user.name), file);
    temp_user.name[strcspn(temp_user.name, "\n")] = 0; // remove newline
    fscanf(file, "%d %d %d", &temp_user.campaign_lvl, &temp_user.endless_r, &temp_user.endless_c);

    fclose(file);
    return temp_user;
}

void update_file(user player)
{
    FILE *file = fopen("tetris_data.txt", "w");
    if (file == NULL)
    {
        printw("Error opening file!\n");
        return;
    }
    fprintf(file, "%d %s\n%d %d %d\n",
            player.newplayer,
            player.name,
            player.campaign_lvl,
            player.endless_r,
            player.endless_c);
    fclose(file);
}

// how to play
void intro2tetris()
{
    nodelay(stdscr, FALSE);
    int intro;
    printw("To play the game:\nTo move the Tetromino:\na - to move left\nd - to move right\ns - to move down\nj - to down quickly\nr/w - to rotate\nspace - to quickly drop the block");
    printw("\nPress any key to continue\n");
    getch();
    clear();

    printw("If a row is filled with blocks, that row will be cleared and gain 100 score!\n");
    printw("Clearing multiple lines at the same time multiplies your score!\n");
    printw("\nPress any key to continue\n");
    getch();
    clear();

    printw("There are two types of game: Campaign and Endless\n");
    printw("In campaign you go through levels and try to reach a certain score in a time limit. There are groups of levels where there is a chosen gimmick so be prepared!\n");
    printw("Note: Gimmick are events that can alter the gameplay, each can help or harden the game!\n");
    printw("\nPress any key to continue\n");
    getch();
    clear();

    printw("In endless however, you go with no time limit, just skill and a little bit of luck to gain as many score as you can!\n");
    printw("Though based on your campaign level, the gimmick might also follow you through the endless! \n");
    printw("Note: Gimmick are events that can alter the gameplay, each can help or harden the game!\n");
    printw("\nPress any key to continue\n");
    getch();
    clear();
}

// initilize new player's info and a little information
user new_player()
{
    nodelay(stdscr, FALSE);

    // initialize data
    user temp;
    temp.newplayer = 1;
    temp.campaign_lvl = 1;
    temp.endless_r = 0;
    temp.endless_c = 0;

    // greetings
    printw("Welcome to Tetris Remix!");
    printw("\nPress any key to continue\n");
    refresh();
    getch();

    // take username
    do
    {
        clear();
        printw("A new player! Please input your user name here! (Max of 30 characters)\n ");
        refresh();
        echo();

        // safer: limit input to 30 chars (user struct has char name[31])
        getnstr(temp.name, 30);

        // remove newline if present
        temp.name[strcspn(temp.name, "\n")] = 0;

        noecho();
    } while (!confirmation());

    clear();
    noecho();

    intro2tetris(); // how to play
    printw("\n Hi, %s!\nAre you ready to start Tetris Remix?\n", temp.name);
    printw("\nPress any key to continue\n");
    getch();
    clear();

    update_file(temp); // save user data
    return temp;
}

// Update User data in settings
int user_settings(user *player)
{
    while (1)
    {
        nodelay(stdscr, FALSE);
        clear();
        printw("USER settings:\n1 - Change User Name\n2 - Reset Data\n\n0 - Back\n");
        refresh();
        int x = getch();
        switch (x)
        {
        // To change user name
        case '1':
            clear();
            do
            {
                clear();
                printw("Please input your user name here! (Max of 50 characters)\n ");
                refresh();
                echo();
                getnstr(player->name, sizeof(player->name) - 1);
                noecho();
            } while (!confirmation());

            clear();
            update_file(*player); // save the new name immediately
            printw("Name successfully changed to %s!\n", player->name);
            refresh();
            napms(2000);
            break;

        // To clear all data
        case '2':
            clear();
            if (confirmation())
            {
                player->newplayer = 0;
                update_file(*player);
                printw("Bye %s, User data resets successfully!\n Please Restart Game!", player->name);
                napms(5000);
                return 1;
            }
            break;
        // To return
        case '0':
            return 0;
        default:
            break;
        }
    }
}

// CORE TETRIS LOGIC
// Clear the line updates the grid
void update_grid(int grid[][MaxX], int line)
{
    // move the top rows down 1, this also clears filled row
    for (int y = line; y > 0; y--)
    {
        for (int x = 0; x < MaxX; x++)
        {
            grid[y][x] = grid[y - 1][x];
        }
    }

    // Clear/initialize the new first row
    for (int i = 0; i < MaxX; i++)
    {
        grid[0][i] = 0;
    }
}

// Check each line bottoms up and use update_grid if line is filled then check again, returns number of lines cleared
int line_check(int grid[][MaxX], int *score)
{
    int line = 0;
    // Check each row from bottom to top
    for (int j = MaxY - 1; j >= 0; j--)
    {
        int flag = 1;
        // Check each element if the row is filled
        for (int i = 0; i < MaxX; i++)
        {
            // If the row has gaps, stop checking
            if (grid[j][i] == 0)
            {
                flag = 0;
                break;
            }
        }

        // if there is no 0's/gaps, then the row is filled
        if (flag)
        {
            update_grid(grid, j);
            line++;
            j++; // check again at the same line
        }
    }
    // return score
    *score += 100 * line * line;
    return line;
}

// helps check if a tetromino can move in a position
int move_check(int side, int grid[][MaxX], int xpos[], int ypos[])
{
    for (int i = 0; i < 4; i++)
    {
        // create a copy of the the block then move it's coordinates
        int new_x = xpos[i] + (side == -1 ? -1 : (side == 1 ? 1 : 0));
        int new_y = ypos[i] + (side == 0 ? 1 : 0);

        // Check if the copy is within boundaries
        if (new_x < 0 || new_x >= MaxX || new_y >= MaxY)
            return 0;

        // Check if the new position is occupied
        if (grid[new_y][new_x] == 1)
            return 0;
    }
    return 1;
}

// Print grid with the main matrix, the block the player is controlling, and formatting from GIMMICKS
void print_grid(int grid[][MaxX], int xpos[], int ypos[], int EMOvar, int isEMO, int isKDA, int isCountry)
{
    char pxl[] = "[]";
    char kda[] = "[]";
    char emo[] = "??";
    char none[] = " .";
    char down[] = "[]";

    // To check if the gimmick will be activated
    EMOvar = MaxY;
    if (isEMO)
        for (int i = 0; i < MaxY; i++)
        {
            int count = 0;
            for (int j = 0; j < MaxX; j++)
                if (grid[i][j] == 1)
                    count++;
            if (count)
            {
                EMOvar = ((i + 4 < MaxY) ? i + 4 : MaxY); // Update EMOvar
                break;                                    // No need to check further in this row
            }
        }

    // For the tetromino shadow
    // create a copy of tetromino coordinates then use movecheck to predict where it would fall
    int xcpy[4];
    int ycpy[4];
    for (int i = 0; i < 4; i++)
    {
        xcpy[i] = xpos[i];
        ycpy[i] = ypos[i];
    }
    while (move_check(0, grid, xcpy, ycpy))
        for (int i = 0; i < 4; i++)
            ycpy[i]++;

    // check if the grid has 1 or the coordinates of the player's tetromino, then print a block on that coordinates
    for (int i = 0; i < MaxY; i++)
    {
        for (int j = 0; j < MaxX; j++)
        {
            int isBlock = grid[i][j];
            int isdown = grid[i][j];
            for (int k = 0; k < 4; k++)
            {
                if (i == ypos[k] && j == xpos[k])
                    isBlock = 1; // for non-solid tetromino
                if (i == ycpy[k] && j == xcpy[k])
                    isdown = 1; // it's shadow
            }
            const char *symbol;
            int color_pair;

            // to choose what color would that specific coordinate takes
            move_c(mid_x + (j * 2), i + 1);
            if (i > EMOvar)
            {
                symbol = emo;
                color_pair = 4;
            }
            else if (isBlock)
            {
                symbol = isKDA ? kda : pxl;
                color_pair = isKDA ? 5 : 7;
            }
            else if (isdown)
            {
                symbol = down;
                color_pair = 1;
            }
            else
            {
                symbol = none;
                color_pair = 7;
            }

            // print the specific coordinate
            attron(COLOR_PAIR(color_pair));
            printw("%s", symbol);
            attroff(COLOR_PAIR(color_pair));
        }
    }

    // if country gimmick is active, print alternating rows of slashes
    if (isCountry)
    {
        for (int i = 0; i < MaxY; i++)
        {
            if (i % 2 == 0)
            {
                move_c(mid_x, i + 1);
                attron(COLOR_PAIR(3));
                printw("%s", (i % 4 == 0) ? "////////////////////" : "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");
                attroff(COLOR_PAIR(3));
            }
        }
    }

    refresh();
}

// put correct values of xpos and ypos for each tetromino to start
void tetromino(int type, int xpos[], int ypos[])
{
    switch (type)
    {
    case 0:
        ypos[0] = 1;
        ypos[1] = 1;
        ypos[2] = 0;
        ypos[3] = 0;
        xpos[0] = 4;
        xpos[1] = 5;
        xpos[2] = 5;
        xpos[3] = 6;
        break; // S
    case 1:
        ypos[0] = 1;
        ypos[1] = 1;
        ypos[2] = 0;
        ypos[3] = 0;
        xpos[0] = 4;
        xpos[1] = 5;
        xpos[2] = 4;
        xpos[3] = 3;
        break; // Z
    case 2:
        ypos[0] = 1;
        ypos[1] = 0;
        ypos[2] = 0;
        ypos[3] = 2;
        xpos[0] = 4;
        xpos[1] = 3;
        xpos[2] = 4;
        xpos[3] = 4;
        break; // L
    case 3:
        ypos[0] = 1;
        ypos[1] = 0;
        ypos[2] = 0;
        ypos[3] = 2;
        xpos[0] = 4;
        xpos[1] = 5;
        xpos[2] = 4;
        xpos[3] = 4;
        break; // J
    case 4:
        ypos[0] = 1;
        ypos[1] = 1;
        ypos[2] = 1;
        ypos[3] = 1;
        xpos[0] = 4;
        xpos[1] = 3;
        xpos[2] = 5;
        xpos[3] = 6;
        break; // I
    case 5:
        ypos[0] = 1;
        ypos[1] = 1;
        ypos[2] = 0;
        ypos[3] = 1;
        xpos[0] = 4;
        xpos[1] = 3;
        xpos[2] = 4;
        xpos[3] = 5;
        break; // T
    case 6:
        ypos[0] = 1;
        ypos[1] = 1;
        ypos[2] = 0;
        ypos[3] = 0;
        xpos[0] = 4;
        xpos[1] = 5;
        xpos[2] = 4;
        xpos[3] = 5;
        break; // O
    }
}

// Rotate the tetromino only if the new position is valid
void rotate_tetro(int type, int *rotation, int xpos[], int ypos[], int grid[][MaxX])
{
    // Temporary arrays to store rotated positions
    int new_xpos[4], new_ypos[4];

    // Copy the current positions
    for (int i = 0; i < 4; i++)
    {
        new_xpos[i] = xpos[i];
        new_ypos[i] = ypos[i];
    }

    // Use copy of rotation to check if rotation is valid
    int new_rotation = (*rotation < 3) ? (*rotation + 1) : 0;

    switch (type)
    {
    case 0: // S
        if (new_rotation % 2 == 1)
        {
            new_xpos[3] -= 2;
            new_ypos[2] += 2;
        }
        else
        {
            new_xpos[3] += 2;
            new_ypos[2] -= 2;
        }
        break;
    case 1: // Z
        if (new_rotation % 2 == 1)
        {
            new_xpos[3] += 2;
            new_ypos[2] += 2;
        }
        else
        {
            new_xpos[3] -= 2;
            new_ypos[2] -= 2;
        }
        break;
    case 2: // L
        switch (new_rotation)
        {
        case 1:
            new_xpos[1] += 2;
            new_xpos[2] += 1;
            new_xpos[3] -= 1;
            new_ypos[2] += 1;
            new_ypos[3] -= 1;
            break;
        case 2:
            new_xpos[2] -= 1;
            new_xpos[3] += 1;
            new_ypos[1] += 2;
            new_ypos[2] += 1;
            new_ypos[3] -= 1;
            break;
        case 3:
            new_xpos[1] -= 2;
            new_xpos[2] -= 1;
            new_xpos[3] += 1;
            new_ypos[2] -= 1;
            new_ypos[3] += 1;
            break;
        case 0:
            new_xpos[2] += 1;
            new_xpos[3] -= 1;
            new_ypos[1] -= 2;
            new_ypos[2] -= 1;
            new_ypos[3] += 1;
            break;
        }
        break;
    case 3: // J
        switch (new_rotation)
        {
        case 1:
            new_xpos[2] += 1;
            new_xpos[3] -= 1;
            new_ypos[1] += 2;
            new_ypos[2] += 1;
            new_ypos[3] -= 1;
            break;
        case 2:
            new_xpos[1] -= 2;
            new_xpos[2] -= 1;
            new_xpos[3] += 1;
            new_ypos[2] += 1;
            new_ypos[3] -= 1;
            break;
        case 3:
            new_xpos[2] -= 1;
            new_xpos[3] += 1;
            new_ypos[1] -= 2;
            new_ypos[2] -= 1;
            new_ypos[3] += 1;
            break;
        case 0:
            new_xpos[1] += 2;
            new_xpos[2] += 1;
            new_xpos[3] -= 1;
            new_ypos[2] -= 1;
            new_ypos[3] += 1;
            break;
        }
        break;
    case 4: // I
        if (new_rotation % 2 == 0)
        {
            new_ypos[1] += 1;
            new_ypos[2] -= 1;
            new_ypos[3] -= 2;
            new_xpos[1] -= 1;
            new_xpos[2] += 1;
            new_xpos[3] += 2;
        }
        else
        {
            new_ypos[1] -= 1;
            new_ypos[2] += 1;
            new_ypos[3] += 2;
            new_xpos[1] += 1;
            new_xpos[2] -= 1;
            new_xpos[3] -= 2;
        }
        break;
    case 5: // T
        switch (new_rotation)
        {
        case 1:
            new_xpos[1] += 1;
            new_ypos[1] += 1;
            break;
        case 2:
            new_xpos[1] -= 1;
            new_ypos[1] -= 1;
            new_ypos[2] += 2;
            break;
        case 3:
            new_ypos[2] -= 2;
            new_xpos[3] -= 1;
            new_ypos[3] += 1;
            break;
        case 0:
            new_ypos[3] -= 1;
            new_xpos[3] += 1;
            break;
        }
        break;
    case 6: // O doesnt rotate
        return;
    }

    // **Check if the new rotated positions are valid**
    for (int i = 0; i < 4; i++)
    {
        if (new_xpos[i] < 0 || new_xpos[i] >= MaxX || new_ypos[i] < 0 || new_ypos[i] >= MaxY)
        {
            return; // Cancel rotation if out of bounds
        }
        if (grid[new_ypos[i]][new_xpos[i]] != 0)
        {
            return; // Cancel rotation if another block is in the way
        }
    }

    // Apply the rotation if all position is valid
    *rotation = new_rotation;
    for (int i = 0; i < 4; i++)
    {
        xpos[i] = new_xpos[i];
        ypos[i] = new_ypos[i];
    }
}

// GIMMICKS THAT NEEDED TO BE IMPLEMENTED AS FUNCTIONS
// Hyper POP: randomly pop solid blocks
void HyperPop(int grid[][MaxX])
{
    int tilesToPop = 2;
    int availableTiles[MaxX * MaxY][2];
    int availableCount = 0;

    // Find and count all tiles that can be popped
    for (int y = 0; y < MaxY; y++)
    {
        for (int x = 0; x < MaxX; x++)
        {
            if (grid[y][x] == 1)
            {
                availableTiles[availableCount][0] = x;
                availableTiles[availableCount][1] = y;
                availableCount++;
            }
        }
    }

    // Handle base case where fewer than 3 tiles exist
    if (availableCount == 0)
        return;

    tilesToPop = (availableCount < tilesToPop) ? availableCount : tilesToPop;

    // Randomly pop tiles
    for (int i = 0; i < tilesToPop; i++)
    {
        int randIndex = rand() % availableCount;
        int xpos = availableTiles[randIndex][0];
        int ypos = availableTiles[randIndex][1];

        grid[ypos][xpos] = 0;

        // Remove the selected tile by swapping with the last and reducing count
        availableTiles[randIndex][0] = availableTiles[availableCount - 1][0];
        availableTiles[randIndex][1] = availableTiles[availableCount - 1][1];
        availableCount--;
    }
}

// jazz: summon solids and fill gaps
void Jazz(int grid[][MaxX])
{
    // Find the highest row with at least one block
    int height = MaxY; // Start from the bottom
    for (int i = 0; i < MaxY; i++)
    {
        for (int j = 0; j < MaxX; j++)
        {
            if (grid[i][j] == 1)
            {
                height = i;
                goto found; // break both loops using goto
            }
        }
    }
found:

    int tofill = 3;

    // Try to fill gaps from bottom to top starting from the found height
    for (int i = MaxY - 1; i >= height && tofill > 0; i--)
    {
        for (int j = MaxX - 1; j >= 0 && tofill > 0; j--)
        {
            if (grid[i][j] == 0)
            {
                grid[i][j] = 1;
                tofill--;
            }
        }
    }

    // If not enough gaps filled, go one row higher
    if (tofill > 0 && height > 0)
    {
        height--;
        for (int i = MaxY - 1; i >= height && tofill > 0; i--)
        {
            for (int j = MaxX - 1; j >= 0 && tofill > 0; j--)
            {
                if (grid[i][j] == 0)
                {
                    grid[i][j] = 1;
                    tofill--;
                }
            }
        }
    }
}

// illbeats: raise the solids one row higher then the bottom copies the previous bottom row
void Illbeats(int grid[][MaxX])
{
    for (int y = 0; y < MaxY - 1; y++)
    {
        for (int x = 0; x < MaxX; x++)
        {
            grid[y][x] = grid[y + 1][x];
        }
    }

    // Copy the second-to-last row to the last row
    for (int x = 0; x < MaxX; x++)
    {
        grid[MaxY - 1][x] = grid[MaxY - 2][x];
    }
}

// disco: play animation then move all solids to left and right
void Disco(int grid[][MaxX], int direction, int xpos[], int ypos[])
{
    // direction = -1 for left, 1 for right
    int start = (direction < 0) ? 0 : MaxX - 1;
    int end = (direction < 0) ? MaxX : -1;
    int step = (direction < 0) ? 1 : -1;

    // First wave: overlay disco animation
    for (int x = start; x != end; x += step)
    {
        for (int y = 0; y < MaxY; y++)
        {
            move_c(mid_x + (x * 2), y + 1);
            attron(COLOR_PAIR(6));
            printw("%s", (direction < 0) ? ">>" : "<<");
            attroff(COLOR_PAIR(6));
        }
        refresh();
        napms(100);
    }

    // Push blocks to left
    for (int i = 0; i < MaxY; i++)
    {
        int count = 0;
        // Count the number of blocks in the row
        for (int j = 0; j < MaxX; j++)
        {
            if (grid[i][j] == 1)
                count++;
        }

        // Clear row
        for (int j = 0; j < MaxX; j++)
            grid[i][j] = 0;

        // Place blocks to the left or right
        if (direction > 0)
        { // Push left
            for (int j = 0; j < count; j++)
                grid[i][j] = 1;
        }
        else if (direction < 0)
        { // Push right
            for (int j = MaxX - count; j < MaxX; j++)
                grid[i][j] = 1;
        }
    }

    // Reverse wave: reveal actual grid
    for (int x = start; x != end; x += step)
    {
        for (int y = 0; y < MaxY; y++)
        {
            move_c(mid_x + (x * 2), y + 1);
            if (grid[y][x] == 1)
            {
                attron(COLOR_PAIR(7));
                printw("[]");
                attroff(COLOR_PAIR(7));
            }
            else
            {
                printw(" .");
            }
        }
        refresh();
        napms(50);
    }
}

// heartsteel: punch heart holes in solids randomly, can be small or big
void createHeart(int grid[MaxY][MaxX])
{
    // Choose a random column to start the heart
    int startX = rand() % (MaxX - 5) + 2; // Ensures heart won't go beyond grid's right side

    // Small Heart at the bottom
    int startY = rand() % (MaxY - 4) + 2; // Start anywhere

    // Heart shape
    grid[startY][startX] = 0;
    grid[startY + 1][startX] = 0;
    grid[startY + 2][startX] = 0;
    grid[startY][startX - 2] = 0;
    grid[startY - 1][startX - 1] = 0;
    grid[startY][startX - 1] = 0;
    grid[startY + 1][startX - 1] = 0;
    grid[startY - 1][startX + 1] = 0;
    grid[startY][startX + 1] = 0;
    grid[startY + 1][startX + 1] = 0;
    grid[startY][startX + 2] = 0;

    attron(COLOR_PAIR(1));
    move_c(mid_x + (startX * 2), 1 + startY);
    printw("\\/");
    move_c(mid_x + ((startX) * 2), 1 + (startY + 1));
    printw("  ");
    move_c(mid_x + ((startX) * 2), 1 + (startY + 2));
    printw("\\/");
    move_c(mid_x + ((startX - 2) * 2), 1 + (startY));
    printw("< ");
    move_c(mid_x + ((startX - 1) * 2), 1 + (startY - 1));
    printw("/\\");
    move_c(mid_x + ((startX - 1) * 2), 1 + (startY));
    printw("  ");
    move_c(mid_x + ((startX - 1) * 2), 1 + (startY + 1));
    printw("\\ ");
    move_c(mid_x + ((startX + 1) * 2), 1 + (startY - 1));
    printw("/\\");
    move_c(mid_x + ((startX + 1) * 2), 1 + (startY));
    printw("  ");
    move_c(mid_x + ((startX + 1) * 2), 1 + (startY + 1));
    printw(" /");
    move_c(mid_x + ((startX + 2) * 2), 1 + (startY));
    printw(" >");
    attroff(COLOR_PAIR(1));
    refresh();
    napms(1000);
}

// mixmaster: can be kinetic, ethereal, or concussive
void Mixmaster(int grid[][MaxX], int cycle, int *score)
{
    // kinetic randomly ups or downs each columns
    if (cycle == 1)
    {
        for (int x = 0; x < MaxX; x++)
        {
            int dir = (rand() % 2 == 0) ? 1 : -1;
            if (dir > 0)
            {
                for (int y = MaxY - 1; y > 0; y--)
                    grid[y][x] = grid[y - 1][x];
                grid[0][x] = 0;
            }
            else
            {
                for (int y = 0; y < MaxY - 1; y++)
                    grid[y][x] = grid[y + 1][x];
                grid[MaxY - 1][x] = 0;
            }
        }
    }
    // ethereal: give bonus 50 points per lines with atleast one block
    else if (cycle == 2)
    {
        for (int i = 0; i < MaxY; i++)
        {
            int count = 0;
            for (int j = 0; j < MaxX; j++)
            {
                if (grid[i][j] == 1)
                {
                    count += 1;
                    break;
                }
            }
            if (count)
                *score += 50;
        }
    }
    // concussive: destroys all solids
    else if (cycle == 3)
    {
        for (int i = 0; i < MaxY; i++)
            for (int j = 0; j < MaxX; j++)
                grid[i][j] = 0;
    }
}
