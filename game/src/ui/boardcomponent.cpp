
#include <game_object.h>
#include <ui/boardcomponent.h>
#include "ui/texturebutton.h"
#include "ui/texthelpers.h"
#include "gameplay/settlement.h"
#include <screens.h>

void BoardComponent::OnUpdate()
{
    if (!m_Shown)
        return;

    const float maxOffset = 5.0f;
    const float offset = 0.1f;

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

void BoardComponent::OnRender3D()
{

}

void BoardComponent::OnRender()
{
    if (!m_Shown)
        return;

    SettlementComponent* settlement = GetComponent<SettlementComponent>();

    if (settlement == nullptr)
        return;

    m_bMouseOverCloseButton = false;

    GameObject& gameObject = GetGameObject();
    Transform3DComponent* transform = gameObject.GetComponent<Transform3DComponent>();
    if (!transform)
        return;

    Vector2 pos = GetWorldToScreen(transform->GetPosition(), camera);

    if (IsTextureReady(m_BackgroundSprite)) {
        Vector2 spritePos = { pos.x - fabsf(SourceRect.width) / 2, GetScreenHeight() / 2 - fabsf(SourceRect.height) / 2 };
        pos.y = spritePos.y + 40;
        DrawTexture(m_BackgroundSprite, spritePos.x, spritePos.y - m_yOffset, WHITE);
    }

    if (m_bCanBeClosed && IsTextureReady(m_CloseButtonSprite)) {
        Vector2 spritePos = { pos.x + fabsf(SourceRect.width) / 2 - fabsf(m_CloseButtonSourceRect.width / BUTTON_STATE_COUNT) / 2, GetScreenHeight() / 2 - fabsf(SourceRect.height) / 2 };
        pos.y = spritePos.y + 40;

        float halfWidth = m_CloseButtonSourceRect.width / BUTTON_STATE_COUNT / 2;

        Vector2 origin = { spritePos.x, spritePos.y - halfWidth };
        if (TextureButton({ origin.x, origin.y, 64.f, 64.f }, m_CloseButtonSprite, false)) {
            m_bMouseOverCloseButton = true;
        }
    }

    Vector2 position;
        position.x = pos.x;
        position.y = pos.y + 50;

    DrawTextCentered(settlement->GetName(), position, 70, PAL_YELLOW);

    const int eventSize = 60;
    const int infoSize = 30;
    position.y += 80;
    if (Event* event = settlement->GetEvent())
    {
        DrawTextCentered(TextFormat("%s", event->GetName()), position, eventSize, PAL_DARK_RED);
        position.y += 60;
        DrawTextCentered(TextFormat("HP : %d/%d", settlement->GetHealth(), settlement->GetMaxHealth()), position, infoSize, WHITE);
        position.y += 60;
        DrawTextCentered(TextFormat("Inflicts : %d per turn", event->GetDamage()), position, infoSize, WHITE);
        position.y += 60;
        DrawTextCentered(TextFormat("Requires : %d power level", event->GetRequiredPower()), position, infoSize, WHITE);
    }
    else
    {
        if (settlement->GetStatus() == SettlementStatus::Destroyed) 
        {
            position.y += 60;
            DrawTextCentered("Destroyed", position, eventSize, PAL_NEAR_BLACK);
        }
        else 
        {
            DrawTextCentered("All Clear", position, eventSize, WHITE);
            position.y += 60;
            DrawTextCentered(TextFormat("HP : %d", settlement->GetHealth()), position, infoSize, WHITE);
        }
    }
}

BoardComponent* BoardComponent::SetSprite(const Texture2D& background, const Texture2D& closeButton)
{
    m_BackgroundSprite = background;
    SourceRect = Rectangle{ 0,0,float(background.width),float(background.height) };

    m_CloseButtonSprite = closeButton;
    m_CloseButtonSourceRect = Rectangle{ 0,0,float(closeButton.width),float(closeButton.height) };

    return this;
}