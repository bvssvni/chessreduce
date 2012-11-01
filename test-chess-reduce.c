/*
 
 
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "chess-reduce/chess-reduce.h"

#define func(ns, name) static __typeof__(ns##_##name) * const name = ns##_##name

func(chz, board_Shift);
func(chz, board_Inside);
func(chz, board_SetPosition);
func(chz, board_GetPosition);
func(chz, board_Row);
func(chz, board_Column);
func(chz, board_Or);
func(chz, board_Diagonals);
func(chz, board_OfColor);
func(chz, board_Next);
func(chz, board_Count);
func(chz, board_PawnMoves);
func(chz, board_InState);
func(chz, board_NotInState);
func(chz, board_Left);
func(chz, board_Right);
func(chz, board_Up);
func(chz, board_Down);
func(chz, board_Direction);
func(chz, board_PawnDirection);
func(chz, board_Target);
func(chz, board_PawnAttacks);

func(chz_potential, Default);
func(chz_potential, UnknownBricks);

typedef chz_board_t board_t;
typedef chz_potential_t potential_t;

void test_1(void)
{
	// Shift = NULL;
	
	// Test basic shifting.
	int shift = board_Shift(0, 0);
	assert(shift == 63);
	
	shift = board_Shift(7, 7);
	assert(shift == 0);
	
	shift = board_Shift(0, 7);
	assert(shift == 7);
}

void test_2(void)
{
	int inside = board_Inside(0, 0);
	assert(inside);
	
	inside = board_Inside(-1, 0);
	assert(!inside);
	
	inside = board_Inside(0, -1);
	assert(!inside);
	
	inside = board_Inside(7, 7);
	assert(inside);
	
	inside = board_Inside(8, 8);
	assert(!inside);
}

void test_3(void)
{
	board_t board = {};
	
	board_t newboard = board_SetPosition(board, 7, 7, 1);
	assert(newboard.val == 1);
	
	newboard = board_SetPosition(board, 6, 7, 1);
	assert(newboard.val == 2);
	
	newboard = board_SetPosition(board, 0, 7, 1);
	assert(newboard.val == 128);
	
	newboard = board_SetPosition(board, 0, 0, 1);
	assert(newboard.val == ((uint64_t)1 << 63));
	
	newboard = board_SetPosition(board, -1, 0, 1);
	assert(newboard.val == (uint64_t)0);
	
	newboard = board_SetPosition(board, 0, 0, 1);
	newboard = board_SetPosition(board, 7, 7, 1);
	newboard = board_SetPosition(newboard, 0, 0, 0);
	assert(newboard.val == (uint64_t)1);
}

void test_4(void)
{
	board_t board = {};
	
	board = board_SetPosition(board, 0, 0, 1);
	assert(board_GetPosition(board, 0, 0));
	assert(!board_GetPosition(board, 0, 1));
	assert(!board_GetPosition(board, 1, 0));
	assert(!board_GetPosition(board, -1, 0));
}

void test_5(void)
{
	board_t board = board_Row(7);
	
	assert(board.val == 255ULL);
	
	board = board_Row(6);
	assert(board.val == 0xFF00ULL);
	
	board = board_Or(board_Row(6), board_Row(7));
	assert(board.val == 0xFFFFULL);
}

void test_6(void)
{
	board_t board;
	int i, j;
	for (i = 0; i < 8; i++) {
		board = board_Column(i);
		for (j = 0; j < 8; j++) {
			assert(board_GetPosition(board, i, j));
		}
	}
}

void test_7(void)
{
	board_t board = {};
	board_SetPosition(board, 3, 4, 1);
	
	board_t newboard = board_Diagonals(3, 4);
	assert(board_GetPosition(newboard, 3-1, 4-1));
	assert(board_GetPosition(newboard, 3+1, 4+1));
	assert(board_GetPosition(newboard, 3-1, 4+1));
	assert(board_GetPosition(newboard, 3+1, 4-1));
}

void test_8(void)
{
	board_t board = {};
	
	board = board_SetPosition(board, 0, 0, 1);
	
	// PrintBoard(board);
	
	potential_t potential;
	
	UnknownBricks(board, &potential);
	
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
	potential_t p = {};
	
	Default(&p);
	
	// PrintPotential(&p);
	
	board_t white = board_OfColor(&p, 1);
	
	// PrintBoard(white);
	
	int i;
	for (i = 0; i < 8; i++) {
		assert(board_GetPosition(white, i, 6));
		assert(board_GetPosition(white, i, 7));
	}
	
	board_t black = board_OfColor(&p, 0);
	for (i = 0; i < 8; i++) {
		assert(board_GetPosition(black, i, 0));
		assert(board_GetPosition(black, i, 1));
	}
}

void test_10(void)
{
	board_t board = {};
	board = board_SetPosition(board, 2, 0, 1);
	board = board_SetPosition(board, 2, 1, 1);
	assert(board_Next(board, 0) == 2);
	assert(board_Next(board, 2) == 10);
	assert(board_Next(board, 10) == 64);
}

void test_11(void)
{
	potential_t start = {};
	Default(&start);
	
	// Get white bricks.
	board_t white = board_OfColor(&start, 1);
	// PrintBoard(white);
	
	int count = board_Count(white);
	// printf("%i\n", count);
	assert(count == 16);
	
	// Check that two steps are allowed at start for pawns.
	board_t pawnMoves = board_PawnMoves(0, 6, 1, 1);
	count = board_Count(pawnMoves);
	// printf("%i\n", count);
	assert(count == 2);
	assert(board_GetPosition(pawnMoves, 0, 5));
	assert(board_GetPosition(pawnMoves, 0, 4));
	
	// Check that two steps are not allowed after start.
	pawnMoves = board_PawnMoves(0, 6, 1, 0);
	count = board_Count(pawnMoves);
	assert(count == 1);
	assert(board_GetPosition(pawnMoves, 0, 5));
	assert(!board_GetPosition(pawnMoves, 0, 4));
	
	// Check that black is setup right.
	board_t black = board_OfColor(&start, 0);
	count = board_Count(black);
	assert(count == 16);
	
	// Check all white pawns.
	board_t whitePawns = board_InState(&start, chz_state_Pawn(1));
	count = board_Count(whitePawns);
	assert(count == 8);
	assert(board_GetPosition(whitePawns, 0, 6));
	
	// Check all black pawns.
	board_t blackPawns = board_InState(&start, chz_state_Pawn(0));
	count = board_Count(blackPawns);
	assert(count == 8);
	assert(board_GetPosition(blackPawns, 0, 1));
	
	// Check all bricks.
	board_t bricks = board_NotInState(&start, chz_state_Empty());
	count = board_Count(bricks);
	assert(count == 32);
}

void test_12(void)
{
	board_t board = {};
	
	board = board_SetPosition(board, 4, 4, 1);
	int left = board_Left(board, 4, 4);
	assert(left == -1);
	
	board = board_SetPosition(board, 0, 4, 1);
	left = board_Left(board, 4, 4);
	// printf("%i\n", left);
	assert(left == 0);
	
	int right = board_Right(board, 4, 4);
	assert(right == -1);
	
	board = board_SetPosition(board, 5, 4, 1);
	right = board_Right(board, 4, 4);
	assert(right == 5);
	
	int up = board_Up(board, 4, 4);
	assert(up == -1);
	
	board = board_SetPosition(board, 4, 2, 1);
	up = board_Up(board, 4, 4);
	assert(up == 2);
	
	int down = board_Down(board, 4, 4);
	assert(down == -1);
	
	board = board_SetPosition(board, 4, 7, 1);
	down = board_Down(board, 4, 4);
	assert(down == 7);
	
	right = board_Right(board, 4, 4);
	assert(right == 5);
	
	left = board_Left(board, 4, 4);
	assert(left == 0);
	
	up = board_Up(board, 4, 4);
	assert(up == 2);
}

void test_13(void)
{
	board_t board = {};
	
	board = board_SetPosition(board, 0, 0, 1);
	int s = board_Direction(board, 0, 0, 1, 1);
	assert(s == -1);
	
	board = board_SetPosition(board, 7, 7, 1);
	s = board_Direction(board, 0, 0, 1, 1);
	assert(s == 63);
}

void test_14(void)
{
	assert(chz_state_CanTake(chz_state_Pawn(0), chz_state_Queen(1)));
	assert(!chz_state_CanTake(chz_state_Pawn(0), chz_state_Queen(0)));
	
	assert(board_PawnDirection(0) == 1);
	assert(board_PawnDirection(1) == -1);
	
	// Check target.
	potential_t potential;
	Default(&potential);
	
	board_t board = board_Target(&potential, chz_state_Queen(1), 3, 1);
	assert(board_Count(board) == 1);
	assert(board_GetPosition(board, 3, 1));
	
	// Make sure pawn can not attack anybody at start.
	board_t pawnAttacks = board_PawnAttacks(&potential, 1, 6);
	assert(board_Count(pawnAttacks) == 0);
	
	potential.arr[0 + 8*5] = chz_state_Pawn(0);
	pawnAttacks = board_PawnAttacks(&potential, 1, 6);
	assert(board_Count(pawnAttacks) == 1);
	
	potential.arr[2 + 8*5] = chz_state_Pawn(1);
	pawnAttacks = board_PawnAttacks(&potential, 1, 6);
	assert(board_Count(pawnAttacks) == 1);
	
	potential.arr[2 + 8*5] = chz_state_Pawn(0);
	pawnAttacks = board_PawnAttacks(&potential, 1, 6);
	assert(board_Count(pawnAttacks) == 2);
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
