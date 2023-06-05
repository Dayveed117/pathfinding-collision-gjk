#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Node.hpp"

using namespace std;

class Map
{
private:
    unsigned int width;
    unsigned int height;
    string type;
    string map_name;

    bool IsValid(glm::ivec2 cell);

public:
    vector<vector<Node*>> grid;
    
    Map(string map_name);
    
    void LoadMap();
    void PrintMap();

    bool IsInBounds(glm::ivec2 cell);
    bool ValidStart(glm::ivec2 cell);

    inline glm::ivec2 GetDims() { return glm::ivec2(width, height); }

    vector<glm::ivec2> GetAdj(glm::ivec2 start);
    static void PrintPath(vector<Node*> path);
    
    ~Map();
};

