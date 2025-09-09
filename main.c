/*
CS103 FINAL PROJECT
GROUP 6
    Atizado, Sean Antonio+
    Munoz, Jose Amiele
    Paderna, Michael Jerome
    Salting, Marvin Andrei
    Suhi-an, Eury Marc
*/

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

// MAIN GAME MODES
// game mode where player need to reach goal score and avoid game over to advance
void campaign(user *player)
{
    while (player->campaign_lvl <= 30)
    {
        // Load the first music
        int musicvar = player->campaign_lvl - 1;
        Mix_Music *music = Mix_LoadMUS(track[musicvar]);

        notify();
        move_c(mid_x + 2, 10);
        printw("CAMPAIGN  LVL %02d", player->campaign_lvl);
        move_c(mid_x + 2, 11);
        printw("[1]PLAY  [0]BACK");
        refresh();

        nodelay(stdscr, FALSE);
        int confirm = '2';
        do
        {
            confirm = getch();
        } while (confirm != '1' && confirm != '0');
        if (confirm == '0')
        {
            Mix_FreeMusic(music);
            return;
        }
        notify();
        move_c(mid_x + 2, 10);
        printw(" PRESS ANY KEY  ");
        move_c(mid_x + 2, 11);
        printw("   TO START!    ");
        refresh();
        int x = getch();
        nodelay(stdscr, TRUE);
        Mix_PlayMusic(music, 0);

        // clear the terminal
        clear();
        draw_UI();

        // Declaration of flags and stats
        int flag = 0;
        int user = 0;
        int solidify = 0;
        char intro;
        int gameover = 0;
        int l_clear = 0;

        // Declaration
        int xpos[4] = {0};
        int ypos[4] = {0};
        int current = 0;
        int next = rand() % 7;
        int rotate = 0;
        int score = 0;
        int paused = 0;
        int level = player->campaign_lvl;
        int goal = 2500 + (level - 1) * 250 + (level % 10 == 0) * 1000 * (level / 10);

        // gimmicks
        int isKDA = 0, Hyperpop = 0, Pianist = 0, isEMO = 0, EMOvar = 0;
        int TRUED = 0, isJazz = 0, isIll = 0, isDisco = 0, isPunk = 0, Punkhead = 0;
        int isEDM = 0, isPenta = 0, isCountry = 0, isHeart = 0;
        int isMix = 0, kinetic = 0, ethereal = 0, concussive = 0;
        int isMaestro = 0;

        time_t startTime = time(NULL); // Capture the start time
        time_t currentTime;
        int elapsedSeconds;
        int pause_duration = 0; // Duration of the pause in seconds

        // grid
        int grid[MaxY][MaxX] = {0};
        draw_left_menu();
        draw_right_menu(0, 0, 0, score, 0, player->campaign_lvl, 0);

        // Game body
        while (Mix_PlayingMusic() && !gameover)
        {
            // timers
            currentTime = time(NULL);
            elapsedSeconds = (int)(currentTime - startTime - pause_duration); // Adjust elapsed time for pause duration

            // draw_right_menu
            draw_right_menu(elapsedSeconds, current, next, score, 0, player->campaign_lvl, 0);

            // print tetris grid
            if (user == 0)
            {
                user = 1;
                current = next;
                next = rand() % 7;
                tetromino(current, xpos, ypos);
            }
            print_grid(grid, xpos, ypos, EMOvar, isEMO, isKDA, isCountry);

            // Check if game is already over
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    if (grid[j][i + 4] == 1)
                    {
                        gameover = 1;
                        break;
                    }
            if (gameover)
            {
                Mix_PauseMusic();
                game_over();
                napms(5000);
                break;
            }

            // gimmicks
            // every 10 seconds there is 5% chance hyperpop pop tiles
            if ((rand() % 100) <= 5 && !Hyperpop && (level == 2 || level == 3 || level == 7 || level == 9 || level == 17 || level == 21 || level == 29 || level == 30))
            {
                HyperPop(grid);
                Hyperpop = elapsedSeconds;
            }
            if (Hyperpop && elapsedSeconds - Hyperpop > 10)
                Hyperpop = 0;

            // every 15 seconds there is 15% chance for KDA to summon a spotlight speeding up the board
            if (rand() % 100 <= 15 && !isKDA && (level == 4 || level == 5 || level == 7 || level == 9 || level == 19 || level == 27 || level == 30))
                isKDA = elapsedSeconds;
            if (isKDA && elapsedSeconds - isKDA > 15)
                isKDA = 0;

            // pianist calms the board
            if ((level == 6 || level == 11) && !Pianist)
                Pianist = 1;

            // emo floods the board if a specific height is reached
            if (level == 8 || level == 9 || level == 11 || level == 13 || level == 21 || level == 30)
                isEMO = 1;

            // true damage appreciates a mic drop, granting bonuses to clears 3-4 in a drop
            if ((level == 10 || level == 11 || level == 27 || level == 30) && !TRUED)
            {
                if (l_clear >= 3)
                {
                    TRUED = elapsedSeconds;
                    score += l_clear == 3 ? 100 : 200;
                }
            }
            if (TRUED && elapsedSeconds - TRUED > 5)
                TRUED = 0;

            // jazz randomly fill gaps or summon blocks every 15 seconds
            if (elapsedSeconds % 15 == 0 && (level == 12 || level == 13 || level == 15 || level == 17 || level == 23 || level == 30) && !isJazz)
            {
                Jazz(grid);
                isJazz = elapsedSeconds;
            }
            if (isJazz && elapsedSeconds - isJazz > 15)
                isJazz = 0;

            // illbeats, every 50 seconds raises the blocks
            if (elapsedSeconds % 50 == 0 && (level == 14 || level == 15 || level == 30) && !isIll)
            {
                Illbeats(grid);
                isIll = elapsedSeconds;
            }
            if (isIll && elapsedSeconds - isIll > 15)
                isIll = 0;

            // disco shifts the blocks to the left or right every 15 seconds
            if (elapsedSeconds % 15 && (level == 16 || level == 17 || level == 30) && !isDisco)
            {
                Disco(grid, (rand() % 2 == 0) ? -1 : 1, xpos, ypos);
                isDisco = elapsedSeconds;
            }
            if (isDisco && elapsedSeconds - isDisco > 15)
                isDisco = 0;

            // punk breaks the rules of the game, allowing the player to gain bonus points if dropping 4 blocks with no clear
            if ((level == 18 || level == 19 || level == 23 || level == 30) && isPunk >= 4 && l_clear == 0)
            {
                score += 150;
                isPunk = 0;
                Punkhead = elapsedSeconds;
            }
            if (Punkhead && elapsedSeconds - Punkhead > 5)
                Punkhead = 0;

            // EDM shoots a laser in random column every 25 seconds
            if (elapsedSeconds % 25 == 0 && (level == 20 || level == 21 || level == 29 || level == 30) && !isEDM)
            {
                int laser_x = rand() % MaxX;
                for (int i = 0; i < MaxY; i++)
                {
                    move_c(mid_x + (laser_x * 2), i + 1);
                    attron(COLOR_PAIR(4));
                    printw("||");
                    attroff(COLOR_PAIR(4));
                    grid[i][laser_x] = 0;
                }
                refresh();
                napms(1000);

                isEDM = elapsedSeconds;
            }
            if (isEDM && elapsedSeconds - isEDM > 25)
                isEDM = 0;

            // country has 2% chance to summon grime and dust to hide alternating rows for 10 seconds
            if ((level == 24 || level == 25 || level == 30) && !isCountry && rand() % 100 <= 1)
            {
                isCountry = elapsedSeconds;
            }
            if (isCountry && elapsedSeconds - isCountry > (rand() % 10 + 1))
                isCountry = 0;

            // heartsteel robs and breaks heart, 5% chance to carve a heart in the grid every 25 seconds
            if ((level == 26 || level == 27 || level == 30) && !isHeart && rand() % 100 <= 5)
            {
                createHeart(grid);
                isHeart = elapsedSeconds;
            }
            if (isHeart && elapsedSeconds - isHeart > 20)
                isHeart = 0;

            // mixmaster remixes the board, at 100 sec randomly shift up and down columns, at 200 gain bonus score to the number of lines with blocks, at 300 clear the grid
            if ((level == 28 || level == 29 || level == 30) && !isMix && !kinetic && elapsedSeconds >= 100)
            {
                Mixmaster(grid, 1, &score);
                isMix = elapsedSeconds;
                kinetic = 1;
            }
            if ((level == 28 || level == 29 || level == 30) && !isMix && !ethereal && elapsedSeconds >= 200)
            {
                Mixmaster(grid, 2, &score);
                isMix = elapsedSeconds;
                ethereal = 1;
            }
            if ((level == 28 || level == 29 || level == 30) && !isMix && !concussive && elapsedSeconds >= 300)
            {
                Mixmaster(grid, 3, &score);
                isMix = elapsedSeconds;
                concussive = 1;
            }
            if (isMix && elapsedSeconds - isMix > 10)
                isMix = 0;

            // maestro prepares for a duel increasing the difficulty of the game.
            if (level == 30)
                isMaestro = 1;

            // user input
            int timer = 5;

            while (timer > 0)
            {
                int quit = 0;
                int input = getch();
                switch (input)
                {
                case 'a':
                case 'A':
                    if (move_check(-1, grid, xpos, ypos))
                        for (int i = 0; i < 4; i++)
                            xpos[i]--;
                    break;
                case 'd':
                case 'D':
                    if (move_check(1, grid, xpos, ypos))
                        for (int i = 0; i < 4; i++)
                            xpos[i]++;
                    break;
                case 's':
                case 'S':
                    if (move_check(0, grid, xpos, ypos))
                        for (int i = 0; i < 4; i++)
                            ypos[i]++;
                    break;
                case 'W':
                case 'w':
                case 'r':
                case 'R':
                    rotate_tetro(current, &rotate, xpos, ypos, grid);
                    break;
                case ' ':
                    while (move_check(0, grid, xpos, ypos))
                        for (int i = 0; i < 4; i++)
                            ypos[i]++;
                    solidify += 2;
                    quit = 1;
                    break;
                case '0':
                    Mix_PauseMusic();
                    if (confirmation())
                    {
                        Mix_FreeMusic(music);
                        return;
                    }
                    Mix_ResumeMusic();
                    break;
                case 'P':
                case 'p':
                    if (!paused)
                    {
                        pause_game();
                        paused = 1;
                        Mix_PauseMusic();
                        time_t pause_start = time(NULL);
                        while (paused)
                        {
                            int temp = getch();
                            if (temp == '0')
                            {
                                Mix_FreeMusic(music);
                                return;
                            }
                            if (temp == 'P' || temp == 'p')
                            {
                                Mix_ResumeMusic();
                                time_t c = time(NULL);
                                pause_duration += c - pause_start;
                                paused = 0;
                            }
                        }
                    }
                    break;
                default:
                }
                if (quit)
                    break;
                draw_right_menu(elapsedSeconds, current, next, score, 0, player->campaign_lvl, 0);
                print_grid(grid, xpos, ypos, EMOvar, isEMO, isKDA, isCountry);
                timer--;
                int difficulty = ((level - 1) * 5) + (elapsedSeconds / 30);
                int baseSpeed = 200 - difficulty - (isKDA ? 20 : 0);
                int speed = Pianist ? 200 : (baseSpeed <= 0 ? 1 : baseSpeed);
                napms(speed);
            }

            // Natural movement and Check if a line is created
            if (user == 1 && move_check(0, grid, xpos, ypos))
                for (int i = 0; i < 4; i++)
                    ypos[i]++;

            if (!move_check(0, grid, xpos, ypos))
            {
                solidify += 1;
                if (solidify > 1)
                {
                    for (int i = 0; i < 4; i++)
                        grid[ypos[i]][xpos[i]] = 1;
                    user = 0;
                    solidify = 0;
                    rotate = 0;
                    isPunk++;
                }
            }
            // Check if a line is created and update score
            l_clear = line_check(grid, &score);
            if (l_clear)
                isPunk = 0;
            // pentakill clears line and grants bonus if you clear 4 lines in a row
            if (l_clear == 4 && (level == 22 || level == 23 || level == 25 || level == 30) && !isPenta)
            {
                // Check each row from bottom to top
                int bonus_row = -1;
                // Find the highest row with at least one block
                for (int j = 0; j < MaxY; j++)
                {
                    for (int i = 0; i < MaxX; i++)
                    {
                        if (grid[j][i] == 1)
                        {
                            bonus_row = j;
                            break;
                        }
                    }
                    if (bonus_row != -1)
                        break;
                }
                if (bonus_row == -1)
                    bonus_row = MaxY - 1;

                // clear the row
                for (int i = 0; i < MaxX; i++)
                    grid[bonus_row][i] = 0;

                // Show "PENTAKILL XXX"
                move_c(mid_x, bonus_row + 1); // Center text
                attron(COLOR_PAIR(1));
                printw(" XXX PENTAKILL XXX ");
                attroff(COLOR_PAIR(1));

                refresh();
                napms(500);
                score += 500;
                isPenta = elapsedSeconds;
            }
            if (isPenta && elapsedSeconds - isPenta > 5)
                isPenta = 0;
        }

        notify();
        move_c(mid_x + 2, 10);
        if (!gameover && score >= goal)
        {
            printw("   YOU PASSED!  ");
            move_c(mid_x + 2, 11);
            printw(" Level %02d -> %02d ", player->campaign_lvl, player->campaign_lvl + 1);
            player->campaign_lvl++;
        }
        if (gameover || score < goal)
        {
            printw("   YOU FAILED!  ");
            move_c(mid_x + 2, 11);
            printw("   TRY AGAIN!   ");
        }
        refresh();
        update_file(*player); // save user data
        napms(2500);
    }

    if (player->campaign_lvl > 30)
    {
        notify();
        move_c(mid_x + 2, 10);
        printw("   YOU WON!     ");
        move_c(mid_x + 2, 11);
        printw("CONGRATULATIONS!");
        refresh();
        napms(5000);
    }
}

