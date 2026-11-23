#include "memoria.h"

static std::vector<Segmento> mem;
static int algoritmo = PRIMER_AJUSTE;

void memIniciar(const Config& cfg) {
    algoritmo = cfg.algoritmo;
    mem.clear();
    mem.push_back({LIBRE, 0, cfg.memoriaKB});
}

static int buscarHueco(int tam) {
    int elegido = -1;
    for (int i = 0; i < (int)mem.size(); i++) {
        if (mem[i].id != LIBRE || mem[i].tam < tam) continue;
        if (algoritmo == PRIMER_AJUSTE) return i;
        if (elegido == -1 || mem[i].tam > mem[elegido].tam) elegido = i;
    }
    return elegido;
}

bool memAsignar(const Bloque& p) {
    int i = buscarHueco(p.tam);
    if (i == -1) return false;
    Segmento hueco = mem[i];
    if (hueco.tam - p.tam >= SPLIT_MINIMO_KB) {
        mem[i] = {p.id, hueco.inicio, p.tam};
        mem.insert(mem.begin() + i + 1,
                   {LIBRE, hueco.inicio + p.tam, hueco.tam - p.tam});
    } else {
        mem[i].id = p.id;
    }
    return true;
}

void descargarProceso(int id) {
    for (int i = 0; i < (int)mem.size(); i++) {
        if (mem[i].id != id) continue;
        mem[i].id = LIBRE;
        if (i + 1 < (int)mem.size() && mem[i + 1].id == LIBRE) {
            mem[i].tam += mem[i + 1].tam;
            mem.erase(mem.begin() + i + 1);
        }
        if (i > 0 && mem[i - 1].id == LIBRE) {
            mem[i - 1].tam += mem[i].tam;
            mem.erase(mem.begin() + i);
        }
        return;
    }
}

const std::vector<Segmento>& memLista() {
    return mem;
}
