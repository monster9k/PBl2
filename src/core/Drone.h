#pragma once
#include <bits/stdc++.h>
#include <string>

using namespace std;

class Drone
{
protected:
    string id;
    string type;
    pair<int, int> pos;
    float battery;
    float speed;
    string isOp;

public:
    Drone();
    string getId() const;
    string getType() const;
    pair<int, int> getPos() const;
    float getBattery() const;
    float getSpeed() const;
    string getIsOpStatus() const;

    void setId(string id);
    void setType(string type);
    void setPos(int x, int y);
    void setBattery(float battery);
    void setSpeed(float speed);
    void setIsOp(string isOp);

    void showAllData();

    ~Drone();

    // TODO: Thuộc tính & phương thức điều khiển drone
};

void saveDroneToFile(const Drone &drone);

vector<Drone> readDronesFromFile(const string &filename);
