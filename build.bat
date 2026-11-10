@echo off

gcc src/main.c src/floor.c src/camera.c src/object.c src/model.c libs/glad.c -Ilibs/include -Iincludes -Llibs/lib libassimp-6.dll -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -lwinmm -o jogo.exe

if errorLevel 1 (
    echo
    echo ERROR COMPILATION. 
    pause
    exit /b 1
)

echo SUCESS!!