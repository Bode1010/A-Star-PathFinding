#include "Board.h"
using namespace std;
//when calles it sets all the cells internal x and y values to their grid placement
Board::Board()
{
	for (int i = 0; i < boardHeight; i++) {
		for (int j = 0; j < boardWidth; j++) {
			cellGrid[j][i].x = j;
			cellGrid[j][i].y = i;
		}
	}
}

//returns height of the board in squares
int Board::getHeight() {
	return boardHeight;
}

//returns width of the board in squares
int Board::getWidth() {
	return boardWidth;
}

//returns the start bool of the specified cell
bool Board::isStart(int x, int y) {
	return cellGrid[x][y].start;
}

//returns the obstacle bool of the specified cell
bool Board::isObstacle(int x, int y) {
	return cellGrid[x][y].obstacle;
}


//returs the end bool of the specified cell
bool Board::isEnd(int x, int y) {
	return cellGrid[x][y].end;
}

//returns the considered bool of the specified cell
bool Board::isConsidered(int x, int y) {
	return cellGrid[x][y].considered;
}

//returns the visited bool of the specified cell
bool Board::isVisited(int x, int y) {
	return cellGrid[x][y].visited;
}

//returns the path bool of the specified cell
bool Board::isPath(int x, int y) {
	return cellGrid[x][y].Path;
}

//sets the obstacle bool of the specified cell to true
void Board::setObstacle(int x, int y) {
	for (int l = 0; l < visited.size(); l++) {
		int j = visited[l].x;
		int i = visited[l].y;
		cellGrid[j][i].start = false;
		cellGrid[j][i].visited = false;
		cellGrid[j][i].considered = false;
		cellGrid[j][i].Path = false;
		cellGrid[j][i].distToBeg = 999999;
		if (!cellGrid[j][i].end)
			cellGrid[j][i].distToEnd = 999999;
		cellGrid[j][i].totVal = 999999;
	}
	for (int l = 0; l < considered.size(); l++) {
		int j = considered[l].x;
		int i = considered[l].y;
		cellGrid[j][i].start = false;
		cellGrid[j][i].visited = false;
		cellGrid[j][i].considered = false;
		cellGrid[j][i].Path = false;
		cellGrid[j][i].distToBeg = 999999;
		if (!cellGrid[j][i].end)
			cellGrid[j][i].distToEnd = 999999;
		cellGrid[j][i].totVal = 999999;
	}
	cellGrid[x][y].obstacle = true;
}

//sets the obstacle bool of the specified cell to false
void Board::removeObstacle(int x, int y) {
	cellGrid[x][y].obstacle = false;
}

//sets the start bool of the specified cell to true
void Board::setStart(int x, int y) {
	if (start != 0) {
		start->start = false;
		start->visited = false;
		start->considered = false;
		start->Path = false;
	}
	for (int l = 0; l < visited.size(); l++) {
		int j = visited[l].x;
		int i = visited[l].y;
		cellGrid[j][i].start = false;
		cellGrid[j][i].visited = false;
		cellGrid[j][i].considered = false;
		cellGrid[j][i].Path = false;
		cellGrid[j][i].distToBeg = 999999;
		if (!cellGrid[j][i].end)
			cellGrid[j][i].distToEnd = 999999;
		cellGrid[j][i].totVal = 999999;
	}
	for (int l = 0; l < considered.size(); l++) {
		int j = considered[l].x;
		int i = considered[l].y;
		cellGrid[j][i].start = false;
		cellGrid[j][i].visited = false;
		cellGrid[j][i].considered = false;
		cellGrid[j][i].Path = false;
		cellGrid[j][i].distToBeg = 999999;
		if (!cellGrid[j][i].end)
			cellGrid[j][i].distToEnd = 999999;
		cellGrid[j][i].totVal = 999999;
	}
	
	resetPath();
	cellGrid[x][y].start = true;
	cellGrid[x][y].distToBeg = 0;
	start = &cellGrid[x][y];
}

//sets the end bool of the specified cell to true
void Board::setEnd(int x, int y) {
	if (end != 0) {
		end->end = false;
		end->visited = false;
		end->considered = false;
		end->Path = false;
	}
	end = 0;
	for (int l = 0; l < visited.size(); l++) {
		int j = visited[l].x;
		int i = visited[l].y;
		cellGrid[j][i].end = false;
		cellGrid[j][i].visited = false;
		cellGrid[j][i].considered = false;
		cellGrid[j][i].Path = false;
		if (!cellGrid[j][i].start)
			cellGrid[j][i].distToBeg = 999999;
		cellGrid[j][i].distToEnd = 999999;
		cellGrid[j][i].totVal = 999999;
	}
	for (int l = 0; l < considered.size(); l++) {
		int j = considered[l].x;
		int i = considered[l].y;
		cellGrid[j][i].end = false;
		cellGrid[j][i].visited = false;
		cellGrid[j][i].considered = false;
		cellGrid[j][i].Path = false;
		if (!cellGrid[j][i].start)
			cellGrid[j][i].distToBeg = 999999;
		cellGrid[j][i].distToEnd = 999999;
		cellGrid[j][i].totVal = 999999;
	}
	resetPath();
	cellGrid[x][y].end = true;
	cellGrid[x][y].distToEnd = 0;
	end = &cellGrid[x][y];
}

