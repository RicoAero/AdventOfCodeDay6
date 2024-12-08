#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <tuple> //store my static data sets
#include <string>

using namespace std;

// Define directions
const vector<pair<int, int>> directions = {
    {-1, 0}, // Up
    {0, 1},  // Right
    {1, 0},  // Down
    {0, -1}  // Left
};

// Helper function to check if a position is an obstacle (#)
bool isObstacle(const vector<string>& map, int x, int y) {
    return map[x][y] == '#';
}


// Helper function to check if a position is out of bounds
bool isOutOfBounds(const vector<string>& map, int x, int y) {
    return x < 0 || x >= map.size() || y < 0 || y >= map[0].size();
}


// Simulate the guard's patrol and check for loops
bool causesLoop(vector<string> map, int obstructionX, int obstructionY, int startX, int startY, int startDir) {
    map[obstructionX][obstructionY] = '#'; // Place the obstruction
    set<tuple<int, int, int>> statePast; // Track (x, y, direction)
    
    int x = startX, y = startY, direction = startDir;
    while (true) {
        
		int nextX = x + directions[direction].first;
        int nextY = y + directions[direction].second;

        // If out of bounds, return false (cant loop)
        if (isOutOfBounds(map, nextX, nextY)) return false;

        // If there's an obstacle, turn right
        if (isObstacle(map, nextX, nextY)) {
            direction = (direction + 1) % 4;
        } else {
            // Move forward
            x = nextX;
            y = nextY;

            // Check if the current state has been seen before
            tuple<int, int, int> currentState = make_tuple(x, y, direction);
            if (statePast.count(currentState) > 0) {
                return true; // Loop detected
            }

            statePast.insert(currentState);
        }
    }
}

int main() {
    // File name for the input map
    string fileName = "map.txt";
    ifstream inputFile(fileName);

    if (!inputFile) {
        cerr << "Error: Could not open the file " << fileName << endl;
        return 1;
    }

    // Read the map from the file
    vector<string> map;
    string line;
    while (getline(inputFile, line)) {
        map.push_back(line);
    }
    inputFile.close();

    if (map.empty()) {
        cerr << "Error: Map is empty!" << endl;
        return 1;
    }

    int rows = map.size();
    int cols = map[0].size();

    // Find the guard's initial position and direction
    int startX, startY, startDir = 0;
    bool guardFound = false;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (map[i][j] == '^') {
                startX = i;
                startY = j;
                startDir = 0; // Facing up
                guardFound = true;
                break;
            }
        }
        if (guardFound) break;
    }

    if (!guardFound) {
        cerr << "Error: No guard (^) found on the map!" << endl;
        return 1;
    }

    // Track possible obstruction positions
    int obstructionCount = 0;
    set<pair<int, int>> validObstructionPositions;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Skip the guard's starting position or existing obstacles
            if (map[i][j] == '^' || map[i][j] == '#') continue;

            // Check if placing an obstruction here causes a loop
            if (causesLoop(map, i, j, startX, startY, startDir)) {
                validObstructionPositions.insert({i, j});
                obstructionCount++; //add 1 to the total number of obstructions
                // Debug output for valid positions
                cout << "I can place an obstruction at (" << i << ", " << j << ")" << endl; //remove later or export to txt?
            }
        }
    }

    // Output the result
    cout << "Number of positions to place an obstruction: " << obstructionCount << endl;

    return 0;
}

