
#include <game_object.h>
#include <ui/boardcomponent.h>
#include "ui/texturebutton.h"
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

    if (m_Message.length() == 0)
        return;

    //SettlementComponent* settlement = GetComponent<SettlementComponent>();

    //if (settlement == nullptr)
    //    return;

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
        position.x = pos.x - MeasureTextEx(TextFont, m_Message.c_str(), TextFont.baseSize, 2).x / 2.0f;
        position.y = pos.y;

    if (m_Header.length() > 0) {
        DrawTextEx(TextFont, m_Header.c_str(), position, TextFont.baseSize * 1.2f, 2, WHITE);
        position.y += 50;
    }

    SetTextLineSpacing(30);
    DrawTextEx(TextFont, m_Message.c_str(), position, TextFont.baseSize, 2, WHITE);
}

BoardComponent* BoardComponent::SetSprite(const Texture2D& background, const Texture2D& closeButton)
{
    m_BackgroundSprite = background;
    SourceRect = Rectangle{ 0,0,float(background.width),float(background.height) };

    m_CloseButtonSprite = closeButton;
    m_CloseButtonSourceRect = Rectangle{ 0,0,float(closeButton.width),float(closeButton.height) };

    return this;
}