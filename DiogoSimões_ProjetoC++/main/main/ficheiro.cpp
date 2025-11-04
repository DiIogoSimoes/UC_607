#include "ficheiro.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>

using namespace std;

// funcao que aplica cifra de Cesar a uma string
// desloca letras X posicoes no alfabeto
string cesarCifra(const string& s, int desloc) {
    string r = s;
    for (char& ch : r) {
        unsigned char u = static_cast<unsigned char>(ch);
        if (isalpha(u)) { // se for letra
            bool up = isupper(u);  // verifica se e maiuscula ou minuscula
            char base = up ? 'A' : 'a';
            // aplica o deslocamento circular (alfabeto)
            ch = static_cast<char>(base + ((u - base + desloc + 26) % 26));
        }
    }
    return r;
}

// grava os dados dos colaboradores num ficheiro TXT
// - o nome vai cifrado
// - cada marcacao e gravada no formato YYYY-MM-DD;X
void gravarDados(const string& caminho, const vector<Colaborador>& lista) {
    ofstream ofs(caminho);
    if (!ofs) { cerr << "Erro: nao foi possivel abrir ficheiro para gravar.\n"; return; }

    for (const auto& c : lista) {
        // grava nome cifrado
        ofs << cesarCifra(c.nome, 4) << "\n";

        // grava cada dia marcado
        for (const auto& p : c.marcacoes)
            ofs << p.first << ";" << p.second << "\n";

        // separador de colaborador
        ofs << "===\n";
    }
}

// le o ficheiro TXT e reconstrui os colaboradores e marcacoes em memoria
bool carregarDados(const string& caminho, vector<Colaborador>& lista) {
    ifstream ifs(caminho);
    if (!ifs) return false; // se nao existir ficheiro, retorna false

    string linha;
    Colaborador cur;
    bool temNome = false;

    while (getline(ifs, linha)) {

        // "===" separa colaboradores
        if (linha == "===") {
            if (temNome) lista.push_back(cur);
            cur = Colaborador{};
            temNome = false;
        }
        // linha sem ';' e o nome
        else if (linha.find(';') == string::npos) {
            cur.nome = cesarCifra(linha, -4); // descifra o nome
            temNome = true;
        }
        // linha com ';' e uma marcacao (data;tipo)
        else {
            size_t p = linha.find(';');
            string data = linha.substr(0, p);
            char t = linha[p + 1];
            cur.marcacoes[data] = t;
        }
    }

    if (temNome) lista.push_back(cur);
    return true;
}

// imprime o calendario mensal com marcacoes
void imprimirCalendario(const Colaborador& c, int mes, int ano) {
    cout << "\nCalendario de " << c.nome << " (" << mes << "/" << ano << ")\n";
    cout << "Su  Mo  Tu  We  Th  Fr  Sa\n";

    // calcular em que dia da semana comeca o mes
    tm first = { 0,0,0,1, mes - 1, ano - 1900 };
    mktime(&first);
    int start = first.tm_wday; // 0=Domingo

    // numero de dias de cada mes
    int diasMes[] = { 31, ehBissexto(ano) ? 29 : 28, 31,30,31,30,31,31,30,31,30,31 };
    int dias = diasMes[mes - 1];
    int col = 0;

    // imprime espacos ate ao dia de inicio
    for (int i = 0; i < start; i++) { cout << "    "; col++; }

    // ciclo pelos dias
    for (int d = 1; d <= dias; ++d) {
        // construir chave YYYY-MM-DD
        string key = to_string(ano) + "-" + (mes < 10 ? "0" : "") + to_string(mes) + "-" + (d < 10 ? "0" : "") + to_string(d);

        // verifica se ha marcacao nesse dia
        char mark = ' ';
        auto it = c.marcacoes.find(key);
        if (it != c.marcacoes.end()) mark = it->second;

        // verifica se fim de semana
        tm t = { 0,0,0,d,mes - 1,ano - 1900 }; mktime(&t);
        bool fimsem = (t.tm_wday == 0 || t.tm_wday == 6);

        // imprime o dia com simbolo
        if (fimsem) {
            if (mark == ' ') cout << setw(2) << d << "* ";
            else             cout << setw(2) << d << mark << " ";
        }
        else {
            if (mark == ' ') cout << setw(3) << d << " ";
            else             cout << setw(2) << d << mark << " ";
        }

        col++;
        if (col % 7 == 0) cout << "\n"; // quebra de linha
    }

    cout << "\nLegenda: F=Ferias | X=Falta | *=Fim-de-semana\n";
}
