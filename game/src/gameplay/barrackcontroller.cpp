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

void BarrackController::OnRenderUI()
{
    float ScreenWidth = (float)GetScreenWidth();
    float ScreenHeight = (float)GetScreenHeight();

    const float buttonWidth = 40;
    float xOffset = buttonWidth + 100;

    if (m_bExpanded) 
    {
        DrawRectangle(0, ScreenHeight - m_fHeight, ScreenWidth, m_fHeight, PAL_DARK_BROWN);

        if (IsTextureReady(m_WarriorSprite))
        {
            int warriorIndex = 0;
            GameDirector& gameDirector = GameDirector::GetInstance();
            SettlementComponent* pickedSettlement = gameDirector.GetPickedSettlement();

            for (Warrior& warrior : gameDirector.GetWarriors())
            {
                TextureButtonState state = TextureButtonState::STATE_NORMAL;
                if (warrior.GetStatus() == WarriorStatus::Dead) 
                {
                    state = TextureButtonState::STATE_DISABLED;
                }
                bool isWarriorAssignedToPickedSettlement = false;
                if (pickedSettlement) 
                {
                    std::vector<Warrior*> assignedWarriors;
                    pickedSettlement->GetWarriors(assignedWarriors);
                    for (Warrior* assignedWarrior : assignedWarriors)
                    {
                        if (warriorIndex == assignedWarrior->GetIndex()) 
                        {
                            isWarriorAssignedToPickedSettlement = true;
                            break;
                        }
                    }
                }

                Rectangle bounds = { xOffset, ScreenHeight - m_WarriorSprite.height, m_WarriorSprite.width / (float)BUTTON_STATE_COUNT, (float)m_WarriorSprite.height };
                bool isMouseOver = false;
                if (TextureButtonWithMouseOver(bounds, m_WarriorSprite, false, &isMouseOver, state))
                {

                    if (pickedSettlement) {
                        pickedSettlement->AddWarrior(gameDirector.GetWarrior(warriorIndex));
                    }
                }
                if (isWarriorAssignedToPickedSettlement) 
                {
                    DrawCircle(xOffset + m_WarriorSprite.width / (float)BUTTON_STATE_COUNT / 2.f, ScreenHeight - 12.f, 10.f, PAL_DARK_GREEN);
                }
                else if (warrior.GetStatus() == WarriorStatus::Dispatched)
                {
                    DrawCircle(xOffset + m_WarriorSprite.width / (float)BUTTON_STATE_COUNT / 2.f, ScreenHeight - 10.f, 6.f, PAL_GREY);
                }
                if (isMouseOver) 
                {
                    float height = ScreenHeight - m_WarriorPanelSprite.height;
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

                    Color statusColor = { 0 };
                    switch (warrior.GetStatus())
                    {
                    case WarriorStatus::Waiting:
                        status = "Waiting";
                        statusColor = WHITE;
                        break;
                    case WarriorStatus::Dispatched:
                        status = "Dispatched";
                        statusColor = PAL_CADET_BLUE;
                        break;
                    case WarriorStatus::Healing:
                        status = "Healing";
                        statusColor = PAL_DARK_GREEN;
                        break;
                    case WarriorStatus::Dead:
                        status = "Dead";
                        statusColor = PAL_RED;
                        break;
                    default:
                        status = "Unknown";
                        statusColor = PAL_RED;
                        break;
                    }

                    textY += 50;
                    float warriorStatusFontSize = 40.f;
                    textSize = MeasureTextEx(TextFont, status, warriorStatusFontSize, 2.f);
                    DrawTextPro(TextFont, status, {textX - textSize.x / 2.f, textY}, {0,0}, 0.f, warriorStatusFontSize, 2.f, statusColor);

                    textY += 70;
                    float warriorInfoFontSize = 20.f;
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
        DrawRectangle(0, ScreenHeight - m_fHeight, 116, m_fHeight, PAL_DARK_BROWN);
    }

    if (IsTextureReady(m_SummonWarriorSprite))
    {
        bool isMouseOver = false;
        float yOrigin = ScreenHeight - 40.f - (float)m_SummonWarriorSprite.height;

        TextureButtonState state = TextureButtonState::STATE_NORMAL;
        Warrior* war = GameDirector::GetInstance().GetWarrior(0);
        if (!GameDirector::GetInstance().CanBuy(*war))
        {
            state = TextureButtonState::STATE_DISABLED;
        }
        if (TextureButtonWithMouseOver({ 12.f, yOrigin, (float)m_SummonWarriorSprite.width / BUTTON_STATE_COUNT, (float)m_SummonWarriorSprite.height }, m_SummonWarriorSprite, false, &isMouseOver, state))
        {
            GameDirector::GetInstance().AddWarrior(Warrior("Ludum Dare", 150, 100));
            m_bExpanded = true;
        }
        if (isMouseOver)
        {
            const char* text = (state != TextureButtonState::STATE_DISABLED) ? TextFormat("SUMMON a Warrior! (%d$)", war->GetPrice()) : TextFormat("Not enough funds! (%d$)", war->GetPrice());

            DrawTextPro(TextFont, text, { 50.f, yOrigin + 6.f }, { 0.f }, 0.f, 30.f, 2.f, WHITE);
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
