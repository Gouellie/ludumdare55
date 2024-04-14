
#include "raylib.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"                 // Required for GUI controls

#define TEXTUREBUTTON_IMPLEMENTATION
#include "ui/texturebutton.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "game_object.h"
#include "simple_components.h"
#include "behavior.h"
#include "scene.h"

#include "screens.h" 

#include <gameplay/settlement.h>
#include <ui/boardcomponent.h>

#include <gameplay/gamedirector.h>
#include <gameplay/barrackcontroller.h>

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
GameScreen currentScreen = LOGO;
Font font = { 0 };
Sound fxCoin = { 0 };
Scene TestScene;
Scene Settlements;
Scene Barracks;

static const int ScreenWidth = 1280;
static const int ScreenHeight = 800;

Texture Wabbit = { 0 };
Texture Logo = { 0 };
Texture Sprite = { 0 };

Texture BoardBackground = { 0 };
Texture CloseButton = { 0 };
Texture SummonWarriorButton = { 0 };
Texture BarracksButton = { 0 };
Texture WarriorButton = { 0 };
Texture WarriorPanel = { 0 };

Model   Board = {0};
Model   Settlement = { 0 };

Camera camera = { 0 };
Font TextFont = { 0 };

// Local Variables
Music music = { 0 };

// Required variables to manage screen transitions (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static GameScreen transToScreen = UNKNOWN;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void ChangeToScreen(GameScreen screen);     // Change to screen, no transition effect
static void TransitionToScreen(GameScreen screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)
static void UpdateMainLoop(void);           // Update and draw one frame

void SetupScene()
{
    GameDirector& director = GameDirector::GetInstance();
    director.PopulateEventList();

    auto* board = TestScene.AddObject();
    board->AddComponent<ModelComponent>()->SetModel(Board);
    board->AddComponent<Transform3DComponent>()->SetPosition({0.0f, 0.0f, 0.0f});

    // Settlements
    auto* settlement_1 = Settlements.AddObject();
    settlement_1->AddComponent<SettlementComponent>();
    settlement_1->AddComponent<Transform3DComponent>()->SetPosition({ -6.0, 0.0, -3.0});
    settlement_1->AddComponent<ModelComponent>()->SetModel(Settlement);
    settlement_1->AddComponent<BoardComponent>()->SetSprite(BoardBackground, CloseButton);

    auto* settlement_2 = Settlements.AddObject();
    settlement_2->AddComponent<SettlementComponent>();
    settlement_2->AddComponent<Transform3DComponent>()->SetPosition({ 6.30, 0.0, 2.25 });
    settlement_2->AddComponent<ModelComponent>()->SetModel(Settlement);
    settlement_2->AddComponent<BoardComponent>()->SetSprite(BoardBackground, CloseButton);

    auto* settlement_3 = Settlements.AddObject();
    settlement_3->AddComponent<SettlementComponent>();
    settlement_3->AddComponent<Transform3DComponent>()->SetPosition({ -6.25, 0.0, 2.75 });
    settlement_3->AddComponent<ModelComponent>()->SetModel(Settlement);
    settlement_3->AddComponent<BoardComponent>()->SetSprite(BoardBackground, CloseButton);

    auto* settlement_4 = Settlements.AddObject();
    settlement_4->AddComponent<SettlementComponent>();
    settlement_4->AddComponent<Transform3DComponent>()->SetPosition({ 3.5f, 0.0, -2.5 });
    settlement_4->AddComponent<ModelComponent>()->SetModel(Settlement);
    settlement_4->AddComponent<BoardComponent>()->SetSprite(BoardBackground, CloseButton);

    // Barack
    Barracks.AddComponent<BarrackController>()->SetSprite(SummonWarriorButton, BarracksButton, WarriorButton, WarriorPanel);
}

void LoadResources()
{
    BoardBackground = LoadTexture("resources/board_background.png");
    CloseButton = LoadTexture("resources/ui/ui_close.png");
    SummonWarriorButton = LoadTexture("resources/ui/ui_summon_warrior.png");
    BarracksButton = LoadTexture("resources/ui/ui_barracks.png");
    WarriorButton = LoadTexture("resources/ui/ui_warrior.png");
    WarriorPanel = LoadTexture("resources/ui/warrior_panel.png");

    Board = LoadModel("resources/models/board.glb");
    Settlement = LoadModel("resources/models/settlement.glb");

    // Define the camera to look into our 3d world
    camera = { 0 };
    camera.position = { 25.0f, 20.f, 25.0f };    // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    TextFont = LoadFont("resources/fonts/custom_alagard.png");
    music = LoadMusicStream("resources/audio/ambient.ogg");
}

