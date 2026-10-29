#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>

#include "../include/AVL_Tree.hpp"
#include "../include/RB_Tree.hpp"
#include "../include/testes.hpp"

using namespace std;
using namespace std::chrono;

int main() {
    // Executar para diferentes magnitudes de dados para ter gráficos na apresentação
    vector<int> sizes = {1000, 10000, 100000};
    for (int size : sizes) {
        runTests(size);
    }
    return 0;
}