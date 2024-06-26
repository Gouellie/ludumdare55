
#ifndef SCREENS_H
#define SCREENS_H

#include <palette.h>

class Scene;

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { UNKNOWN = -1, LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING } GameScreen;

//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------

extern const int ScreenWidth;
extern const int ScreenHeight;

extern GameScreen currentScreen;
extern Font font;
extern Music music;
extern Sound fxCoin;
extern Camera camera;

extern Scene WorldScene;
extern Scene Settlements;
extern Scene Barracks;

extern Font TextFont;

extern Texture UIButton;

extern Sound SoundFXButton;
extern Sound SoundFXButtonClose;
extern Sound SoundFXButtonSettlement;
extern Sound SoundFXButtonNextTurn;
extern Sound SoundFXButtonSummon;
extern Sound SoundFXGameOverLose;

extern Sound SoundNone;

extern bool PickingHandled;

extern bool CameraOrbit;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Logo Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);

//----------------------------------------------------------------------------------
// Title Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
int FinishTitleScreen(void);

//----------------------------------------------------------------------------------
// Options Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitOptionsScreen(void);
void UpdateOptionsScreen(void);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
int FinishOptionsScreen(void);

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
int FinishGameplayScreen(void);

//----------------------------------------------------------------------------------
// Ending Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitEndingScreen(void);
void UpdateEndingScreen(void);
void DrawEndingScreen(void);
void UnloadEndingScreen(void);
int FinishEndingScreen(void);

void SetSoundsVolumes(float volume);

#ifdef __cplusplus
}
#endif

#endif // SCREENS_H