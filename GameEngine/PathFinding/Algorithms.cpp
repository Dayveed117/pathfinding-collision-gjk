#include "Algorithms.hpp"

ivec2 Algorithms::SearchForLowestCost(const int heuristic, const std::vector<ivec2> ol, ivec2 start, ivec2 dest)
{
    ivec2 lowest_node = ol[0];
    float lowest_cost = 0;

    // Enviar função como parâmetro?
    switch (heuristic)
    {
        // Manhattan
    case 1:
        lowest_cost = Algorithms::Manhattan(ol[0], dest) + Algorithms::Manhattan(start, ol[0]);
        // Euclidean
    case 2:
        lowest_cost = Algorithms::Euclidean(ol[0], dest) + Algorithms::Euclidean(start, ol[0]);
    default:
        lowest_cost = Algorithms::Manhattan(ol[0], dest) + Algorithms::Manhattan(start, ol[0]);
    }

    for (ivec2 pair : ol)
    {
        float cost = 0;

        switch (heuristic)
        {
            // Manhattan
        case 1:
            cost = Algorithms::Manhattan(pair, dest) + Algorithms::Manhattan(start, pair);
            // Euclidean
        case 2:
            cost = Algorithms::Euclidean(pair, dest) + Algorithms::Euclidean(start, pair);
        default:
            cost = Algorithms::Manhattan(pair, dest) + Algorithms::Manhattan(start, pair);
        }

        if (cost < lowest_cost)
        {
            lowest_cost = cost;
            lowest_node = pair;
        }

    }

    return lowest_node;
}

bool Algorithms::IsInList(const vector<Node*> ol, ivec2 el)
{
    for (Node* n : ol)
        if ((n->x == el.x) && (n->y == el.y))
            return true;
    return false;
}

bool Algorithms::IsInList(const vector<ivec2> ol, ivec2 el)
{
    for (ivec2 pair : ol)
        if ((pair.x == el.x) && (pair.y == el.y))
            return true;
    return false;
}

vector<Node*> Algorithms::BreadthFirstSearch(Map* map, ivec2 start, ivec2 dest)
{
    std::vector<Node*> path = {};

    if (!map->ValidStart(start) && !map->IsInBounds(dest))
        return path;

    // Start is 6
    // End is 9
    map->grid[start.y][start.x]->data = 6;
    map->grid[dest.y][dest.x]->data = 9;

    std::queue<ivec2> queue;
    queue.push(start);

    while (!queue.empty())
    {
        ivec2 u = queue.front();
        queue.pop();

        // Mark destination
        // Find path back to the start
        if (u == dest)
        {
            // Set final path to 3 ? 

            path.push_back(map->grid[u.y][u.x]);

            // u is going from destination to start
            while (u != start)
            {
                // Go up the parent tree
                // Set final path to 3
                // Insert parents to path
                u = map->grid[u.y][u.x]->parent;

                if (!(map->grid[u.y][u.x]->data == 6) && !(map->grid[u.y][u.x]->data == 9))
                    map->grid[u.y][u.x]->data = 3;

                path.push_back(map->grid[u.y][u.x]);
            }
             
            reverse(path.begin(), path.end());

            return path;
        }
        
        // GetAdj gets adjacent nodes that are not visited and its data
        vector<ivec2> adjList = map->GetAdj(u);
        
        for (ivec2 v : adjList)
        {
            // Add as visited, 
            // Add +1 distance
            // u as parent of v
            map->grid[v.y][v.x]->visited = true;
            map->grid[v.y][v.x]->distance = map->grid[u.y][u.x]->distance + 1;
            map->grid[v.y][v.x]->parent = u;
            queue.push(v);   
        }
        // Set to explored
        if (!(map->grid[u.y][u.x]->data == 6) && !(map->grid[u.y][u.x]->data == 9))
            map->grid[u.y][u.x]->data = 2;
    }

    return path;
}

vector<Node*> Algorithms::Astar(Map* map, ivec2 start, ivec2 dest, const int heuristic)
{
    std::vector<ivec2> open_list = {};
    std::vector<Node*> closed_list = {};

    if (!map->ValidStart(start) && !map->IsInBounds(dest))
        return closed_list;

    // Start is 6
    // End is 9
    map->grid[start.y][start.x]->data = 6;
    map->grid[dest.y][dest.x]->data = 9;

    open_list.push_back(start);

    while (!open_list.empty())
    {
        ivec2 cur = SearchForLowestCost(heuristic, open_list, start, dest);
        
        if (cur == dest)
        {
            closed_list.push_back(map->grid[dest.y][dest.x]);

            while (cur != start)
            {
                // Go up the parent tree
                // Set final path to 3
                cur = map->grid[cur.y][cur.x]->parent;

                // Alterar cor para path
                if (!(map->grid[cur.y][cur.x]->data == 6) && !(map->grid[cur.y][cur.x]->data == 9))
                    map->grid[cur.y][cur.x]->data = 3;

                return closed_list;
            }
        }
        
        else
        {
            int i = Algorithms::GetIndexOf(open_list, cur);
            open_list.erase(open_list.begin() + i);
            closed_list.push_back(map->grid[cur.y][cur.x]);
            vector<ivec2> adjList = map->GetAdj(cur);

            for (ivec2 pair : adjList)
            {
                if ((!IsInList(open_list, pair)) && (!IsInList(closed_list, pair)))
                {
                    open_list.push_back(pair);
                    map->grid[pair.y][pair.x]->visited = true;
                    map->grid[pair.y][pair.x]->parent = cur;
                    // map->grid[pair.y][pair.x]->data = 2;
                }
            }
        }
    }

    return closed_list;
}

// ---------------------------- //
//          HEURISTICS          //
// ---------------------------- //
float Algorithms::Manhattan(ivec2 start, ivec2 dest)
{
    return abs(start.x - dest.x) + abs(start.y - dest.y);
}

float Algorithms::Euclidean(ivec2 start, ivec2 dest)
{
    return sqrt(pow(start.x - dest.x, 2) + (pow(start.y - dest.y, 2)));
}


// ------------------------ //
//          HELPER          //
// ------------------------ //
int Algorithms::GetIndexOf(vector<ivec2> vec, ivec2 el)
{
    for (size_t i = 0; i < vec.size(); i++)
        if ((vec[i].x == el.x) && (vec[i].y == el.y))
            return i;

    return -1;
}
