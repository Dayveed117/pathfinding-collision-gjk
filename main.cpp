#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

// GameEngine
#include "GameEngine/Components/GameObject.hpp"
#include "GameEngine/Components/Transform.hpp"
#include "GameEngine/Primitives/Square.hpp"
#include "GameEngine/Primitives/Cube.hpp"
#include "GameEngine/Camera/Orthographic.hpp"
#include "GameEngine/Camera/Perspective.hpp"
#include "GameEngine/Scene.hpp"
#include "GameEngine/Window.hpp"
#include "GameEngine/Input/Input.hpp"
#include "GameEngine/Time/Time.hpp"
#include "GameEngine/Config/Config.hpp"
#include "GameEngine/UI/Canvas.hpp"
#include "GameEngine/UI/Button.hpp"
#include "GameEngine/Colliders/GJK.hpp"
#include "GameEngine/Colliders/SquareCollider.hpp"
#include "GameEngine/Colliders/CubeCollider.hpp"
#include "GameEngine/PathFinding/Map.hpp"
#include "GameEngine/PathFinding/Algorithms.hpp"


// Prefabs
#include "Assets/Prefabs/Wall.hpp"
#include "Assets/Prefabs/PlayerObj.hpp"

// Scripts
#include "Assets/Scripts/PlayerMovement.hpp"

#include "main.hpp"

unique_ptr<Scene> scene (new Scene());


