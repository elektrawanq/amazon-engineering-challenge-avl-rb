#ifndef TESTES_HPP
#define TESTES_HPP
#include <string>
#include <vector>

using namespace std;
using val_tp = int;

// Estrutura para armazenar métricas
struct Metrics {
    long long duration_ms;
    int rotations;
    int final_height;
};

// Função para gerar dados aleatórios (evitando duplicatas indesejadas, mas permitindo se necessário)
std::vector<int> generateRandomData(val_tp size, bool allow_duplicates = false);

// Função utilitária para imprimir métricas formatadas
void printMetrics(const string& treeName, long long timeMs, val_tp rotations, val_tp height);

void exportToCSV(const string& treeName, const string& cenario, val_tp size, long long timeMs, val_tp rotations, val_tp height);

void runTests(int num_elements);

#endif