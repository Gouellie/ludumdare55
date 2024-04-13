/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

-- Copyright (c) 2020-2024 Jeffery Myers
--
--This software is provided "as-is", without any express or implied warranty. In no event
--will the authors be held liable for any damages arising from the use of this software.

--Permission is granted to anyone to use this software for any purpose, including commercial
--applications, and to alter it and redistribute it freely, subject to the following restrictions:

--  1. The origin of this software must not be misrepresented; you must not claim that you
--  wrote the original software. If you use this software in a product, an acknowledgment
--  in the product documentation would be appreciated but is not required.
--
--  2. Altered source versions must be plainly marked as such, and must not be misrepresented
--  as being the original software.
--
--  3. This notice may not be removed or altered from any source distribution.

*/

#include "raylib.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"                 // Required for GUI controls

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

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
GameScreen currentScreen = LOGO;
Font font = { 0 };
Music music = { 0 };
Sound fxCoin = { 0 };
Scene TestScene;
Scene Settlements;

static const int ScreenWidth = 1280;
static const int ScreenHeight = 800;

Texture Wabbit = { 0 };
Texture Logo = { 0 };
Texture Sprite = { 0 };

Model   Board = {0};
Model   Settlement = { 0 };

Camera camera = { 0 };

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

// a simple behavior class that handles input
class PlayerController : public GameObjectBehavior
{
public:
    DEFINE_BEHAVIOR(PlayerController)

        float Speed = 300;

    void OnUpdate() override
    {
        TransformComponent* transform = GetComponent<TransformComponent>();
        if (!transform)
            return;

        Vector2 movement = { 0 };
        if (IsKeyDown(KEY_W))
            movement.y -= 1;
        if (IsKeyDown(KEY_S))
            movement.y += 1;

        if (IsKeyDown(KEY_A))
            movement.x -= 1;
        if (IsKeyDown(KEY_D))
            movement.x += 1;

        transform->SetPosition(Vector2Add(transform->GetPosition(), Vector2Scale(movement, Speed * GetFrameTime())));
    }
};

class Spiner : public GameObjectBehavior
{
public:
    DEFINE_BEHAVIOR(Spiner)

    float Speed = -90;

    void OnUpdate() override
    {
        TransformComponent* transform = GetComponent<TransformComponent>();
        if (!transform)
            return;

        transform->SetRotation(transform->GetRotation() + Speed * GetFrameTime());
        Vector2 movement = { 0 };
    }
};

void SetupScene()
{
    auto* player = TestScene.AddObject();
    player->AddComponent<TransformComponent>()->SetPosition(Vector2{ 100, 100 });
    player->AddComponent<PlayerController>();
    auto* sprite = player->AddComponent<SpriteComponent>();
    sprite->SetSprite(Wabbit);
    sprite->SetScale(3);

    //auto* logo = TestScene.AddObject();
    //logo->AddComponent<TransformComponent>()->SetPosition(Vector2{ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f });
    // sprite = logo->AddComponent<SpriteComponent>();
    //sprite->SetSprite(Logo);
    //sprite->SetScale(0.5f);

    //auto* spinner = logo->AddChild();
    //spinner->AddComponent<TransformComponent>();
    //spinner->AddComponent<Spiner>();

    //auto* orbit = spinner->AddChild();
    //orbit->AddComponent<TransformComponent>()->SetPosition(Vector2{ 0, 200 });
    //sprite = orbit->AddComponent<SpriteComponent>();
    //sprite->SetSprite(Sprite);

    //auto* animator = orbit->AddComponent<SpriteAnimationComponent>();
    //auto& normalSequence = animator->AddSequence("normal");
    //normalSequence.FromSpriteSheet(Sprite, Sprite.width / 6, Sprite.height);
    //normalSequence.Loop = true;
    //normalSequence.FPS = 7;

    //auto& reverseSequence = animator->AddSequence("reverse");
    //reverseSequence.FromSpriteSheet(Sprite, Sprite.width / 6, Sprite.height);
    //reverseSequence.Loop = true;
    //reverseSequence.FPS = normalSequence.FPS;
    //reverseSequence.FlipFrames(true, false);

    //animator->SetCurrentSequence("normal");

    auto* board = TestScene.AddObject();
    board->AddComponent<ModelComponent>()->SetModel(Board);
    board->AddComponent<Transform3DComponent>()->SetPosition({0.0f, 0.0f, 0.0f});

    // Settlements
    auto* settlement_1 = Settlements.AddObject();
    settlement_1->AddComponent<SettlementComponent>();
    settlement_1->AddComponent<Transform3DComponent>()->SetPosition({ -6.0, 0.0, -3.0});
    settlement_1->AddComponent<ModelComponent>()->SetModel(Settlement);

    auto* settlement_2 = Settlements.AddObject();
    settlement_2->AddComponent<SettlementComponent>();
    settlement_2->AddComponent<Transform3DComponent>()->SetPosition({ 6.30, 0.0, 2.25 });
    settlement_2->AddComponent<ModelComponent>()->SetModel(Settlement);

    auto* settlement_3 = Settlements.AddObject();
    settlement_3->AddComponent<SettlementComponent>();
    settlement_3->AddComponent<Transform3DComponent>()->SetPosition({ -6.25, 0.0, 2.75 });
    settlement_3->AddComponent<ModelComponent>()->SetModel(Settlement);

    auto* settlement_4 = Settlements.AddObject();
    settlement_4->AddComponent<SettlementComponent>();
    settlement_4->AddComponent<Transform3DComponent>()->SetPosition({ 3.5f, 0.0, -2.5 });
    settlement_4->AddComponent<ModelComponent>()->SetModel(Settlement);
}

void LoadResources()
{
    Wabbit = LoadTexture("resources/wabbit_alpha.png");
    SetTextureFilter(Wabbit, TEXTURE_FILTER_POINT);
    Logo = LoadTexture("resources/raylib_logo.png");
    Sprite = LoadTexture("resources/scarfy.png");
    Board = LoadModel("resources/models/board.glb");
    Settlement = LoadModel("resources/models/settlement.glb");

    // Define the camera to look into our 3d world
    camera = { 0 };
    camera.position = { 15.0f, 10.0f, -15.0f };    // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
}

int main()
{
    // set up the window
    InitWindow(ScreenWidth, ScreenHeight, "Game Object Test");
    InitAudioDevice();      // Initialize audio device

    LoadResources();
    SetupScene();

    // Load global data (assets that must be available in all screens, i.e. font)
    //font = LoadFont("resources/mecha.png");
    //music = LoadMusicStream("resources/ambient.ogg");
    //fxCoin = LoadSound("resources/coin.wav");

    //SetMusicVolume(music, 1.0f);
    //PlayMusicStream(music);

// Setup and init first screen
#ifdef DEBUG
    currentScreen = GAMEPLAY;
    InitGameplayScreen();
#else
    currentScreen = LOGO;
    InitLogoScreen();
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
    // Update
    //----------------------------------------------------------------------------------
    //UpdateMusicStream(music);       // NOTE: Music keeps playing between screens
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

            if (FinishGameplayScreen() == 1) TransitionToScreen(ENDING);
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
    //----------------------------------------------------------------------------------
}
