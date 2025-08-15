#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "../core/Drone.h"
#include "../core/Node.h"
#include "../core/Edge.h"

// check toa do
bool isInside(int x, int y, SDL_Rect rect);

// render drone va click to show id

void renderDrone(SDL_Renderer *renderer, const vector<Drone> &drones, int selectedDroneIndex, TTF_Font *font);

void renderInfoPanel(SDL_Renderer *renderer, const Drone &drone, TTF_Font *font, int screenWidth, int screenHeight);