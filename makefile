all:
	make hw8
hw8: hw8.o
	g++ -g -Wall -o hw8 hw8.o
hw8.o: hw8.cpp
	g++ -g -Wall -c hw8.cpp
clean:
	rm *.o hw8
