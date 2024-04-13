#include <raygui.h>
#include "gameplay/barrackcontroller.h"
#include "gameplay/gamedirector.h"

void BarrackController::OnRender()
{
    const float ScreenWidth = 1280;
    const float ScreenHeight = 800;

    const float buttonWidth = 40;
    float xOffset = buttonWidth + 10;
    if (m_bExpanded) {
        DrawRectangle(0, ScreenHeight - m_fHeight, ScreenWidth, m_fHeight, DARKGREEN);

        int warriorIndex = 0;
        GameDirector& gameDirector = GameDirector::GetInstance();
        for (Warrior& warrior : gameDirector.m_AvailableWarriors)
        {
            if (warriorIndex == gameDirector.GetPickedWarriorIndex()) {
                DrawRectangle(xOffset, ScreenHeight - m_fHeight, 40, m_fHeight, DARKBROWN);
            }
            else
            {
                if (GuiButton({ xOffset, ScreenHeight - m_fHeight, 40, m_fHeight }, "")) {
                    gameDirector.SetPickedWarriorIndex(warriorIndex);
                }
            }

            xOffset += 50;
            warriorIndex++;
        }
    }
    if (GuiButton({ 0, ScreenHeight - m_fHeight, buttonWidth, m_fHeight }, "->")) {
        m_bExpanded = !m_bExpanded;
    }
}
