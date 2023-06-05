#include "Map.hpp"

Map::Map(string map_name)
{
    this->map_name = map_name;
    this->type = "";
    this->grid = {};
    this->width = 0;
    this->height = 0;
}

void Map::LoadMap()
{
    string type;
    int x = 0;
    int y = 0;
    vector<vector<Node*>> grid;

    ifstream map_file(this->map_name);

    if (!map_file.is_open())
    {
        cout << "Could not load " << this->map_name << endl;
        return;
    }

    while (!map_file.eof())
    {
        // Reading map file headers
        string header;
        map_file >> header;

        if (header == "type")
            map_file >> type;

        else if (header == "height")
            map_file >> y;

        else if (header == "width")
            map_file >> x;

        // Read map file contents
        else if (header == "map")
        {
            // Resize Grid accordingly
            grid.resize(y);
            for (auto& v : grid)
                v.resize(x);

            // dummy getline
            string line;
            getline(map_file, line);

            int ty = 0;

            // For each line of the map's contents
            while (getline(map_file, line))
            {
                int tx = 0;

                // Iterate over the entire line and store accordingly
                for (char c : line)
                {
                    if (c == '.')
                        grid[ty][tx] = new Node(tx, ty, 1, false);
                    else
                        grid[ty][tx] = new Node(tx, ty, 0, false);

                    tx += 1;
                }
                ty += 1;
            }
            break;
        }
    }

    map_file.close();

    this->width = x;
    this->height = y;
    this->grid = grid;
    this->type = type;
    this->map_name = map_name;
}

void Map::PrintMap()
{
    for (auto &v : grid)
    {
        for (Node* el : v)
            cout << el->data;
        cout << endl;
    }
}

bool Map::IsInBounds(glm::ivec2 cell)
{
    return ((cell.x >= 0 && cell.x < width) &&
        (cell.y >= 0 && cell.y < height));
}

bool Map::ValidStart(glm::ivec2 cell)
{
    return ((cell.x >= 0 && cell.x < width) &&
        (cell.y >= 0 && cell.y < height) &&
        (grid[cell.y][cell.x]->data != 0));
}

bool Map::IsValid(glm::ivec2 cell)
{
    // Is in boundaries of map
    // Is not visited
    // Is part of the path , data != 0
    return ((cell.x >= 0 && cell.x < width) &&
        (cell.y >= 0 && cell.y < height) &&
        (grid[cell.y][cell.x]->visited == false) &&
        (grid[cell.y][cell.x]->data != 0));
}

vector<glm::ivec2> Map::GetAdj(glm::ivec2 cell)
{
    vector<glm::ivec2> adjacents = {};

    glm::ivec2 up(cell.x, cell.y - 1);
    glm::ivec2 down(cell.x, cell.y + 1);
    glm::ivec2 left(cell.x - 1, cell.y);
    glm::ivec2 right(cell.x + 1, cell.y);

    if (IsValid(up))
        adjacents.push_back(up);
    if (IsValid(down))
        adjacents.push_back(down);
    if (IsValid(left))
        adjacents.push_back(left);
    if (IsValid(right))
        adjacents.push_back(right);

    return adjacents;
}

void Map::PrintPath(vector<Node*> path)
{
    for (Node* n : path)
        cout << "[" << n->x << "," << n->y << "]";
    cout << endl;
}
