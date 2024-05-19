#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <climits>
#include <ctime>

using namespace std;

struct Position {
    int x, y;
};

const Position moves[8] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};
const char move_codes[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};  // Corresponding move codes

void mcp_bt(vector<vector<int>>& map, int x, int y, vector<vector<bool>>& visited, int& minCost, vector<Position>& path, vector<Position>& bestPath, int currentCost, int& nvisit, int& nexplored, int& nleaf, int& nunfeasible, int& nnot_promising) {
    int n = map.size();
    int m = map[0].size();
    
    if (x < 0 || x >= n || y < 0 || y >= m || visited[x][y]) {
        nunfeasible++;
        return;
    }

    visited[x][y] = true;
    path.push_back({x, y});
    nvisit++;
    currentCost += map[x][y];

    if (x == n-1 && y == m-1) {
        if (currentCost < minCost) {
            minCost = currentCost;
            bestPath = path;
        }
        nleaf++;
    } else {
        for (int i = 0; i < 8; i++) {
            int nextX = x + moves[i].x;
            int nextY = y + moves[i].y;
            if (nextX >= 0 && nextX < n && nextY >= 0 && nextY < m && !visited[nextX][nextY]) {
                nexplored++;
                mcp_bt(map, nextX, nextY, visited, minCost, path, bestPath, currentCost, nvisit, nexplored, nleaf, nunfeasible, nnot_promising);
            }
        }
    }

    visited[x][y] = false;
    path.pop_back();
}

int main(int argc, char* argv[]) {
    string filename;
    bool showPath = false, showPath2D = false;
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-f" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "-p") {
            showPath = true;
        } else if (arg == "--p2D") {
            showPath2D = true;
        }
    }

    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        return 1;
    }

    int n, m;
    inputFile >> n >> m;
    vector<vector<int>> map(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            inputFile >> map[i][j];
        }
    }

    vector<vector<bool>> visited(n, vector<bool>(m, false));
    int minCost = INT_MAX, nvisit = 0, nexplored = 0, nleaf = 0, nunfeasible = 0, nnot_promising = 0;
    vector<Position> path, bestPath;
    clock_t start = clock();

    mcp_bt(map, 0, 0, visited, minCost, path, bestPath, 0, nvisit, nexplored, nleaf, nunfeasible, nnot_promising);

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000.0;

    cout << minCost << endl;
    cout << nvisit << " " << nexplored << " " << nleaf << " " << nunfeasible << " " << nnot_promising << endl;
    cout << cpu_time_used << endl;

    if (showPath) {
        cout << "<";
        for (size_t i = 1; i < bestPath.size(); i++) {
            Position prev = bestPath[i-1];
            Position curr = bestPath[i];
            for (int j = 0; j < 8; j++) {
                if (prev.x + moves[j].x == curr.x && prev.y + moves[j].y == curr.y) {
                    cout << move_codes[j];
                    break;
                }
            }
        }
        cout << ">" << endl;
    }

    if (showPath2D) {
        vector<string> displayMap(n, string(m, '.'));
        for (auto& pos : bestPath) {
            displayMap[pos.x][pos.y] = 'x';
        }
        for (auto& row : displayMap) {
            cout << row << endl;
        }
        cout << minCost << endl;
    }

    return 0;
}
