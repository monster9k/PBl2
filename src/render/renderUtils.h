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

void renderInfoPanel(SDL_Renderer *renderer, const Drone &drone, TTF_Font *font, int screenWidth, int screenHeight);

void renderLine(SDL_Renderer *renderer, const vector<Edge *> &edges, TTF_Font *font);

void renderNode(SDL_Renderer *renderer, const NodeData &nodeData, TTF_Font *font);

SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &filePath, int &w, int &h);

void handlePlacement(int mouseX, int mouseY, bool &isPlacing,
                     const SDL_Rect &panelRect, std::vector<Drone> &drones, int &winW, int &winH, int &imgDroneH, int &imgDroneW);

void renderButtonWithHover(SDL_Renderer *renderer, SDL_Rect buttonRect, SDL_Texture *buttonTex = nullptr);

float distancePointToSegment(float x1, float y1, float x2, float y2, float mx, float my);