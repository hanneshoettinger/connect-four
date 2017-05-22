#pragma once
#include "stdafx.h"
#include "global.h"

// Game Colors
enum Cells {
	Red = 1,
	Blue = -1,
	Empty = 0
};

// Board
struct Board {
	Cells _slots[height][width];
};


class ScoreMiniMax
{
public:
	ScoreMiniMax();
	~ScoreMiniMax();

	void abNegamax(Cells color, int depth, Board & board, int & move, int & score, int a, int b);

	int nextMove(Board & board, bool & playerswitch, int mymove); // toggle between players and check for finish -> main function

	void Minimax(bool maximizeOrMinimize, Cells color, int depth, Board & board, int & move, int & score); // MiniMax Algorithm

	void Negamax(Cells color, int depth, Board & board, int & move, int & score);  // NegaMax Algorithm
		
	int ScoreBoard(const Board & board);  // score function for the board with heuristic

	int dropDisk(Board & board, int column, Cells color);	// drop CPU disk
	int dropMyDisk(Board & board, int column, Cells color); // drop Player disk

	bool inside(int y, int x); // assert if inside

	void printBoard(Board & board);			// print board in console window

};
