#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Surface *surface;
  SDL_Texture *texture;
  SDL_Color textColor = {255, 255, 255, 0};
  SDL_Event event;

  int quit = 0;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Could not initialize sdl2: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  if (TTF_Init() == -1) {
    fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }

  // Create an SDL window
  window =
      SDL_CreateWindow("SDL2 Simple Window", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  // Create a renderer
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (renderer == NULL) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    return EXIT_FAILURE;
  }

  if (window == NULL) {
    fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  TTF_Font *font = TTF_OpenFont("/home/max/C/maxterm/font.ttf", 24);

  if (font == NULL) {
    fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }

  // Create a surface with the text
  surface = TTF_RenderText_Solid(font, "Hello, World!", textColor);
  if (surface == NULL) {
    fprintf(stderr, "TTF_RenderText_Solid Error: %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }

  // Convert the surface to texture
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (texture == NULL) {
    fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  char text[2000] = "H";
  int idx = 0;

  // Listen for keys
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        quit = 1;
        break;
      case SDL_TEXTINPUT:
        // Replace current text with input text
        printf("Text input: %s\n", text);
        text[idx] = event.text.text[0];
        idx++;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_BACKSPACE:
          // Handle backspace
          if (idx > 0) {
            idx--;
            text[idx] = '\0';
          }
          break;
        case SDLK_RETURN: {
          FILE *fp;
          char path[1035];

          /* Open the command for reading. */
          fp = popen("/bin/ls", "r");
          if (fp == NULL) {
            printf("Failed to run command\n");
            exit(1);
          }

          /* Read the output a line at a time - output it. */
          while (fgets(path, sizeof(path) - 1, fp) != NULL) {
            // add to text
            strncat(text, path, sizeof(text) - strlen(text) - 1);
            printf("%s", path);
          }

          /* close */
          pclose(fp);

          printf("Entered: %s\n", text);
          break;
        }
        case SDLK_q:
          quit = 1;
          break;
        default:
          break;
        }
        break;
      default:
        break;
      }
    }

    if (strlen(text) > 0) {
      surface = TTF_RenderText_Shaded(font, text, textColor, (SDL_Color){50, 0, 0, 0});
      if (surface == NULL) {
        fprintf(stderr, "TTF_RenderText_Solid Error: %s\n", TTF_GetError());
        return EXIT_FAILURE;
      }
    } else {
      surface = TTF_RenderText_Solid(font, " ", textColor);
      if (surface == NULL) {
        fprintf(stderr, "TTF_RenderText_Solid Error: %s\n", TTF_GetError());
        return EXIT_FAILURE;
      }
    }

    // Convert the surface to texture
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
      fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n",
              SDL_GetError());
      return EXIT_FAILURE;
    }

    SDL_RenderClear(renderer);

    SDL_Rect texRect = {10, 10, surface->w, surface->h}; // top-left at (10, 10)
    SDL_RenderCopy(renderer, texture, NULL, &texRect);
    SDL_RenderPresent(renderer);

    // Draw new text
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
  }

  // Draw to the window

  // Clean up
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
  TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return EXIT_SUCCESS;
}
