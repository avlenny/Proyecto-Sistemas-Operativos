#include "tipos.h"
#include <iostream>
#include <conio.h>      // _kbhit y _getch (teclado sin bloquear)
#define NOMINMAX        // evita que windows.h rompa std::min
#include <windows.h>    // Sleep
using namespace std;

// Espera velMs milisegundos revisando el teclado cada 20 ms.
// Enter = detener (devuelve true). '+' = mas rapido, '-' = mas lento.
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
    Config cfg = pedirConfig();      // aqui el usuario elige tambien el algoritmo
    mostrarConfig(cfg);
    int alg = cfg.algoritmo;         // 1 = primer ajuste, 2 = peor ajuste

    iniciarMemoria(cfg.memoriaKB);
    rrLimpiar();

    cout << "\nENTER = detener | '+' = mas rapido | '-' = mas lento\n";

    int ciclo = 0, siguienteId = 1, terminados = 0;
    long long sumaAtencion = 0;
    double sumaOcupacion = 0;        // para el % promedio de ocupacion
    bool detener = false;

    Bloque pendiente = genProceso(cfg, siguienteId++, ciclo);   // siempre hay uno listo

    while (!detener) {
        ciclo++;
        mostrarCiclo(ciclo);

        // 1. Entrada a memoria
        if (colocarProceso(pendiente, alg)) {
            pendiente.cicloEntrada = ciclo;
            rrAgregar(pendiente);
            mostrarEntrada(pendiente);
            mostrarMemoria();                                   // como en el PDF
            pendiente = genProceso(cfg, siguienteId++, ciclo);
        } else {
            mostrarEspera(pendiente);
        }

        // 2. Turno de Round Robin (imprime atencion, lista RR y descarga)
        int tiempoAtencion = 0;
        if (rrAtender(cfg.quantum, ciclo, tiempoAtencion)) {
            terminados++;
            sumaAtencion += tiempoAtencion;
        }

        // 3. Estado final del ciclo
        mostrarMemoria();
        sumaOcupacion += memoriaOcupadaKB() * 100.0 / cfg.memoriaKB;

        detener = esperar(cfg.velMs);
    }

    mostrarEstadisticas(ciclo, terminados, sumaAtencion, sumaOcupacion / ciclo);
    return 0;
}