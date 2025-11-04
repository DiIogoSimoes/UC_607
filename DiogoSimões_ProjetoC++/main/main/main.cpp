#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "colaborador.h"
#include "ficheiro.h"

using namespace std;

int main() {
    // lista de colaboradores em memoria
    vector<Colaborador> colaboradores;

    // nome do ficheiro onde vamos guardar / ler os dados
    const string ficheiro = "diogo_data.txt";

    // carregar dados do ficheiro se existir
    if (carregarDados(ficheiro, colaboradores))
        cout << "Dados carregados de '" << ficheiro << "'.\n";
    else
        cout << "Nenhum ficheiro encontrado. Base de dados vazia.\n";

    int opc = -1; // opcao do menu

    // ciclo principal do menu
    do {
        cout << "\n--- MiniRH (Diogo) ---\n"
            "1) Adicionar colaborador\n"
            "2) Listar\n"
            "3) Marcar ferias (F)\n"
            "4) Marcar falta (X)\n"
            "5) Desmarcar\n"
            "6) Ver calendario\n"
            "0) Sair e gravar\n"
            "Escolha: ";

        // validar leitura de numero
        if (!(cin >> opc)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // 1) adicionar colaborador
        if (opc == 1) {
            Colaborador c;
            cout << "Nome completo: "; getline(cin, c.nome);

            // verificar se ja existe
            auto it = find_if(colaboradores.begin(), colaboradores.end(),
                [&](const Colaborador& x) { return x.nome == c.nome; });

            if (it != colaboradores.end()) {
                cout << "Ja existe um colaborador com esse nome. Confirmar duplicacao (s/n)? ";
                char r; cin >> r; cin.ignore();
                if (r == 's' || r == 'S') colaboradores.push_back(c);
            }
            else colaboradores.push_back(c);
        }

        // 2) listar colaboradores
        else if (opc == 2) {
            cout << "\nLista de colaboradores:\n";
            for (const auto& c : colaboradores)
                cout << " - " << c.nome << "\n";
        }

        // 3,4,5) marcar ferias/falta / desmarcar
        else if (opc >= 3 && opc <= 5) {
            string nome; cout << "Nome do colaborador: "; getline(cin, nome);
            auto it = find_if(colaboradores.begin(), colaboradores.end(),
                [&](const Colaborador& x) { return x.nome == nome; });

            if (it == colaboradores.end()) {
                cout << "Colaborador nao encontrado.\n";
                continue;
            }

            // pedir data
            int d, m, a;
            cout << "Dia: "; if (!(cin >> d)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida\n";
                continue;
            }
            cout << "Mes: "; cin >> m;
            cout << "Ano: "; cin >> a;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // validar data e fim de semana
            if (!validar_data(d, m, a)) { cout << "Data invalida.\n"; continue; }
            if (eh_fim_de_semana(d, m, a)) { cout << "Nao e permitido marcar ao fim-de-semana.\n"; continue; }

            // formatar data
            string key = to_string(a) + "-" + (m < 10 ? "0" : "") + to_string(m)
                + "-" + (d < 10 ? "0" : "") + to_string(d);

            // 5) desmarcar
            if (opc == 5) {
                it->marcacoes.erase(key);
                cout << "Marcacao removida.\n";
            }
            else {
                // marcar F ou X
                char tipo = (opc == 3) ? 'F' : 'X';
                it->marcacoes[key] = tipo;
                cout << "Marcado " << tipo << " em " << key << ".\n";
            }
        }

        // 6) imprimir calendario
        else if (opc == 6) {
            string nome; cout << "Nome: "; getline(cin, nome);
            auto it = find_if(colaboradores.begin(), colaboradores.end(),
                [&](const Colaborador& x) { return x.nome == nome; });

            if (it == colaboradores.end()) { cout << "Colaborador nao encontrado.\n"; continue; }
            int mes, ano; cout << "Mes (1-12): "; cin >> mes; cout << "Ano: "; cin >> ano; cin.ignore();

            // apresenta calendario do colaborador
            imprimirCalendario(*it, mes, ano);
        }

    } while (opc != 0); // ciclo menu acaba quando opc == 0

    // guardar dados antes de sair
    gravarDados(ficheiro, colaboradores);
    cout << "Dados guardados em '" << ficheiro << "'. Adeus!\n";
    return 0;
}
