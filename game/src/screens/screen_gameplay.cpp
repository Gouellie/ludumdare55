#include "raylib.h"
#include "raymath.h"
#include "screens.h"
#include "scene.h"
#include "simple_components.h"
#include "game_object.h"

#include <gameplay/gamedirector.h>
#include <gameplay/settlement.h>
#include <ui/boardcomponent.h>

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

    GameDirector& directorInstance = GameDirector::GetInstance();
    directorInstance.AddWarrior("Guillaume", 150, 100);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    TestScene.Update();
    Settlements.Update();
    Barracks.Update();

    GameDirector& directorInstance = GameDirector::GetInstance();
    bool isMousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    Vector2 mousePos = GetMousePosition();
    Ray ray = GetScreenToWorldRay(mousePos, camera);
    for (GameObject* child : Settlements.GetChildren())
    {
        if (!child)
            continue;

        if (SettlementComponent* settlement = child->GetComponent<SettlementComponent>())
        {
            if (BoardComponent* board = settlement->GetComponent<BoardComponent>()) {
                if (board->GetShown() && board->GetCanBeClosed()) 
                {
                    if (isMousePressed && board->GetMouseOverCloseButton())
                    {
                        board->SetShown(false);
                        directorInstance.SetPickedModel(nullptr);
                    }
                }
            }
            if (ModelComponent* model = settlement->GetComponent<ModelComponent>())
            {
                if (model->GetPicked())
                {
                    model->SetTint(YELLOW);
                }

                Transform3DComponent* transform = settlement->GetComponent<Transform3DComponent>();
                Matrix matrix = transform->GetMatrix();

                RayCollision raycol = GetRayCollisionBox(ray, model->GetBoundingBox(&matrix));
                if (raycol.hit && isMousePressed && settlement->GetStatus() != SettlementStatus::Destroyed)
                {
                    if (directorInstance.GetWarrior(0)->GetStatus() == WarriorStatus::Waiting)
                    {
                        settlement->AddWarrior(directorInstance.GetWarrior(0)); // Must have a proper way to assign
                    }
                    directorInstance.SetPickedModel(model);
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

    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) 
    {
        finishScreen = 42; 
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
    Settlements.Render();
    Barracks.Render();
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