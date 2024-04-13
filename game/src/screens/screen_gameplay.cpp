#include "raylib.h"
#include "screens.h"
#include "scene.h"
#include "simple_components.h"
#include "game_object.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    TestScene.Update();
    Ray ray = GetScreenToWorldRay(GetMousePosition(), camera);
    for (GameObject* child : TestScene.GetChildren()) {
        ModelComponent* model = child->GetComponent<ModelComponent>();
        if (model) 
        {
            RayCollision raycol = GetRayCollisionBox(ray, model->GetBoundingBox());
            if (raycol.hit) 
            {
                model->SetTint(RED);
            }
            else {
                model->SetTint(WHITE);
            }
        }
    }
}

void test(GameObject& game) {
    game.GetComponent<ModelComponent>();
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    BeginMode3D(camera);
    TestScene.Render3D();
    EndMode3D();
    TestScene.Render();
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}