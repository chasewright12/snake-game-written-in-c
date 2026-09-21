/*
 * Snake game in C (terminal, using ncurses)
 *
 * Compile:  gcc snake.c -o snake -lncurses
 * Run:      ./snake
 *
 * Controls: arrows or WASD to move, q to quit.
 */

#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 30
#define HEIGHT 20
#define MAX_LEN (WIDTH * HEIGHT)
#define TICK_MS 110

typedef struct
{
    int x, y;
} Point;
typedef enum
{
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

static Point snake[MAX_LEN]; /* snake[0] is the head */
static int length;
static Direction dir;
static Point food;
static int score;

static void spawn_food(void)
{
    int is_free;
    do
    {
        is_free = 1;
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
        for (int i = 0; i < length; i++)
        {
            if (snake[i].x == food.x && snake[i].y == food.y)
            {
                is_free = 0;
                break;
            }
        }
    } while (!is_free);
}

static void init_game(void)
{
    length = 3;
    dir = DIR_RIGHT;
    score = 0;
    for (int i = 0; i < length; i++)
    {
        snake[i].x = WIDTH / 2 - i;
        snake[i].y = HEIGHT / 2;
    }
    spawn_food();
}

static void draw(void)
{
    erase();

    for (int c = 0; c < WIDTH * 2 + 2; c++)
    {
        mvaddch(0, c, '#');
        mvaddch(HEIGHT + 1, c, '#');
    }
    for (int l = 0; l < HEIGHT + 2; l++)
    {
        mvaddch(l, 0, '#');
        mvaddch(l, WIDTH * 2 + 1, '#');
    }

    mvaddch(food.y + 1, food.x * 2 + 1, '*');

    for (int i = length - 1; i >= 0; i--)
    {
        mvaddch(snake[i].y + 1, snake[i].x * 2 + 1, i == 0 ? 'O' : 'o');
    }

    mvprintw(HEIGHT + 2, 0, "Score: %d   (arrows/WASD move, q/Q quits)", score);
    refresh();
}

static int move_snake(void)
{
    Point new_head = snake[0];

    switch (dir)
    {
    case DIR_UP:
        new_head.y--;
        break;
    case DIR_DOWN:
        new_head.y++;
        break;
    case DIR_LEFT:
        new_head.x--;
        break;
    case DIR_RIGHT:
        new_head.x++;
        break;
    }

    if (new_head.x < 0 || new_head.x >= WIDTH || new_head.y < 0 || new_head.y >= HEIGHT)
        return 0;

    int eats = (new_head.x == food.x && new_head.y == food.y);

    int limit = eats ? length : length - 1;
    for (int i = 0; i < limit; i++)
    {
        if (snake[i].x == new_head.x && snake[i].y == new_head.y)
            return 0;
    }

    if (eats && length < MAX_LEN)
        length++;

    for (int i = length - 1; i > 0; i--)
        snake[i] = snake[i - 1];
    snake[0] = new_head;

    if (eats)
    {
        score += 10;
        if (length < MAX_LEN)
            spawn_food();
    }
    return 1;
}

int main(void)
{
    srand((unsigned)time(NULL));

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int playing = 1;
    while (playing)
    {
        init_game();
        int alive = 1;
        int quit = 0;

        timeout(0);

        while (alive && !quit)
        {
            int ch;
            while ((ch = getch()) != ERR)
            {
                switch (ch)
                {
                case KEY_UP:
                case 'w':
                case 'W':
                    if (dir != DIR_DOWN)
                        dir = DIR_UP;
                    break;
                case KEY_DOWN:
                case 's':
                case 'S':
                    if (dir != DIR_UP)
                        dir = DIR_DOWN;
                    break;
                case KEY_LEFT:
                case 'a':
                case 'A':
                    if (dir != DIR_RIGHT)
                        dir = DIR_LEFT;
                    break;
                case KEY_RIGHT:
                case 'd':
                case 'D':
                    if (dir != DIR_LEFT)
                        dir = DIR_RIGHT;
                    break;
                case 'q':
                case 'Q':
                    quit = 1;
                    break;
                }
            }

            alive = move_snake();
            draw();
            napms(TICK_MS);
        }

        if (quit)
            break;

        timeout(-1);
        mvprintw(HEIGHT / 2, WIDTH - 8, " GAME OVER ");
        mvprintw(HEIGHT / 2 + 1, WIDTH - 12, " r = play again, q = quit ");
        refresh();

        int ch;
        do
        {
            ch = getch();
        } while (ch != 'r' && ch != 'R' && ch != 'q' && ch != 'Q');
        playing = (ch == 'r' || ch == 'R');
    }

    endwin();
    return 0;
}