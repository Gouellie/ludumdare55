
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
    framesCounter = 0;
    finishScreen = 0;

    if (GameDirector::GetInstance().GetGameState() == GameState::Fail) 
    {
        PlaySound(SoundFXGameOverLose);
    }
}

// Ending Screen Update logic
void UpdateEndingScreen(void)
{

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

    float realWidth = (float)UIButton.width / BUTTON_STATE_COUNT;
    Rectangle bounds = { GetScreenWidth() / 2.f - realWidth / 2.f, GetScreenHeight() - UIButton.height - 50.f, realWidth, (float)UIButton.height };
    if (TextureButtonWithText(bounds, UIButton, "Restart", 20)) {
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