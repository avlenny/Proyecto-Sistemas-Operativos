#include "tipos.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <deque>
using namespace std;

// 
Config pedirConfig();
void mostrarConfig(const Config& cfg);
Bloque genProceso(const Config& cfg, int id, int ciclo);

// ---- planificador.cpp ----
void rrAgregar(const Bloque& p);
const deque<Bloque>& rrLista();
void rrLimpiar();
bool rrAtender(int quantum, int ciclo, int& tiempoAtencion);

// ---- Módulo de memoria : AJUSTAR NOMBRES SI DIFIEREN ----
// alg: 1 = primer ajuste, 2 = peor ajuste. Devuelve false si no cabe.
void iniciarMemoria(int memoriaKB);
bool colocarProceso(const Bloque& p, int alg);
void descargarProceso(int id);   // debe condensar huecos
void imprimirMemoria();          // imprime [id,tam,cuanto][...]

static int pedirAlgoritmo() {
    while (true) {
        cout << "Algoritmo de colocacion:\n1) Primer ajuste\n2) Peor ajuste\n[1-2] (Enter = 1): ";
        string s;
        if (!getline(cin, s) || s.empty() || s == "1") return 1;
        if (s == "2") return 2;
        cout << "  Valor invalido, intenta de nuevo\n";
    }
}

static void imprimirListaRR() {
    cout << "Lista RR";
    for (const auto& p : rrLista())
        cout << " [" << p.id << "," << p.tam << "," << p.cuanto << "]";
    cout << "\n";
}

int main() {
    Config cfg = pedirConfig();
    int alg = pedirAlgoritmo();
    mostrarConfig(cfg);
    cout << "Algoritmo: " << (alg == 1 ? "Primer ajuste" : "Peor ajuste") << "\n";

    iniciarMemoria(cfg.memoriaKB);
    rrLimpiar();

    // Condición de paro: el usuario presiona Enter
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
