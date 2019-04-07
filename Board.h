#pragma once
#include <algorithm>//for std::sort
#include <vector>

class Board
{
private:
	static const int boardWidth = 40;
	static const int boardHeight = 40;
	void sortConsidered();//sorts considered cells;

	struct Cell {
		int x, y;//cells hold their own locations cause its easier on me
		bool start = false;
		bool obstacle = false;
		bool end = false;
		bool visited = false;
		bool considered = false;
		bool Path = false;
		int distToBeg = 99999;
		int distToEnd = 99999;
		int totVal;
		Cell *owner = 0;
	};
	int visit(Cell cell);
	Cell *start = 0;
	Cell *end = 0;
	std::vector<Cell> visited;
	std::vector<Cell> considered;
	int cellDistToEnd(Cell a);
	void resetPath();
	Cell cellGrid[boardWidth][boardHeight];

public:
	Board();
	int getWidth();
	int getHeight();
	bool isStart(int x, int y);
	bool isObstacle(int x, int y);
	bool isEnd(int x, int y);
	bool isConsidered(int x, int y);
	bool isVisited(int x, int y);
	bool isPath(int x, int y);
	void setObstacle(int x, int y);
	void setStart(int x, int y);
	void setEnd(int x, int y);
	void removeObstacle(int x, int y);
	void findPath(bool needed);
	~Board();
};

