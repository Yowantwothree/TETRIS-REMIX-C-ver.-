#ifndef TETRIS_H
#define TETRIS_H

// game layout dimensions
#define left_x 0
#define mid_x 22
#define right_x 44
#define left_width 20
#define mid_width 24
#define right_width 34
#define left_y 22
#define mid_y 22
#define right_y 22

// tetris grid dimension
#define MaxX 10
#define MaxY 20

// path for music tracks
static const char *track[30] = {
    "tracks/8bit.mp3",
    "tracks/hyperpop.mp3",
    "tracks/8bit_hyperpop.mp3",
    "tracks/kda.mp3",
    "tracks/8bit_kda.mp3",
    "tracks/piano.mp3",
    "tracks/8bit_hyperpop_kda.mp3",
    "tracks/emo.mp3",
    "tracks/emo_hyperpop_kda.mp3",
    "tracks/truedamage.mp3",
    "tracks/true_emo_piano.mp3",
    "tracks/jazz.mp3",
    "tracks/emo_jazz.mp3",
    "tracks/illbeats.mp3",
    "tracks/jazz_illbeats.mp3",
    "tracks/disco.mp3",
    "tracks/disco_jazz_hyperpop.mp3",
    "tracks/punk.mp3",
    "tracks/kda_punk.mp3",
    "tracks/edm.mp3",
    "tracks/emo_edm_hyperpop.mp3",
    "tracks/pentakill.mp3",
    "tracks/jazz_pentakill_punk.mp3",
    "tracks/country.mp3",
    "tracks/country_pentakill.mp3",
    "tracks/heartsteel.mp3",
    "tracks/heartsteel_kda_truedamage.mp3",
    "tracks/mixmaster.mp3",
    "tracks/mixmaster_edm_hyperpop.mp3",
    "tracks/maestro.mp3"};

// user data structure
typedef struct
{
    int newplayer;    // checks for new user
    char name[51];    // user name
    int campaign_lvl; // campaign level - max 30
    int endless_r;    // highscore in endless remix
    int endless_c;    // highscore in endless classic
} user;

// UI
void move_c(int x, int y);
void draw_UI(void);
void draw_left_menu();
void draw_right_menu(int time, int current, int next, int score, int endless, int level, int highscore);
char *mechanic(int track);
char *track_title(int level);
void mech(int level);
void notify();
void pause_game();
void game_over();
int confirmation();
void open_game();
void draw_main_menu(user player);
void color();

// User Data
user user_check();
void update_file(user player);
user new_player();
void intro2tetris();
int user_settings(user *player);

// Tetris Mechanics
void update_grid(int grid[][MaxX], int line);
int line_check(int grid[][MaxX], int *score);
int move_check(int side, int grid[][MaxX], int xpos[], int ypos[]);
void print_grid(int grid[][MaxX], int xpos[], int ypos[], int EMOvar, int isEMO, int isKDA, int isCountry);
void tetromino(int type, int xpos[], int ypos[]);
void rotate_tetro(int type, int *rotation, int xpos[], int ypos[], int grid[][MaxX]);

// Gimmicks
void HyperPop(int grid[][MaxX]);
void Jazz(int grid[][MaxX]);
void Illbeats(int grid[][MaxX]);
void Disco(int grid[][MaxX], int direction, int xpos[], int ypos[]);
void createHeart(int grid[MaxY][MaxX]);
void Mixmaster(int grid[][MaxX], int cycle, int *score);

#endif