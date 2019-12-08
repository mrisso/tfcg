all: full

full: arena.o tf.o imageloader.o
	g++ arena.o tf.o imageloader.o -lGL -lGLU -lglut -lm -o trabalhocg

tf.o:
	g++ -c tf.cpp -o tf.o -std=c++11

arena.o:
	g++ -c arena.cpp -o arena.o -std=c++11

imageloader.o:
	g++ -c imageloader.cpp -o imageloader.o -std=c++11

tiny.o:
	g++ -c tinyxml2.cpp

clean:
	rm -f *.o trabalhocg
