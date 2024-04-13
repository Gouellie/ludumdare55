#include "raylib.h"
#include "raymath.h"
#include "screens.h"
#include "scene.h"
#include "simple_components.h"
#include "game_object.h"

#include <gameplay/gamedirector.h>
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

    GameDirector directorInstance = GameDirector::GetInstance();
    bool isMousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);


    Ray ray = GetScreenToWorldRay(GetMousePosition(), camera);
    for (GameObject* child : Settlements.GetChildren())
    {
        if (!child)
            continue;

        if (SettlementComponent* settlement = child->GetComponent<SettlementComponent>())
        {
            if (ModelComponent* model = child->GetComponent<ModelComponent>())
            {
                if (model->GetPicked())
                {
                    model->SetTint(YELLOW);
                    continue;
                }

                Transform3DComponent* transform = child->GetComponent<Transform3DComponent>();
                Matrix matrix = transform->GetMatrix();

                RayCollision raycol = GetRayCollisionBox(ray, model->GetBoundingBox(&matrix));
                if (raycol.hit && isMousePressed && settlement->GetStatus() != SettlementStatus::Destroyed)
                {
                    model->SetPicked(true);
                }
                else
                {
                    model->SetTint(settlement->GetColor());
                }
            }
        }
    }

    // Here, plug event on "EndTurn" button pressed
    if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
    {
        directorInstance.ResolveTurn(Settlements);
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    BeginMode3D(camera);
    TestScene.Render3D();
    Settlements.Render3D();
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