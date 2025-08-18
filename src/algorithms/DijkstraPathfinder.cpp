#include "DijkstraPathfinder.h"

vector<Node *> dijkstra(Node *start, Node *goal)
{
    unordered_map<Node *, float> dist;
    unordered_map<Node *, Node *> prev;
    auto cmp = [&](Node *a, Node *b)
    { return dist[a] > dist[b]; };
    priority_queue<Node *, vector<Node *>, decltype(cmp)> pq(cmp);

    // Khởi tạo khoảng cách = INF
    dist[start] = 0.0f;
    pq.push(start);

    while (!pq.empty())
    {
        Node *u = pq.top();
        pq.pop();

        if (u == goal)
            break;

        for (auto &neighbor : u->getNeighbors())
        {
            if (neighbor.cost >= 1e6)
                continue;
            Node *v = neighbor.node;
            float weight = neighbor.cost;
            float alt = dist[u] + weight;

            if (!dist.count(v) || alt < dist[v])
            {
                dist[v] = alt;
                prev[v] = u;
                pq.push(v);
            }
        }
    }

    // Truy vết ngược để lấy path
    vector<Node *> path;
    if (!prev.count(goal) && start != goal)
    {
        return path; // không có đường đi
    }

    for (Node *at = goal; at != nullptr; at = prev.count(at) ? prev[at] : nullptr)
    {
        path.push_back(at);
        if (at == start)
            break;
    }
    reverse(path.begin(), path.end());
    return path;
}