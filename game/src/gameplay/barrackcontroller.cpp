#include <raygui.h>
#include "gameplay/barrackcontroller.h"
#include "gameplay/gamedirector.h"
#include "gameplay/settlement.h"

#include "screens.h"
#include "ui/texturebutton.h"

void BarrackController::SetSprite(const Texture2D& summonWarriorSprite, const Texture2D& barracksSprite, const Texture2D& warriorSprite, const Texture2D& warriorPanelSprite)
{
    m_SummonWarriorSprite = summonWarriorSprite;
    m_BarracksSprite = barracksSprite;
    m_WarriorSprite = warriorSprite;
    m_WarriorPanelSprite = warriorPanelSprite;
}

void BarrackController::OnRender()
{
    int ScreenWidth = GetScreenWidth();
    int ScreenHeight = GetScreenHeight();

    const float buttonWidth = 40;
    float xOffset = buttonWidth + 100;

    if (m_bExpanded) 
    {
        DrawRectangle(0, ScreenHeight - m_fHeight, ScreenWidth, m_fHeight, DARKBROWN);

        if (IsTextureReady(m_WarriorSprite))
        {
            int warriorIndex = 0;
            GameDirector& gameDirector = GameDirector::GetInstance();
            for (Warrior& warrior : gameDirector.GetWarriors())
            {
                TextureButtonState state = TextureButtonState::STATE_NORMAL;
                if (warrior.GetStatus() == WarriorStatus::Dead) 
                {
                    state = TextureButtonState::STATE_DISABLED;
                }

                Rectangle bounds = { xOffset, ScreenHeight - m_fHeight, 40, m_fHeight };
                bool isMouseOver = false;
                if (TextureButtonWithMouseOver(bounds, m_WarriorSprite, false, &isMouseOver, state))
                {
                    SettlementComponent* pickedSettlement = gameDirector.GetPickedSettlement();
                    if (pickedSettlement) {
                        pickedSettlement->AddWarrior(gameDirector.GetWarrior(warriorIndex));
                    }
                }
                if (isMouseOver) 
                {
                    float height = GetScreenHeight() - m_WarriorPanelSprite.height;
                    float width = m_WarriorPanelSprite.width;

                    float panelX = xOffset - width / 2 + 20;
                    DrawTexture(m_WarriorPanelSprite, panelX, height - 64, WHITE);

                    float textX = panelX + width / 2;
                    float textY = height - 32.f;

                    float warriorNameFontSize = 30.f;
                    Vector2 textSize;
                    textSize = MeasureTextEx(TextFont, warrior.GetName(), warriorNameFontSize, 2.f);
                    DrawTextPro(TextFont, warrior.GetName(), {textX - textSize.x / 2.f, textY}, {0,0}, 0.f, warriorNameFontSize, 2.f, WHITE);

                    const char* status;

                    switch (warrior.GetStatus())
                    {
                    case WarriorStatus::Waiting:
                        status = "Waiting";
                        break;
                    case WarriorStatus::Dispatched:
                        status = "Dispatched";
                        break;
                    case WarriorStatus::Healing:
                        status = "Healing";
                        break;
                    case WarriorStatus::Dead:
                        status = "Dead";
                        break;
                    default:
                        status = "Unknown";
                        break;
                    }

                    textY += 50;
                    float warriorInfoFontSize = 20.f;
                    textSize = MeasureTextEx(TextFont, status, warriorInfoFontSize, 2.f);
                    DrawTextPro(TextFont, status, {textX - textSize.x / 2.f, textY}, {0,0}, 0.f, warriorInfoFontSize, 2.f, WHITE);

                    textY += 30;

                    const char* healthText = TextFormat("Health: %d/%d", warrior.GetHealth(), warrior.GetMaxHealth());
                    textSize = MeasureTextEx(TextFont, healthText, warriorInfoFontSize, 2.f);
                    DrawTextPro(TextFont, healthText, { textX - textSize.x / 2.f, textY }, { 0,0 }, 0.f, warriorInfoFontSize, 2.f, WHITE);

                    textY += 30;
                    const char* powerText = TextFormat("Power Level: %d", warrior.GetPowerLevel());
                    textSize = MeasureTextEx(TextFont, powerText, warriorInfoFontSize, 2.f);
                    DrawTextPro(TextFont, powerText, { textX - textSize.x / 2.f, textY }, { 0,0 }, 0.f, warriorInfoFontSize, 2.f, WHITE);
                }

                xOffset += 50;
                warriorIndex++;
            }
        }
    }
    else 
    {
        DrawRectangle(0, ScreenHeight - m_fHeight, 100, m_fHeight, DARKBROWN);
    }

    if (IsTextureReady(m_SummonWarriorSprite))
    {
        bool isMouseOver = false;
        float yOrigin = ScreenHeight - 48.f - (float)m_SummonWarriorSprite.height;

        TextureButtonState state = TextureButtonState::STATE_NORMAL;
        if (GameDirector::GetInstance().GetCash() == 0)
        {
            state = TextureButtonState::STATE_DISABLED;
        }
        if (TextureButtonWithMouseOver({ 8.f, yOrigin, (float)m_SummonWarriorSprite.width / BUTTON_STATE_COUNT, (float)m_SummonWarriorSprite.height }, m_SummonWarriorSprite, false, &isMouseOver, state))
        {
            GameDirector::GetInstance().AddWarrior(Warrior("Ludum Dare", 150, 100));
            m_bExpanded = true;
        }
        if (state != TextureButtonState::STATE_DISABLED && isMouseOver)
        {
            DrawTextPro(TextFont, "SUMMON A WARRIOR!", { 50.f, yOrigin + 3.f }, { 0.f }, 0.f, 30.f, 2.f, WHITE);
        }
    }

    if (IsTextureReady(m_BarracksSprite))
    {
        Rectangle bounds = { 90, ScreenHeight - m_fHeight, buttonWidth, m_fHeight };
        if (TextureButton(bounds, m_BarracksSprite, m_bExpanded)) 
        {
            m_bExpanded = !m_bExpanded;
        }

        DrawTextPro(TextFont, TextFormat("%d$", GameDirector::GetInstance().GetCash()), {8.f, ScreenHeight - 32.f}, {0.f}, 0.f, 30.f, 2.f, WHITE);
    }
}
