#pragma once

#include <simple_components.h>

class BoardComponent : public Component
{
private:
    std::string m_Header;
    std::string m_Message;
    bool m_Shown = false;

    Texture2D m_BackgroundSprite = { 0 };
    Rectangle SourceRect = { 0,0,-1,-1 };

    Texture2D m_CloseButtonSprite = { 0 };
    Rectangle m_CloseButtonSourceRect = { 0,0,-1,-1 };

    float m_yOffset = 0.f;
    bool m_bDown = false;
    bool m_bCanBeClosed = false;
    bool m_bMouseOverCloseButton = false;
    
public:
    DEFINE_COMPONENT(BoardComponent)

    void OnUpdate() override;
    void OnRender3D() override;
    void OnRender() override;

    void SetShown(bool value, bool canBeClosed = false) { m_Shown = value; m_bCanBeClosed = canBeClosed; }
    bool GetShown() { return m_Shown; }
    bool GetCanBeClosed() { return m_bCanBeClosed; }

    bool GetMouseOverCloseButton() { return m_bMouseOverCloseButton; }

    BoardComponent* SetHeader(const char* header) { m_Header = header; return this; }
    std::string GetHeader() { return m_Header; };
    BoardComponent* SetMessage(const char* message) { m_Message = message; return this; }
    std::string GetMessage() { return m_Message; };

    BoardComponent* SetSprite(const Texture2D& background, const Texture2D& closeButton);
};