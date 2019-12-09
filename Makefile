all: g++
	@echo "OK"

clean:
	@rm -f *.o* .output trabalhocg
	@echo "OK"

g++:
	g++ tf.cpp imageloader.cpp tinyxml2.cpp base.cpp nave.cpp arena.cpp pista.cpp projetil.cpp controle.cpp camera.cpp -o trabalhocg -lGL -lGLU -lglut -lm
