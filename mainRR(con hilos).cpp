#include "tipos.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <deque>
using namespace std;

// ---- planificador.cpp ----
void rrAgregar(const Bloque& p);
const deque<Bloque>& rrLista();
void rrLimpiar();
bool rrAtender(int quantum, int ciclo, int& tiempoAtencion);

// ---- memoria.cpp (Primer y Peor ajuste) ----
// alg: 1 = primer ajuste, 2 = peor ajuste. colocarProceso devuelve false si no cabe.
void iniciarMemoria(int memoriaKB);
bool colocarProceso(const Bloque& p, int alg);
void descargarProceso(int id);   // libera el bloque y condensa huecos
void imprimirMemoria();          // imprime [id,tam,cuanto][...]

static void imprimirListaRR() {
    cout << "Lista RR";
    for (const auto& p : rrLista())
        cout << " [" << p.id << "," << p.tam << "," << p.cuanto << "]";
    cout << "\n";
}

int main() {
    Config cfg = pedirConfig();      // aqui el usuario elige tambien el algoritmo
    mostrarConfig(cfg);
    int alg = cfg.algoritmo;         // 1 = primer ajuste, 2 = peor ajuste

    iniciarMemoria(cfg.memoriaKB);
    rrLimpiar();

    // Condicion de paro: el usuario presiona Enter
    atomic<bool> detener(false);
    cout << "\nPresiona ENTER para detener la simulacion.\n\n";
    thread hilo([&detener]() {
        string s;
        getline(cin, s);
        detener = true;
    });
    hilo.detach();
    int ciclo = 0, siguienteId = 1, terminados = 0;
    long long sumaAtencion = 0;
    Bloque pendiente = genProceso(cfg, siguienteId++, ciclo);   // siempre hay uno listo
    while (!detener) {
        ciclo++;
        cout << "\n===== CICLO " << ciclo << " =====\n";

        // 1. Entrada a memoria
        if (colocarProceso(pendiente, alg)) {
            pendiente.cicloEntrada = ciclo;   // ciclo en que entro a memoria
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
        this_thread::sleep_for(chrono::milliseconds(cfg.velMs));
    }

    cout << "\n--- Estadisticas ---\n"
         << "Ciclos ejecutados: " << ciclo << "\n"
         << "Procesos atendidos: " << terminados << "\n";
    if (terminados > 0)
        cout << "Tiempo medio de atencion: " << (double)sumaAtencion / terminados << " ciclos\n";

    return 0;
}