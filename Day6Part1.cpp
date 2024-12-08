#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>

using namespace std;

// Define directions
const vector<pair<int, int>> directions = {
    {-1, 0}, // Up
    {0, 1},  // Right
    {1, 0},  // Down
    {0, -1}  // Left
};


bool isObstacle(const vector<string>& map, int x, int y) {
    return map[x][y] == '#';
}

bool isOutOfBounds(const vector<string>& map, int x, int y) {
    return x < 0 || x >= map.size() || y < 0 || y >= map[0].size();
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
    int guardX, guardY, direction = 0;
    bool guardFound = false;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (map[i][j] == '^') {
                guardX = i;
                guardY = j;
                direction = 0; // Facing up
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

    // Set to track distinct positions visited
    set<pair<int, int>> visited;
    visited.insert({guardX, guardY});

    while (true) {
        int nextX = guardX + directions[direction].first;
        int nextY = guardY + directions[direction].second;

        if (isOutOfBounds(map, nextX, nextY)) {
            break; // Guard leaves the map
            
            
        }
        

        if (isObstacle(map, nextX, nextY)) {
            // Turn right (90 degrees)
            direction = (direction + 1) % 4;
        } else {
            // Move forward
            guardX = nextX;
            guardY = nextY;
            visited.insert({guardX, guardY});
        }
    }

    // Output the result
    cout << "Distinct positions visited before leaving mapped area: " << visited.size() << endl;

    return 0;
}

