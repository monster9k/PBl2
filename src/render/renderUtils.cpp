
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "renderUtils.h"
#include "../core/Drone.h"
#include "../core/Node.h"
#include "../core/Edge.h"

// check toa do
bool isInside(int x, int y, SDL_Rect rect)
{
    return (x > rect.x && x < rect.x + rect.w &&
            y > rect.y && y < rect.y + rect.h);
}

// render drone va click to show id
// dùng tham trị

void renderDrone(SDL_Renderer *renderer, const vector<Drone> &drones, int selectedDroneIndex, TTF_Font *font)
{
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
}

void renderInfoPanel(SDL_Renderer *renderer, const Drone &drone, TTF_Font *font, int screenWidth, int screenHeight)
{

    // Panel bên phải
    SDL_Rect panel = {screenWidth - 250, 0, 250, screenHeight};

    // Vẽ nền panel (xám)
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &panel);

    // Màu chữ trắng
    SDL_Color white = {255, 255, 255, 255};

    // Danh sách thông tin
    vector<string> info = {
        "ID: " + drone.getId(),
        "Type: " + drone.getType(),
        "Pos: (" + to_string(drone.getPos().first) + ", " + to_string(drone.getPos().second) + ")",
        "Battery: " + to_string(drone.getBattery()) + "%",
        "Speed: " + to_string(drone.getSpeed()),
        "Active: " + drone.getIsOpStatus(),

    };

    int y = 20;
    for (auto &line : info)
    {
        SDL_Surface *surf = TTF_RenderUTF8_Blended(font, line.c_str(), white);
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect textRect = {screenWidth - 240, y, surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, NULL, &textRect);

        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);

        y += 30; // cách dòng
    }
}
