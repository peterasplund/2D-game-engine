#include <stdio.h>
#include <iostream>

int main() {
  printf("hej\n");
  std::cout << "test" << std::endl;
  return 0;
}

/*
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;
const int WINDOW_FPS    = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / WINDOW_FPS;

typedef struct Renderable {
  SDL_Texture* texture;
  SDL_Surface* surface;
  SDL_Rect*    rectangle;
} Renderable;

void cleanup(SDL_Renderer* renderer, SDL_Window* window) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int initializeRenderable(Renderable* renderable, SDL_Renderer* renderer, char* path) {
  SDL_Surface* surface = IMG_Load(path);

  if (!surface) {
    printf("error creating surface\n");
    return 1;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  if (!texture) {
    printf("error creating texture: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Rect rectangle = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 32, 32};

  renderable->surface   = surface;
  renderable->texture   = texture;
  renderable->rectangle = &rectangle;

  return 0;
}

int renderRenderable(Renderable* renderable, SDL_Renderer* renderer) {

  SDL_Rect r = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 32, 32};
  SDL_RenderCopy(renderer, renderable->texture, NULL, &r);
  SDL_RenderPresent(renderer);

  return 0;
}

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Hello",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!window) {
      printf("error creating window: %s\n", SDL_GetError());
      SDL_Quit();
      return 1;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, render_flags);

    LTimer fpsTimer;
    LTimer capTimer;
    int countedFrames = 0;
    fpsTimer.start();

    if (!renderer) {
      printf("error creating renderer: %s\n", SDL_GetError());
      cleanup(renderer, window);
      return 1;
    }
  
    SDL_RenderClear(renderer);

    Renderable *renderable1;
    initializeRenderable(renderable1, renderer, "resources/hello.png");

    int quit = 0;
    SDL_Event e;
    float x = 0;

    while (!quit) {
      while (SDL_PollEvent(&e) != 0) {
        SDL_Rect r = {x, 0, 32, 32};
        renderable1->rectangle = &r;

        capTimer.start();
        float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
        if( avgFPS > 2000000 ) {
          avgFPS = 0;
        }

        renderRenderable(renderable1, renderer);

        if (e.type == SDL_QUIT) {
          quit = 1;
        }
        x += 1.0f;
      }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}
*/