int main(void)
{
  // Initialization of the GLFW
  glfwInit();

  glfwWindowHint(GLFW_SAMPLES, 4); //4xMSAA
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Opengl 3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Opengl 3.3
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Removes functionality deprecated opengl functions
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // sets the Opengl profile as core, added in 3.2

  // Creates the window
  vec2 dimensions = vec2(1280, 720);
  Window::SetDimensions(dimensions);
  GLFWwindow *window = glfwCreateWindow((int)dimensions.x, (int)dimensions.y, "PathFinding", NULL, NULL);

  // Create window context
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true;
  // Initialization of GLEW
  glewInit();

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Callbacks
  glfwSetKeyCallback(window, Input::KeyCallback);
  glfwSetWindowSizeCallback(window, Window::WindowReshapeCallback);

  // Background Color
  //glClearColor(45.f/255, 52.f/255, 54.f/255, 0.0f);
  glClearColor(0.1, 0.1, 0.1, 0.0f);

  // Enable depth test and blend
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  // Transfer VAO
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  
  // Initializations and configurations
  Square::Init();
  Cube::Init();
  Config::LoadConfig("config.cfg");


  // Pathfinding
  string fname = "maps/mymap.map";
  Map* map = new Map(fname);
  map->LoadMap();

  // Loads the scene and sets it as the active one
  //scene->AddCamera(new Perspective(45.0f, 4/3.0f, 0.1f, 500.0f, vec3(50, -100, 250), vec3(50, -100, 0), vec3(0, 1, 0)));
  //scene->AddCamera(new Orthographic(vec3(0, 0, 0), vec3(100, 100, 1)));
  
  loadPathFindingScene(scene, map);

  Scene::LoadScene(scene);

  glfwSwapInterval(1);
  // render scene for each frame
  do
  {
    Time::UpdateValues();
    
    // Iterate Objects
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lifeCycle(scene);

    // Swap buffers and looking for events
    glfwSwapBuffers(window);
    glfwPollEvents();

  }while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

  // Cleanup VAO, VBOs, and shaders from GPU
  for(GameObject *g: scene->GetGameObjects())
    g->~GameObject();
  scene->GetGameObjects().clear();

  /// Frees the vertex array
  glDeleteVertexArrays(1, &VertexArrayID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();
  return 0;
}


void lifeCycle(unique_ptr<Scene>& scene)
{
  // Garbage Collector
  for (GameObject* g : scene->GetGameObjects())
    if (g->CanDestroy())
      scene->RemoveGameObject(g);

  // Invokes the Update callback
  for (GameObject* g : scene->GetGameObjects())
    if (g->IsEnabled())
      for (Script* s : g->GetScripts())
        s->Update();

  // Inicializar as colisões?
  // Testar Game Colisões entre gameobject

  /*
  for (GameObject* g1 : scene->GetGameObjects())
  {
      if (g1->Tag() == "Player" && g1->IsEnabled())
      {
          for (GameObject* g2 : scene->GetGameObjects())
          {
              if (g1->Tag() != g2->Tag() && g2->IsEnabled())
              {
                  if (GJK::DetectCollision(g1->GetCollider(), g2->GetCollider()))
                  {
                      for (Script* s : g1->GetScripts())
                          s->OnCollision(g2->GetRenderer()->GetIPrimitive(), g2->Tag());
                  }
              }
          }
          break;
      }
  }
  */

  // Invokes the Draw callback
  scene->DrawScene();

  scene->DrawGUI();
}


/*
void loadCollisionScene(unique_ptr<Scene>& scene)
{
    // 4 Walls
    GameObject* lWall = Wall::AddWall(
        new Transform(vec3(5.0f, 50.0f, 0.0f), vec3(0.0f), vec3(10.0f, 100.0f, 1.0f)),
        vec3(255, 165, 0),
        new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)
    );
    GameObject* rWall = Wall::AddWall(
        new Transform(vec3(95.0f, 50.0f, 0.0f), vec3(0.0f), vec3(10.0f, 100.0f, 1.0f)),
        vec3(255, 165, 0),
        new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)
    );
    GameObject* dWall = Wall::AddWall(
        new Transform(vec3(50.0f, 5.0f, 0.0f), vec3(0.0f), vec3(80.0f, 10.0f, 1.0f)),
        vec3(255, 165, 0),
        new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)
    );
    GameObject* uWall = Wall::AddWall(
        new Transform(vec3(50.0f, 95.0f, 0.0f), vec3(0.0f), vec3(80.0f, 10.0f, 1.0f)),
        vec3(255, 165, 0),
        new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)
    );


    // 1 Obstacle
    GameObject* obstacle = Wall::AddWall(
        new Transform(vec3(75.0f, 75.0f, 0.0f), vec3(0.0f), vec3(10.0f, 10.0f, 1.0f)),
        vec3(0, 128, 128),
        new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)
    );

    // 1 Player
    GameObject* player = PlayerObj::AddPlayerObj(
        new Square(vec3(0, 165, 255)),
        new Transform(vec3(25.0f, 25.0f, 0.0f), vec3(0.0f), vec3(5.0f, 5.0f, 1.0f)),
        new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)
    );

    // Add Scripts
    player->AddScript(new PlayerMovement(player, 2.0f, 2.0f));

    // Add GameObjects to scene
    scene->AddGameObject(lWall);
    scene->AddGameObject(rWall);
    scene->AddGameObject(dWall);
    scene->AddGameObject(uWall);
    scene->AddGameObject(obstacle);
    scene->AddGameObject(player);
}
*/

void loadPathFindingScene(unique_ptr<Scene>& scene, Map* map)
{
    // vector<Node*> path = Algorithms::BreadthFirstSearch(map, glm::ivec2(0, 0), glm::ivec2(13, 6));
    vector<Node*> path = Algorithms::Astar(map, glm::ivec2(0, 0), glm::ivec2(7, 6), 1);
    // vector<Node*> path = Algorithms::Astar(map, glm::ivec2(0, 0), glm::ivec2(3, 2), 1);

    const int offx = 1.25;
    const int offy = 1.25;

    auto dims = map->GetDims();
    scene->AddCamera(new Orthographic(vec3(0, 0, 0), vec3(offx + dims.x * 1.25, offy + dims.y * 1.25, 1)));

    for (size_t i = 0; i < dims.y; i++)
        for (size_t j = 0; j < dims.x; j++)
        {
            vec3 color = {};
            auto node = map->grid[i][j]->data;

            // Start - Green
            if (node == 6)
                color = vec3(50, 205, 50);
            // Finish - Orange
            else if (node == 9)
                color = vec3(255, 165, 0);
            // Wall - Red
            else if (node == 0)
                color = vec3(139, 0, 0);
            // Not Explored - White
            else if (node == 1)
                color = vec3(255, 255, 255);
            // Explored -- Gray
            else if (node == 2)
                color = vec3(90, 90, 90);
            // Path -- Blue
            else if (node == 3)
                color = vec3(176, 224, 230);

            GameObject* go = Wall::AddWall(
                new Transform(vec3(offx + j*1.25, offy + i*1.25, 0.0f), vec3(0.0f), vec3(1.0f, 1.0f, 1.0f)),
                color,
                new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)
            );

            scene->AddGameObject(go);
        }
}



