#include "tipos.h"
#include <vector>
#include <deque>
#include <iostream>

static std::vector<Bloque> memoria;

void iniciarMemoria(int memoriaKB) {
    memoria.clear();
    memoria.push_back(Bloque{0, memoriaKB, 0, 0});  
}

static int elegirHueco(int tam, int alg) {
    int elegido = -1;
    for (int i = 0; i < (int)memoria.size(); i++) {
        if (memoria[i].id != 0 || memoria[i].tam < tam) continue;  

        if (alg == 1) return i;                      

        if (elegido == -1 || memoria[i].tam > memoria[elegido].tam)
            elegido = i;
    }
    return elegido;
}

bool colocarProceso(const Bloque& p, int alg) {
    int i = elegirHueco(p.tam, alg);
    if (i == -1) return false;                       

    int tamHueco = memoria[i].tam;
    int sobra = tamHueco - p.tam;

    memoria[i] = p;                                   
    if (sobra >= SPLIT_MINIMO_KB)
        memoria.insert(memoria.begin() + i + 1, Bloque{0, sobra, 0, 0});  
    else
        memoria[i].tam = tamHueco;                    
    return true;
}

//fusiona huecos contiguos para evitar fragmentacion externa
static void condensar() {
    for (int i = 0; i + 1 < (int)memoria.size(); ) {
        if (memoria[i].id == 0 && memoria[i + 1].id == 0) {
            memoria[i].tam += memoria[i + 1].tam;
            memoria.erase(memoria.begin() + i + 1);   
        } else {
            i++;
        }
    }
}

void descargarProceso(int id) {
    for (auto& b : memoria) {
        if (b.id == id) { b.id = 0; b.cuanto = 0; break; }
    }
    imprimirMemoria();       
    mostrarCondensando();
    condensar();
}

int memoriaOcupadaKB() {
    int total = 0;
    for (const auto& b : memoria)
        if (b.id != 0) total += b.tam;
    return total;
}

void imprimirMemoria() {
    for (const auto& b : memoria) {
        int cuanto = 0;
        if (b.id != 0)
            for (const auto& p : rrLista())
                if (p.id == b.id) { cuanto = p.cuanto; break; }
        std::cout << "[" << b.id << "," << b.tam << "," << cuanto << "]";
    }
    std::cout << "\n";
}