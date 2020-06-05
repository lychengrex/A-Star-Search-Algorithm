#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::abs;
using std::cout;
using std::ifstream;
using std::istringstream;
using std::sort;
using std::string;
using std::vector;

enum class State { kEmpty,
                   kObstacle,
                   kClosed,
                   kPath,
                   kStart,
                   kFinish };

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
        if (n == 0) {
            row.push_back(State::kEmpty);
        } else {
            row.push_back(State::kObstacle);
        }
    }
    return row;
}

vector<vector<State>> ReadBoardFile(string path) {
    ifstream myfile(path);
    vector<vector<State>> board{};
    if (myfile) {
        string line;
        while (getline(myfile, line)) {
            vector<State> row = ParseLine(line);
            board.push_back(row);
        }
    }
    return board;
}

/**
 * Compare the f-value of two nodes: f = g + h
 * @param node1 Node(x, y, g, h)
 * @param node2 Node(x, y, g, h)
 */
bool Compare(const vector<int> &node1, const vector<int> &node2) {
    return node1[2] + node1[3] > node2[2] + node2[3];
}

/**
 * Sort the two-dimensional vector of ints in descending order.
 * @param v The pointer to openlist vector
 */
void CellSort(vector<vector<int>> *v) {
    sort(v->begin(), v->end(), Compare);
}

/**
 * Calculate the Manhattan Distance from one coordinate to the other: |x_2- x_1| + |y_2 - y_1|
 * @param x1 X coordinate of node 1
 * @param y1 Y coordinate of node 1
 * @param x2 X coordinate of node 2
 * @param y2 Y coordinate of node 2
 */
int Heuristic(const int &x1, const int &y1, const int &x2, const int &y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

/**
 * Check that the cell is on the grid and not an obstacle (i.e. equals kEmpty).
 * @param x X coordinate
 * @param y Y coordinate
 * @param grid The grid
 */
bool CheckValidCell(int x, int y, vector<vector<State>> &grid) {
    if (x < 0 || x >= grid.size() || y < 0 || y >= grid[0].size())
        return false;
    if (grid[x][y] == State::kEmpty)
        return true;
    return false;
}

/**
 * Add a node to the open list and mark it as open.
 * @param x X coordinate
 * @param y Y coordinate
 * @param g Cost that has accumulated up to that cell
 * @param h Value for the current cell given by the heuristic function
 */
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<State>> &grid) {
    vector<int> node{x, y, g, h};
    openlist.push_back(node);
    grid[x][y] = State::kClosed;
}

/** 
 * Expand current nodes's neighbors and add them to the open list.
 * @param currentNode The current node (x, y, g, h)
 * @param goal The coordinates of destination node
 * @param openlist The candidates to be searched
 * @param grid The grid
 */
void ExpandNeighbors(const vector<int> &currentNode, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &grid) {
    int x = currentNode[0];
    int y = currentNode[1];
    int g = currentNode[2];

    for (int i = 0; i < 4; i++) {
        int newX = x + delta[i][0];
        int newY = y + delta[i][1];
        if (CheckValidCell(newX, newY, grid)) {
            int h = Heuristic(newX, newY, goal[0], goal[1]);
            AddToOpen(newX, newY, g + 1, h, openlist, grid);
        }
    }
}

/**
 * Implementation of A* search algorithm
 * @param grid The grid
 * @param init Start coordinates for the search
 * @param goal End coordinates for the search
 */
vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2]) {
    // Create the vector of open nodes.
    vector<vector<int>> openlist{};

    // Initialize the starting node.
    int x = init[0];
    int y = init[1];
    int g = 0;
    int h = Heuristic(x, y, goal[0], goal[1]);

    // Use AddToOpen to add the starting node to the openlist vector.
    AddToOpen(x, y, g, h, openlist, grid);

    // while openlist vector is non empty {
    while (!openlist.empty()) {
        // Sort the openlist using CellSort, and get the current node.
        CellSort(&openlist);  // pass the address since the function needs a pointer
        vector<int> currentNode = openlist.back();
        openlist.pop_back();

        // Get the x and y values from the current node, and set grid[x][y] to kPath.
        int curX = currentNode[0];
        int curY = currentNode[1];
        grid[curX][curY] = State::kPath;

        // Check if you've reached the goal. If so, return grid.
        if (curX == goal[0] && curY == goal[1]) {
            grid[init[0]][init[1]] = State::kStart;
            grid[goal[0]][goal[1]] = State::kFinish;
            return grid;
        }

        // If we're not done, expand search to current node's neighbors. This step will be completed in a later quiz.
        ExpandNeighbors(currentNode, goal, openlist, grid);
    }

    // We've run out of new nodes to explore and haven't found a path.
    cout << "No path found!\n";
    return vector<vector<State>>{};
}

string CellString(State cell) {
    switch (cell) {
        case State::kObstacle:
            return "⛰️   ";
        case State::kPath:
            return "🚗   ";
        case State::kStart:
            return "🚦   ";
        case State::kFinish:
            return "🏁   ";
        default:
            return "0   ";
    }
}

void PrintBoard(const vector<vector<State>> board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            cout << CellString(board[i][j]);
        }
        cout << "\n";
    }
}

#include "test.cpp"  // For testing solution

int main() {
    int init[2]{0, 0};
    int goal[2]{4, 5};
    auto board = ReadBoardFile("1.board");
    auto solution = Search(board, init, goal);
    PrintBoard(solution);
    // Tests
    TestHeuristic();
    TestAddToOpen();
    TestCompare();
    TestSearch();
    TestCheckValidCell();
    TestExpandNeighbors();
}