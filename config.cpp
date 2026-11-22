#include "tipos.h"
#include <iostream>
#include <string>
using namespace std;

Config pedirConfigu();
void mostrarConfig(const Config& cfg);

static int leerEntero(const string& texto, int minimo, int maximo, int defecto) {
    while (true) {
        cout << texto << " [" << minimo << "-" << maximo
             << "] (Enter = " << defecto << "): ";
        string s;
        if (!getline(cin, s)) return defecto;   
        if (s.empty()) return defecto;        
        try {
            size_t usados = 0;
            int v = stoi(s, &usados);
            if (usados == s.size() && v >= minimo && v <= maximo) return v;
        } catch (...) {
            // no era un numero: cae al mensaje de error
        }
        cout << "  Valor invalido, intenta de nuevo.\n";
    }
}


