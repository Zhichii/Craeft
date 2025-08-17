g++ -c utf8.cpp -std=c++20 -o Build\utf8.o
g++ -c json.cpp -std=c++20 -o Build\json.o
g++ -c thing.cpp -std=c++20 -o Build\thing.o
g++ -c mod.cpp -std=c++20 -o Build\mod.o
g++ -c chunk.cpp -std=c++20 -o Build\chunk.o
g++ -c main.cpp -std=c++20 -o Build\main.o
g++ ..\json\json_reader.o ..\json\json_value.o ..\json\json_writer.o Build\utf8.o Build\thing.o Build\mod.o Build\chunk.o Build\main.o -o CraeftServer.exe
