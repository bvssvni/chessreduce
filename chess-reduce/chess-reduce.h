
/*
 
 Chess potential solver.
 
 THEORY
 
 Given N bits, there are 2^N possible states.
 A specific description of possibilities is called a 'potential'.
 
 There can only be one brick at one position at one time.
 
 The potential for one location is the possible brick types.
 For the whole board, this can be described as 64 bytes.
 
 STATE			BIT
 empty			0
 pawn			1
 rook			2
 knight			3
 bishop			4
 queen			5
 king			6
 color			7
 
 The potential for one brick is the possible locations.
 
 64 bits, each for one location on board.
 Here we have the potential for a knight at location marked 'x'.
 
 0 0 1 0 1 0 0 0
 0 1 0 0 0 1 0 0
 0 0 0 x 0 0 0 0
 0 1 0 0 0 1 0 0
 0 0 1 0 1 0 0 0
 0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0
 
 */

//
// Contains 1 bit for every positin on the chess board.
// This is used to make decisions and look for legal moves.
//
typedef struct chz_board
{
	uint64_t val;
} chz_board_t;

//
// Contains 64 bytes, for all possible states of a chess board.
//
typedef struct chz_potential
{
	unsigned char arr[64];
} chz_potential_t;

//
// Returns a empty state of a location.
//
unsigned char chz_state_Empty();

//
// Returns a pawn state of location.
//
unsigned char chz_state_Pawn(int color);

//
// Returns a rook state of location.
//
unsigned char chz_state_Rook(int color);

//
// Returns a knight state of location.
//
unsigned char chz_state_Knight(int color);

//
// Returns a bishop state of location.
//
unsigned char chz_state_Bishop(int color);

//
// Returns a queen state of location.
//
unsigned char chz_state_Queen(int color);

//
// Returns a king state of location.
//
unsigned char chz_state_King(int color);

//
// Returns initial game setup.
//
void chz_potential_Default(chz_potential_t *potential);

//
// Returns true if coordinates are inside board coordinates.
//
int chz_board_Inside(int i, int j);

//
// Returns the number of bits to shift to get position.
//
int chz_board_Shift(int i, int j);

//
// Returns a new board with the position set to true.
//
chz_board_t chz_board_SetPosition(chz_board_t board, int i, int j, int val);

//
// Returns true if the position is occupied.
//
int chz_board_GetPosition(chz_board_t board, int i, int j);

//
// Returns places by row.
//
chz_board_t chz_board_Row(int i);

//
// Returns places by column.
//
chz_board_t chz_board_Column(int i);

//
// Returns the diagonals at a given location.
//
chz_board_t chz_board_Diagonals(int i, int j);

//
// Returns the knight target locations by position.
//
chz_board_t chz_board_KnightMoves(int i, int j);

//
// Combines two boards with Boolean Or.
//
chz_board_t chz_board_Or(chz_board_t a, chz_board_t b);

//
// Combines two boards with Boolean And.
//
chz_board_t chz_board_And(chz_board_t a, chz_board_t b);

//
// Combines two boards with Boolean Except.
//
chz_board_t chz_board_Except(chz_board_t a, chz_board_t b);

//
// Returns the possible moves given unknown bricks.
//
void chz_potential_UnknownBricks(chz_board_t board, chz_potential_t *res);

//
// Returns a board with bricks of a color.
//
chz_board_t chz_board_OfColor(chz_potential_t *potential, int color);

//
// Returns the next non-zero position on board.
//
// for (i = 0; i < 64; i = chz_board_Next(board, i)) {
//  ...
// }
//
int chz_board_Next(chz_board_t board, int prev);

//
// Returns the number of occupied positions on board.
//
int chz_board_Count(chz_board_t board);

//
// Returns the direction for a pawn by color.
//
int chz_board_PawnDirection(int color);

//
// Returns the moves for a pawn at position.
// 2 steps are allowed at first move by setting start to true.
// This does not include attacking targets.
//
chz_board_t chz_board_PawnMoves(int i, int j, int color, int start);

//
// Returns the board position including the target if the bricks are
// not of same colors.
//
chz_board_t chz_board_Target
(const chz_potential_t *potential, int state, int i, int j);

//
// Returns possible pawn attacks for a pawn at specified location.
//
chz_board_t chz_board_PawnAttacks
(const chz_potential_t *potential, int i, int j);

//
// Returns the positions that has a certain state.
//
chz_board_t chz_board_InState(const chz_potential_t *potential, int state);

//
// Returns the positions are that not in a certain state.
//
chz_board_t chz_board_NotInState(const chz_potential_t *potential, int state);

//
// Returns the column of the closest active position.
// If there are none bricks to left, it returns -1.
//
int chz_board_Left(chz_board_t board, int i, int j);

//
// Returns the column of the closest active position.
// If the are none bricks to the right, it returns -1.
//
int chz_board_Right(chz_board_t board, int i, int j);

//
// Returns the column of the closest active position.
// If the are none bricks in upward direction, it returns -1.
//
int chz_board_Up(chz_board_t board, int i, int j);

//
// Returns the column of the closest active position.
// If the are none bricks in downward direction, it returns -1.
//
int chz_board_Down(chz_board_t board, int i, int j);

//
// Returns the bit index of closest active position in a direction.
// Evalues each di and dj step.
//
int chz_board_Direction
(chz_board_t board, int i, int j, int di, int dj);

//
// Returns true if the source state can take target state.
// If they are of same colors, the target cannot be attacked.
//
int chz_state_CanTake(int source, int target);

//
// Prints a board to standard output.
//
void chz_board_PrintBoard(chz_board_t board);

//
// Prints potential to standard output.
//
void chz_potential_PrintPotential(const chz_potential_t *potential);


