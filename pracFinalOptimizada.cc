// Nombre: Omar Bouaouda Ruiz - DNI/NIE: 48736098H

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <functional>

using namespace std;

struct Position {
    int x, y;
};

struct Node {
    Position pos; // Almacena solo la posición actual
    int costoActual;
    int estimacion; 
    Node* parent; // Puntero al nodo padre para reconstruir el camino más tarde

    Node(int x, int y, int costo, int est, Node* parent)
        : pos{x, y}, costoActual(costo), estimacion(est), parent(parent) {}

    bool operator<(const Node& otro) const {
        return this->estimacion > otro.estimacion;
    }
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
    int distX = abs((n-1) - x);
    int distY = abs((m-1) - y);
    return (distX + distY) * menorCosto;
}

void mcp_bb(vector<vector<int>>& mapa, int n, int m, bool mostrarCamino, bool mostrarCamino2D) {
    unordered_map<int, Node*> visitado;
    priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*) >> pq(
        [](Node* a, Node* b) { return a->estimacion > b->estimacion; });
    int menorCosto = findLowestCost(mapa);

    // Inicializar estadísticas
    int nvisit = 0, nexplored = 0, nleaf = 0, nunfeasible = 0;
    int nnot_promising = 0, npromising_but_discarded = 0;
    int nbest_solution_updated_from_leaves = 0, nbest_solution_updated_from_pessimistic_bound = 0;

    Node* inicial = new Node(0, 0, mapa[0][0], mapa[0][0] + estimarCotaPesimista(mapa, 0, 0, n, m, menorCosto), nullptr);
    pq.push(inicial);
    visitado[inicial->pos.x * m + inicial->pos.y] = inicial;
    nvisit++; // Contando el nodo inicial como visitado

    Node* mejorNodo = nullptr;
    int costoMin = INT_MAX;
    clock_t start = clock();

    while (!pq.empty()) {
        Node* actual = pq.top();
        pq.pop();
        nexplored++; // Cada nodo extraído de la cola se cuenta como explorado

        if (actual->pos.x == n - 1 && actual->pos.y == m - 1) {
            if (actual->costoActual < costoMin) {
                costoMin = actual->costoActual;
                mejorNodo = actual;
                nbest_solution_updated_from_leaves++;
            }
            nleaf++; // Contar como nodo hoja
            continue;
        }


        //Miramos todos los movimientos posibles
        for (int i = 0; i < 8; i++) {
            int nx = actual->pos.x + moves[i].x;
            int ny = actual->pos.y + moves[i].y;
            if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
                int idx = nx * m + ny;
                if (!visitado.count(idx)) { // Solo expandir si no ha sido visitado
                    int nuevoCosto = actual->costoActual + mapa[nx][ny];
                    int est = nuevoCosto + estimarCotaPesimista(mapa, nx, ny, n, m, menorCosto);
                    if (est < costoMin) {
                        Node* nuevoNodo = new Node(nx, ny, nuevoCosto, est, actual);
                        pq.push(nuevoNodo);
                        visitado[idx] = nuevoNodo;
                        nvisit++;
                    } else {
                        nnot_promising++;
                    }
                } else {
                    npromising_but_discarded++;
                }
            } else {
                nunfeasible++; // Nodo no factible
            }
        }
    }

    clock_t end = clock();
    double cpu_time_used = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000;

    // Mostrar coste mínimo
    cout << costoMin << endl;

    // Mostrar estadísticas
    cout << nvisit << " " << nexplored << " " << nleaf << " " << nunfeasible << " " << nnot_promising << " ";
    cout << npromising_but_discarded << " " << nbest_solution_updated_from_leaves << " ";
    cout << nbest_solution_updated_from_pessimistic_bound << endl;

    cout << fixed << setprecision(3) << cpu_time_used << endl;

    // Mostrar camino si es necesario
    if (mostrarCamino2D || mostrarCamino) {
        vector<Position> camino;
        for (Node* node = mejorNodo; node != nullptr; node = node->parent) {
            camino.push_back(node->pos);
        }
        reverse(camino.begin(), camino.end());

        if (mostrarCamino2D) {
            vector<vector<char>> camino2D(n, vector<char>(m, '.'));
            for (const auto& pos : camino) {
                camino2D[pos.x][pos.y] = 'x';
            }
            for (const auto& row : camino2D) {
                for (char c : row) {
                    cout << c;
                }
                cout << endl;
            }
            cout << costoMin << endl;
        }

        if (mostrarCamino) {
            cout << "<";
            for (size_t i = 1; i < camino.size(); i++) {
                for (int j = 0; j < 8; j++) {
                    if (camino[i].x - camino[i - 1].x == moves[j].x && camino[i].y - camino[i - 1].y == moves[j].y) {
                        cout << move_codes[j];
                        break;
                    }
                }
            }
            cout << ">" << endl;
        }
    }

    // Limpiar memoria
    for (auto& pair : visitado) {
        delete pair.second;
    }
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

    mcp_bb(mapa, n, m, mostrarCamino, mostrarCamino2D);

    return 0;
}
