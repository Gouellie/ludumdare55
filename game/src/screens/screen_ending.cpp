
#include "raylib.h"
#include "screens.h"
#include "scene.h"

#include "gameplay/gamedirector.h"
#include "ui/texthelpers.h"
#include "ui/texturebutton.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Ending Screen Functions Definition
//----------------------------------------------------------------------------------

// Ending Screen Initialization logic
void InitEndingScreen(void)
{
    // TODO: Initialize ENDING screen variables here!
    framesCounter = 0;
    finishScreen = 0;
}

// Ending Screen Update logic
void UpdateEndingScreen(void)
{
    //// Press enter or tap to return to TITLE screen
    //if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    //{
    //    finishScreen = 1;
    //    PlaySound(fxCoin);
    //}
}

// Ending Screen Draw logic
void DrawEndingScreen(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PAL_HOSPITAL_BLUE);

    GameState state = GameDirector::GetInstance().GetGameState();

    const char* message;
    switch (state)
    {
    case GameState::Running:
        return;
    case GameState::Victory:
        message = "YOU WIN!";
        break;
    case GameState::Fail:
        message = "GAME OVER";
        break;
    default:
        return;
    }

    DrawTextCentered(message, { GetScreenWidth() / 2.f, 200.f }, 200, PAL_HOSPITAL_YELLOW);

    DrawTextCentered("Thank you For Playing!", { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f }, 100, PAL_HOSPITAL_YELLOW);

    bool isMouseOver = false;
    float realWidth = (float)UIButton.width / BUTTON_STATE_COUNT;
    Rectangle bounds = { GetScreenWidth() / 2.f - realWidth / 2.f, GetScreenHeight() - UIButton.height - 50.f, realWidth, (float)UIButton.height };
    if (TextureButtonWithMouseOverAndText(bounds, UIButton, "Restart", 20, &isMouseOver)) {
        finishScreen = 1;
    }
}

// Ending Screen Unload logic
void UnloadEndingScreen(void)
{
    // TODO: Unload ENDING screen variables here!
}

// Ending Screen should finish?
int FinishEndingScreen(void)
{
    return finishScreen;
}