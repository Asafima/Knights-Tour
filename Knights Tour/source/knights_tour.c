/*=========================== LIBRARIES & MACROS ============================*/

#include <assert.h> /* assert */
#include <time.h>   /* time */

#include "knights_tour.h"
#include "bitarray.h" /* bitarray */

#define IS_VALID_POSITION(x, y) (0 <= (x) && BOARD_LEN > (x) && 0 <= (y) && BOARD_LEN > (y))
#define GET_BOARD_INDEX(x, y) ((BOARD_LEN * (y)) + (x))
#define HAS_VISITED(board, pos) (BitArrayGetVal(board, pos))
#define VISITED_ALL(board) (BitArrayCountOn(board))
#define TIMEFRAME (300)

enum board
{
    BOARD_LEN = 8,
    MOVE_COUNT = 8,
    BOARD_SIZE = BOARD_LEN * BOARD_LEN,
    INVALID_MOVE = 65
};

typedef enum lut_status
{
    NOT_INITIALIZED,
    INITIALIZED
} lut_status_t;

typedef enum method
{
    BRUTE_FORCE,
    WARNSDORFF
} method_t;

/*============================== DECLARATIONS ===============================*/

static return_status_t DoKnightsTour(unsigned char, unsigned char, unsigned char *, method_t);
static return_status_t KnightsTourRec(unsigned char, bitarray_t, unsigned char *, int next_pos_table[BOARD_SIZE][MOVE_COUNT], time_t, int);

static void InitNextPositionsTable(int next_pos_table[BOARD_SIZE][MOVE_COUNT], int);
static void SortByDegree(int, int next_pos_table[BOARD_SIZE][MOVE_COUNT], bitarray_t);
void InsertionSortKnightTour(int *degree_counter, int next_pos_table[BOARD_SIZE][MOVE_COUNT], int curr_pos);
static void SaveCurrentPosition(bitarray_t *board, int pos, unsigned char *path);

/*====================== FUNCTION DEFINITION =======================*/

return_status_t KnightsTour(unsigned char x_pos, unsigned y_pos, unsigned char *path)
{
    return (DoKnightsTour(x_pos, y_pos, path, BRUTE_FORCE));
}

return_status_t WarnsdorffKnightsTour(unsigned char x_pos, unsigned char y_pos, unsigned char *path)
{
    return (DoKnightsTour(x_pos, y_pos, path, WARNSDORFF));
}

static return_status_t DoKnightsTour(unsigned char x_pos, unsigned char y_pos, unsigned char *path, method_t is_warnsdorff)
{
    static int next_pos_table[BOARD_SIZE][MOVE_COUNT];

    bitarray_t board = 0;
    int start_pos = GET_BOARD_INDEX(x_pos, y_pos);

#ifndef NDEBUG
    int x = x_pos;
    int y = y_pos;
    assert(IS_VALID_POSITION(x, y));
#endif /* __NDEBUG __*/
    assert(NULL != path);
    InitNextPositionsTable(next_pos_table, is_warnsdorff);

    return (KnightsTourRec(start_pos, board, path, next_pos_table, time(NULL), is_warnsdorff));
}

static return_status_t KnightsTourRec(unsigned char curr_pos, bitarray_t board, unsigned char *path,
                                      int next_pos_table[BOARD_SIZE][MOVE_COUNT], time_t start_time,
                                      int is_warnsdorff)
{
    int move = 0;

    if (TIMEFRAME <= (time(NULL) - start_time))
    {
        return (TIMEOUT);
    }
    SaveCurrentPosition(&board, curr_pos, path);
    if (BOARD_SIZE == VISITED_ALL(board))
    {
        return (TRUE);
    }

    if (is_warnsdorff)
    {
        SortByDegree(curr_pos, next_pos_table, board);
    }

    for (move = 0; move < MOVE_COUNT; ++move)
    {
        int next_pos = next_pos_table[curr_pos][move];
        if (INVALID_MOVE != next_pos && !HAS_VISITED(board, next_pos))
        {
            return_status_t status = KnightsTourRec(next_pos, board, path + 1, next_pos_table, start_time, is_warnsdorff);
            if (FALSE != status)
            {
                return (status);
            }
        }
    }
    return (FALSE);
}

static void SaveCurrentPosition(bitarray_t *board, int pos, unsigned char *path)
{
    *board = BitArraySetOn(*board, pos);
    *path = pos;
}

static void SortByDegree(int curr_pos, int next_pos_table[BOARD_SIZE][MOVE_COUNT], bitarray_t board)
{
    int curr_move = 0, next_move = 0;
    int degree_counter[MOVE_COUNT] = {0};

    for (curr_move = 0; curr_move < MOVE_COUNT; ++curr_move)
    {
        int next_pos = next_pos_table[curr_pos][curr_move];
        if (INVALID_MOVE == next_pos || HAS_VISITED(board, next_pos))
        {
            degree_counter[curr_move] = INVALID_MOVE;
            continue;
        }
        for (next_move = 0; next_move < MOVE_COUNT; ++next_move)
        {
            int next_next = next_pos_table[next_pos_table[curr_pos][curr_move]][next_move];
            if (INVALID_MOVE != next_next && !HAS_VISITED(board, next_next))
            {
                ++degree_counter[curr_move];
            }
        }
    }

    InsertionSortKnightTour(degree_counter, next_pos_table, curr_pos);
}

void InsertionSortKnightTour(int *degree_counter, int next_pos_table[BOARD_SIZE][MOVE_COUNT], int curr_pos)
{
    size_t i = 0;
    long j = 0;
    int key = 0, key2 = 0;

    for (i = 1; i < MOVE_COUNT; ++i)
    {
        key = degree_counter[i];
        key2 = next_pos_table[curr_pos][i];
        j = i - 1;
        while (degree_counter[j] > key && 0 <= j)
        {
            degree_counter[j + 1] = degree_counter[j];
            next_pos_table[curr_pos][j + 1] = next_pos_table[curr_pos][j];
            j--;
        }
        degree_counter[j + 1] = key;
        next_pos_table[curr_pos][j + 1] = key2;
    }
}

static void InitNextPositionsTable(int next_pos_table[BOARD_SIZE][MOVE_COUNT], int warnsdorff)
{
    static lut_status_t is_lut_init = NOT_INITIALIZED;
    if (!warnsdorff || !is_lut_init)
    {
        static const int x_moves[MOVE_COUNT] = {2, 1, -1, -2, -2, -1, 1, 2};
        static const int y_moves[MOVE_COUNT] = {1, 2, 2, 1, -1, -2, -2, -1};
        int x = 0, y = 0, curr_move = 0;
        int curr_pos = 0, next_pos = 0;
        int next_x_pos = 0, next_y_pos = 0;

        for (; y < BOARD_LEN; ++y)
        {
            for (x = 0; x < BOARD_LEN; ++x)
            {
                for (curr_move = 0; curr_move < MOVE_COUNT; ++curr_move)
                {
                    curr_pos = GET_BOARD_INDEX(x, y);

                    next_x_pos = x + x_moves[curr_move];
                    next_y_pos = y + y_moves[curr_move];
                    next_pos = GET_BOARD_INDEX(next_x_pos, next_y_pos);

                    next_pos_table[curr_pos][curr_move] =
                        (IS_VALID_POSITION(next_x_pos, next_y_pos)) ? next_pos : INVALID_MOVE;
                }
            }
        }
        is_lut_init = INITIALIZED;
    }
}
