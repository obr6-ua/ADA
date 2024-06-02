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

int findLowestCost(const vector<vector<int>>& mapa) {
    int menorCosto = INT_MAX;
    for (const auto& row : mapa) {
        for (int cost : row) {
            if (cost < menorCosto) menorCosto = cost;
        }
    }
    return menorCosto;
}

int estimarCotaPesimista(const vector<vector<int>>& mapa, int x, int y, int n, int m, int menorCosto) {
    // Calcula la distancia hasta el destino (n-1, m-1)
    int distX = abs((n-1) - x);
    int distY = abs((m-1) - y);

    // Estimación pesimista basada en la distancia de Manhattan multiplicada por el menor costo encontrado
    return (distX + distY) * menorCosto;
}

void mcp_bb(vector<vector<int>>& mapa, int x, int y, vector<vector<bool>>& visitado, int& costoMin, vector<Position>& camino, vector<Position>& mejorCamino, int costoActual, int menorCosto, int& visitas, int& explorados, int& hojas, int& noFactibles, int& noPrometedores, int& prometedoresDescartados) {
    int n = mapa.size();
    int m = mapa[0].size();
    
    if (x < 0 || x >= n || y < 0 || y >= m || visitado[x][y]) {
        noFactibles++;
        return;
    }

    int cotaPesimista = estimarCotaPesimista(mapa, x, y, n, m, menorCosto);
    if (costoActual + cotaPesimista >= costoMin) {
        noPrometedores++;
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
            explorados++;
            int siguienteX = x + moves[i].x;
            int siguienteY = y + moves[i].y;
            mcp_bb(mapa, siguienteX, siguienteY, visitado, costoMin, camino, mejorCamino, costoActual, menorCosto, visitas, explorados, hojas, noFactibles, noPrometedores, prometedoresDescartados);
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
    int costoMin = INT_MAX;
    int menorCosto = findLowestCost(mapa);
    int visitas = 0, explorados = 0, hojas = 0, noFactibles = 0, noPrometedores = 0, prometedoresDescartados = 0;
    vector<Position> camino, mejorCamino;
    clock_t inicio = clock();

    mcp_bb(mapa, 0, 0, visitado, costoMin, camino, mejorCamino, 0, menorCosto, visitas, explorados, hojas, noFactibles, noPrometedores, prometedoresDescartados);

    clock_t fin = clock();
    double tiempoCPU = ((double) (fin - inicio)) / CLOCKS_PER_SEC * 1000;

    cout << costoMin << endl;
    cout << visitas << " " << explorados << " " << hojas << " " << noFactibles << " " << noPrometedores << " " << prometedoresDescartados << endl;
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
