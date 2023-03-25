run:./main.cpp
	g++ -c ./main.cpp
	g++ main.o -o app -lsfml-graphics -lsfml-window -lsfml-system -lGL
	./app	

clean:
	rm -rf *.o app