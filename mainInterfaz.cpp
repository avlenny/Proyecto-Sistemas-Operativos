#include "tipos.h"
#include "interfaz.h"
#include "estadisticas.h"
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void rrAgregar(const Bloque& p);
const deque<Bloque>& rrLista();
void rrLimpiar();
bool rrAtender(int quantum, int ciclo, int& tiempoAtencion);

void iniciarMemoria(int memoriaKB);
bool colocarProceso(const Bloque& p, int alg);
void descargarProceso(int id);
void imprimirMemoria();

class AtraparCout {
    ostringstream buffer;
    streambuf* original;
public:
    AtraparCout()  { original = cout.rdbuf(buffer.rdbuf()); }
    ~AtraparCout() { cout.rdbuf(original); }
    string texto() const { return buffer.str(); }
};

static vector<Bloque> leerMemoria() {
    string t;
    {
        AtraparCout trampa;
        imprimirMemoria();
        t = trampa.texto();
    }

    vector<Bloque> lista;
    size_t pos = 0;
    while ((pos = t.find('[', pos)) != string::npos) {
        int id = 0, tam = 0, cuanto = 0;
        char c1 = 0, c2 = 0, c3 = 0, c4 = 0;
        istringstream in(t.substr(pos));
        if (in >> c1 >> id >> c2 >> tam >> c3 >> cuanto >> c4 &&
            c1 == '[' && c2 == ',' && c3 == ',' && c4 == ']')
            lista.push_back(Bloque{id, tam, cuanto, 0});
        pos++;
    }
    return lista;
}

static void agregarLineas(vector<string>& eventos, const string& t) {
    istringstream in(t);
    string linea;
    while (getline(in, linea))
        if (!linea.empty()) eventos.push_back(linea);
}

int main() {
    Config cfg = pedirConfig();
    mostrarConfig(cfg);
    cout << "\nPresiona ENTER para iniciar...\n";
    cin.get();

    Control ctl;
    ctl.velocidadMs = cfg.velMs;
    Estadisticas est;

    iniciarMemoria(cfg.memoriaKB);
    rrLimpiar();

    int ciclo = 0, siguienteId = 1;

    Bloque pendiente = genProceso(cfg, siguienteId++, ciclo);

    iniciarInterfaz();

    while (!ctl.parar) {
        ciclo++;
        vector<string> eventos;

        if (colocarProceso(pendiente, cfg.algoritmo)) {
            pendiente.cicloEntrada = ciclo;
            registrarCreado(est, pendiente, cfg.quantum);
            rrAgregar(pendiente);
            eventos.push_back("Entra P" + to_string(pendiente.id) + " (" +
                              to_string(pendiente.tam) + " KB, cuanto " +
                              to_string(pendiente.cuanto) + ")");
            // ...y generamos el siguiente (asi siempre hay uno listo).
            pendiente = genProceso(cfg, siguienteId++, ciclo);
        } else {
            registrarSinEspacio(est);
            eventos.push_back("P" + to_string(pendiente.id) + " (" +
                              to_string(pendiente.tam) + " KB) espera: no cabe");
        }

        int idAtendido = rrLista().empty() ? 0 : rrLista().front().id;
        int tiempoAtencion = 0;
        bool termino;
        {
            AtraparCout trampa;
            termino = rrAtender(cfg.quantum, ciclo, tiempoAtencion);
            agregarLineas(eventos, trampa.texto());
        }

        if (termino) registrarTerminado(est, idAtendido, ciclo);

        vector<Bloque> memoria = leerMemoria();
        registrarTick(est, memoria, cfg.memoriaKB);

        limpiarPantalla();
        mostrarControles(ctl, ciclo);
        mostrarMemoria(memoria, cfg.memoriaKB);
        mostrarColaRR(rrLista(), cfg.quantum);
        mostrarEventos(eventos);

        esperarTick(ctl);
    }

    cerrarInterfaz();
    mostrarEstadisticas(est);
    return 0;
}
