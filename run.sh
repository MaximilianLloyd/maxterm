gcc -o sdl_program main.c `sdl2-config --cflags --libs` -lSDL2_ttf
chmod +x sdl_program
./sdl_program
