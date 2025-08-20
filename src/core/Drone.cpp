#include "Drone.h"
#include "Node.h"
#include <bits/stdc++.h>
#include <fstream>
#include <string>

using namespace std;

Drone::Drone()
{
}

string Drone::getId() const
{
    return this->id;
}

string Drone::getType() const
{
    return this->type;
}

pair<float, float> Drone::getPos() const
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

int Drone::geSegIndex() const
{
    return this->segIndex;
}

const vector<Node *> &Drone::getPath() const
{
    return path;
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

void Drone::setPos(float x, float y)
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

// Drone.cpp
void Drone::setPath(const vector<Node *> &p)
{
    path = p;
    segIndex = 0;
    progress = 0.0f;
    if (!path.empty())
    {
        pos = path[0]->getPos(); // bắt đầu tại node đầu tiên
    }
}

void Drone::updateMove()
{
    if (path.empty())
        return;

    pair<float, float> target;

    if (isOp == "going")
    {
        // đi theo path
        if (segIndex >= (int)path.size() - 1)
        {
            isOp = "returning"; // hết đường đi -> quay về
            target = path.front()->getPos();
        }
        else
        {
            target = path[segIndex + 1]->getPos();
        }
    }
    else if (isOp == "returning")
    {
        target = path.front()->getPos();
    }
    else if (isOp == "idle")
    {
        return; // nghỉ thì không bay
    }

    // --- di chuyển ---
    float dx = target.first - pos.first;
    float dy = target.second - pos.second;
    float len = sqrt(dx * dx + dy * dy);
    if (len < 1e-6)
    {
        if (isOp == "going")
        {
            segIndex++;
            return;
        }
        else if (isOp == "returning")
        {
            // đã về tới căn cứ
            pos = path.front()->getPos();
            isOp = "idle"; // nghỉ hoàn toàn
            return;
        }
    }

    float step = speed;
    float vx = dx / len;
    float vy = dy / len;

    float oldX = pos.first, oldY = pos.second;
    pos.first += vx * step;
    pos.second += vy * step;

    // tiêu hao pin
    float movedDist = sqrt((pos.first - oldX) * (pos.first - oldX) +
                           (pos.second - oldY) * (pos.second - oldY));
    float consumptionRate = 0.05f;
    battery = max(0.0f, battery - movedDist * consumptionRate);

    float distToTarget = sqrt((target.first - pos.first) * (target.first - pos.first) +
                              (target.second - pos.second) * (target.second - pos.second));
    if (distToTarget < step)
    {
        pos = target;
        if (isOp == "going")
            segIndex++;
    }
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

vector<Drone> readDronesFromFile(const string &filename)
{
    ifstream file(filename);
    vector<Drone> drones;
    if (!file)
    {
        cerr << "Error: Cannot open file\n";
        return drones;
    }

    string id, type, isOp;
    float x, y;
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
    file.close();
    return drones;
}

void saveDroneToFile(const Drone &drone)
{
    ofstream file("D:/SDL2-project/data/Drone.txt", ios::app);
    file << drone.getId() << " " << drone.getType() << " " << drone.getPos().first << " " << drone.getPos().second << " " << drone.getBattery() << " " << drone.getSpeed() << " " << drone.getIsOpStatus() << "\n";
    file.close();
}

// int main()
// {
//     Drone newDrone;

// newDrone.setId("DR005");
// newDrone.setType("DJI-M2");
// newDrone.setPos(200, 300);
// newDrone.setBattery(90.0f);
// newDrone.setSpeed(11.9f);
// newDrone.setIsOp("false");

//     saveDroneToFile(newDrone);
// }