//sorts the considered cells by their totval
void Board::sortConsidered() {
	std::sort(considered.begin(), considered.end(), [](const Cell &left, const Cell &right) {return (left.totVal < right.totVal); });
}

//loops througgh the best cells and visits each one
void Board::findPath(bool needed) {
	if (needed) {
		if (start != 0 && end != 0) {
			start->distToEnd = cellDistToEnd(*start);
			int endFound = 1;
			if(start->x < boardWidth && start->x >= 0 && start->y < boardHeight && start->y >= 0)
			endFound = visit(*start);

			while (!considered.empty() && endFound != 1) {
				endFound = visit(considered[0]);
			}
			if (endFound == 1) {
				//std::cout << "Path found" << std::endl;
				Cell *curNode = end;
				while (curNode != start && curNode != 0) {
					curNode->Path = true;
					curNode = curNode->owner;
				}
			}
		}
		else
			//std::cout << "There is either no start or end" << std::endl;
			return;
	}
}

//returns the distance between the specified cell and the end cell if any
int Board::cellDistToEnd(Cell a) {
	return sqrt((abs(a.x - end->x)) * (abs(a.x - end->x)) + (abs(a.y- end->y)) * (abs(a.y - end->y))) * 10;
}

//removes the cell from the considered list, adds it to the visited list, sets it visited bool to true
//checks all the cell around it and adds the cells that qualify by the rule of A* propagation to the 
//considered list, return 1 if the end is found
int Board::visit(Cell cell) {
	visited.push_back(cell);
	Cell *VisitedCell = &cellGrid[cell.x][cell.y];
	VisitedCell->visited = true;
	if (!considered.empty()) {
		considered.erase(considered.begin());
	}
	int invalidCounter = 0;
	int diagDist = 14;
	int vertDist = 10;

	//goes to each cell around the selected and updates their value if not visited 
	//or dist to beg( dist from next cell to this + dist from this cell to beg) < former
	//or if it is not a obstacle. if there are 8 invalid cells and considered is empty return 2
	//meaning there is no solution. if it returns 1 it is still seraching and if it returns 0 it found it
	//also pushes updated cells into considered

	//make sure it doest wrap around l8r
	//top left
	if ((cell.x-1 >=0 && cell.y-1 >= 0) &&!cellGrid[cell.x - 1][cell.y - 1].visited && cellGrid[cell.x - 1][cell.y - 1].obstacle == false && ((cell.distToBeg + diagDist) < (cellGrid[cell.x - 1][cell.y - 1].distToBeg))) {
		//update the Cell
		Cell *thisCell = &cellGrid[cell.x - 1][cell.y - 1];
		thisCell->considered = true;
		thisCell->owner = &cellGrid[cell.x][cell.y];
		thisCell->distToBeg = cell.distToBeg + diagDist;
		thisCell->distToEnd = cellDistToEnd(*thisCell);
		thisCell->totVal = thisCell->distToBeg + thisCell->distToEnd;
		considered.push_back(*thisCell);
		if (thisCell->end)
			return 1;
	}
	
	//top mid
	if ((cell.y - 1 >= 0) && cellGrid[cell.x][cell.y - 1].visited == false && cellGrid[cell.x][cell.y - 1].obstacle == false && ((cell.distToBeg + diagDist) < (cellGrid[cell.x][cell.y - 1].distToBeg))) {
		//update the Cell
		Cell *thisCell = &cellGrid[cell.x][cell.y - 1];
		thisCell->considered = true;
		thisCell->owner = &cellGrid[cell.x][cell.y];
		thisCell->distToBeg = cell.distToBeg + vertDist;
		thisCell->distToEnd = cellDistToEnd(*thisCell);
		thisCell->totVal = thisCell->distToBeg + thisCell->distToEnd;
		considered.push_back(*thisCell);
		if (thisCell->end)
			return 1;
	}

	//top right
	if ((cell.x + 1 <= boardHeight -1 && cell.y - 1 >= 0) && cellGrid[cell.x + 1][cell.y - 1].visited == false && cellGrid[cell.x + 1][cell.y - 1].obstacle == false && ((cell.distToBeg + diagDist) < (cellGrid[cell.x + 1][cell.y - 1].distToBeg))) {
		Cell *thisCell = &cellGrid[cell.x + 1][cell.y - 1];
		thisCell->considered = true;
		thisCell->owner = &cellGrid[cell.x][cell.y];
		thisCell->distToBeg = cell.distToBeg + diagDist;
		thisCell->distToEnd = cellDistToEnd(*thisCell);
		thisCell->totVal = thisCell->distToBeg + thisCell->distToEnd;
		considered.push_back(*thisCell);
		if (thisCell->end)
			return 1;
	}

	//left
	if ((cell.x - 1 >= 0) && cellGrid[cell.x - 1][cell.y].visited == false && cellGrid[cell.x - 1][cell.y].obstacle == false && ((cell.distToBeg + diagDist) < (cellGrid[cell.x - 1][cell.y].distToBeg))) {
		//update the Cell
		Cell *thisCell = &cellGrid[cell.x - 1][cell.y];
		thisCell->considered = true;
		thisCell->owner = &cellGrid[cell.x][cell.y];
		thisCell->distToBeg = cell.distToBeg + vertDist;
		thisCell->distToEnd = cellDistToEnd(*thisCell);
		thisCell->totVal = thisCell->distToBeg + thisCell->distToEnd;
		considered.push_back(*thisCell);
		if (thisCell->end)
			return 1;
	}

	//right
	if ((cell.x + 1 <= boardWidth-1) && cellGrid[cell.x + 1][cell.y].visited == false && cellGrid[cell.x + 1][cell.y].obstacle == false && ((cell.distToBeg + diagDist) < (cellGrid[cell.x + 1][cell.y].distToBeg))) {
		//update the Cell
		Cell *thisCell = &cellGrid[cell.x + 1][cell.y];
		thisCell->considered = true;
		thisCell->owner = &cellGrid[cell.x][cell.y];
		thisCell->distToBeg = cell.distToBeg + vertDist;
		thisCell->distToEnd = cellDistToEnd(*thisCell);
		thisCell->totVal = thisCell->distToBeg + thisCell->distToEnd;
		considered.push_back(*thisCell);
		if (thisCell->end)
			return 1;
	}

	//bottom right
	if ((cell.x + 1 <= boardWidth-1 && cell.y + 1 <= boardHeight -1) && cellGrid[cell.x + 1][cell.y + 1].visited == false && cellGrid[cell.x + 1][cell.y + 1].obstacle == false && ((cell.distToBeg + diagDist) < (cellGrid[cell.x + 1][cell.y + 1].distToBeg))) {
		//update the Cell
		Cell *thisCell = &cellGrid[cell.x + 1][cell.y + 1];
		thisCell->considered = true;
		thisCell->owner = &cellGrid[cell.x][cell.y];
		thisCell->distToBeg = cell.distToBeg + diagDist;
		thisCell->distToEnd = cellDistToEnd(*thisCell);
		thisCell->totVal = thisCell->distToBeg + thisCell->distToEnd;
		considered.push_back(*thisCell);
		if (thisCell->end)
			return 1;
	}

	//bottom
	if ((cell.y + 1 <= boardHeight-1) && cellGrid[cell.x][cell.y + 1].visited == false && cellGrid[cell.x][cell.y + 1].obstacle == false && ((cell.distToBeg + diagDist) < (cellGrid[cell.x][cell.y + 1].distToBeg))) {
		//update the Cell
		Cell *thisCell = &cellGrid[cell.x][cell.y + 1];
		thisCell->considered = true;
		thisCell->owner = &cellGrid[cell.x][cell.y];
		thisCell->distToBeg = cell.distToBeg + vertDist;
		thisCell->distToEnd = cellDistToEnd(*thisCell);
		thisCell->totVal = thisCell->distToBeg + thisCell->distToEnd;
		considered.push_back(*thisCell);
		if (thisCell->end)
			return 1;
	}

	//bottom left
	if ((cell.x - 1 >= 0 && cell.y + 1 <= boardHeight-1) && cellGrid[cell.x - 1][cell.y + 1].visited == false && cellGrid[cell.x - 1][cell.y + 1].obstacle == false && ((cell.distToBeg + diagDist) < (cellGrid[cell.x - 1][cell.y + 1].distToBeg))) {
		//update the Cell
		Cell *thisCell = &cellGrid[cell.x - 1][cell.y + 1];
		thisCell->considered = true;
		thisCell->owner = &cellGrid[cell.x][cell.y];
		thisCell->distToBeg = cell.distToBeg + diagDist;
		thisCell->distToEnd = cellDistToEnd(*thisCell);
		thisCell->totVal = thisCell->distToBeg + thisCell->distToEnd;
		considered.push_back(*thisCell);
		if (thisCell->end)
			return 1;
	}

	sortConsidered();
	return 0;
}

//clears out the visited and considered lists
void Board::resetPath() {
	considered.clear();
	visited.clear();
}

Board::~Board()
{
}
