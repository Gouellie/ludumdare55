#include "raylib.h"
#include "screens.h"
#include "scene.h"
#include "simple_components.h"
#include "game_object.h"

#include <gameplay/settlement.h>

#include <cstdlib>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

void ResolveTurn();

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

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Ray ray = GetScreenToWorldRay(GetMousePosition(), camera);
        for (GameObject* child : TestScene.GetChildren())
        {
            if (!child)
                continue;

            if (SettlementComponent* settlement = child->GetComponent<SettlementComponent>())
            {
                if (ModelComponent* model = child->GetComponent<ModelComponent>())
                {
                    RayCollision raycol = GetRayCollisionBox(ray, model->GetBoundingBox());
                    if (raycol.hit)
                    {
                        model->SetTint(RED);
                    }
                    else
                    {
                        model->SetTint(WHITE);
                    }
                }
            }
        }
    }

    // Here, plug event on "EndTurn" button pressed
    if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
    {
        ResolveTurn();
    }
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

// Handle end of turn
void ResolveTurn()
{
    for (GameObject* child : TestScene.GetChildren())
    {
        if (!child)
            continue;

        if (SettlementComponent* settlement = child->GetComponent<SettlementComponent>())
        {
            if (ModelComponent* model = child->GetComponent<ModelComponent>())
            {
                int min = 0;
                int max = 10;
                int random = min + (rand() % static_cast<int>(max - min + 1));
                if (random > 5)
                {
                    model->SetTint(BLUE);
                }
                else
                {
                    model->SetTint(GREEN);
                }
            }
        }
    }
}