int main()
{
    // set up the window
    InitWindow(ScreenWidth, ScreenHeight, "Game Object Test");
    InitAudioDevice();      // Initialize audio device

    LoadResources();
    SetupScene();

// Setup and init first screen
#ifdef DEBUG
    currentScreen = GAMEPLAY;
    InitGameplayScreen();
#else
    currentScreen = LOGO;
    InitLogoScreen();
    SetMusicVolume(music, 1.0f);
    PlayMusicStream(music);
#endif // DEBUG

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateMainLoop, 60, 1);
#else
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        UpdateMainLoop();
    }
#endif
    // cleanup
    CloseWindow();
    return 0;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Change to next screen, no transition
static void ChangeToScreen(GameScreen screen)
{
    // Unload current screen
    switch (currentScreen)
    {
    case LOGO: UnloadLogoScreen(); break;
    case TITLE: UnloadTitleScreen(); break;
    case GAMEPLAY: UnloadGameplayScreen(); break;
    case ENDING: UnloadEndingScreen(); break;
    default: break;
    }

    // Init next screen
    switch (screen)
    {
    case LOGO: InitLogoScreen(); break;
    case TITLE: InitTitleScreen(); break;
    case GAMEPLAY: InitGameplayScreen(); break;
    case ENDING: InitEndingScreen(); break;
    default: break;
    }

    currentScreen = screen;
}

// Request transition to next screen
static void TransitionToScreen(GameScreen screen)
{
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
static void UpdateTransition(void)
{
    if (!transFadeOut)
    {
        transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (transAlpha > 1.01f)
        {
            transAlpha = 1.0f;

            // Unload current screen
            switch (transFromScreen)
            {
            case LOGO: UnloadLogoScreen(); break;
            case TITLE: UnloadTitleScreen(); break;
            case OPTIONS: UnloadOptionsScreen(); break;
            case GAMEPLAY: UnloadGameplayScreen(); break;
            case ENDING: UnloadEndingScreen(); break;
            default: break;
            }

            // Load next screen
            switch (transToScreen)
            {
            case LOGO: InitLogoScreen(); break;
            case TITLE: InitTitleScreen(); break;
            case GAMEPLAY: InitGameplayScreen(); break;
            case ENDING: InitEndingScreen(); break;
            default: break;
            }

            currentScreen = transToScreen;

            // Activate fade out effect to next loaded screen
            transFadeOut = true;
        }
    }
    else  // Transition fade out logic
    {
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f)
        {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = UNKNOWN;
        }
    }
}

// Draw transition effect (full-screen rectangle)
static void DrawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

static void UpdateMainLoop(void)
{
#ifndef DEBUG
    UpdateMusicStream(music);
#endif

    static bool cameraOrbit;

    if (!onTransition)
    {
        switch (currentScreen)
        {
        case LOGO:
        {
            UpdateLogoScreen();

            if (FinishLogoScreen()) TransitionToScreen(GAMEPLAY);

        } break;
        case TITLE:
        {
            UpdateTitleScreen();

            if (FinishTitleScreen() == 1) TransitionToScreen(OPTIONS);
            else if (FinishTitleScreen() == 2) TransitionToScreen(GAMEPLAY);

        } break;
        case OPTIONS:
        {
            UpdateOptionsScreen();

            if (FinishOptionsScreen()) TransitionToScreen(TITLE);

        } break;
        case GAMEPLAY:
        {
            if (cameraOrbit) {
                UpdateCamera(&camera, CAMERA_ORBITAL);
            }

            UpdateGameplayScreen();

            if (FinishGameplayScreen() == 42) TransitionToScreen(ENDING);
            //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);

        } break;
        case ENDING:
        {
            UpdateEndingScreen();

            if (FinishEndingScreen() == 1) TransitionToScreen(TITLE);

        } break;
        default: break;
        }
    }
    else UpdateTransition();    // Update transition (fade-in, fade-out)
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    switch (currentScreen)
    {
    case LOGO: DrawLogoScreen(); break;
    case TITLE: DrawTitleScreen(); break;
    case OPTIONS: DrawOptionsScreen(); break;
    case GAMEPLAY: DrawGameplayScreen(); break;
    case ENDING: DrawEndingScreen(); break;
    default: break;
    }

    // Draw full screen rectangle in front of everything
    if (onTransition) DrawTransition();

    //DrawFPS(10, 10);
    if (GuiButton({ 10, 10, 80, 20 }, "ORBIT")) 
    {
        cameraOrbit = !cameraOrbit;
    }

    EndDrawing();
}
