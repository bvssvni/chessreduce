
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "chess-reduce.h"

unsigned char chz_state_Empty()
{
	return 1;
}

unsigned char chz_state_Pawn(int color)
{
	return 2 | (color << 7);
}

unsigned char chz_state_Rook(int color)
{
	return 4 | (color << 7);
}

unsigned char chz_state_Knight(int color)
{
	return 8 | (color << 7);
}

unsigned char chz_state_Bishop(int color)
{
	return 16 | (color << 7);
}

unsigned char chz_state_Queen(int color)
{
	return 32 | (color << 7);
}

unsigned char chz_state_King(int color)
{
	return 64 | (color << 7);
}

void chz_potential_Default(chz_potential_t *potential)
{
	int i;
	for (i = 0; i < 8; i++) {
		potential->arr[i + 8*1] = chz_state_Pawn(0);
		potential->arr[i + 8*6] = chz_state_Pawn(1);
		
		// Empty locations.
		potential->arr[i + 8*2] = chz_state_Empty();
		potential->arr[i + 8*3] = chz_state_Empty();
		potential->arr[i + 8*4] = chz_state_Empty();
		potential->arr[i + 8*5] = chz_state_Empty();
	}
	
	potential->arr[0 + 8*0] = chz_state_Rook(0);
	potential->arr[0 + 8*7] = chz_state_Rook(1);
	
	potential->arr[1 + 8*0] = chz_state_Knight(0);
	potential->arr[1 + 8*7] = chz_state_Knight(1);
	
	potential->arr[2 + 8*0] = chz_state_Bishop(0);
	potential->arr[2 + 8*7] = chz_state_Bishop(1);
	
	potential->arr[3 + 8*0] = chz_state_King(0);
	potential->arr[3 + 8*7] = chz_state_King(1);
	
	potential->arr[4 + 8*0] = chz_state_Queen(0);
	potential->arr[4 + 8*7] = chz_state_Queen(1);
	
	potential->arr[5 + 8*0] = chz_state_Bishop(0);
	potential->arr[5 + 8*7] = chz_state_Bishop(1);
	
	potential->arr[6 + 8*0] = chz_state_Knight(0);
	potential->arr[6 + 8*7] = chz_state_Knight(1);
	
	potential->arr[7 + 8*0] = chz_state_Rook(0);
	potential->arr[7 + 8*7] = chz_state_Rook(1);
}

int chz_board_Inside(int i, int j)
{
	return i >= 0 && i < 8 && j >= 0 && j < 8;
}

int chz_board_Shift(int i, int j)
{
	return 63 - i - j * 8;
}


chz_board_t chz_board_SetPosition(chz_board_t b, int i, int j, int val)
{
	// Skip coordinats that are outside.
	if (!chz_board_Inside(i, j)) return b;
	
	// Put in positive bit.
	if (val) return (chz_board_t){b.val | (uint64_t)1 << chz_board_Shift(i, j)};
	
	// Remove bit.
	return (chz_board_t){b.val & ~((uint64_t)1 << chz_board_Shift(i, j))};
}

int chz_board_GetPosition(chz_board_t board, int i, int j)
{
	if (!chz_board_Inside(i, j)) return 0;
	
	return (board.val >> chz_board_Shift(i, j)) & 1;
}

chz_board_t chz_board_Row(int i)
{
	return (chz_board_t){255L << (7*8 - i * 8)};
}

chz_board_t chz_board_Column(int i)
{
	switch (i) {
		case 0: {return (chz_board_t){0x8080808080808080ULL};}
		case 1: {return (chz_board_t){0x4040404040404040ULL};}
		case 2: {return (chz_board_t){0x2020202020202020ULL};}
		case 3: {return (chz_board_t){0x1010101010101010ULL};}
		case 4: {return (chz_board_t){0x0808080808080808ULL};}
		case 5: {return (chz_board_t){0x0404040404040404ULL};}
		case 6: {return (chz_board_t){0x0202020202020202ULL};}
		case 7: {return (chz_board_t){0x0101010101010101ULL};}
	}
	return (chz_board_t){};
}

