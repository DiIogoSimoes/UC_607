#ifndef COLABORADOR_H
#define COLABORADOR_H

#include <string>
#include <map>

struct Colaborador {
    std::string nome;
    // "YYYY-MM-DD" -> 'F' (férias) ou 'X' (falta)
    std::map<std::string, char> marcacoes;
};

bool ehBissexto(int ano);
bool validar_data(int dia, int mes, int ano);
bool eh_fim_de_semana(int dia, int mes, int ano);

#endif // COLABORADOR_H
