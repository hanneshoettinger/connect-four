#include "ScoreMiniMax.h"

ScoreMiniMax::ScoreMiniMax()
{
}


ScoreMiniMax::~ScoreMiniMax()
{
}

void ScoreMiniMax::abNegamax(Cells color, int depth, Board& board, int& move, int& score, int a, int b)
{
	int bestScore = a;	
	int bestMove = -1;

	for (int column = 0; column < width; column++)
	{
		if (board._slots[0][column] != Empty) continue;
		int rowFilled = dropDisk(board, column, color);
		if (rowFilled == -1)
			continue;

		int s = ScoreBoard(board);

		if (color == Red && s == redWins) {
			bestMove = column;
			bestScore = s;
			board._slots[rowFilled][column] = Empty;
			break;
		}

		int moveInner, scoreInner;
		if (depth > 1) {
			abNegamax(color == Red ? Blue : Red, depth - 1, board, moveInner, scoreInner, -b, -bestScore);
		}
		else {
			moveInner = -1;
			scoreInner = s;
		}

		board._slots[rowFilled][column] = Empty;

		if (depth == g_maxDepth && g_debug) {
			printf("Depth %d, placing on %d, score:%d\n", depth, column, scoreInner);
		}

		if (scoreInner > bestScore) 
		{
			bestScore = scoreInner;

			if (bestScore >= b)
			{
				break; // pruning
			}
			// write best move on top of tree
			if (depth == g_maxDepth)
			{
				bestMove = column;
			}
		}
	}

	move = bestMove;
	score = bestScore * -1;
}

void ScoreMiniMax::Negamax(Cells color, int depth, Board& board, int& move, int& score)
{
	int bestScore = blueWins;
	int bestMove = -1;
	for (int column = 0; column < width; column++) 
	{
		if (board._slots[0][column] != Empty) continue;
		int rowFilled = dropDisk(board, column, color);
		if (rowFilled == -1)
			continue;

		int s = ScoreBoard(board);

		if (color == Red && s == redWins) {
			bestMove = column;
			bestScore = s;
			board._slots[rowFilled][column] = Empty;
			break;
		}

		int moveInner, scoreInner;
		if (depth > 1) {
			Negamax(color == Red ? Blue : Red, depth - 1, board, moveInner, scoreInner);
		}
		else {
			moveInner = -1;
			scoreInner = s;
		}

		board._slots[rowFilled][column] = Empty;

		if (depth == g_maxDepth && g_debug) {
			printf("Depth %d, placing on %d, score:%d\n", depth, column, scoreInner);
		}

		if (scoreInner > bestScore) {
			bestScore = scoreInner;
			if (depth == g_maxDepth)
			{
				bestMove = column;
			}
		}

	}

	move = bestMove;
	score = bestScore * -1;
}

void ScoreMiniMax::Minimax(bool maximizeOrMinimize, Cells color, int depth, Board& board, int& move, int& score)
{
	int bestScore = maximizeOrMinimize ? blueWins : redWins;
	int bestMove = -1;
	for (int column = 0; column < width; column++) 
	{
		if (board._slots[0][column] != Empty) continue;
		int rowFilled = dropDisk(board, column, color);
		if (rowFilled == -1)
			continue;
		int s = ScoreBoard(board);
		if (s == (maximizeOrMinimize ? redWins : blueWins)) {
			bestMove = column;
			bestScore = s;
			board._slots[rowFilled][column] = Empty;
			break;
		}

		int moveInner, scoreInner;
		if (depth > 1)
			 Minimax(!maximizeOrMinimize, color == Red ? Blue : Red, depth - 1, board, moveInner, scoreInner);
		else {
			moveInner = -1;
			scoreInner = s;
		}

		board._slots[rowFilled][column] = Empty;

		if (depth == g_maxDepth && g_debug) {
			printf("Depth %d, placing on %d, score:%d\n", depth, column, scoreInner);
		}
		if (maximizeOrMinimize) {
			if (scoreInner >= bestScore) {
				bestScore = scoreInner;
				bestMove = column;
			}
		}
		else {
			if (scoreInner <= bestScore) {
				bestScore = scoreInner;
				bestMove = column;
			}
		}
	}

	move = bestMove;
	score = bestScore;
}