//
// Returns top left -> right bottom diagonal.
//
// 100
// 010
// 001
//
int shiftDiagonalA(int i, int j)
{
	// 0,0 -> 7
	// 1,1 -> 7
	// 2,2 -> 7
	
	// 1,0 -> 8
	// 2,0 -> 8
	// 3,0 -> 8
	
	return (i-j)+7;
}

//
// Returns top right -> left bottom diagonal.
//
// 001
// 010
// 100
//
int shiftDiagonalB(int i, int j)
{
	// 0,0 -> 14
	// 1,1 -> 13
	// 2,2 -> 12
	
	// 1,0 -> 13
	// 0,1 -> 13
	
	// 2,0 -> 12
	// 1,1 -> 12
	// 1,2 -> 12
	
	return 14 - (i + j);
}

chz_board_t chz_board_Diagonals(int i, int j)
{
	// Nedd a formula that calculates
	
	int k = shiftDiagonalA(i, j);
	uint64_t diagA = 0;
	
	// Diagonals are moving from lower corners to up.
	
	switch (k) {
		case 0: diagA = 0x0000000000000080ULL; break;
		case 1: diagA = 0x0000000000008040ULL; break;
		case 2: diagA = 0x0000000000804020ULL; break;
		case 3: diagA = 0x0000000080402010ULL; break;
		case 4: diagA = 0x0000008040201008ULL; break;
		case 5: diagA = 0x0000804020100804ULL; break;
		case 6: diagA = 0x0080402010080402ULL; break;
		case 7: diagA = 0x8040201008040201ULL; break;
		case 8: diagA = 0x4020100804020100ULL; break;
		case 9: diagA = 0x2010080402010000ULL; break;
		case 10: diagA = 0x1008040201000000ULL; break;
		case 11: diagA = 0x0804020100000000ULL; break;
		case 12: diagA = 0x0402010000000000ULL; break;
		case 13: diagA = 0x0201000000000000ULL; break;
		case 14: diagA = 0x0100000000000000ULL; break;
	}
	
	k = shiftDiagonalB(i, j);
	uint64_t diagB = 0;
	
	switch (k) {
		case 0: diagB = 0x0000000000000001ULL; break;
		case 1: diagB = 0x0000000000000102ULL; break;
		case 2: diagB = 0x0000000000010204ULL; break;
		case 3: diagB = 0x0000000001020408ULL; break;
		case 4: diagB = 0x0000000102040810ULL; break;
		case 5: diagB = 0x0000010204081020ULL; break;
		case 6: diagB = 0x0001020408102040ULL; break;
		case 7: diagB = 0x0102040810204080ULL; break;
		case 8: diagB = 0x0204081020408000ULL; break;
		case 9: diagB = 0x0408102040800000ULL; break;
		case 10: diagB = 0x0810204080000000ULL; break;
		case 11: diagB = 0x1020408000000000ULL; break;
		case 12: diagB = 0x2040800000000000ULL; break;
		case 13: diagB = 0x4080000000000000ULL; break;
		case 14: diagB = 0x8000000000000000ULL; break;
	}
	
	return (chz_board_t){diagA | diagB};
}


chz_board_t chz_board_KnightMoves(int i, int j)
{
	if (!chz_board_Inside(i, j)) return (chz_board_t){};
	
	static uint64_t knights[64];
	static int knights_set = 0;
	
	// Preprocess knight targets.
	if (!knights_set) {
		knights_set = 1;
		int i;
		for (i = 0; i < 64; i++) {
			chz_board_t board = {};
			
			chz_board_SetPosition(board, i+1, j-2, 1);
			chz_board_SetPosition(board, i+2, j-1, 1);
			chz_board_SetPosition(board, i+2, j+1, 1);
			chz_board_SetPosition(board, i+1, j+2, 1);
			chz_board_SetPosition(board, i-1, j+2, 1);
			chz_board_SetPosition(board, i-2, j+1, 1);
			chz_board_SetPosition(board, i-2, j-1, 1);
			chz_board_SetPosition(board, i-1, j-2, 1);
			
			knights[i] = board.val;
		}
	}
	
	return (chz_board_t){knights[i+j*8]};
}