/*
void callLoadLevelSinglePlayer()
{
    scene.reset(new Scene());

    // Single Player camera 
    if (defaultcamera)
        scene->AddCamera(new Orthographic(vec3(-150, -250, -100), vec3(250, 50, 100)));
    else
        scene->AddCamera(new Perspective(45.0f, 4 / 3.0f, 0.1f, 500.0f, vec3(50, -100, 250), vec3(50, -100, 0), vec3(0, 1, 0)));
    
    Scene::LoadScene(scene);

    loadLevelSingleplayer(scene);
}
*/

/*
void callLoadLevelMainMenu()
{
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Pontuação Maxima Atual: " << Config::highscore << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    scene.reset(new Scene());

    // Single Player camera
    //scene->AddCamera(new Perspective(45.0f, 4 / 3.0f, 0.1f, 500.0f, vec3(50, -100, 250), vec3(50, -100, 0), vec3(0, 1, 0)));
    scene->AddCamera(new Orthographic(vec3(-200, -150, -100), vec3(200, 150, 100)));
    Scene::LoadScene(scene);

    loadLevelMainMenu(scene);

}
*/

/*
void loadLevelSingleplayer(unique_ptr<Scene>& scene)
{
  scene->AddLightSource(new LightSource(vec3(1.0f), vec3(0.4), vec3(1.0f), vec4(5.0f, 5.0f, 2.0f, 1.0f)));
  scene->AddLightSource(new LightSource(vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec4(-5.0f, 5.0f, 2.0f, 1.0f)));

  PhongShader::Init("Assets/Sprites/blocks/sample.png");

  // Tabuleiro
  scene->AddGameObject(Brick::AddBrick(
    new Transform(vec3(45, -195, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(100.0f, 1.0f, 10.0f)), vec3(125, 200, 10), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)));
  scene->AddGameObject(Brick::AddBrick(
    new Transform(vec3(95, -95, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(2.0f, 200.f, 10.0f)), vec3(75, 200, 10), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)));
  scene->AddGameObject(Brick::AddBrick(
    new Transform(vec3(-5, -95, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(2.0f, 200.f, 10.0f)), vec3(20, 200, 10), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)));

  // Game Layout
  Canvas* canvas = new Canvas();

  // Score digits
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), zeroTexture),
      new Transform(vec3(30, 15, -9), vec3(0, 0, 0), vec3(20/2, 40/2, 1))));
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), zeroTexture),
      new Transform(vec3(40, 15, -9), vec3(0, 0, 0), vec3(20/2, 40/2, 1))));
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), zeroTexture),
      new Transform(vec3(50, 15, -9), vec3(0, 0, 0), vec3(20/2, 40/2, 1))));
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), zeroTexture),
      new Transform(vec3(65, 15, -9), vec3(0, 0, 0), vec3(20/2, 40/2, 1))));
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), zeroTexture),
      new Transform(vec3(75, 15, -9), vec3(0, 0, 0), vec3(20/2, 40/2, 1))));
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), zeroTexture),
      new Transform(vec3(85, 15, -9), vec3(0, 0, 0), vec3(20/2, 40/2, 1))));

  // Level digits
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), zeroTexture),
      new Transform(vec3(75, -206, -9), vec3(0, 0, 0), vec3(20 / 2, 40 / 2, 1))));
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), zeroTexture),
      new Transform(vec3(85, -206, -9), vec3(0, 0, 0), vec3(20 / 2, 40 / 2, 1))));

  // HOLD - NEXT - SCORE - LEVEL
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), holdTexture),
      new Transform(vec3(134, -10, -9), vec3(0, 0, 0), vec3(70, 70, 1))));
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), nextTexture),
      new Transform(vec3(134, -80, -9), vec3(0, 0, 0), vec3(70, 70, 1))));
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), scoreTexture),
      new Transform(vec3(42, 15, 0), vec3(0, 0, 0), vec3(110, 20, 1))));
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), levelTexture),
      new Transform(vec3(45, -206, 0), vec3(0, 0, 0), vec3(110, 20, 1))));

  // rotate clockwise
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), buttonA),
      new Transform(vec3(145, -155, 0), vec3(0, 0, 0), vec3(40/2, 40/2, 1))));
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), rotateA),
      new Transform(vec3(180, -155, 0), vec3(0, 0, 0), vec3(40/2, 40/2, 1))));

  // rotate counterclockwise
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), buttonB),
      new Transform(vec3(145, -190, 0), vec3(0, 0, 0), vec3(40/2, 40/2, 1))));
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), rotateB),
      new Transform(vec3(180, -190, 0), vec3(0, 0, 0), vec3(40/2, 40/2, 1))));

  // Pause button
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), buttonSTART),
      new Transform(vec3(-60, -170, 0), vec3(0, 0, 0), vec3(80/2, 40/2, 1))));
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), pauseTexture),
      new Transform(vec3(-60, -190, 0), vec3(0, 0, 0), vec3(41, 12, 1))));

  
  // Background
  canvas->AddSprite(new Sprite(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), darkbgTexture),
      new Transform(vec3(44.5, -95, 0), vec3(0, 0, 0), vec3(101, 200, 1))));
  

  scene->AddCanvas(canvas);


  Canvas* pauseCanvas = new Canvas();

  // MENU BUTTON
  pauseCanvas->AddButton(new Button(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), menuTexture),
    new Transform(vec3(-70, 15, 0), vec3(0, 0, 0), vec3(120/2, 20/2, 5)),
    &callLoadLevelMainMenu));
  pauseCanvas->AddCursor(new Cursor(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), arrowTexture),
    new Transform(vec3(0, 0, 0), vec3(0, 0, 0), vec3(180/20, 80/20, 5)), -40), true);

  scene->AddCanvas(pauseCanvas);

  // GameOver canvas

  Canvas* gameoverCanvas = new Canvas();

  gameoverCanvas->AddButton(new Button(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), wgameoverTexture),
      new Transform(vec3(-65, -95, 20), vec3(0, 0, 0), vec3(120*0.85, 20*0.85, 1)),
      &callLoadLevelMainMenu));
  gameoverCanvas->AddCursor(new Cursor(new OpaqueShader(new Renderer(new Square(vec3(255.f)), new Material(vec3(1.0f), vec3(1.0f), vec3(1.0f), 128.0f)), arrowTexture),
      new Transform(vec3(0, 0, 0), vec3(0, 0, 0), vec3(180 / 10, 80 / 10, 1)), -40), true);

  scene->AddCanvas(gameoverCanvas);


  // GameManager
  GameObject* go = new GameObject(
    new Transform(vec3(-200, -200, -200), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f)), nullptr, "GameManager");
  go->AddScript(new MarathonManager(canvas, gameoverCanvas));
  go->AddScript(new PauseMenu(pauseCanvas));
  go->AddScript(new MenuLogic(pauseCanvas));
  go->AddScript(new MenuLogic(gameoverCanvas));
  scene->AddGameObject(go);
}
*/

/*
int main2()
{
    string fname = "maps/mymap.map";
    Map* map = new Map(fname);
    map->LoadMap();

    vector<Node*> path = Algorithms::BreadthFirstSearch(map, glm::ivec2(0, 0), glm::ivec2(4, 4));



    return 0;
}
*/
