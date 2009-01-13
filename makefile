all: newWave.o bobWave.o
	g++ -o bobWave.exe newWave.o bobWave.o

newWave.o: newWave.cpp newWave.h
	g++ -c -g newWave.cpp
bobWave.o: bobWave.cpp
	   g++ -c -g bobWave.cpp
clean:
	rm -f *.o *.exe