// game mode where player plays an infinite game and try to get the highest score but with gimmicks
void endless_r(user *player)
{
    while (1)
    {
        // Load the first music
        int musicvar = rand() % 28 + 1;
        Mix_Music *music = Mix_LoadMUS(track[musicvar]);

        notify();
        move_c(mid_x + 2, 10);
        printw(" ENDLESS  REMIX ");
        move_c(mid_x + 2, 11);
        printw("[1]PLAY  [0]BACK");
        refresh();

        nodelay(stdscr, FALSE);
        int confirm = '2';
        do
        {
            confirm = getch();
        } while (confirm != '1' && confirm != '0');
        if (confirm == '0')
        {
            // Free the music and close SDL_mixer
            Mix_FreeMusic(music);
            return;
        }

        notify();
        move_c(mid_x + 2, 10);
        printw(" PRESS ANY KEY  ");
        move_c(mid_x + 2, 11);
        printw("   TO START!    ");
        refresh();
        int x = getch();
        nodelay(stdscr, TRUE);

        // clear the terminal
        clear();
        draw_UI();

        // Declaration of flags and stats
        int flag = 0;
        int user = 0;
        int solidify = 0;
        char intro;

        // Declaration
        int xpos[4] = {0};
        int ypos[4] = {0};
        int current = 0;
        int next = rand() % 7;
        int rotate = 0;
        int score = 0;
        int paused = 0;
        int l_clear = 0;

        Mix_PlayMusic(music, 0);
        int level = musicvar + 1;
        mech(level);

        // gimmicks
        int isKDA = 0, Hyperpop = 0, Pianist = 0, isEMO = 0, EMOvar = 0;
        int TRUED = 0, isJazz = 0, isIll = 0, isDisco = 0, isPunk = 0, Punkhead = 0;
        int isEDM = 0, isPenta = 0, isCountry = 0, isHeart = 0;
        int isMix = 0, kinetic = 0, ethereal = 0, concussive = 0;

        time_t startTime = time(NULL); // Capture the start time
        time_t currentTime;
        int elapsedSeconds;
        int pause_duration = 0; // Duration of the pause in seconds

        // grid
        int grid[MaxY][MaxX] = {0};
        clear();
        draw_UI();
        draw_left_menu();
        draw_right_menu(0, 0, 0, score, 1, level, player->endless_r);

        // Game body
        for (flag = 0; 1; flag++)
        {
            currentTime = time(NULL);
            elapsedSeconds = (int)(currentTime - startTime - pause_duration); // Adjust elapsed time for pause duration

            // draw_right_menu
            draw_right_menu(elapsedSeconds, current, next, score, 1, level, player->endless_r);

            // print tetris grid
            if (user == 0)
            {
                user = 1;
                current = next;
                next = rand() % 7;
                tetromino(current, xpos, ypos);
            }
            print_grid(grid, xpos, ypos, EMOvar, isEMO, isKDA, isCountry);

            // Check if game is already over
            int gameover = 0;
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    if (grid[j][i + 4] == 1)
                    {
                        gameover = 1;
                        break;
                    }
            if (gameover)
            {
                Mix_PauseMusic();
                game_over();
                napms(5000);
                break;
            }

            // gimmicks
            // every 10 seconds there is 5% chance hyperpop pop tiles
            if ((rand() % 100) <= 5 && !Hyperpop && (level == 2 || level == 3 || level == 7 || level == 9 || level == 17 || level == 21 || level == 29 || level == 30))
            {
                HyperPop(grid);
                Hyperpop = elapsedSeconds;
            }
            if (Hyperpop && elapsedSeconds - Hyperpop > 10)
                Hyperpop = 0;

            // every 15 seconds there is 15% chance for KDA to summon a spotlight speeding up the board
            if (rand() % 100 <= 15 && !isKDA && (level == 4 || level == 5 || level == 7 || level == 9 || level == 19 || level == 27 || level == 30))
                isKDA = elapsedSeconds;
            if (isKDA && elapsedSeconds - isKDA > 15)
                isKDA = 0;

            // pianist calms the board
            if ((level == 6 || level == 11) && !Pianist)
                Pianist = 1;

            // emo floods the board if a specific height is reached
            if (level == 8 || level == 9 || level == 11 || level == 13 || level == 21 || level == 30)
                isEMO = 1;

            // true damage appreciates a mic drop, granting bonuses to clears 3-4 in a drop
            if ((level == 10 || level == 11 || level == 27 || level == 30) && !TRUED)
            {
                if (l_clear >= 3)
                {
                    TRUED = elapsedSeconds;
                    score += l_clear == 3 ? 100 : 200;
                }
            }
            if (TRUED && elapsedSeconds - TRUED > 5)
                TRUED = 0;

            // jazz randomly fill gaps or summon blocks every 15 seconds
            if (elapsedSeconds % 15 == 0 && (level == 12 || level == 13 || level == 15 || level == 17 || level == 23 || level == 30) && !isJazz)
            {
                Jazz(grid);
                isJazz = elapsedSeconds;
            }
            if (isJazz && elapsedSeconds - isJazz > 15)
                isJazz = 0;

            // illbeats, every 50 seconds raises the blocks
            if (elapsedSeconds % 50 == 0 && (level == 14 || level == 15 || level == 30) && !isIll)
            {
                Illbeats(grid);
                isIll = elapsedSeconds;
            }
            if (isIll && elapsedSeconds - isIll > 15)
                isIll = 0;

            // disco shifts the blocks to the left or right every 15 seconds
            if (elapsedSeconds % 15 && (level == 16 || level == 17 || level == 30) && !isDisco)
            {
                Disco(grid, (rand() % 2 == 0) ? -1 : 1, xpos, ypos);
                isDisco = elapsedSeconds;
            }
            if (isDisco && elapsedSeconds - isDisco > 15)
                isDisco = 0;

            // punk breaks the rules of the game, allowing the player to gain bonus points if dropping 4 blocks with no clear
            if ((level == 18 || level == 19 || level == 23 || level == 30) && isPunk >= 4 && l_clear == 0)
            {
                score += 150;
                isPunk = 0;
                Punkhead = elapsedSeconds;
            }
            if (Punkhead && elapsedSeconds - Punkhead > 5)
                Punkhead = 0;

            // EDM shoots a laser in random column every 25 seconds
            if (elapsedSeconds % 25 == 0 && (level == 20 || level == 21 || level == 29 || level == 30) && !isEDM)
            {
                int laser_x = rand() % MaxX;
                for (int i = 0; i < MaxY; i++)
                {
                    move_c(mid_x + (laser_x * 2), i + 1);
                    attron(COLOR_PAIR(4));
                    printw("||");
                    attroff(COLOR_PAIR(4));
                    grid[i][laser_x] = 0;
                }
                refresh();
                napms(1000);

                isEDM = elapsedSeconds;
            }
            if (isEDM && elapsedSeconds - isEDM > 25)
                isEDM = 0;

            // country has 2% chance to summon grime and dust to hide alternating rows for 10 seconds
            if ((level == 24 || level == 25 || level == 30) && !isCountry && rand() % 100 <= 2)
            {
                isCountry = elapsedSeconds;
            }
            if (isCountry && elapsedSeconds - isCountry > (rand() % 10 + 1))
                isCountry = 0;

            // heartsteel robs and breaks heart, 5% chance to carve a heart in the grid every 25 seconds
            if ((level == 26 || level == 27 || level == 30) && !isHeart && rand() % 100 <= 5)
            {
                createHeart(grid);
                isHeart = elapsedSeconds;
            }
            if (isHeart && elapsedSeconds - isHeart > 25)
                isHeart = 0;

            // mixmaster remixes the board, at 100 sec randomly shift up and down columns, at 200 gain bonus score to the number of lines with blocks, at 300 clear the grid
            if ((level == 28 || level == 29 || level == 30) && !isMix && !kinetic && elapsedSeconds >= 100)
            {
                Mixmaster(grid, 1, &score);
                isMix = elapsedSeconds;
                kinetic = 1;
            }
            if ((level == 28 || level == 29 || level == 30) && !isMix && !ethereal && elapsedSeconds >= 200)
            {
                Mixmaster(grid, 2, &score);
                isMix = elapsedSeconds;
                ethereal = 1;
            }
            if ((level == 28 || level == 29 || level == 30) && !isMix && !concussive && elapsedSeconds >= 300)
            {
                Mixmaster(grid, 3, &score);
                isMix = elapsedSeconds;
                concussive = 1;
            }
            if (isMix && elapsedSeconds - isMix > 10)
                isMix = 0;

            // user input
            int timer = 5;

            while (timer > 0)
            {
                int quit = 0;
                int input = getch();
                switch (input)
                {
                case 'a':
                case 'A':
                    if (move_check(-1, grid, xpos, ypos))
                        for (int i = 0; i < 4; i++)
                            xpos[i]--;
                    break;
                case 'd':
                case 'D':
                    if (move_check(1, grid, xpos, ypos))
                        for (int i = 0; i < 4; i++)
                            xpos[i]++;
                    break;
                case 's':
                case 'S':
                    if (move_check(0, grid, xpos, ypos))
                        for (int i = 0; i < 4; i++)
                            ypos[i]++;
                    break;
                case 'W':
                case 'w':
                case 'r':
                case 'R':
                    rotate_tetro(current, &rotate, xpos, ypos, grid);
                    break;
                case ' ':
                    while (move_check(0, grid, xpos, ypos))
                        for (int i = 0; i < 4; i++)
                            ypos[i]++;
                    solidify += 2;
                    quit = 1;
                    break;
                case '0':
                    Mix_PauseMusic();
                    if (confirmation())
                    {
                        Mix_FreeMusic(music);
                        return;
                    }
                    Mix_ResumeMusic();
                    break;
                case 'P':
                case 'p':
                    if (!paused)
                    {
                        pause_game();
                        paused = 1;
                        Mix_PauseMusic();
                        time_t pause_start = time(NULL); // check the time when paused
                        while (paused)
                        {
                            int temp = getch();
                            if (temp == '0')
                            {
                                Mix_FreeMusic(music);
                                return;
                            }
                            if (temp == 'P' || temp == 'p')
                            {
                                Mix_ResumeMusic();
                                time_t c = time(NULL);
                                pause_duration += c - pause_start;
                                paused = 0;
                            }
                        }
                    }
                    break;
                default:
                }
                if (quit)
                    break;
                draw_right_menu(elapsedSeconds, current, next, score, 1, 0, player->endless_r);
                print_grid(grid, xpos, ypos, EMOvar, isEMO, isKDA, isCountry);
                timer--;
                int difficulty = (score / 250) + (elapsedSeconds / 60);
                int baseSpeed = 200 - difficulty - (isKDA ? 20 : 0);
                int speed = Pianist ? 200 : (baseSpeed <= 0 ? 1 : baseSpeed);
                napms(speed);
            }

            // Natural movement and Check if a line is created
            if (user == 1 && move_check(0, grid, xpos, ypos))
                for (int i = 0; i < 4; i++)
                    ypos[i]++;

            if (!move_check(0, grid, xpos, ypos))
            {
                solidify += 1;
                if (solidify > 1)
                {
                    for (int i = 0; i < 4; i++)
                        grid[ypos[i]][xpos[i]] = 1;
                    user = 0;
                    solidify = 0;
                    rotate = 0;
                    isPunk++;
                }
            }

            // Check if a line is created and update score
            line_check(grid, &score);
            if (l_clear)
                isPunk = 0;
            // pentakill clears line and grants bonus if you clear 4 lines in a row
            if (l_clear == 4 && (level == 22 || level == 23 || level == 25 || level == 30) && !isPenta)
            {
                // Check each row from bottom to top
                int bonus_row = -1;
                // Find the highest row with at least one block
                for (int j = 0; j < MaxY; j++)
                {
                    for (int i = 0; i < MaxX; i++)
                    {
                        if (grid[j][i] == 1)
                        {
                            bonus_row = j;
                            break;
                        }
                    }
                    if (bonus_row != -1)
                        break;
                }
                if (bonus_row == -1)
                    bonus_row = MaxY - 1;

                // clear the row
                for (int i = 0; i < MaxX; i++)
                    grid[bonus_row][i] = 0;

                // Show "PENTAKILL XXX"
                move_c(mid_x, bonus_row + 1);
                attron(COLOR_PAIR(1));
                printw(" XXX PENTAKILL XXX ");
                attroff(COLOR_PAIR(1));

                refresh();
                napms(500);
                score += 500;
                isPenta = elapsedSeconds;
            }
            if (isPenta && elapsedSeconds - isPenta > 5)
                isPenta = 0;

            // check if music stopped to play another one
            if (!Mix_PlayingMusic())
            {
                musicvar = rand() % 28 + 1;
                music = Mix_LoadMUS(track[musicvar]);
                Mix_PlayMusic(music, 0);
                level = musicvar + 1;
                mech(level);
                draw_right_menu(0, 0, 0, score, 1, level, player->endless_r);
                // reset all gimmick variables
                isKDA = 0, Hyperpop = 0, isEMO = 0, EMOvar = 0, TRUED = 0, Pianist = 0;
                isJazz = 0, isIll = 0, isDisco = 0, isPunk = 0;
                isEDM = 0;
                isPenta = 0;
                isCountry = 0;
                isHeart = 0;
                isMix = 0;
                Punkhead = 0;
                kinetic = 0;
                ethereal = 0;
                concussive = 0;
            }
        }

        if (score > player->endless_r)
        {
            notify();
            move_c(mid_x + 2, 10);
            printw(" NEW HIGHSCORE! ");
            move_c(mid_x + 2, 11);
            printw("     %05d", score);
            refresh();

            player->endless_r = score;
            update_file(*player);
            napms(5000);
        }
        // Free the music and close SDL_mixer
        Mix_FreeMusic(music);
    }
}

