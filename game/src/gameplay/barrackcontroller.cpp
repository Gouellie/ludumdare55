#include <raygui.h>
#include "gameplay/barrackcontroller.h"
#include "gameplay/gamedirector.h"

#include "screens.h"
#include "ui/texturebutton.h"

void BarrackController::SetSprite(const Texture2D& barracksSprite, const Texture2D& warriorSprite)
{
    m_BarracksSprite = barracksSprite;
    m_WarriorSprite = warriorSprite;
}

void BarrackController::OnRender()
{
    int ScreenWidth = GetScreenWidth();
    int ScreenHeight = GetScreenHeight();

    const float buttonWidth = 40;
    float xOffset = buttonWidth + 10;
    if (m_bExpanded) {
        DrawRectangle(0, ScreenHeight - m_fHeight, ScreenWidth, m_fHeight, DARKGREEN);

        if (IsTextureReady(m_WarriorSprite))
        {
            int warriorIndex = 0;
            GameDirector& gameDirector = GameDirector::GetInstance();
            for (Warrior& warrior : gameDirector.m_AvailableWarriors)
            {
                Color color;
                switch (warrior.GetStatus()) {
                case WarriorStatus::Waiting:
                    color = WHITE;
                    break;
                case WarriorStatus::Dispatched:
                    color = BLUE;
                    break;
                case WarriorStatus::Healing:
                    color = GREEN;
                    break;
                case WarriorStatus::Dead:
                    color = RED;
                    break;
                }

                TextureButtonState state = TextureButtonState::STATE_NORMAL;
                if (warriorIndex == gameDirector.GetPickedWarriorIndex()) {
                    state = TextureButtonState::STATE_SELECTED;
                }

                Rectangle bounds = { xOffset, ScreenHeight - m_fHeight, 40, m_fHeight };
                if (TextureButton(bounds, m_WarriorSprite, false, state))
                {
                    gameDirector.SetPickedWarriorIndex(warriorIndex);
                }

                xOffset += 50;
                warriorIndex++;
            }
        }
    }

    if (IsTextureReady(m_BarracksSprite))
    {
        Rectangle bounds = { 0, ScreenHeight - m_fHeight, buttonWidth, m_fHeight };
        if (TextureButton(bounds, m_BarracksSprite, m_bExpanded)) {
            m_bExpanded = !m_bExpanded;
        }
    }
}
