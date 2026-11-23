#include "tipos.h"
#include <iostream>
#include <string>
#include <deque>
#include <conio.h>      // _kbhit y _getch (teclado sin bloquear)
#define NOMINMAX        // evita que windows.h rompa std::min
#include <windows.h>    // Sleep
using namespace std;

// ---- planificador.cpp ----
void rrAgregar(const Bloque& p);
const deque<Bloque>& rrLista();
void rrLimpiar();
bool rrAtender(int quantum, int ciclo, int& tiempoAtencion);

// ---- memoria.cpp (Primer y Peor ajuste) ----
void iniciarMemoria(int memoriaKB);
bool colocarProceso(const Bloque& p, int alg);
void descargarProceso(int id);
void imprimirMemoria();

static void imprimirListaRR() {
    cout << "Lista RR";
    for (const auto& p : rrLista())
        cout << " [" << p.id << "," << p.tam << "," << p.cuanto << "]";
    cout << "\n";
}

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
    Config cfg = pedirConfig();
    mostrarConfig(cfg);
    int alg = cfg.algoritmo;

    iniciarMemoria(cfg.memoriaKB);
    rrLimpiar();

    cout << "\nENTER = detener | '+' = mas rapido | '-' = mas lento\n\n";

    int ciclo = 0, siguienteId = 1, terminados = 0;
    long long sumaAtencion = 0;
    bool detener = false;
    Bloque pendiente = genProceso(cfg, siguienteId++, ciclo);

    while (!detener) {
        ciclo++;
        cout << "\n===== CICLO " << ciclo << " =====\n";

        // 1. Entrada a memoria
        if (colocarProceso(pendiente, alg)) {
            pendiente.cicloEntrada = ciclo;
            rrAgregar(pendiente);
            cout << "Entra Proceso [" << pendiente.id << "," << pendiente.tam
                 << "," << pendiente.cuanto << "]\n";
            pendiente = genProceso(cfg, siguienteId++, ciclo);
        } else {
            cout << "Proceso [" << pendiente.id << "," << pendiente.tam
                 << "," << pendiente.cuanto << "] espera: no cabe\n";
        }

        // 2. Turno de Round Robin
        int tiempoAtencion = 0;
        if (rrAtender(cfg.quantum, ciclo, tiempoAtencion)) {
            terminados++;
            sumaAtencion += tiempoAtencion;
        }

        // 3. Estado del ciclo
        imprimirListaRR();
        imprimirMemoria();
        cout << "(velocidad: " << cfg.velMs << " ms por ciclo)\n";

        detener = esperar(cfg.velMs);
    }

    cout << "\n--- Estadisticas ---\n"
         << "Ciclos ejecutados: " << ciclo << "\n"
         << "Procesos atendidos: " << terminados << "\n";
    if (terminados > 0)
        cout << "Tiempo medio de atencion: "
             << (double)sumaAtencion / terminados << " ciclos\n";

    return 0;
}