// game mode where player plays an infinite game and try to get the highest score
void endless_c(user *player)
{
    while (1)
    {
        notify();
        move_c(mid_x + 2, 10);
        printw("ENDLESS  CLASSIC");
        move_c(mid_x + 2, 11);
        printw("[1]PLAY  [0]BACK");
        refresh();

        nodelay(stdscr, FALSE);
        int confirm = '2';
        do
        {
            confirm = getch();
        } while (confirm != '1' && confirm != '0');
        if (confirm == '0')
            return;

        notify();
        move_c(mid_x + 2, 10);
        printw(" PRESS ANY KEY  ");
        move_c(mid_x + 2, 11);
        printw("   TO START!    ");
        refresh();
        int x = getch();
        nodelay(stdscr, TRUE);

        // clear the terminal
        clear();
        draw_UI();

        // Load the first music
        int musicvar = rand() % 29 + 1;
        Mix_Music *music = Mix_LoadMUS(track[musicvar]);
        Mix_PlayMusic(music, 0);

        // Declaration of flags and stats
        int flag = 0;
        int user = 0;
        int solidify = 0;
        char intro;

        // Declaration
        int xpos[4] = {0};
        int ypos[4] = {0};
        int current = 0;
        int next = rand() % 7;
        int rotate = 0;
        int score = 0;
        int paused = 0;

        time_t startTime = time(NULL); // Capture the start time
        time_t currentTime;
        int elapsedSeconds;
        int pause_duration = 0; // Duration of the pause in seconds

        // grid
        int grid[MaxY][MaxX] = {0};
        clear();
        draw_UI();
        draw_left_menu();
        draw_right_menu(0, 0, 0, score, 1, 0, player->endless_c);

        // Game body
        for (flag = 0; 1; flag++)
        {
            currentTime = time(NULL);
            elapsedSeconds = (int)(currentTime - startTime - pause_duration); // Adjust elapsed time for pause duration

            // draw_right_menu
            draw_right_menu(elapsedSeconds, current, next, score, 1, 0, player->endless_c);

            // print tetris grid
            if (user == 0)
            {
                user = 1;
                current = next;
                next = rand() % 7;
                tetromino(current, xpos, ypos);
            }
            print_grid(grid, xpos, ypos, 0, 0, 0, 0);

            // Check if game is already over
            int gameover = 0;
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    if (grid[j][i + 4] == 1)
                    {
                        gameover = 1;
                        break;
                    }
            if (gameover)
            {
                Mix_PauseMusic();
                game_over();
                napms(5000);
                break;
            }

            // user input
            int timer = 5;

            while (timer > 0)
            {
                int quit = 0;
                int input = getch();
                switch (input)
                {
                case 'a':
                case 'A':
                    if (move_check(-1, grid, xpos, ypos))
                        for (int i = 0; i < 4; i++)
                            xpos[i]--;
                    break;
                case 'd':
                case 'D':
                    if (move_check(1, grid, xpos, ypos))
                        for (int i = 0; i < 4; i++)
                            xpos[i]++;
                    break;
                case 's':
                case 'S':
                    if (move_check(0, grid, xpos, ypos))
                        for (int i = 0; i < 4; i++)
                            ypos[i]++;
                    break;
                case 'W':
                case 'w':
                case 'r':
                case 'R':
                    rotate_tetro(current, &rotate, xpos, ypos, grid);
                    break;
                case ' ':
                    while (move_check(0, grid, xpos, ypos))
                        for (int i = 0; i < 4; i++)
                            ypos[i]++;
                    solidify += 2;
                    quit = 1;
                    break;
                case '0':
                    Mix_PauseMusic();
                    if (confirmation())
                    {
                        Mix_FreeMusic(music);
                        return;
                    }
                    Mix_ResumeMusic();

                    break;
                case 'P':
                case 'p':
                    if (!paused)
                    {
                        pause_game();
                        paused = 1;
                        Mix_PauseMusic();
                        time_t pause_start = time(NULL); // check the time when paused
                        while (paused)
                        {
                            int temp = getch();
                            if (temp == '0')
                            {
                                Mix_FreeMusic(music);
                                return;
                            }
                            if (temp == 'P' || temp == 'p')
                            {
                                Mix_ResumeMusic();
                                time_t c = time(NULL);
                                pause_duration += c - pause_start;
                                paused = 0;
                            }
                        }
                    }
                    break;
                default:
                }
                if (quit)
                    break;
                draw_right_menu(elapsedSeconds, current, next, score, 1, 0, player->endless_c);
                print_grid(grid, xpos, ypos, 0, 0, 0, 0);
                timer--;
                int difficulty = (score / 250) + (elapsedSeconds / 60);
                int speed = 200 - difficulty;
                napms(speed);
            }

            // Natural movement and Check if a line is created
            if (user == 1 && move_check(0, grid, xpos, ypos))
                for (int i = 0; i < 4; i++)
                    ypos[i]++;

            if (!move_check(0, grid, xpos, ypos))
            {
                solidify += 1;
                if (solidify > 1)
                {
                    for (int i = 0; i < 4; i++)
                        grid[ypos[i]][xpos[i]] = 1;
                    user = 0;
                    solidify = 0;
                    rotate = 0;
                }
            }

            // Check if a line is created and update score
            line_check(grid, &score);

            // check if music stopped to play another one
            if (!Mix_PlayingMusic())
            {
                if (musicvar >= 30)
                    musicvar = 0;
                else
                    musicvar++;
                music = Mix_LoadMUS(track[musicvar]);
                Mix_PlayMusic(music, 0);
            }
        }

        if (score > player->endless_c)
        {
            notify();
            move_c(mid_x + 2, 10);
            printw(" NEW HIGHSCORE! ");
            move_c(mid_x + 2, 11);
            printw("     %05d", score);
            refresh();

            player->endless_c = score;
            update_file(*player);

            refresh();
            napms(5000);
        }

        // Free the music and close SDL_mixer
        Mix_FreeMusic(music);
    }
}

