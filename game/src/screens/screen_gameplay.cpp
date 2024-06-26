#include "raylib.h"
#include "raymath.h"
#include "screens.h"
#include "scene.h"
#include "simple_components.h"
#include "game_object.h"

#include <gameplay/gamedirector.h>
#include <gameplay/settlement.h>
#include <ui/boardcomponent.h>
#include <ui/texturebutton.h>

#include <cstdlib>


//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static bool isSoundMuted = false;
static bool isMusicMuted = false;
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
    directorInstance.AddStartingWarriors();
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    WorldScene.Update();
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
                    if (board->GetMouseOverCloseButton())
                    {
                        board->SetShown(false);
                        directorInstance.SetPickedModel(nullptr);
                    }
                }
            }
            if (ModelComponent* model = settlement->GetComponent<ModelComponent>())
            {
                Transform3DComponent* transform = settlement->GetComponent<Transform3DComponent>();
                Matrix matrix = transform->GetMatrix();

                RayCollision raycol = GetRayCollisionBox(ray, model->GetBoundingBox(&matrix));
                if (raycol.hit) 
                {
                    model->SetTint(LIGHTGRAY);
                    if (!PickingHandled && isMousePressed)
                    {
                        directorInstance.SetPickedModel(model);
                        PlaySound(SoundFXButtonSettlement);
                    }
                }
                else 
                {
                    model->SetTint(WHITE);
                }
            }
        }
    }

    if (directorInstance.GetGameOver()) 
    {
        // Check for GameState to go to a different screen
        finishScreen = 42; 
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    BeginMode3D(camera);
    WorldScene.Render3D();
    Settlements.Render3D();
    EndMode3D();

    WorldScene.Render();
    Settlements.Render();
    Barracks.Render();

    Settlements.RenderUI();
    Barracks.RenderUI();

    float uiButtonWitdh = (float)(UIButton.width / BUTTON_STATE_COUNT);
    Rectangle bounds = { (float)ScreenWidth - uiButtonWitdh - 10.f, 10.f, uiButtonWitdh, (float)UIButton.height };
    if (TextureButtonWithText(bounds, UIButton, TextFormat("Music %s", isMusicMuted ? "OFF" : "ON"), 20.f))
    {
        isMusicMuted = !isMusicMuted;
        SetMusicVolume(music, isMusicMuted ? 0.0f : 1.0f);
    }
    bounds.x -= uiButtonWitdh + 10.f;
    if (isSoundMuted) {
        // There's a glitch when playing a sound while unmuting.
        TextureButtonSetSound(SoundNone);
    }
    if (TextureButtonWithText(bounds, UIButton, TextFormat("Audio %s", isSoundMuted ? "OFF" : "ON"), 20.f))
    {
        isSoundMuted = !isSoundMuted;
        SetSoundsVolumes(isSoundMuted ? 0.0f : 1.0f);
    }
    TextureButtonSetSound(SoundFXButton);

    bounds = { 10.f, 10.f, uiButtonWitdh, (float)UIButton.height };
    if (TextureButtonWithText(bounds, UIButton, "Orbit", 20.f))
    {
        CameraOrbit = !CameraOrbit;
    }
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    GameDirector::GetInstance().ResetDirector();
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}

void SetSoundsVolumes(float volume) 
{
    SetSoundVolume(SoundFXButton, volume);
    SetSoundVolume(SoundFXButtonClose, volume);
    SetSoundVolume(SoundFXButtonSettlement, volume);
    SetSoundVolume(SoundFXButtonNextTurn, volume);
    SetSoundVolume(SoundFXButtonSummon, volume);
    SetSoundVolume(SoundFXGameOverLose, volume);
}
