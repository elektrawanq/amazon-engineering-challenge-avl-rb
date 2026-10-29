#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <string>
#include <fstream>

#include "../include/AVL_Tree.hpp"
#include "../include/RB_Tree.hpp"
#include "../include/teste2.hpp"

using namespace std;
using namespace std::chrono;
using val_tp = int;

// Tempo decorrido em milissegundos com parte fracionária
// (com ms inteiros, operações rápidas como o cenário 5 sempre davam 0)
static double elapsedMs(const high_resolution_clock::time_point& start) {
    return duration<double, milli>(high_resolution_clock::now() - start).count();
}

// Função para gerar dados aleatórios sem duplicatas
vector<val_tp> generateRandomData(val_tp size, bool allow_duplicates) {
    vector<val_tp> data(size);
    mt19937 gen(42);
    uniform_int_distribution<> dist(1, size * 10);

    for (val_tp i = 0; i < size; ++i) {
        data[i] = dist(gen);
    }
    if (!allow_duplicates) {
        sort(data.begin(), data.end());
        data.erase(unique(data.begin(), data.end()), data.end());
        shuffle(data.begin(), data.end(), gen);
    }
    return data;
}

// Função utilitária para imprimir métricas formatadas
void printMetrics(const string& treeName, double timeMs, val_tp rotations, val_tp height) {
    cout << left << setw(4) << treeName
         << " -> Tempo: " << fixed << setprecision(3) << setw(9) << timeMs << "ms | "
         << "Rotações: " << setw(8) << rotations << " | "
         << "Altura Final: " << height << "\n";
    cout.unsetf(ios::fixed);
}

// Para exportar o csv
void exportToCSV(const string& treeName, const string& cenario, val_tp size, double timeMs, val_tp rotations, val_tp height) {
    // Na primeira chamada de cada execução o arquivo é recriado (trunc),
    // evitando linhas repetidas quando o programa é rodado várias vezes.
    // Nas chamadas seguintes (outros tamanhos / cenários) apenas acrescenta.
    static bool first_call = true;
    ofstream file("metrics.csv", first_call ? ios::trunc : ios::app);
    if (first_call) {
        file << "Arvore,Cenario,Tamanho,Tempo_ms,Rotacoes,Altura\n";
        first_call = false;
    }
    file << treeName << "," << cenario << "," << size << ","
         << fixed << setprecision(3) << timeMs << ","
         << rotations << "," << height << "\n";
}

