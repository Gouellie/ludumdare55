
#include "raylib.h"
#include "raymath.h"

#define TEXTUREBUTTON_IMPLEMENTATION
#include "ui/texturebutton.h"

#define TEXT_HELPERS_IMPLEMENTATION
#include "ui/texthelpers.h"

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
Scene WorldScene;
Scene Settlements;
Scene Barracks;

const int ScreenWidth = 1280;
const int ScreenHeight = 800;
bool PickingHandled;

Texture Wabbit = { 0 };
Texture Logo = { 0 };
Texture Sprite = { 0 };

Texture BoardBackground = { 0 };
Texture CloseButton = { 0 };
Texture SummonWarriorButton = { 0 };
Texture BarracksButton = { 0 };
Texture WarriorButton = { 0 };
Texture WarriorPanel = { 0 };
Texture SettlementIcon = { 0 };

Texture UIButton = { 0 };

Model   Board = {0};
Model   Settlement = { 0 };

bool CameraOrbit = false;
Camera camera = { 0 };
Font TextFont = { 0 };

// Local Variables
Music music = { 0 };

Sound SoundFXButton = { 0 };
Sound SoundFXButtonClose = { 0 };
Sound SoundFXButtonSettlement = { 0 };
Sound SoundFXButtonNextTurn = { 0 };
Sound SoundFXButtonSummon = { 0 };
Sound SoundFXGameOverLose = { 0 };

Sound SoundNone = { 0 };

// Required variables to manage screen transitions (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static GameScreen transToScreen = UNKNOWN;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void ChangeToScreen(GameScreen screen);      // Change to screen, no transition effect
static void TransitionToScreen(GameScreen screen);  // Request transition to next screen
static void UpdateTransition(void);                 // Update transition effect
static void DrawTransition(void);                   // Draw transition effect (full-screen rectangle)
static void UpdateMainLoop(void);                   // Update and draw one frame

class NextTurnButtonComponent : public Component
{
private:
    Texture2D m_buttonSprite = { 0 };
public:
    DEFINE_COMPONENT(NextTurnButtonComponent)

    void SetSprite(const Texture2D& buttonSprite) { m_buttonSprite = buttonSprite; }

    void OnRender() override 
    {
        if (IsTextureReady(m_buttonSprite)) 
        {
            int currentTurn = GameDirector::GetInstance().GetCurrentTurn();

            Vector2 textPos = { GetScreenWidth() - 90.f, GetScreenHeight() - 120.f };
            DrawTextCentered(TextFormat("Turn : %d", currentTurn), textPos, 20, WHITE);

            float realWidth = (float)m_buttonSprite.width / BUTTON_STATE_COUNT;
            bool mouseOver;
            Rectangle bounds = { GetScreenWidth() - realWidth - 10.f , GetScreenHeight() - m_buttonSprite.height - 50.f, realWidth, (float)m_buttonSprite.height};
            TextureButtonSetFontColor(PAL_DARK_BLUE);
            TextureButtonSetSound(SoundFXButtonNextTurn);
            if (TextureButtonWithMouseOverAndText(bounds, m_buttonSprite, "Next Turn", 28, &mouseOver)) 
            {
                GameDirector::GetInstance().ResolveTurn(Settlements);
            }
            TextureButtonSetSound(SoundFXButton);
            TextureButtonSetFontColor(WHITE);
        }
    }
};

class SettlementIconComponent : public Component
{
    bool m_bDown = false;
    float m_yOffset = 0;
    Texture2D m_IconSprite = { 0 };
public:
    DEFINE_COMPONENT(SettlementIconComponent)

    void SetSprite(const Texture2D& iconSprite) { m_IconSprite = iconSprite; }

    void OnUpdate() override
    {
        const float maxOffset = 5.0f;
        const float offset = 0.1f;

        SettlementComponent* settlement = GetComponent<SettlementComponent>();
        if (settlement == nullptr)
            return;

        if (settlement->GetStatus() == SettlementStatus::Clear)
            return;

        if (m_bDown) {
            m_yOffset -= offset;
            if (m_yOffset < -maxOffset)
            {
                m_yOffset = -maxOffset;
                m_bDown = false;
            }
        }
        else
        {
            m_yOffset += offset;
            if (m_yOffset > maxOffset)
            {
                m_yOffset = maxOffset;
                m_bDown = true;
            }
        }
    }

    void OnRender() override
    {
        if (!IsTextureReady(m_IconSprite))
            return;

        SettlementComponent* settlement = GetComponent<SettlementComponent>();
        if (settlement == nullptr)
            return;

        Transform3DComponent* transform = GetComponent<Transform3DComponent>();
        if (transform == nullptr)
            return;

        Vector2 pos = GetWorldToScreen(transform->GetPosition(), camera);

        float realWidth = m_IconSprite.width / 3.0f; // the three possible states.

        pos.x -= realWidth / 2.f;
        pos.y -= (m_IconSprite.height + 128.f) + m_yOffset;

        SettlementStatus status = settlement->GetStatus();
        Rectangle source = { realWidth * (float)status, 0, realWidth, (float)m_IconSprite.height };
        DrawTextureRec(m_IconSprite, source, pos, WHITE);
    }
};

void CreateSettlement(const char* name, Vector3 pos, float rotation) 
{
    auto* settlement = Settlements.AddObject();
    settlement->AddComponent<Transform3DComponent>()->SetPosition(pos)->SetRotation(rotation);
    settlement->AddComponent<ModelComponent>()->SetModel(Settlement);
    settlement->AddComponent<BoardComponent>()->SetSprite(BoardBackground, CloseButton);
    settlement->AddComponent< SettlementIconComponent>()->SetSprite(SettlementIcon);
    auto* component = settlement->AddComponent<SettlementComponent>();
    component->SetName(name);
}

