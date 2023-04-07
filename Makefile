compiler = g++

huffmanEncoding: main.o huffmanEncoding.o
	$(compiler) main.o huffmanEncoding.o -o huffmanEncoding

main.o: src/main.cpp
	$(compiler) -c src/main.cpp

huffmanEncoding.o: src/huffmanEncoding.cpp src/huffmanEncoding.h
	$(compiler) -c src/huffmanEncoding.cpp

clean:
	rm *.o huffmanEncoding *.txt
