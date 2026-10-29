# Amazon Engineering Challenge: AVL vs Rubro-Negra

## Link do Github

<https://github.com/elektrawanq/amazon-engineering-challenge-avl-rb.git>

## Autoria

- Aluna: Letícia Queiroz Wanderley
  - Email: <leticia.queiroz.109@ufrn.edu.br>

## Introdução

Este repositório contém a solução para o **Amazon Engineering Challenge**, desenvolvido para a disciplina de Estruturas de Dados Básicas II. O projeto consiste na implementação do zero de **Árvores AVL** e **Árvores Rubro-Negras (Red-Black Trees)** em C++, com o objetivo de analisar empiricamente qual estrutura oferece a melhor performance para um cenário logístico de alta volatilidade (milhões de inserções e remoções de IDs de pacotes).

## Funcionalidades Implementadas

- **Árvore AVL:** Balanceamento rígido estrito ($|Fator de Balanceamento| \le 1$), rotações simples (LL, RR) e duplas (LR, RL).
- **Árvore Rubro-Negra:** Balanceamento relaxado por regras de cores, utilizando nó sentinela (`TNULL`) para prevenção de *Segmentation Faults*, tratamento de Tio Vermelho/Preto na inserção e casos de Duplo Preto na remoção.
- **Bateria de Testes Automatizada:** Cenários de inserção/remoção (Crescente, Decrescente, Aleatório), tratamento de chaves inexistentes e duplicadas.
- **Coleta de Métricas:** Rastreamento em tempo real de tempo de execução (ms), altura final da árvore e quantidade exata de rotações geométricas realizadas.
- **Exportação de Dados & Gráficos:** Geração automática de arquivo `.csv` e script Python para renderização de gráficos comparativos.

## Estrutura do Projeto

```text
amazon-engineering-challenge/
├── CMakeLists.txt         # Arquivo de configuração do build system
├── include/
│   ├── AVL_Tree.hpp       # Cabeçalhos da Árvore AVL
│   ├── RB_Tree.hpp        # Cabeçalhos da Árvore Rubro-Negra
│   └── testes.hpp         # Definições da bateria de testes
├── src/
│   ├── AVL_Tree.cpp       # Implementação core da AVL
│   ├── RB_Tree.cpp        # Implementação core da Rubro-Negra
│   └── testes.cpp         # Lógica de execução dos cenários
├── main.cpp               # Ponto de entrada
└── gerador_graficos.py    # Script Python para plotagem de resultados
```

## Pré-requisitos

Para compilar e executar este projeto, você precisará de:

- Compilador C++ com suporte ao padrão **C++17** (g++ ou clang).
- **CMake** (versão 3.10 ou superior).
- **Python 3** com bibliotecas `pandas` e `matplotlib` (apenas para geração dos gráficos).

No Ubuntu/Linux Mint, instale as dependências com:

```bash
sudo apt update && sudo apt install build-essential cmake python3-pandas python3-matplotlib
```

## ⚙️ Como Compilar e Executar (C++)

O projeto utiliza o **CMake** para garantir uma compilação limpa e livre de conflitos. Execute os seguintes comandos na raiz do projeto:

1. Gere os arquivos de build e compile o executável:

```bash
rm -rf build && mkdir build
cd build
cmake ..
make
```

1. Execute a bateria de testes (o executável deve ser rodado a partir da raiz para gerar o CSV corretamente no local esperado):

```bash
cd ..
./build/avlrb
```

Ao final da execução, os resultados dos cenários serão impressos no terminal e consolidados no arquivo `metrics.csv`.

## Como Gerar os Gráficos

Com o arquivo `metrics.csv` gerado após a execução do binário C++, execute o script Python para extrair os gráficos comparativos:

```bash
python3 gerador_graficos.py
```

As imagens em formato `.png` (Tempo vs Rotações vs Carga) serão salvas automaticamente na pasta `graficos_slides/`, prontas para serem anexadas à apresentação.

## Conclusão Arquitetural (Spoiler)

A análise empírica demonstra que, para o cenário da Amazon (alta volatilidade de dados):

- A **AVL** realiza buscas ligeiramente mais rápidas devido à sua altura estritamente menor, porém sofre um *overhead* massivo de rotações durante remoções e inserções em ordem.
- A **Rubro-Negra** é a escolha ideal para o inventário logístico. O seu limite de reestruturação geométrica (máximo de 3 rotações por remoção) garante tempos de atualização de catálogo muito superiores e mais estáveis, compensando a ínfima perda de velocidade na pesquisa.

## Inteligência Artificial

Durante o projeto, foi utilizada Inteligência Artificial para orientação de dúvidas sobre bugs, principalmente de falha de segmentação da memória e na elaboração dos testes - de inserção e de remoção - e por fim, na elaboração do `gerador_graficos.py`.
