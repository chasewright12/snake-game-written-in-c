# Snake Game written in C

Snake game for the terminal, written in pure C with the [ncurses](https://invisible-island.net/ncurses/) library. Everything lives in a single file: `snake.c`.

```
##########################################################
#                                                        #
#                          *                             #
#                                                        #
#              ooooO                                     #
#                                                        #
##########################################################
Score: 30   (arrows/WASD move, q/Q quits)
```

## How to play

Guide the snake around the field to eat the food (`*`). Each time it eats, it grows one segment and you earn **10 points**. If the snake hits a wall or its own body, the game ends.

| Key | Action |
|---|---|
| `↑` / `W` | Move up |
| `↓` / `S` | Move down |
| `←` / `A` | Move left |
| `→` / `D` | Move right |
| `Q` | Quit the game |
| `R` | Play again (on the game over screen) |

## Requirements

- A C compiler (`gcc` or `clang`)
- The **ncurses** development library
- A terminal (Linux, macOS, or WSL on Windows)

## Installing ncurses

```bash
# Ubuntu / Debian
sudo apt install libncurses-dev

# Fedora
sudo dnf install ncurses-devel

# Arch
sudo pacman -S ncurses

# macOS (usually preinstalled; only if needed)
brew install ncurses
```

> **Windows:** ncurses doesn't come with plain Windows. The simplest way is to use [WSL](https://learn.microsoft.com/windows/wsl/install) and follow the Linux instructions above. Another option is [PDCurses](https://pdcurses.org/), which has a very similar API.

## Compiling and running

```bash
gcc snake.c -o snake -lncurses
./snake
```

## How the code works

- **The snake** is an array of points (`snake[0]` is the head), each with `x` and `y` coordinates.
- **The game loop** repeats on every "tick": read the keys → calculate the new head position → check collisions → move the body → draw the screen → wait (`napms`).
- **Movement:** each segment takes the position of the segment before it, from back to front. To grow, `length` is increased before shifting the body.
- **Collisions:** with the walls (position outside the field) and with the snake's own body. The tip of the tail is ignored when the snake isn't growing, since it moves out of place in the same step.
- **Food:** a random position generated with `rand()`, retrying if it lands on the snake.
- **No 180° turns:** if the snake is going right, the "left" key is ignored.
- **Each cell uses 2 screen columns**, so the field looks square in the terminal.

### Main functions

| Function | What it does |
|---|---|
| `init_game()` | Resets the snake, the score, and the food |
| `spawn_food()` | Picks a free position for the food |
| `move_snake()` | Moves the snake one step; returns `0` if it died |
| `draw()` | Draws the field, the snake, the food, and the score |
| `main()` | Sets up ncurses and runs the main loop and the game over screen |

## Customizing

The constants at the top of `snake.c` control the game:

```c
#define WIDTH 30       // field columns
#define HEIGHT 20      // field rows
#define TICK_MS 110    // time between moves (lower = faster)
```

Ideas to extend the project:

- Increase the speed as the score grows
- Save the high score to a file
- Let the snake wrap around the walls (using `%` instead of checking the limits)
- Add colors with ncurses' `start_color()` and `init_pair()`
- Add a start menu with difficulty levels