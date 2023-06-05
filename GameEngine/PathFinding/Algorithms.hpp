#include "Map.hpp"
#include <cmath>
#include <queue>
#include <glm/glm.hpp>

using namespace glm;

class Algorithms
{
public:
    // Algorithms
    static vector<Node*> BreadthFirstSearch(Map* map, ivec2 start, ivec2 dest);
    static vector<Node*> Astar(Map* map, ivec2 start, ivec2 dest, int heuristic);
    static int GetIndexOf(vector<ivec2> vec, ivec2 el);

    // Heuristics
    static float Manhattan(ivec2 start, ivec2 dest);
    static float Euclidean(ivec2 start, ivec2 dest);

    static ivec2 SearchForLowestCost(const int heuristic, const std::vector<ivec2> ol, ivec2 start, ivec2 dest);
    static bool IsInList(const vector<ivec2> ol, ivec2 el);
    static bool IsInList(const vector<Node*> ol, ivec2 el);
};
