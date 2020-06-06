#ifndef AStarSearchAlgorithm_hpp
#define AStarSearchAlgorithm_hpp
#include <string>
#include <vector>
using std::string;
using std::vector;

namespace AStarSearchAlgorithm {

enum class State { kEmpty,
                   kObstacle,
                   kClosed,
                   kPath,
                   kStart,
                   kFinish };

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

/**
 * Parse the string (with consecutive number and comma).
 * @param line The string 
 */
vector<State> ParseLine(string line);

/**
 * Read the text file.
 * @param path The path to the file
 */
vector<vector<State>> ReadBoardFile(string path);

/**
 * Compare the f-value of two nodes: f = g + h
 * @param node1 Node(x, y, g, h)
 * @param node2 Node(x, y, g, h)
 */
bool Compare(const vector<int> &node1, const vector<int> &node2);

/**
 * Sort the two-dimensional vector of ints in descending order.
 * @param v The pointer to openlist vector
 */
void CellSort(vector<vector<int>> *v);

/**
 * Calculate the Manhattan Distance from one coordinate to the other: |x_2- x_1| + |y_2 - y_1|
 * @param x1 X coordinate of node 1
 * @param y1 Y coordinate of node 1
 * @param x2 X coordinate of node 2
 * @param y2 Y coordinate of node 2
 */
int Heuristic(const int &x1, const int &y1, const int &x2, const int &y2);

/**
 * Check if the x, y coordinates are valid
 * @param x X coordinate (row)
 * @param y Y coordinate (col)
 * @param grid The grid
 */
bool IsValidCoord(int x, int y, vector<vector<State>> &grid, bool verbose);

/**
 * Check that the cell is on the grid and not an obstacle (i.e. equals kEmpty).
 * @param x X coordinate
 * @param y Y coordinate
 * @param grid The grid
 */
bool CheckValidCell(int x, int y, vector<vector<State>> &grid);

/**
 * Add a node to the open list and mark it as open.
 * @param x X coordinate
 * @param y Y coordinate
 * @param g Cost that has accumulated up to that cell
 * @param h Value for the current cell given by the heuristic function
 */
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<State>> &grid);

/** 
 * Expand current nodes's neighbors and add them to the open list.
 * @param currentNode The current node (x, y, g, h)
 * @param goal The coordinates of destination node
 * @param openlist The candidates to be searched
 * @param grid The grid
 */
void ExpandNeighbors(const vector<int> &currentNode, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &grid);

/**
 * Implementation of A* search algorithm
 * @param grid The grid
 * @param init Start coordinates for the search
 * @param goal End coordinates for the search
 */
vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2]);

/**
 * Convert cell value from enum class to string.
 * @param cell The State of the cell
 */
string CellString(State cell);

/**
 * Print the board.
 * @param board The grid
 */
void PrintBoard(const vector<vector<State>> board);

}  // namespace AStarSearchAlgorithm
#endif  // AStarSearchAlgorithm_hpp