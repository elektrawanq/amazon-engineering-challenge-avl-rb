#ifndef TESTES_HPP
#define TESTES_HPP
#include <string>
#include <vector>

using namespace std;
using val_tp = int;

// Estrutura para armazenar métricas
struct Metrics {
    double duration_ms;
    int rotations;
    int final_height;
};

// Função para gerar dados aleatórios (evitando duplicatas indesejadas, mas permitindo se necessário)
std::vector<int> generateRandomData(val_tp size, bool allow_duplicates = false);

// Função utilitária para imprimir métricas formatadas
void printMetrics(const string& treeName, double timeMs, val_tp rotations, val_tp height);

// Exporta uma linha para metrics.csv (o arquivo é recriado na primeira chamada de cada execução)
void exportToCSV(const string& treeName, const string& cenario, val_tp size, double timeMs, val_tp rotations, val_tp height);

void runTests(int num_elements);

#endif