#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Node.h"
#include "Edge.h"

using namespace std;

// Hàm tính khoảng cách Euclidean
float calcDistance(pair<float, float> p1, pair<float, float> p2)
{
    return sqrt((p1.first - p2.first) * (p1.first - p2.first) +
                (p1.second - p2.second) * (p1.second - p2.second));
}

// Hàm thêm Node mới và sinh cạnh
void addNodeAndEdges(float mouseX, float mouseY,
                     NodeData &nodeData,
                     vector<Edge *> &edges,
                     const string &nodeFile,
                     const string &edgeFile)
{
    // --- Tạo Node mới ---
    string newId = "N" + to_string(nodeData.allNodes.size());
    Node *newNode = new Node(newId, mouseX, mouseY, "zoneA");

    // Thêm vào NodeData
    nodeData.allNodes.push_back(newNode);
    nodeData.nodeMap[newId] = newNode;

    // --- Ghi node vào file ---
    ofstream fout(nodeFile, ios::app);
    fout << newId << " " << mouseX << " " << mouseY << " " << "zoneA" << "\n";
    fout.close();

    // --- Tạo cạnh nối với tất cả node khác ---
    srand(time(0));
    ofstream eout(edgeFile, ios::app);

    for (auto &kv : nodeData.nodeMap)
    {
        Node *other = kv.second;
        if (other == newNode)
            continue;

        float dist = calcDistance(newNode->getPos(), other->getPos());
        float speed1 = 8.0f + static_cast<float>(rand()) / RAND_MAX * (9.0f - 8.0f);
        float speed2 = 8.0f + static_cast<float>(rand()) / RAND_MAX * (9.0f - 8.0f);

        // Tạo edge newNode -> other
        Edge *e1 = new Edge(newNode, other, dist, speed1, 1);
        edges.push_back(e1);
        eout << newNode->getId() << " " << other->getId() << " "
             << dist << " " << speed1 << " 1\n";

        // Tạo edge other -> newNode
        Edge *e2 = new Edge(other, newNode, dist, speed2, 1);
        edges.push_back(e2);
        eout << other->getId() << " " << newNode->getId() << " "
             << dist << " " << speed2 << " 1\n";

        // Cập nhật neighbors trong Node
        newNode->addNeighbor(other, dist);
        other->addNeighbor(newNode, dist);
    }

    eout.close();
}

void rebuildNeighbors(vector<Edge *> &edges, unordered_map<string, Node *> &nodeMap)
{
    // clear hết neighbors cũ
    for (auto &p : nodeMap)
    {
        p.second->clearNeighbors(); // bạn cần implement hàm clearNeighbors() trong Node
    }

    // build lại từ edges
    for (auto &edge : edges)
    {
        if (edge->isActive())
        {
            Node *u = edge->getFrom();
            Node *v = edge->getTo();
            u->addNeighbor(v, edge->getCost());
        }
    }
}