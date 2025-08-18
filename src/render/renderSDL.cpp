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
#include "../core/Graph.h"
#include "../algorithms/DijkstraPathFinder.h"
#include "renderUtils.h"
#include <ctime>
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
    SDL_Surface *cursorSurface = IMG_Load("D:/SDL2-project/assets/drone.png");
    SDL_Cursor *droneCursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
    // lay kich thuoc window render

    SDL_GetWindowSize(window, &winW, &winH);

    //  khai bao bien
    vector<Drone> drones = readDronesFromFile("D:/SDL2-project/data/Drone.txt");
    NodeData nodeData = readAllNodeFromFile("D:/SDL2-project/data/Node.txt");
    vector<Edge *> edges = readAllEdgeFromFile("D:/SDL2-project/data/Edge.txt", nodeData.nodeMap);
    // Ví dụ path: N1 -> N2 -> N7
    // vector<Node *> fakePath;
    // fakePath.push_back(nodeData.nodeMap["N1"]);
    // fakePath.push_back(nodeData.nodeMap["N2"]);
    // fakePath.push_back(nodeData.nodeMap["N3"]);
    // vector<Node *> path2;
    // path2.push_back(nodeData.nodeMap["N4"]);
    // path2.push_back(nodeData.nodeMap["N5"]);
    // path2.push_back(nodeData.nodeMap["N8"]);
    // path2.push_back(nodeData.nodeMap["N9"]);
    // drones[0].setPath(fakePath);
    // drones[1].setPath(path2);

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
    bool noDrone = false;
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
                }

                SDL_Rect noDroneRect = {winW - imgNoDroneW - 90, winH - imgNoDroneH - 10, imgNoDroneW, imgNoDroneH};
                if (isInside(mouseX, mouseY, noDroneRect))
                {
                    noDrone = true;
                }

                // Nếu đang ở chế độ chọn cạnh -> kiểm tra xem click có trúng cạnh nào không
                if (noDrone)
                {
                    bool foundEdge = false;
                    for (auto &edge : edges)
                    {
                        SDL_Point clickPoint = {mouseX, mouseY};
                        Node *u = edge->getFrom();
                        Node *v = edge->getTo();

                        auto upos = u->getPos();
                        auto vpos = v->getPos();

                        float dist = distancePointToSegment(
                            upos.first, upos.second,
                            vpos.first, vpos.second,
                            mouseX, mouseY);

                        if (!SDL_PointInRect(&clickPoint, &panelRect))
                        {
                            if (dist < 10.0f)
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
                        noDrone = false; // tắt chế độ chọn cạnh sau khi disable 1 cạnh
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

        // click drone and it would show
        if (showPanel && selectedDroneIndex >= 0 && selectedDroneIndex < drones.size())
        {

            renderInfoPanel(renderer, drones[selectedDroneIndex], font, winW, winH);
            // vẽ nút mo rong
            SDL_Rect formRect = {winW - imgW - 10, 10, imgW, imgH};
            renderButtonWithHover(renderer, formRect, img1);

            // vẽ button drones
            SDL_Rect droneRect = {winW - imgDroneW - 10, winH - imgDroneH - 10, imgDroneW, imgDroneH};
            renderButtonWithHover(renderer, droneRect, img2);

            SDL_Rect nodeRect = {winW - imgNodeW - 50, winH - imgNodeH - 10, imgNodeW, imgNodeH};
            renderButtonWithHover(renderer, nodeRect, img3);

            SDL_Rect noDroneRect = {winW - imgNoDroneW - 90, winH - imgNoDroneH - 10, imgNoDroneW, imgNoDroneH};
            renderButtonWithHover(renderer, noDroneRect, img5);
        }
        if (isPlacingDrone == true)
        {
            SDL_SetCursor(droneCursor);
        }
        else
        {
            // Khi thoát chế độ đặt drone (cancel hoặc đặt xong):
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
                vector<string> tasks = {"A3", "A2", "A4"};
                Node *current = nodeData.nodeMap["A0"];

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
                pathInitialized = true;
            }

            drones[0].updateMove();
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Cleanup
    SDL_FreeSurface(cursorSurface);
    SDL_FreeCursor(droneCursor);
    SDL_StopTextInput();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
