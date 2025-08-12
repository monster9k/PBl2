#include "Drone.h"
#include <bits/stdc++.h>
#include <fstream>
#include <string>

using namespace std;

Drone::Drone()
{
}

/// get

string Drone::getId() const
{
    return this->id;
}

string Drone::getType() const
{
    return this->type;
}

pair<int, int> Drone::getPos() const
{
    return this->pos;
}

float Drone::getBattery() const
{
    return this->battery;
}

float Drone::getSpeed() const
{
    return this->speed;
}

string Drone::getIsOpStatus() const
{
    return this->isOp;
}

//// set

void Drone::setId(string id)
{
    this->id = id;
}

void Drone::setType(string type)
{
    this->type = type;
}

void Drone::setPos(int x, int y)
{
    pos = {x, y};
}

void Drone::setBattery(float battery)
{
    this->battery = battery;
}

void Drone::setSpeed(float speed)
{
    this->speed = speed;
}

void Drone::setIsOp(string isOp)
{
    this->isOp = isOp;
}

void Drone::showAllData()
{
    cout << "ID: " << id << endl;
    cout << "Type: " << type << endl;
    cout << "Pos: " << pos.first << ", " << pos.second << endl;
    cout << "Battery: " << battery << endl;
    cout << "Speed: " << speed << endl;
    cout << "isOp: " << isOp << endl;
}

Drone::~Drone() {}


vector<Drone> readDronesFromFile(const string& filename)
{
    ifstream file(filename);
    vector<Drone> drones;
    if (!file) {
        cerr << "Error: Cannot open file\n";
        return drones;
    }

    string id, type, isOp;
    int x, y;
    float battery, speed;

    while (file >> id >> type >> x >> y >> battery >> speed >> isOp)
    {
        Drone temp;
        temp.setId(id);
        temp.setType(type);
        temp.setPos(x, y);
        temp.setBattery(battery);
        temp.setSpeed(speed);
        temp.setIsOp(isOp);
        drones.push_back(temp);
    }
    return drones;
}



