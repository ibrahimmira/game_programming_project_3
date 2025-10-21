/**
* Author: Ibrahim Mira
* Assignment: Lunar Lander
* Date due: 2025-10-25, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Entity.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 1500,
              SCREEN_HEIGHT = 800,
              FPS           = 120;

constexpr char BG_COLOUR[]    = "#000000ff";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

constexpr char ROCKET_IDLE[]  = "assets/idling_rocket.png";
constexpr char ROCKET_THRUSTING[]  = "assets/thrusting_rocket.png";

Vector2 gRocketPosition = ORIGIN,
        gRocketScale    = { (float) 100 , (float) 100 };


Entity *gRocket = nullptr;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gAngle          = 0.0f,
      gTimeAccumulator = 0.0f;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();


void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lunar Lander");

    std::map<RocketState, std::vector<int>> animationAtlas = {
        {IDLE,          {  0, 1, 2, 3, 4, 5      }},
        {THRUSTING,     {  0, 1, 2, 3, 4, 5      }},
    };

    SetTargetFPS(FPS);

    gRocket = new Entity(
        gRocketPosition, 
        gRocketScale, 
        { ROCKET_IDLE, ROCKET_THRUSTING },
        ATLAS, 
        { 1, 6 },
        animationAtlas
    );

    gRocket->setColliderDimensions({ gRocketScale.x/2, gRocketScale.y/2});
    gRocket->setAcceleration({ 0.0f, GRAVITATIONAL_ACCELERATION });
}

void processInput() 
{

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;

    // gRocket->resetMovement(); 
    // gRocket->setAcceleration({ 0.0f, GRAVITATIONAL_ACCELERATION });

    // if      (IsKeyDown(KEY_A))  gRocket->accelerateLeft();
    // else if (IsKeyDown(KEY_D))  gRocket->accelerateRight();
    // else if (IsKeyDown(KEY_W))  gRocket->accelerateUp();
    // else if  (IsKeyDown(KEY_SPACE)) gRocket->accelerateDown();  

    if      (IsKeyDown(KEY_A))  gRocket->accelerateLeft();
    if (IsKeyDown(KEY_D))  gRocket->accelerateRight();
    if (IsKeyDown(KEY_W))  gRocket->accelerateUp();
    gRocket->accelerateDown();
    // else if  (IsKeyDown(KEY_SPACE)) gRocket->accelerateDown(); 

    // if (IsKeyDown(KEY_SPACE)) {
    //     gRocket->setAcceleration({0.0f, -1 * GRAVITATIONAL_ACCELERATION});
    // }
    // else {
    //     gRocket->setAcceleration({0.0f, GRAVITATIONAL_ACCELERATION});
    // }

}

void update() 
{
    // Delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;


    // Fixed timestep
    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        // Update game here and call use FIXED_TIMESTEP for any calls
        deltaTime -= FIXED_TIMESTEP;
    }

    if (gRocket != nullptr) {
        gRocket->update(FIXED_TIMESTEP, nullptr, 0);
}
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));
    gRocket->render();
    EndDrawing();
}

void shutdown() 
{ 
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}