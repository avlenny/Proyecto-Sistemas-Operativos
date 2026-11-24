#include "tipos.h"
#include <iostream>
#include <string>
using namespace std;

static int leerInt(const string& texto, int min, int max, int defecto) {
    while (true) {
        cout << texto << ": ";
        string s;
        if (!getline(cin, s)) return defecto;   
        if (s.empty()) return defecto;        
        try {
            size_t usados = 0;
            int v = stoi(s, &usados);
            if (usados == s.size() && v >= min && v <= max) return v;
        } catch (...) {
        }
        cout << "  Valor invalido, intenta de nuevo\n";
    }
}

Config pedirConfig() {
    Config cfg;
    cout << "--- Configuracion de la Simulacion ---\n";
    cfg.algoritmo = leerInt("Algoritmo: 1) Primer ajuste  2) Peor ajuste", 1, 2, 1);
    int mem = leerInt("Memoria: 1) 1 MB  2) 4 MB  3) 8 MB", 1, 3, 2);
    cfg.memoriaKB = (mem == 1) ? 1024 : (mem == 2) ? 4096 : 8192;
    cfg.quantum = leerInt("Cuanto del sistema", 1, 20, 2);
    cfg.maxTam = leerInt("Tamano maximo de proceso (KB)", 1, cfg.memoriaKB, 500);
    cfg.maxCuanto = leerInt("Cuanto maximo por proceso", 1, 100, 10);
    system("cls");
    return cfg;
}

