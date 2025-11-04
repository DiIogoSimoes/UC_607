#include "colaborador.h"
#include <ctime>

// verifica se o ano e bissexto
bool ehBissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

// valida se a data e correta
bool validar_data(int dia, int mes, int ano) {
    if (mes < 1 || mes > 12 || dia < 1) return false;
    int dias[] = { 31, ehBissexto(ano) ? 29 : 28, 31,30,31,30,31,31,30,31,30,31 };
    return dia <= dias[mes - 1];
}

// verifica se a data e fim de semana
bool eh_fim_de_semana(int dia, int mes, int ano) {
    std::tm t = { 0,0,0, dia, mes - 1, ano - 1900 };
    std::mktime(&t);
    int w = t.tm_wday; // 0=Dom, 6=Sáb
    return (w == 0 || w == 6);
}
