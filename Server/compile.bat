g++ -c chunk.cpp -std=c++17 -o Build/chunk.o
g++ -c main.cpp -std=c++17 -o Build/main.o
g++ Build/chunk.o Build/main.o -o CraeftServer.exe