chz_board_t chz_board_Or(chz_board_t a, chz_board_t b)
{
	return (chz_board_t){a.val | b.val};
}

chz_board_t chz_board_And(chz_board_t a, chz_board_t b)
{
	return (chz_board_t){a.val & b.val};
}

chz_board_t chz_board_Except(chz_board_t a, chz_board_t b)
{
	return (chz_board_t){a.val & ~b.val};
}

void chz_potential_UnknownBricks(chz_board_t board, chz_potential_t *res)
{
	chz_board_t a, b, c;
	int i, j, addr;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			addr = i+j*8;
			
			res->arr[addr] = 0;
			
			// Positions with brick can be any brick.
			if (chz_board_GetPosition(board, i, j)) {
				res->arr[addr] = ~1;
				continue;
			}
			
			// Check horizontally and vertically.
			a = chz_board_Column(i);
			b = chz_board_Row(j);
			c = chz_board_And(board, chz_board_Or(a, b));
			if (c.val) {
				res->arr[addr] |= (1<<2) | (1<<5);
			}
			
			// Check for bishop or queen.
			a = chz_board_Diagonals(i, j);
			b = chz_board_And(board, a);
			if (b.val) {
				res->arr[addr] |= (1<<4) | (1<<5);
			}
			
			// Check for knight.
			a = chz_board_KnightMoves(i, j);
			b = chz_board_And(board, a);
			if (b.val) {
				res->arr[addr] |= (1<<3);
			}
			
			// Pawn or king.
			if (chz_board_GetPosition(board, i, j-1) ||
				chz_board_GetPosition(board, i, j+1)) {
				res->arr[addr] |= 1 | (1<<6);
			}
			
			// Pawn on diagonal attack.
			if (chz_board_GetPosition(board, i-1, j-1) ||
				chz_board_GetPosition(board, i+1, j-1) ||
				chz_board_GetPosition(board, i-1, j+1) ||
				chz_board_GetPosition(board, i+1, j+1)) {
				res->arr[addr] |= 1;
			}
			
			// King.
			if (chz_board_GetPosition(board, i-1, j) ||
				chz_board_GetPosition(board, i+1, j)) {
				res->arr[addr] |= (1<<6);
			}
			
		}
	}
}

chz_board_t chz_board_OfColor(chz_potential_t *potential, int color)
{
	chz_board_t board = {};
	
	int i;
	int brickColor;
	int non_empty;
	int sameColor;
	unsigned char state;
	for (i = 0; i < 64; i++) {
		state = potential->arr[i];
		brickColor = (state >> 7) & 1;
		non_empty = state != 1;
		sameColor = brickColor == color;
		if (sameColor && non_empty) {
			board.val |= 1ULL << (63-i);
		}
	}
	return board;
}

int chz_board_Next(chz_board_t board, int prev)
{
	int i;
	for (i = prev + 1; i < 64; i++) {
		if ((board.val >> (63-i)) & 1) {
			return i;
		}
	}
	
	return 64;
}

int chz_board_Count(chz_board_t board)
{
	int i;
	int sum = 0;
	for (i = 0; i < 64; i++) {
		if ((board.val >> (63-i)) & 1) sum++;
	}
	
	return sum;
}

int chz_board_PawnDirection(int color)
{
	return color ? -1 : 1;
}

