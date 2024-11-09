/**
 *  Alex Carbajal
 *  CPT_S 411, Fall 2024
 *  Homework 3: Conway's Game of Life
 */

#include "board.h"

void init_board(int board[][N])
{
    for (int row = 0; row < M; row++)
    {
        for (int col = 0; col < N; col++)
        {
            // random value of 0 or 1
            board[row][col] = rand() % 2;
        }
    }
}

void print_board(int board[][N], int M_local)
{
    for (int row = 0; row < M_local; row++)
    {
        for (int col = 0; col < N; col++)
        {
            printf("%d ", board[row][col]);
        }
        printf("\n");
    }
}

void get_firstp_board(int board_row, int total_rpt, int local_board[][N], int board[][N])
{
    // first row of local_board is the last row of board (wrap-around)
    for (int col = 0; col < N; col++)
    {
        local_board[0][col] = board[M - 1][col];
    }

    for (int row = 1; row < total_rpt; row++, board_row++)
    {
        for (int col = 0; col < N; col++)
        {
            local_board[row][col] = board[board_row][col];
        }
    }
}

void get_lastp_board(int board_row, int total_rpt, int local_board[][N], int board[][N])
{
    for (int row = 0; row < (total_rpt - 1); row++, board_row++)
    {
        for (int col = 0; col < N; col++)
        {
            local_board[row][col] = board[board_row][col];
        }
    }

    // last row of local_board is the first row of board (wrap-around)
    for (int col = 0; col < N; col++)
    {
        local_board[total_rpt - 1][col] = board[0][col];
    }
}

void get_p_board(int board_row, int total_rpt, int local_board[][N], int board[][N])
{
    for (int row = 0; row < total_rpt; row++, board_row++)
    {
        for (int col = 0; col < N; col++)
        {
            local_board[row][col] = board[board_row][col];
        }
    }
}

void apply_p_board(int board_row, int total_rpt, int local_board[][N], int board[][N])
{
    for (int row = 1; row < (total_rpt - 1); row++, board_row++)
    {
        for (int col = 0; col < N; col++)
        {
            board[board_row][col] = local_board[row][col];
        }
    }
}

int count_neighbors(int board[][N], int row, int col, int M_local, int thread_id, int size)
{
    int neighbors = 0;
    int left = (col == 0);        // check if leftmost column
    int right = (col == (N - 1)); // check if rightmost column

    // check that the cell position exists and then check if the cell is alive or dead
    // top left (second condition for wrap-around)
    if ((board[row - 1][col - 1]) || (left && board[row - 1][N - 1]))
    {
        neighbors++;
    }
    // top
    if (board[row - 1][col])
    {
        neighbors++;
    }
    // top right (second condition for wrap-around)
    if ((board[row - 1][col + 1]) || (right && board[row - 1][0]))
    {
        neighbors++;
    }
    // right (second condition for wrap-around)
    if ((board[row][col + 1]) || (right && board[row][0]))
    {
        neighbors++;
    }
    // bottom right (second condition for wrap-around)
    if ((board[row + 1][col + 1]) || (right && board[row + 1][0]))
    {
        neighbors++;
    }
    // bottom
    if (board[row + 1][col])
    {
        neighbors++;
    }
    // bottom left (second condition for wrap-around)
    if ((board[row + 1][col - 1]) || (left && board[row + 1][N - 1]))
    {
        neighbors++;
    }
    // left (second condition for wrap-around)
    if ((board[row][col - 1]) || (left && board[row][N - 1]))
    {
        neighbors++;
    }

    return neighbors;
}

void update_board(int board[][N], int M_local, int thread_id, int size)
{
    int neighbors = 0; // # of neighbors a cell has
    int new_board[M_local][N];
    int final_row = M_local + 1;

    for (int row = 1; row < final_row; row++)
    {
        for (int col = 0; col < N; col++)
        {
            neighbors = count_neighbors(board, row, col, M_local, thread_id, size);

            // cell is alive (1)
            if (board[row][col] == 1)
            {
                // underpopulation: less than 3 neighbors -> cell dies
                // overpopulation: greater than 5 neighbors -> cell dies
                if (neighbors < 3 || neighbors > 5)
                {
                    new_board[row - 1][col] = 0;
                }
                // between 3 & 5 neighbors -> cell lives
                else
                {
                    new_board[row - 1][col] = 1;
                }
            }
            // cell is not alive (0)
            else
            {
                // reproduction: dead cell has between 3 & 5 (inclusive) living neighbors -> cell becomes alive
                if (neighbors >= 3 && neighbors <= 5)
                {
                    new_board[row - 1][col] = 1;
                }
                // cell remains dead
                else
                {
                    new_board[row - 1][col] = 0;
                }
            }
        }
    }

    // update the board with the new values
    for (int row = 1; row < final_row; row++)
    {
        for (int col = 0; col < N; col++)
        {
            board[row][col] = new_board[row - 1][col];
        }
    }
}
