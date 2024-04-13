#pragma once

#include <simple_components.h>

class BoardComponent : public Component
{
private:
    std::string m_Header;
    std::string m_Message;
    bool m_Shown = false;

    Texture2D Sprite = { 0 };
    Rectangle SourceRect = { 0,0,-1,-1 };

    float m_yOffset = 0.f;
    bool m_bDown = false;

public:
    DEFINE_COMPONENT(BoardComponent)

    void OnUpdate() override;
    void OnRender3D() override;
    void OnRender() override;

    void SetShown(bool value) { m_Shown = value; }
    bool GetShown() { return m_Shown; }

    BoardComponent* SetHeader(const char* header) { m_Header = header; return this; }
    std::string GetHeader() { return m_Header; };
    BoardComponent* SetMessage(const char* message) { m_Message = message; return this; }
    std::string GetMessage() { return m_Message; };

    BoardComponent* SetSprite(const Texture2D& texture);
    void SetSprite(const Texture2D& texture, const Rectangle& sourceRect);
    void SetSpriteRect(const Rectangle& sourceRect);
};