chz_board_t chz_board_PawnMoves(int i, int j, int color, int start)
{
	// White is up, black is down.
	int dir = chz_board_PawnDirection(color);
	
	chz_board_t board = {};
	
	// Moving one step forward.
	board = chz_board_SetPosition(board, i, j+dir, 1);
	
	// At first move, we can move two steps forward.
	if (start) board = chz_board_SetPosition(board, i, j+2*dir, 1);
	
	return board;
}

chz_board_t chz_board_Target
(const chz_potential_t *potential, int state, int i, int j)
{
	if (!chz_board_Inside(i, j)) return (chz_board_t){};
	
	int targetState = potential->arr[i + 8*j];
	int empty = targetState & 1;
	
	if (empty) return (chz_board_t){};
	if (!chz_state_CanTake(state, targetState)) return (chz_board_t){};
	
	return (chz_board_t){.val = 1ULL << chz_board_Shift(i, j)};
}

chz_board_t chz_board_PawnAttacks
(const chz_potential_t *potential, int i, int j)
{
	int state = potential->arr[i+8*j];
	int color = (state >> 7) & 1;
	int dir = chz_board_PawnDirection(color);
	
	chz_board_t board = chz_board_Target(potential, state, i-1, j+dir);
	
	board = chz_board_Or(board, chz_board_Target(potential, state, i+1, j+dir));
	
	return board;
}

chz_board_t chz_board_InState(const chz_potential_t *potential, int state)
{
	chz_board_t board = {};
	
	int i;
	for (i = 0; i < 64; i++) {
		if (potential->arr[i] == state) {
			board.val |= (1ULL << (63-i));
		}
	}
	
	return board;
}

chz_board_t chz_board_NotInState(const chz_potential_t *potential, int state)
{
	chz_board_t board = {};
	
	int i;
	for (i = 0; i < 64; i++) {
		if (potential->arr[i] != state) {
			board.val |= (1ULL << (63-i));
		}
	}
	
	return board;
}

int chz_board_Left(chz_board_t board, int i, int j)
{
	int k;
	for (k = i-1; k >= 0; k--) {
		if (chz_board_GetPosition(board, k, j)) return k;
	}
	
	return -1;
}

int chz_board_Right(chz_board_t board, int i, int j)
{
	int k;
	for (k = i+1; k < 8; k++) {
		if (chz_board_GetPosition(board, k, j)) return k;
	}
	
	return -1;
}

int chz_board_Up(chz_board_t board, int i, int j)
{
	int k;
	for (k = j-1; k >= 0; k--) {
		if (chz_board_GetPosition(board, i, k)) return k;
	}
	
	return -1;
}

int chz_board_Down(chz_board_t board, int i, int j)
{
	int k;
	for (k = j+1; k < 8; k++) {
		if (chz_board_GetPosition(board, i, k)) return k;
	}
	
	return -1;
}

int chz_board_Direction
(chz_board_t board, int i, int j, int di, int dj)
{
	int k, m;
	for (k = i+di, m = j+dj; chz_board_Inside(k, m); k += di, m += dj) {
		if (chz_board_GetPosition(board, k, m)) return k + 8*m;
	}
	
	return -1;
}

int chz_state_CanTake(int source, int target)
{
	int colorSource = (source >> 7) & 1;
	int targetSource = (target >> 7) & 1;
	
	if (colorSource == targetSource) return 0;
	
	return 1;
}

void chz_board_PrintBoard(chz_board_t board)
{
	int i, j;
	for (j = 0; j < 8; j++) {
		for (i = 0; i < 8; i++) {
			if (chz_board_GetPosition(board, i, j)) {
				printf("x");
			} else {
				printf("_");
			}
		}
		printf("\n");
	}
	printf("\n");
}

void chz_potential_PrintPotential(const chz_potential_t *potential)
{
	int i, j;
	for (j = 0; j < 8; j++) {
		for (i = 0; i < 8; i++) {
			printf("%2x ", potential->arr[i+8*j]);
		}
		printf("\n");
	}
	printf("\n");
}

