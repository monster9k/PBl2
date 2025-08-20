#pragma once
#include <bits/stdc++.h>
#include <string>
#include "Node.h"

using namespace std;

class Drone
{
protected:
    string id;
    string type;
    vector<Node *> path; // đường đi tạm
    int segIndex = 0;    // đang đi từ path[segIndex] -> path[segIndex+1]
    float progress = 0.0;
    pair<float, float> pos;
    float battery;
    float speed;
    string isOp;

public:
    Drone();
    string getId() const;
    string getType() const;
    pair<float, float> getPos() const;
    const vector<Node *> &getPath() const;
    int geSegIndex() const;
    float getBattery() const;
    float getSpeed() const;
    string getIsOpStatus() const;

    void setId(string id);
    void setType(string type);
    void setPos(float x, float y);
    void setBattery(float battery);
    void setSpeed(float speed);
    void setIsOp(string isOp);
    void setPath(const vector<Node *> &p);
    void updateMove();

    void showAllData();

    ~Drone();

    // TODO: Thuộc tính & phương thức điều khiển drone
};

void saveDroneToFile(const Drone &drone);

vector<Drone> readDronesFromFile(const string &filename);
