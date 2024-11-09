/**
 *  Alex Carbajal
 *  CPT_S 411, Fall 2024
 *  Homework 3: Conway's Game of Life
 */

#ifndef BOARD
#define BOARD

// matrix grid (MxN)
#define M 64  // # of rows
#define N 16  // # of columns
#define T 100 // # of time steps in simulation

// 2 possible states for a cell
#define ALIVE 1
#define DEAD 0

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief initialize a matrix board with random values of 0 or 1
 *
 * @param board matrix board to initialize with random values of 0 or 1
 */
extern void init_board(int board[][N]);

/**
 * @brief print a matrix board
 *
 * @param board matrix board to print out
 * @param M_local number of rows of the matrix board to print out
 */
extern void print_board(int board[][N], int M_local);

/**
 * @brief copy the corresponding rows of the board into the local board of the first thread
 *
 * @param board_row board row being processed
 * @param total_rpt amount of rows each thread works with
 * @param local_board rows and columns a thread works with
 * @param board full board containing all the rows and columns
 */
extern void get_firstp_board(int board_row, int total_rpt, int local_board[][N], int board[][N]);

/**
 * @brief copy the corresponding rows of the board into the local board of the last thread
 *
 * @param board_row board row being processed
 * @param total_rpt amount of rows each thread works with
 * @param local_board rows and columns a thread works with
 * @param board full board containing all the rows and columns
 */
extern void get_lastp_board(int board_row, int total_rpt, int local_board[][N], int board[][N]);

/**
 * @brief copy the corresponding rows of the board into the local board of the thread
 *
 * @param board_row board row being processed
 * @param total_rpt amount of rows each thread works with
 * @param local_board rows and columns a thread works with
 * @param board full board containing all the rows and columns
 */
extern void get_p_board(int board_row, int total_rpt, int local_board[][N], int board[][N]);

/**
 * @brief apply the rows of the local board to the corresponding rows of the board
 *
 * @param board_row board row being processed
 * @param total_rpt amount of rows each thread works with
 * @param local_board rows and columns a thread works with
 * @param board full board containing all the rows and columns
 */
extern void apply_p_board(int board_row, int total_rpt, int local_board[][N], int board[][N]);

/**
 * @brief count the number of live neighbor cells that a cell in a matrix board has
 *
 * @param board matrix board
 * @param row row of the cell to check neighbors of
 * @param col column of the cell to check neighbors of
 * @param M_local number of rows of the matrix board
 * @param thread_id id of the thread to have the number of live neighbors of one of its cells counted
 * @param size total number of threads executing the program
 * @return int number of live neighbors the cell has
 */
extern int count_neighbors(int board[][N], int row, int col, int M_local, int thread_id, int size);

/**
 * @brief fully update a matrix board by checking all of its cells
 *
 * @param board matrix board to check and update
 * @param M_local number of rows of the matrix board
 * @param thread_id id of the thread having its cells updated
 * @param size total number of threads executing the program
 */
extern void update_board(int board[][N], int M_local, int thread_id, int size);

#endif
