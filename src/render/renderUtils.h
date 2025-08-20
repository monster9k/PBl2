#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "../core/Drone.h"
#include "../core/Node.h"
#include "../core/Edge.h"

using namespace std;

// check toa do
bool isInside(int x, int y, SDL_Rect rect);

// render drone va click to show id

void renderDrone(SDL_Renderer *renderer, const vector<Drone> &drones, int selectedDroneIndex, TTF_Font *font);

void renderInfoPanel(SDL_Renderer *renderer, const Drone &drone, TTF_Font *font, int winW,
                     int winH, SDL_Texture *img1, // nút mở rộng
                     SDL_Texture *img2,           // nút drones
                     SDL_Texture *img3,           // nút node
                     SDL_Texture *img5,           // nút noDrone
                     int imgW, int imgDroneW, int imgNodeW, int imgNoDroneW,
                     int imgH, int imgDroneH, int imgNodeH, int imgNoDroneH);

void renderLine(SDL_Renderer *renderer, const vector<Edge *> &edges, TTF_Font *font);

void renderNode(SDL_Renderer *renderer, const NodeData &nodeData, TTF_Font *font);

SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &filePath, int &w, int &h);

void handlePlacement(int mouseX, int mouseY, bool &isPlacing,
                     const SDL_Rect &panelRect, std::vector<Drone> &drones, int &winW, int &winH, int &imgDroneH, int &imgDroneW);

void renderButtonWithHover(SDL_Renderer *renderer, SDL_Rect buttonRect, SDL_Texture *buttonTex = nullptr);

float distancePointToSegment(float x1, float y1, float x2, float y2, float mx, float my);
