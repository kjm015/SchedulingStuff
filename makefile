Assign4: Assign4.o Process.o
	g++ -Wall -o Assign4 Assign4.o Process.o
Assign4.o: Assign4.cpp
	g++ -Wall -c Assign4.cpp
Process.o: Process.cpp
	g++ -Wall -c Process.cpp

clean: 
	-rm *.o Assign4
