/**
 * @file main.hpp
 *
 * @brief Header da função main
 *
 * @author Dário Santos
 * Contact: dariovfsantos@gmail.com
 */
#pragma once

#include <memory>
using namespace std;


void lifeCycle(unique_ptr<Scene>& scene);

void loadCollisionScene(unique_ptr<Scene>& scene);

void loadPathFindingScene(unique_ptr<Scene>& scene, Map* map);
