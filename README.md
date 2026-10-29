# amazon-engineering-challenge-avl-rb

## Autoria

- Aluna: Letícia Queiroz Wanderley
  - Email: <leticia.queiroz.109@ufrn.edu.br>

## Link do GitHub do projeto

amazon-engineering-challenge-avl-rb

## Compilação e Execução

```bash

# Gerando a pasta build
cmake -S . -B build

cd build

# Compilando
cmake --build build

# Executando
./avlrb

# 1. Apaga a pasta build velha (que causou o erro de múltiplos mains) e cria uma nova
rm -rf build && mkdir build

# 2. Entra na pasta, configura o projeto e compila
cd build
cmake ..
make

# 3. Volta para a raiz do projeto e executa o binário (para o CSV ser gerado na raiz)
cd ..
./build/avlrb

```