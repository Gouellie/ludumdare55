
#include "raylib.h"
#include "screens.h"
#include "scene.h"
#include "ui/texthelpers.h"
#include "ui/texturebutton.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

// Title Screen Initialization logic
void InitTitleScreen(void)
{
    // TODO: Initialize TITLE screen variables here!
    framesCounter = 0;
    finishScreen = 0;
}

// Title Screen Update logic
void UpdateTitleScreen(void)
{

}

// Title Screen Draw logic
void DrawTitleScreen(void)
{
    // TODO: Draw TITLE screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PAL_YELLOW);
    DrawTextCentered("Otherworldly Mercenary", { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f }, 100, PAL_HOSPITAL_YELLOW);

    float realWidth = (float)UIButton.width / BUTTON_STATE_COUNT;
    Rectangle bounds = { GetScreenWidth() / 2.f - realWidth / 2.f, GetScreenHeight() - UIButton.height - 128.f, realWidth, (float)UIButton.height };
    if (TextureButtonWithText(bounds, UIButton, "Start Game", 20)) {
        finishScreen = 2;
    }
}

// Title Screen Unload logic
void UnloadTitleScreen(void)
{
    // TODO: Unload TITLE screen variables here!
}

// Title Screen should finish?
int FinishTitleScreen(void)
{
    return finishScreen;
}