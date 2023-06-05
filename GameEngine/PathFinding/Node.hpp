#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <string>

using namespace std;

class Node
{
public:
    int x;
    int y;
    int data;
    int distance;
    bool visited;
    glm::ivec2 parent;

    Node(int x, int y, int data, bool visited);
    string GetContents();
};

