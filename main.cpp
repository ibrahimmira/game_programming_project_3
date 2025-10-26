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
              // NUMBER_OF_LANDING_PADS = 9;

constexpr char BG_COLOUR[]    = "#000000ff";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

constexpr char ROCKET_IDLE[]  = "assets/idling_rocket.png";
constexpr char ROCKET_THRUSTING[]  = "assets/thrusting_rocket.png";
constexpr char LANDING_PAD[] = "assets/white_landing_platform.png";
constexpr char LANDING_PAD_RECTANGLE[] = "assets/white_rectangle.png";

Vector2 gRocketPosition = ORIGIN,
        gRocketScale    = { (float) 100 , (float) 100 },

        gLandingPadPosition = { ORIGIN.x - 500, ORIGIN.y + 300},
        gLandingPadScale    = { (float) 500  , (float) 30/5 },

        gRectanglePadPosition = { ORIGIN.x, ORIGIN.y + 300},
        gRectanglePadScale    = { (float) 500 , (float) 304  };



Entity *gRocket = nullptr;
Entity *gLandingPad = nullptr;
Entity *gRectanglePad = nullptr;

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

    gLandingPad = new Entity(
        gLandingPadPosition,
        gLandingPadScale,
        LANDING_PAD
    );

    gRectanglePad = new Entity(
        gRectanglePadPosition,
        gRectanglePadScale,
        LANDING_PAD_RECTANGLE
    );



    gRocket->setColliderDimensions({ gRocketScale.x/2, gRocketScale.y/2});
    gRocket->setAcceleration({ 0.0f, GRAVITATIONAL_ACCELERATION });
    gLandingPad->setColliderDimensions({ gLandingPadScale.x, gLandingPadScale.y});
    gRectanglePad->setColliderDimensions({ gRectanglePadScale.x, gRectanglePadScale.y});

    SetTargetFPS(FPS);
}


void processInput() 
{

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;

    if      (IsKeyDown(KEY_A))  gRocket->accelerateLeft();
    if      (IsKeyDown(KEY_D))  gRocket->accelerateRight();
    if      (IsKeyDown(KEY_W))  gRocket->accelerateUp();

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
        gRocket->update(FIXED_TIMESTEP, gLandingPad, 1);
}
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));
    gLandingPad->render();
    gRectanglePad->render();

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