int ScoreMiniMax::ScoreBoard(const Board& board)
{
	int counters[9] = { 0,0,0,0,0,0,0,0,0 };

	typedef Cells EnumsAreIntegersInC[height][width];
	const EnumsAreIntegersInC& scores = board._slots;

	// Horizontal spans
	for (int y = 0; y < height; y++) {
		int score = scores[y][0] + scores[y][1] + scores[y][2];
		for (int x = 3; x < width; x++) {
			assert(inside(y, x));
			score += scores[y][x];
			counters[score + 4]++;
			assert(inside(y, x - 3));
			score -= scores[y][x - 3];
		}
	}
	// Vertical spans
	for (int x = 0; x < width; x++) {
		int score = scores[0][x] + scores[1][x] + scores[2][x];
		for (int y = 3; y < height; y++) {
			assert(inside(y, x));
			score += scores[y][x];
			counters[score + 4]++;
			assert(inside(y - 3, x));
			score -= scores[y - 3][x];
		}
	}
	// Down-right (and up-left) diagonals
	for (int y = 0; y < height - 3; y++) {
		for (int x = 0; x < width - 3; x++) {
			int score = 0;
			for (int idx = 0; idx < 4; idx++) {
				int yy = y + idx;
				int xx = x + idx;
				assert(inside(yy, xx));
				score += scores[yy][xx];
			}
			counters[score + 4]++;
		}
	}
	// up-right (and down-left) diagonals
	for (int y = 3; y < height; y++) {
		for (int x = 0; x < width - 3; x++) {
			int score = 0;
			for (int idx = 0; idx < 4; idx++) {
				int yy = y - idx;
				int xx = x + idx;
				assert(inside(yy, xx));
				score += scores[yy][xx];
			}
			counters[score + 4]++;
		}
	}
	if (counters[0] != 0)
		return blueWins;
	else if (counters[8] != 0)
		return redWins;
	else // heuristic function
		return
		counters[5] + 2 * counters[6] + 5 * counters[7] -
		counters[3] - 2 * counters[2] - 5 * counters[1];
}

int ScoreMiniMax::nextMove(Board& board, bool& playerswitch, int mymove)
{
	int scoreOrig = ScoreBoard(board);
	if (scoreOrig == redWins) { system("cls"); printBoard(board); std::cout << "Computer wins!\nGAME OVER\n"; return 1; }
	else if (scoreOrig == blueWins) { system("cls"); printBoard(board); std::cout << "You win!!!!!\n"; return 2; }
	else if (playerswitch == true) {

		int move, score;

		if		(g_selection == 1)	{ Minimax(true, Red, g_maxDepth, board, move, score); }
		else if (g_selection == 2)	{ Negamax(Red, g_maxDepth, board, move, score); }
		else if (g_selection == 3)	{ abNegamax(Red, g_maxDepth, board, move, score, blueWins*10, redWins*10); }

		if (move != -1) {
			std::cout << "Computer chose: " << move << std::endl;
			dropDisk(board, move, Red);
			scoreOrig = ScoreBoard(board);
			if (scoreOrig == redWins) { system("cls"); printBoard(board); std::cout << "Computer wins!\nGAME OVER\n"; return 1; }
			else if (scoreOrig == blueWins) { system("cls"); printBoard(board); std::cout << "You win!!!!!\n"; return 2; }
			else { playerswitch = !playerswitch; };
		}
		else {
			std::cout << "No move possible";
			exit(-1);
		}
	}
	else
	{
		std::cout << "Choose your Move (0-6): --> ";
		int check = dropMyDisk(board, mymove, Blue);
		if (check == -1)
			return -1;//nextMove(board, playerswitch);
		playerswitch = !playerswitch;
	}
	system("cls");
	printBoard(board);
}

bool ScoreMiniMax::inside(int y, int x)
{
	return y >= 0 && y < height && x >= 0 && x < width;
}

int ScoreMiniMax::dropDisk(Board& board, int column, Cells color)
{
	for (int y = height - 1; y >= 0; y--)
		if (board._slots[y][column] == Empty) {
			board._slots[y][column] = color;
			return y;
		}
	return -1;
}

int ScoreMiniMax::dropMyDisk(Board& board, int column, Cells color)
{
	if (board._slots[0][column] != Empty)
	{
		std::cout << "It is full!" << std::endl;
		return -1;
	}
	else if (0 <= column && column <= 6)
	{
		for (int row = height - 1; row >= -1; row--)
		{
			if (row == 5)
			{
				if (board._slots[row][column] == Empty)
				{
					board._slots[row][column] = color;
					break;
				}
			}
			else if (board._slots[row + 1][column] != Empty && board._slots[row][column] == Empty)
			{
				board._slots[row][column] = color;
				break;
			}
		}
	}
	else
	{
		std::cout << "Invalid Move!" << std::endl;
		return -1;
	}
}

// Console Color Function
char Color(int color = 7, char Message = ' ') {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	return Message;
}

void ScoreMiniMax::printBoard(Board& board)
{
	for (int y = 0; y < height; y++)
	{
		std::cout << "[";

		for (int x = 0; x < width; x++)
		{
			if (board._slots[y][x] == Red)
				std::cout << Color(12, 'O');
			//std::cout << "X";
			else if (board._slots[y][x] == Blue)
				std::cout << Color(11, 'O');
			//std::cout << "O";
			else
				std::cout << " ";
			if (x != width - 1)
				std::cout << " ";
		}
		Color(7, ' '); //Sets the color to default
		std::cout << "]";
		std::cout << std::endl;
	}
	std::cout << " 0 1 2 3 4 5 6 \n";
	std::cout << std::endl;
}