void runTests(val_tp num_elements) {
    cout << "\n==============================================\n";
    cout << "INICIANDO BATERIA DE TESTES - " << num_elements << " ELEMENTOS\n";
    cout << "==============================================\n";

    vector<val_tp> random_data = generateRandomData(num_elements, false);
    vector<val_tp> asc_data = random_data;
    sort(asc_data.begin(), asc_data.end());
    vector<val_tp> desc_data = asc_data;
    reverse(desc_data.begin(), desc_data.end());

    // Tamanho real após remover duplicatas geradas aleatoriamente (pode ser < num_elements)
    const val_tp real_size = static_cast<val_tp>(random_data.size());

    // ==========================================
    // CENÁRIO 1: INSERÇÃO CRESCENTE
    // ==========================================
    cout << "\n> CENÁRIO 1: INSERÇÃO CRESCENTE\n";
    AVL_Tree avl_asc;
    RB_Tree rb_asc;

    auto start = high_resolution_clock::now();
    for (val_tp val : asc_data) avl_asc.insert(val);
    double duration = elapsedMs(start);
    printMetrics("AVL", duration, avl_asc.rotation_count, avl_asc.get_height());
    exportToCSV("AVL", "Insercao_Crescente", num_elements, duration, avl_asc.rotation_count, avl_asc.get_height());

    start = high_resolution_clock::now();
    for (val_tp val : asc_data) rb_asc.insert(val);
    duration = elapsedMs(start);
    printMetrics("RB", duration, rb_asc.rotation_count, rb_asc.get_height());
    exportToCSV("RB", "Insercao_Crescente", num_elements, duration, rb_asc.rotation_count, rb_asc.get_height());

    // ==========================================
    // CENÁRIO 2: INSERÇÃO DECRESCENTE
    // ==========================================
    cout << "\n> CENÁRIO 2: INSERÇÃO DECRESCENTE\n";
    AVL_Tree avl_desc;
    RB_Tree rb_desc;

    start = high_resolution_clock::now();
    for (val_tp val : desc_data) avl_desc.insert(val);
    duration = elapsedMs(start);
    printMetrics("AVL", duration, avl_desc.rotation_count, avl_desc.get_height());
    exportToCSV("AVL", "Insercao_Decrescente", num_elements, duration, avl_desc.rotation_count, avl_desc.get_height());

    start = high_resolution_clock::now();
    for (val_tp val : desc_data) rb_desc.insert(val);
    duration = elapsedMs(start);
    printMetrics("RB", duration, rb_desc.rotation_count, rb_desc.get_height());
    exportToCSV("RB", "Insercao_Decrescente", num_elements, duration, rb_desc.rotation_count, rb_desc.get_height());

    // ==========================================
    // CENÁRIO 3: INSERÇÃO ALEATÓRIA
    // ==========================================
    cout << "\n> CENÁRIO 3: INSERÇÃO ALEATÓRIA\n";
    AVL_Tree avl_rand;
    RB_Tree rb_rand;

    start = high_resolution_clock::now();
    for (val_tp val : random_data) avl_rand.insert(val);
    duration = elapsedMs(start);
    printMetrics("AVL", duration, avl_rand.rotation_count, avl_rand.get_height());
    // CORREÇÃO: este cenário não estava sendo exportado para o CSV
    exportToCSV("AVL", "Insercao_Aleatoria", num_elements, duration, avl_rand.rotation_count, avl_rand.get_height());

    start = high_resolution_clock::now();
    for (val_tp val : random_data) rb_rand.insert(val);
    duration = elapsedMs(start);
    printMetrics("RB", duration, rb_rand.rotation_count, rb_rand.get_height());
    exportToCSV("RB", "Insercao_Aleatoria", num_elements, duration, rb_rand.rotation_count, rb_rand.get_height());

    // ==========================================
    // CENÁRIO 4: INSERÇÃO DE ELEMENTOS REPETIDOS
    // (Usando as árvores do cenário aleatório)
    // ==========================================
    cout << "\n> CENÁRIO 4: INSERÇÃO DE DUPLICATAS (10% dos elementos)\n";
    avl_rand.rotation_count = 0; rb_rand.rotation_count = 0; // Reset contadores
    // CORREÇÃO: baseado no tamanho real do vetor, evitando acesso fora dos limites
    val_tp num_duplicates = static_cast<val_tp>(real_size * 0.10);

    start = high_resolution_clock::now();
    for (val_tp i = 0; i < num_duplicates; ++i) avl_rand.insert(random_data[i]);
    duration = elapsedMs(start);
    printMetrics("AVL", duration, avl_rand.rotation_count, avl_rand.get_height());
    exportToCSV("AVL", "Insercao_de_Duplicatas", num_elements, duration, avl_rand.rotation_count, avl_rand.get_height());

    start = high_resolution_clock::now();
    for (val_tp i = 0; i < num_duplicates; ++i) rb_rand.insert(random_data[i]);
    duration = elapsedMs(start);
    printMetrics("RB", duration, rb_rand.rotation_count, rb_rand.get_height());
    exportToCSV("RB", "Insercao_de_Duplicatas", num_elements, duration, rb_rand.rotation_count, rb_rand.get_height());

    // ==========================================
    // CENÁRIO 5: REMOÇÃO DE INEXISTENTES
    // ==========================================
    cout << "\n> CENÁRIO 5: REMOÇÃO DE ELEMENTOS INEXISTENTES (-1 a -100)\n";
    avl_rand.rotation_count = 0; rb_rand.rotation_count = 0;

    start = high_resolution_clock::now();
    for (val_tp i = -1; i >= -100; --i) avl_rand.remove(i);
    duration = elapsedMs(start);
    printMetrics("AVL", duration, avl_rand.rotation_count, avl_rand.get_height());
    // CORREÇÃO: nomes de cenário sem acento (nomes de arquivo/gráficos mais seguros)
    exportToCSV("AVL", "Remocao_de_Inexistentes", num_elements, duration, avl_rand.rotation_count, avl_rand.get_height());

    start = high_resolution_clock::now();
    for (val_tp i = -1; i >= -100; --i) rb_rand.remove(i);
    duration = elapsedMs(start);
    printMetrics("RB", duration, rb_rand.rotation_count, rb_rand.get_height());
    exportToCSV("RB", "Remocao_de_Inexistentes", num_elements, duration, rb_rand.rotation_count, rb_rand.get_height());

    // ==========================================
    // CENÁRIO 6: REMOÇÃO ALEATÓRIA (50% dos dados)
    // ==========================================
    cout << "\n> CENÁRIO 6: REMOÇÃO ALEATÓRIA (50% da carga)\n";
    avl_rand.rotation_count = 0; rb_rand.rotation_count = 0;
    val_tp half_elements = real_size / 2;

    start = high_resolution_clock::now();
    for (val_tp i = 0; i < half_elements; ++i) avl_rand.remove(random_data[i]);
    duration = elapsedMs(start);
    printMetrics("AVL", duration, avl_rand.rotation_count, avl_rand.get_height());
    exportToCSV("AVL", "Remocao_Aleatoria", num_elements, duration, avl_rand.rotation_count, avl_rand.get_height());

    start = high_resolution_clock::now();
    for (val_tp i = 0; i < half_elements; ++i) rb_rand.remove(random_data[i]);
    duration = elapsedMs(start);
    printMetrics("RB", duration, rb_rand.rotation_count, rb_rand.get_height());
    exportToCSV("RB", "Remocao_Aleatoria", num_elements, duration, rb_rand.rotation_count, rb_rand.get_height());
}