#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <ctime>
#include "../core/Drone.h"
#include "../core/Node.h"
#include "../core/Edge.h"
#include "../core/Graph.h"
#include "../algorithms/DijkstraPathFinder.h"
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

    // lay kich thuoc anh
    int imgW, imgH, imgDroneW, imgDroneH, imgNodeW, imgNodeH, imgRunDroneH, imgRunDroneW, imgNoDroneW, imgNoDroneH;

    int winW, winH;
    // loard ảnh mo rong button
    SDL_Texture *img1 = loadTexture(renderer, "D:/SDL2-project/assets/nutmorong1.png", imgW, imgH);
    SDL_Texture *img2 = loadTexture(renderer, "D:/SDL2-project/assets/drone.png", imgDroneW, imgDroneH);
    SDL_Texture *img3 = loadTexture(renderer, "D:/SDL2-project/assets/node.png", imgNodeW, imgNodeH);
    SDL_Texture *img4 = loadTexture(renderer, "D:/SDL2-project/assets/runDrone.png", imgRunDroneW, imgRunDroneH);
    SDL_Texture *img5 = loadTexture(renderer, "D:/SDL2-project/assets/nodrone.png", imgNoDroneW, imgNoDroneH);

    // cursor
    SDL_Surface *cursorSurface = IMG_Load("D:/SDL2-project/assets/drone.png");
    SDL_Cursor *droneCursor = SDL_CreateColorCursor(cursorSurface, 0, 0);

    SDL_Surface *nodecursorSurface = IMG_Load("D:/SDL2-project/assets/node.png");
    SDL_Cursor *nodeCursor = SDL_CreateColorCursor(nodecursorSurface, 0, 0);

    SDL_Surface *noDronecursorSurface = IMG_Load("D:/SDL2-project/assets/nodrone.png");
    SDL_Cursor *noDroneCursor = SDL_CreateColorCursor(noDronecursorSurface, 0, 0);
    // lay kich thuoc window render

    SDL_GetWindowSize(window, &winW, &winH);

    //  khai bao bien
    vector<Drone> drones = readDronesFromFile("D:/SDL2-project/data/Drone.txt");
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

    // khung panel
    SDL_Rect panelRect = {winW - 250, 0, 250, winH};

    bool running = true;
    SDL_Event e;
    // check click panel
    bool showPanel = false;

    // check click drone

    int selectedDroneIndex = -1;
    bool selectingEdge = false;
    bool runDrone = false;
    bool isPlacingDrone = false;
    bool creatingDrone = false;
    bool selectedNode = false;
    bool pathInitialized = false;
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

                SDL_Rect runDroneRect = {10, winH - imgRunDroneH - 10, imgRunDroneW, imgRunDroneH};
                if (isInside(mouseX, mouseY, runDroneRect))
                {
                    runDrone = true;
                    pathInitialized = false;
                    for (auto &d : drones)
                    {
                        d.setIsOp("going"); // reset trạng thái cho tất cả drones
                    }
                }

                SDL_Rect noDroneRect = {winW - imgNoDroneW - 90, winH - imgNoDroneH - 10, imgNoDroneW, imgNoDroneH};
                if (isInside(mouseX, mouseY, noDroneRect))
                {
                    selectingEdge = true;
                    continue;
                }

                // Nếu đang ở chế độ chọn cạnh -> kiểm tra xem click có trúng cạnh nào không
                if (selectingEdge)
                {
                    int mx, my;
                    SDL_GetMouseState(&mx, &my);
                    bool foundEdge = false;
                    for (auto &edge : edges)
                    {
                        SDL_Point clickPoint = {mx, my};
                        Node *u = edge->getFrom();
                        Node *v = edge->getTo();

                        auto upos = u->getPos();
                        auto vpos = v->getPos();

                        float dist = distancePointToSegment(
                            upos.first, upos.second,
                            vpos.first, vpos.second,
                            mx, my);

                        if (!SDL_PointInRect(&clickPoint, &panelRect))
                        {
                            if (dist < 20.0f)
                            {                            // 10 px = ngưỡng click
                                edge->setCost(10000000); // vô hiệu hóa bằng cost lớn
                                edge->setActive(false);  // có thể set thêm flag để render đỏ
                                for (auto &other : edges)
                                {
                                    if (other->getFrom()->getId() == v->getId() &&
                                        other->getTo()->getId() == u->getId())
                                    {
                                        other->setCost(10000000);
                                        other->setActive(false);
                                        break;
                                    }
                                }
                                updateEdgeFile("D:/SDL2-project/data/Edge.txt", edges);
                                rebuildNeighbors(edges, nodeData.nodeMap);
                                foundEdge = true;
                                pathInitialized = false; // buộc nó false để runDrone ms
                                break;
                            }
                        }
                    }

                    if (foundEdge)
                    {
                        selectingEdge = false; // tắt chế độ chọn cạnh sau khi disable 1 cạnh
                    }
                }
                SDL_Rect formRect = {winW - imgW - 10, 10, imgW, imgH};
                if (isInside(mouseX, mouseY, formRect))
                {
                    showPanel = !showPanel;
                    if (!showPanel)
                        selectedDroneIndex = 0;
                }
                // check create drone button
                handlePlacement(mouseX, mouseY, isPlacingDrone, panelRect, drones, winW, winH, imgDroneW, imgDroneH);

                // create node
                SDL_Rect nodeRect = {winW - imgNodeW - 50, winH - imgNodeH - 10, imgNodeW, imgNodeH};
                if (isInside(mouseX, mouseY, nodeRect))
                {
                    selectedNode = true;
                }
                else if (selectedNode)
                {
                    SDL_Point clickPoint = {mouseX, mouseY};
                    if (!SDL_PointInRect(&clickPoint, &panelRect))
                    {
                        addNodeAndEdges(mouseX, mouseY, nodeData, edges, "D:/SDL2-project/data/Node.txt", "D:/SDL2-project/data/Edge.txt");

                        selectedNode = false;
                    }
                }

                for (int i = 0; i < drones.size(); i++)
                {
                    auto pos = drones[i].getPos();
                    SDL_Rect rect = {(int)pos.first, (int)pos.second, 20, 20};
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
        SDL_Rect formRect = {winW - imgW - 10, 10, imgW, imgH};
        renderButtonWithHover(renderer, formRect, img1);

        // nut dieu khien
        SDL_Rect runDroneRect = {10, winH - imgRunDroneH - 10, imgRunDroneW, imgRunDroneH};
        renderButtonWithHover(renderer, runDroneRect, img4);

        // click drone and or click status it would show panel
        if (showPanel && selectedDroneIndex >= 0 && selectedDroneIndex < drones.size())
        {

            renderInfoPanel(renderer, drones[selectedDroneIndex], font, winW, winH, img1, img2, img3, img5,
                            imgW, imgDroneW, imgNodeW, imgNoDroneW,
                            imgH, imgDroneH, imgNodeH, imgNoDroneH);
        }

        // make cursor active
        if (isPlacingDrone)
        {
            SDL_SetCursor(droneCursor);
        }
        else if (selectedNode)
        {
            SDL_SetCursor(nodeCursor);
        }
        else if (selectingEdge)
        {
            SDL_SetCursor(noDroneCursor);
        }
        else
        {
            SDL_SetCursor(SDL_GetDefaultCursor());
        }

        renderDrone(renderer, drones, selectedDroneIndex, font);
        // render line
        renderLine(renderer, edges, font);

        // render node
        renderNode(renderer, nodeData, font);
        // Giải phóng khi quit

        // Vẽ drone và ID
        if (runDrone)
        {
            if (!pathInitialized)
            {
                vector<string> tasks = {"N3", "N2", "N4"};
                Node *current = nodeData.nodeMap["N0"];

                vector<Node *> fullPath;
                fullPath.push_back(current);

                for (auto &t : tasks)
                {
                    Node *target = nodeData.nodeMap[t];
                    vector<Node *> subPath = dijkstra(current, target);

                    if (!subPath.empty())
                    {
                        fullPath.insert(fullPath.end(), subPath.begin() + 1, subPath.end());
                        current = target;
                    }
                }

                drones[0].setPath(fullPath);

                // Drone 2
                vector<string> tasks2 = {"N5", "N6", "N7"};
                Node *current2 = nodeData.nodeMap["N1"];
                vector<Node *> fullPath2;
                fullPath2.push_back(current2);

                for (auto &t : tasks2)
                {
                    Node *target = nodeData.nodeMap[t];
                    vector<Node *> subPath = dijkstra(current2, target);
                    if (!subPath.empty())
                    {
                        fullPath2.insert(fullPath2.end(), subPath.begin() + 1, subPath.end());
                        current2 = target;
                    }
                }
                drones[1].setPath(fullPath2);
                pathInitialized = true;
            }

            drones[0].updateMove();
            drones[1].updateMove();
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Cleanup
    SDL_FreeCursor(droneCursor);
    SDL_FreeCursor(nodeCursor);
    SDL_FreeCursor(noDroneCursor);
    SDL_FreeSurface(cursorSurface);
    SDL_FreeSurface(nodecursorSurface);
    SDL_FreeSurface(noDronecursorSurface);

    SDL_StopTextInput();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
