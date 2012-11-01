/*
 
 
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "chess-reduce.h"

void test_1(void)
{
	// Test basic shifting.
	int shift = chz_board_Shift(0, 0);
	assert(shift == 63);
	
	shift = chz_board_Shift(7, 7);
	assert(shift == 0);
	
	shift = chz_board_Shift(0, 7);
	assert(shift == 7);
}

void test_2(void)
{
	int inside = chz_board_Inside(0, 0);
	assert(inside);
	
	inside = chz_board_Inside(-1, 0);
	assert(!inside);
	
	inside = chz_board_Inside(0, -1);
	assert(!inside);
	
	inside = chz_board_Inside(7, 7);
	assert(inside);
	
	inside = chz_board_Inside(8, 8);
	assert(!inside);
}

void test_3(void)
{
	chz_board_t board = {};
	
	chz_board_t newboard = chz_board_SetPosition(board, 7, 7, 1);
	assert(newboard.val == 1);
	
	newboard = chz_board_SetPosition(board, 6, 7, 1);
	assert(newboard.val == 2);
	
	newboard = chz_board_SetPosition(board, 0, 7, 1);
	assert(newboard.val == 128);
	
	newboard = chz_board_SetPosition(board, 0, 0, 1);
	assert(newboard.val == ((uint64_t)1 << 63));
	
	newboard = chz_board_SetPosition(board, -1, 0, 1);
	assert(newboard.val == (uint64_t)0);
	
	newboard = chz_board_SetPosition(board, 0, 0, 1);
	newboard = chz_board_SetPosition(board, 7, 7, 1);
	newboard = chz_board_SetPosition(newboard, 0, 0, 0);
	assert(newboard.val == (uint64_t)1);
}

void test_4(void)
{
	chz_board_t board = {};
	
	board = chz_board_SetPosition(board, 0, 0, 1);
	assert(chz_board_GetPosition(board, 0, 0));
	assert(!chz_board_GetPosition(board, 0, 1));
	assert(!chz_board_GetPosition(board, 1, 0));
	assert(!chz_board_GetPosition(board, -1, 0));
}

void test_5(void)
{
	chz_board_t board = chz_board_Row(7);
	
	assert(board.val == 255ULL);
	
	board = chz_board_Row(6);
	assert(board.val == 0xFF00ULL);
	
	board = chz_board_Or(chz_board_Row(6), chz_board_Row(7));
	assert(board.val == 0xFFFFULL);
}

void test_6(void)
{
	chz_board_t board;
	int i, j;
	for (i = 0; i < 8; i++) {
		board = chz_board_Column(i);
		for (j = 0; j < 8; j++) {
			assert(chz_board_GetPosition(board, i, j));
		}
	}
}

void test_7(void)
{
	chz_board_t board = {};
	chz_board_SetPosition(board, 3, 4, 1);
	
	chz_board_t newboard = chz_board_Diagonals(3, 4);
	assert(chz_board_GetPosition(newboard, 3-1, 4-1));
	assert(chz_board_GetPosition(newboard, 3+1, 4+1));
	assert(chz_board_GetPosition(newboard, 3-1, 4+1));
	assert(chz_board_GetPosition(newboard, 3+1, 4-1));
}

void test_8(void)
{
	chz_board_t board = {};
	
	board = chz_board_SetPosition(board, 0, 0, 1);
	
	// PrintBoard(board);
	
	chz_potential_t potential;
	
	chz_potential_UnknownBricks(board, &potential);
	
	int verbose = 0;
	if (verbose) {
		int i, j;
		for (j = 0; j < 8; j++) {
			for (i = 0; i < 8; i++) {
				printf("%2x ", potential.arr[i+8*j]);
			}
			printf("\n");
		}
		printf("\n");
	}
	
	// Check that king can take that position.
	assert(potential.arr[0+8*1] & (1<<6));
}

void test_9(void)
{
	chz_potential_t p = {};
	
	chz_potential_Default(&p);
	
	// PrintPotential(&p);
	
	chz_board_t white = chz_board_OfColor(&p, 1);
	
	// PrintBoard(white);
	
	int i;
	for (i = 0; i < 8; i++) {
		assert(chz_board_GetPosition(white, i, 6));
		assert(chz_board_GetPosition(white, i, 7));
	}
	
	chz_board_t black = chz_board_OfColor(&p, 0);
	for (i = 0; i < 8; i++) {
		assert(chz_board_GetPosition(black, i, 0));
		assert(chz_board_GetPosition(black, i, 1));
	}
}

void test_10(void)
{
	chz_board_t board = {};
	board = chz_board_SetPosition(board, 2, 0, 1);
	board = chz_board_SetPosition(board, 2, 1, 1);
	assert(chz_board_Next(board, 0) == 2);
	assert(chz_board_Next(board, 2) == 10);
	assert(chz_board_Next(board, 10) == 64);
}

void test_11(void)
{
	chz_potential_t start = {};
	chz_potential_Default(&start);
	
	// Get white bricks.
	chz_board_t white = chz_board_OfColor(&start, 1);
	// chz_board_PrintBoard(white);
	
	int count = chz_board_Count(white);
	// printf("%i\n", count);
	assert(count == 16);
	
	// Check that two steps are allowed at start for pawns.
	chz_board_t pawnMoves = chz_board_PawnMoves(0, 6, 1, 1);
	count = chz_board_Count(pawnMoves);
	// printf("%i\n", count);
	assert(count == 2);
	assert(chz_board_GetPosition(pawnMoves, 0, 5));
	assert(chz_board_GetPosition(pawnMoves, 0, 4));
	
	// Check that two steps are not allowed after start.
	pawnMoves = chz_board_PawnMoves(0, 6, 1, 0);
	count = chz_board_Count(pawnMoves);
	assert(count == 1);
	assert(chz_board_GetPosition(pawnMoves, 0, 5));
	assert(!chz_board_GetPosition(pawnMoves, 0, 4));
	
	// Check that black is setup right.
	chz_board_t black = chz_board_OfColor(&start, 0);
	count = chz_board_Count(black);
	assert(count == 16);
	
	// Check all white pawns.
	chz_board_t whitePawns = chz_board_InState(&start, chz_state_Pawn(1));
	count = chz_board_Count(whitePawns);
	assert(count == 8);
	assert(chz_board_GetPosition(whitePawns, 0, 6));
	
	// Check all black pawns.
	chz_board_t blackPawns = chz_board_InState(&start, chz_state_Pawn(0));
	count = chz_board_Count(blackPawns);
	assert(count == 8);
	assert(chz_board_GetPosition(blackPawns, 0, 1));
	
	// Check all bricks.
	chz_board_t bricks = chz_board_NotInState(&start, chz_state_Empty());
	count = chz_board_Count(bricks);
	assert(count == 32);
}

void test_12(void)
{
	chz_board_t board = {};
	
	board = chz_board_SetPosition(board, 4, 4, 1);
	int left = chz_board_Left(board, 4, 4);
	assert(left == -1);
	
	board = chz_board_SetPosition(board, 0, 4, 1);
	left = chz_board_Left(board, 4, 4);
	// printf("%i\n", left);
	assert(left == 0);
	
	int right = chz_board_Right(board, 4, 4);
	assert(right == -1);
	
	board = chz_board_SetPosition(board, 5, 4, 1);
	right = chz_board_Right(board, 4, 4);
	assert(right == 5);
	
	int up = chz_board_Up(board, 4, 4);
	assert(up == -1);
	
	board = chz_board_SetPosition(board, 4, 2, 1);
	up = chz_board_Up(board, 4, 4);
	assert(up == 2);
	
	int down = chz_board_Down(board, 4, 4);
	assert(down == -1);
	
	board = chz_board_SetPosition(board, 4, 7, 1);
	down = chz_board_Down(board, 4, 4);
	assert(down == 7);
	
	right = chz_board_Right(board, 4, 4);
	assert(right == 5);
	
	left = chz_board_Left(board, 4, 4);
	assert(left == 0);
	
	up = chz_board_Up(board, 4, 4);
	assert(up == 2);
}

void test_13(void)
{
	chz_board_t board = {};
	
	board = chz_board_SetPosition(board, 0, 0, 1);
	int s = chz_board_Direction(board, 0, 0, 1, 1);
	assert(s == -1);
	
	board = chz_board_SetPosition(board, 7, 7, 1);
	s = chz_board_Direction(board, 0, 0, 1, 1);
	assert(s == 63);
}

void test_14(void)
{
	assert(chz_state_CanTake(chz_state_Pawn(0), chz_state_Queen(1)));
	assert(!chz_state_CanTake(chz_state_Pawn(0), chz_state_Queen(0)));
	
	assert(chz_board_PawnDirection(0) == 1);
	assert(chz_board_PawnDirection(1) == -1);
	
	// Check target.
	chz_potential_t potential;
	chz_potential_Default(&potential);
	
	chz_board_t board = chz_board_Target(&potential, chz_state_Queen(1), 3, 1);
	assert(chz_board_Count(board) == 1);
	assert(chz_board_GetPosition(board, 3, 1));
	
	// Make sure pawn can not attack anybody at start.
	chz_board_t pawnAttacks = chz_board_PawnAttacks(&potential, 1, 6);
	assert(chz_board_Count(pawnAttacks) == 0);
	
	potential.arr[0 + 8*5] = chz_state_Pawn(0);
	pawnAttacks = chz_board_PawnAttacks(&potential, 1, 6);
	assert(chz_board_Count(pawnAttacks) == 1);
	
	potential.arr[2 + 8*5] = chz_state_Pawn(1);
	pawnAttacks = chz_board_PawnAttacks(&potential, 1, 6);
	assert(chz_board_Count(pawnAttacks) == 1);
	
	potential.arr[2 + 8*5] = chz_state_Pawn(0);
	pawnAttacks = chz_board_PawnAttacks(&potential, 1, 6);
	assert(chz_board_Count(pawnAttacks) == 2);
}

int main(int argc, char *argv[])
{
	test_1();
	test_2();
	test_3();
	test_4();
	test_5();
	test_6();
	test_7();
	test_8();
	test_9();
	test_10();
	test_11();
	test_12();
	test_13();
	test_14();
	
	return 0;
}
