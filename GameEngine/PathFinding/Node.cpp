#include "Node.hpp"

Node::Node(int x, int y, int data, bool visited)
{
    this->x = x;
    this->y = y;
    this->data = data;
    this->visited = visited;
    this->distance = 0;
    this->parent = glm::ivec2();
}
