#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "../core/Drone.h"

using namespace std;

bool isInside(int x, int y, SDL_Rect rect)
{
    return (x > rect.x && x < rect.x + rect.w &&
            y > rect.y && y < rect.y + rect.h);
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() < 0)
    {
        std::cout << "TTF Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Drone Viewer",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer)
    {
        std::cout << "Window or Renderer creation failed\n";
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Đọc drone từ file
    std::vector<Drone> drones = readDronesFromFile("D:/SDL2-project/data/Drone.txt");
    if (drones.empty())
    {
        std::cout << "No drones loaded.\n";
    }

    // Mở font
    const char *fontPath = "C:/Windows/Fonts/arial.ttf"; // đổi sang font bạn có
    TTF_Font *font = TTF_OpenFont(fontPath, 14);
    if (!font)
    {
        std::cout << "Font Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    bool running = true;
    SDL_Event e;
    int selectedDroneIndex = -1;

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = false;
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX = e.button.x;
                int mouseY = e.button.y;

                for (int i = 0; i < drones.size(); i++)
                {
                    auto pos = drones[i].getPos();
                    SDL_Rect rect = {pos.first, pos.second, 20, 20};
                    if (isInside(mouseX, mouseY, rect))
                    {
                        selectedDroneIndex = i;
                    }
                }
            }
        }

        // Xóa màn hình
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Vẽ drone và ID
        for (int i = 0; i < drones.size(); i++)
        {
            auto pos = drones[i].getPos();
            SDL_Rect rect = {pos.first, pos.second, 20, 20};

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &rect);

            // Nếu drone này đang được chọn → vẽ ID
            if (i == selectedDroneIndex)
            {
                SDL_Color white = {255, 255, 255, 255};
                SDL_Surface *textSurf = TTF_RenderUTF8_Blended(font, drones[i].getId().c_str(), white);
                SDL_Texture *textTex = SDL_CreateTextureFromSurface(renderer, textSurf);

                SDL_Rect textRect = {pos.first, pos.second - 18, textSurf->w, textSurf->h};
                SDL_RenderCopy(renderer, textTex, nullptr, &textRect);

                SDL_FreeSurface(textSurf);
                SDL_DestroyTexture(textTex);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Cleanup
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
