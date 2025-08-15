#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "../core/Drone.h"
#include "../core/Node.h"
#include "../core/Edge.h"
#include "renderUtils.h"
using namespace std;

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
                                          1200, 800, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer)
    {
        std::cout << "Window or Renderer creation failed\n";
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // loard anh
    SDL_Surface *imgSurface = IMG_Load("D:/SDL2-project/assets/nutmorong1.png"); // <-- đổi thành tên file ảnh của bạn
    if (!imgSurface)
    {
        std::cout << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return 1;
    }

    SDL_Texture *imgTexture = SDL_CreateTextureFromSurface(renderer, imgSurface);
    SDL_FreeSurface(imgSurface);

    // lay kich thuoc anh
    int imgW, imgH;
    SDL_QueryTexture(imgTexture, NULL, NULL, &imgW, &imgH);

    // lay kich thuoc window render
    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);

    //  file drones
    vector<Drone> drones = readDronesFromFile("D:/SDL2-project/data/Drone.txt");
    if (drones.empty())
    {
        std::cout << "No drones loaded.\n";
    }

    NodeData nodeData = readAllNodeFromFile("D:/SDL2-project/data/Node.txt");
    vector<Edge *> edges = readAllEdgeFromFile("D:/SDL2-project/data/Edge.txt", nodeData.nodeMap);

    // Mở font
    const char *fontPath = "C:/Windows/Fonts/arial.ttf"; // đổi sang font bạn có
    TTF_Font *font = TTF_OpenFont(fontPath, 14);
    if (!font)
    {
        std::cout << "Font Error: " << TTF_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    bool running = true;
    SDL_Event e;
    // check click panel
    bool showPanel = false;

    // check click drone
    int selectedDroneIndex = -1;

    bool creatingDrone = false;
    string inputText = "";
    Drone newDrone;
    SDL_StartTextInput();
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
                // check click to nutmorong
                SDL_Rect formRect = {winW - imgW - 10, 10, imgW, imgH};
                if (isInside(mouseX, mouseY, formRect))
                {
                    showPanel = !showPanel;
                    if (!showPanel)
                        selectedDroneIndex = 0;
                }

                for (int i = 0; i < drones.size(); i++)
                {
                    auto pos = drones[i].getPos();
                    SDL_Rect rect = {pos.first, pos.second, 20, 20};
                    if (isInside(mouseX, mouseY, rect))
                    {
                        selectedDroneIndex = i;
                        showPanel = true;
                    }
                }
            }

            // Nhập text
            if (e.type == SDL_TEXTINPUT && creatingDrone)
            {
                inputText += e.text.text;
            }
        }

        // Xóa màn hình
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // vẽ nút mo rong
        SDL_Rect formReact = {winW - imgW - 10, 10, imgW, imgH};
        SDL_RenderCopy(renderer, imgTexture, NULL, &formReact);

        // Vẽ drone và ID
        renderDrone(renderer, drones, selectedDroneIndex, font);

        // click drone and it would show
        if (showPanel && selectedDroneIndex >= 0 && selectedDroneIndex < drones.size())
        {

            renderInfoPanel(renderer, drones[selectedDroneIndex], font, winW, winH);
            // vẽ nút mo rong
            SDL_Rect formReact = {winW - imgW - 10, 10, imgW, imgH};
            SDL_RenderCopy(renderer, imgTexture, NULL, &formReact);
        }

        int thickness = 3; // số pixel độ dày
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        for (auto edge : edges)
        {
            auto fromPos = edge->getFrom()->getPos();
            auto toPos = edge->getTo()->getPos();
            for (int offset = -thickness / 2; offset <= thickness / 2; offset++)
            {
                SDL_RenderDrawLine(renderer,
                                   fromPos.first + offset, fromPos.second,
                                   toPos.first + offset, toPos.second);
                SDL_RenderDrawLine(renderer,
                                   fromPos.first, fromPos.second + offset,
                                   toPos.first, toPos.second + offset);
            }
        }

        for (auto node : nodeData.allNodes)
        {
            auto pos = node->getPos();
            SDL_Rect node_rect = {pos.first, pos.second, 20, 20};

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // đỏ
            SDL_RenderFillRect(renderer, &node_rect);

            SDL_Color white = {255, 255, 255, 255};
            SDL_Surface *textSurf = TTF_RenderUTF8_Blended(font, node->getId().c_str(), white);
            SDL_Texture *textTex = SDL_CreateTextureFromSurface(renderer, textSurf);

            SDL_Rect textRect = {pos.first, pos.second - 18, textSurf->w, textSurf->h};
            SDL_RenderCopy(renderer, textTex, nullptr, &textRect);

            SDL_FreeSurface(textSurf);
            SDL_DestroyTexture(textTex);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Cleanup
    SDL_StopTextInput();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