void SetupScene()
{
    GameDirector& director = GameDirector::GetInstance();
    director.PopulateEventList();

    auto* board = WorldScene.AddObject();
    board->AddComponent<ModelComponent>()->SetModel(Board);
    board->AddComponent<Transform3DComponent>()->SetPosition({0.0f, 0.0f, 0.0f});

    // Settlements
    CreateSettlement("Rencurelli",      { -1.82, 0.0, -0.79 }, 25.f);
    CreateSettlement("Gwellington",     { 13.79, 0.0, -9.21 }, 48.f);
    CreateSettlement("Emmisburg",       { 5.72, 0.0, 18.48 }, 24.f);
    CreateSettlement("ArkMaxter",       { -5.56, 0.0, -15.57 }, -19.f);
    CreateSettlement("Los Hazeles",     { 12.75, 0.0, 7.03 }, 56.f);
    CreateSettlement("Quebec",          { -13.9, 0.0, 10.0 }, 0.f);
    CreateSettlement("Just Bob",        { -14.89, 0.0, -4.87 }, 0.f);

    // Barack
    Barracks.AddComponent<BarrackController>()->SetSprite(SummonWarriorButton, BarracksButton, WarriorButton, WarriorPanel);
    auto* nextTurnButton = Barracks.AddObject();
    nextTurnButton->AddComponent<NextTurnButtonComponent>()->SetSprite(UIButton);
}

void SetupCamera()
{
    // Define the camera to look into our 3d world
    camera = { 0 };
    camera.position = { 29.0f, 15.f, 27.0f };    // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
}

void LoadResources()
{
    BoardBackground = LoadTexture("resources/ui/board_background.png");
    CloseButton = LoadTexture("resources/ui/ui_close.png");
    SummonWarriorButton = LoadTexture("resources/ui/ui_summon_warrior.png");
    BarracksButton = LoadTexture("resources/ui/ui_barracks.png");
    WarriorButton = LoadTexture("resources/ui/ui_warrior.png");
    WarriorPanel = LoadTexture("resources/ui/warrior_panel.png");
    UIButton = LoadTexture("resources/ui/ui_button.png");
    SettlementIcon = LoadTexture("resources/ui/settlement_icon.png");

    Board = LoadModel("resources/models/board.glb");
    Settlement = LoadModel("resources/models/settlement.glb");

    TextFont = LoadFont("resources/fonts/alagard.png");

    music = LoadMusicStream("resources/audio/ambient.ogg");

    SoundFXButton = LoadSound("resources/audio/click3.ogg");
    SoundFXButtonClose = LoadSound("resources/audio/click1.ogg");
    SoundFXButtonSettlement = LoadSound("resources/audio/click2.ogg");
    SoundFXButtonSummon = LoadSound("resources/audio/rollover2.ogg");
    SoundFXButtonNextTurn = LoadSound("resources/audio/rollover2.ogg");
    SoundFXGameOverLose = LoadSound("resources/audio/jingles_PIZZI07.ogg");
}

void UnloadResources() 
{
    UnloadTexture(BoardBackground);
    UnloadTexture(CloseButton);
    UnloadTexture(SummonWarriorButton);
    UnloadTexture(BarracksButton);
    UnloadTexture(WarriorButton);
    UnloadTexture(WarriorPanel);
    UnloadTexture(UIButton);
    UnloadTexture(SettlementIcon);

    UnloadModel(Board);
    UnloadModel(Settlement);

    UnloadFont(TextFont);

    UnloadMusicStream(music);

    UnloadSound(SoundFXButton);
    UnloadSound(SoundFXButtonClose);
    UnloadSound(SoundFXButtonSettlement);
    UnloadSound(SoundFXButtonSummon);
    UnloadSound(SoundFXButtonNextTurn);
    UnloadSound(SoundFXGameOverLose);
}

int main()
{
    // set up the window
    InitWindow(ScreenWidth, ScreenHeight, "Game Object Test");
    InitAudioDevice();      // Initialize audio device

    SetupCamera();
    LoadResources();
    SetupScene();

    TextHelpersSetFont(TextFont);
    TextureButtonSetFont(TextFont);
    TextureButtonSetSound(SoundFXButton);

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
    UnloadResources();
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
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(PAL_NEAR_BLACK, transAlpha));
}

static void UpdateMainLoop(void)
{
#ifndef DEBUG
    UpdateMusicStream(music);
#endif

    if (!onTransition)
    {
        switch (currentScreen)
        {
        case LOGO:
        {
            UpdateLogoScreen();

            if (FinishLogoScreen()) TransitionToScreen(TITLE);

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
            if (CameraOrbit) {
                UpdateCamera(&camera, CAMERA_ORBITAL);
                TraceLog(LOG_INFO, TextFormat("x%.2f:, y:%.2f", camera.position.x, camera.position.z));
            }

            UpdateGameplayScreen();

            if (FinishGameplayScreen() == 42) TransitionToScreen(ENDING);

        } break;
        case ENDING:
        {
            UpdateEndingScreen();

            if (FinishEndingScreen() == 1) TransitionToScreen(GAMEPLAY);

        } break;
        default: break;
        }
    }
    else UpdateTransition();    // Update transition (fade-in, fade-out)
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(PAL_CADET_BLUE);

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

    EndDrawing();
}
