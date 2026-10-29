CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -I./include

# Busca os .cpp, mas exclui as pastas de build do CMake para evitar duplo main()
SRCS = $(shell find . -name "*.cpp" ! -path "*/build/*" ! -path "*/CMakeFiles/*")
TARGET = amazon_challenge

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) metrics.csv