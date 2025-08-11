#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

// Hàm vẽ pixel
void drawPixel(SDL_Renderer* renderer, int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}

// Hàm vẽ hình tròn
void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        drawPixel(renderer, centerX + x, centerY + y);
        drawPixel(renderer, centerX + y, centerY + x);
        drawPixel(renderer, centerX - y, centerY + x);
        drawPixel(renderer, centerX - x, centerY + y);
        drawPixel(renderer, centerX - x, centerY - y);
        drawPixel(renderer, centerX - y, centerY - x);
        drawPixel(renderer, centerX + y, centerY - x);
        drawPixel(renderer, centerX + x, centerY - y);

        if (err <= 0) {
            y += 1;
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Vẽ Hình Tròn SDL2",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       640, 480,
                                       SDL_WINDOW_SHOWN);
    if (!win) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // Tô nền trắng
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderClear(ren);

    // Đặt màu đỏ để vẽ
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    drawCircle(ren, 320, 240, 100); // vẽ hình tròn tâm (320,240), bán kính 100

    SDL_RenderPresent(ren);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
        }
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