int main(int argc, char *argv[])
{
    // prepare ncurses
    initscr();
    cbreak();    // disable line buffering(input needing enter key) and enable immediate input
    noecho();    // disable echoing(appearance of typed input in terminal) of typed characters
    curs_set(0); // hide cursor

    // prepare other settings
    srand(time(NULL)); // seed the random number generator
    color();           // to initialize colors
    open_game();       // to show title screen

    // check SDL if it can intialize
    if (SDL_Init(SDL_INIT_AUDIO) < 0 || Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printw("Audio init failed. Press any key to exit.\n");
        refresh();
        getch();
        endwin();
        return 1;
    }

    // take user information
    user player = user_check();
    if (player.newplayer != 1)
        player = new_player();
    update_file(player);

    // main menu loop
    while (1)
    {
        draw_UI();
        draw_main_menu(player);
        nodelay(stdscr, FALSE);
        int choice = getch();
        nodelay(stdscr, TRUE);

        switch (choice)
        {
        // To Exit
        case '0':
        {
            if (confirmation())
            {
                Mix_CloseAudio();
                SDL_Quit();
                endwin();
                return 0;
            }
            continue;
        }
        // To play campaign
        case '1':
        {
            campaign(&player);
            break;
        }
        // To play endless remix
        case '2':
        {
            endless_r(&player);
            break;
        }
        // To play endless classic
        case '3':
        {
            endless_c(&player);
            break;
        }
        // For How to Play
        case '4':
        {
            clear();
            intro2tetris();
            break;
        }
        // For Settings
        case '5':
        {
            clear();
            if (user_settings(&player) == 1)
                return 0;
            break;
        }
        default:
            continue;
        }

        // notify the player that they will be taken back to the main menu
        notify();
        move_c(mid_x + 2, 10);
        printw("    BACK TO     ");
        move_c(mid_x + 2, 11);
        printw("   MAIN MENU    ");
        refresh();
        napms(2500);
    }
}
