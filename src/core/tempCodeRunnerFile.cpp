#include <iostream>
#include "Node.h"
#include "Edge.h"
#include <fstream>

using namespace std;

int main()
{
    NodeData nodeData = readAllNodeFromFile("D:/SDL2-project/data/Node.txt");
    vector<Edge *> edges = readAllEdgeFromFile("D:/SDL2-project/data/Edge.txt", nodeData.nodeMap);

    // In Node
    cout << "===== ALL NODES =====\n";
    for (auto n : nodeData.allNodes)
    {
        cout << n->getId() << " ("
             << n->getPos().first << ", "
             << n->getPos().second << ") "
             << n->getType() << "\n";

        cout << "====== NEIGHBORS =====\n";
        for (auto &nb : n->getNeighbors())
        {
            cout << "   -> " << nb.node->getId()
                 << " (" << nb.node->getPos().first << ", " << nb.node->getPos().second << ")"
                 << " cost: " << nb.cost << "\n";
        }
    }

    // In Edge
    cout << "\n===== ALL EDGES =====\n";
    for (auto e : edges)
    {
        cout << "From: " << e->getFrom()->getId()
             << " (" << e->getFrom()->getPos().first << ", " << e->getFrom()->getPos().second << ")\n";
        cout << "To:   " << e->getTo()->getId()
             << " (" << e->getTo()->getPos().first << ", " << e->getTo()->getPos().second << ")\n";
        cout << "Cost: " << e->getCost() << "\n";
        cout << "Speed: " << e->getSpeed() << "\n";
        cout << "Active: " << (e->isActive() ? "Yes" : "No") << "\n";
        cout << "-----------------------\n";
    }

    return 0;
}