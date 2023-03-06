#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <random>
#include <vector>
#include <utility>
#include <stack>

//This implementation of a maze generating algorithm uses a randomized version of Prim's algorithm
//   https://en.wikipedia.org/wiki/Maze_generation_algorithm

//Dimensions of the board (needs to be odd numbers)
const int row = 27;
const int col = 61;

void generateMaze(char(&maze)[row][col]);
void solveMazeDFS(int, int, char(&maze)[row][col], bool(&visited)[row][col]);

struct Cell {
	int row;
	int col;
	bool visited;
};

// Define the four possible moves from a cell
const int numMoves = 4;
const int moves[numMoves][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

// Function to check if a cell is within the maze bounds and is unvisited
bool isValidMove(int curRow, int curCol, char (&maze)[row][col], bool (&visited)[row][col]) {
	return curRow >= 1 && curRow < (row - 1) && curCol >= 1 && curCol < (col - 1) && (maze[curRow][curCol] == ' ' || maze[curRow][curCol] == 'e') && !visited[curRow][curCol];
}

int main()
{
	char maze[row][col];
	bool visited[row][col] = { };

	generateMaze(maze);
	solveMazeDFS((row - 2), 1, maze, visited);

	return 0;
}

// Function to perform a depth-first search (DFS) to find a path through the maze
void solveMazeDFS(int startRow, int startCol, char(&maze)[row][col], bool(&visited)[row][col]) {
	//Starts at the end and solves the maze backwards
	//Checks for valid moves
	//Marks valid unvisited cells from the currently visited cell as moves from that cell
	//When we reach start, we have a path
	char solved[row][col];
	char displaySolved[row][col];

	for (int rows = 0; rows < row; rows++)
	{
		for (int cols = 0; cols < col; cols++)
		{
			solved[rows][cols] = maze[rows][cols];
			displaySolved[rows][cols] = maze[rows][cols];
		}
	}

	// Create a stack to keep track of cells to visit
	std::stack<Cell> s;

	int curRow;
	int curCol;
	int lastRow;
	int lastCol;

	// Mark the starting cell as visited and push it onto the stack
	Cell startCell = { startRow, startCol };
	visited[startRow][startCol] = true;
	s.push(startCell);

	// Loop until the stack is empty
	while (!s.empty()) {
		// Pop a cell from the stack
		Cell currCell = s.top();
		lastRow = currCell.row;
		lastCol = currCell.col;
		s.pop();

		// If the current cell is the end of the maze, return true
		if (currCell.row == 1 && currCell.col == col - 2) {
			
			curRow = 1;
			curCol = col - 2;

			while (displaySolved[curRow][curCol] != 'p')
			{
				if (solved[curRow][curCol] == '<')
				{
					curCol -= 1;
					if (displaySolved[curRow][curCol] != 'p')
						displaySolved[curRow][curCol] = '.';
				}
				else if (solved[curRow][curCol] == '>')
				{
					curCol += 1;
					if (displaySolved[curRow][curCol] != 'p')
						displaySolved[curRow][curCol] = '.';
				}
				else if (solved[curRow][curCol] == '^')
				{
					curRow -= 1;
					if (displaySolved[curRow][curCol] != 'p')
						displaySolved[curRow][curCol] = '.';
				}
				else
				{
					curRow += 1;
					if (displaySolved[curRow][curCol] != 'p')
						displaySolved[curRow][curCol] = '.';
				}
			}
			for (int rows = 0; rows < row; rows++)
			{
				for (int cols = 0; cols < col; cols++)
				{
					std::cout << displaySolved[rows][cols];
				}
				std::cout << std::endl;
			}
			return;
		}

		// Loop through the four possible moves from the current cell
		for (int i = 0; i < numMoves; i++) {
			int row = currCell.row + moves[i][0];
			int col = currCell.col + moves[i][1];

			// If the move is valid, mark the cell as visited and push it onto the stack
			if (isValidMove(row, col, maze, visited)) {
				visited[row][col] = true;
				if (lastRow < row && lastCol == col)
				{
					solved[row][col] = '^';
				}
				else if (lastRow > row && lastCol == col)
				{
					solved[row][col] = 'v';
				}
				else if (lastRow == row && lastCol > col)
				{
					solved[row][col] = '>';
				}
				else
				{
					solved[row][col] = '<';
				}
				Cell nextCell = { row, col };
				s.push(nextCell);
			}
		}
	}

	// If no path was found, return
	return;
}

void generateMaze(char(&maze)[row][col])
{

	//Holds coordinates for walls to check
	std::vector<std::pair<int, int>> wallList;
	int selectedWall = 0;
	int selectedRow = 0;
	int selectedCol = 0;

	//Tracks number of non wall cells around a specified wall
	int visitedCells = 0;

	//Logic to prevent walls that may not be adjacent to other walls
	bool oppositeSides = false;
	int topVisitedCells = 0;
	int bottomVisitedCells = 0;
	int leftVisitedCells = 0;
	int rightVisitedCells = 0;

	srand(1);

	//Initialize board
	for (int rows = 0; rows < row; rows++)
	{
		for (int cols = 0; cols < col; cols++)
		{
			maze[rows][cols] = '#';
		}
	}

	//Add a starting location to the wall list
	wallList.push_back(std::make_pair(row - 2, 1));

	//This is definitely not the proper implementation of the algorithm I was looking but its functional

	//While there are walls in the list
	while (wallList.size() > 0)
	{
		//Select a random wall from the list
		selectedWall = rand() % wallList.size();

		//Get the x y coordinates of the selected wall
		selectedRow = wallList.at(selectedWall).first;
		selectedCol = wallList.at(selectedWall).second;

		//Determines if there are existing cells that are on opposite sides
		for (int rows = -1; rows < 2; rows++)
		{
			if (maze[selectedRow - rows][selectedCol - 1] != '#')
			{
				leftVisitedCells++;
			}
			if (maze[selectedRow - rows][selectedCol + 1] != '#')
			{
				rightVisitedCells++;
			}
		}
		for (int cols = -1; cols < 2; cols++)
		{
			if (maze[selectedRow - 1][selectedCol + cols] != '#')
			{
				topVisitedCells++;
			}
			if (maze[selectedRow + 1][selectedCol - cols] != '#')
			{
				bottomVisitedCells++;
			}
		}

		//If there are not cells that exist on opposite sides of the selected wall
		if (!(topVisitedCells > 0 && bottomVisitedCells > 0) && !(leftVisitedCells > 0 && rightVisitedCells > 0))
		{
			//If only one of the cells that the wall divides is visited
			for (int rows = -1; rows < 2; rows++)
			{
				for (int cols = -1; cols < 2; cols++)
				{
					if (maze[selectedRow - rows][selectedCol - cols] != '#')
					{
						visitedCells++;
					}
				}
			}
		}
		else
		{
			visitedCells = 3;
		}

		//Make the wall a passage (' ') and mark the unvisited cell as part of the maze (visited)
		//Adds only the adjacent cells to the wallList
		if (visitedCells < 3)
		{
			maze[selectedRow][selectedCol] = ' ';

			if (selectedRow - 1 != 0)
			{
				if (maze[selectedRow - 1][selectedCol] != ' ')
				{
					wallList.push_back(std::make_pair(selectedRow - 1, selectedCol));
				}
			}
			if (selectedRow + 1 != row - 1)
			{
				if (maze[selectedRow + 1][selectedCol] != ' ')
				{
					wallList.push_back(std::make_pair(selectedRow + 1, selectedCol));
				}
			}
			if (selectedCol - 1 != 0)
			{
				if (maze[selectedRow][selectedCol - 1] != ' ')
				{
					wallList.push_back(std::make_pair(selectedRow, selectedCol - 1));
				}
			}
			if (selectedCol + 1 != col - 1)
			{
				if (maze[selectedRow][selectedCol + 1] != ' ')
				{
					wallList.push_back(std::make_pair(selectedRow, selectedCol + 1));
				}
			}
		}

		//Remove wall from the list
		wallList.erase(wallList.begin() + selectedWall);
		visitedCells = 0;
		topVisitedCells = 0;
		bottomVisitedCells = 0;
		leftVisitedCells = 0;
		rightVisitedCells = 0;
	}

	//Set player and end position characters
	maze[row - 2][1] = 'p';
	maze[1][col - 2] = 'e';

	//Displays board
	/**for (int rows = 0; rows < row; rows++)
	{
		for (int cols = 0; cols < col; cols++)
		{
			std::cout << maze[rows][cols];
		}
		std::cout << std::endl;
	}**/
}