#include "memoria.h"
using namespace std;

Memoria::Memoria(int t, Estrategia e) : total(t), estrategia(e) {
    bloques.push_back({0, t, -1});
}

// Primer ajuste: primer hueco que alcance. Peor ajuste: el hueco más grande.
int Memoria::buscarHueco(int tamano) const {
    int elegido = -1;
    for (int i = 0; i < (int)bloques.size(); i++) {
        if (bloques[i].pid != -1 || bloques[i].tamano < tamano) continue;
        if (estrategia == Estrategia::PRIMER_AJUSTE) return i;
        if (elegido == -1 || bloques[i].tamano > bloques[elegido].tamano) elegido = i;
    }
    return elegido;
}

bool Memoria::asignar(int pid, int tamano) {
    int i = buscarHueco(tamano);
    if (i == -1) {                       // ¿alcanza si se condensa?
        int libre = 0;
        for (auto& b : bloques) if (b.pid == -1) libre += b.tamano;
        if (libre < tamano) return false;
        condensar();
        i = buscarHueco(tamano);
    }
    Bloque hueco = bloques[i];
    bloques[i] = {hueco.inicio, tamano, pid};
    if (hueco.tamano > tamano)
        bloques.insert(bloques.begin() + i + 1,
                       {hueco.inicio + tamano, hueco.tamano - tamano, -1});
    return true;
}

bool Memoria::liberar(int pid) {
    for (int i = 0; i < (int)bloques.size(); i++) {
        if (bloques[i].pid != pid) continue;
        bloques[i].pid = -1;
        // fusionar con el hueco siguiente y con el anterior
        if (i + 1 < (int)bloques.size() && bloques[i + 1].pid == -1) {
            bloques[i].tamano += bloques[i + 1].tamano;
            bloques.erase(bloques.begin() + i + 1);
        }
        if (i > 0 && bloques[i - 1].pid == -1) {
            bloques[i - 1].tamano += bloques[i].tamano;
            bloques.erase(bloques.begin() + i);
        }
        return true;
    }
    return false;
}

// Junta los procesos al inicio y deja un solo hueco al final
void Memoria::condensar() {
    vector<Bloque> nuevos;
    int dir = 0;
    for (auto& b : bloques)
        if (b.pid != -1) { nuevos.push_back({dir, b.tamano, b.pid}); dir += b.tamano; }
    if (dir < total) nuevos.push_back({dir, total - dir, -1});
    bloques = nuevos;
}
