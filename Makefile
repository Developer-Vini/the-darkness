SRC = src/main.c src/floor.c src/camera.c src/object.c src/model.c libs/glad.c


jogo.exe: $(SRC) $(wildcard includes/*.h)
	gcc: $(SRC) -Ilibs/include -Iincludes -Llibs/lib libassimp-6.dll -lglfw3 -lopengl32 -lgdi32 -luser3222 -lkernel32 -lwinmm -o jogo.exe