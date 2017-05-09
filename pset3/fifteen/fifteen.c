/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int ntoils = d * d - 1;
    for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                board[i][j] = ntoils - (i * d + j);
            }
    
        }
    if(!(d % 2))
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    printf("\n");
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j]) 
            {
                printf("\033[s\033[1A+----+\033[\033[u"); 
                printf("| \033[1;31m%2i \033[0m",board[i][j]); 
                printf("\033[s\033[1B\033[5D+----+\033[u");}
            else
            {
                printf("\033[s\033[1A+----+\033[\033[u"); 
                printf("| __ \033[s\033[1B\033[4D----+\033[u");
            }
        }
        printf("|\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    int pos_x;
    int pos_y;
    if(tile > d*d-1 || tile < 1) return false;
    
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] == tile)
            {
                pos_x = j;
                pos_y = i;
            }
        }
    }
    int buffer;
    for(int i = 0; i < 4; i++)
    {
        int temp = i % 2;
        if(i/2)
        {
            if(pos_x + 2 * temp - 1 >= 0 && pos_x + 2 * temp - 1 < d)       // if(0 <= pos_x +|-1 < d)
            {
                if(board[pos_y][pos_x + 2 * temp - 1] == 0)                 // if found empty tile
                {
                    buffer = board[pos_y][pos_x];                           // then swap
                    board[pos_y][pos_x] = board[pos_y][pos_x + 2 * temp - 1];
                    board[pos_y][pos_x + 2 * temp - 1] = buffer;
                    return true;
                }
            }
        }
        else
        {
            if(pos_y + 2 * temp - 1 >= 0 && pos_y + 2 * temp - 1 < d)
            {
                if(board[pos_y + 2 * temp - 1][pos_x] == 0)
                {
                    buffer = board[pos_y][pos_x];
                    board[pos_y][pos_x] = board[pos_y + 2 * temp - 1][pos_x];
                    board[pos_y + 2 * temp - 1][pos_x] = buffer;
                    return true;
                    
                }
                
            }
            
        }
        
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int count = 0;
    while( (board[count / d ][count % d] != 0) 
            && (board[count / d][count % d] == count + 1)  )
    {
        count++;
    }
    
    if(count == d * d -1) return true;                  // if all numbers sorted
    else return false;
}
