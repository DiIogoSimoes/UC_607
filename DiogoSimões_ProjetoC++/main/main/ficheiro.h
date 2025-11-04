#pragma once
#ifndef FICHEIRO_H
#define FICHEIRO_H

#include <vector>
#include <string>
#include "colaborador.h"

// declara funcao para gravar dados no ficheiro TXT
// escreve os nomes cifrados + marcacoes (F ou X)
void gravarDados(const std::string& caminho, const std::vector<Colaborador>& lista);

// declara funcao para carregar dados do ficheiro TXT
// interpreta a estrutura e volta a colocar na lista em memoria
bool carregarDados(const std::string& caminho, std::vector<Colaborador>& lista);

// pequena cifra de Cesar para esconder o nome no ficheiro
// desloca as letras algumas posicoes
std::string cesarCifra(const std::string& s, int desloc);

// imprimir calendario do colaborador
// mostra mes/ano com marcacoes (F, X) e indica fim de semana
void imprimirCalendario(const Colaborador& c, int mes, int ano);

#endif // FICHEIRO_H
