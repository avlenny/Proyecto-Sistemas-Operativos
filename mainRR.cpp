#include "tipos.h"
#include <iostream>
#include <conio.h>      
#define NOMINMAX        
#include <windows.h>    //sleep
using namespace std;

static bool esperar(int& velMs) {
    int espera = 0;
    while (espera < velMs) {
        while (_kbhit()) {
            int c = _getch();
            if (c == '\r' || c == '\n') return true;
            if (c == '+') velMs = (velMs - 100 < 50) ? 50 : velMs - 100;
            if (c == '-') velMs = (velMs + 100 > 5000) ? 5000 : velMs + 100;
        }
        Sleep(20);
        espera += 20;
    }
    return false;
}

int main() {
    Config cfg = pedirConfig();      
    int alg = cfg.algoritmo;        

    iniciarMemoria(cfg.memoriaKB);
    rrLimpiar();

    int ciclo = 0, siguienteId = 1, terminados = 0;
    long long sumaAtencion = 0;
    double sumaOcupacion = 0;        
    bool detener = false;

    Bloque pendiente = genProceso(cfg, siguienteId++, ciclo);   

    while (!detener) {
        ciclo++;
        mostrarCiclo(ciclo);
        if (colocarProceso(pendiente, alg)) {
            pendiente.cicloEntrada = ciclo;
            rrAgregar(pendiente);
            mostrarEntrada(pendiente);
            mostrarMemoria();                              
            pendiente = genProceso(cfg, siguienteId++, ciclo);
        } else {
            mostrarEspera(pendiente);
        }
        int tiempoAtencion = 0;
        if (rrAtender(cfg.quantum, ciclo, tiempoAtencion)) {
            terminados++;
            sumaAtencion += tiempoAtencion;
        }
        mostrarMemoria();
        sumaOcupacion += memoriaOcupadaKB() * 100.0 / cfg.memoriaKB;

        detener = esperar(cfg.velMs);
    }

    mostrarEstadisticas(ciclo, terminados, sumaAtencion, sumaOcupacion / ciclo);
    return 0;
}