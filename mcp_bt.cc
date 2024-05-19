#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <climits>
#include <ctime>
#include <iomanip>

using namespace std;

struct Position {
    int x, y;
};

const Position moves[8] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};
const char move_codes[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};

void mcp_bt(vector<vector<int>>& mapa, int x, int y, vector<vector<bool>>& visitado, int& costoMin, vector<Position>& camino, vector<Position>& mejorCamino, int costoActual, int& visitas, int& explorados, int& hojas, int& noFactibles, int& noPrometedores) {
    int n = mapa.size();
    int m = mapa[0].size();
    
    if (x < 0 || x >= n || y < 0 || y >= m || visitado[x][y] || costoActual >= costoMin) {
        noFactibles++;
        return;
    }

    visitado[x][y] = true;
    camino.push_back({x, y});
    visitas++;
    costoActual += mapa[x][y];

    if (x == n-1 && y == m-1) {
        if (costoActual < costoMin) {
            costoMin = costoActual;
            mejorCamino = camino;
        }
        hojas++;
    } else {
        for (int i = 0; i < 8; i++) {
            int siguienteX = x + moves[i].x;
            int siguienteY = y + moves[i].y;
            if (siguienteX >= 0 && siguienteX < n && siguienteY >= 0 && siguienteY < m && !visitado[siguienteX][siguienteY]) {
                explorados++;
                mcp_bt(mapa, siguienteX, siguienteY, visitado, costoMin, camino, mejorCamino, costoActual, visitas, explorados, hojas, noFactibles, noPrometedores);
            }
        }
    }

    visitado[x][y] = false;
    camino.pop_back();
}

int main(int argc, char* argv[]) {
    string nombreArchivo;
    bool mostrarCamino = false, mostrarCamino2D = false;
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-f" && i + 1 < argc) {
            nombreArchivo = argv[++i];
        } else if (arg == "-p") {
            mostrarCamino = true;
        } else if (arg == "--p2D") {
            mostrarCamino2D = true;
        }
    }

    ifstream archivoEntrada(nombreArchivo);
    if (!archivoEntrada) {
        cerr << "Error al abrir el archivo: " << nombreArchivo << endl;
        return 1;
    }

    int n, m;
    archivoEntrada >> n >> m;
    vector<vector<int>> mapa(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            archivoEntrada >> mapa[i][j];
        }
    }

    vector<vector<bool>> visitado(n, vector<bool>(m, false));
    int costoMin = INT_MAX, visitas = 0, explorados = 0, hojas = 0, noFactibles = 0, noPrometedores = 0;
    vector<Position> camino, mejorCamino;
    clock_t inicio = clock();

    mcp_bt(mapa, 0, 0, visitado, costoMin, camino, mejorCamino, 0, visitas, explorados, hojas, noFactibles, noPrometedores);

    clock_t fin = clock();
    double tiempoCPU = ((double) (fin - inicio)) / CLOCKS_PER_SEC * 1000;

    cout << costoMin << endl;
    cout << visitas << " " << explorados << " " << hojas << " " << noFactibles << " " << noPrometedores << endl;
    cout << fixed << setprecision(3) << tiempoCPU << " ms" << endl;

    if (mostrarCamino2D) {
        vector<string> mapaVisual(n, string(m, '.'));
        for (auto& posicion : mejorCamino) {
            mapaVisual[posicion.x][posicion.y] = 'x';
        }
        for (auto& fila : mapaVisual) {
            cout << fila << endl;
        }
        cout << costoMin << endl;
    }

    if (mostrarCamino) {
        if (!mejorCamino.empty()) {
            cout << "<";
            for (size_t i = 1; i < mejorCamino.size(); i++) {
                Position previa = mejorCamino[i-1];
                Position actual = mejorCamino[i];
                for (int j = 0; j < 8; j++) {
                    if (previa.x + moves[j].x == actual.x && previa.y + moves[j].y == actual.y) {
                        cout << move_codes[j];
                        break;
                    }
                }
            }
            cout << ">" << endl;
        } else {
            cout << "<>" << endl;
        }
    }

    return 0;
}
