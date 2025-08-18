
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include "renderUtils.h"
#include "../core/Drone.h"
#include "../core/Node.h"
#include "../core/Edge.h"

using namespace std;

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
        SDL_Rect rect = {(int)pos.first, (int)pos.second, 20, 20};
        renderButtonWithHover(renderer, rect, nullptr);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        // Nếu drone này đang được chọn → vẽ ID
        if (i == selectedDroneIndex)
        {
            SDL_Color white = {255, 255, 255, 255};
            SDL_Surface *textSurf = TTF_RenderUTF8_Blended(font, drones[i].getId().c_str(), white);
            SDL_Texture *textTex = SDL_CreateTextureFromSurface(renderer, textSurf);

            SDL_Rect textRect = {(int)pos.first, (int)pos.second - 18, textSurf->w, textSurf->h};
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

void renderLine(SDL_Renderer *renderer, const vector<Edge *> &edges, TTF_Font *font)
{
    int thickness = 3; // số pixel độ dày

    for (auto edge : edges)
    {
        auto fromPos = edge->getFrom()->getPos();
        auto toPos = edge->getTo()->getPos();
        if (edge->isActive())
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); // đỏ
        }

        for (int offset = -thickness / 2; offset <= thickness / 2; offset++)
        {
            SDL_RenderDrawLine(renderer,
                               fromPos.first + offset, fromPos.second,
                               toPos.first + offset, toPos.second);
            SDL_RenderDrawLine(renderer,
                               fromPos.first, fromPos.second + offset,
                               toPos.first, toPos.second + offset);
        }
        int midX = (fromPos.first + toPos.first) / 2;
        int midY = (fromPos.second + toPos.second) / 2;

        if (edge->isActive())
        {
            string costText = to_string((int)edge->getCost()); // ép int cho gọn
            SDL_Color textColor = {255, 0, 0, 255};            // đỏ dễ nhìn

            SDL_Surface *textSurface = TTF_RenderText_Solid(font, costText.c_str(), textColor);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            SDL_Rect textRect = {midX - textSurface->w / 2, midY - textSurface->h / 2,
                                 textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

void renderNode(SDL_Renderer *renderer, const NodeData &nodeData, TTF_Font *font)
{
    for (auto node : nodeData.allNodes)
    {
        auto pos = node->getPos();
        SDL_Rect node_rect = {(int)pos.first, (int)pos.second, 14, 14};

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // đỏ
        SDL_RenderFillRect(renderer, &node_rect);

        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface *textSurf = TTF_RenderUTF8_Blended(font, node->getId().c_str(), white);
        SDL_Texture *textTex = SDL_CreateTextureFromSurface(renderer, textSurf);

        SDL_Rect textRect = {(int)pos.first, (int)pos.second - 18, textSurf->w, textSurf->h};
        SDL_RenderCopy(renderer, textTex, nullptr, &textRect);

        SDL_FreeSurface(textSurf);
        SDL_DestroyTexture(textTex);
    }
}

// Hàm load ảnh và trả về SDL_Texture*
SDL_Texture *loadTexture(SDL_Renderer *renderer, const string &filePath, int &w, int &h)
{
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    if (!surface)
    {
        std::cout << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_QueryTexture(texture, NULL, NULL, &w, &h); // Lấy kích thước ảnh
    return texture;
}

// placing drone
void handlePlacement(int mouseX, int mouseY, bool &isPlacing,
                     const SDL_Rect &panelRect, vector<Drone> &drones, int &winW, int &winH, int &imgDroneH, int &imgDroneW)
{
    // check click create drone
    SDL_Rect droneRect = {winW - imgDroneW - 10, winH - imgDroneH - 10, imgDroneW, imgDroneH};
    if (isInside(mouseX, mouseY, droneRect))
    {
        isPlacing = true;
    }
    else if (isPlacing)
    {
        SDL_Point clickPoint = {mouseX, mouseY};
        if (!SDL_PointInRect(&clickPoint, &panelRect))
        {
            Drone d;
            string types[] = {"DJI-MA", "DJI-M2", "ANA"};
            d.setId(string("D" + to_string(rand() % 100000)));
            d.setType(string(types[rand() % 3]));
            d.setPos(mouseX, mouseY);
            d.setBattery(100.0f);
            d.setSpeed(10.0f + static_cast<float>(rand()) / RAND_MAX * (15.0f - 10.0f));
            d.setIsOp("true");
            drones.push_back(d);
            saveDroneToFile(d);
            isPlacing = false;
        }
    }
}

void renderButtonWithHover(SDL_Renderer *renderer, SDL_Rect buttonRect, SDL_Texture *buttonTex)
{
    int mouseX, mouseY;
    SDL_Color color = {240, 0, 0, 255};
    SDL_GetMouseState(&mouseX, &mouseY);

    bool isHover = mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
                   mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h;

    // Scale nhẹ khi hover
    SDL_Rect renderRect = buttonRect;
    if (isHover)
    {
        renderRect.x -= 2;
        renderRect.y -= 2;
        renderRect.w += 4;
        renderRect.h += 4;
    }

    // Render nút
    if (buttonTex)
    {
        SDL_RenderCopy(renderer, buttonTex, nullptr, &renderRect);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &renderRect);
    }

    // Overlay trắng mờ khi hover
    if (isHover)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 80); // alpha 80
        SDL_RenderFillRect(renderer, &renderRect);
    }
}

float distancePointToSegment(float x1, float y1, float x2, float y2, float mx, float my)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    if (dx == 0 && dy == 0)
    {
        return sqrt((mx - x1) * (mx - x1) + (my - y1) * (my - y1));
    }
    float t = ((mx - x1) * dx + (my - y1) * dy) / (dx * dx + dy * dy);
    t = std::max(0.0f, std::min(1.0f, t));
    float projX = x1 + t * dx;
    float projY = y1 + t * dy;
    return sqrt((mx - projX) * (mx - projX) + (my - projY) * (my - projY));
}
