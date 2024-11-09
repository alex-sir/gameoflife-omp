/**
 *  Alex Carbajal
 *  CPT_S 411, Fall 2024
 *  Homework 3: Conway's Game of Life
 */

#include "board.h"

// root process
#define ROOT 0

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        // e.g. ./game_of_life 4
        printf("Usage: %s <threads>\n", argv[0]);
        return 0;
    }
    int num_threads = strtol(argv[1], NULL, 10);

    // exit the program if the number of rows is not divisble by the number of processes
    if ((M % num_threads) != 0)
    {
        printf("Error: %d rows not divisible by %d processes\n", M, num_threads);
        return 0;
    }

    srand(time(NULL));

    // record the start time
    double start_time = omp_get_wtime();

    int board[M][N];
    // int board[M][N] = {
    //     {0, 1, 0, 0, 1}, {1, 0, 1, 1, 0}, {0, 1, 1, 1, 0}, {0, 1, 0, 0, 1}, {0, 0, 1, 1, 1}}; // debugging
    // setup the initial board
    init_board(board);
    printf("*** INITIAL BOARD ***\n");
    print_board(board, M);
    printf("\n");

    int rows_per_thread = M / num_threads;
    int total_rpt = rows_per_thread + 2; // +2 for the top & bottom neighbor rows

#pragma omp parallel num_threads(num_threads)
    {
        int local_board[total_rpt][N];
        int thread_id = omp_get_thread_num();
        int board_row = rows_per_thread * thread_id;
        int top_neighbor = board_row - 1;

        /* steps for every thread to update the board according to the rules of Conway's Game of Life
         * 1. copy the rows from board to local_board
         *    this includes the rows_per_process + 2 for the top and bottom neighbors
         * 2. update all the cells in local_board according to the rules of the game
         * 3. apply the updated cells from local_board to board
         * 4. repeat 1-3 for the specified number of steps T
         */
        for (int step = 0; step < T; step++)
        {
            // first process wraps around to the bottom neighbor
            if (thread_id == 0)
            {
                get_firstp_board(board_row, total_rpt, local_board, board);
            }
            // last process wraps around to the first neighbor
            else if (thread_id == (num_threads - 1))
            {
                get_lastp_board(top_neighbor, total_rpt, local_board, board);
            }
            else
            {
                // copy a section of the board to the local_board
                get_p_board(top_neighbor, total_rpt, local_board, board);
            }
            update_board(local_board, rows_per_thread, thread_id, num_threads);
#pragma omp barrier // wait for all threads to finish before applying the updates to the board
            // copy the updated local_board to board
            apply_p_board(board_row, total_rpt, local_board, board);
#pragma omp barrier // wait for all threads to finish before moving to the next iteration
        }
    }

    // print the final board
    printf("*** FINAL BOARD ***\n");
    print_board(board, M);

    // record the end time
    double end_time = omp_get_wtime();
    // calculate the elapsed time
    double elapsed_time = end_time - start_time;
    printf("\nElapsed Time: %lf seconds\n", elapsed_time);

    return 0;
}
